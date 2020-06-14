#include "articulos.h"
#include "ui_articulos.h"
#include "caducados.h"
#include "conexion.h"
#include "imprimirfacturaproveedor.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>
#include <QtConcurrent/QtConcurrent>

Articulos::Articulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Articulos)
{
    listaConexionesRemotas = conf->getNombreConexionesActivas();
    qDebug() << listaConexionesRemotas;
    ui->setupUi(this);
    ClickableLabel *fotoHR = new ClickableLabel(ui->labelFoto);
    fotoHR->setMinimumSize(200,200);
    connect(fotoHR,SIGNAL(clicked()),this,SLOT(mostrarFoto()));
    llenarComboFormatos();
    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod,0);
    mapper.addMapping(ui->lineEditDesc,1);
    mapper.addMapping(ui->lineEditPvp,2);
    mapper.addMapping(ui->lineEditIva,3);
    mapper.addMapping(ui->lineEditMinimo,5);
    mapper.addMapping(ui->lineEditMaximo,6);
    mapper.addMapping(ui->lineEditEncargados,8);
    mapper.addMapping(ui->lineEditPendientes,7);
    mapper.addMapping(ui->dateEditUltimaVenta,9);
    mapper.addMapping(ui->dateEditUltimoPedido,10);
    mapper.addMapping(ui->lineEditCodFamila,11);
    mapper.addMapping(ui->lineEditCosto,12);
    mapper.addMapping(ui->lineEditCodFabricante,13);
    mapper.addMapping(ui->lineEditFoto,14);
    mapper.addMapping(ui->plainTextEdit,15);
    mapper.addMapping(ui->comboBoxFormato,16);
    mapper.addMapping(ui->lineEditCantidad,17);

    mapper.toFirst();

    remoto = false;

    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
    borrarFormulario();
    ui->lineEditCod->setFocus();


    //conexiones = new conexionesRemotas();


}

Articulos::~Articulos()
{
    delete ui;
}

void Articulos::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() -1);
    QString fichero = QDir::currentPath() + "/" + ui->lineEditFoto->text();
    QImage foto(fichero);
    QPixmap imagen =QPixmap::fromImage(foto);
    QPixmap imagenAjustada = imagen.scaled(200,200,Qt::KeepAspectRatio);

    ui->labelFoto->setPixmap(imagenAjustada);
    ui->labelNombrePrecio->setText(ui->lineEditDesc->text()+ "        "+ui->lineEditPvp->text());
    ui->lineEditStock->setText(base.sumarStockArticulo(ui->lineEditCod->text(),conf->getConexionLocal()));

    cargarVentas();
    cargarCompras();
    cargarCodAux();
    if (ui->checkBoxRemoto->isChecked()){
        llenarStockRemoto(ui->lineEditCod->text());}else{
        ui->treeWidgetStockTiendas->clear();
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
    if(ui->lineEditStock->text().isEmpty()){
        listaDatosFormulario.append("0");
    }else {
        listaDatosFormulario.append(ui->lineEditStock->text());
    }
    if (ui->lineEditMinimo->text().isEmpty()) {
        listaDatosFormulario.append("0");
    }else{
        listaDatosFormulario.append(ui->lineEditMinimo->text());
    }
    if (ui->lineEditMaximo->text().isEmpty()) {
        listaDatosFormulario.append("0");
    }else{
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
    }else{
        listaDatosFormulario.append(ui->lineEditCodFamila->text());
    }
    listaDatosFormulario.append(ui->lineEditCosto->text());
    if (ui->lineEditCodFabricante->text().isEmpty()) {
        listaDatosFormulario.append("0");
    }else{
        listaDatosFormulario.append(ui->lineEditCodFabricante->text());
    }
    if (ui->lineEditFoto->text() == "") {
        listaDatosFormulario.append("imagenes/1.jpg");
    }else{
    listaDatosFormulario.append(ui->lineEditFoto->text());
    }
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());
    listaDatosFormulario.append(ui->comboBoxFormato->currentText());
    if (ui->lineEditCantidad->text().isEmpty()) {
        listaDatosFormulario.append(nullptr);
    }else {
        listaDatosFormulario.append(ui->lineEditCantidad->text());
    }



    return listaDatosFormulario;
}


void Articulos::recargarTabla()
{
    qDebug() << conf->getConexionLocal();
    modeloTabla->setQuery("SELECT * FROM articulos",QSqlDatabase::database(conf->getConexionLocal()));
    mapper.setModel(modeloTabla);
}

void Articulos::cargarCompras(){
    modeloCompras.clear();
    if (ui->radioButtonFacturas->isChecked()) {
        modeloCompras.setQuery("SELECT `nDocumento` , `pedidos`.`idProveedor` , `cantidad` , `bonificacion` , `costo` , `descuento1`, `pedidos`.`fechaPedido` FROM `lineaspedido` JOIN `pedidos` on `nDocumento` = `pedidos`.`npedido` WHERE `cod` = '"
                               +ui->lineEditCod->text()+"' ORDER BY `pedidos`.`fechaPedido` DESC",QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonMeses->isChecked()) {
        modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                               +ui->lineEditCod->text()+"' GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC ",QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonAnos->isChecked()) {
        modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                +ui->lineEditCod->text()+"' GROUP BY YEAR(pedidos.fechaPedido) DESC",QSqlDatabase::database(conf->getConexionLocal()));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
}

void Articulos::cargarCodAux()
{
    modeloAux = new QSqlTableModel(this,QSqlDatabase::database(conf->getConexionLocal()));
    modeloAux->setTable("codaux");
    modeloAux->setEditStrategy(QSqlTableModel::OnRowChange);
    modeloAux->setFilter("cod ="+ui->lineEditCod->text());
    modeloAux->select();
    ui->tableViewAux->setModel(modeloAux);
    ui->tableViewAux->hideColumn(0);

}

void Articulos::llenarComboFormatos()
{
    consulta = base.devolverTablaCompleta("formatos");
    consulta.first();
    do{
        ui->comboBoxFormato->addItem(consulta.value("formato").toString());
    }while (consulta.next());
}

void Articulos::llenarStockRemoto(QString ean)
{
    qDebug() << listaConexionesRemotas.length();
    QStringList encabezado;
    encabezado << "Tienda" << "Stock";
    ui->treeWidgetStockTiendas->setColumnCount(2);
    ui->treeWidgetStockTiendas->setHeaderLabels(encabezado);
    ui->treeWidgetStockTiendas->clear();
    for (int i = 0; i < listaConexionesRemotas.length();i++) {
        QTreeWidgetItem *item = new QTreeWidgetItem(ui->treeWidgetStockTiendas);
        item->setText(0,listaConexionesRemotas.at(i));
        item->setText(1,base.sumarStockArticulo(ean,listaConexionesRemotas.at(i)));
        QSqlQuery lotes = base.lotesProducto(ean,listaConexionesRemotas.at(i));
        while (lotes.next()) {
            QTreeWidgetItem *lote = new QTreeWidgetItem(item);
            lote->setText(0,lotes.record().value("fecha").toString());
            lote->setText(1,lotes.record().value("cantidad").toString());
        }
    }

}

QStringList Articulos::crearConexionesRemotas(QSqlQuery consultaRemota)
{
//    QStringList listaConexionesRemotas;
//    listaConexionesRemotas.clear();
//    QSqlQuery tiendas = consultaRemota;
//    tiendas.first();
//    for (int i = 0; i < tiendas.numRowsAffected();i++) {
//        QString host = tiendas.value("ip").toString();
//        QString puerto = "3306";
//        QString baseDatos = "tienda";
//        QString usuario = tiendas.value("usuario").toString();
//        QString constrasena = tiendas.value("password").toString();
//        QString nombreConexion = tiendas.value("nombre").toString();
//        if(createConnection(host,puerto,baseDatos,usuario,constrasena,nombreConexion)){
//            qDebug() << "conexion creada: " << nombreConexion;
//            listaConexionesRemotas.append(nombreConexion);
//        }

//        tiendas.next();
//    }
//    return listaConexionesRemotas;

}

void Articulos::cargarVentas()
{
    modeloVentas.clear();
    if(ui->radioButtonVentasMes->isChecked()){
        modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0,Qt::Horizontal,"Artculo");
        modeloVentas.setHeaderData(1,Qt::Horizontal,"Año");
        modeloVentas.setHeaderData(2,Qt::Horizontal,"Mes");
        modeloVentas.setHeaderData(3,Qt::Horizontal,"Cantidad");

        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
    }
    if(ui->radioButtonVentasDia->isChecked()){

        modeloVentas.setQuery("SELECT descripcion , fecha , sum(cantidad) FROM lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' group by fecha desc",QSqlDatabase::database(conf->getConexionLocal()));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0,Qt::Horizontal,"Producto");
        modeloVentas.setHeaderData(1,Qt::Horizontal,"Fecha");
        modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");
        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
    }
    if(ui->radioButtonVentasAno->isChecked()){
        modeloVentas.clear();

        modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' GROUP BY YEAR(fecha) desc",QSqlDatabase::database(conf->getConexionLocal()));

        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0,Qt::Horizontal,"Artculo");
        modeloVentas.setHeaderData(1,Qt::Horizontal,"Año");
        modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");

        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
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
    foreach (QLineEdit* le, ui->General->findChildren<QLineEdit*>()) {
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
    qDebug() << datos;
        int i = mapper.currentIndex();

        QMessageBox msgBox;
         msgBox.setText("MODIFICACION.");
         msgBox.setInformativeText("Quiere guardar los cambios?");
         msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
         msgBox.setDefaultButton(QMessageBox::Ok);
         int resp = msgBox.exec();
         if(resp == QMessageBox::Ok){
        if (base.modificarArticulo(QSqlDatabase::database(conf->getConexionLocal()) , datos , ui->lineEditCod->text())){
                msgBox.setText("Guardado con exito");
                msgBox.setInformativeText("El registro se ha modificado correctamente");
                msgBox.setStandardButtons(QMessageBox::Ok);
                msgBox.exec();
        }else{
            msgBox.setText("Error al guardar");
            msgBox.setInformativeText("Revise los datos del formulario o contacte con el administrador");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
        recargarTabla();
        mapper.setCurrentIndex(i);
        refrescarBotones(mapper.currentIndex());

}
}

void Articulos::on_pushButtonBorrar_clicked()
{
    int i = mapper.currentIndex();

    QMessageBox msgBox;
     msgBox.setText("Borrar.");
     msgBox.setInformativeText("Seguro que quiere borrar este articulo?");
     msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
     msgBox.setDefaultButton(QMessageBox::Ok);
     int resp = msgBox.exec();
     if(resp == QMessageBox::Ok){
    base.borrarLotesArticulo(conf->getConexionLocal(),ui->lineEditCod->text());
    if (base.borrarArticulo(QSqlDatabase::database(conf->getConexionLocal()) , ui->lineEditCod->text())){
            msgBox.setText("Borrado con exito");
            msgBox.setInformativeText("El registro se ha borrado correctamente");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
    }else{
        msgBox.setText("Error al borrar");
        msgBox.setInformativeText("Revise los datos del formulario o contacte con el administrador");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(mapper.currentIndex());

}
}

void Articulos::on_pushButtonPonerFoto_clicked()
{
    int curr = mapper.currentIndex();
    QString dir = QDir::currentPath();
    int i = dir.length();
    QString fichero = QFileDialog::getOpenFileName(this,"Elige el archivo",dir);
    fichero.remove(0,i+1);//Quita i caracteres desde la posicion 0 de la cadena fichero i=longitud del path
    qDebug() << fichero;
    qDebug() << ui->lineEditCod->text().toInt();
    base.modificarFotoArticulo(fichero,ui->lineEditCod->text());
    recargarTabla();

    mapper.setCurrentIndex(curr);
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonBorrarFoto_clicked()
{
    int curr = mapper.currentIndex();
    base.modificarFotoArticulo("",ui->lineEditCod->text());
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
    consulta = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()),"articulos","descripcion", ui->lineEditDesc->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    qDebug() << buscar->resultado;
    for (int i = 0; i < modeloTabla->rowCount(); i++){
        if (modeloTabla->record(i).value("cod").toString() == buscar->resultado) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
    delete buscar;
}

void Articulos::on_lineEditCod_returnPressed()
{
    for (int i = 0; i < modeloTabla->rowCount(); i++){
        if (modeloTabla->record(i).value("cod").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            return;
        }
    }
        qDebug() << "MAL";
        QString cod = base.codigoDesdeAux(ui->lineEditCod->text());
        consulta = base.consulta_producto(conf->getConexionLocal(),cod);
        consulta.first();
        if (consulta.numRowsAffected() == 1) {
            ui->lineEditCod->setText(consulta.value(0).toString());
            emit on_lineEditCod_returnPressed();
                }
    QMessageBox msgbox;
    msgbox.setText("NO SE ENCUENTRA EL ARTÍCULO");
    msgbox.setInformativeText("Desea buscar los datos en otras tiendas?");
    msgbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgbox.setDefaultButton(QMessageBox::No);
    if(msgbox.exec() == QMessageBox::Yes){
        qDebug() << "Entrando en buscar";
        qDebug() << "Lista conexiones:" << listaConexionesRemotas.length();
        if(listaConexionesRemotas.isEmpty()){
            listaConexionesRemotas = crearConexionesRemotas(base.tiendas(QSqlDatabase::database(conf->getConexionLocal())));
            qDebug() << "Lista conexiones remotas vacia." << listaConexionesRemotas;

        }
        for (int i = 0 ;i < listaConexionesRemotas.length() ; i++) {
            QSqlQuery consulta = base.consulta_producto(listaConexionesRemotas.at(i),ui->lineEditCod->text());
            qDebug() << consulta.lastQuery();
            qDebug() << listaConexionesRemotas.at(i);
            qDebug() << ui->lineEditCod->text();
            if (consulta.numRowsAffected() > 0) {
                consulta.first();
                QSqlRecord q=consulta.record();
                QStringList datos;
                datos.clear();
                for (int i = 0; i < q.count(); i++) {
                    datos.append(q.value(i).toString());
                    qDebug() << q.value(i).toString();
                }
                msgbox.setText("¿UTILIZAR ESTOS DATOS?");
                msgbox.setInformativeText(datos.join("\n"));
                msgbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
                if(msgbox.exec() == QMessageBox::Ok){
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

void Articulos::on_pushButton_2_clicked()
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
    if(consulta.numRowsAffected() > 0){
        QMessageBox::warning(this, "ATENCION",
                              "El registro ya existe");
        return;
    }


    QStringList datos = recogerDatosFormulario();
    if (base.insertarArticulo(QSqlDatabase::database(conf->getConexionLocal()),datos)) {
        QMessageBox::about(this,"Atención", "Artículo creado con éxito");
    } else {
        QMessageBox::warning(this,"Error","No se ha podido crear el artículo");
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
    if(!ui->lineEditAux->text().isEmpty()){
        modeloAux->insertRow(0);
    QSqlRecord record = modeloAux->record();
    record.setValue(1,ui->lineEditCod->text());
    record.setValue(2,ui->lineEditAux->text());
    modeloAux->setRecord(0,record);
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
    if(ui->radioButtonFacturas->isChecked()){
    QModelIndex indice=modeloCompras.index(index.row(),1);
    idProveedor = modeloCompras.data(indice,Qt::EditRole).toString();
    ui->labelProveedor->setText(base.nombreProveedor(idProveedor));
    indice = modeloCompras.index(index.row(),0);
    nFactura = modeloCompras.data(indice,Qt::EditRole).toString();
    }
}

void Articulos::on_pushButtonEtiqueta_clicked()
{
    base.insertarEtiqueta(ui->lineEditCod->text());
}

void Articulos::on_pushButtonCaducados_clicked()
{
    Caducados *cad = new Caducados(ui->lineEditCod->text(),this);
    cad->exec();
}

void Articulos::on_pushButtonVer_2_clicked()
{

}

void Articulos::mostrarFoto()
{
    VisorImagenes *visor = new VisorImagenes(ui->lineEditFoto->text());
    visor->showMaximized();
}




ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent)
{

}

ClickableLabel::~ClickableLabel()
{

}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    emit clicked();
}



void Articulos::on_pushButtonVerFactura_clicked()
{
    if(nFactura.isEmpty()){
        QMessageBox::information(this,"No hay factura seleccionada","Seleccione una factura antes de usar esta opcion");
        return;
    }
    QStringList datos;
    imprimirFacturaProveedor facturaHtml(conf->getConexionLocal(),datos,nFactura);

//    VisorFacturas *factura = new VisorFacturas(nFactura,this);
//    factura->show();

}

void Articulos::on_checkBoxRemoto_stateChanged(int arg1)
{
    if( remoto == false && arg1 == 2){
//        QSqlQuery consultaRemota = base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
//        listaConexionesRemotas = crearConexionesRemotas(consultaRemota);
//        qDebug() << listaConexionesRemotas;
//        remoto = true;

    }
    refrescarBotones(mapper.currentIndex());
}



void Articulos::on_treeWidgetStockTiendas_itemDoubleClicked(QTreeWidgetItem *item, int column)
{
    QString baseDatosRemota;
    if (!item->parent()) {
        baseDatosRemota = item->text(0);
    }else{
    baseDatosRemota = item->parent()->text(0);
}
    comprasVentasRemoto *cvr = new comprasVentasRemoto(QSqlDatabase::database(baseDatosRemota),ui->lineEditCod->text());
    cvr->show();
}
