#include "articulos.h"
#include "caducados.h"
#include "conexion.h"
#include "graficoventaswidget.h"
#include "imprimirfacturaproveedor.h"
#include "ui_articulos.h"

#include <QDate>
#include <QDir>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QtConcurrent/QtConcurrent>

Articulos::Articulos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Articulos)
{
    listaConexionesRemotas = conf->getNombreConexionesActivas();
    qDebug() << "Lista conexiones remotas: ";
    qDebug() << listaConexionesRemotas;
    ui->setupUi(this);

    // Configuración de iconos profesionales para los botones
    ui->pushButtonNuevo->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->pushButtonModificar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButtonRefrescar->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->pushButtonAnterior->setIcon(style()->standardIcon(QStyle::SP_ArrowLeft));
    ui->pushButtonSiguiente->setIcon(style()->standardIcon(QStyle::SP_ArrowRight));
    ui->pushButtonCaducados->setIcon(style()->standardIcon(QStyle::SP_MessageBoxWarning));
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    
    // Iconos para los botones de búsqueda (Familia y Fabricante)
    ui->pushButtonBuscarFamilia->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    ui->pushButtonBuscarFabricante->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));

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
    mapper.addMapping(ui->plainTextEdit, 15);
    mapper.addMapping(ui->comboBoxFormato, 16);
    mapper.addMapping(ui->lineEditCantidad, 17);

    mapper.toFirst();

    remoto = false;

    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
    borrarFormulario();
    ui->lineEditCod->setFocus();
    graficoVentas = new GraficoVentasWidget(this);
    ui->layOutVentas->addWidget(graficoVentas);
}

Articulos::~Articulos()
{
    delete ui;
}

void Articulos::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
    QString fichero = QDir::currentPath() + "/" + ui->lineEditFoto->text();
    QImage foto(fichero);
    QPixmap imagen = QPixmap::fromImage(foto);
    QPixmap imagenAjustada = imagen.scaled(200, 200, Qt::KeepAspectRatio);

    ui->labelFoto->setPixmap(imagenAjustada);
    ui->labelNombrePrecio->setText(ui->lineEditDesc->text() + "        " + ui->lineEditPvp->text());
    ui->lineEditStock->setText(
        base.sumarStockArticulo(ui->lineEditCod->text(), conf->getConexionLocal()));

    double sumEntrada = 0.0;
    double sumSalida = 0.0;
    QString codArticulo = ui->lineEditCod->text();
    QSqlQuery qEntrada = base.ejecutarSentencia("SELECT sum(cantidad) FROM entradaGenero_tmp WHERE cod = '" + codArticulo + "'", conf->getConexionLocal());
    if (qEntrada.next()) sumEntrada = qEntrada.value(0).toDouble();

    QSqlQuery qSalida = base.ejecutarSentencia("SELECT sum(cantidad) FROM salidaGenero_tmp WHERE cod = '" + codArticulo + "'", conf->getConexionLocal());
    if (qSalida.next()) sumSalida = qSalida.value(0).toDouble();

    if (sumEntrada > 0 && sumSalida > 0) {
        // Si está en ambas tablas, mostramos el detalle (ej. "E: 5 | S: 2") y un color neutro (ej. naranja).
        ui->lineEditPendientes->setText(QString("E: %1 | S: %2").arg(sumEntrada).arg(sumSalida));
        ui->lineEditPendientes->setStyleSheet("color: #d97706;"); // Un color naranja / ámbar
    } else if (sumEntrada > 0) {
        ui->lineEditPendientes->setText(QString::number(sumEntrada));
        ui->lineEditPendientes->setStyleSheet("color: green;");
    } else if (sumSalida > 0) {
        ui->lineEditPendientes->setText(QString::number(sumSalida));
        ui->lineEditPendientes->setStyleSheet("color: red;");
    } else {
        ui->lineEditPendientes->setText("0");
        ui->lineEditPendientes->setStyleSheet("");
    }

    // AÑADIDO: Lógica de encargados
    QSqlQuery qEncargos = base.ejecutarSentencia("SELECT sum(cantidad) FROM encargos WHERE cod_articulo = '" + codArticulo + "' AND estado IN ('Pendiente', 'Recibido')", conf->getConexionLocal());
    double sumEncargos = 0.0;
    if (qEncargos.next()) sumEncargos = qEncargos.value(0).toDouble();
    ui->lineEditEncargados->setText(QString::number(sumEncargos));
    if (sumEncargos > 0) {
        ui->lineEditEncargados->setStyleSheet("color: #1976D2; font-weight: bold;"); // Azul destacado
    } else {
        ui->lineEditEncargados->setStyleSheet("");
    }

    cargarVentas();
    cargarCompras();
    cargarCodAux();

    // Siempre mostrar el desglose de lotes locales en el árbol de stock
    ui->treeWidgetStockTiendas->clear();
    QString localConn = conf->getConexionLocal();
    QTreeWidgetItem *localItem = new QTreeWidgetItem(ui->treeWidgetStockTiendas);
    localItem->setText(0, "Local");
    localItem->setText(1, ui->lineEditStock->text());
    localItem->setBackground(0, QBrush(Qt::lightGray));
    localItem->setExpanded(true);

    QSqlQuery localLots = base.lotesProducto(ui->lineEditCod->text(), localConn);
    while (localLots.next()) {
        QTreeWidgetItem *loteItem = new QTreeWidgetItem(localItem);
        loteItem->setText(0, localLots.value("fecha").toString());
        loteItem->setText(1, localLots.value("cantidad").toString());
        QString L = localLots.value("lote").toString();
        if (!L.isEmpty()) loteItem->setText(0, loteItem->text(0) + " (Lote: " + L + ")");
    }

    if (ui->checkBoxRemoto->isChecked()) {
        llenarStockRemoto(ui->lineEditCod->text());
    }
    qDebug() << listaConexionesRemotas;
}

QStringList Articulos::recogerDatosFormulario()
{
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
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());
    listaDatosFormulario.append(ui->comboBoxFormato->currentText());
    if (ui->lineEditCantidad->text().isEmpty()) {
        listaDatosFormulario.append(nullptr);
    } else {
        listaDatosFormulario.append(ui->lineEditCantidad->text());
    }

    return listaDatosFormulario;
}

void Articulos::recargarTabla()
{
    qDebug() << conf->getConexionLocal();
    modeloTabla->setQuery("SELECT * FROM articulos",
                          QSqlDatabase::database(conf->getConexionLocal()));
    mapper.setModel(modeloTabla);
}

void Articulos::cargarCompras()
{
    modeloCompras.clear();
    if (ui->radioButtonFacturas->isChecked()) {
        modeloCompras
            .setQuery("SELECT `nDocumento` , `pedidos`.`idProveedor` , `cantidad` , `bonificacion` "
                      ", `costo` , `descuento1`, `pedidos`.`fechaPedido` FROM `lineaspedido` JOIN "
                      "`pedidos` on `nDocumento` = `pedidos`.`npedido` WHERE `cod` = '"
                          + ui->lineEditCod->text() + "' ORDER BY `pedidos`.`fechaPedido` DESC",
                      QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonMeses->isChecked()) {
        modeloCompras.setQuery(
            "SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , sum(cantidad) , "
            "sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido "
            "WHERE cod = '"
                + ui->lineEditCod->text()
                + "' GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC ",
            QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonAnos->isChecked()) {
        modeloCompras
            .setQuery("SELECT YEAR(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM "
                      "lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                          + ui->lineEditCod->text() + "' GROUP BY YEAR(pedidos.fechaPedido) DESC",
                      QSqlDatabase::database(conf->getConexionLocal()));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
}

void Articulos::cargarCodAux()
{
    modeloAux = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modeloAux->setTable("codaux");
    modeloAux->setEditStrategy(QSqlTableModel::OnRowChange);
    modeloAux->setFilter("cod =" + ui->lineEditCod->text());
    modeloAux->select();
    ui->tableViewAux->setModel(modeloAux);
    ui->tableViewAux->hideColumn(0);
}

void Articulos::llenarComboFormatos()
{
    consulta = base.devolverTablaCompleta(conf->getConexionLocal(), "formatos");
    consulta.first();
    do {
        ui->comboBoxFormato->addItem(consulta.value("formato").toString());
    } while (consulta.next());
}

void Articulos::llenarStockRemoto(QString ean)
{
    qDebug() << listaConexionesRemotas.length();
    QStringList encabezado;
    encabezado << "Tienda" << "Stock";
    ui->treeWidgetStockTiendas->setColumnCount(2);
    ui->treeWidgetStockTiendas->setHeaderLabels(encabezado);
    ui->treeWidgetStockTiendas->clear();
    for (int i = 0; i < listaConexionesRemotas.length(); i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetStockTiendas);
        item->setText(0, listaConexionesRemotas.at(i));
        item->setText(1, base.sumarStockArticulo(ean, listaConexionesRemotas.at(i)));
        QSqlQuery lotes = base.lotesProducto(ean, listaConexionesRemotas.at(i));
        while (lotes.next()) {
            QTreeWidgetItem *lote = new QTreeWidgetItem(item);
            lote->setText(0, lotes.record().value("fecha").toString());
            lote->setText(1, lotes.record().value("cantidad").toString());
        }
    }
}

DatosGrafico Articulos::extraerVentasPorFechas(QSqlQueryModel *modelo)
{
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

    QDate fechaMin
        = QDate::fromString(modelo->data(modelo->index(modelo->rowCount() - 1, 1)).toString(),
                            formatoFecha);
    QDate fechaMax = QDate::fromString(modelo->data(modelo->index(0, 1)).toString(), formatoFecha);
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

QStringList Articulos::crearConexionesRemotas(QSqlQuery consultaRemota)
{
    Q_UNUSED(consultaRemota);
    return QStringList();
}

void Articulos::cargarDatosGrafico(DatosGrafico nuevosDatos)
{
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

void Articulos::cargarVentas()
{
    modeloVentas.clear();
    if (ui->radioButtonVentasMes->isChecked()) {
        // modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) "
        //                       "from lineasticket WHERE cod = '"
        //                           + ui->lineEditCod->text()
        //                           + "' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",
        //                       QSqlDatabase::database(conf->getConexionLocal()));
        // qDebug() << modeloVentas.lastError();
        modeloVentas.setQuery("SELECT descripcion , DATE_FORMAT(fecha, '%Y-%m') , sum(cantidad) "
                              "from lineasticket WHERE cod = '"
                                  + ui->lineEditCod->text()
                                  + "' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",
                              QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0, Qt::Horizontal, "Artculo");
        modeloVentas.setHeaderData(1, Qt::Horizontal, "Fecha");
        modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");
        //modeloVentas.setHeaderData(3, Qt::Horizontal, "Cantidad");

        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
        DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
        cargarDatosGrafico(nuevosDatos);
    }
    if (ui->radioButtonVentasDia->isChecked()) {
        modeloVentas
            .setQuery("SELECT descripcion , fecha , sum(cantidad) FROM lineasticket WHERE cod = '"
                          + ui->lineEditCod->text() + "' group by fecha desc",
                      QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0, Qt::Horizontal, "Producto");
        modeloVentas.setHeaderData(1, Qt::Horizontal, "Fecha");
        modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");
        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
        DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
        cargarDatosGrafico(nuevosDatos);
    }
    if (ui->radioButtonVentasAno->isChecked()) {
        modeloVentas.clear();

        modeloVentas.setQuery(
            "SELECT descripcion , YEAR(fecha) , sum(cantidad) from lineasticket WHERE cod = '"
                + ui->lineEditCod->text() + "' GROUP BY YEAR(fecha) desc",
            QSqlDatabase::database(conf->getConexionLocal()));

        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0, Qt::Horizontal, "Artculo");
        modeloVentas.setHeaderData(1, Qt::Horizontal, "Año");
        modeloVentas.setHeaderData(2, Qt::Horizontal, "Cantidad");

        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
        DatosGrafico nuevosDatos = extraerVentasPorFechas(&modeloVentas);
        cargarDatosGrafico(nuevosDatos);
    }
}

bool Articulos::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditCod) {
        if (event->type() == QEvent::MouseButtonPress) {
            borrarFormulario();
            return true;
        }
    }
    return false;
}

void Articulos::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F11) {
        ui->pushButtonBorrar->setEnabled(true);
    }
}

void Articulos::borrarFormulario()
{
    foreach (QLineEdit *le, ui->General->findChildren<QLineEdit *>()) {
        le->clear();
    }
    ui->dateEditUltimaVenta->setDate(QDate::fromString("2000-01-01"));
    ui->dateEditUltimoPedido->setDate(QDate::fromString("2000-01-01"));
    ui->labelFoto->clear();
    ui->labelNombrePrecio->clear();
    ui->comboBoxFormato->setCurrentIndex(0);
}

void Articulos::on_pushButtonAnterior_clicked()
{
    borrarFormulario();
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonSiguiente_clicked()
{
    borrarFormulario();
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonModificar_clicked()
{
    QStringList datos = recogerDatosFormulario();
    int i = mapper.currentIndex();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmar Cambios");
    msgBox.setText("¿Desea guardar los cambios realizados en este artículo?");
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Save);
    msgBox.setButtonText(QMessageBox::Save, "Guardar");
    msgBox.setButtonText(QMessageBox::Cancel, "Cancelar");

    if (msgBox.exec() == QMessageBox::Save) {
        if (base.modificarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                                   datos,
                                   ui->lineEditCod->text())) {
            QMessageBox::information(this, "Éxito", "Los cambios se han guardado correctamente.");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo guardar la modificación. Verifique los datos.");
        }
        recargarTabla();
        mapper.setCurrentIndex(i);
        refrescarBotones(mapper.currentIndex());
    }
}

void Articulos::on_pushButtonBorrar_clicked()
{
    int i = mapper.currentIndex();

    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Confirmar Eliminación");
    msgBox.setText("¿Está seguro de que desea eliminar este artículo?");
    msgBox.setInformativeText("Esta acción borrará también los lotes asociados y no se puede deshacer.");
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);
    msgBox.setButtonText(QMessageBox::Yes, "Eliminar");
    msgBox.setButtonText(QMessageBox::No, "Cancelar");

    if (msgBox.exec() == QMessageBox::Yes) {
        base.borrarLotesArticulo(conf->getConexionLocal(), ui->lineEditCod->text());
        if (base.borrarArticulo(QSqlDatabase::database(conf->getConexionLocal()),
                                ui->lineEditCod->text())) {
            QMessageBox::information(this, "Éxito", "El artículo ha sido eliminado correctamente.");
        } else {
            QMessageBox::critical(this, "Error", "No se pudo eliminar el artículo.");
        }
        recargarTabla();
        mapper.setCurrentIndex(qMax(0, i - 1));
        refrescarBotones(mapper.currentIndex());
    }
}

void Articulos::on_pushButtonPonerFoto_clicked()
{
    int curr = mapper.currentIndex();
    QString dir = base.devolverDirectorio("imagenes");
    int i = dir.length();
    QString fichero = QFileDialog::getOpenFileName(this, "Elige el archivo", dir);
    fichero.remove(
        0, i + 1); //Quita i caracteres desde la posicion 0 de la cadena fichero i=longitud del path
    qDebug() << fichero;
    qDebug() << ui->lineEditCod->text().toInt();
    base.modificarFotoArticulo(fichero, ui->lineEditCod->text());
    recargarTabla();

    mapper.setCurrentIndex(curr);
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonBorrarFoto_clicked()
{
    int curr = mapper.currentIndex();
    base.modificarFotoArticulo("", ui->lineEditCod->text());
    recargarTabla();
    mapper.setCurrentIndex(curr);
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonRefrescar_clicked()
{
    mapper.revert();
}

void Articulos::on_lineEditCodFamila_textChanged(const QString &arg1)
{
    ui->labelFamilia->setText(base.nombreFamilia(arg1));
}

void Articulos::on_lineEditCodFabricante_textChanged(const QString &arg1)
{
    ui->labelFabricante->setText(base.nombreFabricante(arg1));
}

void Articulos::on_lineEditDesc_returnPressed()
{
    consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                   "articulos",
                                   ui->lineEditDesc->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    if(buscar->exec() == QDialog::Accepted){
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

void Articulos::on_lineEditCod_returnPressed()
{
    for (int i = 0; i < modeloTabla->rowCount(); i++) {
        if (modeloTabla->record(i).value("cod").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            return;
        }
    }
    qDebug() << "MAL";
    QString cod = base.codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
    consulta = base.consulta_producto(conf->getConexionLocal(), cod);
    consulta.first();
    if (consulta.numRowsAffected() == 1) {
        ui->lineEditCod->setText(consulta.value(0).toString());
        emit on_lineEditCod_returnPressed();
        return;
    }
    QMessageBox msgbox;
    msgbox.setText("NO SE ENCUENTRA EL ARTÍCULO");
    msgbox.setInformativeText("Desea buscar los datos en otras tiendas?");
    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgbox.setDefaultButton(QMessageBox::No);
    if (msgbox.exec() == QMessageBox::Yes) {
        qDebug() << "Entrando en buscar";
        qDebug() << "Lista conexiones:" << listaConexionesRemotas.length();
        if (listaConexionesRemotas.isEmpty()) {
            qDebug() << "No hay conexiones remotas activas.";
        }
        for (int i = 0; i < listaConexionesRemotas.length(); i++) {
            QSqlQuery consulta = base.consulta_producto(listaConexionesRemotas.at(i),
                                                        ui->lineEditCod->text());
            qDebug() << consulta.lastQuery();
            qDebug() << listaConexionesRemotas.at(i);
            qDebug() << ui->lineEditCod->text();
            if (consulta.numRowsAffected() > 0) {
                consulta.first();
                QSqlRecord q = consulta.record();
                QStringList datos;
                datos.clear();
                for (int i = 0; i < q.count(); i++) {
                    datos.append(q.value(i).toString());
                    qDebug() << q.value(i).toString();
                }
                msgbox.setText("¿UTILIZAR ESTOS DATOS?");
                msgbox.setInformativeText(datos.join("\n"));
                msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                if (msgbox.exec() == QMessageBox::Ok) {
                    qDebug() << "Valor devuelto";
                    base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()), datos);
                    recargarTabla();
                    emit on_lineEditCod_returnPressed();
                    return;
                }
            }
        }
        msgbox.setText("NO SE ENCUENTRA");
        msgbox.setInformativeText("No se ha encontrado el producto en tiendas conectadas");
        msgbox.setStandardButtons(QMessageBox::Ok);
        msgbox.exec();
    }
    return;
}

void Articulos::on_pushButtonBuscarFamilia_clicked()
{
    Familias *familia = new Familias(this);
    familia->exec();
    if (familia->result() > 0) {
        ui->lineEditCodFamila->setText(familia->resultado);
    }
}

void Articulos::on_pushButtonBuscarFabricante_clicked()
{
    Fabricantes *fab = new Fabricantes(this);
    fab->exec();
    if (fab->result() > 0) {
        ui->lineEditCodFabricante->setText(fab->resultado);
    }
}

void Articulos::on_pushButtonNuevo_clicked()
{
    consulta = base.consulta_producto(conf->getConexionLocal(), ui->lineEditCod->text());
    if (consulta.numRowsAffected() > 0) {
        QMessageBox::warning(this, "ATENCION", "El registro ya existe");
        return;
    }

    QStringList datos = recogerDatosFormulario();
    if (base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()), datos)) {
        QMessageBox::information(this, "Éxito", "Artículo creado correctamente.");
    } else {
        QMessageBox::critical(this, "Error", "No se ha podido crear el artículo. Verifique los datos obligatorios.");
    }
    recargarTabla();
}

void Articulos::on_radioButtonVentasDia_clicked()
{
    cargarVentas();
}

void Articulos::on_radioButtonVentasMes_clicked()
{
    cargarVentas();
}

void Articulos::on_radioButtonVentasAno_clicked()
{
    cargarVentas();
}

void Articulos::on_radioButtonFacturas_clicked()
{
    cargarCompras();
    ui->pushButtonVerFactura->setEnabled(true);
}

void Articulos::on_radioButtonMeses_clicked()
{
    cargarCompras();
    ui->pushButtonVerFactura->setEnabled(false);
}

void Articulos::on_radioButtonAnos_clicked()
{
    cargarCompras();
    ui->pushButtonVerFactura->setEnabled(false);
}

void Articulos::on_pushButtonVer_clicked()
{
    Stock *stock = new Stock(ui->lineEditCod->text());
    stock->exec();
}

void Articulos::on_pushButtonAnadir_clicked()
{
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

void Articulos::on_pushButtonEliminar_clicked()
{
    modeloAux->removeRow(ui->tableViewAux->currentIndex().row());
}

void Articulos::on_tableViewCompras_clicked(const QModelIndex &index)
{
    if (ui->radioButtonFacturas->isChecked()) {
        QModelIndex indice = modeloCompras.index(index.row(), 1);
        idProveedor = modeloCompras.data(indice, Qt::EditRole).toString();
        ui->labelProveedor->setText(base.nombreProveedor(idProveedor, conf->getConexionLocal()));
        indice = modeloCompras.index(index.row(), 0);
        nFactura = modeloCompras.data(indice, Qt::EditRole).toString();
    }
}

void Articulos::on_pushButtonEtiqueta_clicked()
{
    base.insertarEtiqueta(conf->getConexionLocal(), ui->lineEditCod->text());
}

void Articulos::on_pushButtonCaducados_clicked()
{
    Caducados *cad = new Caducados(ui->lineEditCod->text(), this);
    cad->exec();
}

void Articulos::on_pushButtonVer_2_clicked() {}

void Articulos::mostrarFoto()
{
    VisorImagenes *visor = new VisorImagenes(ui->lineEditFoto->text());
    visor->showMaximized();
}

//ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent)
//{

//}

//ClickableLabel::~ClickableLabel()
//{

//}

//void ClickableLabel::mousePressEvent(QMouseEvent *event)
//{
//    emit clicked();
//}

void Articulos::on_pushButtonVerFactura_clicked()
{
    if (nFactura.isEmpty()) {
        QMessageBox::information(this,
                                 "No hay factura seleccionada",
                                 "Seleccione una factura antes de usar esta opcion");
        return;
    }
    QStringList datos;
    imprimirFacturaProveedor facturaHtml(conf->getConexionLocal(), datos, nFactura);

    //    VisorFacturas *factura = new VisorFacturas(nFactura,this);
    //    factura->show();
}

void Articulos::on_checkBoxRemoto_stateChanged(int arg1)
{
    if (remoto == false && arg1 == 2) {
        // QSqlQuery consultaRemota = base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
        // listaConexionesRemotas = crearConexionesRemotas(consultaRemota);
        // qDebug() << listaConexionesRemotas;
        // remoto = true;
    }
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_treeWidgetStockTiendas_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString baseDatosRemota;
    if (!item->parent()) {
        baseDatosRemota = item->text(0);
    } else {
        baseDatosRemota = item->parent()->text(0);
    }
    comprasVentasRemoto *cvr = new comprasVentasRemoto(QSqlDatabase::database(baseDatosRemota),
                                                       ui->lineEditCod->text());
    cvr->show();
}
