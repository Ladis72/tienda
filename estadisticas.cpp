#include "estadisticas.h"
#include "configuracion.h"
#include "ui_estadisticas.h"
#include <QDebug>
#include <QKeyEvent>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>

extern Configuracion *conf;

Estadisticas::Estadisticas(QWidget *parent)
    : QDialog(parent), ui(new Ui::Estadisticas) {
  ui->setupUi(this);

  // Modo normal por defecto (solo tabla tickets)
  modoConsolidado = false;

  // Configurar fechas por defecto (mes actual)
  QDate fechaActual = QDate::currentDate();
  ui->dateDesde->setDate(QDate(fechaActual.year(), fechaActual.month(), 1));
  ui->dateHasta->setDate(fechaActual);

  // Inicializar el gráfico de ventas vacío
  graficoVentas = new GraficoVentasWidget(this);
  ui->layoutGrafico->addWidget(graficoVentas);

  // Inicializar punteros de modelos a nullptr para gestión de memoria
  modeloTopVendidos = nullptr;
  modeloTopRentables = nullptr;
  modeloMejoresClientes = nullptr;
  modeloVendedores = nullptr;
  modeloFormaPago = nullptr;
  modeloFamilias = nullptr;

  // Cargar combo de tiendas (bloquear señales para evitar cargas innecesarias)
  conexiones = new conexionesRemotas(this);
  conexiones->base = &base;

  // Bloquear señales del combo mientras se llena para evitar
  // disparar on_comboTienda_currentIndexChanged prematuramente
  ui->comboTienda->blockSignals(true);
  cargarTiendas();
  ui->comboTienda->blockSignals(false);

  // Cargar datos iniciales
  cargarEstadisticas();
}

Estadisticas::~Estadisticas() {
  // Liberar modelos si existen
  delete modeloTopVendidos;
  delete modeloTopRentables;
  delete modeloMejoresClientes;
  delete modeloVendedores;
  delete modeloFormaPago;
  delete modeloFamilias;
  delete ui;
}

/**
 * @brief Captura la tecla F2 para conmutar entre modo normal y modo
 * consolidado.
 *
 * Modo normal: consulta solo la tabla 'tickets' (y 'lineasticket').
 * Modo consolidado (F2): consulta 'tickets' + 'ticketss' (y 'lineasticket' +
 * 'lineasticketss'). Tras conmutar, recarga automáticamente todas las
 * estadísticas.
 */
void Estadisticas::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F2) {
    // Conmutar modo consolidado
    modoConsolidado = !modoConsolidado;
    cargarEstadisticas();
  } else {
    // Pasar el evento al padre para comportamiento estándar
    QDialog::keyPressEvent(event);
  }
}

void Estadisticas::cargarTiendas() {
  ui->comboTienda->clear();

  // Primero la tienda local
  QString local = conf->getConexionLocal();
  if (QSqlDatabase::database(local).isOpen()) {
    ui->comboTienda->addItem("Tienda Local", local);
  }

  // Solo usamos las conexiones que ya están activas y abiertas en la aplicación
  QStringList activas = conf->getNombreConexionesActivas();
  for (const QString &tienda : activas) {
    if (tienda != local) {
      // Verificamos si la conexión está realmente abierta antes de añadirla
      if (QSqlDatabase::database(tienda).isOpen()) {
        ui->comboTienda->addItem(tienda, tienda);
      }
    }
  }
}

QString Estadisticas::getConexionSeleccionada() {
  if (ui->comboTienda->count() == 0)
    return conf->getConexionLocal();
  return ui->comboTienda->currentData().toString();
}

void Estadisticas::on_btnActualizar_clicked() { cargarEstadisticas(); }

void Estadisticas::on_comboTienda_currentIndexChanged(int /*index*/) {
  // Recargar automáticamente al cambiar de tienda
  cargarEstadisticas();
}

/**
 * @brief Carga todas las secciones del panel de estadísticas.
 *
 * Usa modoConsolidado para decidir si incluir ticketss en los cálculos.
 */
void Estadisticas::cargarEstadisticas() {
  QString db = getConexionSeleccionada();
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  // Verificar que la conexión esté abierta
  if (!QSqlDatabase::database(db).isOpen()) {
    QMessageBox::warning(
        this, "Error", "La conexión a la tienda seleccionada no está abierta.");
    return;
  }

  // Cargar todas las secciones del panel, pasando el modo consolidado
  cargarKPIs(db, desde, hasta);
  cargarGraficoVentas(db, desde, hasta);
  cargarTablasProductos(db, desde, hasta);
  cargarTablaClientes(db, desde, hasta);
  cargarTablaVendedores(db, desde, hasta);
  cargarTablaFormaPago(db, desde, hasta);
  cargarTablaFamilias(db, desde, hasta);
}

/**
 * @brief Carga los KPIs del panel superior.
 *
 * Las ventas totales, nº de tickets y ticket medio tienen en cuenta
 * la tabla ticketss si modoConsolidado está activado.
 */
void Estadisticas::cargarKPIs(const QString &db, const QDate &desde,
                              const QDate &hasta) {
  // Obtener valores de KPIs desde la base de datos
  double totalVentas =
      base.estadisticasTotalVentas(db, desde, hasta, modoConsolidado);
  int numTickets =
      base.estadisticasNumeroTickets(db, desde, hasta, modoConsolidado);
  double ticketMedio = (numTickets > 0) ? (totalVentas / numTickets) : 0.0;

  double totalCompras = base.estadisticasTotalCompras(db, desde, hasta);
  int clientesAct =
      base.estadisticasClientesActivos(db, desde, hasta, modoConsolidado);
  int totalArticulosStock = base.estadisticasTotalArticulosStock(db);

  // Formatear y mostrar los valores en los labels
  ui->lblVentasTotal->setText(QString::number(totalVentas, 'f', 2) + " €");
  ui->lblNumTickets->setText(QString::number(numTickets));
  ui->lblTicketMedio->setText(QString::number(ticketMedio, 'f', 2) + " €");

  ui->lblTotalCompras->setText(QString::number(totalCompras, 'f', 2) + " €");
  ui->lblClientesActivos->setText(QString::number(clientesAct));
  ui->lblValorStock->setText(QString::number(totalArticulosStock) + " uds");
}

/**
 * @brief Carga el gráfico de evolución de ventas por día.
 *
 * Incluye ticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarGraficoVentas(const QString &db, const QDate &desde,
                                       const QDate &hasta) {
  // Obtener datos de ventas agrupados por día
  QSqlQuery query = base.estadisticasVentasPorPeriodo(db, desde, hasta, "dia",
                                                      modoConsolidado);

  QStringList categorias;
  QList<double> valoresTotales;

  while (query.next()) {
    categorias.append(query.value(0).toString());     // Fecha/Periodo
    valoresTotales.append(query.value(1).toDouble()); // Total ventas
  }

  // Construir las series de datos para el gráfico
  QList<QList<double>> seriesData;
  seriesData.append(valoresTotales);

  QStringList nombresSeries = {"Ventas Totales"};
  QList<QColor> colores = {QColor("#2E7D32")};

  // Configurar el widget de gráfico con los datos obtenidos
  graficoVentas->configurar("Evolución de Ventas", categorias, seriesData,
                            nombresSeries, colores, "barras");
}

/**
 * @brief Carga las tablas de top vendidos y top rentables.
 *
 * Incluye lineasticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarTablasProductos(const QString &db, const QDate &desde,
                                         const QDate &hasta) {
  // --- Tabla de artículos más vendidos (por cantidad) ---
  delete modeloTopVendidos;
  modeloTopVendidos = new QSqlQueryModel(this);

  QSqlQuery queryMasVendidos = base.estadisticasTopArticulosVendidos(
      db, desde, hasta, 100, modoConsolidado);
  modeloTopVendidos->setQuery(std::move(queryMasVendidos));
  modeloTopVendidos->setHeaderData(0, Qt::Horizontal, "Cód");
  modeloTopVendidos->setHeaderData(1, Qt::Horizontal, "Descripción");
  modeloTopVendidos->setHeaderData(2, Qt::Horizontal, "Cant");
  ui->tablaTopVendidos->setModel(modeloTopVendidos);
  ui->tablaTopVendidos->hideColumn(0);
  ui->tablaTopVendidos->resizeColumnsToContents();

  // --- Tabla de artículos más rentables (por importe margen) ---
  delete modeloTopRentables;
  modeloTopRentables = new QSqlQueryModel(this);

  QSqlQuery queryRentables = base.estadisticasTopArticulosRentables(
      db, desde, hasta, 100, modoConsolidado);
  modeloTopRentables->setQuery(std::move(queryRentables));
  modeloTopRentables->setHeaderData(0, Qt::Horizontal, "Cód");
  modeloTopRentables->setHeaderData(1, Qt::Horizontal, "Descripción");
  modeloTopRentables->setHeaderData(2, Qt::Horizontal, "Importe");
  ui->tablaTopRentables->setModel(modeloTopRentables);
  ui->tablaTopRentables->hideColumn(0);
  ui->tablaTopRentables->resizeColumnsToContents();
}

/**
 * @brief Carga la tabla de mejores clientes.
 *
 * Incluye ticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarTablaClientes(const QString &db, const QDate &desde,
                                       const QDate &hasta) {
  delete modeloMejoresClientes;
  modeloMejoresClientes = new QSqlQueryModel(this);

  QSqlQuery query =
      base.estadisticasMejoresClientes(db, desde, hasta, 50, modoConsolidado);
  modeloMejoresClientes->setQuery(std::move(query));
  modeloMejoresClientes->setHeaderData(0, Qt::Horizontal, "DNI/NIF");
  modeloMejoresClientes->setHeaderData(1, Qt::Horizontal, "Nombre");
  modeloMejoresClientes->setHeaderData(2, Qt::Horizontal, "Ventas");
  ui->tablaMejoresClientes->setModel(modeloMejoresClientes);
  ui->tablaMejoresClientes->resizeColumnsToContents();
}

/**
 * @brief Carga la tabla de ventas por vendedor/usuario.
 *
 * Incluye ticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarTablaVendedores(const QString &db, const QDate &desde,
                                         const QDate &hasta) {
  delete modeloVendedores;
  modeloVendedores = new QSqlQueryModel(this);

  QSqlQuery query =
      base.estadisticasVentasPorUsuario(db, desde, hasta, modoConsolidado);
  modeloVendedores->setQuery(std::move(query));
  modeloVendedores->setHeaderData(0, Qt::Horizontal, "Usuario");
  modeloVendedores->setHeaderData(1, Qt::Horizontal, "Total");
  ui->tablaVendedores->setModel(modeloVendedores);
  ui->tablaVendedores->resizeColumnsToContents();
}

/**
 * @brief Carga la tabla de ventas por forma de pago.
 *
 * Incluye ticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarTablaFormaPago(const QString &db, const QDate &desde,
                                        const QDate &hasta) {
  delete modeloFormaPago;
  modeloFormaPago = new QSqlQueryModel(this);

  QSqlQuery query =
      base.estadisticasVentasPorFormaPago(db, desde, hasta, modoConsolidado);
  modeloFormaPago->setQuery(std::move(query));
  modeloFormaPago->setHeaderData(1, Qt::Horizontal, "F. Pago");
  modeloFormaPago->setHeaderData(2, Qt::Horizontal, "Total");
  ui->tablaFormaPago->setModel(modeloFormaPago);
  ui->tablaFormaPago->hideColumn(0);
  ui->tablaFormaPago->resizeColumnsToContents();
}

/**
 * @brief Carga la tabla de ventas agrupadas por familia de artículo.
 *
 * Incluye lineasticketss y ticketss si modoConsolidado está activo.
 */
void Estadisticas::cargarTablaFamilias(const QString &db, const QDate &desde,
                                       const QDate &hasta) {
  delete modeloFamilias;
  modeloFamilias = new QSqlQueryModel(this);

  QSqlQuery query =
      base.estadisticasVentasPorFamilia(db, desde, hasta, modoConsolidado);
  modeloFamilias->setQuery(std::move(query));
  modeloFamilias->setHeaderData(0, Qt::Horizontal, "Id");
  modeloFamilias->setHeaderData(1, Qt::Horizontal, "Familia");
  modeloFamilias->setHeaderData(2, Qt::Horizontal, "Ventas");
  ui->tablaFamilias->setModel(modeloFamilias);
  ui->tablaFamilias->resizeColumnsToContents();
}
