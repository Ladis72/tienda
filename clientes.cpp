#include "clientes.h"
#include "buscarcliente.h"
#include "ui_clientes.h"
#include "saneadorglobal.h"
#include <QMessageBox>
#include <QToolTip>
#include "gestorencargosdialog.h"

Clientes::Clientes(QWidget *parent) : QDialog(parent), ui(new Ui::Clientes) {
  ui->setupUi(this);
  inicializarComponentes();
}

Clientes::Clientes(QWidget *parent, QString codigo)
    : QDialog(parent), ui(new Ui::Clientes) {
  ui->setupUi(this);
  inicializarComponentes();
  borrarFormulario();
  ui->lineEditCod->setText(codigo);
  ui->lineEditCod->setEnabled(false);
}

void Clientes::inicializarComponentes() {
  listaProductos = new QSqlQueryModel(this);
  modeloProductos = new QStandardItemModel(this);
  proxyProductos = new QSortFilterProxyModel(this);
  proxyProductos->setSourceModel(modeloProductos);
  proxyProductos->setSortRole(
      Qt::UserRole); // We'll use UserRole for all sorting data
  proxyProductos->setDynamicSortFilter(true);
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

  // Preparar el Gráfico
  ventasChart = new QChart();
  ventasChart->setTitle("Histórico de Ventas");
  ventasChart->setAnimationOptions(QChart::SeriesAnimations);
  ventasChartView = new QChartView(ventasChart, this);
  ventasChartView->setRenderHint(QPainter::Antialiasing);
  ui->verticalLayoutGrafico->addWidget(ventasChartView);

  ui->dateEditDesde->setDate(QDate::currentDate().addYears(-1));
  ui->dateEditHasta->setDate(QDate::currentDate());

  connect(ui->dateEditDesde, &QDateEdit::dateChanged, this,
          &Clientes::on_dateEditDesde_dateChanged);
  connect(ui->dateEditHasta, &QDateEdit::dateChanged, this,
          &Clientes::on_dateEditHasta_dateChanged);
  connect(ui->comboBoxAgrupacion,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &Clientes::on_comboBoxAgrupacion_currentIndexChanged);
  connect(ui->comboBoxTipoGrafico,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &Clientes::on_comboBoxTipoGrafico_currentIndexChanged);

  ui->dateEditDesde_2->setDate(QDate(QDate::currentDate().year(), 1, 1));
  ui->dateEditHasta_2->setDate(QDate::currentDate());
  ui->radioButtonCantidad->setChecked(true);
  connect(ui->dateEditDesde_2, &QDateEdit::dateChanged, this,
          &Clientes::on_dateEditDesde_2_dateChanged);
  connect(ui->dateEditHasta_2, &QDateEdit::dateChanged, this,
          &Clientes::on_dateEditHasta_2_dateChanged);
  // Checkbox unificado: aplica a Ventas y Productos simultáneamente
  connect(ui->checkBoxTiendasConectadas, &QCheckBox::clicked, this,
          &Clientes::on_checkBoxTiendasConectadas_clicked);
  connect(ui->lineEditBuscarProducto, &QLineEdit::textChanged, this,
          &Clientes::on_lineEditBuscarProducto_textChanged);
  ui->tableViewProductos->setSortingEnabled(true);

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

  recargarTabla();

  mapper.setCurrentIndex(0);
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
  mapper.toFirst();
  refrescarBotones(mapper.currentIndex());

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
}

Clientes::~Clientes() { delete ui; }

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

  QString codigoCliente =
      ui->lineEditCod->text(); // Evitar ejecutar si no hay cliente válido
  if (codigoCliente.isEmpty())
    return;

  double gastoTotal = 0.0;
  int cantidadTickets = 0;
  QDate ultimaVisita(1900, 1, 1);

  // Mapeo: [NombreTienda][Periodo] -> Total
  QMap<QString, QMap<QString, double>> ventasPorTiendaYPeriodo;
  QSet<QString> todosLosPeriodos;
  mapeoCategRango.clear();

  QStringList conexionesConsultar;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    conexionesConsultar = listaConexionesRemotas;
  } else {
    conexionesConsultar << nombreConexionLocal;
  }

  QDate desde = ui->dateEditDesde->date();
  QDate hasta = ui->dateEditHasta->date();
  QString agrupacion = ui->comboBoxAgrupacion->currentData().toString();
  if (agrupacion.isEmpty())
    agrupacion = "mes"; // Fallback por seguridad

  // Limpiar tablas y gráficos
  vistaTickets->clear();
  ui->tableView2->setModel(nullptr);
  ui->tableViewDetalleTicket->setModel(nullptr);

  // Llenar datos de tickets iterando las BDs (solo agregaciones)
  for (int c = 0; c < conexionesConsultar.length(); c++) {
    QString connName = conexionesConsultar.at(c);
    QSqlDatabase db = QSqlDatabase::database(connName);
    
    QString queryStr;
    if (mostrarVentasB && db.tables().contains("ticketss")) {
        QString subQuery = QString("SELECT fecha, total FROM tickets WHERE cliente = %1 "
                                   "UNION ALL "
                                   "SELECT fecha, total FROM ticketss WHERE cliente = %1").arg(codigoCliente);
        queryStr = "SELECT fecha, total FROM (" + subQuery + ") as sub "
                   "WHERE fecha >= '" + desde.toString("yyyy-MM-dd") + "' AND fecha <= '" + hasta.toString("yyyy-MM-dd") + "'";
    } else {
        queryStr = "SELECT fecha, total FROM tickets WHERE cliente = " + codigoCliente +
                   " AND fecha >= '" + desde.toString("yyyy-MM-dd") + "' AND fecha <= '" + hasta.toString("yyyy-MM-dd") + "'";
    }
        
    listaTickets->setQuery(queryStr, db);

    for (int i = 0; i < listaTickets->rowCount(); i++) {
      QDate fecha = listaTickets->record(i).value("fecha").toDate();
      double totalTicket = listaTickets->record(i).value("total").toDouble();

      // Sumar a KPIs
      gastoTotal += totalTicket;
      cantidadTickets++;
      if (fecha > ultimaVisita)
        ultimaVisita = fecha;

      // Sumar al gráfico según agrupación
      QString key;
      if (agrupacion == "ano")
        key = fecha.toString("yyyy");
      else if (agrupacion == "mes")
        key = fecha.toString("yyyy-MM");
      else if (agrupacion == "semana")
        key = QString::number(fecha.year()) + "-W" +
              QString::number(fecha.weekNumber());
      else
        key = fecha.toString("yyyy-MM-dd"); // dia

      ventasPorTiendaYPeriodo[connName][key] += totalTicket;
      todosLosPeriodos.insert(key);
    }
  }

  // Ordenar periodos para el eje X
  QStringList categoriasPeriodos = todosLosPeriodos.values();
  categoriasPeriodos.sort();
  mapeoCategRango = categoriasPeriodos;

  // Refrescar KPIs en la Interfaz
  ui->labelKpiTotalGasto->setText(QString::number(gastoTotal, 'f', 2) + " €");
  ui->labelKpiTotalTickets->setText(QString::number(cantidadTickets));
  if (cantidadTickets > 0) {
    ui->labelKpiFechaUltima->setText(ultimaVisita.toString("dd/MM/yyyy"));
  } else {
    ui->labelKpiFechaUltima->setText("-");
  }

  // Refrescar Gráfico
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

  int colorIdx = 0;
  for (const QString &tienda : conexionesConsultar) {
    if (!ventasPorTiendaYPeriodo.contains(tienda))
      continue;

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
  ui->tableView2->setSortingEnabled(true);
}

void Clientes::cargarTicketsPorRango(const QString &rangoMapeado) {
  vistaTickets->removeRows(0, vistaTickets->rowCount());
  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty())
    return;

  QStringList conexionesConsultar;
  if (ui->checkBoxTiendasConectadas->isChecked()) {
    conexionesConsultar = listaConexionesRemotas;
  } else {
    conexionesConsultar << nombreConexionLocal;
  }

  QString agrupacion = ui->comboBoxAgrupacion->currentData().toString();
  if (agrupacion.isEmpty())
    agrupacion = "mes";
  QDate desde = ui->dateEditDesde->date();
  QDate hasta = ui->dateEditHasta->date();

  for (int c = 0; c < conexionesConsultar.length(); c++) {
    QString connName = conexionesConsultar.at(c);
    QSqlDatabase db = QSqlDatabase::database(connName);
    
    QString queryStr;
    if (mostrarVentasB && db.tables().contains("ticketss")) {
        QString subQuery = QString("SELECT *, 'A' as tipo_sector FROM tickets WHERE cliente = %1 "
                                   "UNION ALL "
                                   "SELECT *, 'B' as tipo_sector FROM ticketss WHERE cliente = %1").arg(codigoCliente);
        queryStr = "SELECT * FROM (" + subQuery + ") as t "
                   "WHERE fecha >= '" + desde.toString("yyyy-MM-dd") + "' AND fecha <= '" + hasta.toString("yyyy-MM-dd") + "'";
    } else {
        queryStr = "SELECT *, 'A' as tipo_sector FROM tickets WHERE cliente = " + codigoCliente +
                   " AND fecha >= '" + desde.toString("yyyy-MM-dd") + "' AND fecha <= '" + hasta.toString("yyyy-MM-dd") + "'";
    }
        
    listaTickets->setQuery(queryStr, db);

    for (int i = 0; i < listaTickets->rowCount(); i++) {
      QDate fecha = listaTickets->record(i).value("fecha").toDate();

      QString key;
      if (agrupacion == "ano")
        key = fecha.toString("yyyy");
      else if (agrupacion == "mes")
        key = fecha.toString("yyyy-MM");
      else if (agrupacion == "semana")
        key = QString::number(fecha.year()) + "-W" +
              QString::number(fecha.weekNumber());
      else
        key = fecha.toString("yyyy-MM-dd");

      if (key != rangoMapeado)
        continue;

      listaItems.clear();
      listaItems << new QStandardItem(
          listaTickets->record(i).value("ticket").toString());
      listaItems << new QStandardItem(base.nombreUsusario(
          listaTickets->record(i).value("usuario").toString(), connName));
      listaItems << new QStandardItem(fecha.toString("dd-MM-yyyy"));
      listaItems << new QStandardItem(
          listaTickets->record(i).value("hora").toString());
      listaItems << new QStandardItem(
          listaTickets->record(i).value("descuento").toString());
      listaItems << new QStandardItem(QString::number(
          listaTickets->record(i).value("total").toDouble(), 'f', 2));
      listaItems << new QStandardItem(base.nombreFormaPago(
          listaTickets->record(i).value("fpago").toString(), connName));
      QString pagado =
          (listaTickets->record(i).value("cobrado").toString() == "1") ? "Sí"
                                                                       : "No";
      listaItems << new QStandardItem(pagado);
      listaItems << new QStandardItem(
          listaTickets->record(i).value("entrega").toString());
      listaItems << new QStandardItem(
          listaTickets->record(i).value("cambio").toString());
      listaItems << new QStandardItem(connName);
      
      // Columna oculta para el tipo de sector (A o B)
      QStandardItem *itemTipo = new QStandardItem(listaTickets->record(i).value("tipo_sector").toString());
      listaItems << itemTipo;
      
      vistaTickets->appendRow(listaItems);
      
      // Resaltar en rojo claro si es del sector B
      if (listaTickets->record(i).value("tipo_sector").toString() == "B") {
          for (int col = 0; col < listaItems.count(); ++col) {
              listaItems.at(col)->setBackground(QBrush(QColor(255, 235, 238)));
              listaItems.at(col)->setForeground(QBrush(QColor(183, 28, 28)));
          }
      }
    }
  }
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
  
  QSqlDatabase db = QSqlDatabase::database(dbName);
  QString tablaLineas = "lineasticket";
  if (sector == "B" && db.tables().contains("lineasticketss")) {
      tablaLineas = "lineasticketss";
  }

  ticket->setQuery("SELECT * FROM " + tablaLineas + " WHERE nticket = '" + nTicket +
                       "'",
                   db);

  ui->tableViewDetalleTicket->setModel(ticket);
  ui->tableViewDetalleTicket->hideColumn(0);
  ui->tableViewDetalleTicket->hideColumn(1);
  ui->tableViewDetalleTicket->hideColumn(2);
  ui->tableViewDetalleTicket->resizeColumnsToContents();
}

void Clientes::on_radioButtonCantidad_clicked() {
  ui->tableViewProductos->setSortingEnabled(false);
  modeloProductos->clear();
  QStringList labels;
  labels << "Código" << "Descripción" << "Cantidad";
  modeloProductos->setHorizontalHeaderLabels(labels);

  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty())
    return;

  QStringList conexiones;
  if (ui->checkBoxTiendasConectadas->isChecked())
    conexiones = listaConexionesRemotas;
  else
    conexiones << nombreConexionLocal;

  QMap<QString, QPair<QString, double>> totales; // cod -> (desc, cant)

  QString filtro = ui->lineEditBuscarProducto->text().toLower();

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

      if (!filtro.isEmpty() && !cod.toLower().contains(filtro) &&
          !desc.toLower().contains(filtro)) {
        continue;
      }

      totales[cod].first = desc;
      totales[cod].second += cant;
    }
  }

  for (auto it = totales.begin(); it != totales.end(); ++it) {
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
  ui->tableViewProductos->setSortingEnabled(true);
  proxyProductos->invalidate();
  proxyProductos->sort(2, Qt::DescendingOrder);
  ui->tableViewProductos->resizeColumnsToContents();
}

void Clientes::on_radioButtonFecha_clicked() {
  ui->tableViewProductos->setSortingEnabled(false);
  modeloProductos->clear();
  QStringList labels;
  labels << "Código" << "Descripción" << "Cantidad" << "Fecha";
  modeloProductos->setHorizontalHeaderLabels(labels);

  QString codigoCliente = ui->lineEditCod->text();
  if (codigoCliente.isEmpty())
    return;

  QStringList conexiones;
  if (ui->checkBoxTiendasConectadas->isChecked())
    conexiones = listaConexionesRemotas;
  else
    conexiones << nombreConexionLocal;

  QString filtro = ui->lineEditBuscarProducto->text().toLower();

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

      if (!filtro.isEmpty() && !cod.toLower().contains(filtro) &&
          !desc.toLower().contains(filtro)) {
        continue;
      }

      QList<QStandardItem *> items;

      QStandardItem *itemCod = new QStandardItem(cod);
      itemCod->setData(cod, Qt::UserRole);
      items << itemCod;

      QStandardItem *itemDesc = new QStandardItem(desc);
      itemDesc->setData(desc, Qt::UserRole);
      items << itemDesc;

      QStandardItem *itemCant = new QStandardItem();
      itemCant->setData(QVariant::fromValue(q.value(2).toDouble()),
                        Qt::UserRole);
      itemCant->setData(q.value(2).toString(), Qt::DisplayRole);
      items << itemCant;

      QStandardItem *itemFecha = new QStandardItem();
      QDate fecha = q.value(3).toDate();
      itemFecha->setData(QVariant::fromValue(fecha), Qt::UserRole);
      itemFecha->setData(fecha.toString("dd/MM/yyyy"), Qt::DisplayRole);
      items << itemFecha;

      modeloProductos->appendRow(items);
    }
  }
  ui->tableViewProductos->setSortingEnabled(true);
  proxyProductos->setSortRole(Qt::UserRole);
  proxyProductos->invalidate();
  proxyProductos->sort(3, Qt::DescendingOrder);
  ui->tableViewProductos->resizeColumnsToContents();
}

// Este slot ya no existe; la lógica se consolidó en on_checkBoxTiendasConectadas_clicked()

void Clientes::on_lineEditBuscarProducto_textChanged(const QString &arg1) {
  Q_UNUSED(arg1);
  if (ui->radioButtonCantidad->isChecked())
    on_radioButtonCantidad_clicked();
  else if (ui->radioButtonFecha->isChecked())
    on_radioButtonFecha_clicked();
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
