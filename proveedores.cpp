#include "proveedores.h"
#include "ui_proveedores.h"
#include <QMessageBox>

Proveedores::Proveedores(QWidget *parent)
    : QDialog(parent), ui(new Ui::Proveedores) {
  ui->setupUi(this);
  borrarFormulario();

  // 1. Inicializar variables de estado
  mostrarVentasB = false;
  modeloTabla = new QSqlQueryModel(this);
  mapper.setModel(modeloTabla);

  // 2. Inicializar Gráficos y UI adicional (DEBE IR ANTES DE CARGAR DATOS)
  chartViewCompras = new QChartView(this);
  chartViewCompras->setRenderHint(QPainter::Antialiasing);
  chartViewCompras->setMinimumHeight(250);
  if (ui->verticalLayoutCompras) {
    ui->verticalLayoutCompras->addWidget(chartViewCompras);
  }

  comboBoxVentasFiltro = new QComboBox(this);
  comboBoxVentasFiltro->addItem("Mayor Facturación", "Total_Euros");
  comboBoxVentasFiltro->addItem("Más Vendido (Unidades)", "Unidades");
  connect(comboBoxVentasFiltro,
          QOverload<int>::of(&QComboBox::currentIndexChanged), this,
          &Proveedores::cargarVentas);

  chartViewVentas = new QChartView(this);
  chartViewVentas->setRenderHint(QPainter::Antialiasing);
  chartViewVentas->setMinimumHeight(250);

  QFrame *separator = new QFrame(this);
  separator->setFrameShape(QFrame::HLine);
  separator->setFrameShadow(QFrame::Sunken);

  if (ui->verticalLayoutVentas) {
    ui->verticalLayoutVentas->addWidget(separator);
    QHBoxLayout *hLayoutFilter = new QHBoxLayout();
    hLayoutFilter->addWidget(new QLabel("Graficar por:", this));
    hLayoutFilter->addWidget(comboBoxVentasFiltro);
    hLayoutFilter->addStretch();
    ui->verticalLayoutVentas->addLayout(hLayoutFilter);
    ui->verticalLayoutVentas->addWidget(chartViewVentas);
  }

  // 3. Configuración de fechas y UI
  QDate hoy = QDate::currentDate();
  ui->dateEditDesde->setDate(QDate(hoy.year(), 1, 1));
  ui->dateEditHasta->setDate(hoy);
  setupIcons();

  // 4. Mapeos (IMPORTANTE: Después de setModel)
  mapper.addMapping(ui->lineEditCod, 0);
  mapper.addMapping(ui->lineEditNombre, 1);
  mapper.addMapping(ui->lineEditNIF, 2);
  mapper.addMapping(ui->lineEditDireccion, 3);
  mapper.addMapping(ui->lineEditCP, 4);
  mapper.addMapping(ui->lineEditLocalidad, 5);
  mapper.addMapping(ui->lineEditProvincia, 6);
  mapper.addMapping(ui->lineEditRepresentante, 7);
  mapper.addMapping(ui->lineEditTelefonoR, 8);
  mapper.addMapping(ui->lineEditMailR, 9);
  mapper.addMapping(ui->lineEditTelefono, 10);
  mapper.addMapping(ui->lineEditMail, 11);
  mapper.addMapping(ui->lineEditDescuento, 12);
  mapper.addMapping(ui->lineEditFechaUltimaCompra, 13);
  mapper.addMapping(ui->lineEditFormaPago, 14);
  mapper.addMapping(ui->plainTextEdit, 15);

  ui->lineEditCod->installEventFilter(this);

  // Checkbox remoto
  checkBoxRemoto = new QCheckBox("Cargar datos de tiendas remotas", this);
  checkBoxRemoto->setStyleSheet("font-weight: bold; color: #1565c0;");
  if (ui->horizontalLayoutHeader) {
    ui->horizontalLayoutHeader->insertWidget(2, checkBoxRemoto);
  }
  connect(checkBoxRemoto, &QCheckBox::toggled, this,
          &Proveedores::on_pushButtonRefrescar_clicked);

  connect(ui->tabWidget, &QTabWidget::currentChanged, this, [=](int index) {
    if (index == 1)
      cargarGraficoCompras();
    else if (index == 2)
      cargarGraficoVentas();
  });

  // 5. CARGA DE DATOS (AL FINAL)
  recargarTabla();
  mapper.toFirst();
  qDebug() << "Constructor: Index tras toFirst =" << mapper.currentIndex();
  if (mapper.currentIndex() >= 0) {
    qDebug() << "Valor en lineEditNombre:" << ui->lineEditNombre->text();
  }
  refrescarBotones(mapper.currentIndex());
}

void Proveedores::setupIcons() {
  if (ui->pushButtonNuevo)
    ui->pushButtonNuevo->setIcon(style()->standardIcon(QStyle::SP_FileIcon));
  if (ui->pushButtonModificar)
    ui->pushButtonModificar->setIcon(
        style()->standardIcon(QStyle::SP_DialogSaveButton));
  if (ui->pushButtonBorrar)
    ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
  if (ui->pushButtonAnterior)
    ui->pushButtonAnterior->setIcon(
        style()->standardIcon(QStyle::SP_ArrowBack));
  if (ui->pushButtonSiguiente)
    ui->pushButtonSiguiente->setIcon(
        style()->standardIcon(QStyle::SP_ArrowForward));
  if (ui->pushButtonCerrar)
    ui->pushButtonCerrar->setIcon(
        style()->standardIcon(QStyle::SP_DialogCloseButton));
  if (ui->pushButtonRefrescar)
    ui->pushButtonRefrescar->setIcon(
        style()->standardIcon(QStyle::SP_BrowserReload));

  ui->labelTotalVentasB->hide();
  ui->labelTotalVentasSum->hide();
}

Proveedores::~Proveedores() { delete ui; }

void Proveedores::recargarTabla() {
  QString connName = conf->getConexionLocal();
  QSqlDatabase db = QSqlDatabase::database(connName);

  if (!db.isOpen()) {
    qDebug() << "ERROR: La conexión a la base de datos" << connName
             << "no está abierta.";
    if (!db.open()) {
      qDebug() << "ERROR: No se pudo abrir la conexión:"
               << db.lastError().text();
      return;
    }
  }

  qDebug() << "Recargando tabla proveedores desde la base de datos:"
           << db.databaseName();

  modeloTabla->setQuery("SELECT * FROM proveedores", db);

  if (modeloTabla->lastError().isValid()) {
    qDebug() << "SQL Error Crítico en Proveedores:"
             << modeloTabla->lastError().text();
    qDebug() << "Query ejecutada:" << modeloTabla->query().lastQuery();
  }

  // Forzar la carga de todas las filas para que rowCount() sea preciso
  while (modeloTabla->canFetchMore()) {
    modeloTabla->fetchMore();
  }

  qDebug() << "Proveedores cargados:" << modeloTabla->rowCount();
  if (modeloTabla->rowCount() > 0) {
    qDebug() << "Primer proveedor en modelo:"
             << modeloTabla->record(0).value("nombre").toString();
  }
}

void Proveedores::borrarFormulario() {
  foreach (QLineEdit *le, ui->tab->findChildren<QLineEdit *>()) {
    le->clear();
  }
  ui->plainTextEdit->clear();
  ui->labelFPago->clear();
  ui->labelNombre->clear();
}

bool Proveedores::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->lineEditCod) {
    if (event->type() == QEvent::MouseButtonPress) {
      borrarFormulario();
      return true;
    }
  }
  return false;
}

void Proveedores::refrescarBotones(int i) {
  ui->pushButtonAnterior->setEnabled(i > 0);

  ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
  ui->labelNombre->setText(ui->lineEditNombre->text());
  cargarCompras();
  cargarVentas();
}

QStringList Proveedores::recogerDatosFormulario() {
  QStringList listaDatosFormulario;
  listaDatosFormulario.append(ui->lineEditCod->text());
  listaDatosFormulario.append(ui->lineEditNombre->text());
  listaDatosFormulario.append(ui->lineEditNIF->text());
  listaDatosFormulario.append(ui->lineEditDireccion->text());
  listaDatosFormulario.append(ui->lineEditCP->text());
  listaDatosFormulario.append(ui->lineEditLocalidad->text());
  listaDatosFormulario.append(ui->lineEditProvincia->text());
  listaDatosFormulario.append(ui->lineEditRepresentante->text());
  listaDatosFormulario.append(ui->lineEditTelefonoR->text());
  listaDatosFormulario.append(ui->lineEditMailR->text());
  listaDatosFormulario.append(ui->lineEditTelefono->text());
  listaDatosFormulario.append(ui->lineEditMail->text());
  listaDatosFormulario.append(ui->lineEditDescuento->text());
  if (ui->lineEditFechaUltimaCompra->text().isEmpty()) {
    listaDatosFormulario.append("2000-01-01");
  } else {
    listaDatosFormulario.append(ui->lineEditFechaUltimaCompra->text());
  }
  listaDatosFormulario.append(ui->lineEditFormaPago->text());
  listaDatosFormulario.append(ui->plainTextEdit->toPlainText());

  return listaDatosFormulario;
}

void Proveedores::cargarCompras() {
  QString idProv = ui->lineEditCod->text();
  if (idProv.isEmpty())
    return;

  QString desde = ui->dateEditDesde->date().toString("yyyy-MM-dd");
  QString hasta = ui->dateEditHasta->date().toString("yyyy-MM-dd");

  QStringList conexiones;
  conexiones << conf->getConexionLocal();
  if (checkBoxRemoto->isChecked()) {
    conexiones << conf->getNombreConexionesActivas();
  }

  modeloComprasGlobal.clear();

  // Configurar cabeceras según el tipo de agrupación
  if (ui->radioButtonComprasFacturas->isChecked()) {
    modeloComprasGlobal.setHorizontalHeaderLabels(
        {"Factura", "Fecha", "Base", "IVA", "Total", "Tienda"});
  } else if (ui->radioButtonComprasMeses->isChecked()) {
    modeloComprasGlobal.setHorizontalHeaderLabels(
        {"Año", "Mes", "Tienda", "Total"});
  } else {
    modeloComprasGlobal.setHorizontalHeaderLabels({"Año", "Tienda", "Total"});
  }

  // Estructura para desglose: periodo -> tienda -> total
  QMap<QString, QMap<QString, double>> desglose;

  QString nombreProv = ui->lineEditNombre->text().trimmed();
  foreach (QString conn, conexiones) {
    qDebug() << "Intentando cargar compras desde conexión:" << conn;
    QSqlDatabase db = QSqlDatabase::database(conn);
    if (!db.isOpen())
      continue;

    // Usamos el ID directamente como indica el usuario (confiando en la
    // sincronización de IDs)
    QString idLocal = ui->lineEditCod->text();
    if (idLocal.isEmpty())
      continue;

    QString query =
        QString(
            "SELECT * FROM facturas "
            "WHERE idProveedor = '%1' AND fechaFactura BETWEEN '%2' AND '%3' "
            "ORDER BY fechaFactura DESC")
            .arg(idLocal, desde, hasta);

    QSqlQuery q(db);
    if (q.exec(query)) {
      int count = 0;
      while (q.next()) {
        count++;
        if (ui->radioButtonComprasFacturas->isChecked()) {
          QList<QStandardItem *> row;
          row << new QStandardItem(q.value(1).toString()); // nFactura
          row << new QStandardItem(
              q.value(2).toDate().toString("yyyy-MM-dd")); // fecha
          row << new QStandardItem(
              QString::number(q.value(4).toDouble(), 'f', 2)); // base
          row << new QStandardItem(q.value(5).toString());     // iva
          row << new QStandardItem(
              QString::number(q.value(7).toDouble(), 'f', 2)); // total
          row << new QStandardItem(conn);
          modeloComprasGlobal.appendRow(row);
        } else {
          QDate f = q.value(2).toDate();
          QString clave = ui->radioButtonComprasMeses->isChecked()
                              ? QString("%1-%2").arg(f.year()).arg(
                                    f.month(), 2, 10, QChar('0'))
                              : QString::number(f.year());
          desglose[clave][conn] += q.value(7).toDouble();
        }
      }
      qDebug() << "Filas encontradas en" << conn << ":" << count;
    }
  }

  // Si hay agrupación, pasar del desglose al modelo con subtotales
  if (!ui->radioButtonComprasFacturas->isChecked()) {
    QStringList periodos = desglose.keys();
    std::sort(periodos.begin(), periodos.end(), std::greater<QString>());

    foreach (const QString &periodo, periodos) {
      double totalPeriodo = 0;
      QMap<QString, double> tiendas = desglose[periodo];

      // Fila por cada tienda
      foreach (const QString &tienda, tiendas.keys()) {
        double val = tiendas[tienda];
        totalPeriodo += val;

        QList<QStandardItem *> row;
        if (ui->radioButtonComprasMeses->isChecked()) {
          QStringList p = periodo.split("-");
          row << new QStandardItem(p.at(0)) << new QStandardItem(p.at(1));
        } else {
          row << new QStandardItem(periodo);
        }
        row << new QStandardItem(tienda);
        row << new QStandardItem(QString::number(val, 'f', 2));
        modeloComprasGlobal.appendRow(row);
      }

      // Fila de TOTAL para el periodo
      QList<QStandardItem *> totalRow;
      if (ui->radioButtonComprasMeses->isChecked()) {
        QStringList p = periodo.split("-");
        totalRow << new QStandardItem(p.at(0)) << new QStandardItem(p.at(1));
      } else {
        totalRow << new QStandardItem(periodo);
      }

      QStandardItem *itemTotalLabel =
          new QStandardItem("--- TOTAL PERIODO ---");
      itemTotalLabel->setData(QBrush(Qt::blue), Qt::ForegroundRole);
      QFont font = itemTotalLabel->font();
      font.setBold(true);
      itemTotalLabel->setFont(font);

      QStandardItem *itemTotalVal =
          new QStandardItem(QString::number(totalPeriodo, 'f', 2));
      itemTotalVal->setFont(font);
      itemTotalVal->setData(QBrush(Qt::blue), Qt::ForegroundRole);

      totalRow << itemTotalLabel << itemTotalVal;
      modeloComprasGlobal.appendRow(totalRow);
    }
  }

  ui->tableViewCompras->setModel(&modeloComprasGlobal);
  ui->tableViewCompras->resizeColumnsToContents();

  if (ui->tabWidget->currentIndex() == 1) {
    cargarGraficoCompras();
  }
}

void Proveedores::cargarVentas() {
  QString idProv = ui->lineEditCod->text();
  if (idProv.isEmpty())
    return;

  QString desde = ui->dateEditDesde->date().toString("yyyy-MM-dd");
  QString hasta = ui->dateEditHasta->date().toString("yyyy-MM-dd");

  QString nombreProv = ui->lineEditNombre->text().trimmed();
  QStringList conexiones;
  conexiones << conf->getConexionLocal();
  if (checkBoxRemoto->isChecked()) {
    conexiones << conf->getNombreConexionesActivas();
  }

  // Estructura para desglose: cod -> tienda -> datos
  QMap<QString, QMap<QString, VentasProd>> desglose;

  // 1. Recolectar TODOS los códigos de producto asociados a este proveedor en TODAS las tiendas
  QSet<QString> codigosProveedor;
  foreach (QString conn, conexiones) {
    QSqlDatabase db = QSqlDatabase::database(conn);
    if (!db.isOpen()) continue;
    
    QSqlQuery qCod(db);
    qCod.exec(QString("SELECT DISTINCT cod FROM lineaspedido WHERE idProveedor = '%1'").arg(idProv));
    while(qCod.next()) {
        codigosProveedor.insert(qCod.value(0).toString());
    }
  }

  if (codigosProveedor.isEmpty()) {
      // Si no hay compras registradas, no podemos saber qué artículos son del proveedor
      modeloVentasGlobal.clear();
      ui->tableViewVentas->setModel(&modeloVentasGlobal);
      return;
  }

  // Convertir a lista para SQL
  QString listadoCodigos = "'" + QStringList(codigosProveedor.values()).join("','") + "'";

  // 2. Consultar ventas en cada tienda para esos códigos
  foreach (QString conn, conexiones) {
    QSqlDatabase db = QSqlDatabase::database(conn);
    if (!db.isOpen())
      continue;

    QString subQuery = "SELECT cod, descripcion, cantidad, totallinea, fecha, "
                       "'A' as tipo FROM lineasticket";
    if (mostrarVentasB) {
      subQuery += " UNION ALL SELECT cod, descripcion, cantidad, totallinea, "
                  "fecha, 'B' as tipo FROM lineasticketss";
    }

    QString query =
        QString("SELECT lt.cod, lt.descripcion, "
                "SUM(lt.cantidad) as Unidades, "
                "SUM(CASE WHEN lt.tipo = 'A' THEN lt.totallinea ELSE 0 END) as Total_A, "
                "SUM(CASE WHEN lt.tipo = 'B' THEN lt.totallinea ELSE 0 END) as Total_B "
                "FROM (%1) lt "
                "WHERE lt.fecha BETWEEN '%2' AND '%3' "
                "AND lt.cod IN (%4) "
                "GROUP BY lt.cod, lt.descripcion")
            .arg(subQuery, desde, hasta, listadoCodigos);

    QSqlQuery q(db);
    if (q.exec(query)) {
      while (q.next()) {
        QString cod = q.value(0).toString();
        VentasProd &d = desglose[cod][conn];
        d.descripcion = q.value(1).toString();
        d.unidades += q.value(2).toDouble();
        d.totalA += q.value(3).toDouble();
        d.totalB += q.value(4).toDouble();
      }
    }
  }

  modeloVentasGlobal.clear();
  modeloVentasGlobal.setHorizontalHeaderLabels({"Codigo", "Descripcion",
                                                "Tienda", "Unidades", "Total A",
                                                "Total B", "Total"});

  double sumTotalA = 0, sumTotalB = 0;

  // Preparar ordenación según el filtro (0: Total €, 1: Unidades)
  bool ordenarPorUnid = (comboBoxVentasFiltro->currentIndex() == 1);
  QMap<QString, double> mapaSort;
  foreach (const QString &cod, desglose.keys()) {
    double valor = 0;
    foreach (const auto &dat, desglose[cod]) {
      valor += ordenarPorUnid ? dat.unidades : (dat.totalA + dat.totalB);
    }
    mapaSort[cod] = valor;
  }

  QStringList codigos = desglose.keys();
  std::sort(codigos.begin(), codigos.end(),
            [&](const QString &a, const QString &b) {
              return mapaSort[a] > mapaSort[b]; // De mayor a menor
            });

  foreach (const QString &cod, codigos) {
    double pTotalUnid = 0, pTotalA = 0, pTotalB = 0;
    QString desc;
    QMap<QString, VentasProd> tiendas = desglose[cod];

    foreach (const QString &tienda, tiendas.keys()) {
      const auto &dat = tiendas[tienda];
      desc = dat.descripcion;
      pTotalUnid += dat.unidades;
      pTotalA += dat.totalA;
      pTotalB += dat.totalB;

      QList<QStandardItem *> row;
      row << new QStandardItem(cod) << new QStandardItem(desc)
          << new QStandardItem(tienda);
      row << new QStandardItem(QString::number(dat.unidades, 'f', 2));
      row << new QStandardItem(QString::number(dat.totalA, 'f', 2));
      row << new QStandardItem(QString::number(dat.totalB, 'f', 2));
      row << new QStandardItem(
          QString::number(dat.totalA + dat.totalB, 'f', 2));
      modeloVentasGlobal.appendRow(row);
    }

    // Fila TOTAL PRODUCTO
    QList<QStandardItem *> totalRow;
    totalRow << new QStandardItem(cod) << new QStandardItem(desc);
    QStandardItem *itemTotalLabel = new QStandardItem("--- TOTAL PRODUCTO ---");
    itemTotalLabel->setData(QBrush(Qt::blue), Qt::ForegroundRole);
    QFont font = itemTotalLabel->font();
    font.setBold(true);
    itemTotalLabel->setFont(font);

    totalRow << itemTotalLabel;
    totalRow << new QStandardItem(QString::number(pTotalUnid, 'f', 2));
    totalRow << new QStandardItem(QString::number(pTotalA, 'f', 2));
    totalRow << new QStandardItem(QString::number(pTotalB, 'f', 2));
    totalRow << new QStandardItem(QString::number(pTotalA + pTotalB, 'f', 2));

    foreach (QStandardItem *item, totalRow) {
      item->setFont(font);
      item->setData(QBrush(Qt::blue), Qt::ForegroundRole);
    }
    modeloVentasGlobal.appendRow(totalRow);
    sumTotalA += pTotalA;
    sumTotalB += pTotalB;
  }

  ui->tableViewVentas->setModel(&modeloVentasGlobal);
  ui->tableViewVentas->setColumnHidden(4, !mostrarVentasB);
  ui->tableViewVentas->setColumnHidden(5, !mostrarVentasB);
  ui->tableViewVentas->resizeColumnsToContents();

  actualizarTotalesVentas(sumTotalA, sumTotalB);

  if (ui->tabWidget->currentIndex() == 2) {
    cargarGraficoVentas();
  }
}

void Proveedores::actualizarTotalesVentas(double totalA, double totalB) {
  ui->labelTotalVentasA->setText(
      QString("Ventas A: %1 €").arg(QString::number(totalA, 'f', 2)));
  ui->labelTotalVentasB->setText(
      QString("Ventas B: %1 €").arg(QString::number(totalB, 'f', 2)));
  ui->labelTotalVentasSum->setText(
      QString("Total: %1 €").arg(QString::number(totalA + totalB, 'f', 2)));

  ui->labelTotalVentasB->setVisible(mostrarVentasB);
  ui->labelTotalVentasSum->setVisible(mostrarVentasB);
}

void Proveedores::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_F11) {
    ui->pushButtonBorrar->setEnabled(true);
  } else if (e->key() == Qt::Key_F2) {
    mostrarVentasB = !mostrarVentasB;
    ui->tableViewVentas->setColumnHidden(4, !mostrarVentasB);
    ui->tableViewVentas->setColumnHidden(5, !mostrarVentasB);
    cargarVentas(); // Recargar para actualizar totales visibles
  }
}

void Proveedores::on_pushButtonRefrescar_clicked() {
  cargarCompras();
  cargarVentas();
}

void Proveedores::on_dateEditDesde_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  cargarCompras();
  cargarVentas();
}

void Proveedores::on_dateEditHasta_dateChanged(const QDate &date) {
  Q_UNUSED(date);
  cargarCompras();
  cargarVentas();
}

void Proveedores::on_pushButtonCerrar_clicked() { close(); }

void Proveedores::on_pushButtonNuevo_clicked() {
  if (base.existeDatoEnTabla(QSqlDatabase::database("DB"), "proveedores",
                             "idProveedor", ui->lineEditCod->text())) {
    QMessageBox::warning(this, "ATENCION", "El registro ya existe");
    return;
  }
  QStringList datos = recogerDatosFormulario();
  if (base.crearProveedor(QSqlDatabase::database("DB"), datos)) {
    QMessageBox::about(this, "Atención", "Proveedor creado con éxito");
  } else {
    QMessageBox::warning(this, "Error", "No se ha podido crear el proveedor.");
  }
  recargarTabla();
}

void Proveedores::on_pushButtonAnterior_clicked() {
  mapper.toPrevious();
  refrescarBotones(mapper.currentIndex());
}

void Proveedores::on_pushButtonSiguiente_clicked() {
  mapper.toNext();
  refrescarBotones(mapper.currentIndex());
}

void Proveedores::on_lineEditNombre_returnPressed() {
  BuscarProveedor *buscaProveedor =
      new BuscarProveedor(this, ui->lineEditNombre->text());
  buscaProveedor->exec();
  for (int i = 0; i < modeloTabla->rowCount(); ++i) {
    if (modeloTabla->record(i).value("idProveedor").toString() ==
        buscaProveedor->resultado) {
      mapper.setCurrentIndex(i);
      refrescarBotones(i);
      break;
    }
  }
  delete buscaProveedor;
}

void Proveedores::on_pushButtonModificar_clicked() {
  QStringList datos = recogerDatosFormulario();
  int i = mapper.currentIndex();
  QMessageBox msgBox;
  msgBox.setText("MODIFICACION.");
  msgBox.setInformativeText("Quiere guardar los cambios?");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  int resp = msgBox.exec();
  if (resp == QMessageBox::Ok) {
    if (base.modificarProveedor(QSqlDatabase::database("DB"), datos,
                                ui->lineEditCod->text())) {
      msgBox.setText("Guardado con exito");
      msgBox.setInformativeText("El registro se ha modificado correctamente");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    } else {
      msgBox.setText("Error al guardar");
      msgBox.setInformativeText(
          "Revise los datos del formulario o contacte con el administrador");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
  }
  recargarTabla();
  mapper.setCurrentIndex(i);
}

void Proveedores::on_lineEditFormaPago_textChanged(const QString &arg1) {
  ui->labelFPago->setText(base.nombreFormaPago(arg1, conf->getConexionLocal()));
}

void Proveedores::on_pushButtonFPago_clicked() {
  FormasPago *fpago = new FormasPago(this);
  fpago->exec();
  if (fpago->result() > 0) {
    ui->lineEditFormaPago->setText(fpago->resultado);
  }
}

void Proveedores::on_lineEditCod_editingFinished() {
  for (int i = 0; i < modeloTabla->rowCount(); ++i) {
    if (modeloTabla->record(i).value("idProveedor").toString() ==
        ui->lineEditCod->text()) {
      mapper.setCurrentIndex(i);
      refrescarBotones(i);
      break;
    }
  }
}

void Proveedores::on_pushButton_clicked() {
  borrarFormulario();
  ui->lineEditCod->setText(base.codigoParaNuevoProveedor());
}

void Proveedores::on_radioButtonComprasFacturas_clicked() { cargarCompras(); }

void Proveedores::on_radioButtonComprasMeses_clicked() { cargarCompras(); }

void Proveedores::on_radioButtonComprasAnos_clicked() { cargarCompras(); }

void Proveedores::on_pushButtonBorrar_clicked() {
  int i = mapper.currentIndex();
  QMessageBox msgBox;
  msgBox.setText("Borrar.");
  msgBox.setInformativeText("Seguro que quiere borrar este proveedor?");
  msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Ok);
  int resp = msgBox.exec();
  if (resp == QMessageBox::Ok) {
    if (base.borrarProveedor(QSqlDatabase::database("DB"),
                             ui->lineEditCod->text())) {
      msgBox.setText("Borrado con exito");
      msgBox.setInformativeText("El registro se ha borrado correctamente");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    } else {
      msgBox.setText("Error al borrar");
      msgBox.setInformativeText(
          "Revise los datos del formulario o contacte con el administrador");
      msgBox.setStandardButtons(QMessageBox::Ok);
      msgBox.exec();
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(mapper.currentIndex());
  }
}

void Proveedores::cargarGraficoCompras() {
  QAbstractItemModel *model = ui->tableViewCompras->model();
  if (!model || model->rowCount() == 0) {
    chartViewCompras->setChart(new QChart());
    chartViewCompras->chart()->setTitle("Sin datos para graficar");
    return;
  }

  QChart *chart = new QChart();
  QLineSeries *seriesLine = new QLineSeries();
  QBarSeries *seriesBar = new QBarSeries();
  QStringList categories;

  int rowCount = model->rowCount();

  if (ui->radioButtonComprasFacturas->isChecked()) {
    // Individual invoices (Line Chart)
    // Sort chronologically (model is usually Desc)
    QList<QPair<QDate, double>> data;
    for (int i = 0; i < rowCount; ++i) {
      QDate fecha =
          QDate::fromString(model->index(i, 1).data().toString(), "yyyy-MM-dd");
      double total = model->index(i, 4).data().toDouble();
      data.append(qMakePair(fecha, total));
    }
    std::sort(data.begin(), data.end(),
              [](const QPair<QDate, double> &a, const QPair<QDate, double> &b) {
                return a.first < b.first;
              });

    for (const auto &point : data) {
      seriesLine->append(point.first.toJulianDay(), point.second);
    }

    chart->addSeries(seriesLine);
    chart->setTitle("Evolución de Compras Individuales");

    QDateTimeAxis *axisX = new QDateTimeAxis;
    axisX->setFormat("dd MMM yyyy");
    chart->addAxis(axisX, Qt::AlignBottom);
    seriesLine->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis;
    chart->addAxis(axisY, Qt::AlignLeft);
    seriesLine->attachAxis(axisY);
  } else {
    // Estructura para agrupar: periodo -> (tienda -> valor)
    QMap<QString, QMap<QString, double>> matrix;
    QStringList periodos;
    QStringList tiendas;

    // Determinar columnas según vista
    int colTienda = ui->radioButtonComprasMeses->isChecked() ? 2 : 1;
    int colTotal  = ui->radioButtonComprasMeses->isChecked() ? 3 : 2;

    for (int i = 0; i < rowCount; ++i) {
        QString labelTienda = model->index(i, colTienda).data().toString();
        if (labelTienda == "--- TOTAL PERIODO ---") continue;

        QString labelPeriodo;
        if (ui->radioButtonComprasMeses->isChecked()) {
            labelPeriodo = QString("%1-%2")
                        .arg(model->index(i, 0).data().toString())
                        .arg(model->index(i, 1).data().toString());
        } else {
            labelPeriodo = model->index(i, 0).data().toString();
        }

        if (!periodos.contains(labelPeriodo)) periodos.prepend(labelPeriodo); // Cronológico
        if (!tiendas.contains(labelTienda)) tiendas << labelTienda;

        matrix[labelPeriodo][labelTienda] += model->index(i, colTotal).data().toDouble();
    }

    // Crear series de barras por tienda
    QMap<QString, QBarSet*> sets;
    foreach(const QString &t, tiendas) {
        sets[t] = new QBarSet(t);
        foreach(const QString &p, periodos) {
            *sets[t] << matrix[p].value(t, 0.0);
        }
        seriesBar->append(sets[t]);
    }
    categories = periodos;

    chart->addSeries(seriesBar);
    chart->setTitle(ui->radioButtonComprasMeses->isChecked()
                        ? "Desglose de Compras por Meses"
                        : "Desglose de Compras por Años");

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    seriesBar->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    seriesBar->attachAxis(axisY);
  }

  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  QChart *oldChart = chartViewCompras->chart();
  chartViewCompras->setChart(chart);
  if (oldChart) {
    delete oldChart;
  }
}

void Proveedores::cargarGraficoVentas() {
  QAbstractItemModel *model = ui->tableViewVentas->model();
  if (!model || model->rowCount() == 0) {
    chartViewVentas->setChart(new QChart());
    chartViewVentas->chart()->setTitle("Sin datos de ventas para graficar");
    return;
  }

  int rowCount = model->rowCount();
  int colValor = (comboBoxVentasFiltro->currentIndex() == 1) ? 3 : 6;

  // 1. Extraer los Totales por Producto para identificar el Top 5
  struct VentaObj {
    QString cod;
    QString desc;
    double val;
  };
  QList<VentaObj> listaTop;
  for (int i = 0; i < rowCount; ++i) {
    if (model->index(i, 2).data().toString() == "--- TOTAL PRODUCTO ---") {
      listaTop.append({model->index(i, 0).data().toString(),
                       model->index(i, 1).data().toString(),
                       model->index(i, colValor).data().toDouble()});
    }
  }

  // Ordenar de mayor a menor según el filtro seleccionado
  std::sort(listaTop.begin(), listaTop.end(),
            [](const VentaObj &a, const VentaObj &b) { return a.val > b.val; });

  int limit = qMin(5, (int)listaTop.size());
  if (limit <= 0) return;

  // 2. Preparar el Gráfico
  QChart *chart = new QChart();
  QBarSeries *series = new QBarSeries();
  QStringList categories;
  QMap<QString, QBarSet *> shopSets;

  // Recolectar nombres de tiendas activas en el modelo para este Top
  QStringList tiendasPresentes;
  for (int i = 0; i < rowCount; ++i) {
      QString t = model->index(i, 2).data().toString();
      if (t != "--- TOTAL PRODUCTO ---" && !tiendasPresentes.contains(t)) {
          tiendasPresentes << t;
      }
  }

  // Inicializar BarSets por tienda
  foreach (const QString &t, tiendasPresentes) {
      shopSets[t] = new QBarSet(t);
  }

  // 3. Rellenar datos para el Top 5
  for (int i = 0; i < limit; ++i) {
    QString cod = listaTop.at(i).cod;
    QString desc = listaTop.at(i).desc;
    if (desc.length() > 20) desc = desc.left(17) + "...";
    categories << desc;

    // Buscar ventas de este producto en cada tienda
    QMap<QString, double> ventasPorTienda;
    for (int j = 0; j < rowCount; ++j) {
        if (model->index(j, 0).data().toString() == cod) {
            QString tienda = model->index(j, 2).data().toString();
            if (tienda != "--- TOTAL PRODUCTO ---") {
                ventasPorTienda[tienda] = model->index(j, colValor).data().toDouble();
            }
        }
    }

    // Añadir valor a cada set (0 si la tienda no lo vendió)
    foreach (const QString &t, tiendasPresentes) {
        *shopSets[t] << ventasPorTienda.value(t, 0.0);
    }
  }

  // 4. Añadir series y configurar ejes
  foreach (QBarSet *set, shopSets.values()) {
    series->append(set);
  }

  chart->addSeries(series);
  chart->setTitle("Top " + QString::number(limit) + " Productos Vendidos por Tienda (" +
                  comboBoxVentasFiltro->currentText() + ")");

  QBarCategoryAxis *axisX = new QBarCategoryAxis();
  axisX->append(categories);
  chart->addAxis(axisX, Qt::AlignBottom);
  series->attachAxis(axisX);

  QValueAxis *axisY = new QValueAxis();
  axisY->setTitleText(colValor == 3 ? "Unidades" : "Euros");
  chart->addAxis(axisY, Qt::AlignLeft);
  series->attachAxis(axisY);

  chart->setAnimationOptions(QChart::SeriesAnimations);
  chart->legend()->setVisible(true);
  chart->legend()->setAlignment(Qt::AlignBottom);

  QChart *oldChart = chartViewVentas->chart();
  chartViewVentas->setChart(chart);
  if (oldChart) delete oldChart;
}
