#include "dialogestadisticasstockmuerto.h"
#include "ui_dialogestadisticasstockmuerto.h"
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

DialogEstadisticasStockMuerto::DialogEstadisticasStockMuerto(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasStockMuerto) {
  ui->setupUi(this);

  // Inicializar punteros a modelos en memoria
  modeloStockMuerto = nullptr;
  modeloNuncaVendidos = nullptr;
  modeloInmovilizadoFamilias = nullptr;
  modeloLiquidacion = nullptr;

  // Configurar filtros y desplegables
  inicializarFiltros();
}

DialogEstadisticasStockMuerto::~DialogEstadisticasStockMuerto() {
  delete modeloStockMuerto;
  delete modeloNuncaVendidos;
  delete modeloInmovilizadoFamilias;
  delete modeloLiquidacion;
  delete ui;
}

void DialogEstadisticasStockMuerto::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarEstadisticas();
}

/**
 * @brief Rellena los desplegables de filtros y el simulador de liquidación.
 */
void DialogEstadisticasStockMuerto::inicializarFiltros() {
  ui->comboDias->blockSignals(true);
  ui->comboTienda->blockSignals(true);
  ui->comboFamilia->blockSignals(true);
  ui->comboLimite->blockSignals(true);
  ui->comboDescuento->blockSignals(true);

  // 1. Selector de días de inactividad
  ui->comboDias->clear();
  ui->comboDias->addItem("Más de 30 días sin ventas", 30);
  ui->comboDias->addItem("Más de 60 días sin ventas", 60);
  ui->comboDias->addItem("Más de 90 días (3 meses)", 90);
  ui->comboDias->addItem("Más de 180 días (6 meses)", 180);
  ui->comboDias->addItem("Más de 365 días (1 año)", 365);
  ui->comboDias->setCurrentIndex(2); // 90 días por defecto

  // 2. Límites
  ui->comboLimite->clear();
  ui->comboLimite->addItem("Top 50", 50);
  ui->comboLimite->addItem("Top 100", 100);
  ui->comboLimite->addItem("Top 250", 250);
  ui->comboLimite->addItem("Top 500", 500);
  ui->comboLimite->addItem("Todos (sin límite)", 0);
  ui->comboLimite->setCurrentIndex(1); // Top 100 por defecto

  // 3. Tiendas
  ui->comboTienda->clear();
  ui->comboTienda->addItem("🌐 Cadena Completa (Todas las Tiendas)", -1);

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

  // 4. Familias
  ui->comboFamilia->clear();
  ui->comboFamilia->addItem("📂 Todas las Familias", -1);
  QSqlQuery qFam = base.listaFamiliasNube();
  while (qFam.next()) {
    int idFam = qFam.value(0).toInt();
    QString nombreFam = qFam.value(1).toString();
    ui->comboFamilia->addItem(nombreFam, idFam);
  }

  // 5. Simulador de descuentos
  ui->comboDescuento->clear();
  ui->comboDescuento->addItem("🏷️ -15% Descuento (Promoción)", 15);
  ui->comboDescuento->addItem("🏷️ -25% Descuento (Rebajas)", 25);
  ui->comboDescuento->addItem("🔥 -35% Descuento (Liquidación)", 35);
  ui->comboDescuento->addItem("⚡ -50% Descuento (Outlet / Mitad precio)", 50);
  ui->comboDescuento->addItem("🎯 A Precio de Coste (0% Margen)", 0);
  ui->comboDescuento->setCurrentIndex(2); // 35% por defecto

  ui->comboDias->blockSignals(false);
  ui->comboTienda->blockSignals(false);
  ui->comboFamilia->blockSignals(false);
  ui->comboLimite->blockSignals(false);
  ui->comboDescuento->blockSignals(false);
}

int DialogEstadisticasStockMuerto::obtenerDiasSeleccionados() const {
  return ui->comboDias->currentData().toInt();
}

int DialogEstadisticasStockMuerto::obtenerIdTiendaSeleccionada() const {
  return ui->comboTienda->currentData().toInt();
}

int DialogEstadisticasStockMuerto::obtenerIdFamiliaSeleccionada() const {
  return ui->comboFamilia->currentData().toInt();
}

int DialogEstadisticasStockMuerto::obtenerLimiteSeleccionado() const {
  return ui->comboLimite->currentData().toInt();
}

/**
 * @brief Recarga los datos analíticos de stock muerto.
 */
void DialogEstadisticasStockMuerto::cargarEstadisticas() {
  QSqlDatabase dbNube = base.obtenerConexionNube();
  if (!dbNube.isValid() || !dbNube.isOpen()) {
    QMessageBox::warning(
        this, "Aviso Conexión Nube",
        "La base de datos central en la nube no está disponible.\n"
        "El análisis de stock muerto requiere conexión activa a la nube.");
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  QElapsedTimer cronometro;
  cronometro.start();

  int dias = obtenerDiasSeleccionados();
  int idFamilia = obtenerIdFamiliaSeleccionada();
  int idTienda = obtenerIdTiendaSeleccionada();

  // 1. Cargar KPIs superiores
  cargarKPIs(dias, idFamilia, idTienda);

  // 2. Marcar pestañas para recarga diferida
  for (int i = 0; i < ui->tabWidgetStockMuerto->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar inmediatamente la pestaña activa
  int activa = ui->tabWidgetStockMuerto->currentIndex();
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
void DialogEstadisticasStockMuerto::cargarPestana(int index) {
  int dias = obtenerDiasSeleccionados();
  int idFamilia = obtenerIdFamiliaSeleccionada();
  int limite = obtenerLimiteSeleccionado();
  int idTienda = obtenerIdTiendaSeleccionada();

  switch (index) {
  case 0:
    cargarTablaStockMuerto(dias, idFamilia, limite, idTienda);
    break;
  case 1:
    cargarTablaNuncaVendidos(idFamilia, limite, idTienda);
    break;
  case 2:
    cargarTablaInmovilizadoFamilias(dias, idTienda);
    break;
  case 3:
    cargarTablaSimulador(dias, idFamilia, limite, idTienda);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula y visualiza las tarjetas KPI de stock muerto.
 */
void DialogEstadisticasStockMuerto::cargarKPIs(int dias, int idFamilia, int idTienda) {
  QSqlQuery query = base.estadisticasStockMuertoNube(dias, idFamilia, 0, idTienda);

  double sumaInmovilizado = 0.0;
  double sumaValorPVP = 0.0;
  double sumaUnidades = 0.0;
  double sumaDias = 0.0;
  int totalArticulos = 0;

  while (query.next()) {
    totalArticulos++;
    sumaInmovilizado += query.value("capital_inmovilizado").toDouble();
    sumaValorPVP += query.value("valor_venta").toDouble();
    sumaUnidades += query.value("stock_actual").toDouble();
    sumaDias += query.value("dias_sin_venta").toDouble();
  }

  if (totalArticulos > 0) {
    ui->kpiCapitalInmovilizadoValor->setText(QString("%1 €").arg(QString::number(sumaInmovilizado, 'f', 2)));
    ui->kpiArticulosAfectadosValor->setText(QString("%1 referencias").arg(totalArticulos));
    ui->kpiArticulosAfectadosSub->setText(QString("%1 unidades en stock").arg(QString::number(sumaUnidades, 'f', 0)));

    double diasMedios = sumaDias / totalArticulos;
    ui->kpiAntiguedadMediaValor->setText(QString("%1 días").arg(QString::number(diasMedios, 'f', 0)));
    ui->kpiAntiguedadMediaSub->setText(QString("Inactivos desde hace > %1 días").arg(dias));

    ui->kpiValorVentaValor->setText(QString("%1 €").arg(QString::number(sumaValorPVP, 'f', 2)));
    ui->kpiValorVentaSub->setText(QString("Margen potencial: %1 €")
                                     .arg(QString::number(sumaValorPVP - sumaInmovilizado, 'f', 2)));
  } else {
    ui->kpiCapitalInmovilizadoValor->setText("0,00 €");
    ui->kpiArticulosAfectadosValor->setText("0 referencias");
    ui->kpiArticulosAfectadosSub->setText("0 unidades");
    ui->kpiAntiguedadMediaValor->setText("0 días");
    ui->kpiAntiguedadMediaSub->setText("Sin stock inactivo detectado");
    ui->kpiValorVentaValor->setText("0,00 €");
    ui->kpiValorVentaSub->setText("Todo el catálogo tiene rotación");
  }
}

/**
 * @brief Carga la tabla de artículos con stock y más de N días sin ventas.
 */
void DialogEstadisticasStockMuerto::cargarTablaStockMuerto(int dias, int idFamilia, int limite, int idTienda) {
  if (modeloStockMuerto) {
    delete modeloStockMuerto;
  }
  modeloStockMuerto = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "Código", "Descripción del Artículo", "Familia",
                           "Stock", "Coste", "PVP", "Capital Inmovilizado",
                           "Última Venta", "Días Sin Venta", "Acción Sugerida"};
  modeloStockMuerto->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasStockMuertoNube(dias, idFamilia, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    QString fam = query.value("nombre_familia").toString();
    double stock = query.value("stock_actual").toDouble();
    double coste = query.value("coste").toDouble();
    double pvp = query.value("pvp").toDouble();
    double inmovilizado = query.value("capital_inmovilizado").toDouble();
    QString fechaUlt = query.value("fecha_ultima_venta").toString();
    int diasSinVenta = query.value("dias_sin_venta").toInt();

    // Determinar acción sugerida
    QString accion;
    QColor colorAccion;
    if (diasSinVenta >= 365) {
      accion = "🚨 Liquidar a Coste";
      colorAccion = QColor(180, 40, 40);
    } else if (diasSinVenta >= 180) {
      accion = "🔥 Liquidación (-40%)";
      colorAccion = QColor(200, 80, 20);
    } else if (diasSinVenta >= 90) {
      accion = "🏷️ Descuento (-25%)";
      colorAccion = QColor(180, 120, 20);
    } else if (diasSinVenta >= 60) {
      accion = "📢 Promoción (-15%)";
      colorAccion = QColor(60, 120, 180);
    } else {
      accion = "👀 Revisar Exposición";
      colorAccion = QColor(100, 100, 100);
    }

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

    NumericStandardItem *iStock = new NumericStandardItem(stock, 0);
    fila << iStock;

    NumericStandardItem *iCoste = new NumericStandardItem(coste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iPvp = new NumericStandardItem(pvp, 2, "€");
    fila << iPvp;

    NumericStandardItem *iInmov = new NumericStandardItem(inmovilizado, 2, "€");
    iInmov->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iInmov;

    QStandardItem *iFecha = new QStandardItem(fechaUlt);
    iFecha->setTextAlignment(Qt::AlignCenter);
    fila << iFecha;

    NumericStandardItem *iDias = new NumericStandardItem(diasSinVenta, 0, "días");
    fila << iDias;

    QStandardItem *iAccion = new QStandardItem(accion);
    iAccion->setTextAlignment(Qt::AlignCenter);
    iAccion->setForeground(QBrush(colorAccion));
    fila << iAccion;

    modeloStockMuerto->appendRow(fila);
  }

  ui->tablaStockMuerto->setModel(modeloStockMuerto);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
  ui->tablaStockMuerto->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga los artículos con stock físico que nunca han tenido ninguna venta registrada.
 */
void DialogEstadisticasStockMuerto::cargarTablaNuncaVendidos(int idFamilia, int limite, int idTienda) {
  if (modeloNuncaVendidos) {
    delete modeloNuncaVendidos;
  }
  modeloNuncaVendidos = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "Código", "Descripción", "Familia", "Stock",
                           "Coste Unit.", "PVP", "Capital Inmovilizado", "Valoración PVP", "Diagnóstico"};
  modeloNuncaVendidos->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasStockNuncaVendidoNube(idFamilia, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    QString fam = query.value("nombre_familia").toString();
    double stock = query.value("stock_actual").toDouble();
    double coste = query.value("coste").toDouble();
    double pvp = query.value("pvp").toDouble();
    double inmovilizado = query.value("capital_inmovilizado").toDouble();
    double valorPvp = query.value("valor_venta").toDouble();

    QString diag;
    if (pvp <= coste && coste > 0) {
      diag = "⚠️ PVP <= Coste (Revisar margen)";
    } else if (stock >= 20) {
      diag = "📦 Sobrestock sin rotar";
    } else {
      diag = "🔍 Sin ventas registradas";
    }

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

    NumericStandardItem *iStock = new NumericStandardItem(stock, 0);
    fila << iStock;

    NumericStandardItem *iCoste = new NumericStandardItem(coste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iPvp = new NumericStandardItem(pvp, 2, "€");
    fila << iPvp;

    NumericStandardItem *iInmov = new NumericStandardItem(inmovilizado, 2, "€");
    iInmov->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iInmov;

    NumericStandardItem *iValPvp = new NumericStandardItem(valorPvp, 2, "€");
    fila << iValPvp;

    QStandardItem *iDiag = new QStandardItem(diag);
    iDiag->setTextAlignment(Qt::AlignCenter);
    fila << iDiag;

    modeloNuncaVendidos->appendRow(fila);
  }

  ui->tablaNuncaVendidos->setModel(modeloNuncaVendidos);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaNuncaVendidos->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga el inmovilizado agrupado por familias.
 */
void DialogEstadisticasStockMuerto::cargarTablaInmovilizadoFamilias(int dias, int idTienda) {
  if (modeloInmovilizadoFamilias) {
    delete modeloInmovilizadoFamilias;
  }
  modeloInmovilizadoFamilias = new QStandardItemModel(this);

  QStringList cabeceras = {"ID", "Familia / Categoría", "Artículos Inactivos", "Unidades en Stock",
                           "Capital Inmovilizado", "% Cuota Inmovilizado", "Valoración PVP Teórica"};
  modeloInmovilizadoFamilias->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasInmovilizadoPorFamiliaNube(dias, idTienda);

  struct FilaFamInmov {
    int id;
    QString nombre;
    int arts;
    double uds;
    double inmov;
    double pvpTeorico;
  };
  QList<FilaFamInmov> lista;
  double sumaInmovTotal = 0.0;

  while (query.next()) {
    FilaFamInmov f;
    f.id = query.value("id_familia").toInt();
    f.nombre = query.value("nombre_familia").toString();
    f.arts = query.value("articulos_afectados").toInt();
    f.uds = query.value("unidades_inmovilizadas").toDouble();
    f.inmov = query.value("capital_inmovilizado").toDouble();
    f.pvpTeorico = query.value("valor_venta_potencial").toDouble();
    sumaInmovTotal += f.inmov;
    lista.append(f);
  }

  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    NumericStandardItem *iId = new NumericStandardItem(item.id, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(item.nombre);
    fila << iNom;

    NumericStandardItem *iArts = new NumericStandardItem(item.arts, 0);
    fila << iArts;

    NumericStandardItem *iUds = new NumericStandardItem(item.uds, 0);
    fila << iUds;

    NumericStandardItem *iInmov = new NumericStandardItem(item.inmov, 2, "€");
    iInmov->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iInmov;

    double cuota = (sumaInmovTotal > 0) ? (item.inmov / sumaInmovTotal) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iPvp = new NumericStandardItem(item.pvpTeorico, 2, "€");
    fila << iPvp;

    modeloInmovilizadoFamilias->appendRow(fila);
  }

  ui->tablaInmovilizadoFamilias->setModel(modeloInmovilizadoFamilias);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaInmovilizadoFamilias->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga el simulador de liquidación calculando la liquidez neta recuperable según el descuento elegido.
 */
void DialogEstadisticasStockMuerto::cargarTablaSimulador(int dias, int idFamilia, int limite, int idTienda) {
  if (modeloLiquidacion) {
    delete modeloLiquidacion;
  }
  modeloLiquidacion = new QStandardItemModel(this);

  QStringList cabeceras = {"Código", "Descripción", "Stock", "Coste Unitario",
                           "PVP Original", "PVP Liquidación", "Margen Unitario", "Caja Recuperable Total"};
  modeloLiquidacion->setHorizontalHeaderLabels(cabeceras);

  int pctDescuento = ui->comboDescuento->currentData().toInt();
  QSqlQuery query = base.estadisticasStockMuertoNube(dias, idFamilia, limite, idTienda);

  double sumaCajaRecuperable = 0.0;

  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    double stock = query.value("stock_actual").toDouble();
    double coste = query.value("coste").toDouble();
    double pvpOriginal = query.value("pvp").toDouble();

    double pvpLiquidacion = 0.0;
    if (pctDescuento == 0) {
      // Venta directa a coste
      pvpLiquidacion = coste;
    } else {
      pvpLiquidacion = pvpOriginal * (1.0 - (pctDescuento / 100.0));
    }

    double margenUnit = pvpLiquidacion - coste;
    double cajaArticulo = stock * pvpLiquidacion;
    sumaCajaRecuperable += cajaArticulo;

    QList<QStandardItem *> fila;

    QStandardItem *iCod = new QStandardItem(cod);
    iCod->setTextAlignment(Qt::AlignCenter);
    fila << iCod;

    QStandardItem *iDesc = new QStandardItem(desc);
    fila << iDesc;

    NumericStandardItem *iStock = new NumericStandardItem(stock, 0);
    fila << iStock;

    NumericStandardItem *iCoste = new NumericStandardItem(coste, 2, "€");
    fila << iCoste;

    NumericStandardItem *iPvpOrig = new NumericStandardItem(pvpOriginal, 2, "€");
    fila << iPvpOrig;

    NumericStandardItem *iPvpLiq = new NumericStandardItem(pvpLiquidacion, 2, "€");
    iPvpLiq->setForeground(QBrush(QColor(16, 124, 65)));
    fila << iPvpLiq;

    NumericStandardItem *iMargen = new NumericStandardItem(margenUnit, 2, "€");
    if (margenUnit < 0) {
      iMargen->setForeground(QBrush(QColor(180, 40, 40)));
    }
    fila << iMargen;

    NumericStandardItem *iCaja = new NumericStandardItem(cajaArticulo, 2, "€");
    fila << iCaja;

    modeloLiquidacion->appendRow(fila);
  }

  ui->labelCajaRecuperableValor->setText(QString("%1 €").arg(QString::number(sumaCajaRecuperable, 'f', 2)));

  ui->tablaLiquidacion->setModel(modeloLiquidacion);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaLiquidacion->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
}

void DialogEstadisticasStockMuerto::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasStockMuerto::on_tabWidgetStockMuerto_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    cargarPestana(index);
    pestanasPendientes[index] = false;
    QApplication::restoreOverrideCursor();
  }
}

void DialogEstadisticasStockMuerto::on_comboDescuento_currentIndexChanged(int /*index*/) {
  if (ui->tabWidgetStockMuerto->currentIndex() == 3) {
    int dias = obtenerDiasSeleccionados();
    int idFamilia = obtenerIdFamiliaSeleccionada();
    int limite = obtenerLimiteSeleccionado();
    int idTienda = obtenerIdTiendaSeleccionada();
    cargarTablaSimulador(dias, idFamilia, limite, idTienda);
  }
}

void DialogEstadisticasStockMuerto::on_btnExportarCSV_clicked() {
  int tabIdx = ui->tabWidgetStockMuerto->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  QString sufijo = "stock_muerto";

  switch (tabIdx) {
  case 0: modeloActual = modeloStockMuerto; sufijo = "stock_inactivo"; break;
  case 1: modeloActual = modeloNuncaVendidos; sufijo = "nunca_vendidos"; break;
  case 2: modeloActual = modeloInmovilizadoFamilias; sufijo = "inmovilizado_familias"; break;
  case 3: modeloActual = modeloLiquidacion; sufijo = "simulacion_liquidacion"; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Exportar a CSV", "No hay datos para exportar en la pestaña actual.");
    return;
  }

  QString nombreSugerido = QString("estadisticas_%1_%2dias.csv")
                               .arg(sufijo)
                               .arg(obtenerDiasSeleccionados());

  QString ruta = QFileDialog::getSaveFileName(this, "Guardar estadísticas como CSV", nombreSugerido, "Archivos CSV (*.csv)");
  if (ruta.isEmpty()) return;

  QFile archivo(ruta);
  if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::critical(this, "Error al Exportar", "No se pudo escribir en el archivo:\n" + archivo.errorString());
    return;
  }

  QTextStream out(&archivo);

  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    out << "\"" << modeloActual->headerData(c, Qt::Horizontal).toString() << "\"";
    if (c < modeloActual->columnCount() - 1) out << ";";
  }
  out << "\n";

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
  QMessageBox::information(this, "Exportación Completada", "Datos exportados correctamente a:\n" + ruta);
}

void DialogEstadisticasStockMuerto::on_btnCopiarTabla_clicked() {
  int tabIdx = ui->tabWidgetStockMuerto->currentIndex();
  QStandardItemModel *modeloActual = nullptr;

  switch (tabIdx) {
  case 0: modeloActual = modeloStockMuerto; break;
  case 1: modeloActual = modeloNuncaVendidos; break;
  case 2: modeloActual = modeloInmovilizadoFamilias; break;
  case 3: modeloActual = modeloLiquidacion; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Copiar al Portapapeles", "No hay datos en la pestaña activa para copiar.");
    return;
  }

  QString buffer;

  for (int c = 0; c < modeloActual->columnCount(); ++c) {
    buffer += modeloActual->headerData(c, Qt::Horizontal).toString();
    if (c < modeloActual->columnCount() - 1) buffer += "\t";
  }
  buffer += "\n";

  for (int r = 0; r < modeloActual->rowCount(); ++r) {
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      QStandardItem *item = modeloActual->item(r, c);
      buffer += item ? item->text() : "";
      if (c < modeloActual->columnCount() - 1) buffer += "\t";
    }
    buffer += "\n";
  }

  QApplication::clipboard()->setText(buffer);
  QMessageBox::information(this, "Copiado al Portapapeles", "La tabla activa se ha copiado al portapapeles.");
}
