#include "clientes.h"
#include "buscarcliente.h"
#include "ui_clientes.h"
#include "saneadorglobal.h"
#include "syncmanager.h"
#include <QMessageBox>
#include <QToolTip>
#include "gestorencargosdialog.h"

#include <QSignalBlocker>
#include <QHash>

Clientes::Clientes(QWidget *parent) : QDialog(parent), ui(new Ui::Clientes) {
  ui->setupUi(this);
  inicializarComponentes();
  mapper.setCurrentIndex(0);
  refrescarBotones(mapper.currentIndex());
}

Clientes::Clientes(QWidget *parent, QString codigo)
    : QDialog(parent), ui(new Ui::Clientes) {
  ui->setupUi(this);
  inicializarComponentes();

  // Buscar si el código recibido ya existe en la tabla para posicionar el mapper directamente
  // sin cargar primero el cliente 0 ("Cliente Contado") innecesariamente.
  int idxEncontrado = -1;
  if (!codigo.isEmpty() && modeloTabla) {
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
      if (modeloTabla->record(i).value("idCliente").toString() == codigo) {
        idxEncontrado = i;
        break;
      }
    }
  }

  if (idxEncontrado >= 0) {
    mapper.setCurrentIndex(idxEncontrado);
    refrescarBotones(idxEncontrado);
  } else {
    borrarFormulario();
    ui->lineEditCod->setText(codigo);
    ui->lineEditCod->setEnabled(false);
  }
}

void Clientes::inicializarComponentes() {
  listaProductos = new QSqlQueryModel(this);
  modeloProductos = new QStandardItemModel(this);
  proxyProductos = new QSortFilterProxyModel(this);
  proxyProductos->setSourceModel(modeloProductos);
  proxyProductos->setSortRole(
      Qt::UserRole); // Usamos UserRole para ordenar correctamente números y fechas
  proxyProductos->setFilterCaseSensitivity(Qt::CaseInsensitive);
  proxyProductos->setFilterKeyColumn(-1); // Filtrar por cualquier columna (código o descripción)
  proxyProductos->setDynamicSortFilter(false);
  ui->tableViewProductos->setModel(proxyProductos);
  ui->tableViewProductos->setSortingEnabled(true);
  listaTickets = new QSqlQueryModel(this);
  ticket = new QSqlQueryModel(this);
  vistaTickets = new QStandardItemModel(this);
  modeloTabla = new QSqlQueryModel(this);
  nTicket = "";

  nombreConexionLocal = conf->getConexionLocal();

  listaConexionesRemotas = conf->getNombreConexionesActivas();
  listaConexionesRemotas.insert(0, nombreConexionLocal);

  // Preparar el Gráfico (sin animaciones pesadas para respuesta instantánea)
  ventasChart = new QChart();
  ventasChart->setTitle("Histórico de Ventas");
  ventasChart->setAnimationOptions(QChart::NoAnimation);
  ventasChartView = new QChartView(ventasChart, this);
  ventasChartView->setRenderHint(QPainter::Antialiasing);
  ui->verticalLayoutGrafico->addWidget(ventasChartView);

  // Bloquear señales al inicializar fechas y combos para no disparar consultas
  // antes de que el QDataWidgetMapper tenga seleccionado el cliente inicial.
  // Nota: ui->setupUi(this) ya conecta automáticamente todos los slots on_<objeto>_<señal>,
  // por lo que no deben duplicarse con connect() manuales.
  {
    QSignalBlocker b1(ui->dateEditDesde);
    QSignalBlocker b2(ui->dateEditHasta);
    QSignalBlocker b3(ui->dateEditDesde_2);
    QSignalBlocker b4(ui->dateEditHasta_2);
    QSignalBlocker b5(ui->comboBoxAgrupacion);
    QSignalBlocker b6(ui->comboBoxTipoGrafico);

    ui->dateEditDesde->setDate(QDate::currentDate().addYears(-1));
    ui->dateEditHasta->setDate(QDate::currentDate());
    ui->dateEditDesde_2->setDate(QDate(QDate::currentDate().year(), 1, 1));
    ui->dateEditHasta_2->setDate(QDate::currentDate());
    ui->radioButtonCantidad->setChecked(true);

    // Inicializar estados de los Combos UI
    if (ui->comboBoxTipoGrafico->count() >= 2) {
      ui->comboBoxTipoGrafico->setItemData(0, "agrupado");
      ui->comboBoxTipoGrafico->setItemData(1, "apilado");
    }
    if (ui->comboBoxAgrupacion->count() >= 4) {
      ui->comboBoxAgrupacion->setItemData(0, "mes");
      ui->comboBoxAgrupacion->setItemData(1, "ano");
      ui->comboBoxAgrupacion->setItemData(2, "semana");
      ui->comboBoxAgrupacion->setItemData(3, "dia");
    }
  }

  recargarTabla();

  mapper.addMapping(ui->lineEditCod, 0);
  mapper.addMapping(ui->lineEditNombre, 1);
  mapper.addMapping(ui->lineEditApellidos, 2);
  mapper.addMapping(ui->lineEditDireccion, 3);
  mapper.addMapping(ui->lineEditCP, 4);
  mapper.addMapping(ui->lineEditLocalidad, 5);
  mapper.addMapping(ui->lineEditProvincia, 6);
  mapper.addMapping(ui->lineEditNIF, 7);
  mapper.addMapping(ui->lineEditTlfn1, 8);
  mapper.addMapping(ui->lineEditTlfn2, 9);
  mapper.addMapping(ui->lineEditMail, 10);
  mapper.addMapping(ui->lineEditDescuento, 11);
  mapper.addMapping(ui->dateEdit, 12);
  mapper.addMapping(ui->plainTextEdit, 13);

  ui->lineEditCod->installEventFilter(this);

  // Iconos para botones de acción
  ui->pushButtonNuevo->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
  ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
  ui->pushButtonModificar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
  ui->pushButtonAnterior->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
  ui->pushButtonSiguiente->setIcon(style()->standardIcon(QStyle::SP_ArrowForward));
  ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
  ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
  ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
  ui->btn_encargos_cliente->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
  ui->pushButtonRefrescar->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

  aplicarPermisos();
}

Clientes::~Clientes() { delete ui; }

/**
 * @brief Aplica permisos a los botones del formulario de clientes.
 */
void Clientes::aplicarPermisos() {
  if (!conf || !conf->permisos())
    return;

  ui->pushButtonNuevo->setEnabled(conf->permisos()->tiene("clientes.crear"));
  ui->pushButtonModificar->setEnabled(conf->permisos()->tiene("clientes.modificar"));
  ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("clientes.borrar"));
  ui->pushButtonUnificar->setEnabled(conf->permisos()->tiene("clientes.unificar"));
  ui->pushButtonSaneador->setEnabled(conf->permisos()->tiene("saneador_global"));
  ui->btn_encargos_cliente->setEnabled(conf->permisos()->tiene("encargos"));
}

void Clientes::recargarTabla() {
  modeloTabla->setQuery("SELECT * FROM clientes",
                        QSqlDatabase::database(conf->getConexionCommon()));
  mapper.setModel(modeloTabla);
}

void Clientes::borrarFormulario() {
  foreach (QLineEdit *le, ui->Principal->findChildren<QLineEdit *>()) {
    le->clear();
  }
  ui->dateEdit->setDate(QDate::currentDate());
  ui->plainTextEdit->clear();
  ui->labelNombreCliente->clear();
}

QStringList Clientes::recogerDatosFormulario() {
  QStringList listaDatosFormulario;
  listaDatosFormulario.append(ui->lineEditCod->text());
  listaDatosFormulario.append(ui->lineEditNombre->text());
  listaDatosFormulario.append(ui->lineEditApellidos->text());
  listaDatosFormulario.append(ui->lineEditDireccion->text());
  listaDatosFormulario.append(ui->lineEditCP->text());
  listaDatosFormulario.append(ui->lineEditLocalidad->text());
  listaDatosFormulario.append(ui->lineEditProvincia->text());
  listaDatosFormulario.append(ui->lineEditNIF->text());
  listaDatosFormulario.append(ui->lineEditTlfn1->text());
  listaDatosFormulario.append(ui->lineEditTlfn2->text());
  listaDatosFormulario.append(ui->lineEditMail->text());
  listaDatosFormulario.append(ui->lineEditDescuento->text());
  listaDatosFormulario.append(ui->dateEdit->text());
  listaDatosFormulario.append(ui->plainTextEdit->toPlainText());

  return listaDatosFormulario;
}
bool Clientes::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->lineEditCod) {
    if (event->type() == QEvent::MouseButtonPress) {
      borrarFormulario();
      // El usuario hizo clic en el campo código: se limpia el formulario
      // para facilitar la búsqueda de otro cliente.
      qDebug() << "Clientes: clic en lineEditCod, limpiando formulario";
      return true;
    }
  }
  return false;
}

void Clientes::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_F2) {
    mostrarVentasB = !mostrarVentasB;
    if (mostrarVentasB) {
      // Cambio a tonos rojizos/rosáceos para indicar modo "B" (consolidado)
      ui->headerFrame->setStyleSheet(
          "QFrame#headerFrame { background-color: #fce4ec; border-radius: 10px; "
          "border: 1px solid #f8bbd0; }");
      ui->labelNombreCliente->setStyleSheet("color: #c2185b;");
      qDebug() << "Clientes: Modo Consolidado (F2) ACTIVADO";
    } else {
      // Volver a los tonos verdes originales
      ui->headerFrame->setStyleSheet(
          "QFrame#headerFrame { background-color: #e8f5e9; border-radius: 10px; "
          "border: 1px solid #c8e6c9; }");
      ui->labelNombreCliente->setStyleSheet("color: #2e7d32;");
      qDebug() << "Clientes: Modo Consolidado (F2) DESACTIVADO";
    }
    refrescarBotones(mapper.currentIndex());
  } else {
    QDialog::keyPressEvent(e);
  }
}

void Clientes::refrescarBotones(int i) {
  ui->pushButtonAnterior->setEnabled(i > 0);
  ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
  ui->labelNombreCliente->setText(ui->lineEditNombre->text() + " " +
                                  ui->lineEditApellidos->text());
  cargarCompras();
  if (ui->radioButtonCantidad->isChecked())
    on_radioButtonCantidad_clicked();
  else if (ui->radioButtonFecha->isChecked())
    on_radioButtonFecha_clicked();
}

static QDate obtenerQDateValida(const QVariant &var) {
  if (!var.isValid() || var.isNull())
    return QDate();
  if (var.typeId() == QMetaType::QDate)
    return var.toDate();
  if (var.typeId() == QMetaType::QDateTime)
    return var.toDateTime().date();

  QDate d = var.toDate();
  if (d.isValid())
    return d;

  QDateTime dt = var.toDateTime();
  if (dt.isValid())
    return dt.date();

  QString str = var.toString().trimmed();
  if (str.isEmpty())
    return QDate();

  d = QDate::fromString(str.left(10), "yyyy-MM-dd");
  if (d.isValid())
    return d;

  d = QDate::fromString(str.left(10), "dd/MM/yyyy");
  if (d.isValid())
    return d;

  d = QDate::fromString(str.left(10), "yyyy/MM/dd");
  if (d.isValid())
    return d;

  return QDate::fromString(str, Qt::ISODate);
}

void Clientes::cargarCompras() {
  vistaTickets->clear();
  listaTickets->clear();
  modeloCompras.clear();
  ticket->clear();

  // Limpiar Gráfico
  ventasChart->removeAllSeries();
  const auto axes = ventasChart->axes();
  for (QAbstractAxis *axis : axes) {
    ventasChart->removeAxis(axis);
    delete axis;
  }
  seriesVentas = nullptr;

  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty() && modeloTabla && modeloTabla->rowCount() > 0) {
    int curIdx = mapper.currentIndex();
    if (curIdx < 0) curIdx = 0;
    codigoCliente = modeloTabla->record(curIdx).value("idCliente").toString();
    ui->lineEditCod->setText(codigoCliente);
  }
  if (codigoCliente.isEmpty())
    return;

  double gastoTotal = 0.0;
  int cantidadTickets = 0;
  QDate ultimaVisita(1900, 1, 1);

  // Mapeo: [NombreTienda][Periodo] -> Total
  QMap<QString, QMap<QString, double>> ventasPorTiendaYPeriodo;
  QSet<QString> todosLosPeriodos;
  mapeoCategRango.clear();

  QSqlDatabase dbNube;
  bool usarNubeParaRemotas = false;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    dbNube = base.obtenerConexionNube();
    if (dbNube.isValid() && dbNube.isOpen()) {
      usarNubeParaRemotas = true;
    }
  }

  int idTiendaLocal = conf ? conf->getIdTienda() : 1;
  QStringList conexionesConsultar;
  // Si estamos en PC de desarrollo (idTiendaLocal == 4) con tiendas conectadas y Nube activa,
  // consultamos las 3 tiendas reales directamente desde la Nube.
  if (!(usarNubeParaRemotas && (idTiendaLocal <= 0 || idTiendaLocal >= 4))) {
    conexionesConsultar << nombreConexionLocal;
  }
  if (ui->checkBoxTiendasConectadas->isChecked() && !usarNubeParaRemotas) {
    QStringList activas = conf ? conf->getNombreConexionesActivas() : listaConexionesRemotas;
    for (const QString &c : activas) {
      if (c != nombreConexionLocal && !conexionesConsultar.contains(c)) {
        conexionesConsultar << c;
      }
    }
  }

  QDate desde = ui->dateEditDesde->date();
  QDate hasta = ui->dateEditHasta->date();
  QString agrupacion = ui->comboBoxAgrupacion->currentData().toString();
  if (agrupacion.isEmpty()) {
    int idx = ui->comboBoxAgrupacion->currentIndex();
    if (idx == 1)
      agrupacion = "ano";
    else if (idx == 2)
      agrupacion = "semana";
    else if (idx == 3)
      agrupacion = "dia";
    else
      agrupacion = "mes"; // Fallback por defecto a meses
  }

  // Formato SQL de agrupación para delegar el GROUP BY al motor MariaDB
  // evitando descargar decenas de miles de filas individuales por red.
  QString fmtPeriodoSql = "%Y-%m";
  if (agrupacion == "ano") {
    fmtPeriodoSql = "%Y";
  } else if (agrupacion == "semana") {
    fmtPeriodoSql = "%x-W%v"; // Año ISO (4 dígitos) + Semana ISO (01-53)
  } else if (agrupacion == "dia") {
    fmtPeriodoSql = "%Y-%m-%d";
  }

  // Limpiar tablas y gráficos
  vistaTickets->clear();
  ui->tableView2->setModel(nullptr);
  ui->tableViewDetalleTicket->setModel(nullptr);

  // 1. Consultar agregaciones por periodo en las BDs locales/directas
  for (int c = 0; c < conexionesConsultar.length(); c++) {
    QString connName = conexionesConsultar.at(c);
    QSqlDatabase db = QSqlDatabase::database(connName);
    if (!db.isOpen() && !db.open()) {
      qDebug() << "Clientes::cargarCompras error abriendo BD:" << connName;
      continue;
    }

    QSqlQuery q(db);
    q.setForwardOnly(true);
    if (mostrarVentasB) {
      QString subQuery =
          "SELECT fecha, total FROM tickets WHERE cliente = ? AND fecha BETWEEN ? AND ? "
          "UNION ALL "
          "SELECT fecha, total FROM ticketss WHERE cliente = ? AND fecha BETWEEN ? AND ?";
      q.prepare(QString("SELECT DATE_FORMAT(fecha, '%1') AS periodo, "
                        "COUNT(*) AS num_tickets, "
                        "COALESCE(SUM(total), 0) AS suma_total, "
                        "MAX(DATE_FORMAT(fecha, '%Y-%m-%d')) AS max_fecha "
                        "FROM (%2) AS sub GROUP BY periodo")
                    .arg(fmtPeriodoSql, subQuery));
      q.bindValue(0, codigoCliente);
      q.bindValue(1, desde.toString("yyyy-MM-dd"));
      q.bindValue(2, hasta.toString("yyyy-MM-dd"));
      q.bindValue(3, codigoCliente);
      q.bindValue(4, desde.toString("yyyy-MM-dd"));
      q.bindValue(5, hasta.toString("yyyy-MM-dd"));
    } else {
      q.prepare(QString("SELECT DATE_FORMAT(fecha, '%1') AS periodo, "
                        "COUNT(*) AS num_tickets, "
                        "COALESCE(SUM(total), 0) AS suma_total, "
                        "MAX(DATE_FORMAT(fecha, '%Y-%m-%d')) AS max_fecha "
                        "FROM tickets "
                        "WHERE cliente = ? AND fecha BETWEEN ? AND ? "
                        "GROUP BY periodo")
                    .arg(fmtPeriodoSql));
      q.bindValue(0, codigoCliente);
      q.bindValue(1, desde.toString("yyyy-MM-dd"));
      q.bindValue(2, hasta.toString("yyyy-MM-dd"));
    }

    if (!q.exec()) {
      qDebug() << "Clientes::cargarCompras error:" << q.lastError().text();
      continue;
    }

    while (q.next()) {
      QString key = q.value(0).toString();
      int numT = q.value(1).toInt();
      double sumaPeriodo = q.value(2).toDouble();
      QDate maxFecha = QDate::fromString(q.value(3).toString(), "yyyy-MM-dd");

      gastoTotal += sumaPeriodo;
      cantidadTickets += numT;
      if (maxFecha.isValid() && maxFecha > ultimaVisita) {
        ultimaVisita = maxFecha;
      }

      if (!key.isEmpty()) {
        ventasPorTiendaYPeriodo[connName][key] += sumaPeriodo;
        todosLosPeriodos.insert(key);
      }
    }
  }

  // 2. Si la Nube está disponible y se seleccionó ver todas las tiendas, consultar agregaciones en Nube
  if (usarNubeParaRemotas) {
    QSqlQuery qNube(dbNube);
    qNube.setForwardOnly(true);

    QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                 ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                 : QString("t.id_tienda IN (1, 2, 3)");

    QString sqlNube;
    if (mostrarVentasB) {
      sqlNube = QString(
          "SELECT COALESCE(ti.nombre, CONCAT('Tienda ', sub.id_tienda)) AS tienda_nombre, "
          "DATE_FORMAT(sub.fecha, '%1') AS periodo, "
          "COUNT(*) AS num_tickets, "
          "COALESCE(SUM(sub.total), 0) AS suma_total, "
          "MAX(DATE_FORMAT(sub.fecha, '%Y-%m-%d')) AS max_fecha "
          "FROM ("
          "  SELECT t.id_tienda, t.fecha, t.total FROM tickets_nube t "
          "  WHERE t.cliente = :clienteA AND %2 AND t.fecha BETWEEN :desdeA AND :hastaA "
          "  UNION ALL "
          "  SELECT t.id_tienda, t.fecha, t.total FROM ticketss_nube t "
          "  WHERE t.cliente = :clienteB AND %2 AND t.fecha BETWEEN :desdeB AND :hastaB "
          ") AS sub "
          "LEFT JOIN tiendas ti ON sub.id_tienda = ti.id "
          "GROUP BY sub.id_tienda, tienda_nombre, periodo")
                    .arg(fmtPeriodoSql, condTiendaNube);
      qNube.prepare(sqlNube);
      qNube.bindValue(":clienteA", codigoCliente);
      qNube.bindValue(":desdeA", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaA", hasta.toString("yyyy-MM-dd"));
      qNube.bindValue(":clienteB", codigoCliente);
      qNube.bindValue(":desdeB", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaB", hasta.toString("yyyy-MM-dd"));
    } else {
      sqlNube = QString(
          "SELECT COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda_nombre, "
          "DATE_FORMAT(t.fecha, '%1') AS periodo, "
          "COUNT(*) AS num_tickets, "
          "COALESCE(SUM(t.total), 0) AS suma_total, "
          "MAX(DATE_FORMAT(t.fecha, '%Y-%m-%d')) AS max_fecha "
          "FROM tickets_nube t "
          "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
          "WHERE t.cliente = :cliente AND %2 "
          "AND t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY t.id_tienda, tienda_nombre, periodo")
                    .arg(fmtPeriodoSql, condTiendaNube);
      qNube.prepare(sqlNube);
      qNube.bindValue(":cliente", codigoCliente);
      qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
    }

    if (qNube.exec()) {
      while (qNube.next()) {
        QString nombreTienda = qNube.value(0).toString();
        QString key = qNube.value(1).toString();
        int numT = qNube.value(2).toInt();
        double sumaPeriodo = qNube.value(3).toDouble();
        QDate maxFecha = QDate::fromString(qNube.value(4).toString(), "yyyy-MM-dd");

        gastoTotal += sumaPeriodo;
        cantidadTickets += numT;
        if (maxFecha.isValid() && maxFecha > ultimaVisita) {
          ultimaVisita = maxFecha;
        }

        if (!key.isEmpty()) {
          ventasPorTiendaYPeriodo[nombreTienda][key] += sumaPeriodo;
          todosLosPeriodos.insert(key);
        }
      }
    } else {
      qDebug() << "Clientes::cargarCompras - Error en consulta Nube:" << qNube.lastError().text();
    }
  }

  // Ordenar periodos para el eje X
  QStringList categoriasPeriodos = todosLosPeriodos.values();
  categoriasPeriodos.sort();
  mapeoCategRango = categoriasPeriodos;

  // Refrescar KPIs en la Interfaz
  ui->labelKpiTotalGasto->setText(QString::number(gastoTotal, 'f', 2) + " €");
  ui->labelKpiTotalTickets->setText(QString::number(cantidadTickets));
  if (cantidadTickets > 0 && ultimaVisita.year() > 1900) {
    ui->labelKpiFechaUltima->setText(ultimaVisita.toString("yyyy-MM-dd"));
  } else {
    ui->labelKpiFechaUltima->setText("-");
  }

  // Refrescar Gráfico incluyendo todas las tiendas con datos (locales y nube)
  QString tipoGrafico = ui->comboBoxTipoGrafico->currentData().toString();
  QAbstractBarSeries *series;
  if (tipoGrafico == "apilado") {
    series = new QStackedBarSeries();
  } else {
    series = new QBarSeries();
  }
  seriesVentas = series;
  connect(series, &QAbstractBarSeries::clicked, this, &Clientes::onBarClicked);
  connect(series, &QAbstractBarSeries::hovered, this, &Clientes::onBarHovered);

  // Colores suaves para las tiendas
  QList<QColor> colores;
  colores << QColor("#4A90E2") << QColor("#50E3C2") << QColor("#F5A623")
          << QColor("#D0021B") << QColor("#BD10E0") << QColor("#7ED321");

  // Construir orden de tiendas para el gráfico: tienda local primero, luego el resto
  QStringList tiendasGrafico;
  if (ventasPorTiendaYPeriodo.contains(nombreConexionLocal)) {
    tiendasGrafico << nombreConexionLocal;
  }
  for (auto it = ventasPorTiendaYPeriodo.constBegin(); it != ventasPorTiendaYPeriodo.constEnd(); ++it) {
    if (!tiendasGrafico.contains(it.key())) {
      tiendasGrafico << it.key();
    }
  }

  int colorIdx = 0;
  for (const QString &tienda : tiendasGrafico) {
    QBarSet *setVentas = new QBarSet(tienda);
    setVentas->setColor(colores.at(colorIdx % colores.size()));
    colorIdx++;

    for (const QString &d : categoriasPeriodos) {
      *setVentas << ventasPorTiendaYPeriodo[tienda].value(d, 0.0);
    }
    series->append(setVentas);
  }

  ventasChart->addSeries(series);

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categoriasPeriodos);
  ventasChart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  ventasChart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  // Títulos de las columnas
  QStringList etiquetas;
  etiquetas << "Ticket" << "Vendedor" << "Fecha" << "Hora" << "Dto" << "Total"
            << "F. Pago" << "Pagado" << "Entrega" << "Cambio" << "Tienda";
  vistaTickets->setHorizontalHeaderLabels(etiquetas);
  ui->tableView2->setModel(vistaTickets);

  // Cargar automáticamente los tickets más recientes del período en la tabla
  cargarTicketsPorRango("");
}

void Clientes::cargarTicketsPorRango(const QString &rangoMapeado) {
  vistaTickets->removeRows(0, vistaTickets->rowCount());
  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty() && modeloTabla && modeloTabla->rowCount() > 0) {
    int curIdx = mapper.currentIndex();
    if (curIdx < 0) curIdx = 0;
    codigoCliente = modeloTabla->record(curIdx).value("idCliente").toString();
    ui->lineEditCod->setText(codigoCliente);
  }
  if (codigoCliente.isEmpty())
    return;

  QSqlDatabase dbNube;
  bool usarNubeParaRemotas = false;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    dbNube = base.obtenerConexionNube();
    if (dbNube.isValid() && dbNube.isOpen()) {
      usarNubeParaRemotas = true;
    }
  }

  int idTiendaLocal = conf ? conf->getIdTienda() : 1;
  QStringList conexionesConsultar;
  if (!(usarNubeParaRemotas && (idTiendaLocal <= 0 || idTiendaLocal >= 4))) {
    conexionesConsultar << nombreConexionLocal;
  }
  if (ui->checkBoxTiendasConectadas->isChecked() && !usarNubeParaRemotas) {
    for (const QString &c : listaConexionesRemotas) {
      if (!conexionesConsultar.contains(c)) {
        conexionesConsultar << c;
      }
    }
  }

  QString agrupacion = ui->comboBoxAgrupacion->currentData().toString();
  if (agrupacion.isEmpty()) {
    int idx = ui->comboBoxAgrupacion->currentIndex();
    if (idx == 1)
      agrupacion = "ano";
    else if (idx == 2)
      agrupacion = "semana";
    else if (idx == 3)
      agrupacion = "dia";
    else
      agrupacion = "mes";
  }

  QString fmtPeriodoSql = "%Y-%m";
  if (agrupacion == "ano") {
    fmtPeriodoSql = "%Y";
  } else if (agrupacion == "semana") {
    fmtPeriodoSql = "%x-W%v";
  } else if (agrupacion == "dia") {
    fmtPeriodoSql = "%Y-%m-%d";
  }

  QDate desde = ui->dateEditDesde->date();
  QDate hasta = ui->dateEditHasta->date();

  // Precargar mapas de usuarios y formas de pago en memoria (2 consultas en total
  // en lugar de ejecutar 1.000 consultas SQL dentro del bucle de filas).
  QHash<QString, QString> mapaUsuarios;
  QHash<QString, QString> mapaFormasPago;
  {
    QSqlDatabase dbCatalogo = QSqlDatabase::database(conf ? conf->getConexionLocal() : nombreConexionLocal);
    if (dbCatalogo.isOpen()) {
      QSqlQuery qUsu(dbCatalogo);
      qUsu.setForwardOnly(true);
      if (qUsu.exec("SELECT id, nombre FROM usuarios")) {
        while (qUsu.next()) {
          mapaUsuarios.insert(qUsu.value(0).toString(), qUsu.value(1).toString());
        }
      }
      QSqlQuery qFp(dbCatalogo);
      qFp.setForwardOnly(true);
      if (qFp.exec("SELECT id, tipo FROM fpago")) {
        while (qFp.next()) {
          mapaFormasPago.insert(qFp.value(0).toString(), qFp.value(1).toString());
        }
      }
    }
    if (usarNubeParaRemotas && dbNube.isOpen()) {
      QSqlQuery qUsuNube(dbNube);
      qUsuNube.setForwardOnly(true);
      if (qUsuNube.exec("SELECT id, nombre FROM usuarios")) {
        while (qUsuNube.next()) {
          if (!mapaUsuarios.contains(qUsuNube.value(0).toString())) {
            mapaUsuarios.insert(qUsuNube.value(0).toString(), qUsuNube.value(1).toString());
          }
        }
      }
      QSqlQuery qFpNube(dbNube);
      qFpNube.setForwardOnly(true);
      if (qFpNube.exec("SELECT id, tipo FROM fpago")) {
        while (qFpNube.next()) {
          if (!mapaFormasPago.contains(qFpNube.value(0).toString())) {
            mapaFormasPago.insert(qFpNube.value(0).toString(), qFpNube.value(1).toString());
          }
        }
      }
    }
  }

  // Desactivar el ordenamiento mientras se insertan filas para evitar
  // que Qt reordene el modelo tras cada appendRow (rendimiento O(n²)).
  ui->tableView2->setSortingEnabled(false);

  // Límite máximo de tickets por origen para evitar congelar la UI.
  const int LIMITE_FILAS = 500;
  QString filtroRangoSql = rangoMapeado.isEmpty()
                               ? QString()
                               : QString(" AND DATE_FORMAT(fecha, '%1') = :rango").arg(fmtPeriodoSql);

  for (int c = 0; c < conexionesConsultar.length(); c++) {
    QString connName = conexionesConsultar.at(c);
    QSqlDatabase db = QSqlDatabase::database(connName);
    if (!db.isOpen() && !db.open()) {
      qDebug() << "Clientes::cargarTicketsPorRango error abriendo BD:" << connName;
      continue;
    }

    QSqlQuery q(db);
    q.setForwardOnly(true);
    QString cols = "ticket, usuario, DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha_str, "
                   "TIME_FORMAT(hora, '%H:%i:%s') AS hora_str, descuento, total, fpago, cobrado, entrega, cambio";

    if (mostrarVentasB) {
      QString sql = QString(
          "SELECT * FROM ("
          "  SELECT %1, 'A' AS tipo_sector FROM tickets "
          "  WHERE cliente = :clienteA AND fecha BETWEEN :desdeA AND :hastaA %2 "
          "  UNION ALL "
          "  SELECT %1, 'B' AS tipo_sector FROM ticketss "
          "  WHERE cliente = :clienteB AND fecha BETWEEN :desdeB AND :hastaB %2 "
          ") AS sub ORDER BY fecha_str DESC, hora_str DESC LIMIT %3")
                        .arg(cols, filtroRangoSql)
                        .arg(LIMITE_FILAS);
      q.prepare(sql);
      q.bindValue(":clienteA", codigoCliente);
      q.bindValue(":desdeA", desde.toString("yyyy-MM-dd"));
      q.bindValue(":hastaA", hasta.toString("yyyy-MM-dd"));
      q.bindValue(":clienteB", codigoCliente);
      q.bindValue(":desdeB", desde.toString("yyyy-MM-dd"));
      q.bindValue(":hastaB", hasta.toString("yyyy-MM-dd"));
      if (!rangoMapeado.isEmpty()) {
        q.bindValue(":rango", rangoMapeado);
      }
    } else {
      QString sql = QString(
          "SELECT %1, 'A' AS tipo_sector FROM tickets "
          "WHERE cliente = :cliente AND fecha BETWEEN :desde AND :hasta %2 "
          "ORDER BY fecha DESC, hora DESC LIMIT %3")
                        .arg(cols, filtroRangoSql)
                        .arg(LIMITE_FILAS);
      q.prepare(sql);
      q.bindValue(":cliente", codigoCliente);
      q.bindValue(":desde", desde.toString("yyyy-MM-dd"));
      q.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
      if (!rangoMapeado.isEmpty()) {
        q.bindValue(":rango", rangoMapeado);
      }
    }

    if (!q.exec()) {
      qDebug() << "Clientes::cargarTicketsPorRango error:" << q.lastError().text();
      continue;
    }

    while (q.next()) {
      QString fechaStr = q.value(2).toString();
      if (fechaStr.isEmpty())
        continue;

      QString idUsu = q.value(1).toString();
      QString idFp = q.value(6).toString();
      QString nomUsu = mapaUsuarios.value(idUsu, idUsu);
      QString nomFp = mapaFormasPago.value(idFp, idFp);

      listaItems.clear();
      listaItems << new QStandardItem(q.value(0).toString());
      listaItems << new QStandardItem(nomUsu);
      listaItems << new QStandardItem(fechaStr);
      listaItems << new QStandardItem(q.value(3).toString());
      listaItems << new QStandardItem(q.value(4).toString());
      listaItems << new QStandardItem(QString::number(q.value(5).toDouble(), 'f', 2));
      listaItems << new QStandardItem(nomFp);
      QString pagado = (q.value(7).toInt() == 1) ? "Sí" : "No";
      listaItems << new QStandardItem(pagado);
      listaItems << new QStandardItem(q.value(8).toString());
      listaItems << new QStandardItem(q.value(9).toString());
      listaItems << new QStandardItem(connName);

      // Columna oculta para el tipo de sector (A o B)
      QStandardItem *itemTipo = new QStandardItem(q.value(10).toString());
      listaItems << itemTipo;

      vistaTickets->appendRow(listaItems);

      // Resaltar en rojo claro si es del sector B
      if (q.value(10).toString() == "B") {
        for (int col = 0; col < listaItems.count(); ++col) {
          listaItems.at(col)->setBackground(QBrush(QColor(255, 235, 238)));
          listaItems.at(col)->setForeground(QBrush(QColor(183, 28, 28)));
        }
      }
    }
  }

  // Si la Nube está disponible, cargar los tickets más recientes de las tiendas en la Nube
  if (usarNubeParaRemotas) {
    QSqlQuery qNube(dbNube);
    qNube.setForwardOnly(true);

    QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                 ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                 : QString("t.id_tienda IN (1, 2, 3)");
    QString filtroRangoNubeSql = rangoMapeado.isEmpty()
                                     ? QString()
                                     : QString(" AND DATE_FORMAT(t.fecha, '%1') = :rango").arg(fmtPeriodoSql);

    QString sqlNube;
    if (mostrarVentasB) {
      sqlNube = QString(
          "SELECT * FROM ("
          "  SELECT t.ticket, t.usuario, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str, "
          "  TIME_FORMAT(t.hora, '%H:%i:%s') AS hora_str, t.descuento, t.total, t.fpago, "
          "  t.cobrado, t.entrega, t.cambio, 'A' AS tipo_sector, "
          "  COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda_nombre "
          "  FROM tickets_nube t LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
          "  WHERE t.cliente = :clienteA AND %1 AND t.fecha BETWEEN :desdeA AND :hastaA %2 "
          "  UNION ALL "
          "  SELECT t.ticket, t.usuario, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str, "
          "  TIME_FORMAT(t.hora, '%H:%i:%s') AS hora_str, t.descuento, t.total, t.fpago, "
          "  t.cobrado, t.entrega, t.cambio, 'B' AS tipo_sector, "
          "  COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda_nombre "
          "  FROM ticketss_nube t LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
          "  WHERE t.cliente = :clienteB AND %1 AND t.fecha BETWEEN :desdeB AND :hastaB %2 "
          ") AS sub ORDER BY fecha_str DESC, hora_str DESC LIMIT %3")
                    .arg(condTiendaNube, filtroRangoNubeSql)
                    .arg(LIMITE_FILAS);
      qNube.prepare(sqlNube);
      qNube.bindValue(":clienteA", codigoCliente);
      qNube.bindValue(":desdeA", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaA", hasta.toString("yyyy-MM-dd"));
      qNube.bindValue(":clienteB", codigoCliente);
      qNube.bindValue(":desdeB", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaB", hasta.toString("yyyy-MM-dd"));
      if (!rangoMapeado.isEmpty()) {
        qNube.bindValue(":rango", rangoMapeado);
      }
    } else {
      sqlNube = QString(
          "SELECT t.ticket, t.usuario, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str, "
          "TIME_FORMAT(t.hora, '%H:%i:%s') AS hora_str, t.descuento, t.total, t.fpago, "
          "t.cobrado, t.entrega, t.cambio, 'A' AS tipo_sector, "
          "COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda_nombre "
          "FROM tickets_nube t "
          "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
          "WHERE t.cliente = :cliente AND %1 "
          "AND t.fecha BETWEEN :desde AND :hasta %2 "
          "ORDER BY t.fecha DESC, t.hora DESC LIMIT %3")
                    .arg(condTiendaNube, filtroRangoNubeSql)
                    .arg(LIMITE_FILAS);
      qNube.prepare(sqlNube);
      qNube.bindValue(":cliente", codigoCliente);
      qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
      qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
      if (!rangoMapeado.isEmpty()) {
        qNube.bindValue(":rango", rangoMapeado);
      }
    }

    if (qNube.exec()) {
      while (qNube.next()) {
        QString fechaStr = qNube.value(2).toString();
        if (fechaStr.isEmpty())
          continue;

        QString idUsu = qNube.value(1).toString();
        QString idFp = qNube.value(6).toString();
        QString nomUsu = mapaUsuarios.value(idUsu, idUsu);
        QString nomFp = mapaFormasPago.value(idFp, idFp);
        QString connName = qNube.value(11).toString();

        QList<QStandardItem *> itemsFila;
        itemsFila << new QStandardItem(qNube.value(0).toString());
        itemsFila << new QStandardItem(nomUsu);
        itemsFila << new QStandardItem(fechaStr);
        itemsFila << new QStandardItem(qNube.value(3).toString());
        itemsFila << new QStandardItem(qNube.value(4).toString());
        itemsFila << new QStandardItem(QString::number(qNube.value(5).toDouble(), 'f', 2));
        itemsFila << new QStandardItem(nomFp);
        QString pagado = (qNube.value(7).toInt() == 1) ? "Sí" : "No";
        itemsFila << new QStandardItem(pagado);
        itemsFila << new QStandardItem(qNube.value(8).toString());
        itemsFila << new QStandardItem(qNube.value(9).toString());
        itemsFila << new QStandardItem(connName);
        itemsFila << new QStandardItem(qNube.value(10).toString());

        vistaTickets->appendRow(itemsFila);

        if (qNube.value(10).toString() == "B") {
          for (int col = 0; col < itemsFila.count(); ++col) {
            itemsFila.at(col)->setBackground(QBrush(QColor(255, 235, 238)));
            itemsFila.at(col)->setForeground(QBrush(QColor(183, 28, 28)));
          }
        }
      }
    } else {
      qDebug() << "Clientes::cargarTicketsPorRango - Error en consulta Nube:" << qNube.lastError().text();
    }
  }

  // Reactivar ordenamiento y ajustar columnas una vez insertadas todas las filas
  ui->tableView2->setSortingEnabled(true);
  ui->tableView2->resizeColumnsToContents();
  ui->tableView2->hideColumn(11); // Ocultar el tipo_sector
  ui->tableView2->sortByColumn(2, Qt::DescendingOrder);
}

void Clientes::onBarClicked(int index, QBarSet *barset) {
  Q_UNUSED(barset);
  if (index >= 0 && index < mapeoCategRango.size()) {
    cargarTicketsPorRango(mapeoCategRango.at(index));
  }
}

void Clientes::onBarHovered(bool status, int index, QBarSet *barset) {
  if (status && index >= 0 && index < mapeoCategRango.size()) {
    double totalPeriodo = 0;
    QList<QBarSet *> sets = seriesVentas->barSets();
    for (QBarSet *set : sets) {
      if (index < set->count())
        totalPeriodo += set->at(index);
    }

    QString texto =
        QString("<b>Periodo: %1</b><br/>").arg(mapeoCategRango.at(index));

    texto += QString("Tienda: %1 (%2 €)<br/>")
                 .arg(barset->label())
                 .arg(QString::number(barset->at(index), 'f', 2));

    if (sets.count() > 1) {
      texto += QString("<hr/><b>Total Todos: %1 €</b>")
                   .arg(QString::number(totalPeriodo, 'f', 2));
    }

    QToolTip::showText(QCursor::pos(), texto, ventasChartView);
  }
}

void Clientes::on_dateEditDesde_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  cargarCompras();
}

void Clientes::on_dateEditHasta_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  cargarCompras();
}

void Clientes::on_comboBoxAgrupacion_currentIndexChanged(int index) {
  Q_UNUSED(index);
  cargarCompras();
}

void Clientes::on_comboBoxTipoGrafico_currentIndexChanged(int index) {
  Q_UNUSED(index);
  cargarCompras();
}

void Clientes::on_pushButtonAnterior_clicked() {
  mapper.toPrevious();
  refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonSiguiente_clicked() {
  mapper.toNext();
  refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonNuevo_clicked() {
  // Verificar que el código no exista ya en LOCAL antes de crear
  if (base.existeDatoEnTabla(QSqlDatabase::database(nombreConexionLocal),
                             "clientes", "idCliente",
                             ui->lineEditCod->text())) {
    QMessageBox::warning(this, "ATENCION", "El registro ya existe");
    return;
  }

  QStringList datos = recogerDatosFormulario();

  // Solo escribir en LOCAL: el SyncManager propagará el cambio a la nube
  // mediante la cola de sincronización (sync_cola) de forma automática.
  if (base.crearCliente(QSqlDatabase::database(nombreConexionLocal), datos)) {
    QMessageBox::about(this, "Atención", "Cliente creado con éxito");
  } else {
    QMessageBox::warning(this, "Error",
                         "No se ha podido crear el Cliente");
    return;
  }

  recargarTabla();
}

void Clientes::on_pushButtonModificar_clicked() {
  QStringList datos = recogerDatosFormulario();
  int i = mapper.currentIndex();

  QMessageBox msgBox;
  msgBox.setText("MODIFICACION");
  msgBox.setInformativeText("Quiere guardar los cambios?");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  int resp = msgBox.exec();
  if (resp == QMessageBox::Ok) {
    // Solo modificar en LOCAL: el SyncManager propagará a la nube
    // automáticamente según la cola de sincronización (sync_cola).
    if (base.modificarCliente(QSqlDatabase::database(nombreConexionLocal),
                              datos, ui->lineEditCod->text())) {
      msgBox.setText("Guardado con exito");
      msgBox.setInformativeText(
          "El registro se ha modificado correctamente");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    } else {
      msgBox.setText("Error al guardar");
      msgBox.setInformativeText(
          "Revise los datos del formulario o contacte con el administrador");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
      return;
    }
  }
  recargarTabla();
  mapper.setCurrentIndex(i);
  refrescarBotones(i);
}

void Clientes::on_pushButtonBorrar_clicked() {
  int idCliente = mapper.currentIndex();
  QMessageBox msgBox;
  msgBox.setText("Borrar?.");
  msgBox.setInformativeText(
      "Borrar un cliente puede influir en documentos anteriores.");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Cancel);
  if (msgBox.exec() == QMessageBox::Ok) {
    // Borrar en LOCAL: el SyncManager gestionará la propagación.
    if (base.borrarCliente(QSqlDatabase::database(nombreConexionLocal),
                           ui->lineEditCod->text().toInt())) {
      msgBox.setText("Borrado");
      msgBox.setInformativeText("El cliente se ha borrado con éxito");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    } else {
      msgBox.setText("Error al borrar");
      msgBox.setInformativeText("No se ha podido borrar el cliente");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
  recargarTabla();
  mapper.setCurrentIndex(idCliente);
  refrescarBotones(idCliente);
}

void Clientes::on_pushButtonRefrescar_clicked() {
  recargarTabla();
  refrescarBotones(mapper.currentIndex());
}

void Clientes::on_lineEditNombre_returnPressed() {
  consulta =
      base.buscarEnTabla(QSqlDatabase::database(nombreConexionLocal),
                         "clientes", "nombre", ui->lineEditNombre->text());
  consulta.first();
  qDebug() << consulta.lastError().text();
  BuscarCliente *buscar = new BuscarCliente(this, consulta);
  buscar->exec();
  qDebug() << buscar->resultado;
  for (int i = 0; i < modeloTabla->rowCount(); i++) {
    if (modeloTabla->record(i).value("idCliente").toString() ==
        buscar->resultado) {
      mapper.setCurrentIndex(i);
      refrescarBotones(i);
      break;
    }
  }
  delete buscar;
}

void Clientes::on_lineEditCod_editingFinished() {
  for (int i = 0; i < modeloTabla->rowCount(); ++i) {
    if (modeloTabla->record(i).value("idCliente").toString() ==
        ui->lineEditCod->text()) {
      mapper.setCurrentIndex(i);
      refrescarBotones(i);
      break;
    }
  }
}

// Slot del checkbox unificado de tiendas: recarga ventas y productos
void Clientes::on_checkBoxTiendasConectadas_clicked() {
  cargarCompras();
  if (ui->radioButtonCantidad->isChecked())
    on_radioButtonCantidad_clicked();
  else if (ui->radioButtonFecha->isChecked())
    on_radioButtonFecha_clicked();
}

void Clientes::on_tableView2_clicked(const QModelIndex &index) {
  on_tableView2_doubleClicked(index);
}

void Clientes::on_tableView2_doubleClicked(const QModelIndex &index) {
  QModelIndex indice = vistaTickets->index(index.row(), 0);
  nTicket = vistaTickets->data(indice, Qt::EditRole).toString();

  QModelIndex indiceTienda = vistaTickets->index(index.row(), 10);
  QString dbName = vistaTickets->data(indiceTienda, Qt::EditRole).toString();

  QModelIndex indiceTipo = vistaTickets->index(index.row(), 11);
  QString sector = vistaTickets->data(indiceTipo, Qt::EditRole).toString();

  // En lineasticket y lineasticket_nube, los tickets de Serie B llevan prefijo 'B' en nticket
  QString nTicketBusqueda = (sector == "B" && !nTicket.startsWith("B"))
                                ? ("B" + nTicket)
                                : nTicket;

  QSqlQuery qTicket;
  QSqlDatabase dbNube = (dbName != nombreConexionLocal) ? base.obtenerConexionNube() : QSqlDatabase();

  // Si el ticket es de una tienda remota y la Nube está disponible, consultarlo directamente desde lineasticket_nube
  if (dbName != nombreConexionLocal && dbNube.isValid() && dbNube.isOpen()) {
    qTicket = QSqlQuery(dbNube);
    qTicket.prepare("SELECT id_local, nticket, cod, descripcion, cantidad, iva, precio, descuento, totallinea, "
                    "DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha, TIME_FORMAT(hora, '%H:%i:%s') AS hora "
                    "FROM lineasticket_nube "
                    "WHERE nticket = :nticket "
                    "AND id_tienda = (SELECT id FROM tiendas WHERE nombre = :nombre LIMIT 1)");
    qTicket.bindValue(":nticket", nTicketBusqueda);
    qTicket.bindValue(":nombre", dbName);
    if (!qTicket.exec()) {
      qDebug() << "Clientes::detalleTicket Nube:" << qTicket.lastError().text();
    }
  } else {
    QSqlDatabase db = QSqlDatabase::database(dbName);
    qTicket = QSqlQuery(db);
    qTicket.prepare("SELECT id, nticket, cod, descripcion, cantidad, iva, precio, descuento, totallinea, "
                    "DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha, TIME_FORMAT(hora, '%H:%i:%s') AS hora "
                    "FROM lineasticket WHERE nticket = ?");
    qTicket.addBindValue(nTicketBusqueda);
    if (!qTicket.exec()) {
      qDebug() << "Clientes::detalleTicket:" << qTicket.lastError().text();
    }
  }
  ticket->setQuery(qTicket);

  ui->tableViewDetalleTicket->setModel(ticket);
  ui->tableViewDetalleTicket->hideColumn(0);
  ui->tableViewDetalleTicket->hideColumn(1);
  ui->tableViewDetalleTicket->hideColumn(2);
  ui->tableViewDetalleTicket->resizeColumnsToContents();
}

void Clientes::on_radioButtonCantidad_clicked() {
  ui->tableViewProductos->setSortingEnabled(false);
  proxyProductos->setSourceModel(nullptr);
  modeloProductos->clear();
  QStringList labels;
  labels << "Código" << "Descripción" << "Cantidad";
  modeloProductos->setHorizontalHeaderLabels(labels);

  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty()) {
    proxyProductos->setSourceModel(modeloProductos);
    return;
  }

  QSqlDatabase dbNube;
  bool usarNubeParaRemotas = false;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    dbNube = base.obtenerConexionNube();
    if (dbNube.isValid() && dbNube.isOpen()) {
      usarNubeParaRemotas = true;
    }
  }

  int idTiendaLocal = conf ? conf->getIdTienda() : 1;
  QStringList conexiones;
  if (!(usarNubeParaRemotas && (idTiendaLocal <= 0 || idTiendaLocal >= 4))) {
    conexiones << nombreConexionLocal;
  }
  if (ui->checkBoxTiendasConectadas->isChecked() && !usarNubeParaRemotas) {
    QStringList activas = conf ? conf->getNombreConexionesActivas() : listaConexionesRemotas;
    for (const QString &c : activas) {
      if (c != nombreConexionLocal && !conexiones.contains(c)) {
        conexiones << c;
      }
    }
  }

  QMap<QString, QPair<QString, double>> totales; // cod -> (desc, cant)

  for (const QString &conn : conexiones) {
    if (!QSqlDatabase::database(conn).isOpen())
      continue;
    QSqlQuery q = base.productosPorClienteCantidad(conn, codigoCliente,
                                                   ui->dateEditDesde_2->date(),
                                                   ui->dateEditHasta_2->date(),
                                                   mostrarVentasB);
    while (q.next()) {
      QString cod = q.value(0).toString();
      QString desc = q.value(1).toString();
      double cant = q.value(2).toDouble();

      totales[cod].first = desc;
      totales[cod].second += cant;
    }
  }

  // Si usamos la Nube para las remotas, consultar lineasticket_nube usando CAST(t.ticket AS CHAR)
  // para aprovechar los índices idx_cliente_fecha (en tickets_nube) e idx_tienda_ticket (en lineasticket_nube).
  if (usarNubeParaRemotas) {
    QSqlQuery qNube(dbNube);
    qNube.setForwardOnly(true);

    QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                 ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                 : QString("t.id_tienda IN (1, 2, 3)");

    QString sqlNube;
    if (mostrarVentasB) {
      sqlNube = QString(
          "SELECT cod, descripcion, SUM(cantidad) AS cant FROM ("
          "  SELECT l.cod, l.descripcion, l.cantidad "
          "  FROM tickets_nube t "
          "  INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CAST(t.ticket AS CHAR) "
          "  WHERE t.cliente = :clienteA AND %1 AND t.fecha BETWEEN :desdeA AND :hastaA "
          "  UNION ALL "
          "  SELECT l.cod, l.descripcion, l.cantidad "
          "  FROM ticketss_nube t "
          "  INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CONCAT('B', t.ticket) "
          "  WHERE t.cliente = :clienteB AND %1 AND t.fecha BETWEEN :desdeB AND :hastaB "
          ") AS sub GROUP BY cod, descripcion")
                    .arg(condTiendaNube);
      qNube.prepare(sqlNube);
      qNube.bindValue(":clienteA", codigoCliente);
      qNube.bindValue(":desdeA", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaA", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":clienteB", codigoCliente);
      qNube.bindValue(":desdeB", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaB", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
    } else {
      sqlNube = QString(
          "SELECT l.cod, l.descripcion, SUM(l.cantidad) AS cant "
          "FROM tickets_nube t "
          "INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CAST(t.ticket AS CHAR) "
          "WHERE t.cliente = :cliente AND %1 "
          "AND t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY l.cod, l.descripcion")
                    .arg(condTiendaNube);
      qNube.prepare(sqlNube);
      qNube.bindValue(":cliente", codigoCliente);
      qNube.bindValue(":desde", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hasta", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
    }

    if (qNube.exec()) {
      while (qNube.next()) {
        QString cod = qNube.value(0).toString();
        QString desc = qNube.value(1).toString();
        double cant = qNube.value(2).toDouble();

        totales[cod].first = desc;
        totales[cod].second += cant;
      }
    } else {
      qDebug() << "Clientes::on_radioButtonCantidad_clicked Nube:" << qNube.lastError().text();
    }
  }

  for (auto it = totales.constBegin(); it != totales.constEnd(); ++it) {
    QList<QStandardItem *> items;

    QStandardItem *itemCod = new QStandardItem(it.key());
    itemCod->setData(it.key(), Qt::UserRole);
    items << itemCod;

    QStandardItem *itemDesc = new QStandardItem(it.value().first);
    itemDesc->setData(it.value().first, Qt::UserRole);
    items << itemDesc;

    QStandardItem *itemCant = new QStandardItem();
    itemCant->setData(QVariant::fromValue(it.value().second), Qt::UserRole);
    itemCant->setData(QString::number(it.value().second, 'f', 2),
                      Qt::DisplayRole);
    items << itemCant;

    modeloProductos->appendRow(items);
  }

  proxyProductos->setSourceModel(modeloProductos);
  proxyProductos->setFilterFixedString(ui->lineEditBuscarProducto->text().trimmed());
  ui->tableViewProductos->setSortingEnabled(true);
  proxyProductos->sort(2, Qt::DescendingOrder);
  ui->tableViewProductos->resizeColumnsToContents();
}

void Clientes::on_radioButtonFecha_clicked() {
  ui->tableViewProductos->setSortingEnabled(false);
  proxyProductos->setSourceModel(nullptr);
  modeloProductos->clear();
  QStringList labels;
  labels << "Código" << "Descripción" << "Cantidad" << "Fecha";
  modeloProductos->setHorizontalHeaderLabels(labels);

  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty()) {
    proxyProductos->setSourceModel(modeloProductos);
    return;
  }

  QSqlDatabase dbNube;
  bool usarNubeParaRemotas = false;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    dbNube = base.obtenerConexionNube();
    if (dbNube.isValid() && dbNube.isOpen()) {
      usarNubeParaRemotas = true;
    }
  }

  int idTiendaLocal = conf ? conf->getIdTienda() : 1;
  QStringList conexiones;
  if (!(usarNubeParaRemotas && (idTiendaLocal <= 0 || idTiendaLocal >= 4))) {
    conexiones << nombreConexionLocal;
  }
  if (ui->checkBoxTiendasConectadas->isChecked() && !usarNubeParaRemotas) {
    QStringList activas = conf ? conf->getNombreConexionesActivas() : listaConexionesRemotas;
    for (const QString &c : activas) {
      if (c != nombreConexionLocal && !conexiones.contains(c)) {
        conexiones << c;
      }
    }
  }

  const int LIMITE_LINEAS_FECHA = 1000;

  for (const QString &conn : conexiones) {
    if (!QSqlDatabase::database(conn).isOpen())
      continue;
    QSqlQuery q = base.productosPorClienteFecha(conn, codigoCliente,
                                                ui->dateEditDesde_2->date(),
                                                ui->dateEditHasta_2->date(),
                                                mostrarVentasB);
    while (q.next()) {
      QString cod = q.value(0).toString();
      QString desc = q.value(1).toString();

      QList<QStandardItem *> items;

      QStandardItem *itemCod = new QStandardItem(cod);
      itemCod->setData(cod, Qt::UserRole);
      items << itemCod;

      QStandardItem *itemDesc = new QStandardItem(desc);
      itemDesc->setData(desc, Qt::UserRole);
      items << itemDesc;

      double cant = q.value(2).toDouble();
      QStandardItem *itemCant = new QStandardItem();
      itemCant->setData(QVariant::fromValue(cant), Qt::UserRole);
      itemCant->setData(QString::number(cant, 'f', 2), Qt::DisplayRole);
      items << itemCant;

      QDate fecha = obtenerQDateValida(q.value(3));
      QString fechaStr = fecha.isValid() ? fecha.toString("yyyy-MM-dd") : q.value(3).toString();
      QStandardItem *itemFecha = new QStandardItem();
      itemFecha->setData(QVariant::fromValue(fecha), Qt::UserRole);
      itemFecha->setData(fechaStr, Qt::DisplayRole);
      items << itemFecha;

      modeloProductos->appendRow(items);
    }
  }

  // Si usamos la Nube para las remotas, consultar lineasticket_nube aprovechando índices y limitando a 1000 filas
  if (usarNubeParaRemotas) {
    QSqlQuery qNube(dbNube);
    qNube.setForwardOnly(true);

    QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                 ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                 : QString("t.id_tienda IN (1, 2, 3)");

    QString sqlNube;
    if (mostrarVentasB) {
      sqlNube = QString(
          "SELECT cod, descripcion, cantidad, fecha_str FROM ("
          "  SELECT l.cod, l.descripcion, l.cantidad, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str "
          "  FROM tickets_nube t "
          "  INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CAST(t.ticket AS CHAR) "
          "  WHERE t.cliente = :clienteA AND %1 AND t.fecha BETWEEN :desdeA AND :hastaA "
          "  UNION ALL "
          "  SELECT l.cod, l.descripcion, l.cantidad, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str "
          "  FROM ticketss_nube t "
          "  INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CONCAT('B', t.ticket) "
          "  WHERE t.cliente = :clienteB AND %1 AND t.fecha BETWEEN :desdeB AND :hastaB "
          ") AS sub ORDER BY fecha_str DESC LIMIT %2")
                    .arg(condTiendaNube)
                    .arg(LIMITE_LINEAS_FECHA);
      qNube.prepare(sqlNube);
      qNube.bindValue(":clienteA", codigoCliente);
      qNube.bindValue(":desdeA", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaA", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":clienteB", codigoCliente);
      qNube.bindValue(":desdeB", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hastaB", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
    } else {
      sqlNube = QString(
          "SELECT l.cod, l.descripcion, l.cantidad, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str "
          "FROM tickets_nube t "
          "INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CAST(t.ticket AS CHAR) "
          "WHERE t.cliente = :cliente AND %1 "
          "AND t.fecha BETWEEN :desde AND :hasta "
          "ORDER BY t.fecha DESC LIMIT %2")
                    .arg(condTiendaNube)
                    .arg(LIMITE_LINEAS_FECHA);
      qNube.prepare(sqlNube);
      qNube.bindValue(":cliente", codigoCliente);
      qNube.bindValue(":desde", ui->dateEditDesde_2->date().toString("yyyy-MM-dd"));
      qNube.bindValue(":hasta", ui->dateEditHasta_2->date().toString("yyyy-MM-dd"));
    }

    if (qNube.exec()) {
      while (qNube.next()) {
        QString cod = qNube.value(0).toString();
        QString desc = qNube.value(1).toString();

        QList<QStandardItem *> items;

        QStandardItem *itemCod = new QStandardItem(cod);
        itemCod->setData(cod, Qt::UserRole);
        items << itemCod;

        QStandardItem *itemDesc = new QStandardItem(desc);
        itemDesc->setData(desc, Qt::UserRole);
        items << itemDesc;

        double cant = qNube.value(2).toDouble();
        QStandardItem *itemCant = new QStandardItem();
        itemCant->setData(QVariant::fromValue(cant), Qt::UserRole);
        itemCant->setData(QString::number(cant, 'f', 2), Qt::DisplayRole);
        items << itemCant;

        QDate fecha = obtenerQDateValida(qNube.value(3));
        QString fechaStr = fecha.isValid() ? fecha.toString("yyyy-MM-dd") : qNube.value(3).toString();
        QStandardItem *itemFecha = new QStandardItem();
        itemFecha->setData(QVariant::fromValue(fecha), Qt::UserRole);
        itemFecha->setData(fechaStr, Qt::DisplayRole);
        items << itemFecha;

        modeloProductos->appendRow(items);
      }
    } else {
      qDebug() << "Clientes::on_radioButtonFecha_clicked Nube:" << qNube.lastError().text();
    }
  }

  proxyProductos->setSourceModel(modeloProductos);
  proxyProductos->setFilterFixedString(ui->lineEditBuscarProducto->text().trimmed());
  ui->tableViewProductos->setSortingEnabled(true);
  proxyProductos->setSortRole(Qt::UserRole);
  proxyProductos->sort(3, Qt::DescendingOrder);
  ui->tableViewProductos->resizeColumnsToContents();
}

// Este slot ya no existe; la lógica se consolidó en on_checkBoxTiendasConectadas_clicked()

void Clientes::on_lineEditBuscarProducto_textChanged(const QString &arg1) {
  // Filtrado instantáneo en memoria mediante QSortFilterProxyModel sin re-ejecutar consultas SQL
  proxyProductos->setFilterFixedString(arg1.trimmed());
}

void Clientes::on_dateEditDesde_2_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  if (ui->radioButtonCantidad->isChecked())
    on_radioButtonCantidad_clicked();
  else if (ui->radioButtonFecha->isChecked())
    on_radioButtonFecha_clicked();
}

void Clientes::on_dateEditHasta_2_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  if (ui->radioButtonCantidad->isChecked())
    on_radioButtonCantidad_clicked();
  else if (ui->radioButtonFecha->isChecked())
    on_radioButtonFecha_clicked();
}

void Clientes::on_btn_encargos_cliente_clicked() {
    QString codigo = ui->lineEditCod->text();
    if (codigo.isEmpty()) {
        QMessageBox::warning(this, "Aviso", "Seleccione un cliente primero");
        return;
    }
    GestorEncargosDialog dial(codigo, this);
    dial.exec();
}

void Clientes::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraClientes(), this);
    unif.exec();
    recargarTabla();
}

void Clientes::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraClientes(), this);
    saneador.exec();
    recargarTabla();
}
