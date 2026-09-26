#include "dialogestadisticastiendas.h"
#include "ui_dialogestadisticastiendas.h"
#include "syncmanager.h"
#include <QApplication>
#include <QClipboard>
#include <QElapsedTimer>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QTextStream>

DialogEstadisticasTiendas::DialogEstadisticasTiendas(QWidget *parent)
    : QDialog(parent), ui(new Ui::DialogEstadisticasTiendas) {
  ui->setupUi(this);

  // Fechas iniciales: desde el 1 del mes en curso hasta hoy (formato obligatorio "yyyy-MM-dd")
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);

  // Modelos desacoplados en memoria
  modeloComparativa = nullptr;
  modeloFormasPago = nullptr;
  modeloEvolucion = nullptr;

  // Agrupación temporal inicial
  ui->comboAgrupacion->setCurrentIndex(0); // Mes a Mes
}

DialogEstadisticasTiendas::~DialogEstadisticasTiendas() {
  delete modeloComparativa;
  delete modeloFormasPago;
  delete modeloEvolucion;
  delete ui;
}

void DialogEstadisticasTiendas::showEvent(QShowEvent *event) {
  QDialog::showEvent(event);
  cargarEstadisticas();
}

/**
 * @brief Recarga la información estadística multitienda.
 */
void DialogEstadisticasTiendas::cargarEstadisticas() {
  QSqlDatabase dbNube = base.obtenerConexionNube();
  if (!dbNube.isValid() || !dbNube.isOpen()) {
    QMessageBox::warning(
        this, "Aviso Conexión Nube",
        "La base de datos central en la nube no está disponible.\n"
        "La comparativa multitienda requiere conexión activa a la nube.");
    return;
  }

  QApplication::setOverrideCursor(Qt::WaitCursor);
  ui->btnActualizar->setEnabled(false);

  QElapsedTimer cronometro;
  cronometro.start();

  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  // 1. Cargar KPIs superiores
  cargarKPIs(desde, hasta);

  // 2. Marcar pestañas como pendientes de carga
  for (int i = 0; i < ui->tabWidgetTiendas->count(); ++i) {
    pestanasPendientes[i] = true;
  }

  // 3. Cargar la pestaña activa de inmediato
  int activa = ui->tabWidgetTiendas->currentIndex();
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
void DialogEstadisticasTiendas::cargarPestana(int index) {
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();

  switch (index) {
  case 0:
    cargarTablaComparativa(desde, hasta);
    break;
  case 1:
    cargarTablaFormasPago(desde, hasta);
    break;
  case 2:
    cargarTablaEvolucion(desde, hasta);
    break;
  default:
    break;
  }
}

/**
 * @brief Calcula los KPIs comparativos de la cabecera: líder de ventas, mayor afluencia, mayor ticket medio y total cadena.
 */
void DialogEstadisticasTiendas::cargarKPIs(const QDate &desde, const QDate &hasta) {
  QSqlQuery query = base.estadisticasComparativaTiendasNube(desde, hasta);

  double granTotalVentas = 0.0;
  int granTotalTickets = 0;

  QString tiendaLiderVentas = "--";
  double maxVentas = -1.0;
  int ticketsLider = 0;

  QString tiendaMayorAfluencia = "--";
  int maxTickets = -1;

  QString tiendaMayorTicketMedio = "--";
  double maxTicketMedio = -1.0;

  struct ResumenTiendaKPI {
    QString nombre;
    int tickets;
    double ventas;
    double ticketMedio;
  };
  QList<ResumenTiendaKPI> tiendas;

  while (query.next()) {
    ResumenTiendaKPI t;
    t.nombre = query.value(1).toString();
    t.tickets = query.value(2).toInt();
    t.ventas = query.value(3).toDouble();
    t.ticketMedio = (t.tickets > 0) ? (t.ventas / t.tickets) : 0.0;

    granTotalVentas += t.ventas;
    granTotalTickets += t.tickets;

    if (t.ventas > maxVentas) {
      maxVentas = t.ventas;
      tiendaLiderVentas = t.nombre;
      ticketsLider = t.tickets;
    }
    if (t.tickets > maxTickets) {
      maxTickets = t.tickets;
      tiendaMayorAfluencia = t.nombre;
    }
    if (t.ticketMedio > maxTicketMedio) {
      maxTicketMedio = t.ticketMedio;
      tiendaMayorTicketMedio = t.nombre;
    }

    tiendas.append(t);
  }
  query.finish();

  // Tienda Líder en Ventas
  if (maxVentas >= 0.0) {
    double cuotaLider = (granTotalVentas > 0.0) ? (maxVentas * 100.0 / granTotalVentas) : 0.0;
    ui->kpiLiderVentasValor->setText(tiendaLiderVentas);
    ui->kpiLiderVentasSub->setText(
        QString("%L1 € · %2% cuota").arg(maxVentas, 0, 'f', 2).arg(cuotaLider, 0, 'f', 1));
  } else {
    ui->kpiLiderVentasValor->setText("--");
    ui->kpiLiderVentasSub->setText("Sin datos");
  }

  // Mayor Afluencia
  if (maxTickets >= 0) {
    ui->kpiMayorAfluenciaValor->setText(tiendaMayorAfluencia);
    ui->kpiMayorAfluenciaSub->setText(QString("%L1 tickets emitidos").arg(maxTickets));
  } else {
    ui->kpiMayorAfluenciaValor->setText("--");
    ui->kpiMayorAfluenciaSub->setText("Sin datos");
  }

  // Mayor Ticket Medio
  if (maxTicketMedio >= 0.0) {
    ui->kpiMayorTicketMedioValor->setText(QString("%L1 €").arg(maxTicketMedio, 0, 'f', 2));
    ui->kpiMayorTicketMedioSub->setText(QString("Líder: %1").arg(tiendaMayorTicketMedio));
  } else {
    ui->kpiMayorTicketMedioValor->setText("--");
    ui->kpiMayorTicketMedioSub->setText("Sin datos");
  }

  // Total Cadena
  ui->kpiTotalCadenaValor->setText(QString("%L1 €").arg(granTotalVentas, 0, 'f', 2));
  ui->kpiTotalCadenaSub->setText(QString("%L1 tickets en toda la cadena").arg(granTotalTickets));
}

/**
 * @brief Carga la tabla principal de resumen comparativo entre tiendas.
 */
void DialogEstadisticasTiendas::cargarTablaComparativa(const QDate &desde, const QDate &hasta) {
  delete modeloComparativa;
  modeloComparativa = new QStandardItemModel(this);
  modeloComparativa->setHorizontalHeaderLabels({
      "Tienda", "Total Ventas €", "% Cuota Ventas", "Nº Tickets",
      "% Cuota Tickets", "Ticket Medio €", "Días Activos", "Venta Diaria Media €"});

  QSqlQuery query = base.estadisticasComparativaTiendasNube(desde, hasta);

  struct FilaTienda {
    int id;
    QString nombre;
    int tickets;
    double ventas;
    int dias;
  };
  QList<FilaTienda> filas;
  double sumaVentas = 0.0;
  int sumaTickets = 0;

  while (query.next()) {
    FilaTienda f;
    f.id = query.value(0).toInt();
    f.nombre = query.value(1).toString();
    f.tickets = query.value(2).toInt();
    f.ventas = query.value(3).toDouble();
    f.dias = query.value(4).toInt();

    sumaVentas += f.ventas;
    sumaTickets += f.tickets;
    filas.append(f);
  }
  query.finish();

  for (int i = 0; i < filas.size(); ++i) {
    const auto &f = filas.at(i);
    double pctVentas = (sumaVentas > 0.0) ? (f.ventas * 100.0 / sumaVentas) : 0.0;
    double pctTickets = (sumaTickets > 0) ? (f.tickets * 100.0 / sumaTickets) : 0.0;
    double ticketMedio = (f.tickets > 0) ? (f.ventas / f.tickets) : 0.0;
    double ventaDiaria = (f.dias > 0) ? (f.ventas / f.dias) : 0.0;

    QStandardItem *itNombre = new QStandardItem(f.nombre);

    QStandardItem *itVentas = new QStandardItem();
    itVentas->setData(f.ventas, Qt::EditRole);
    itVentas->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctV = new QStandardItem(QString("%1 %").arg(pctVentas, 0, 'f', 1));
    itPctV->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itTickets = new QStandardItem();
    itTickets->setData(f.tickets, Qt::EditRole);
    itTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itPctT = new QStandardItem(QString("%1 %").arg(pctTickets, 0, 'f', 1));
    itPctT->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itMedio = new QStandardItem();
    itMedio->setData(ticketMedio, Qt::EditRole);
    itMedio->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itDias = new QStandardItem();
    itDias->setData(f.dias, Qt::EditRole);
    itDias->setTextAlignment(Qt::AlignCenter);

    QStandardItem *itDiaria = new QStandardItem();
    itDiaria->setData(ventaDiaria, Qt::EditRole);
    itDiaria->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloComparativa->setItem(i, 0, itNombre);
    modeloComparativa->setItem(i, 1, itVentas);
    modeloComparativa->setItem(i, 2, itPctV);
    modeloComparativa->setItem(i, 3, itTickets);
    modeloComparativa->setItem(i, 4, itPctT);
    modeloComparativa->setItem(i, 5, itMedio);
    modeloComparativa->setItem(i, 6, itDias);
    modeloComparativa->setItem(i, 7, itDiaria);
  }

  ui->tablaComparativa->setModel(modeloComparativa);
  ui->tablaComparativa->resizeColumnsToContents();
}

/**
 * @brief Carga el desglose por formas de pago (efectivo vs tarjeta) por tienda.
 */
void DialogEstadisticasTiendas::cargarTablaFormasPago(const QDate &desde, const QDate &hasta) {
  delete modeloFormasPago;
  modeloFormasPago = new QStandardItemModel(this);
  modeloFormasPago->setHorizontalHeaderLabels({
      "Tienda", "Forma de Pago", "Modalidad", "Nº Tickets", "Total Ventas €", "Ticket Medio €"});

  QSqlQuery query = base.estadisticasFormasPagoPorTiendaNube(desde, hasta);

  int fila = 0;
  while (query.next()) {
    QString tienda = query.value(1).toString();
    QString formaPago = query.value(2).toString();
    bool esEfectivo = query.value(3).toInt() == 1;
    int tickets = query.value(4).toInt();
    double ventas = query.value(5).toDouble();
    double ticketMedio = (tickets > 0) ? (ventas / tickets) : 0.0;

    QStandardItem *itTienda = new QStandardItem(tienda);
    QStandardItem *itForma = new QStandardItem(formaPago);

    QString modalidad = esEfectivo ? "💵 Efectivo" : "💳 Tarjeta / Electrónico";
    QStandardItem *itModalidad = new QStandardItem(modalidad);

    QStandardItem *itTickets = new QStandardItem();
    itTickets->setData(tickets, Qt::EditRole);
    itTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itVentas = new QStandardItem();
    itVentas->setData(ventas, Qt::EditRole);
    itVentas->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itMedio = new QStandardItem();
    itMedio->setData(ticketMedio, Qt::EditRole);
    itMedio->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloFormasPago->setItem(fila, 0, itTienda);
    modeloFormasPago->setItem(fila, 1, itForma);
    modeloFormasPago->setItem(fila, 2, itModalidad);
    modeloFormasPago->setItem(fila, 3, itTickets);
    modeloFormasPago->setItem(fila, 4, itVentas);
    modeloFormasPago->setItem(fila, 5, itMedio);
    fila++;
  }
  query.finish();

  ui->tablaFormasPago->setModel(modeloFormasPago);
  ui->tablaFormasPago->resizeColumnsToContents();
}

/**
 * @brief Carga la evolución temporal comparada entre tiendas (mes, semana o día).
 */
void DialogEstadisticasTiendas::cargarTablaEvolucion(const QDate &desde, const QDate &hasta) {
  delete modeloEvolucion;
  modeloEvolucion = new QStandardItemModel(this);
  modeloEvolucion->setHorizontalHeaderLabels({
      "Período", "Tienda", "Nº Tickets", "Total Ventas €", "Ticket Medio €"});

  QString agrupacion = "mes";
  int idx = ui->comboAgrupacion->currentIndex();
  if (idx == 1) agrupacion = "semana";
  else if (idx == 2) agrupacion = "dia";

  QSqlQuery query = base.estadisticasEvolucionPorTiendaNube(desde, hasta, agrupacion);

  int fila = 0;
  while (query.next()) {
    QString tienda = query.value(1).toString();
    QString periodo = query.value(2).toString();
    int tickets = query.value(3).toInt();
    double ventas = query.value(4).toDouble();
    double ticketMedio = (tickets > 0) ? (ventas / tickets) : 0.0;

    QStandardItem *itPeriodo = new QStandardItem(periodo);
    itPeriodo->setTextAlignment(Qt::AlignCenter);

    QStandardItem *itTienda = new QStandardItem(tienda);

    QStandardItem *itTickets = new QStandardItem();
    itTickets->setData(tickets, Qt::EditRole);
    itTickets->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itVentas = new QStandardItem();
    itVentas->setData(ventas, Qt::EditRole);
    itVentas->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    QStandardItem *itMedio = new QStandardItem();
    itMedio->setData(ticketMedio, Qt::EditRole);
    itMedio->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);

    modeloEvolucion->setItem(fila, 0, itPeriodo);
    modeloEvolucion->setItem(fila, 1, itTienda);
    modeloEvolucion->setItem(fila, 2, itTickets);
    modeloEvolucion->setItem(fila, 3, itVentas);
    modeloEvolucion->setItem(fila, 4, itMedio);
    fila++;
  }
  query.finish();

  ui->tablaEvolucion->setModel(modeloEvolucion);
  ui->tablaEvolucion->resizeColumnsToContents();
}

void DialogEstadisticasTiendas::on_btnActualizar_clicked() {
  cargarEstadisticas();
}

void DialogEstadisticasTiendas::on_tabWidgetTiendas_currentChanged(int index) {
  if (pestanasPendientes.value(index, false)) {
    cargarPestana(index);
    pestanasPendientes[index] = false;
  }
}

void DialogEstadisticasTiendas::on_comboAgrupacion_currentIndexChanged(int) {
  QDate desde = ui->dateDesde->date();
  QDate hasta = ui->dateHasta->date();
  cargarTablaEvolucion(desde, hasta);
}

void DialogEstadisticasTiendas::on_btnHoy_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(hoy);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasTiendas::on_btnSemana_clicked() {
  QDate hoy = QDate::currentDate();
  int diaSemana = hoy.dayOfWeek();
  QDate lunes = hoy.addDays(-(diaSemana - 1));
  ui->dateDesde->setDate(lunes);
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasTiendas::on_btnMes_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasTiendas::on_btnAno_clicked() {
  QDate hoy = QDate::currentDate();
  ui->dateDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateHasta->setDate(hoy);
  cargarEstadisticas();
}

void DialogEstadisticasTiendas::on_btnExportarCSV_clicked() {
  QString ruta = QFileDialog::getSaveFileName(
      this, "Guardar comparativa multitienda en CSV",
      "comparativa_tiendas_" + QDate::currentDate().toString("yyyy-MM-dd") + ".csv",
      "Archivos CSV (*.csv);;Todos los archivos (*)");
  if (ruta.isEmpty()) return;

  QFile archivo(ruta);
  if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
    QMessageBox::warning(this, "Error de Exportación", "No se pudo escribir en el archivo.");
    return;
  }

  QTextStream out(&archivo);
  int tab = ui->tabWidgetTiendas->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  if (tab == 0) modeloActual = modeloComparativa;
  else if (tab == 1) modeloActual = modeloFormasPago;
  else if (tab == 2) modeloActual = modeloEvolucion;

  if (modeloActual) {
    QStringList headers;
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      headers << modeloActual->headerData(c, Qt::Horizontal).toString();
    }
    out << headers.join(";") << "\n";
    for (int r = 0; r < modeloActual->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloActual->columnCount(); ++c) {
        row << modeloActual->item(r, c)->text();
      }
      out << row.join(";") << "\n";
    }
  }

  archivo.close();
  QMessageBox::information(this, "Exportación Completada", "Datos exportados correctamente a CSV.");
}

void DialogEstadisticasTiendas::on_btnCopiarTabla_clicked() {
  QString tsv;
  int tab = ui->tabWidgetTiendas->currentIndex();
  QStandardItemModel *modeloActual = nullptr;
  if (tab == 0) modeloActual = modeloComparativa;
  else if (tab == 1) modeloActual = modeloFormasPago;
  else if (tab == 2) modeloActual = modeloEvolucion;

  if (modeloActual) {
    QStringList headers;
    for (int c = 0; c < modeloActual->columnCount(); ++c) {
      headers << modeloActual->headerData(c, Qt::Horizontal).toString();
    }
    tsv += headers.join("\t") + "\n";
    for (int r = 0; r < modeloActual->rowCount(); ++r) {
      QStringList row;
      for (int c = 0; c < modeloActual->columnCount(); ++c) {
        row << modeloActual->item(r, c)->text();
      }
      tsv += row.join("\t") + "\n";
    }
  }

  QApplication::clipboard()->setText(tsv);
  QMessageBox::information(this, "Portapapeles", "Tabla copiada al portapapeles.");
}
