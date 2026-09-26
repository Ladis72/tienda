/**
 * @file estadisticas.cpp
 * @brief Implementación del diálogo de estadísticas avanzadas.
 *
 * Optimizado para consultar la infraestructura central en la nube (SyncManager)
 * o bases de datos locales, implementando carga bajo demanda (lazy-loading)
 * de pestañas y soporte para vista consolidada global de toda la cadena.
 * Utiliza QStandardItemModel en memoria para desacoplar la interfaz gráfica
 * de los cursores de base de datos y evitar colisiones de concurrencia.
 */

#include "estadisticas.h"
#include "configuracion.h"
#include "syncmanager.h"
#include "ui_estadisticas.h"
#include <QApplication>
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QStandardItem>

extern Configuracion *conf;

Estadisticas::Estadisticas(QWidget *parent)
    : QDialog(parent), ui(new Ui::Estadisticas) {
  ui->setupUi(this);

  // Modo normal por defecto (solo tabla tickets)
  modoConsolidado = false;

  // Configurar fechas por defecto: mes actual hasta la fecha de hoy ("yyyy-MM-dd")
  QDate fechaActual = QDate::currentDate();
  ui->dateDesde->setDate(QDate(fechaActual.year(), fechaActual.month(), 1));
  ui->dateHasta->setDate(fechaActual);

  // Inicializar el gráfico de ventas vacío en su contenedor
  graficoVentas = new GraficoVentasWidget(this);
  ui->layoutGrafico->addWidget(graficoVentas);

  // Inicializar punteros de modelos a nullptr para gestión segura de memoria
  modeloTopVendidos = nullptr;
  modeloTopRentables = nullptr;
  modeloMejoresClientes = nullptr;
  modeloVendedores = nullptr;
  modeloFormaPago = nullptr;
  modeloFamilias = nullptr;

  // Gestor de conexiones remotas
  conexiones = new conexionesRemotas(this);
  conexiones->base = &base;

  // Conectar cambio de pestaña para carga bajo demanda (lazy loading)
  connect(ui->tabWidgetEstadisticas, &QTabWidget::currentChanged, this,
          &Estadisticas::on_tabWidgetEstadisticas_currentChanged);

  // Cargar combo de tiendas bloqueando señales para evitar ejecuciones prematuras
  ui->comboTienda->blockSignals(true);
  cargarTiendas();
  ui->comboTienda->blockSignals(false);
}

Estadisticas::~Estadisticas() {
  // Liberar modelos asignados a las tablas
  delete modeloTopVendidos;
  delete modeloTopRentables;
  delete modeloMejoresClientes;
  delete modeloVendedores;
  delete modeloFormaPago;
  delete modeloFamilias;
  delete ui;
}

/**
 * @brief Se dispara automáticamente cuando el diálogo se va a mostrar.
 *
 * Refresca la lista de tiendas y carga los datos según el ámbito seleccionado.
 */
void Estadisticas::showEvent(QShowEvent *event) {
  ui->comboTienda->blockSignals(true);
  cargarTiendas();
  ui->comboTienda->blockSignals(false);

  // Actualizar estadísticas con el ámbito seleccionado al abrir
  cargarEstadisticas();

  // Propagar el evento al padre
  QDialog::showEvent(event);
}

/**
 * @brief Captura la tecla F2 para alternar entre modo normal y consolidado.
 *
 * En modo local/remoto directo conmuta entre tickets y tickets+ticketss.
 */
void Estadisticas::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F2) {
    modoConsolidado = !modoConsolidado;
    cargarEstadisticas();
  } else {
    QDialog::keyPressEvent(event);
  }
}

/**
 * @brief Llena el combo de tiendas priorizando la Nube centralizada.
 *
 * Si la Nube está disponible:
 *  - 1º: Opción global para toda la cadena.
 *  - 2º: Tienda local directa.
 *  - 3º: Tiendas remotas individuales vía Nube (máxima velocidad).
 * Si la Nube no está disponible, añade las conexiones abiertas existentes como fallback.
 */
void Estadisticas::cargarTiendas() {
  ui->comboTienda->clear();

  bool nubeDisponible = QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
                        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen();

  // 1. Si la Nube está activa, añadir opción global de toda la cadena
  if (nubeDisponible) {
    QVariantMap dataGlobal;
    dataGlobal["tipo"] = static_cast<int>(TipoOrigenEstadisticas::NubeGlobal);
    dataGlobal["idTienda"] = -1;
    dataGlobal["nombre"] = "Todas las tiendas (Global)";
    ui->comboTienda->addItem("🌐 Todas las tiendas (Global)", dataGlobal);
  }

  // 2. Tienda local
  QString local = conf->getConexionLocal();
  int idLocal = conf->getIdTienda();
  QString nombreLocal = "Tienda Local";
  if (QSqlDatabase::database(local).isOpen()) {
    QSqlQuery qNom(QSqlDatabase::database(local));
    if (qNom.exec("SELECT nombre FROM tiendas WHERE local = 1 LIMIT 1") && qNom.next()) {
      nombreLocal = qNom.value(0).toString();
    }

    QVariantMap dataLocal;
    dataLocal["tipo"] = static_cast<int>(TipoOrigenEstadisticas::Local);
    dataLocal["idTienda"] = idLocal;
    dataLocal["conn"] = local;
    dataLocal["nombre"] = nombreLocal;
    ui->comboTienda->addItem("🏠 " + nombreLocal + " (Local)", dataLocal);
  }

  // 3. Tiendas remotas individuales
  if (nubeDisponible) {
    // Consultar lista de tiendas en la nube
    QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    QSqlQuery qTiendas(dbNube);
    if (qTiendas.exec(QString("SELECT id, nombre FROM tiendas WHERE id != %1 ORDER BY id").arg(idLocal))) {
      while (qTiendas.next()) {
        int idT = qTiendas.value(0).toInt();
        QString nomT = qTiendas.value(1).toString();
        QVariantMap dataRemota;
        dataRemota["tipo"] = static_cast<int>(TipoOrigenEstadisticas::NubeTienda);
        dataRemota["idTienda"] = idT;
        dataRemota["nombre"] = nomT;
        ui->comboTienda->addItem("☁️ " + nomT + " [Nube]", dataRemota);
      }
    }
  } else {
    // Fallback cuando no hay nube: conexiones directas activas
    QStringList activas = conf->getNombreConexionesActivas();
    for (const QString &tienda : activas) {
      if (tienda != local && QSqlDatabase::database(tienda).isOpen()) {
        QVariantMap dataRemota;
        dataRemota["tipo"] = static_cast<int>(TipoOrigenEstadisticas::RemotoDirecto);
        dataRemota["conn"] = tienda;
        dataRemota["nombre"] = tienda;
        dataRemota["idTienda"] = 0;
        ui->comboTienda->addItem("🔗 " + tienda + " [Remoto]", dataRemota);
      }
    }
  }
}

/**
 * @brief Obtiene la información estructurada del ámbito/tienda seleccionado.
 */
InfoOrigenEstadisticas Estadisticas::obtenerOrigenActual() {
  InfoOrigenEstadisticas info;
  info.tipo = TipoOrigenEstadisticas::Local;
  info.idTienda = conf ? conf->getIdTienda() : 1;
  info.nombreConexion = conf ? conf->getConexionLocal() : "DB";
  info.nombreTienda = "Tienda Local";

  if (ui->comboTienda->count() == 0) {
    return info;
  }

  QVariant dataVar = ui->comboTienda->currentData();
  if (dataVar.canConvert<QVariantMap>()) {
    QVariantMap map = dataVar.toMap();
    info.tipo = static_cast<TipoOrigenEstadisticas>(map.value("tipo").toInt());
    info.idTienda = map.value("idTienda", -1).toInt();
    info.nombreConexion = map.value("conn", conf->getConexionLocal()).toString();
    info.nombreTienda = map.value("nombre", ui->comboTienda->currentText()).toString();
  }

  return info;
}

/**
 * @brief Actualiza los indicadores visuales en la cabecera del diálogo.
 */
void Estadisticas::actualizarEstadoUI() {
  InfoOrigenEstadisticas origen = obtenerOrigenActual();

  QString textoOrigen;
  switch (origen.tipo) {
  case TipoOrigenEstadisticas::NubeGlobal:
    textoOrigen = "Fuente: 🌐 Nube Central (Consolidado Global - Toda la cadena)";
    break;
  case TipoOrigenEstadisticas::NubeTienda:
    textoOrigen = QString("Fuente: ☁️ Nube Central (%1)").arg(origen.nombreTienda);
    break;
  case TipoOrigenEstadisticas::Local:
    textoOrigen = QString("Fuente: 🏠 Base de Datos Local (%1)").arg(origen.nombreTienda);
    break;
  case TipoOrigenEstadisticas::RemotoDirecto:
    textoOrigen = QString("Fuente: 🔗 Conexión Remota Directa (%1)").arg(origen.nombreTienda);
    break;
  }

  ui->lblInfoOrigen->setText(textoOrigen);

  if (modoConsolidado) {
    ui->lblInfoConsolidado->setText("[F2] Modo: CONSOLIDADO (tickets + ticketss)");
    ui->lblInfoConsolidado->setStyleSheet("color: #C62828; font-weight: bold;");
  } else {
    ui->lblInfoConsolidado->setText("[F2] Modo: Normal (Solo tickets)");
    ui->lblInfoConsolidado->setStyleSheet("color: #555555; font-style: italic;");
  }
}

void Estadisticas::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void Estadisticas::on_comboTienda_currentIndexChanged(int /*index*/) {
  cargarEstadisticas();
}

/**
 * @brief Se activa cuando el usuario cambia de pestaña en el QTabWidget.
 * Si la pestaña seleccionada está marcada como pendiente, se carga en ese instante.
 */
void Estadisticas::on_tabWidgetEstadisticas_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    cargarPestana(index);
    pestanasPendientes[index] = false;
    QApplication::restoreOverrideCursor();
  }
}

/**
 * @brief Carga las estadísticas principales (KPIs) y la pestaña activa actual.
 *
 * Implementa lazy-loading: marca las demás pestañas como pendientes de actualización
 * para optimizar la respuesta y evitar consultas SQL masivas innecesarias.
 */
void Estadisticas::cargarEstadisticas() {
  InfoOrigenEstadisticas origen = obtenerOrigenActual();
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  // Validación de disponibilidad de la conexión
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    QSqlDatabase dbNube = base.obtenerConexionNube();
    if (!dbNube.isValid() || !dbNube.isOpen()) {
      QMessageBox::warning(this, "Aviso Nube",
                           "La conexión central en la nube no está abierta actualmente.\n"
                           "Por favor, verifique la conexión a Internet o seleccione la Tienda Local.");
      return;
    }
  } else {
    if (!QSqlDatabase::database(origen.nombreConexion).isOpen()) {
      QMessageBox::warning(this, "Error de Conexión",
                           QString("La conexión a '%1' no está abierta.").arg(origen.nombreTienda));
      return;
    }
  }

  // Feedback visual de espera
  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  // Actualizar indicadores visuales de estado y modo
  actualizarEstadoUI();

  // 1. Cargar KPIs del panel superior (siempre visible e inmediato)
  cargarKPIs(origen, desde, hasta);

  // 2. Marcar todas las pestañas como pendientes
  for (int i = 0; i < ui->tabWidgetEstadisticas->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar de inmediato únicamente la pestaña actualmente visible
  int tabActiva = ui->tabWidgetEstadisticas->currentIndex();
  cargarPestana(tabActiva);
  pestanasPendientes[tabActiva] = false;

  // Restaurar estado de controles y cursor
  ui->btnActualizar->setEnabled(true);
  QApplication::restoreOverrideCursor();
}

/**
 * @brief Despacha la carga de la pestaña indicada según su índice.
 */
void Estadisticas::cargarPestana(int index) {
  InfoOrigenEstadisticas origen = obtenerOrigenActual();
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  switch (index) {
  case 0:
    cargarGraficoVentas(origen, desde, hasta);
    break;
  case 1:
    cargarTablasProductos(origen, desde, hasta);
    break;
  case 2:
    cargarTablaFamilias(origen, desde, hasta);
    break;
  case 3:
    cargarTablaClientes(origen, desde, hasta);
    break;
  case 4:
    cargarTablaVendedores(origen, desde, hasta);
    cargarTablaFormaPago(origen, desde, hasta);
    break;
  default:
    break;
  }
}

/**
 * @brief Carga los KPIs numéricos del panel superior.
 */
void Estadisticas::cargarKPIs(const InfoOrigenEstadisticas &origen, const QDate &desde,
                              const QDate &hasta) {
  double totalVentas = 0.0;
  int numTickets = 0;
  double totalCompras = 0.0;
  int clientesAct = 0;
  int totalArticulosStock = 0;

  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    // Consultas directas y optimizadas a la Nube
    totalVentas = base.estadisticasTotalVentasNube(desde, hasta, origen.idTienda);
    numTickets = base.estadisticasNumeroTicketsNube(desde, hasta, origen.idTienda);
    totalCompras = base.estadisticasTotalComprasNube(desde, hasta, origen.idTienda);
    clientesAct = base.estadisticasClientesActivosNube(desde, hasta, origen.idTienda);
    totalArticulosStock = base.estadisticasTotalArticulosStockNube(origen.idTienda);
  } else {
    // Consulta a base de datos local o remota directa
    totalVentas = base.estadisticasTotalVentas(origen.nombreConexion, desde, hasta, modoConsolidado);
    numTickets = base.estadisticasNumeroTickets(origen.nombreConexion, desde, hasta, modoConsolidado);
    totalCompras = base.estadisticasTotalCompras(origen.nombreConexion, desde, hasta);
    clientesAct = base.estadisticasClientesActivos(origen.nombreConexion, desde, hasta, modoConsolidado);
    totalArticulosStock = base.estadisticasTotalArticulosStock(origen.nombreConexion);
  }

  double ticketMedio = (numTickets > 0) ? (totalVentas / numTickets) : 0.0;

  // Formatear y mostrar los valores en los labels
  ui->lblVentasTotal->setText(QString::number(totalVentas, 'f', 2) + " €");
  ui->lblNumTickets->setText(QString::number(numTickets));
  ui->lblTicketMedio->setText(QString::number(ticketMedio, 'f', 2) + " €");

  ui->lblTotalCompras->setText(QString::number(totalCompras, 'f', 2) + " €");
  ui->lblClientesActivos->setText(QString::number(clientesAct));
  ui->lblValorStock->setText(QString::number(totalArticulosStock) + " uds");
}

/**
 * @brief Carga los datos del gráfico de ventas por día.
 */
void Estadisticas::cargarGraficoVentas(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                       const QDate &hasta) {
  QSqlQuery query;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    query = base.estadisticasVentasPorPeriodoNube(desde, hasta, "dia", origen.idTienda);
  } else {
    query = base.estadisticasVentasPorPeriodo(origen.nombreConexion, desde, hasta, "dia", modoConsolidado);
  }

  QStringList categorias;
  QList<double> valoresTotales;

  while (query.next()) {
    categorias.append(query.value(0).toString());
    valoresTotales.append(query.value(1).toDouble());
  }
  query.finish();

  QList<QList<double>> seriesData;
  seriesData.append(valoresTotales);

  QStringList nombresSeries = {"Ventas Totales"};
  QList<QColor> colores = {QColor("#2E7D32")};

  graficoVentas->configurar("Evolución de Ventas", categorias, seriesData,
                            nombresSeries, colores, "barras");
}

/**
 * @brief Carga las tablas de artículos más vendidos y más rentables en memoria (QStandardItemModel).
 */
void Estadisticas::cargarTablasProductos(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                         const QDate &hasta) {
  // 1. Artículos más vendidos por cantidad
  delete modeloTopVendidos;
  modeloTopVendidos = new QStandardItemModel(this);
  modeloTopVendidos->setHorizontalHeaderLabels({"Cód", "Descripción", "Cant"});

  QSqlQuery queryMasVendidos;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    queryMasVendidos = base.estadisticasTopArticulosVendidosNube(desde, hasta, 100, origen.idTienda);
  } else {
    queryMasVendidos = base.estadisticasTopArticulosVendidos(origen.nombreConexion, desde, hasta, 100, modoConsolidado);
  }

  int filaV = 0;
  while (queryMasVendidos.next()) {
    QStandardItem *itCod = new QStandardItem(queryMasVendidos.value(0).toString());
    QStandardItem *itDesc = new QStandardItem(queryMasVendidos.value(1).toString());
    QStandardItem *itCant = new QStandardItem();
    itCant->setData(queryMasVendidos.value(2).toDouble(), Qt::EditRole);
    itCant->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloTopVendidos->setItem(filaV, 0, itCod);
    modeloTopVendidos->setItem(filaV, 1, itDesc);
    modeloTopVendidos->setItem(filaV, 2, itCant);
    filaV++;
  }
  queryMasVendidos.finish();

  ui->tablaTopVendidos->setModel(modeloTopVendidos);
  ui->tablaTopVendidos->hideColumn(0);
  ui->tablaTopVendidos->resizeColumnsToContents();

  // 2. Artículos más rentables (margen PVP - Coste)
  delete modeloTopRentables;
  modeloTopRentables = new QStandardItemModel(this);
  modeloTopRentables->setHorizontalHeaderLabels({"Cód", "Descripción", "Margen €"});

  QSqlQuery queryRentables;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    queryRentables = base.estadisticasTopArticulosRentablesNube(desde, hasta, 100, origen.idTienda);
  } else {
    queryRentables = base.estadisticasTopArticulosRentables(origen.nombreConexion, desde, hasta, 100, modoConsolidado);
  }

  int filaR = 0;
  while (queryRentables.next()) {
    QStandardItem *itCod = new QStandardItem(queryRentables.value(0).toString());
    QStandardItem *itDesc = new QStandardItem(queryRentables.value(1).toString());
    QStandardItem *itMargen = new QStandardItem();
    itMargen->setData(queryRentables.value(2).toDouble(), Qt::EditRole);
    itMargen->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloTopRentables->setItem(filaR, 0, itCod);
    modeloTopRentables->setItem(filaR, 1, itDesc);
    modeloTopRentables->setItem(filaR, 2, itMargen);
    filaR++;
  }
  queryRentables.finish();

  ui->tablaTopRentables->setModel(modeloTopRentables);
  ui->tablaTopRentables->hideColumn(0);
  ui->tablaTopRentables->resizeColumnsToContents();
}

/**
 * @brief Carga la tabla de mejores clientes por importe total en memoria.
 */
void Estadisticas::cargarTablaClientes(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                       const QDate &hasta) {
  delete modeloMejoresClientes;
  modeloMejoresClientes = new QStandardItemModel(this);
  modeloMejoresClientes->setHorizontalHeaderLabels({"DNI/NIF", "Nombre", "Ventas €"});

  QSqlQuery query;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    query = base.estadisticasMejoresClientesNube(desde, hasta, 50, origen.idTienda);
  } else {
    query = base.estadisticasMejoresClientes(origen.nombreConexion, desde, hasta, 50, modoConsolidado);
  }

  int fila = 0;
  while (query.next()) {
    QStandardItem *itNif = new QStandardItem(query.value(0).toString());
    QStandardItem *itNom = new QStandardItem(query.value(1).toString());
    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(query.value(2).toDouble(), Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloMejoresClientes->setItem(fila, 0, itNif);
    modeloMejoresClientes->setItem(fila, 1, itNom);
    modeloMejoresClientes->setItem(fila, 2, itTotal);
    fila++;
  }
  query.finish();

  ui->tablaMejoresClientes->setModel(modeloMejoresClientes);
  ui->tablaMejoresClientes->resizeColumnsToContents();
}

/**
 * @brief Carga las ventas desglosadas por usuario/vendedor en memoria.
 */
void Estadisticas::cargarTablaVendedores(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                         const QDate &hasta) {
  delete modeloVendedores;
  modeloVendedores = new QStandardItemModel(this);
  modeloVendedores->setHorizontalHeaderLabels({"Usuario", "Total €"});

  QSqlQuery query;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    query = base.estadisticasVentasPorUsuarioNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasVentasPorUsuario(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  int fila = 0;
  while (query.next()) {
    QStandardItem *itUsuario = new QStandardItem(query.value(0).toString());
    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(query.value(1).toDouble(), Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloVendedores->setItem(fila, 0, itUsuario);
    modeloVendedores->setItem(fila, 1, itTotal);
    fila++;
  }
  query.finish();

  ui->tablaVendedores->setModel(modeloVendedores);
  ui->tablaVendedores->resizeColumnsToContents();
}

/**
 * @brief Carga las ventas desglosadas por forma de pago en memoria.
 */
void Estadisticas::cargarTablaFormaPago(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                        const QDate &hasta) {
  delete modeloFormaPago;
  modeloFormaPago = new QStandardItemModel(this);
  modeloFormaPago->setHorizontalHeaderLabels({"Id", "F. Pago", "Total €"});

  QSqlQuery query;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    query = base.estadisticasVentasPorFormaPagoNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasVentasPorFormaPago(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  int fila = 0;
  while (query.next()) {
    QStandardItem *itId = new QStandardItem(query.value(0).toString());
    QStandardItem *itTipo = new QStandardItem(query.value(1).toString());
    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(query.value(2).toDouble(), Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloFormaPago->setItem(fila, 0, itId);
    modeloFormaPago->setItem(fila, 1, itTipo);
    modeloFormaPago->setItem(fila, 2, itTotal);
    fila++;
  }
  query.finish();

  ui->tablaFormaPago->setModel(modeloFormaPago);
  ui->tablaFormaPago->hideColumn(0);
  ui->tablaFormaPago->resizeColumnsToContents();
}

/**
 * @brief Carga las ventas agrupadas por familia de artículo en memoria.
 */
void Estadisticas::cargarTablaFamilias(const InfoOrigenEstadisticas &origen, const QDate &desde,
                                       const QDate &hasta) {
  delete modeloFamilias;
  modeloFamilias = new QStandardItemModel(this);
  modeloFamilias->setHorizontalHeaderLabels({"Id", "Familia", "Ventas €"});

  QSqlQuery query;
  if (origen.tipo == TipoOrigenEstadisticas::NubeGlobal ||
      origen.tipo == TipoOrigenEstadisticas::NubeTienda) {
    query = base.estadisticasVentasPorFamiliaNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasVentasPorFamilia(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  int fila = 0;
  while (query.next()) {
    QStandardItem *itId = new QStandardItem(query.value(0).toString());
    QStandardItem *itFam = new QStandardItem(query.value(1).toString());
    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(query.value(2).toDouble(), Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloFamilias->setItem(fila, 0, itId);
    modeloFamilias->setItem(fila, 1, itFam);
    modeloFamilias->setItem(fila, 2, itTotal);
    fila++;
  }
  query.finish();

  ui->tablaFamilias->setModel(modeloFamilias);
}

