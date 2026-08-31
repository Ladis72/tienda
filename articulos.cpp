#include "articulos.h"
#include "conexion.h"
#include "familias.h"
#include "graficoventaswidget.h"
#include "historialprecios.h"
#include "imprimirfacturaproveedor.h"
#include "ui_articulos.h"
#include "buscarpornotas.h"

#include "dialoganadirapedido.h"
#include "dialoganadirasalidas.h"
#include "dialogcambiocodigo.h"
#include "dialogcomparararticulos.h"
#include "dialoggenerardescripcionia.h"
#include "dialogbuscarfotointernet.h"
#include <QAction>
#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QInputDialog>
#include <QList>
#include <QMessageBox>
#include <QMimeData>
#include <QTextCharFormat>
#include <QTextCursor>
#include <QToolBar>
#include <QVBoxLayout>
#include <QtConcurrent/QtConcurrent>

// Escapa un literal de cadena MySQL para usarlo dentro de un setFilter
// de QSqlTableModel (que no admite parámetros enlazados).
static QString escSQL(const QString &s) {
  QString r = s;
  r.replace("\\", "\\\\");
  r.replace("'", "''");
  return r;
}

Articulos::Articulos(QWidget *parent) : QDialog(parent), ui(new Ui::Articulos) {
  listaConexionesRemotas = conf->getNombreConexionesActivas();
  qDebug() << "Lista conexiones remotas: ";
  qDebug() << listaConexionesRemotas;
  ui->setupUi(this);

  // Configuración de iconos profesionales para los botones
  ui->pushButtonNuevo->setIcon(
      style()->standardIcon(QStyle::SP_FileDialogNewFolder));
  ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
  ui->pushButtonModificar->setIcon(
      style()->standardIcon(QStyle::SP_DialogSaveButton));
  ui->pushButtonRefrescar->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
  ui->pushButtonAnterior->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
  ui->pushButtonSiguiente->setIcon(
      style()->standardIcon(QStyle::SP_ArrowRight));
  ui->pushButtonCambiarCodigo->setIcon(
      style()->standardIcon(QStyle::SP_BrowserReload));

  ui->pushButtonCerrar->setIcon(
      style()->standardIcon(QStyle::SP_DialogCloseButton));

  // Iconos para los botones de búsqueda (Familia y Fabricante)
  ui->pushButtonBuscarFamilia->setIcon(
      style()->standardIcon(QStyle::SP_FileDialogContentsView));
  ui->pushButtonBuscarFabricante->setIcon(
      style()->standardIcon(QStyle::SP_FileDialogContentsView));

  ClickableLabel *fotoHR = new ClickableLabel(ui->labelFoto);
  fotoHR->setMinimumSize(200, 200);
  connect(fotoHR, SIGNAL(clicked()), this, SLOT(mostrarFoto()));
  llenarComboFormatos();
  modeloTabla = new QSqlQueryModel;
  recargarTabla();

  mapper.setCurrentIndex(0);
  mapper.addMapping(ui->lineEditCod, 0);
  mapper.addMapping(ui->lineEditDesc, 1);
  mapper.addMapping(ui->lineEditPvp, 2);
  mapper.addMapping(ui->lineEditIva, 3);
  mapper.addMapping(ui->lineEditMinimo, 5);
  mapper.addMapping(ui->lineEditMaximo, 6);
  mapper.addMapping(ui->lineEditEncargados, 8);
  mapper.addMapping(ui->lineEditPendientes, 7);
  mapper.addMapping(ui->dateEditUltimaVenta, 9);
  mapper.addMapping(ui->dateEditUltimoPedido, 10);
  mapper.addMapping(ui->lineEditCodFamila, 11);
  mapper.addMapping(ui->lineEditCosto, 12);
  mapper.addMapping(ui->lineEditCodFabricante, 13);
  mapper.addMapping(ui->lineEditFoto, 14);
  // NOTA: textEditNotas (notas HTML) se carga/guarda manualmente en
  //       refrescarBotones() y recogerDatosFormulario() respectivamente.
  //       QDataWidgetMapper no gestiona bien HTML en QTextEdit.
  mapper.addMapping(ui->comboBoxFormato, 16);
  mapper.addMapping(ui->lineEditCantidad, 17);

  // ── Barra de herramientas de formato para las notas ──────────────────────
  // Se inserta dinámicamente encima del QTextEdit usando el layout de la
  // fila 9. QToolBar necesita un QWidget padre; aquí usamos el widget
  // «General».
  QToolBar *tbNotas = new QToolBar(ui->General);
  tbNotas->setObjectName("toolBarNotas");
  tbNotas->setIconSize(QSize(16, 16));
  tbNotas->setStyleSheet(
      "QToolBar { background: #fff9f0; border: 1px solid #ffe0b2; "
      "           border-radius: 4px; padding: 2px; spacing: 4px; }"
      "QToolButton { padding: 2px 6px; font-weight: bold; }"
      "QToolButton:checked { background: #ffe0b2; border-radius: 3px; }");

  // Botón Negrita
  QAction *actBold = tbNotas->addAction("B");
  actBold->setCheckable(true);
  actBold->setToolTip(tr("Negrita (Ctrl+B)"));
  QFont fBold = actBold->font();
  fBold.setBold(true);
  actBold->setFont(fBold);
  connect(actBold, &QAction::triggered, this, [this](bool on) {
    QTextCharFormat fmt;
    fmt.setFontWeight(on ? QFont::Bold : QFont::Normal);
    ui->textEditNotas->mergeCurrentCharFormat(fmt);
  });

  // Botón Cursiva
  QAction *actItalic = tbNotas->addAction("I");
  actItalic->setCheckable(true);
  actItalic->setToolTip(tr("Cursiva (Ctrl+I)"));
  QFont fItalic = actItalic->font();
  fItalic.setItalic(true);
  actItalic->setFont(fItalic);
  connect(actItalic, &QAction::triggered, this, [this](bool on) {
    QTextCharFormat fmt;
    fmt.setFontItalic(on);
    ui->textEditNotas->mergeCurrentCharFormat(fmt);
  });

  // Botón Subrayado
  QAction *actUnder = tbNotas->addAction("U");
  actUnder->setCheckable(true);
  actUnder->setToolTip(tr("Subrayado (Ctrl+U)"));
  QFont fUnder = actUnder->font();
  fUnder.setUnderline(true);
  actUnder->setFont(fUnder);
  connect(actUnder, &QAction::triggered, this, [this](bool on) {
    QTextCharFormat fmt;
    fmt.setFontUnderline(on);
    ui->textEditNotas->mergeCurrentCharFormat(fmt);
  });

  tbNotas->addSeparator();

  // Botón: Pegar como texto plano (sin formato)
  QAction *actPastePlain = tbNotas->addAction(tr("Pegar sin formato"));
  actPastePlain->setToolTip(
      tr("Pega el texto del portapapeles eliminando todo el formato"));
  connect(actPastePlain, &QAction::triggered, this, [this]() {
    const QMimeData *md = QApplication::clipboard()->mimeData();
    if (md && md->hasText()) {
      ui->textEditNotas->insertPlainText(md->text());
    }
  });

  tbNotas->addSeparator();

  // Botón: Limpiar todo el formato de la selección
  QAction *actClearFmt = tbNotas->addAction(tr("Limpiar formato"));
  actClearFmt->setToolTip(tr("Elimina el formato del texto seleccionado"));
  connect(actClearFmt, &QAction::triggered, this, [this]() {
    QTextCursor cur = ui->textEditNotas->textCursor();
    if (cur.hasSelection()) {
      QTextCharFormat fmt;
      // Al no establecer FontWeight, mergeCharFormat mantendrá el estado actual
      // (negrita o normal)
      fmt.setFontItalic(false);
      fmt.setFontUnderline(false);
      fmt.clearProperty(QTextFormat::ForegroundBrush);
      fmt.clearProperty(QTextFormat::BackgroundBrush);
      fmt.clearProperty(QTextFormat::FontFamily);
      fmt.clearProperty(QTextFormat::FontPointSize);

      // mergeCharFormat mezcla el formato nuevo con el existente en lugar de
      // reemplazarlo totalmente
      cur.mergeCharFormat(fmt);
    }
  });

  // Sincronizar estado de los botones de formato con el cursor actual
  connect(ui->textEditNotas, &QTextEdit::currentCharFormatChanged, this,
          [actBold, actItalic, actUnder](const QTextCharFormat &fmt) {
            actBold->setChecked(fmt.fontWeight() >= QFont::Bold);
            actItalic->setChecked(fmt.fontItalic());
            actUnder->setChecked(fmt.fontUnderline());
          });

  // Insertar la toolbar de formato en la cabecera de notas
  if (ui->horizontalLayoutCabeceraNotas) {
    ui->horizontalLayoutCabeceraNotas->insertWidget(1, tbNotas);
  }

  mapper.toFirst();

  remoto = false;

  refrescarBotones(mapper.currentIndex());

  ui->lineEditCod->installEventFilter(this);
  borrarFormulario();
  ui->lineEditCod->setFocus();
  graficoVentas = new GraficoVentasWidget(this);
  ui->layOutVentas->addWidget(graficoVentas);

  aplicarPermisos();
}

Articulos::~Articulos() { delete ui; }

/**
 * @brief Aplica las restricciones de permisos a los botones del formulario de artículos.
 */
void Articulos::aplicarPermisos() {
  if (!conf || !conf->permisos())
    return;

  ui->pushButtonNuevo->setEnabled(conf->permisos()->tiene("articulos.crear"));
  ui->pushButtonModificar->setEnabled(conf->permisos()->tiene("articulos.modificar"));
  ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("articulos.borrar"));
  ui->pushButtonCambiarCodigo->setEnabled(conf->permisos()->tiene("articulos.cambiar_codigo"));
  ui->pushButtonHistorialPrecios->setEnabled(conf->permisos()->tiene("articulos.historial_precios"));
  ui->pushButtonTrazabilidad->setEnabled(conf->permisos()->tiene("articulos.trazabilidad"));
  ui->pushButtonPonerFoto->setEnabled(conf->permisos()->tiene("articulos.modificar"));
  ui->pushButtonBuscarFotoInternet->setEnabled(conf->permisos()->tiene("articulos.modificar"));
  ui->pushButtonBorrarFoto->setEnabled(conf->permisos()->tiene("articulos.modificar"));
  ui->pushButtonGenerarDescripcionIA->setEnabled(
      conf->permisos()->tiene("articulos.modificar") || conf->permisos()->tiene("articulos.crear"));
}

void Articulos::refrescarBotones(int i) {
  ui->pushButtonAnterior->setEnabled(i > 0);
  ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
  ui->pushButtonCambiarCodigo->setEnabled(false);
  QString fichero = base.resolverRutaImagen(ui->lineEditFoto->text());
  QImage foto(fichero);
  if (foto.isNull()) {
    qDebug() << "Error cargando imagen:" << fichero;
  }
  QPixmap imagen = QPixmap::fromImage(foto);
  QPixmap imagenAjustada = imagen.scaled(200, 200, Qt::KeepAspectRatio);

  ui->labelFoto->setPixmap(imagenAjustada);

  // ── Cargar notas como HTML en el editor de texto enriquecido ─────────────
  // El campo notas se lee directamente del modelo para esta fila.
  // Como el mapper ya no gestiona textEditNotas, lo hacemos aquí manualmente.
  int filaActual = mapper.currentIndex();
  if (filaActual >= 0 && modeloTabla && filaActual < modeloTabla->rowCount()) {
    QString notasRaw =
        modeloTabla->record(filaActual).value("notas").toString();
    // Si el contenido parece HTML (empieza con '<'), cargarlo como tal;
    // de lo contrario tratarlo como texto plano para retrocompatibilidad.
    if (notasRaw.trimmed().startsWith('<')) {
      ui->textEditNotas->setHtml(notasRaw);
    } else {
      ui->textEditNotas->setPlainText(notasRaw);
    }
  }

  // Buscar excepción de precio local para mostrarla en el formulario
  QSqlRecord registroConOverride = base.consulta_producto(
      conf->getConexionCommon(), ui->lineEditCod->text());
  if (!registroConOverride.isEmpty()) {
    ui->lineEditPvp->setText(registroConOverride.value("pvp").toString());
  }

  ui->labelNombreProducto->setText(ui->lineEditDesc->text());
  ui->labelPrecioGrande->setText(ui->lineEditPvp->text() + " €");
  ui->lineEditStock->setText(base.sumarStockArticulo(ui->lineEditCod->text(),
                                                     conf->getConexionLocal()));

  double sumEntrada = 0.0;
  double sumSalida = 0.0;
  double sumPedido = 0.0;
  QString codArticulo = ui->lineEditCod->text();

  QSqlQuery qEntrada(QSqlDatabase::database(conf->getConexionLocal()));
  qEntrada.prepare("SELECT sum(cantidad) FROM entradaGenero_tmp WHERE cod = ?");
  qEntrada.bindValue(0, codArticulo);
  if (qEntrada.exec() && qEntrada.next())
    sumEntrada = qEntrada.value(0).toDouble();

  QSqlQuery qSalida(QSqlDatabase::database(conf->getConexionLocal()));
  qSalida.prepare("SELECT sum(cantidad) FROM salidaGenero_tmp WHERE cod = ?");
  qSalida.bindValue(0, codArticulo);
  if (qSalida.exec() && qSalida.next())
    sumSalida = qSalida.value(0).toDouble();

  QSqlQuery qPedido(QSqlDatabase::database(conf->getConexionLocal()));
  qPedido.prepare("SELECT sum(cantidad + bonificacion) FROM lineaspedido_tmp "
                  "WHERE cod = ?");
  qPedido.bindValue(0, codArticulo);
  if (qPedido.exec() && qPedido.next())
    sumPedido = qPedido.value(0).toDouble();

  QStringList resumenTmp;
  if (sumEntrada > 0)
    resumenTmp << QString("E: %1").arg(sumEntrada);
  if (sumSalida > 0)
    resumenTmp << QString("S: %1").arg(sumSalida);
  if (sumPedido > 0)
    resumenTmp << QString("P: %1").arg(sumPedido);

  if (!resumenTmp.isEmpty()) {
    ui->lineEditPendientes->setText(resumenTmp.join(" | "));
    // Cambiar color según lo que haya (usamos ámbar si hay mezcla, o colores
    // específicos si solo hay uno)
    if (resumenTmp.size() > 1) {
      ui->lineEditPendientes->setStyleSheet("color: #d97706;"); // Naranja/Ámbar
    } else if (sumEntrada > 0 || sumPedido > 0) {
      ui->lineEditPendientes->setStyleSheet("color: green;");
    } else {
      ui->lineEditPendientes->setStyleSheet("color: red;");
    }
  } else {
    ui->lineEditPendientes->setText("0");
    ui->lineEditPendientes->setStyleSheet("");
  }

  // AÑADIDO: Lógica de encargados
  QSqlQuery qEncargos(QSqlDatabase::database(conf->getConexionLocal()));
  qEncargos.prepare("SELECT sum(cantidad) FROM encargos WHERE cod_articulo = ? "
                    "AND estado IN ('Pendiente', 'Recibido')");
  qEncargos.bindValue(0, codArticulo);
  double sumEncargos = 0.0;
  if (qEncargos.exec() && qEncargos.next())
    sumEncargos = qEncargos.value(0).toDouble();
  ui->lineEditEncargados->setText(QString::number(sumEncargos));
  if (sumEncargos > 0) {
    ui->lineEditEncargados->setStyleSheet(
        "color: #1976D2; font-weight: bold;"); // Azul destacado
  } else {
    ui->lineEditEncargados->setStyleSheet("");
  }

  cargarVentas();
  cargarCompras();
  cargarCodAux();

  ui->treeWidgetStockTiendas->clear();
  ui->treeWidgetStockTiendas->setColumnCount(3);
  ui->treeWidgetStockTiendas->setHeaderLabels(
      QStringList() << "Tienda" << "Stock" << "Pendiente");

  // Ajustar columnas para visión completa
  ui->treeWidgetStockTiendas->header()->setSectionResizeMode(
      0, QHeaderView::ResizeToContents);
  ui->treeWidgetStockTiendas->header()->setSectionResizeMode(
      1, QHeaderView::ResizeToContents);
  ui->treeWidgetStockTiendas->header()->setSectionResizeMode(
      2, QHeaderView::Stretch);

  // Solo llenamos la tabla si el usuario solicita ver el stock remoto.
  // La información local ya está presente en los lineEdits superiores.
  if (ui->checkBoxRemoto->isChecked()) {
    llenarStockRemoto(ui->lineEditCod->text());
  }
  qDebug() << listaConexionesRemotas;
}

QStringList Articulos::recogerDatosFormulario() {
  QStringList listaDatosFormulario;
  listaDatosFormulario.clear();
  listaDatosFormulario.append(ui->lineEditCod->text());
  listaDatosFormulario.append(ui->lineEditDesc->text());
  listaDatosFormulario.append(ui->lineEditPvp->text());
  listaDatosFormulario.append(ui->lineEditIva->text());
  if (ui->lineEditStock->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditStock->text());
  }
  if (ui->lineEditMinimo->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditMinimo->text());
  }
  if (ui->lineEditMaximo->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditMaximo->text());
  }
  if (ui->lineEditPendientes->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditPendientes->text());
  }
  if (ui->lineEditEncargados->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditEncargados->text());
  }
  listaDatosFormulario.append(ui->dateEditUltimaVenta->text());
  listaDatosFormulario.append(ui->dateEditUltimoPedido->text());
  if (ui->lineEditCodFamila->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditCodFamila->text());
  }
  listaDatosFormulario.append(ui->lineEditCosto->text());
  if (ui->lineEditCodFabricante->text().isEmpty()) {
    listaDatosFormulario.append("0");
  } else {
    listaDatosFormulario.append(ui->lineEditCodFabricante->text());
  }
  if (ui->lineEditFoto->text() == "") {
    listaDatosFormulario.append("imagenes/1.jpg");
  } else {
    listaDatosFormulario.append(ui->lineEditFoto->text());
  }
  // Guardar las notas como HTML si tienen formato, o como texto plano si están
  // vacías. Esto garantiza retrocompatibilidad con registros que ya tenían
  // texto plano en la BD.
  QString notasHtml = ui->textEditNotas->toHtml();
  QString notasPlain = ui->textEditNotas->toPlainText().trimmed();
  if (notasPlain.isEmpty()) {
    listaDatosFormulario.append(""); // Campo vacío → guardar vacío
  } else {
    listaDatosFormulario.append(notasHtml); // Guardar HTML completo
  }
  listaDatosFormulario.append(ui->comboBoxFormato->currentText());
  if (ui->lineEditCantidad->text().isEmpty()) {
    listaDatosFormulario.append(nullptr);
  } else {
    listaDatosFormulario.append(ui->lineEditCantidad->text());
  }

  return listaDatosFormulario;
}

void Articulos::recargarTabla() {
  qDebug() << "Cargando artículos desde:" << conf->getConexionCommon();

  QString pvpQuery = conf->getUsarPreciosLocales()
                         ? "IF(pt.pvp IS NOT NULL, pt.pvp, a.pvp)"
                         : "a.pvp";

  QString sql =
      QString("SELECT a.cod, a.descripcion, %1 as pvp, a.iva, 0 as stock, "
              "a.min, a.max, "
              "0 as pendientes_pedido, 0 as encargados, a.ultima_venta, "
              "a.ultimo_pedido, "
              "a.familia, a.precio_compra, a.fabricante, a.foto, a.notas, "
              "a.formato, "
              "a.cantformato "
              "FROM articulos a "
              "LEFT JOIN precios_tienda pt ON a.cod = pt.cod_articulo")
          .arg(pvpQuery);

  modeloTabla->setQuery(sql, QSqlDatabase::database(conf->getConexionCommon()));
  mapper.setModel(modeloTabla);
}

void Articulos::cargarCompras() {
  modeloCompras.clear();
  QString codArticulo = ui->lineEditCod->text();
  QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());

  if (ui->radioButtonFacturas->isChecked()) {
    QSqlQuery query(db);
    query.prepare(
        "SELECT `nDocumento` as 'Nº Factura', `pedidos`.`idProveedor` as 'Cód Prov', `cantidad` as 'Uds', "
        "`bonificacion` as 'Bonif', `costo` as 'Costo Bruto', `descuento1` as '% Dto', DATE_FORMAT(`pedidos`.`fechaPedido`, '%Y-%m-%d') as 'Fecha' "
        "FROM `lineaspedido` "
        "JOIN `pedidos` on `nDocumento` = `pedidos`.`npedido` "
        "WHERE `cod` = ? ORDER BY `pedidos`.`fechaPedido` DESC");
    query.addBindValue(codArticulo);
    query.exec();
    modeloCompras.setQuery(query);
    if (modeloCompras.lastError().isValid())
      qDebug() << modeloCompras.lastError();
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
  }
  if (ui->radioButtonMeses->isChecked()) {
    QSqlQuery query(db);
    query.prepare(
        "SELECT YEAR(pedidos.fechaPedido) as 'Año', MONTH(pedidos.fechaPedido) as 'Mes', "
        "sum(cantidad) as 'Uds', sum(bonificacion) as 'Bonif' "
        "FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido "
        "WHERE cod = ? "
        "GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) "
        "DESC");
    query.addBindValue(codArticulo);
    query.exec();
    modeloCompras.setQuery(query);
    if (modeloCompras.lastError().isValid())
      qDebug() << modeloCompras.lastError();
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
  }
  if (ui->radioButtonAnos->isChecked()) {
    QSqlQuery query(db);
    query.prepare(
        "SELECT YEAR(pedidos.fechaPedido) as 'Año', sum(cantidad) as 'Uds', sum(bonificacion) as 'Bonif' "
        "FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido "
        "WHERE cod = ? "
        "GROUP BY YEAR(pedidos.fechaPedido) DESC");
    query.addBindValue(codArticulo);
    query.exec();
    modeloCompras.setQuery(query);
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
  }
  if (ui->radioButtonProveedores->isChecked()) {
    QSqlQuery query(db);
    query.prepare(
        "SELECT proveedores.idProveedor as 'Cód Prov', proveedores.nombre as 'Proveedor', "
        "SUM(lp.cantidad) as 'Uds', SUM(lp.bonificacion) as 'Bonif', "
        "ROUND(SUM("
        "  lp.costo * (1 - COALESCE(lp.descuento1, 0)/100) * "
        "  (1 - COALESCE(p.descuento, 0)/100) * "
        "  (1 + i.porcentaje/100 + i.recargo/100) * "
        "  lp.cantidad"
        ") / SUM(lp.cantidad + lp.bonificacion), 2) as 'Costo Medio con IVA y "
        "RE' "
        "FROM lineaspedido lp "
        "JOIN pedidos p ON lp.nDocumento = p.npedido "
        "JOIN proveedores ON p.idProveedor = proveedores.idProveedor "
        "LEFT JOIN impuestos i ON i.porcentaje = lp.tipoIva "
        "WHERE lp.cod = ? "
        "AND YEAR(p.fechaPedido) >= YEAR(CURDATE()) - 1 "
        "GROUP BY proveedores.idProveedor, proveedores.nombre");
    query.addBindValue(codArticulo);
    query.exec();
    modeloCompras.setQuery(query);
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->hideColumn(0); // Ocultar ID del proveedor
    ui->tableViewCompras->resizeColumnsToContents();
  }
}

void Articulos::cargarCodAux() {
  modeloAux = new QSqlTableModel(
      this, QSqlDatabase::database(conf->getConexionLocal()));
  modeloAux->setTable("codaux");
  modeloAux->setEditStrategy(QSqlTableModel::OnRowChange);

  // Se sanitiza la entrada rodeándola con comillas simples y escapando comillas
  // y barras internas (evita inyección vía backslash)
  QString codSanitizado = escSQL(ui->lineEditCod->text());
  modeloAux->setFilter(QString("cod = '%1'").arg(codSanitizado));

  modeloAux->select();
  ui->tableViewAux->setModel(modeloAux);
  ui->tableViewAux->hideColumn(0);
}

void Articulos::llenarComboFormatos() {
  QSqlQuery consulta =
      base.devolverTablaCompleta(conf->getConexionLocal(), "formatos");
  consulta.first();
  do {
    ui->comboBoxFormato->addItem(consulta.value("formato").toString());
  } while (consulta.next());
}

void Articulos::llenarStockRemoto(QString ean) {
  // El encabezado ya se pone en refrescarBotones, aquí solo añadimos filas
  for (int i = 0; i < listaConexionesRemotas.length(); i++) {
    QString conn = listaConexionesRemotas.at(i);

    // Verificar que la conexión esté abierta
    if (!QSqlDatabase::database(conn).isOpen()) {
      qDebug() << "Saltando conexión remota no abierta:" << conn;
      continue;
    }

    QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetStockTiendas);
    item->setText(0, conn);
    item->setText(1, base.sumarStockArticulo(ean, conn));

    // Calcular pendientes remotos
    double rE = 0, rS = 0, rP = 0;
    QSqlQuery qRE(QSqlDatabase::database(conn));
    qRE.prepare("SELECT sum(cantidad) FROM entradaGenero_tmp WHERE cod = ?");
    qRE.bindValue(0, ean);
    if (qRE.exec() && qRE.next())
      rE = qRE.value(0).toDouble();

    QSqlQuery qRS(QSqlDatabase::database(conn));
    qRS.prepare("SELECT sum(cantidad) FROM salidaGenero_tmp WHERE cod = ?");
    qRS.bindValue(0, ean);
    if (qRS.exec() && qRS.next())
      rS = qRS.value(0).toDouble();

    QSqlQuery qRP(QSqlDatabase::database(conn));
    qRP.prepare("SELECT sum(cantidad + bonificacion) FROM lineaspedido_tmp "
                "WHERE cod = ?");
    qRP.bindValue(0, ean);
    if (qRP.exec() && qRP.next())
      rP = qRP.value(0).toDouble();

    QStringList pList;
    if (rE > 0)
      pList << QString("E:%1").arg(rE);
    if (rS > 0)
      pList << QString("S:%1").arg(rS);
    if (rP > 0)
      pList << QString("P:%1").arg(rP);
    item->setText(2, pList.join("|"));

    QSqlQuery lotes = base.lotesProducto(ean, conn);
    while (lotes.next()) {
      QTreeWidgetItem *lote = new QTreeWidgetItem(item);
      lote->setText(0, lotes.record().value("fecha").toString());
      lote->setText(1, lotes.record().value("cantidad").toString());
      QString L = lotes.value("lote").toString();
      if (!L.isEmpty())
        lote->setText(0, lote->text(0) + " (Lote: " + L + ")");
    }
  }
}

DatosGrafico Articulos::extraerVentasPorFechas(QSqlQueryModel *modelo) {
  DatosGrafico datos;

  if (!modelo || modelo->rowCount() == 0 || modelo->columnCount() < 3)
    return datos; // modelo vacío o mal estructurado

  QString nombreProducto = modelo->data(modelo->index(0, 0)).toString();
  datos.titulo = QString("Evolución de ventas: %1").arg(nombreProducto);
  QString formatoFecha;
  QString granularidad;
  QString muestra = modelo->data(modelo->index(0, 1)).toString();
  if (muestra.contains("-")) {
    if (muestra.size() == 7) {
      formatoFecha = "yyyy-MM";
      granularidad = "mes";
    } else if (muestra.size() == 10) {
      formatoFecha = "yyyy-MM-dd";
      granularidad = "dia";
    }
  } else {
    formatoFecha = "yyyy";
    granularidad = "ano";
  }

  QDate fechaMin = QDate::fromString(
      modelo->data(modelo->index(modelo->rowCount() - 1, 1)).toString(),
      formatoFecha);
  QDate fechaMax = QDate::fromString(
      modelo->data(modelo->index(0, 1)).toString(), formatoFecha);
  QList<double> serieUnica;

  QMap<QString, double> mapaDatos;
  for (int fila = 0; fila < modelo->rowCount(); ++fila) {
    QString fecha = modelo->data(modelo->index(fila, 1)).toString();
    double cantidad = modelo->data(modelo->index(fila, 2)).toDouble();
    mapaDatos[fecha] += cantidad;
  }

  QDate actual = fechaMin;

  while (actual <= fechaMax) {
    QString clave = actual.toString(formatoFecha);
    double cantidad = mapaDatos.value(clave, 0);

    datos.categorias << clave;
    serieUnica << cantidad;

    if (granularidad == "dia")
      actual = actual.addDays(1);
    else if (granularidad == "mes")
      actual = actual.addMonths(1);
    else if (granularidad == "ano")
      actual = actual.addYears(1);
  }

  datos.series << serieUnica;
  datos.nombresSeries << "Ventas uds.";

  return datos;
}

QStringList Articulos::crearConexionesRemotas(QSqlQuery consultaRemota) {
  Q_UNUSED(consultaRemota);
  return QStringList();
}

void Articulos::cargarDatosGrafico(DatosGrafico nuevosDatos) {
  if (!graficoVentas) {
    qDebug() << "Grafico es nullptr";
    return;
  }
  QString tit = nuevosDatos.titulo;
  QStringList cat = nuevosDatos.categorias;
  QList<QList<double>> ser = nuevosDatos.series;
  QStringList nomSer = nuevosDatos.nombresSeries;
  QList<QColor> color = {Qt::darkGreen};
  graficoVentas->configurar(tit, cat, ser, nomSer, color);
}

void Articulos::cargarVentas() {
  modeloVentas.clear();
  QString codArticulo = ui->lineEditCod->text();
  QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());

  if (ui->radioButtonVentasMes->isChecked()) {
    QSqlQuery query(db);
    query.prepare(
        "SELECT descripcion , DATE_FORMAT(fecha, '%Y-%m') , sum(cantidad) "
        "from lineasticket WHERE cod = ? "
        "GROUP BY YEAR(fecha) desc , MONTH(fecha) desc");
    query.addBindValue(codArticulo);
    query.exec();
    modeloVentas.setQuery(query);
    if (modeloVentas.lastError().isValid())
      qDebug() << modeloVentas.lastError();
    modeloVentas.setHeaderData(0, Qt::Horizontal, "Artículo");
    modeloVentas.setHeaderData(1, Qt::Horizontal, "Fecha");
    modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");

    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
    DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
    cargarDatosGrafico(nuevosDatos);
  }
  if (ui->radioButtonVentasDia->isChecked()) {
    QSqlQuery query(db);
    query.prepare("SELECT descripcion , DATE_FORMAT(fecha, '%Y-%m-%d') , "
                  "sum(cantidad), precio FROM "
                  "lineasticket WHERE cod = ? group by fecha desc");
    query.addBindValue(codArticulo);
    query.exec();
    modeloVentas.setQuery(query);
    if (modeloVentas.lastError().isValid())
      qDebug() << modeloVentas.lastError();
    modeloVentas.setHeaderData(0, Qt::Horizontal, "Producto");
    modeloVentas.setHeaderData(1, Qt::Horizontal, "Fecha");
    modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");
    modeloVentas.setHeaderData(3, Qt::Horizontal, "Precio");
    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
    DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
    cargarDatosGrafico(nuevosDatos);
  }
  if (ui->radioButtonVentasAno->isChecked()) {
    modeloVentas.clear();
    QSqlQuery query(db);
    query.prepare("SELECT descripcion , YEAR(fecha) , sum(cantidad) "
                  "from lineasticket WHERE cod = ? "
                  "GROUP BY YEAR(fecha) desc");
    query.addBindValue(codArticulo);
    query.exec();
    modeloVentas.setQuery(query);

    if (modeloVentas.lastError().isValid())
      qDebug() << modeloVentas.lastError();
    modeloVentas.setHeaderData(0, Qt::Horizontal, "Artículo");
    modeloVentas.setHeaderData(1, Qt::Horizontal, "Año");
    modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");

    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
    DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
    cargarDatosGrafico(nuevosDatos);
  }
}

bool Articulos::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->lineEditCod) {
    if (event->type() == QEvent::MouseButtonPress) {
      borrarFormulario();
      return true;
    }
  }
  return false;
}

void Articulos::keyPressEvent(QKeyEvent *e) {
  if (e->key() == Qt::Key_F11) {
    if (conf && conf->permisos()) {
      if (conf->permisos()->tiene("articulos.borrar"))
        ui->pushButtonBorrar->setEnabled(true);
      if (conf->permisos()->tiene("articulos.cambiar_codigo"))
        ui->pushButtonCambiarCodigo->setEnabled(true);
    }
  }
}

void Articulos::borrarFormulario() {
  foreach (QLineEdit *le, ui->General->findChildren<QLineEdit *>()) {
    le->clear();
  }
  ui->dateEditUltimaVenta->setDate(QDate::fromString("2000-01-01"));
  ui->dateEditUltimoPedido->setDate(QDate::fromString("2000-01-01"));
  ui->labelFoto->clear();
  ui->labelNombreProducto->clear();
  ui->labelPrecioGrande->setText("0.00 €");
  ui->comboBoxFormato->setCurrentIndex(0);
  ui->pushButtonCambiarCodigo->setEnabled(false);
  ui->textEditNotas->clear(); // Limpiar el editor de notas enriquecidas
}

void Articulos::on_pushButtonAnterior_clicked() {
  borrarFormulario();
  mapper.toPrevious();
  refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonSiguiente_clicked() {
  borrarFormulario();
  mapper.toNext();
  refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonModificar_clicked() {
  // Comprobar que el código del artículo no esté vacío antes de guardar o modificar
  if (ui->lineEditCod->text().trimmed().isEmpty()) {
    QMessageBox::warning(this, tr("ATENCIÓN"),
                         tr("No se pueden guardar los cambios de un artículo sin código. Por favor, introduzca un código válido."));
    ui->lineEditCod->setFocus();
    return;
  }

  QStringList datos = recogerDatosFormulario();
  QString cod = ui->lineEditCod->text().trimmed();
  int idx = mapper.currentIndex();

  QMessageBox msgBox(this);
  msgBox.setWindowTitle("Confirmar Cambios");
  msgBox.setText("¿Desea guardar los cambios realizados?");

  msgBox.setIcon(QMessageBox::Question);
  msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
  msgBox.setDefaultButton(QMessageBox::Save);
  msgBox.setButtonText(QMessageBox::Save, "Guardar");
  msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");

  if (msgBox.exec() == QMessageBox::Save) {
    bool ok = false;
    if (conf->getUsarPreciosLocales()) {
      // 1. Guardar PVP localmente (No sincronizado)
      QSqlQuery qLocal(QSqlDatabase::database(conf->getConexionLocal()));
      qLocal.prepare(
          "INSERT INTO precios_tienda (cod_articulo, pvp, precio_venta) "
          "VALUES (?,?,?) ON DUPLICATE KEY UPDATE pvp=?, precio_venta=?");
      qLocal.bindValue(0, cod);
      qLocal.bindValue(1, ui->lineEditPvp->text().toDouble());
      qLocal.bindValue(2, ui->lineEditPvp->text().toDouble());
      qLocal.bindValue(3, ui->lineEditPvp->text().toDouble());
      qLocal.bindValue(4, ui->lineEditPvp->text().toDouble());
      qLocal.exec();

      // 2. Guardar el resto de campos globalmente (Sincronizado)
      // Para no alterar el PVP global de otras tiendas, mantenemos el PVP
      // original de 'articulos'
      QSqlQuery queryOri(QSqlDatabase::database(conf->getConexionLocal()));
      queryOri.prepare("SELECT pvp FROM articulos WHERE cod = ?");
      queryOri.bindValue(0, cod);
      if (queryOri.exec() && queryOri.next()) {
        datos.replace(2, queryOri.value(0).toString());
      }
      ok = base.modificarArticulo(
          QSqlDatabase::database(conf->getConexionLocal()), datos, cod);
    } else {
      // Guardar todo de forma global
      ok = base.modificarArticulo(
          QSqlDatabase::database(conf->getConexionLocal()), datos, cod);

      // Limpiar excepción local si existiera
      QSqlQuery qDel(QSqlDatabase::database(conf->getConexionLocal()));
      qDel.prepare("DELETE FROM precios_tienda WHERE cod_articulo = ?");
      qDel.bindValue(0, cod);
      qDel.exec();
    }

    if (ok) {
      QMessageBox::information(this, "Éxito",
                               "Cambios guardados correctamente.");
    } else {
      QMessageBox::critical(this, "Error",
                            "No se pudieron guardar los cambios.");
    }
    recargarTabla();
    mapper.setCurrentIndex(idx);
    refrescarBotones(mapper.currentIndex());
  }
}

void Articulos::on_pushButtonBorrar_clicked() {
  int i = mapper.currentIndex();

  QMessageBox msgBox(this);
  msgBox.setWindowTitle("Confirmar Eliminación");
  msgBox.setText("¿Está seguro de que desea eliminar este artículo?");
  msgBox.setInformativeText("Esta acción borrará también los lotes asociados y "
                            "no se puede deshacer.");
  msgBox.setIcon(QMessageBox::Warning);
  msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
  msgBox.setDefaultButton(QMessageBox::No);
  msgBox.setButtonText(QMessageBox::Yes, "Eliminar");
  msgBox.setButtonText(QMessageBox::No, "Cancelar");

  if (msgBox.exec() == QMessageBox::Yes) {
    base.borrarLotesArticulo(conf->getConexionLocal(), ui->lineEditCod->text());
    if (base.borrarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                            ui->lineEditCod->text())) {
      QMessageBox::information(this, "Éxito",
                               "El artículo ha sido eliminado correctamente.");
    } else {
      QMessageBox::critical(this, "Error", "No se pudo eliminar el artículo.");
    }
    recargarTabla();
    mapper.setCurrentIndex(qMax(0, i - 1));
    refrescarBotones(mapper.currentIndex());
  }
}

void Articulos::on_pushButtonPonerFoto_clicked() {
  int curr = mapper.currentIndex();
  QString dir = base.devolverDirectorio("imagenes");
  QString absoluteDir = QDir(dir).absolutePath();
  QString fichero =
      QFileDialog::getOpenFileName(this, "Elige el archivo", absoluteDir);

  if (fichero.isEmpty())
    return;

  // Convertir a ruta relativa respecto al directorio de imágenes para mayor
  // portabilidad
  QString relativeFichero = QDir(absoluteDir).relativeFilePath(fichero);

  qDebug() << "Guardando imagen con ruta relativa:" << relativeFichero;
  base.modificarFotoArticulo(relativeFichero, ui->lineEditCod->text());
  recargarTabla();

  mapper.setCurrentIndex(curr);
  refrescarBotones(mapper.currentIndex());
}

/**
 * @brief Slot para buscar fotos del producto en internet, previsualizarlas y guardarlas en el directorio configurado.
 */
void Articulos::on_pushButtonBuscarFotoInternet_clicked() {
  QString desc = ui->lineEditDesc->text().trimmed();
  if (desc.isEmpty()) {
    QMessageBox::warning(this, tr("Buscar foto en Internet"),
                         tr("Debes introducir la descripción del producto para buscar su foto."));
    ui->lineEditDesc->setFocus();
    return;
  }

  int curr = mapper.currentIndex();
  QString fabricante = ui->labelFabricante->text().trimmed();
  QString cod = ui->lineEditCod->text().trimmed();

  // Abrir diálogo modal de búsqueda de fotos
  DialogBuscarFotoInternet dlg(desc, fabricante, cod, this);
  if (dlg.exec() == QDialog::Accepted) {
    QString relativeFile = dlg.getNombreFicheroRelativo();
    if (!relativeFile.isEmpty()) {
      qDebug() << "Asignando foto descargada al artículo:" << relativeFile;
      base.modificarFotoArticulo(relativeFile, cod);
      recargarTabla();
      mapper.setCurrentIndex(curr);
      refrescarBotones(mapper.currentIndex());
    }
  }
}

void Articulos::on_pushButtonBorrarFoto_clicked() {
  int curr = mapper.currentIndex();
  base.modificarFotoArticulo("", ui->lineEditCod->text());
  recargarTabla();
  mapper.setCurrentIndex(curr);
  refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonRefrescar_clicked() { mapper.revert(); }

void Articulos::on_lineEditCodFamila_textChanged(const QString &arg1) {
  ui->labelFamilia->setText(base.nombreFamilia(arg1));
}

void Articulos::on_lineEditCodFabricante_textChanged(const QString &arg1) {
  ui->labelFabricante->setText(base.nombreFabricante(arg1));
}

void Articulos::on_lineEditDesc_returnPressed() {
  QSqlQuery consulta =
      base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                          "articulos", ui->lineEditDesc->text());
  consulta.first();
  qDebug() << consulta.lastError().text();
  BuscarProducto *buscar = new BuscarProducto(this, consulta);
  if (buscar->exec() == QDialog::Accepted) {
    for (int i = 0; i < modeloTabla->rowCount(); i++) {
      if (modeloTabla->record(i).value("cod").toString() == buscar->resultado) {
        mapper.setCurrentIndex(i);
        refrescarBotones(i);
        break;
      }
    }
  }
  delete buscar;
}

// Slot para realizar una búsqueda inteligente en el campo notas del artículo
void Articulos::on_pushButtonBuscarNotas_clicked() {
  // Ejecutar la consulta inteligente en la base de datos con el texto ingresado en lineEditDesc
  QSqlQuery consulta =
      base.buscarPorNotas(QSqlDatabase::database(conf->getConexionLocal()),
                          ui->lineEditDesc->text());
  consulta.first();
  BuscarPorNotas *buscarNotas = new BuscarPorNotas(this, consulta);
  if (buscarNotas->exec() == QDialog::Accepted) {
    // Si el usuario selecciona un artículo, actualizar la posición del mapper al código retornado
    for (int i = 0; i < modeloTabla->rowCount(); i++) {
      if (modeloTabla->record(i).value("cod").toString() == buscarNotas->resultado) {
        mapper.setCurrentIndex(i);
        refrescarBotones(i);
        break;
      }
    }
  }
  delete buscarNotas;
}

/**
 * @brief Slot para generar automáticamente la ficha y descripción del producto actual con IA y búsqueda web.
 */
void Articulos::on_pushButtonGenerarDescripcionIA_clicked() {
  QString descripcion = ui->lineEditDesc->text().trimmed();
  if (descripcion.isEmpty()) {
    QMessageBox::warning(this, tr("Generar descripción con IA"),
                         tr("Debes introducir al menos la descripción o nombre del producto antes de generar su ficha técnica."));
    ui->lineEditDesc->setFocus();
    return;
  }

  QString fabricante = ui->labelFabricante->text().trimmed();
  QString familia = ui->labelFamilia->text().trimmed();
  QString formato = ui->comboBoxFormato->currentText().trimmed();
  QString ean = ui->lineEditCod->text().trimmed();

  // Abrir el diálogo modal de generación con IA
  DialogGenerarDescripcionIA dlg(descripcion, fabricante, familia, formato, ean, this);
  if (dlg.exec() == QDialog::Accepted) {
    QString resultadoHtml = dlg.getDescripcionGenerada();
    if (!resultadoHtml.isEmpty()) {
      ui->textEditNotas->setHtml(resultadoHtml);
    }
  }
}

void Articulos::on_lineEditCod_returnPressed() {
  for (int i = 0; i < modeloTabla->rowCount(); i++) {
    if (modeloTabla->record(i).value("cod").toString() ==
        ui->lineEditCod->text()) {
      mapper.setCurrentIndex(i);
      refrescarBotones(i);
      return;
    }
  }
  qDebug() << "MAL";
  QString cod =
      base.codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
  QSqlRecord registroProd =
      base.consulta_producto(conf->getConexionLocal(), cod);

  if (!registroProd.isEmpty()) {
    ui->lineEditCod->setText(registroProd.value("cod").toString());
    emit on_lineEditCod_returnPressed();
    return;
  }
  // --- Búsqueda automática: primero en la nube, luego en tiendas remotas ---
  qDebug()
      << "Artículo no encontrado localmente. Iniciando búsqueda automática...";

  QString codBuscado = ui->lineEditCod->text();

  // 1) Buscar en la conexión Master (nube)
  QString connMaster = conf->getConexionMaster();
  if (!connMaster.isEmpty() && QSqlDatabase::database(connMaster).isOpen()) {
    qDebug() << "Buscando en la nube (Master):" << connMaster;
    QSqlRecord registroNube = base.consulta_producto(connMaster, codBuscado);
    if (!registroNube.isEmpty()) {
      qDebug() << "Artículo encontrado en la nube. Preguntando al usuario...";
      QStringList datos;
      for (int j = 0; j < registroNube.count(); j++) {
        datos.append(registroNube.value(j).toString());
      }
      QMessageBox msgNube(this);
      msgNube.setWindowTitle("Artículo encontrado en la nube");
      msgNube.setText("¿UTILIZAR ESTOS DATOS?");
      msgNube.setInformativeText(datos.join("\n"));
      msgNube.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      msgNube.setButtonText(QMessageBox::Ok, "Importar");
      msgNube.setButtonText(QMessageBox::Cancel, "Cancelar");
      if (msgNube.exec() == QMessageBox::Ok) {
        base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                              datos);
        recargarTabla();
        emit on_lineEditCod_returnPressed();
        return;
      }
      // El usuario canceló: procedemos normalmente sin importar
      return;
    }
  } else {
    qDebug() << "Conexión Master no disponible o no abierta:" << connMaster;
  }

  // 2) Buscar en las tiendas remotas conectadas
  qDebug() << "Buscando en tiendas remotas. Conexiones:"
           << listaConexionesRemotas.length();
  for (int i = 0; i < listaConexionesRemotas.length(); i++) {
    QString connRemota = listaConexionesRemotas.at(i);

    // Saltar la conexión Master si ya está en la lista remota (evitar doble
    // búsqueda)
    if (connRemota == connMaster)
      continue;

    if (!QSqlDatabase::database(connRemota).isOpen()) {
      qDebug() << "Saltando conexión remota no abierta:" << connRemota;
      continue;
    }

    QSqlRecord registroRemoto = base.consulta_producto(connRemota, codBuscado);
    if (!registroRemoto.isEmpty()) {
      qDebug() << "Artículo encontrado en tienda remota:" << connRemota;
      QStringList datos;
      for (int j = 0; j < registroRemoto.count(); j++) {
        datos.append(registroRemoto.value(j).toString());
        qDebug() << registroRemoto.value(j).toString();
      }
      QMessageBox msgRemota(this);
      msgRemota.setWindowTitle(
          QString("Artículo encontrado en: %1").arg(connRemota));
      msgRemota.setText("¿UTILIZAR ESTOS DATOS?");
      msgRemota.setInformativeText(datos.join("\n"));
      msgRemota.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
      msgRemota.setButtonText(QMessageBox::Ok, "Importar");
      msgRemota.setButtonText(QMessageBox::Cancel, "Cancelar");
      if (msgRemota.exec() == QMessageBox::Ok) {
        base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                              datos);
        recargarTabla();
        emit on_lineEditCod_returnPressed();
        return;
      }
      // El usuario canceló: procedemos normalmente
      return;
    }
  }

  // 3) No se encontró en ningún sitio → aviso y continuar normalmente
  QMessageBox msgNoEnc(this);
  msgNoEnc.setWindowTitle("Artículo no encontrado");
  msgNoEnc.setText("NO SE ENCUENTRA EL ARTÍCULO");
  msgNoEnc.setInformativeText("No se ha encontrado el producto ni en la nube "
                              "ni en tiendas conectadas.");
  msgNoEnc.setStandardButtons(QMessageBox::Ok);
  msgNoEnc.exec();
  return;
}

void Articulos::on_pushButtonBuscarFamilia_clicked() {
  Familias *familia = new Familias(this);
  familia->exec();
  if (familia->result() > 0) {
    ui->lineEditCodFamila->setText(familia->resultado);
  }
}

void Articulos::on_pushButtonBuscarFabricante_clicked() {
  Fabricantes *fab = new Fabricantes(this);
  fab->exec();
  if (fab->result() > 0) {
    ui->lineEditCodFabricante->setText(fab->resultado);
  }
}

void Articulos::on_pushButtonEtiqueta_clicked() {
  QString cod = ui->lineEditCod->text();
  if (!cod.isEmpty()) {
    base.insertarEtiqueta(conf->getConexionLocal(), cod);
    QMessageBox::information(this, "Etiqueta", "Etiqueta creada");
  }
}

void Articulos::on_pushButtonCambiarCodigo_clicked() {
  QString oldCod = ui->lineEditCod->text();
  if (oldCod.isEmpty()) {
    return;
  }

  QString descripcion = ui->lineEditDesc->text();

  DialogCambioCodigo dialog(oldCod, descripcion, listaConexionesRemotas, this);
  if (dialog.exec() != QDialog::Accepted) {
    return;
  }

  QString newCod = dialog.getNuevoCodigo();
  if (newCod.isEmpty()) {
    return;
  }

  if (newCod == oldCod) {
    QMessageBox::warning(this, tr("Atención"),
                         tr("El nuevo código es idéntico al actual."));
    return;
  }

  // Validar que el nuevo código no existe localmente
  if (base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                             "articulos", "cod", newCod)) {
    QMessageBox::warning(
        this, tr("Error"),
        tr("El código introducido ya existe en la base de datos."));
    return;
  }

  // --- Validar existencia en la Nube ---
  bool existeNube = base.existeArticuloEnNube(newCod);
  DialogCompararArticulos::Resultado eleccion =
      DialogCompararArticulos::Cancelar;

  if (existeNube) {
    QSqlRecord recordNube = base.getArticuloNube(newCod);
    QSqlRecord recordLocal;
    QSqlQuery qLocal(QSqlDatabase::database(conf->getConexionLocal()));
    qLocal.prepare("SELECT * FROM articulos WHERE cod = ?");
    qLocal.addBindValue(oldCod);
    if (qLocal.exec() && qLocal.next())
      recordLocal = qLocal.record();

    DialogCompararArticulos dialog(recordLocal, recordNube, this);
    if (dialog.exec() == QDialog::Accepted) {
      eleccion = dialog.getResultado();
    } else {
      return; // Cancelar
    }
  }

  // Si no existe en nube o el usuario decidió continuar tras ver el conflicto
  if (!existeNube) {
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(
        this, tr("Confirmar"),
        tr("¿Está seguro de que desea cambiar el código de '%1' a '%2'?\n\n"
           "Este cambio realizará las siguientes acciones:\n"
           "1. Migrará TODO el historial local (ventas, stock, pedidos) al "
           "nuevo "
           "código.\n"
           "2. El código anterior seguiría activo en la nube y otras tiendas.\n"
           "3. El nuevo código aparecerá en la nube tras la sincronización.")
            .arg(oldCod, newCod),
        QMessageBox::Yes | QMessageBox::No);

    if (reply != QMessageBox::Yes) {
      return;
    }
  }

  // --- Ejecutar el cambio ---
  if (base.propagarCambioCodigoArticulo(oldCod, newCod)) {
    // Si hubo conflicto y se eligió usar datos de la nube, actualizar ahora el
    // local
    if (existeNube && eleccion == DialogCompararArticulos::UsarNube) {
      QSqlRecord recordNube = base.getArticuloNube(newCod);
      base.actualizarArticuloDesdeRecord(newCod, recordNube);
    }

    QMessageBox::information(
        this, tr("Éxito"),
        tr("El código ha sido cambiado correctamente localmente.\n"
           "El proceso ha terminado exitosamente."));
    recargarTabla();
    // Buscar el nuevo registro para posicionar el cursor
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
      if (modeloTabla->record(i).value("cod").toString() == newCod) {
        mapper.setCurrentIndex(i);
        refrescarBotones(i);
        break;
      }
    }
  } else {
    QMessageBox::critical(
        this, tr("Error"),
        tr("No se pudo completar el cambio de código. La operación ha sido "
           "revertida para proteger la integridad de los datos."));
  }
}

void Articulos::on_pushButtonNuevo_clicked() {
  // Comprobar que el campo del código no esté vacío al intentar crear un nuevo artículo
  QString cod = ui->lineEditCod->text().trimmed();
  if (cod.isEmpty()) {
    QMessageBox::warning(this, tr("ATENCIÓN"),
                         tr("No se puede crear un artículo sin código. Por favor, introduzca un código de artículo válido."));
    ui->lineEditCod->setFocus();
    return;
  }

  QSqlRecord registroExistente =
      base.consulta_producto(conf->getConexionLocal(), cod);
  if (!registroExistente.isEmpty()) {
    QMessageBox::warning(this, tr("ATENCION"), tr("El registro ya existe"));
    return;
  }

  QStringList datos = recogerDatosFormulario();
  if (base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                            datos)) {
    QMessageBox::information(this, "Éxito", "Artículo creado correctamente.");
  } else {
    QMessageBox::critical(
        this, "Error",
        "No se ha podido crear el artículo. Verifique los datos obligatorios.");
  }
  recargarTabla();
}

void Articulos::on_radioButtonVentasDia_clicked() { cargarVentas(); }

void Articulos::on_radioButtonVentasMes_clicked() { cargarVentas(); }

void Articulos::on_radioButtonVentasAno_clicked() { cargarVentas(); }

void Articulos::on_radioButtonFacturas_clicked() {
  cargarCompras();
  ui->pushButtonVerFactura->setEnabled(true);
}

void Articulos::on_radioButtonMeses_clicked() {
  cargarCompras();
  ui->pushButtonVerFactura->setEnabled(false);
}

void Articulos::on_radioButtonAnos_clicked() {
  cargarCompras();
  ui->pushButtonVerFactura->setEnabled(false);
}

void Articulos::on_radioButtonProveedores_clicked() {
  cargarCompras();
  ui->pushButtonVerFactura->setEnabled(false);
}

void Articulos::on_pushButtonVer_clicked() {
  Stock *stock = new Stock(ui->lineEditCod->text(), &base, this);
  stock->exec();
}

void Articulos::on_pushButtonAnadir_clicked() {
  // Verificar que exista un código de artículo antes de asociarle un código auxiliar
  if (ui->lineEditCod->text().trimmed().isEmpty()) {
    QMessageBox::warning(this, tr("ATENCIÓN"),
                         tr("No se puede añadir un código auxiliar si el artículo no tiene código."));
    ui->lineEditCod->setFocus();
    return;
  }

  if (!ui->lineEditAux->text().isEmpty()) {
    modeloAux->insertRow(0);
    QSqlRecord record = modeloAux->record();
    record.setValue(1, ui->lineEditCod->text());
    record.setValue(2, ui->lineEditAux->text());
    modeloAux->setRecord(0, record);
    modeloAux->submitAll();
    cargarCodAux();
  }
  return;
}

void Articulos::on_pushButtonAnadirAPedido_clicked() {
  QString cod = ui->lineEditCod->text();
  if (cod.isEmpty()) {
    QMessageBox::warning(this, "Aviso",
                         "No hay ningún artículo seleccionado para añadir.");
    return;
  }
  DialogAnadirAPedido *dialogo = new DialogAnadirAPedido(cod, this);
  dialogo->exec();
  delete dialogo;
}

/**
 * @brief Abre el diálogo para añadir el producto actual a la lista de salidas de mercancía / traspasos.
 */
void Articulos::on_pushButtonAnadirASalidas_clicked() {
  QString cod = ui->lineEditCod->text().trimmed();
  if (cod.isEmpty()) {
    QMessageBox::warning(this, tr("Aviso"),
                         tr("No hay ningún artículo seleccionado para añadir a salidas."));
    return;
  }
  DialogAnadirASalidas *dialogo = new DialogAnadirASalidas(cod, this);
  dialogo->exec();
  delete dialogo;
}

// Slot para consultar la trazabilidad completa del producto seleccionado (compras, entradas y salidas)
void Articulos::on_pushButtonTrazabilidad_clicked() {
  QString cod = ui->lineEditCod->text();
  if (cod.isEmpty()) {
    QMessageBox::warning(this, tr("Atención"),
                         tr("Debe seleccionar un artículo para consultar su trazabilidad."));
    return;
  }
  QString desc = ui->lineEditDesc->text();
  DialogTrazabilidad dialog(cod, desc, this);
  dialog.exec();
}

void Articulos::on_pushButtonEliminar_clicked() {
  modeloAux->removeRow(ui->tableViewAux->currentIndex().row());
}

void Articulos::on_tableViewCompras_clicked(const QModelIndex &index) {
  // Si estamos viendo un listado de facturas (vista normal o detalle de un
  // proveedor) En ambos casos, columna 0 es nDocumento y columna 1 es
  // idProveedor
  if (ui->radioButtonFacturas->isChecked() ||
      (ui->radioButtonProveedores->isChecked() &&
       modeloCompras.columnCount() > 5)) {
    QModelIndex indice = modeloCompras.index(index.row(), 1);
    idProveedor = modeloCompras.data(indice, Qt::EditRole).toString();
    ui->labelProveedor->setText(
        base.nombreProveedor(idProveedor, conf->getConexionLocal()));

    indice = modeloCompras.index(index.row(), 0);
    nFactura = modeloCompras.data(indice, Qt::EditRole).toString();

    ui->pushButtonVerFactura->setEnabled(true);
    return; // Salimos para no procesar el drill-down
  }

  // Si estamos en la vista de Proveedores (resumen), al pinchar mostramos el
  // detalle de ese proveedor
  if (ui->radioButtonProveedores->isChecked() &&
      modeloCompras.columnCount() <= 5) {
    QModelIndex indiceId = modeloCompras.index(index.row(), 0);
    QString idProv = modeloCompras.data(indiceId, Qt::EditRole).toString();
    QString nombreProv =
        modeloCompras.data(modeloCompras.index(index.row(), 1), Qt::EditRole)
            .toString();

    ui->labelProveedor->setText("Detalle de compras: " + nombreProv);

    // Cargamos el detalle específico para este proveedor, calculando el neto
    // por línea (Año actual y anterior)
    modeloCompras.setQuery(
        "SELECT lp.nDocumento as 'Nº Factura', p.idProveedor as 'Cód Prov', lp.cantidad as 'Uds', "
        "lp.bonificacion as 'Bonif', "
        "lp.costo as 'Bruto', lp.descuento1 as '% Desc L', p.descuento as '% "
        "Desc P', "
        "ROUND((lp.costo * (1 - COALESCE(lp.descuento1, 0)/100) * "
        "(1 - COALESCE(p.descuento, 0)/100) * "
        "(1 + i.porcentaje/100 + i.recargo/100) * "
        "lp.cantidad) / (lp.cantidad + lp.bonificacion), 2) as 'Neto + IVA + "
        "RE', "
        "DATE_FORMAT(p.fechaPedido, '%Y-%m-%d') as 'Fecha' "
        "FROM lineaspedido lp "
        "JOIN pedidos p ON lp.nDocumento = p.npedido "
        "LEFT JOIN impuestos i ON i.porcentaje = lp.tipoIva "
        "WHERE lp.cod = '" +
            ui->lineEditCod->text() + "' AND p.idProveedor = '" + idProv +
            "' "
            "AND YEAR(p.fechaPedido) >= YEAR(CURDATE()) - 1 "
            "ORDER BY p.fechaPedido DESC",
        QSqlDatabase::database(conf->getConexionLocal()));

    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
    // En esta vista detalle, habilitamos ver la factura
    ui->pushButtonVerFactura->setEnabled(true);
  }
}

void Articulos::on_pushButtonVer_2_clicked() {}

void Articulos::mostrarFoto() {
  VisorImagenes *visor = new VisorImagenes(ui->lineEditFoto->text());
  visor->showMaximized();
}

// ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) :
// QLabel(parent)
//{

//}

// ClickableLabel::~ClickableLabel()
//{

//}

// void ClickableLabel::mousePressEvent(QMouseEvent *event)
//{
//     emit clicked();
// }

void Articulos::on_pushButtonVerFactura_clicked() {
  if (nFactura.isEmpty()) {
    QMessageBox::information(
        this, "No hay factura seleccionada",
        "Seleccione una factura antes de usar esta opcion");
    return;
  }
  QStringList datos;
  imprimirFacturaProveedor facturaHtml(conf->getConexionLocal(), datos,
                                       nFactura);

  //    VisorFacturas *factura = new VisorFacturas(nFactura,this);
  //    factura->show();
}

void Articulos::on_checkBoxRemoto_stateChanged(int arg1) {
  if (remoto == false && arg1 == 2) {
    // QSqlQuery consultaRemota =
    // base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    // listaConexionesRemotas = crearConexionesRemotas(consultaRemota);
    // qDebug() << listaConexionesRemotas;
    // remoto = true;
  }
  refrescarBotones(mapper.currentIndex());
}

void Articulos::on_treeWidgetStockTiendas_itemDoubleClicked(
    QTreeWidgetItem *item, int column) {
  QString baseDatosRemota;
  if (!item->parent()) {
    baseDatosRemota = item->text(0);
  } else {
    baseDatosRemota = item->parent()->text(0);
  }
  comprasVentasRemoto *cvr = new comprasVentasRemoto(
      QSqlDatabase::database(baseDatosRemota), ui->lineEditCod->text());
  cvr->show();
}

void Articulos::on_pushButtonHistorialPrecios_clicked() {
  QString codArticulo = ui->lineEditCod->text();
  if (codArticulo.isEmpty()) {
    QMessageBox::warning(
        this, tr("Atención"),
        tr("Debe seleccionar un artículo para ver su historial de precios."));
    return;
  }

  HistorialPrecios dialog(codArticulo, this);
  dialog.exec();
}
