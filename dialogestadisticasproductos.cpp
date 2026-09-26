#include "dialogestadisticasproductos.h"
#include "ui_dialogestadisticasproductos.h"
#include "configuracion.h"
#include "syncmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include "estadisticas_utils.h"
#include <QSqlQuery>
#include <QTextStream>

extern Configuracion *conf;

DialogEstadisticasProductos::DialogEstadisticasProductos(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasProductos) {
  ui->setupUi(this);

  // Inicializar fechas: desde el primer día del mes actual hasta la fecha actual
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);

  // Inicializar límites en el combo
  ui->comboLimite->addItem("Top 25", 25);
  ui->comboLimite->addItem("Top 50", 50);
  ui->comboLimite->addItem("Top 100", 100);
  ui->comboLimite->addItem("Top 250", 250);
  ui->comboLimite->addItem("Todos (sin límite)", 0);
  ui->comboLimite->setCurrentIndex(2); // Top 100 por defecto

  // Inicializar punteros a modelos de memoria
  modeloTopVendidos = nullptr;
  modeloTopRentables = nullptr;
  modeloFamilias = nullptr;
  modeloBajaRotacion = nullptr;

  // Cargar combos de tiendas y familias
  inicializarFiltros();
}

DialogEstadisticasProductos::~DialogEstadisticasProductos() {
  delete modeloTopVendidos;
  delete modeloTopRentables;
  delete modeloFamilias;
  delete modeloBajaRotacion;
  delete ui;
}

void DialogEstadisticasProductos::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarEstadisticas();
}

/**
 * @brief Rellena los desplegables de Tiendas y Familias desde la base de datos central en la nube.
 */
void DialogEstadisticasProductos::inicializarFiltros() {
  ui->comboTienda->blockSignals(true);
  ui->comboFamilia->blockSignals(true);

  ui->comboTienda->clear();
  ui->comboFamilia->clear();

  // Opción global de tienda
  ui->comboTienda->addItem("🌐 Cadena Completa (Todas las Tiendas)", -1);

  // Cargar tiendas individuales registradas en la nube
  QSqlDatabase dbNube = base.obtenerConexionNube();
  if (dbNube.isValid() && dbNube.isOpen()) {
    QSqlQuery qTiendas(dbNube);
    if (qTiendas.exec("SELECT id, nombre FROM tiendas ORDER BY id ASC")) {
      while (qTiendas.next()) {
        int idT = qTiendas.value(0).toInt();
        QString nombreT = qTiendas.value(1).toString();
        ui->comboTienda->addItem(QString("🏪 %1").arg(nombreT), idT);
      }
    }
  }

  // Opción global de familias
  ui->comboFamilia->addItem("📂 Todas las Familias", -1);

  // Cargar lista de familias desde la base de datos
  QSqlQuery qFam = base.listaFamiliasNube();
  while (qFam.next()) {
    int idFam = qFam.value(0).toInt();
    QString nombreFam = qFam.value(1).toString();
    ui->comboFamilia->addItem(nombreFam, idFam);
  }

  ui->comboTienda->blockSignals(false);
  ui->comboFamilia->blockSignals(false);
}

int DialogEstadisticasProductos::obtenerIdTiendaSeleccionada() const {
  return ui->comboTienda->currentData().toInt();
}

int DialogEstadisticasProductos::obtenerIdFamiliaSeleccionada() const {
  return ui->comboFamilia->currentData().toInt();
}

int DialogEstadisticasProductos::obtenerLimiteSeleccionado() const {
  return ui->comboLimite->currentData().toInt();
}

/**
 * @brief Recarga todos los cálculos estadísticos según los filtros vigentes.
 */
void DialogEstadisticasProductos::cargarEstadisticas() {
  QSqlDatabase dbNube = base.obtenerConexionNube();
  if (!dbNube.isValid() || !dbNube.isOpen()) {
    QMessageBox::warning(
        this, "Aviso Conexión Nube",
        "La base de datos central en la nube no está disponible.\n"
        "El análisis analítico de productos requiere conexión activa a la nube.");
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  QElapsedTimer cronometro;
  cronometro.start();

  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  int idTienda = obtenerIdTiendaSeleccionada();
  int idFamilia = obtenerIdFamiliaSeleccionada();

  // 1. Cargar tarjetas KPI
  cargarKPIs(desde, hasta, idFamilia, idTienda);

  // 2. Marcar pestañas para recarga bajo demanda
  for (int i = 0; i < ui->tabWidgetProductos->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar inmediatamente la pestaña activa
  int activa = ui->tabWidgetProductos->currentIndex();
  cargarPestana(activa);
  pestanasPendientes[activa] = false;

  qint64 tiempoMs = cronometro.elapsed();
  ui->labelTiempoConsulta->setText(QString("⚡ %1 ms").arg(tiempoMs));

  ui->btnActualizar->setEnabled(true);
  QApplication::restoreOverrideCursor();
}

/**
 * @brief Carga bajo demanda la pestaña activa.
 */
void DialogEstadisticasProductos::cargarPestana(int index) {
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  int idTienda = obtenerIdTiendaSeleccionada();
  int idFamilia = obtenerIdFamiliaSeleccionada();
  int limite = obtenerLimiteSeleccionado();

  switch (index) {
  case 0:
    cargarTablaTopVendidos(desde, hasta, idFamilia, limite, idTienda);
    break;
  case 1:
    cargarTablaTopRentables(desde, hasta, idFamilia, limite, idTienda);
    break;
  case 2:
    cargarTablaFamilias(desde, hasta, idTienda);
    break;
  case 3:
    cargarTablaBajaRotacion(desde, hasta, idFamilia, limite, idTienda);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula y visualiza las métricas clave (KPIs) en las tarjetas superiores.
 */
void DialogEstadisticasProductos::cargarKPIs(const QDate &desde, const QDate &hasta, int idFamilia, int idTienda) {
  // 1. Producto más vendido en facturación
  QSqlQuery qTopVenta = base.estadisticasRankingProductosVentasNube(desde, hasta, idFamilia, 1, idTienda);
  if (qTopVenta.next()) {
    QString desc = qTopVenta.value("descripcion").toString();
    double totalVentas = qTopVenta.value("total_ventas").toDouble();
    double uds = qTopVenta.value("cantidad_total").toDouble();

    ui->kpiTopVendidoValor->setText(desc);
    ui->kpiTopVendidoValor->setToolTip(desc);
    ui->kpiTopVendidoSub->setText(QString("%1 € · %2 uds")
                                      .arg(QString::number(totalVentas, 'f', 2))
                                      .arg(QString::number(uds, 'f', 0)));
  } else {
    ui->kpiTopVendidoValor->setText("Sin datos");
    ui->kpiTopVendidoSub->setText("0,00 € · 0 uds");
  }

  // 2. Producto más rentable
  QSqlQuery qTopRent = base.estadisticasRankingProductosRentablesNube(desde, hasta, idFamilia, 1, idTienda);
  if (qTopRent.next()) {
    QString desc = qTopRent.value("descripcion").toString();
    double beneficio = qTopRent.value("beneficio").toDouble();
    double margenPct = qTopRent.value("margen_pct").toDouble();

    ui->kpiTopRentableValor->setText(desc);
    ui->kpiTopRentableValor->setToolTip(desc);
    ui->kpiTopRentableSub->setText(QString("+%1 € beneficio · %2% margen")
                                       .arg(QString::number(beneficio, 'f', 2))
                                       .arg(QString::number(margenPct, 'f', 1)));
  } else {
    ui->kpiTopRentableValor->setText("Sin datos");
    ui->kpiTopRentableSub->setText("0,00 € · 0%");
  }

  // 3. Familia líder y cálculo de margen medio global
  QSqlQuery qFam = base.estadisticasDesgloseFamiliasNube(desde, hasta, idTienda);
  double totalVentasCadena = 0.0;
  double totalBeneficioCadena = 0.0;
  QString mejorFamilia = "Sin datos";
  double maxVentaFam = 0.0;

  bool primera = true;
  while (qFam.next()) {
    QString nomFam = qFam.value("nombre_familia").toString();
    double ventasFam = qFam.value("total_ventas").toDouble();
    double benFam = qFam.value("beneficio_familia").toDouble();

    totalVentasCadena += ventasFam;
    totalBeneficioCadena += benFam;

    if (primera || ventasFam > maxVentaFam) {
      maxVentaFam = ventasFam;
      mejorFamilia = nomFam;
      primera = false;
    }
  }

  if (!primera && totalVentasCadena > 0) {
    double cuotaFamLider = (maxVentaFam / totalVentasCadena) * 100.0;
    ui->kpiFamiliaLiderValor->setText(mejorFamilia);
    ui->kpiFamiliaLiderValor->setToolTip(mejorFamilia);
    ui->kpiFamiliaLiderSub->setText(QString("%1 € (%2% de cuota)")
                                        .arg(QString::number(maxVentaFam, 'f', 2))
                                        .arg(QString::number(cuotaFamLider, 'f', 1)));

    double margenMedioGlobal = (totalBeneficioCadena / totalVentasCadena) * 100.0;
    ui->kpiMargenMedioValor->setText(QString("%1 %").arg(QString::number(margenMedioGlobal, 'f', 1)));
    ui->kpiMargenMedioSub->setText(QString("+%1 € beneficio neto total").arg(QString::number(totalBeneficioCadena, 'f', 2)));
  } else {
    ui->kpiFamiliaLiderValor->setText("Sin datos");
    ui->kpiFamiliaLiderSub->setText("0,00 € · 0%");
    ui->kpiMargenMedioValor->setText("--");
    ui->kpiMargenMedioSub->setText("Sin datos en el período");
  }
}

/**
 * @brief Carga el ranking de productos por volumen y facturación en la tabla 1.
 */
void DialogEstadisticasProductos::cargarTablaTopVendidos(const QDate &desde, const QDate &hasta,
                                                         int idFamilia, int limite, int idTienda) {
  if (modeloTopVendidos) {
    delete modeloTopVendidos;
  }
  modeloTopVendidos = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "Código", "Descripción del Artículo", "Familia",
                           "Uds. Vendidas", "Total Facturado", "Precio Medio", "% Cuota Catálogo"};
  modeloTopVendidos->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasRankingProductosVentasNube(desde, hasta, idFamilia, limite, idTienda);

  // Calcular la suma total facturada de los artículos devueltos para calcular cuotas
  struct FilaProducto {
    QString cod;
    QString desc;
    QString fam;
    double uds;
    double total;
    double pMedio;
  };
  QList<FilaProducto> lista;
  double sumaFacturacion = 0.0;

  while (query.next()) {
    FilaProducto f;
    f.cod = query.value("cod").toString();
    f.desc = query.value("descripcion").toString();
    f.fam = query.value("nombre_familia").toString();
    f.uds = query.value("cantidad_total").toDouble();
    f.total = query.value("total_ventas").toDouble();
    f.pMedio = query.value("precio_medio").toDouble();
    sumaFacturacion += f.total;
    lista.append(f);
  }

  int pos = 1;
  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    // Posición
    NumericStandardItem *iPos = new NumericStandardItem(pos++, 0);
    iPos->setTextAlignment(Qt::AlignCenter);
    fila << iPos;

    // Código
    QStandardItem *iCod = new QStandardItem(item.cod);
    iCod->setTextAlignment(Qt::AlignCenter);
    fila << iCod;

    // Descripción
    QStandardItem *iDesc = new QStandardItem(item.desc);
    fila << iDesc;

    // Familia
    QStandardItem *iFam = new QStandardItem(item.fam);
    fila << iFam;

    // Unidades
    NumericStandardItem *iUds = new NumericStandardItem(item.uds, 0);
    fila << iUds;

    // Total Facturado
    NumericStandardItem *iTotal = new NumericStandardItem(item.total, 2, "€");
    fila << iTotal;

    // Precio Medio
    NumericStandardItem *iPMedio = new NumericStandardItem(item.pMedio, 2, "€");
    fila << iPMedio;

    // % Cuota
    double cuota = (sumaFacturacion > 0) ? (item.total / sumaFacturacion) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    modeloTopVendidos->appendRow(fila);
  }

  ui->tablaTopVendidos->setModel(modeloTopVendidos);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaTopVendidos->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga el ranking de productos por rentabilidad neta y margen comercial en la tabla 2.
 */
void DialogEstadisticasProductos::cargarTablaTopRentables(const QDate &desde, const QDate &hasta,
                                                          int idFamilia, int limite, int idTienda) {
  if (modeloTopRentables) {
    delete modeloTopRentables;
  }
  modeloTopRentables = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "Código", "Descripción del Artículo", "Familia",
                           "Uds.", "Venta Total", "Coste Total", "Beneficio Neto", "Margen %"};
  modeloTopRentables->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasRankingProductosRentablesNube(desde, hasta, idFamilia, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    QString fam = query.value("nombre_familia").toString();
    double uds = query.value("cantidad_total").toDouble();
    double totalVentas = query.value("total_ventas").toDouble();
    double totalCoste = query.value("total_coste").toDouble();
    double beneficio = query.value("beneficio").toDouble();
    double margenPct = query.value("margen_pct").toDouble();

    QList<QStandardItem *> fila;

    NumericStandardItem *iPos = new NumericStandardItem(pos++, 0);
    iPos->setTextAlignment(Qt::AlignCenter);
    fila << iPos;

    QStandardItem *iCod = new QStandardItem(cod);
    iCod->setTextAlignment(Qt::AlignCenter);
    fila << iCod;

    QStandardItem *iDesc = new QStandardItem(desc);
    fila << iDesc;

    QStandardItem *iFam = new QStandardItem(fam);
    fila << iFam;

    NumericStandardItem *iUds = new NumericStandardItem(uds, 0);
    fila << iUds;

    NumericStandardItem *iVentas = new NumericStandardItem(totalVentas, 2, "€");
    fila << iVentas;

    NumericStandardItem *iCoste = new NumericStandardItem(totalCoste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iBeneficio = new NumericStandardItem(beneficio, 2, "€");
    if (beneficio > 0) {
      iBeneficio->setForeground(QBrush(QColor(16, 124, 65))); // Verde financiero
    } else if (beneficio < 0) {
      iBeneficio->setForeground(QBrush(QColor(180, 40, 40))); // Rojo alerta
    }
    fila << iBeneficio;

    NumericStandardItem *iMargen = new NumericStandardItem(margenPct, 1, "%");
    fila << iMargen;

    modeloTopRentables->appendRow(fila);
  }

  ui->tablaTopRentables->setModel(modeloTopRentables);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaTopRentables->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga el desglose agregado por familias de productos en la tabla 3.
 */
void DialogEstadisticasProductos::cargarTablaFamilias(const QDate &desde, const QDate &hasta, int idTienda) {
  if (modeloFamilias) {
    delete modeloFamilias;
  }
  modeloFamilias = new QStandardItemModel(this);

  QStringList cabeceras = {"ID", "Familia / Categoría", "Uds. Vendidas", "Total Facturado",
                           "% Cuota Ventas", "Nº Tickets", "Beneficio Total", "Margen %"};
  modeloFamilias->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasDesgloseFamiliasNube(desde, hasta, idTienda);

  struct FilaFam {
    int id;
    QString nombre;
    double uds;
    double ventas;
    int tickets;
    double beneficio;
  };
  QList<FilaFam> lista;
  double sumaVentas = 0.0;

  while (query.next()) {
    FilaFam f;
    f.id = query.value("id_familia").toInt();
    f.nombre = query.value("nombre_familia").toString();
    f.uds = query.value("unidades_vendidas").toDouble();
    f.ventas = query.value("total_ventas").toDouble();
    f.tickets = query.value("num_tickets").toInt();
    f.beneficio = query.value("beneficio_familia").toDouble();
    sumaVentas += f.ventas;
    lista.append(f);
  }

  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    NumericStandardItem *iId = new NumericStandardItem(item.id, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(item.nombre);
    fila << iNom;

    NumericStandardItem *iUds = new NumericStandardItem(item.uds, 0);
    fila << iUds;

    NumericStandardItem *iVentas = new NumericStandardItem(item.ventas, 2, "€");
    fila << iVentas;

    double cuota = (sumaVentas > 0) ? (item.ventas / sumaVentas) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iTickets = new NumericStandardItem(item.tickets, 0);
    fila << iTickets;

    NumericStandardItem *iBeneficio = new NumericStandardItem(item.beneficio, 2, "€");
    if (item.beneficio > 0) {
      iBeneficio->setForeground(QBrush(QColor(16, 124, 65)));
    }
    fila << iBeneficio;

    double margenPct = (item.ventas > 0) ? (item.beneficio / item.ventas) * 100.0 : 0.0;
    NumericStandardItem *iMargen = new NumericStandardItem(margenPct, 1, "%");
    fila << iMargen;

    modeloFamilias->appendRow(fila);
  }

  ui->tablaFamilias->setModel(modeloFamilias);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaFamilias->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga los artículos con stock físico positivo pero sin ninguna venta en el período (baja rotación).
 */
void DialogEstadisticasProductos::cargarTablaBajaRotacion(const QDate &desde, const QDate &hasta,
                                                          int idFamilia, int limite, int idTienda) {
  if (modeloBajaRotacion) {
    delete modeloBajaRotacion;
  }
  modeloBajaRotacion = new QStandardItemModel(this);

  QStringList cabeceras = {"Código", "Descripción", "Familia", "PVP", "Precio Coste",
                           "Stock en Almacén", "Capital Inmovilizado"};
  modeloBajaRotacion->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasProductosSinVentasNube(desde, hasta, idFamilia, limite, idTienda);

  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    QString fam = query.value("famil").toString();
    if (fam.isEmpty()) fam = query.value("familia").toString();
    double pvp = query.value("pvp").toDouble();
    double coste = query.value("coste").toDouble();
    double stock = query.value("stock_actual").toDouble();
    double inmovilizado = query.value("valor_inmovilizado").toDouble();

    QList<QStandardItem *> fila;

    QStandardItem *iCod = new QStandardItem(cod);
    iCod->setTextAlignment(Qt::AlignCenter);
    fila << iCod;

    QStandardItem *iDesc = new QStandardItem(desc);
    fila << iDesc;

    QStandardItem *iFam = new QStandardItem(fam);
    fila << iFam;

    NumericStandardItem *iPvp = new NumericStandardItem(pvp, 2, "€");
    fila << iPvp;

    NumericStandardItem *iCoste = new NumericStandardItem(coste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iStock = new NumericStandardItem(stock, 0);
    fila << iStock;

    NumericStandardItem *iInmov = new NumericStandardItem(inmovilizado, 2, "€");
    iInmov->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iInmov;

    modeloBajaRotacion->appendRow(fila);
  }

  ui->tablaBajaRotacion->setModel(modeloBajaRotacion);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaBajaRotacion->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

void DialogEstadisticasProductos::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasProductos::on_tabWidgetProductos_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    cargarPestana(index);
    pestanasPendientes[index] = false;
    QApplication::restoreOverrideCursor();
  }
}

// Accesos rápidos a períodos
void DialogEstadisticasProductos::on_btnHoy_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(hoy);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasProductos::on_btnSemana_clicked() {
  QDate hoy = QDate::currentDate();
  QDate inicioSemana = hoy.addDays(-(hoy.dayOfWeek() - 1));
  ui->dateDesde->setDate(inicioSemana);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasProductos::on_btnMes_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasProductos::on_btnAno_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

/**
 * @brief Exporta la tabla activa a un archivo CSV estructurado.
 */
void DialogEstadisticasProductos::on_btnExportarCSV_clicked() {
  int tabIdx = ui->tabWidgetProductos->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  QString sufijo = "productos";

  switch (tabIdx) {
  case 0:
    modeloActual = modeloTopVendidos;
    sufijo = "ranking_ventas";
    break;
  case 1:
    modeloActual = modeloTopRentables;
    sufijo = "ranking_rentabilidad";
    break;
  case 2:
    modeloActual = modeloFamilias;
    sufijo = "desglose_familias";
    break;
  case 3:
    modeloActual = modeloBajaRotacion;
    sufijo = "baja_rotacion_stock_muerto";
    break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Exportar a CSV", "No hay datos disponibles en la tabla activa para exportar.");
    return;
  }

  QString nombreSugerido = QString("estadisticas_%1_%2_%3.csv")
                               .arg(sufijo)
                               .arg(ui->dateDesde->date().toString("yyyy-MM-dd"))
                               .arg(ui->dateHasta->date().toString("yyyy-MM-dd"));

  QString ruta = QFileDialog::getSaveFileName(this, "Guardar estadísticas como CSV", nombreSugerido, "Archivos CSV (*.csv)");
  if (ruta.isEmpty()) return;

  QFile archivo(ruta);
  if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(this, "Error al Exportar", "No se pudo escribir en el archivo seleccionado:\n" + archivo.errorString());
    return;
  }

  QTextStream out(&archivo);

  // Escribir cabeceras
  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    out << "\"" << modeloActual->headerData(c, Qt::Horizontal).toString() << "\"";
    if (c < modeloActual->columnCount() - 1) out << ";";
  }
  out << "\n";

  // Escribir filas
  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *item = modeloActual->item(r, c);
      QString texto = item ? item->text() : "";
      out << "\"" << texto << "\"";
      if (c < modeloActual->columnCount() - 1) out << ";";
    }
    out << "\n";
  }

  archivo.close();
  QMessageBox::information(this, "Exportación Completada", "Los datos se exportaron correctamente a:\n" + ruta);
}

/**
 * @brief Copia al portapapeles el contenido completo de la tabla activa formateado con tabulaciones (para Excel/Calc).
 */
void DialogEstadisticasProductos::on_btnCopiarTabla_clicked() {
  int tabIdx = ui->tabWidgetProductos->currentIndex();
  QStandardItemModel *modeloActual = nullptr;

  switch (tabIdx) {
  case 0: modeloActual = modeloTopVendidos; break;
  case 1: modeloActual = modeloTopRentables; break;
  case 2: modeloActual = modeloFamilias; break;
  case 3: modeloActual = modeloBajaRotacion; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Copiar al Portapapeles", "No hay datos para copiar en la tabla activa.");
    return;
  }

  QString buffer;

  // Cabeceras
  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    buffer += modeloActual->headerData(c, Qt::Horizontal).toString();
    if (c < modeloActual->columnCount() - 1) buffer += "\t";
  }
  buffer += "\n";

  // Filas
  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *item = modeloActual->item(r, c);
      buffer += item ? item->text() : "";
      if (c < modeloActual->columnCount() - 1) buffer += "\t";
    }
    buffer += "\n";
  }

  QApplication::clipboard()->setText(buffer);
  QMessageBox::information(this, "Copiado al Portapapeles", "La tabla activa se ha copiado al portapapeles.\nPuede pegarla directamente en Excel, LibreOffice Calc o cualquier editor.");
}
