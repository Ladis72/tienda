#include "dialogestadisticasclientes.h"
#include "ui_dialogestadisticasclientes.h"
#include "clientes.h"
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

DialogEstadisticasClientes::DialogEstadisticasClientes(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasClientes) {
  ui->setupUi(this);

  // Inicializar fechas: desde el 1 de enero del año actual hasta hoy
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);

  // Inicializar punteros a modelos
  modeloRankingVIP = nullptr;
  modeloRecurrencia = nullptr;
  modeloRiesgoFuga = nullptr;
  modeloLocalidades = nullptr;

  // Configurar filtros
  inicializarFiltros();

  // Conectar doble clic en tablas para abrir directamente la ficha del cliente
  connect(ui->tablaRankingVIP, &QTableView::doubleClicked, this, &DialogEstadisticasClientes::onTablaDobleClick);
  connect(ui->tablaRecurrencia, &QTableView::doubleClicked, this, &DialogEstadisticasClientes::onTablaDobleClick);
  connect(ui->tablaRiesgoFuga, &QTableView::doubleClicked, this, &DialogEstadisticasClientes::onTablaDobleClick);
}

DialogEstadisticasClientes::~DialogEstadisticasClientes() {
  delete modeloRankingVIP;
  delete modeloRecurrencia;
  delete modeloRiesgoFuga;
  delete modeloLocalidades;
  delete ui;
}

void DialogEstadisticasClientes::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarEstadisticas();
}

/**
 * @brief Rellena los desplegables de tiendas, segmentación y límites.
 */
void DialogEstadisticasClientes::inicializarFiltros() {
  ui->comboTienda->blockSignals(true);
  ui->comboSegmento->blockSignals(true);
  ui->comboLimite->blockSignals(true);

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

  // 2. Segmentación de clientes
  ui->comboSegmento->clear();
  ui->comboSegmento->addItem("⭐ Solo Clientes Fidelizados (Registrados)", 1);
  ui->comboSegmento->addItem("👥 Todos los Clientes (incluye Mostrador)", 0);
  ui->comboSegmento->setCurrentIndex(0); // Clientes registrados por defecto

  // 3. Límites de ranking
  ui->comboLimite->clear();
  ui->comboLimite->addItem("Top 25", 25);
  ui->comboLimite->addItem("Top 50", 50);
  ui->comboLimite->addItem("Top 100", 100);
  ui->comboLimite->addItem("Top 250", 250);
  ui->comboLimite->addItem("Todos (sin límite)", 0);
  ui->comboLimite->setCurrentIndex(2); // Top 100 por defecto

  ui->comboTienda->blockSignals(false);
  ui->comboSegmento->blockSignals(false);
  ui->comboLimite->blockSignals(false);
}

int DialogEstadisticasClientes::obtenerIdTiendaSeleccionada() const {
  return ui->comboTienda->currentData().toInt();
}

bool DialogEstadisticasClientes::obtenerSoloRegistrados() const {
  return ui->comboSegmento->currentData().toInt() == 1;
}

int DialogEstadisticasClientes::obtenerLimiteSeleccionado() const {
  return ui->comboLimite->currentData().toInt();
}

/**
 * @brief Obtiene el ID del cliente seleccionado en la fila activa de la pestaña visible.
 */
int DialogEstadisticasClientes::obtenerIdClienteSeleccionado() const {
  int tab = ui->tabWidgetClientes->currentIndex();
  QTableView *tabla = nullptr;
  int colId = 1;

  switch (tab) {
  case 0: tabla = ui->tablaRankingVIP; colId = 1; break;
  case 1: tabla = ui->tablaRecurrencia; colId = 1; break;
  case 2: tabla = ui->tablaRiesgoFuga; colId = 0; break;
  default: return -1;
  }

  if (!tabla || !tabla->selectionModel() || !tabla->selectionModel()->hasSelection()) {
    return -1;
  }

  QModelIndex idx = tabla->selectionModel()->selectedRows().value(0);
  if (!idx.isValid()) return -1;

  QModelIndex idxId = tabla->model()->index(idx.row(), colId);
  return idxId.data(Qt::UserRole).isValid() ? idxId.data(Qt::UserRole).toInt() : idxId.data(Qt::DisplayRole).toInt();
}

/**
 * @brief Recarga la información analítica de clientes.
 */
void DialogEstadisticasClientes::cargarEstadisticas() {
  QSqlDatabase dbNube = base.obtenerConexionNube();
  if (!dbNube.isValid() || !dbNube.isOpen()) {
    QMessageBox::warning(
        this, "Aviso Conexión Nube",
        "La base de datos central en la nube no está disponible.\n"
        "El análisis analítico de clientes requiere conexión activa a la nube.");
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  QElapsedTimer cronometro;
  cronometro.start();

  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  int idTienda = obtenerIdTiendaSeleccionada();

  // 1. Cargar KPIs superiores
  cargarKPIs(desde, hasta, idTienda);

  // 2. Marcar pestañas para recarga bajo demanda
  for (int i = 0; i < ui->tabWidgetClientes->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar inmediatamente la pestaña activa
  int activa = ui->tabWidgetClientes->currentIndex();
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
void DialogEstadisticasClientes::cargarPestana(int index) {
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  int idTienda = obtenerIdTiendaSeleccionada();
  bool soloRegistrados = obtenerSoloRegistrados();
  int limite = obtenerLimiteSeleccionado();

  switch (index) {
  case 0:
    cargarTablaRankingVIP(desde, hasta, soloRegistrados, limite, idTienda);
    break;
  case 1:
    cargarTablaRecurrencia(desde, hasta, limite, idTienda);
    break;
  case 2:
    cargarTablaRiesgoFuga(60, limite, idTienda);
    break;
  case 3:
    cargarTablaLocalidades(desde, hasta, idTienda);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula y visualiza las tarjetas KPI superiores.
 */
void DialogEstadisticasClientes::cargarKPIs(const QDate &desde, const QDate &hasta, int idTienda) {
  // 1. Ventas a clientes registrados vs global
  QSqlQuery qTotal = base.estadisticasRankingClientesNube(desde, hasta, false, 0, idTienda);
  double totalVentasGlobal = 0.0;
  double totalVentasFidelizadas = 0.0;
  int totalTicketsGlobal = 0;
  int totalTicketsFidelizados = 0;

  QString mejorCliente = "Sin datos";
  double maxCompraCliente = 0.0;
  int visitasMejorCliente = 0;

  while (qTotal.next()) {
    int idCli = qTotal.value("id_cliente").toInt();
    double totalCli = qTotal.value("total_compras").toDouble();
    int ticketsCli = qTotal.value("num_tickets").toInt();

    totalVentasGlobal += totalCli;
    totalTicketsGlobal += ticketsCli;

    if (idCli > 1) {
      totalVentasFidelizadas += totalCli;
      totalTicketsFidelizados += ticketsCli;

      if (totalCli > maxCompraCliente) {
        maxCompraCliente = totalCli;
        mejorCliente = qTotal.value("nombre_completo").toString();
        visitasMejorCliente = ticketsCli;
      }
    }
  }

  // Tarjeta 1: Ventas Fidelizadas
  if (totalVentasGlobal > 0) {
    double cuotaFidelizada = (totalVentasFidelizadas / totalVentasGlobal) * 100.0;
    ui->kpiVentasFidelizadasValor->setText(QString("%1 €").arg(QString::number(totalVentasFidelizadas, 'f', 2)));
    ui->kpiVentasFidelizadasSub->setText(QString("%1% cuota fidelizada (%2 € global)")
                                            .arg(QString::number(cuotaFidelizada, 'f', 1))
                                            .arg(QString::number(totalVentasGlobal, 'f', 2)));
  } else {
    ui->kpiVentasFidelizadasValor->setText("0,00 €");
    ui->kpiVentasFidelizadasSub->setText("0% cuota fidelización");
  }

  // Tarjeta 2: Cliente VIP / Mayor Compra
  if (maxCompraCliente > 0) {
    ui->kpiClienteEstrellaValor->setText(mejorCliente);
    ui->kpiClienteEstrellaValor->setToolTip(mejorCliente);
    ui->kpiClienteEstrellaSub->setText(QString("%1 € acumulados · %2 compras")
                                           .arg(QString::number(maxCompraCliente, 'f', 2))
                                           .arg(visitasMejorCliente));
  } else {
    ui->kpiClienteEstrellaValor->setText("Sin compras");
    ui->kpiClienteEstrellaSub->setText("0,00 € · 0 visitas");
  }

  // Tarjeta 3: Ticket Medio Cliente Fidelizado vs Mostrador
  if (totalTicketsFidelizados > 0) {
    double tmFidelizado = totalVentasFidelizadas / totalTicketsFidelizados;
    double tmGlobal = (totalTicketsGlobal > 0) ? (totalVentasGlobal / totalTicketsGlobal) : 0.0;
    ui->kpiTicketMedioFidelizadoValor->setText(QString("%1 €").arg(QString::number(tmFidelizado, 'f', 2)));
    ui->kpiTicketMedioFidelizadoSub->setText(QString("Ticket medio global: %1 €")
                                                .arg(QString::number(tmGlobal, 'f', 2)));
  } else {
    ui->kpiTicketMedioFidelizadoValor->setText("--");
    ui->kpiTicketMedioFidelizadoSub->setText("Sin visitas registradas");
  }

  // Tarjeta 4: Clientes en Riesgo de Fuga (>60 días sin compra)
  QSqlQuery qRiesgo = base.estadisticasClientesEnRiesgoNube(60, 0, idTienda);
  int enRiesgo = 0;
  double inmovRiesgo = 0.0;
  while (qRiesgo.next()) {
    enRiesgo++;
    inmovRiesgo += qRiesgo.value("total_historico").toDouble();
  }
  ui->kpiClientesRiesgoValor->setText(QString("%1 clientes").arg(enRiesgo));
  ui->kpiClientesRiesgoSub->setText(QString("%1 € valor histórico en riesgo")
                                        .arg(QString::number(inmovRiesgo, 'f', 2)));
}

/**
 * @brief Carga el ranking de clientes por volumen de compras acumuladas.
 */
void DialogEstadisticasClientes::cargarTablaRankingVIP(const QDate &desde, const QDate &hasta,
                                                       bool soloRegistrados, int limite, int idTienda) {
  if (modeloRankingVIP) {
    delete modeloRankingVIP;
  }
  modeloRankingVIP = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "ID", "Cliente / Razón Social", "Teléfono", "Localidad",
                           "Total Compras", "Nº Visitas", "Ticket Medio", "Última Compra", "Días Inactivo"};
  modeloRankingVIP->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasRankingClientesNube(desde, hasta, soloRegistrados, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    int idCli = query.value("id_cliente").toInt();
    QString nombre = query.value("nombre_completo").toString();
    QString tel = query.value("telefono").toString();
    QString loc = query.value("localidad").toString();
    double total = query.value("total_compras").toDouble();
    int tickets = query.value("num_tickets").toInt();
    double tMedio = query.value("ticket_medio").toDouble();
    QString ultFecha = query.value("ultima_compra").toString();
    int diasInactivo = query.value("dias_inactivo").toInt();

    QList<QStandardItem *> fila;

    NumericStandardItem *iPos = new NumericStandardItem(pos++, 0);
    iPos->setTextAlignment(Qt::AlignCenter);
    fila << iPos;

    NumericStandardItem *iId = new NumericStandardItem(idCli, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(nombre);
    fila << iNom;

    QStandardItem *iTel = new QStandardItem(tel);
    iTel->setTextAlignment(Qt::AlignCenter);
    fila << iTel;

    QStandardItem *iLoc = new QStandardItem(loc);
    fila << iLoc;

    NumericStandardItem *iTotal = new NumericStandardItem(total, 2, "€");
    fila << iTotal;

    NumericStandardItem *iTickets = new NumericStandardItem(tickets, 0);
    fila << iTickets;

    NumericStandardItem *iTMedio = new NumericStandardItem(tMedio, 2, "€");
    fila << iTMedio;

    QStandardItem *iUlt = new QStandardItem(ultFecha);
    iUlt->setTextAlignment(Qt::AlignCenter);
    fila << iUlt;

    NumericStandardItem *iDias = new NumericStandardItem(diasInactivo, 0, "días");
    fila << iDias;

    modeloRankingVIP->appendRow(fila);
  }

  ui->tablaRankingVIP->setModel(modeloRankingVIP);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaRankingVIP->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga las estadísticas de recurrencia y frecuencia de visitas.
 */
void DialogEstadisticasClientes::cargarTablaRecurrencia(const QDate &desde, const QDate &hasta,
                                                        int limite, int idTienda) {
  if (modeloRecurrencia) {
    delete modeloRecurrencia;
  }
  modeloRecurrencia = new QStandardItemModel(this);

  QStringList cabeceras = {"#", "ID", "Cliente / Razón Social", "Teléfono",
                           "Total Visitas", "Total Comprado", "Ticket Medio",
                           "Primera Compra", "Última Compra", "Frecuencia Media"};
  modeloRecurrencia->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasRecurrenciaClientesNube(desde, hasta, limite, idTienda);

  int pos = 1;
  while (query.next()) {
    int idCli = query.value("id_cliente").toInt();
    QString nombre = query.value("nombre_completo").toString();
    QString tel = query.value("telefono").toString();
    int visitas = query.value("total_visitas").toInt();
    double total = query.value("total_compras").toDouble();
    double tMedio = query.value("ticket_medio").toDouble();
    QString primera = query.value("primera_compra").toString();
    QString ultima = query.value("ultima_compra").toString();
    double freqDias = query.value("dias_entre_visitas").toDouble();

    QList<QStandardItem *> fila;

    NumericStandardItem *iPos = new NumericStandardItem(pos++, 0);
    iPos->setTextAlignment(Qt::AlignCenter);
    fila << iPos;

    NumericStandardItem *iId = new NumericStandardItem(idCli, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(nombre);
    fila << iNom;

    QStandardItem *iTel = new QStandardItem(tel);
    iTel->setTextAlignment(Qt::AlignCenter);
    fila << iTel;

    NumericStandardItem *iVis = new NumericStandardItem(visitas, 0);
    fila << iVis;

    NumericStandardItem *iTotal = new NumericStandardItem(total, 2, "€");
    fila << iTotal;

    NumericStandardItem *iTMedio = new NumericStandardItem(tMedio, 2, "€");
    fila << iTMedio;

    QStandardItem *iPrim = new QStandardItem(primera);
    iPrim->setTextAlignment(Qt::AlignCenter);
    fila << iPrim;

    QStandardItem *iUlt = new QStandardItem(ultima);
    iUlt->setTextAlignment(Qt::AlignCenter);
    fila << iUlt;

    NumericStandardItem *iFreq = new NumericStandardItem(freqDias, 1, "días/visita");
    fila << iFreq;

    modeloRecurrencia->appendRow(fila);
  }

  ui->tablaRecurrencia->setModel(modeloRecurrencia);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(2, QHeaderView::Stretch);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaRecurrencia->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga los clientes en riesgo de abandono (> 60 días inactivos).
 */
void DialogEstadisticasClientes::cargarTablaRiesgoFuga(int dias, int limite, int idTienda) {
  if (modeloRiesgoFuga) {
    delete modeloRiesgoFuga;
  }
  modeloRiesgoFuga = new QStandardItemModel(this);

  QStringList cabeceras = {"ID", "Cliente / Razón Social", "Teléfono", "Email", "Localidad",
                           "Tickets Históricos", "Compras Históricas", "Última Compra",
                           "Días Sin Comprar", "Alerta Retención"};
  modeloRiesgoFuga->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasClientesEnRiesgoNube(dias, limite, idTienda);

  while (query.next()) {
    int idCli = query.value("id_cliente").toInt();
    QString nombre = query.value("nombre_completo").toString();
    QString tel = query.value("telefono").toString();
    QString mail = query.value("mail").toString();
    QString loc = query.value("localidad").toString();
    int ticketsHist = query.value("total_tickets_historicos").toInt();
    double totalHist = query.value("total_historico").toDouble();
    QString ultFecha = query.value("ultima_compra").toString();
    int diasInactivo = query.value("dias_inactivo").toInt();

    QString alerta;
    QColor colorAlerta;
    if (diasInactivo >= 180) {
      alerta = "🚨 Fuga Consolidada (>6 meses)";
      colorAlerta = QColor(180, 40, 40);
    } else if (diasInactivo >= 90) {
      alerta = "⚠️ Alto Riesgo (>3 meses)";
      colorAlerta = QColor(200, 80, 20);
    } else {
      alerta = "👀 Riesgo Moderado (>60 días)";
      colorAlerta = QColor(180, 140, 20);
    }

    QList<QStandardItem *> fila;

    NumericStandardItem *iId = new NumericStandardItem(idCli, 0);
    iId->setTextAlignment(Qt::AlignCenter);
    fila << iId;

    QStandardItem *iNom = new QStandardItem(nombre);
    fila << iNom;

    QStandardItem *iTel = new QStandardItem(tel);
    iTel->setTextAlignment(Qt::AlignCenter);
    fila << iTel;

    QStandardItem *iMail = new QStandardItem(mail);
    fila << iMail;

    QStandardItem *iLoc = new QStandardItem(loc);
    fila << iLoc;

    NumericStandardItem *iTick = new NumericStandardItem(ticketsHist, 0);
    fila << iTick;

    NumericStandardItem *iTotal = new NumericStandardItem(totalHist, 2, "€");
    fila << iTotal;

    QStandardItem *iUlt = new QStandardItem(ultFecha);
    iUlt->setTextAlignment(Qt::AlignCenter);
    fila << iUlt;

    NumericStandardItem *iDias = new NumericStandardItem(diasInactivo, 0, "días");
    fila << iDias;

    QStandardItem *iAlerta = new QStandardItem(alerta);
    iAlerta->setTextAlignment(Qt::AlignCenter);
    iAlerta->setForeground(QBrush(colorAlerta));
    fila << iAlerta;

    modeloRiesgoFuga->appendRow(fila);
  }

  ui->tablaRiesgoFuga->setModel(modeloRiesgoFuga);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
  ui->tablaRiesgoFuga->horizontalHeader()->setSectionResizeMode(9, QHeaderView::ResizeToContents);
}

/**
 * @brief Carga la distribución geográfica de ventas por localidad.
 */
void DialogEstadisticasClientes::cargarTablaLocalidades(const QDate &desde, const QDate &hasta, int idTienda) {
  if (modeloLocalidades) {
    delete modeloLocalidades;
  }
  modeloLocalidades = new QStandardItemModel(this);

  QStringList cabeceras = {"Localidad / Municipio", "Clientes Únicos", "Total Tickets",
                           "Facturación Total", "% Cuota Ventas", "Ticket Medio"};
  modeloLocalidades->setHorizontalHeaderLabels(cabeceras);

  QSqlQuery query = base.estadisticasClientesPorLocalidadNube(desde, hasta, idTienda);

  struct FilaLoc {
    QString nombre;
    int clientes;
    int tickets;
    double ventas;
    double tMedio;
  };
  QList<FilaLoc> lista;
  double sumaVentas = 0.0;

  while (query.next()) {
    FilaLoc f;
    f.nombre = query.value("localidad").toString();
    f.clientes = query.value("total_clientes").toInt();
    f.tickets = query.value("total_tickets").toInt();
    f.ventas = query.value("total_ventas").toDouble();
    f.tMedio = query.value("ticket_medio").toDouble();
    sumaVentas += f.ventas;
    lista.append(f);
  }

  for (const auto &item : lista) {
    QList<QStandardItem *> fila;

    QStandardItem *iNom = new QStandardItem(item.nombre);
    fila << iNom;

    NumericStandardItem *iCli = new NumericStandardItem(item.clientes, 0);
    fila << iCli;

    NumericStandardItem *iTick = new NumericStandardItem(item.tickets, 0);
    fila << iTick;

    NumericStandardItem *iVentas = new NumericStandardItem(item.ventas, 2, "€");
    fila << iVentas;

    double cuota = (sumaVentas > 0) ? (item.ventas / sumaVentas) * 100.0 : 0.0;
    NumericStandardItem *iCuota = new NumericStandardItem(cuota, 2, "%");
    fila << iCuota;

    NumericStandardItem *iTMedio = new NumericStandardItem(item.tMedio, 2, "€");
    fila << iTMedio;

    modeloLocalidades->appendRow(fila);
  }

  ui->tablaLocalidades->setModel(modeloLocalidades);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
  ui->tablaLocalidades->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
}

void DialogEstadisticasClientes::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasClientes::on_tabWidgetClientes_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    QApplication::setOverrideCursor(Qt::WaitCursor);
    cargarPestana(index);
    pestanasPendientes[index] = false;
    QApplication::restoreOverrideCursor();
  }
}

// Filtros de fecha rápidos
void DialogEstadisticasClientes::on_btnHoy_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(hoy);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasClientes::on_btnSemana_clicked() {
  QDate hoy = QDate::currentDate();
  QDate inicioSemana = hoy.addDays(-(hoy.dayOfWeek() - 1));
  ui->dateDesde->setDate(inicioSemana);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasClientes::on_btnMes_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasClientes::on_btnAno_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

/**
 * @brief Abre la ficha del cliente seleccionado.
 */
void DialogEstadisticasClientes::on_btnVerFicha_clicked() {
  int idCliente = obtenerIdClienteSeleccionado();
  if (idCliente <= 1) {
    QMessageBox::information(this, "Ficha de Cliente", "Por favor, seleccione un cliente registrado en la tabla.");
    return;
  }

  Clientes *ficha = new Clientes(this, QString::number(idCliente));
  ficha->exec();
  delete ficha;
}

void DialogEstadisticasClientes::onTablaDobleClick(const QModelIndex & /*index*/) {
  on_btnVerFicha_clicked();
}

void DialogEstadisticasClientes::on_btnExportarCSV_clicked() {
  int tabIdx = ui->tabWidgetClientes->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  QString sufijo = "clientes";

  switch (tabIdx) {
  case 0: modeloActual = modeloRankingVIP; sufijo = "top_clientes_vip"; break;
  case 1: modeloActual = modeloRecurrencia; sufijo = "recurrencia_clientes"; break;
  case 2: modeloActual = modeloRiesgoFuga; sufijo = "clientes_riesgo_fuga"; break;
  case 3: modeloActual = modeloLocalidades; sufijo = "distribucion_localidades"; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Exportar a CSV", "No hay datos disponibles en la tabla activa.");
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

void DialogEstadisticasClientes::on_btnCopiarTabla_clicked() {
  int tabIdx = ui->tabWidgetClientes->currentIndex();
  QStandardItemModel *modeloActual = nullptr;

  switch (tabIdx) {
  case 0: modeloActual = modeloRankingVIP; break;
  case 1: modeloActual = modeloRecurrencia; break;
  case 2: modeloActual = modeloRiesgoFuga; break;
  case 3: modeloActual = modeloLocalidades; break;
  }

  if (!modeloActual || modeloActual->rowCount() == 0) {
    QMessageBox::information(this, "Copiar al Portapapeles", "No hay datos para copiar en la tabla activa.");
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
