#include "dialogestadisticasafluencia.h"
#include "ui_dialogestadisticasafluencia.h"
#include "configuracion.h"
#include "syncmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTableWidgetItem>
#include <QTextStream>

extern Configuracion *conf;

DialogEstadisticasAfluencia::DialogEstadisticasAfluencia(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasAfluencia) {
  ui->setupUi(this);

  // Modo normal por defecto (solo tabla tickets)
  modoConsolidado = false;

  // Fechas iniciales: mes en curso hasta hoy (formato obligatorio "yyyy-MM-dd")
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);

  // Modelos de memoria desacoplados
  modeloHoras = nullptr;
  modeloDias = nullptr;

  // Gestor de conexiones remotas
  conexiones = new conexionesRemotas(this);
  conexiones->base = &base;

  // Cargar combo de tiendas bloqueando señales para evitar llamadas tempranas
  ui->comboTienda->blockSignals(true);
  cargarTiendas();
  ui->comboTienda->blockSignals(false);
}

DialogEstadisticasAfluencia::~DialogEstadisticasAfluencia() {
  delete modeloHoras;
  delete modeloDias;
  delete ui;
}

/**
 * @brief Captura la tecla F2 para alternar entre tickets normales y consolidado (tickets+ticketss).
 */
void DialogEstadisticasAfluencia::keyPressEvent(QKeyEvent *event) {
  if (event->key() == Qt::Key_F2) {
    modoConsolidado = !modoConsolidado;
    actualizarEstadoUI();
    cargarEstadisticas();
    event->accept();
    return;
  }
  QDialog::keyPressEvent(event);
}

/**
 * @brief Al mostrar la ventana, refresca la lista de tiendas y carga los datos.
 */
void DialogEstadisticasAfluencia::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  ui->comboTienda->blockSignals(true);
  cargarTiendas();
  ui->comboTienda->blockSignals(false);
  cargarEstadisticas();
}

/**
 * @brief Pobla el desplegable de tiendas disponibles: Nube Global, Local y Remotas.
 */
void DialogEstadisticasAfluencia::cargarTiendas() {
  ui->comboTienda->clear();

  bool nubeDisponible = QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
                        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen();

  // 1. Ámbito Global en la Nube
  if (nubeDisponible) {
    QVariantMap dataGlobal;
    dataGlobal["tipo"] = static_cast<int>(TipoOrigenAfluencia::NubeGlobal);
    dataGlobal["idTienda"] = -1;
    dataGlobal["nombre"] = "Todas las tiendas (Global Nube)";
    ui->comboTienda->addItem("🌐 Todas las tiendas (Consolidado Global)", dataGlobal);
  }

  // 2. Tienda Local
  QString localConn = conf ? conf->getConexionLocal() : "DB";
  int idLocal = conf ? conf->getIdTienda() : 1;
  QString nombreLocal = "Tienda Local";
  if (QSqlDatabase::database(localConn).isOpen()) {
    QSqlQuery qNom(QSqlDatabase::database(localConn));
    if (qNom.exec("SELECT nombre FROM tiendas WHERE local = 1 LIMIT 1") && qNom.next()) {
      nombreLocal = qNom.value(0).toString();
    }
    QVariantMap dataLocal;
    dataLocal["tipo"] = static_cast<int>(TipoOrigenAfluencia::Local);
    dataLocal["idTienda"] = idLocal;
    dataLocal["conn"] = localConn;
    dataLocal["nombre"] = nombreLocal;
    ui->comboTienda->addItem("🏠 " + nombreLocal + " (Local)", dataLocal);
  }

  // 3. Tiendas remotas individuales
  if (nubeDisponible) {
    QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    QSqlQuery qTiendas(dbNube);
    if (qTiendas.exec(QString("SELECT id, nombre FROM tiendas WHERE id != %1 ORDER BY id").arg(idLocal))) {
      while (qTiendas.next()) {
        int idT = qTiendas.value(0).toInt();
        QString nomT = qTiendas.value(1).toString();
        QVariantMap dataRemota;
        dataRemota["tipo"] = static_cast<int>(TipoOrigenAfluencia::NubeTienda);
        dataRemota["idTienda"] = idT;
        dataRemota["nombre"] = nomT;
        ui->comboTienda->addItem("☁️ " + nomT + " [Nube]", dataRemota);
      }
    }
  } else if (conf) {
    QStringList activas = conf->getNombreConexionesActivas();
    for (const QString &tienda : activas) {
      if (tienda != localConn && QSqlDatabase::database(tienda).isOpen()) {
        QVariantMap dataRemota;
        dataRemota["tipo"] = static_cast<int>(TipoOrigenAfluencia::RemotoDirecto);
        dataRemota["conn"] = tienda;
        dataRemota["nombre"] = tienda;
        dataRemota["idTienda"] = 0;
        ui->comboTienda->addItem("🔗 " + tienda + " [Remoto]", dataRemota);
      }
    }
  }

  if (ui->comboTienda->count() > 0) {
    ui->comboTienda->setCurrentIndex(0);
  }
}

/**
 * @brief Obtiene los datos del origen seleccionado actualmente en el combo.
 */
InfoOrigenAfluencia DialogEstadisticasAfluencia::obtenerOrigenActual() {
  InfoOrigenAfluencia info;
  info.tipo = TipoOrigenAfluencia::Local;
  info.idTienda = conf ? conf->getIdTienda() : 1;
  info.nombreConexion = conf ? conf->getConexionLocal() : "DB";
  info.nombreTienda = "Tienda Local";

  if (ui->comboTienda->count() == 0) return info;

  QVariant dataVar = ui->comboTienda->currentData();
  if (dataVar.canConvert<QVariantMap>()) {
    QVariantMap map = dataVar.toMap();
    info.tipo = static_cast<TipoOrigenAfluencia>(map.value("tipo").toInt());
    info.idTienda = map.value("idTienda", -1).toInt();
    info.nombreConexion = map.value("conn", conf ? conf->getConexionLocal() : "DB").toString();
    info.nombreTienda = map.value("nombre", ui->comboTienda->currentText()).toString();
  }
  return info;
}

/**
 * @brief Actualiza las etiquetas de fuente y modo en la cabecera.
 */
void DialogEstadisticasAfluencia::actualizarEstadoUI() {
  InfoOrigenAfluencia origen = obtenerOrigenActual();
  QString txtOrigen;
  switch (origen.tipo) {
  case TipoOrigenAfluencia::NubeGlobal:
    txtOrigen = "Fuente: 🌐 Nube Central (Consolidado Global)";
    break;
  case TipoOrigenAfluencia::NubeTienda:
    txtOrigen = QString("Fuente: ☁️ Nube Central (%1)").arg(origen.nombreTienda);
    break;
  case TipoOrigenAfluencia::Local:
    txtOrigen = QString("Fuente: 🏠 Base de Datos Local (%1)").arg(origen.nombreTienda);
    break;
  case TipoOrigenAfluencia::RemotoDirecto:
    txtOrigen = QString("Fuente: 🔗 Conexión Remota Directa (%1)").arg(origen.nombreTienda);
    break;
  }
  ui->labelOrigen->setText(txtOrigen);

  if (modoConsolidado) {
    ui->labelModoConsolidado->setText("Modo: <b>Consolidado [F2 activo]</b> (tickets + ticketss)");
    ui->labelModoConsolidado->setStyleSheet("color: #b71c1c; font-weight: bold;");
  } else {
    ui->labelModoConsolidado->setText("Modo: Normal [F2 para conmutar]");
    ui->labelModoConsolidado->setStyleSheet("color: #555555;");
  }
}

/**
 * @brief Realiza la recarga general de estadísticas de afluencia según filtros.
 */
void DialogEstadisticasAfluencia::cargarEstadisticas() {
  InfoOrigenAfluencia origen = obtenerOrigenActual();
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  // Verificación de conexión
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    QSqlDatabase dbNube = base.obtenerConexionNube();
    if (!dbNube.isValid() || !dbNube.isOpen()) {
      QMessageBox::warning(this, "Aviso Nube",
                           "La base de datos central en la nube no está abierta.\n"
                           "Seleccione la Tienda Local o revise la conexión.");
      return;
    }
  } else {
    if (!QSqlDatabase::database(origen.nombreConexion).isOpen()) {
      QMessageBox::warning(this, "Aviso Conexión",
                           QString("La conexión a '%1' no está abierta.").arg(origen.nombreTienda));
      return;
    }
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  QElapsedTimer cronometro;
  cronometro.start();

  actualizarEstadoUI();

  // 1. Cargar tarjetas KPI
  cargarKPIs(origen, desde, hasta);

  // 2. Marcar pestañas como pendientes
  for (int i = 0; i < ui->tabWidgetAfluencia->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar la pestaña activa inmediatamente
  int activa = ui->tabWidgetAfluencia->currentIndex();
  cargarPestana(activa);
  pestanasPendientes[activa] = false;

  qint64 tiempoMs = cronometro.elapsed();
  ui->labelTiempoConsulta->setText(QString("⚡ %1 ms").arg(tiempoMs));

  ui->btnActualizar->setEnabled(true);
  QApplication::restoreOverrideCursor();
}

/**
 * @brief Carga bajo demanda la pestaña indicada.
 */
void DialogEstadisticasAfluencia::cargarPestana(int index) {
  InfoOrigenAfluencia origen = obtenerOrigenActual();
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  switch (index) {
  case 0:
    cargarTablaHoras(origen, desde, hasta);
    break;
  case 1:
    cargarTablaDias(origen, desde, hasta);
    break;
  case 2:
    cargarMatrizDiaHora(origen, desde, hasta);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula los KPIs numéricos superiores: Pico horario, día más fuerte, ticket medio y total.
 */
void DialogEstadisticasAfluencia::cargarKPIs(const InfoOrigenAfluencia &origen, const QDate &desde,
                                            const QDate &hasta) {
  QSqlQuery qH;
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    qH = base.estadisticasVentasPorHoraNube(desde, hasta, origen.idTienda);
  } else {
    qH = base.estadisticasVentasPorHora(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  int picoHora = -1;
  int maxTicketsHora = 0;
  double ventasPicoHora = 0.0;
  int totalTickets = 0;
  double totalVentas = 0.0;

  while (qH.next()) {
    int h = qH.value(0).toInt();
    int t = qH.value(1).toInt();
    double v = qH.value(2).toDouble();

    totalTickets += t;
    totalVentas += v;

    if (t > maxTicketsHora) {
      maxTicketsHora = t;
      picoHora = h;
      ventasPicoHora = v;
    }
  }
  qH.finish();

  if (picoHora >= 0) {
    ui->kpiHoraPicoValor->setText(
        QString("%1:00 - %2:00").arg(picoHora, 2, 10, QChar('0')).arg((picoHora + 1) % 24, 2, 10, QChar('0')));
    ui->kpiHoraPicoSub->setText(
        QString("%L1 tickets · %L2 €").arg(maxTicketsHora).arg(ventasPicoHora, 0, 'f', 2));
  } else {
    ui->kpiHoraPicoValor->setText("--:--");
    ui->kpiHoraPicoSub->setText("Sin datos");
  }

  // Día más fuerte
  QSqlQuery qD;
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    qD = base.estadisticasVentasPorDiaSemanaNube(desde, hasta, origen.idTienda);
  } else {
    qD = base.estadisticasVentasPorDiaSemana(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  static const QStringList nombresDias = {
      "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};

  int picoDia = -1;
  int maxTicketsDia = 0;
  double ventasPicoDia = 0.0;

  while (qD.next()) {
    int d = qD.value(0).toInt();
    int t = qD.value(1).toInt();
    double v = qD.value(2).toDouble();

    if (v > ventasPicoDia) {
      ventasPicoDia = v;
      picoDia = d;
      maxTicketsDia = t;
    }
  }
  qD.finish();

  if (picoDia >= 0 && picoDia < nombresDias.size()) {
    ui->kpiDiaPicoValor->setText(nombresDias.at(picoDia));
    ui->kpiDiaPicoSub->setText(
        QString("%L1 tickets · %L2 €").arg(maxTicketsDia).arg(ventasPicoDia, 0, 'f', 2));
  } else {
    ui->kpiDiaPicoValor->setText("--");
    ui->kpiDiaPicoSub->setText("Sin datos");
  }

  // Ticket medio y total
  double ticketMedioGlobal = (totalTickets > 0) ? (totalVentas / totalTickets) : 0.0;
  double ticketMedioPico = (maxTicketsHora > 0) ? (ventasPicoHora / maxTicketsHora) : 0.0;

  ui->kpiTicketMedioValor->setText(QString("%L1 €").arg(ticketMedioGlobal, 0, 'f', 2));
  ui->kpiTicketMedioSub->setText(QString("En hora punta: %L1 €").arg(ticketMedioPico, 0, 'f', 2));

  ui->kpiTotalTicketsValor->setText(QString("%L1 Tickets").arg(totalTickets));
  ui->kpiTotalTicketsSub->setText(QString("Total: %L1 €").arg(totalVentas, 0, 'f', 2));
}

/**
 * @brief Carga las franjas horarias con tickets, ventas, porcentaje y ticket medio.
 */
void DialogEstadisticasAfluencia::cargarTablaHoras(const InfoOrigenAfluencia &origen,
                                                 const QDate &desde, const QDate &hasta) {
  delete modeloHoras;
  modeloHoras = new QStandardItemModel(this);
  modeloHoras->setHorizontalHeaderLabels(
      {"Franja Horaria", "Nº Tickets", "% Tickets", "Total Ventas €", "% Ventas", "Ticket Medio €"});

  QSqlQuery qTotal;
  double sumaVentas = 0.0;
  int sumaTickets = 0;

  QSqlQuery query;
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    query = base.estadisticasVentasPorHoraNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasVentasPorHora(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  // Almacenar temporalmente los datos para calcular porcentajes sobre el total
  struct FilaHora {
    int h;
    int tickets;
    double ventas;
  };
  QList<FilaHora> filas;

  while (query.next()) {
    FilaHora f;
    f.h = query.value(0).toInt();
    f.tickets = query.value(1).toInt();
    f.ventas = query.value(2).toDouble();
    sumaTickets += f.tickets;
    sumaVentas += f.ventas;
    filas.append(f);
  }
  query.finish();

  for (int i = 0; i < filas.size(); ++i) {
    const auto &f = filas.at(i);
    double pctTickets = (sumaTickets > 0) ? (f.tickets * 100.0 / sumaTickets) : 0.0;
    double pctVentas = (sumaVentas > 0.0) ? (f.ventas * 100.0 / sumaVentas) : 0.0;
    double ticketMedio = (f.tickets > 0) ? (f.ventas / f.tickets) : 0.0;

    QString franja = QString("%1:00 - %2:00")
                         .arg(f.h, 2, 10, QChar('0'))
                         .arg((f.h + 1) % 24, 2, 10, QChar('0'));
    QStandardItem *itFranja = new QStandardItem(franja);

    QStandardItem *itTickets = new QStandardItem();
    itTickets->setData(f.tickets, Qt::EditRole);
    itTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctTickets = new QStandardItem(QString("%1 %").arg(pctTickets, 0, 'f', 1));
    itPctTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(f.ventas, Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctVentas = new QStandardItem(QString("%1 %").arg(pctVentas, 0, 'f', 1));
    itPctVentas->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itMedio = new QStandardItem();
    itMedio->setData(ticketMedio, Qt::EditRole);
    itMedio->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloHoras->setItem(i, 0, itFranja);
    modeloHoras->setItem(i, 1, itTickets);
    modeloHoras->setItem(i, 2, itPctTickets);
    modeloHoras->setItem(i, 3, itTotal);
    modeloHoras->setItem(i, 4, itPctVentas);
    modeloHoras->setItem(i, 5, itMedio);
  }

  ui->tablaHoras->setModel(modeloHoras);
  ui->tablaHoras->resizeColumnsToContents();
}

/**
 * @brief Carga las ventas por día de la semana y comparativa laborables vs fin de semana.
 */
void DialogEstadisticasAfluencia::cargarTablaDias(const InfoOrigenAfluencia &origen,
                                                 const QDate &desde, const QDate &hasta) {
  delete modeloDias;
  modeloDias = new QStandardItemModel(this);
  modeloDias->setHorizontalHeaderLabels(
      {"Día de la Semana", "Nº Tickets", "% Tickets", "Total Ventas €", "% Ventas", "Ticket Medio €"});

  QSqlQuery query;
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    query = base.estadisticasVentasPorDiaSemanaNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasVentasPorDiaSemana(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  static const QStringList nombresDias = {
      "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo"};

  struct FilaDia {
    int d;
    int tickets;
    double ventas;
  };
  QList<FilaDia> filas;
  int sumaTickets = 0;
  double sumaVentas = 0.0;

  int ticketsLab = 0;
  double ventasLab = 0.0;
  int ticketsFinde = 0;
  double ventasFinde = 0.0;

  while (query.next()) {
    FilaDia f;
    f.d = query.value(0).toInt();
    f.tickets = query.value(1).toInt();
    f.ventas = query.value(2).toDouble();
    sumaTickets += f.tickets;
    sumaVentas += f.ventas;

    if (f.d >= 0 && f.d <= 4) { // Lunes a Viernes
      ticketsLab += f.tickets;
      ventasLab += f.ventas;
    } else { // Sábado y Domingo
      ticketsFinde += f.tickets;
      ventasFinde += f.ventas;
    }
    filas.append(f);
  }
  query.finish();

  for (int i = 0; i < filas.size(); ++i) {
    const auto &f = filas.at(i);
    double pctTickets = (sumaTickets > 0) ? (f.tickets * 100.0 / sumaTickets) : 0.0;
    double pctVentas = (sumaVentas > 0.0) ? (f.ventas * 100.0 / sumaVentas) : 0.0;
    double ticketMedio = (f.tickets > 0) ? (f.ventas / f.tickets) : 0.0;

    QString nomDia = (f.d >= 0 && f.d < nombresDias.size()) ? nombresDias.at(f.d) : QString("Día %1").arg(f.d);
    QStandardItem *itDia = new QStandardItem(nomDia);

    QStandardItem *itTickets = new QStandardItem();
    itTickets->setData(f.tickets, Qt::EditRole);
    itTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctTickets = new QStandardItem(QString("%1 %").arg(pctTickets, 0, 'f', 1));
    itPctTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itTotal = new QStandardItem();
    itTotal->setData(f.ventas, Qt::EditRole);
    itTotal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctVentas = new QStandardItem(QString("%1 %").arg(pctVentas, 0, 'f', 1));
    itPctVentas->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itMedio = new QStandardItem();
    itMedio->setData(ticketMedio, Qt::EditRole);
    itMedio->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloDias->setItem(i, 0, itDia);
    modeloDias->setItem(i, 1, itTickets);
    modeloDias->setItem(i, 2, itPctTickets);
    modeloDias->setItem(i, 3, itTotal);
    modeloDias->setItem(i, 4, itPctVentas);
    modeloDias->setItem(i, 5, itMedio);
  }

  ui->tablaDias->setModel(modeloDias);
  ui->tablaDias->resizeColumnsToContents();

  // Comparativa Laborables vs Fin de semana
  double pctLabT = (sumaTickets > 0) ? (ticketsLab * 100.0 / sumaTickets) : 0.0;
  double pctLabV = (sumaVentas > 0.0) ? (ventasLab * 100.0 / sumaVentas) : 0.0;
  double medioLab = (ticketsLab > 0) ? (ventasLab / ticketsLab) : 0.0;

  double pctFindeT = (sumaTickets > 0) ? (ticketsFinde * 100.0 / sumaTickets) : 0.0;
  double pctFindeV = (sumaVentas > 0.0) ? (ventasFinde * 100.0 / sumaVentas) : 0.0;
  double medioFinde = (ticketsFinde > 0) ? (ventasFinde / ticketsFinde) : 0.0;

  ui->labelComparativaLab->setText(
      QString("🏢 <b>Laborables (Lunes a Viernes):</b> %L1 tickets (%2%) · %L3 € (%4%) · Media: %L5 €")
          .arg(ticketsLab).arg(pctLabT, 0, 'f', 1).arg(ventasLab, 0, 'f', 2).arg(pctLabV, 0, 'f', 1).arg(medioLab, 0, 'f', 2));

  ui->labelComparativaFinde->setText(
      QString("🏖️ <b>Fin de Semana (Sáb-Dom):</b> %L1 tickets (%2%) · %L3 € (%4%) · Media: %L5 €")
          .arg(ticketsFinde).arg(pctFindeT, 0, 'f', 1).arg(ventasFinde, 0, 'f', 2).arg(pctFindeV, 0, 'f', 1).arg(medioFinde, 0, 'f', 2));
}

/**
 * @brief Carga la matriz cruzada de actividad Día de la semana x Franja Horaria (Heatmap).
 */
void DialogEstadisticasAfluencia::cargarMatrizDiaHora(const InfoOrigenAfluencia &origen,
                                                     const QDate &desde, const QDate &hasta) {
  QSqlQuery query;
  if (origen.tipo == TipoOrigenAfluencia::NubeGlobal ||
      origen.tipo == TipoOrigenAfluencia::NubeTienda) {
    query = base.estadisticasMatrizDiaHoraNube(desde, hasta, origen.idTienda);
  } else {
    query = base.estadisticasMatrizDiaHora(origen.nombreConexion, desde, hasta, modoConsolidado);
  }

  // Estructura para almacenar matriz [dia 0..6][hora 0..23]
  struct CeldaMatriz {
    int tickets = 0;
    double ventas = 0.0;
  };
  CeldaMatriz matriz[7][24];
  int maxTickets = 0;

  // Rango de horas activas observado
  int horaMin = 23;
  int horaMax = 0;

  while (query.next()) {
    int d = query.value(0).toInt();
    int h = query.value(1).toInt();
    int t = query.value(2).toInt();
    double v = query.value(3).toDouble();

    if (d >= 0 && d < 7 && h >= 0 && h < 24) {
      matriz[d][h].tickets = t;
      matriz[d][h].ventas = v;
      if (t > maxTickets) maxTickets = t;
      if (t > 0) {
        if (h < horaMin) horaMin = h;
        if (h > horaMax) horaMax = h;
      }
    }
  }
  query.finish();

  // Si no hay ventas, mostramos franja estándar 08:00 a 22:00
  if (horaMin > horaMax) {
    horaMin = 8;
    horaMax = 21;
  } else {
    horaMin = qMax(0, horaMin - 1);
    horaMax = qMin(23, horaMax + 1);
  }

  int numFilas = horaMax - horaMin + 1;
  ui->tablaMatriz->clear();
  ui->tablaMatriz->setRowCount(numFilas);
  ui->tablaMatriz->setColumnCount(9);

  static const QStringList cabeceras = {
      "Franja Horaria", "Lunes", "Martes", "Miércoles", "Jueves", "Viernes", "Sábado", "Domingo", "Total Franja"};
  ui->tablaMatriz->setHorizontalHeaderLabels(cabeceras);

  for (int r = 0; r < numFilas; ++r) {
    int h = horaMin + r;
    QString franja = QString("%1:00 - %2:00")
                         .arg(h, 2, 10, QChar('0'))
                         .arg((h + 1) % 24, 2, 10, QChar('0'));
    QTableWidgetItem *itemFranja = new QTableWidgetItem(franja);
    itemFranja->setTextAlignment(Qt::AlignCenter);
    itemFranja->setFont(QFont(font().family(), font().pointSize(), QFont::Bold));
    ui->tablaMatriz->setItem(r, 0, itemFranja);

    int totalTicketsFranja = 0;
    double totalVentasFranja = 0.0;

    for (int d = 0; d < 7; ++d) {
      int t = matriz[d][h].tickets;
      double v = matriz[d][h].ventas;
      totalTicketsFranja += t;
      totalVentasFranja += v;

      QTableWidgetItem *item = new QTableWidgetItem();
      item->setTextAlignment(Qt::AlignCenter);

      if (t > 0) {
        item->setText(QString::number(t));
        double medio = v / t;
        item->setToolTip(QString("Tickets: %1\nTotal Ventas: %2 €\nTicket Medio: %3 €")
                             .arg(t).arg(v, 0, 'f', 2).arg(medio, 0, 'f', 2));

        // Gradiente suave de calor basado en intensidad de afluencia
        double factor = (maxTickets > 0) ? ((double)t / (double)maxTickets) : 0.0;
        int red = 230 - static_cast<int>(factor * 140);
        int green = 245 - static_cast<int>(factor * 70);
        int blue = 255;
        item->setBackground(QColor(red, green, blue));
        if (factor > 0.6) {
          item->setForeground(Qt::white);
          QFont boldFont = item->font();
          boldFont.setBold(true);
          item->setFont(boldFont);
        }
      } else {
        item->setText("-");
        item->setForeground(QColor(180, 180, 180));
      }
      ui->tablaMatriz->setItem(r, d + 1, item);
    }

    // Columna Total Franja
    QTableWidgetItem *itemTotal = new QTableWidgetItem();
    itemTotal->setTextAlignment(Qt::AlignCenter);
    if (totalTicketsFranja > 0) {
      itemTotal->setText(QString("%1 tks (%2 €)")
                             .arg(totalTicketsFranja)
                             .arg(totalVentasFranja, 0, 'f', 0));
      itemTotal->setFont(QFont(font().family(), font().pointSize(), QFont::Bold));
    } else {
      itemTotal->setText("-");
    }
    ui->tablaMatriz->setItem(r, 8, itemTotal);
  }

  ui->tablaMatriz->resizeColumnsToContents();
}

void DialogEstadisticasAfluencia::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasAfluencia::on_comboTienda_currentIndexChanged(int) {
  cargarEstadisticas();
}

void DialogEstadisticasAfluencia::on_tabWidgetAfluencia_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    cargarPestana(index);
    pestanasPendientes[index] = false;
  }
}

// Filtros de fecha rápidos
void DialogEstadisticasAfluencia::on_btnHoy_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(hoy);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasAfluencia::on_btnSemana_clicked() {
  QDate hoy = QDate::currentDate();
  int diaSemana = hoy.dayOfWeek(); // 1=Lunes .. 7=Domingo
  QDate lunes = hoy.addDays(-(diaSemana - 1));
  ui->dateDesde->setDate(lunes);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasAfluencia::on_btnMes_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasAfluencia::on_btnAno_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

/**
 * @brief Exporta los datos de la pestaña activa a un fichero CSV con codificación UTF-8.
 */
void DialogEstadisticasAfluencia::on_btnExportarCSV_clicked() {
  QString ruta = QFileDialog::getSaveFileName(
      this, "Guardar estadísticas de afluencia en CSV", "afluencia_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv",
      "Archivos CSV (*.csv);;Todos los archivos (*)");
  if (ruta.isEmpty()) return;

  QFile archivo(ruta);
  if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error de Exportación", "No se pudo escribir en el archivo seleccionado.");
    return;
  }

  QTextStream out(&archivo);
  int tab = ui->tabWidgetAfluencia->currentIndex();

  if (tab == 0 && modeloHoras) {
    // Cabeceras
    QStringList headers;
    for (int c = 0; c < modeloHoras->columnCount(); ++c) {
      headers << modeloHoras->headerData(c, Qt::Horizontal).toString();
    }
    out << headers.join(";") << "\n";
    for (int r = 0; r < modeloHoras->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloHoras->columnCount(); ++c) {
        row << modeloHoras->item(r, c)->text();
      }
      out << row.join(";") << "\n";
    }
  } else if (tab == 1 && modeloDias) {
    QStringList headers;
    for (int c = 0; c < modeloDias->columnCount(); ++c) {
      headers << modeloDias->headerData(c, Qt::Horizontal).toString();
    }
    out << headers.join(";") << "\n";
    for (int r = 0; r < modeloDias->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloDias->columnCount(); ++c) {
        row << modeloDias->item(r, c)->text();
      }
      out << row.join(";") << "\n";
    }
  } else if (tab == 2) {
    QStringList headers;
    for (int c = 0; c < ui->tablaMatriz->columnCount(); ++c) {
      headers << ui->tablaMatriz->horizontalHeaderItem(c)->text();
    }
    out << headers.join(";") << "\n";
    for (int r = 0; r < ui->tablaMatriz->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < ui->tablaMatriz->columnCount(); ++c) {
        QTableWidgetItem *item = ui->tablaMatriz->item(r, c);
        row << (item ? item->text() : "");
      }
      out << row.join(";") << "\n";
    }
  }

  archivo.close();
  QMessageBox::information(this, "Exportación Completada", "Datos exportados correctamente a CSV.");
}

/**
 * @brief Copia el contenido tabular de la pestaña activa al portapapeles en formato TSV.
 */
void DialogEstadisticasAfluencia::on_btnCopiarTabla_clicked() {
  QString tsv;
  int tab = ui->tabWidgetAfluencia->currentIndex();

  if (tab == 0 && modeloHoras) {
    QStringList headers;
    for (int c = 0; c < modeloHoras->columnCount(); ++c) {
      headers << modeloHoras->headerData(c, Qt::Horizontal).toString();
    }
    tsv += headers.join("\t") + "\n";
    for (int r = 0; r < modeloHoras->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloHoras->columnCount(); ++c) {
        row << modeloHoras->item(r, c)->text();
      }
      tsv += row.join("\t") + "\n";
    }
  } else if (tab == 1 && modeloDias) {
    QStringList headers;
    for (int c = 0; c < modeloDias->columnCount(); ++c) {
      headers << modeloDias->headerData(c, Qt::Horizontal).toString();
    }
    tsv += headers.join("\t") + "\n";
    for (int r = 0; r < modeloDias->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloDias->columnCount(); ++c) {
        row << modeloDias->item(r, c)->text();
      }
      tsv += row.join("\t") + "\n";
    }
  } else if (tab == 2) {
    QStringList headers;
    for (int c = 0; c < ui->tablaMatriz->columnCount(); ++c) {
      headers << ui->tablaMatriz->horizontalHeaderItem(c)->text();
    }
    tsv += headers.join("\t") + "\n";
    for (int r = 0; r < ui->tablaMatriz->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < ui->tablaMatriz->columnCount(); ++c) {
        QTableWidgetItem *item = ui->tablaMatriz->item(r, c);
        row << (item ? item->text() : "");
      }
      tsv += row.join("\t") + "\n";
    }
  }

  QApplication::clipboard()->setText(tsv);
  QMessageBox::information(this, "Portapapeles", "Tabla copiada al portapapeles (lista para pegar en Excel o Calc).");
}
