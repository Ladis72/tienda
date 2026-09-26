#include "dialogestadisticasmermas.h"
#include "ui_dialogestadisticasmermas.h"
#include "configuracion.h"
#include "estadisticas_utils.h"
#include "syncmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTextStream>

extern Configuracion *conf;

DialogEstadisticasMermas::DialogEstadisticasMermas(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasMermas) {
  ui->setupUi(this);

  // Inicializar fechas: desde el primer día del mes actual hasta la fecha actual
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);

  // Inicializar punteros a modelos en memoria
  modeloRankingMermas = nullptr;
  modeloMotivos = nullptr;
  modeloTiendas = nullptr;
  modeloFamilias = nullptr;

  // Configurar filtros
  inicializarFiltros();
}

DialogEstadisticasMermas::~DialogEstadisticasMermas() {
  delete modeloRankingMermas;
  delete modeloMotivos;
  delete modeloTiendas;
  delete modeloFamilias;
  delete ui;
}

void DialogEstadisticasMermas::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarEstadisticas();
}

/**
 * @brief Rellena los desplegables de filtros.
 */
void DialogEstadisticasMermas::inicializarFiltros() {
  ui->comboTienda->blockSignals(true);
  ui->comboFamilia->blockSignals(true);
  ui->comboTipoSalida->blockSignals(true);

  // 1. Selector de tienda
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

  // 2. Familias
  ui->comboFamilia->clear();
  ui->comboFamilia->addItem("📂 Todas las Familias", -1);
  QSqlQuery qFam = base.listaFamiliasNube();
  while (qFam.next()) {
    int idFam = qFam.value(0).toInt();
    QString nombreFam = qFam.value(1).toString();
    ui->comboFamilia->addItem(nombreFam, idFam);
  }

  // 3. Tipo de merma
  ui->comboTipoSalida->clear();
  ui->comboTipoSalida->addItem("⚠️ Todas las Mermas (Caducados + Roturas)", 0);
  ui->comboTipoSalida->addItem("📅 Solo Caducados", 1);
  ui->comboTipoSalida->addItem("💔 Solo Roturas y Desperfectos", 2);
  ui->comboTipoSalida->setCurrentIndex(0); // Todas las mermas por defecto

  ui->comboTienda->blockSignals(false);
  ui->comboFamilia->blockSignals(false);
  ui->comboTipoSalida->blockSignals(false);
}

int DialogEstadisticasMermas::obtenerIdTiendaSeleccionada() const {
  return ui->comboTienda->currentData().toInt();
}

int DialogEstadisticasMermas::obtenerIdFamiliaSeleccionada() const {
  return ui->comboFamilia->currentData().toInt();
}

int DialogEstadisticasMermas::obtenerTipoMermaSeleccionado() const {
  return ui->comboTipoSalida->currentData().toInt();
}

/**
 * @brief Recarga la información analítica de mermas y desperdicios.
 */
void DialogEstadisticasMermas::cargarEstadisticas() {
  QSqlDatabase dbNube = base.obtenerConexionNube();
  QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
  if ((!dbNube.isValid() || !dbNube.isOpen()) && (!dbLocal.isValid() || !dbLocal.isOpen())) {
    QMessageBox::warning(
        this, "Aviso de Conexión",
        "No se pudo conectar con la base de datos central en la nube ni con la base de datos local.");
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
  int tipoMerma = obtenerTipoMermaSeleccionado();

  // 1. Cargar KPIs superiores
  cargarKPIs(desde, hasta, idFamilia, tipoMerma, idTienda);

  // 2. Marcar pestañas para recarga bajo demanda
  for (int i = 0; i < ui->tabWidgetMermas->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar la pestaña activa de inmediato
  int activa = ui->tabWidgetMermas->currentIndex();
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
void DialogEstadisticasMermas::cargarPestana(int index) {
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  int idTienda = obtenerIdTiendaSeleccionada();
  int idFamilia = obtenerIdFamiliaSeleccionada();
  int tipoMerma = obtenerTipoMermaSeleccionado();

  switch (index) {
  case 0:
    cargarTablaRankingMermas(desde, hasta, idFamilia, tipoMerma, 100, idTienda);
    break;
  case 1:
    cargarTablaMotivos(desde, hasta, idTienda);
    break;
  case 2:
    cargarTablaTiendas(desde, hasta);
    break;
  case 3:
    cargarTablaFamilias(desde, hasta, idTienda);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula y visualiza las tarjetas KPI superiores.
 */
void DialogEstadisticasMermas::cargarKPIs(const QDate &desde, const QDate &hasta,
                                         int idFamilia, int tipoMerma, int idTienda) {
  QSqlQuery query = base.estadisticasRankingMermasNube(desde, hasta, idFamilia, tipoMerma, 0, idTienda);

  double sumaCostePerdida = 0.0;
  double sumaUnidades = 0.0;
  QString peorProducto = "Sin mermas";
  double maxPerdidaProd = 0.0;
  double udsPeorProd = 0.0;

  bool primero = true;
  while (query.next()) {
    QString desc = query.value("descripcion").toString();
    double uds = query.value("unidades_mermadas").toDouble();
    double coste = query.value("coste_total_perdida").toDouble();

    sumaCostePerdida += coste;
    sumaUnidades += uds;

    if (primero || coste > maxPerdidaProd) {
      maxPerdidaProd = coste;
      peorProducto = desc;
      udsPeorProd = uds;
      primero = false;
    }
  }

  // Tarjeta 1: Coste Financiero Total de Pérdidas
  ui->kpiCostePerdidasValor->setText(QString("%1 €").arg(QString::number(sumaCostePerdida, 'f', 2)));
  ui->kpiCostePerdidasSub->setText("Pérdida neta a precio de coste");

  // Tarjeta 2: Unidades Físicas Perdidas
  ui->kpiUnidadesMermadasValor->setText(QString("%1 uds").arg(QString::number(sumaUnidades, 'f', 0)));
  ui->kpiUnidadesMermadasSub->setText(QString("En el período analizado"));

  // Tarjeta 3: Artículo con Mayor Merma
  if (!primero && maxPerdidaProd > 0) {
    ui->kpiArticuloMayorMermaValor->setText(peorProducto);
    ui->kpiArticuloMayorMermaValor->setToolTip(peorProducto);
    ui->kpiArticuloMayorMermaSub->setText(QString("%1 € coste · %2 uds")
                                              .arg(QString::number(maxPerdidaProd, 'f', 2))
                                              .arg(QString::number(udsPeorProd, 'f', 0)));
  } else {
    ui->kpiArticuloMayorMermaValor->setText("Sin mermas registradas");
    ui->kpiArticuloMayorMermaSub->setText("0,00 €");
  }

  // Tarjeta 4: Tienda con Mayor Pérdida
  QSqlQuery qTiendas = base.estadisticasMermasPorTiendaNube(desde, hasta);
  QString peorTienda = "Sin datos";
  double maxPerdidaTienda = 0.0;
  double totalCadenaMermas = 0.0;

  bool primeraTienda = true;
  while (qTiendas.next()) {
    QString nomT = qTiendas.value("nombre_tienda").toString();
    double costeT = qTiendas.value("coste_total").toDouble();
    totalCadenaMermas += costeT;

    if (primeraTienda || costeT > maxPerdidaTienda) {
      maxPerdidaTienda = costeT;
      peorTienda = nomT;
      primeraTienda = false;
    }
  }

  if (!primeraTienda && totalCadenaMermas > 0) {
    double cuotaTienda = (maxPerdidaTienda / totalCadenaMermas) * 100.0;
    ui->kpiTiendaMayorMermaValor->setText(peorTienda);
    ui->kpiTiendaMayorMermaSub->setText(QString("%1 € (%2% de la cadena)")
                                            .arg(QString::number(maxPerdidaTienda, 'f', 2))
                                            .arg(QString::number(cuotaTienda, 'f', 1)));
  } else {
    ui->kpiTiendaMayorMermaValor->setText("Sin datos");
    ui->kpiTiendaMayorMermaSub->setText("0,00 € · 0%");
  }
}

/**
 * @brief Carga el ranking de productos con mayores mermas y pérdidas.
 */
void DialogEstadisticasMermas::cargarTablaRankingMermas(const QDate &desde, const QDate &hasta,
                                                        int idFamilia, int tipoMerma, int limite, int idTienda) {
  if (modeloRankingMermas) {
    delete modeloRankingMermas;
  }
  modeloRankingMermas = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "Código", "Descripción del Artículo", "Familia", "Causa",
                           "Uds. Mermadas", "Coste Unit.", "PVP Unit.",
                           "Coste Total Pérdida", "Valoración PVP", "Última Fecha"};
  modeloRankingMermas->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasRankingMermasNube(desde, hasta, idFamilia, tipoMerma, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    QString cod = query.value("cod").toString();
    QString desc = query.value("descripcion").toString();
    QString fam = query.value("nombre_familia").toString();
    QString causa = query.value("motivo_grupo").toString();
    double uds = query.value("unidades_mermadas").toDouble();
    double costeUnit = query.value("coste_unitario").toDouble();
    double pvpUnit = query.value("pvp_unitario").toDouble();
    double costePerdida = query.value("coste_total_perdida").toDouble();
    double pvpPerdida = query.value("pvp_total_perdida").toDouble();
    QString ultFecha = query.value("ultima_fecha").toString();

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

    QStandardItem *iCausa = new QStandardItem(causa);
    iCausa->setTextAlignment(Qt::AlignCenter);
    if (causa == "Caducidad") {
      iCausa->setForeground(QBrush(QColor(180, 50, 50)));
    } else {
      iCausa->setForeground(QBrush(QColor(180, 100, 20)));
    }
    fila << iCausa;

    NumericStandardItem *iUds = new NumericStandardItem(uds, 0);
    fila << iUds;

    NumericStandardItem *iCosteU = new NumericStandardItem(costeUnit, 2, "€");
    fila << iCosteU;

    NumericStandardItem *iPvpU = new NumericStandardItem(pvpUnit, 2, "€");
    fila << iPvpU;

    NumericStandardItem *iCosteT = new NumericStandardItem(costePerdida, 2, "€");
    iCosteT->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iCosteT;

    NumericStandardItem *iPvpT = new NumericStandardItem(pvpPerdida, 2, "€");
    fila << iPvpT;

    QStandardItem *iFecha = new QStandardItem(ultFecha);
    iFecha->setTextAlignment(Qt::AlignCenter);
    fila << iFecha;

    modeloRankingMermas->appendRow(fila);
  }

  ui->tablaRankingMermas->setModel(modeloRankingMermas);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
  ui->tablaRankingMermas->horizontalHeader()->setSectionResizeMode(10, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga el desglose de mermas por concepto o motivo de salida.
 */
void DialogEstadisticasMermas::cargarTablaMotivos(const QDate &desde, const QDate &hasta, int idTienda) {
  if (modeloMotivos) {
    delete modeloMotivos;
  }
  modeloMotivos = new QStandardItemModel(this);

  QStringList cabeceras = {"Concepto / Motivo de Salida", "Registros", "Unidades Totales",
                           "Coste Total Pérdida", "% Cuota Pérdida", "Valoración PVP Total"};
  modeloMotivos->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasMermasPorMotivoNube(desde, hasta, idTienda);

  struct FilaMotivo {
    QString motivo;
    int reg;
    double uds;
    double coste;
    double pvp;
  };
  QList<FilaMotivo> lista;
  double sumaCosteTotal = 0.0;

  while (query.next()) {
    FilaMotivo f;
    f.motivo = query.value("motivo").toString();
    f.reg = query.value("registros").toInt();
    f.uds = query.value("unidades_totales").toDouble();
    f.coste = query.value("coste_total").toDouble();
    f.pvp = query.value("pvp_total").toDouble();
    sumaCosteTotal += f.coste;
    lista.append(f);
  }

  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    QStandardItem *iMot = new QStandardItem(item.motivo);
    fila << iMot;

    NumericStandardItem *iReg = new NumericStandardItem(item.reg, 0);
    fila << iReg;

    NumericStandardItem *iUds = new NumericStandardItem(item.uds, 0);
    fila << iUds;

    NumericStandardItem *iCoste = new NumericStandardItem(item.coste, 2, "€");
    iCoste->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iCoste;

    double cuota = (sumaCosteTotal > 0) ? (item.coste / sumaCosteTotal) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iPvp = new NumericStandardItem(item.pvp, 2, "€");
    fila << iPvp;

    modeloMotivos->appendRow(fila);
  }

  ui->tablaMotivos->setModel(modeloMotivos);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaMotivos->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga la comparativa de mermas entre tiendas físicas.
 */
void DialogEstadisticasMermas::cargarTablaTiendas(const QDate &desde, const QDate &hasta) {
  if (modeloTiendas) {
    delete modeloTiendas;
  }
  modeloTiendas = new QStandardItemModel(this);

  QStringList cabeceras = {"ID", "Nombre de Tienda", "Registros", "Unidades Perdidas",
                           "Coste Total Pérdida", "% Cuota Cadena", "Valoración PVP"};
  modeloTiendas->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasMermasPorTiendaNube(desde, hasta);

  struct FilaTienda {
    int id;
    QString nombre;
    int reg;
    double uds;
    double coste;
    double pvp;
  };
  QList<FilaTienda> lista;
  double sumaCosteTotal = 0.0;

  while (query.next()) {
    FilaTienda f;
    f.id = query.value("id_tienda").toInt();
    f.nombre = query.value("nombre_tienda").toString();
    f.reg = query.value("registros").toInt();
    f.uds = query.value("unidades_totales").toDouble();
    f.coste = query.value("coste_total").toDouble();
    f.pvp = query.value("pvp_total").toDouble();
    sumaCosteTotal += f.coste;
    lista.append(f);
  }

  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    NumericStandardItem *iId = new NumericStandardItem(item.id, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(item.nombre);
    fila << iNom;

    NumericStandardItem *iReg = new NumericStandardItem(item.reg, 0);
    fila << iReg;

    NumericStandardItem *iUds = new NumericStandardItem(item.uds, 0);
    fila << iUds;

    NumericStandardItem *iCoste = new NumericStandardItem(item.coste, 2, "€");
    iCoste->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iCoste;

    double cuota = (sumaCosteTotal > 0) ? (item.coste / sumaCosteTotal) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iPvp = new NumericStandardItem(item.pvp, 2, "€");
    fila << iPvp;

    modeloTiendas->appendRow(fila);
  }

  ui->tablaTiendas->setModel(modeloTiendas);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaTiendas->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga las pérdidas acumuladas por familias de productos.
 */
void DialogEstadisticasMermas::cargarTablaFamilias(const QDate &desde, const QDate &hasta, int idTienda) {
  if (modeloFamilias) {
    delete modeloFamilias;
  }
  modeloFamilias = new QStandardItemModel(this);

  QStringList cabeceras = {"ID", "Familia / Categoría", "Artículos Afectados", "Unidades Perdidas",
                           "Coste Total Pérdida", "% Cuota Mermas", "Valoración PVP"};
  modeloFamilias->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasMermasPorFamiliaNube(desde, hasta, idTienda);

  struct FilaFam {
    int id;
    QString nombre;
    int arts;
    double uds;
    double coste;
    double pvp;
  };
  QList<FilaFam> lista;
  double sumaCosteTotal = 0.0;

  while (query.next()) {
    FilaFam f;
    f.id = query.value("id_familia").toInt();
    f.nombre = query.value("nombre_familia").toString();
    f.arts = query.value("articulos_afectados").toInt();
    f.uds = query.value("unidades_totales").toDouble();
    f.coste = query.value("coste_total").toDouble();
    f.pvp = query.value("pvp_total").toDouble();
    sumaCosteTotal += f.coste;
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

    NumericStandardItem *iCoste = new NumericStandardItem(item.coste, 2, "€");
    iCoste->setForeground(QBrush(QColor(180, 40, 40)));
    fila << iCoste;

    double cuota = (sumaCosteTotal > 0) ? (item.coste / sumaCosteTotal) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iPvp = new NumericStandardItem(item.pvp, 2, "€");
    fila << iPvp;

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
}

void DialogEstadisticasMermas::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasMermas::on_tabWidgetMermas_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    cargarPestana(index);
    pestanasPendientes[index] = false;
    QApplication::restoreOverrideCursor();
  }
}

// Filtros rápidos de fechas
void DialogEstadisticasMermas::on_btnHoy_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(hoy);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasMermas::on_btnSemana_clicked() {
  QDate hoy = QDate::currentDate();
  QDate inicioSemana = hoy.addDays(-(hoy.dayOfWeek() - 1));
  ui->dateDesde->setDate(inicioSemana);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasMermas::on_btnMes_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasMermas::on_btnAno_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasMermas::on_btnExportarCSV_clicked() {
  int tabIdx = ui->tabWidgetMermas->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  QString sufijo = "mermas";

  switch (tabIdx) {
  case 0: modeloActual = modeloRankingMermas; sufijo = "ranking_mermas"; break;
  case 1: modeloActual = modeloMotivos; sufijo = "mermas_por_motivo"; break;
  case 2: modeloActual = modeloTiendas; sufijo = "mermas_por_tienda"; break;
  case 3: modeloActual = modeloFamilias; sufijo = "mermas_por_familia"; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Exportar a CSV", "No hay datos para exportar en la tabla activa.");
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

void DialogEstadisticasMermas::on_btnCopiarTabla_clicked() {
  int tabIdx = ui->tabWidgetMermas->currentIndex();
  QStandardItemModel *modeloActual = nullptr;

  switch (tabIdx) {
  case 0: modeloActual = modeloRankingMermas; break;
  case 1: modeloActual = modeloMotivos; break;
  case 2: modeloActual = modeloTiendas; break;
  case 3: modeloActual = modeloFamilias; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Copiar al Portapapeles", "No hay datos en la tabla activa para copiar.");
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
