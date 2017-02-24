#include "articulos.h"
#include "ui_articulos.h"
#include "caducados.h"

#include <QDir>
#include <QMessageBox>
#include <QFileDialog>

Articulos::Articulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Articulos)
{
    ui->setupUi(this);

    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod,0);
    mapper.addMapping(ui->lineEditDesc,1);
    mapper.addMapping(ui->lineEditPvp,2);
    mapper.addMapping(ui->lineEditIva,3);
    mapper.addMapping(ui->lineEditStock,4);
    mapper.addMapping(ui->lineEditMinimo,5);
    mapper.addMapping(ui->lineEditMaximo,6);
    mapper.addMapping(ui->lineEditPendientes,8);
    mapper.addMapping(ui->dateEditUltimaVenta,9);
    mapper.addMapping(ui->dateEditUltimoPedido,10);
    mapper.addMapping(ui->lineEditCodFamila,11);
    mapper.addMapping(ui->lineEditCosto,12);
    mapper.addMapping(ui->lineEditCodFabricante,13);
    mapper.addMapping(ui->lineEditFoto,14);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
    borrarFormulario();
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
    cargarVentas();
    cargarCompras();
    cargarCodAux();
}

QStringList Articulos::recogerDatosFormulario()
{
    QStringList listaDatosFormulario;
    listaDatosFormulario.clear();
    listaDatosFormulario.append(ui->lineEditCod->text());
    listaDatosFormulario.append(ui->lineEditDesc->text());
    listaDatosFormulario.append(ui->lineEditPvp->text());
    listaDatosFormulario.append(ui->lineEditIva->text());
    listaDatosFormulario.append(ui->lineEditStock->text());
    listaDatosFormulario.append(ui->lineEditMinimo->text());
    listaDatosFormulario.append(ui->lineEditMaximo->text());
    listaDatosFormulario.append(ui->lineEditPendientes->text());
    listaDatosFormulario.append(ui->lineEditEncargados->text());
    listaDatosFormulario.append(ui->dateEditUltimaVenta->text());
    listaDatosFormulario.append(ui->dateEditUltimoPedido->text());
    listaDatosFormulario.append(ui->lineEditCodFamila->text());
    listaDatosFormulario.append(ui->lineEditCosto->text());
    listaDatosFormulario.append(ui->lineEditCodFabricante->text());
    if (ui->lineEditFoto->text() == "") {
        listaDatosFormulario.append("imagenes/1.jpg");
    }else{
    listaDatosFormulario.append(ui->lineEditFoto->text());
    }
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());



    return listaDatosFormulario;
}


void Articulos::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM articulos",QSqlDatabase::database("DB"));
    mapper.setModel(modeloTabla);
}

void Articulos::cargarCompras(){
    modeloCompras.clear();
    if (ui->radioButtonFacturas->isChecked()) {
        modeloCompras.setQuery("SELECT `nDocumento` , `pedidos`.`idProveedor` , `cantidad` , `bonificacion` , `costo` , `descuento1`, `pedidos`.`fechaPedido` FROM `lineaspedido` JOIN `pedidos` on `nDocumento` = `pedidos`.`npedido` WHERE `cod` = '"
                               +ui->lineEditCod->text()+"' ORDER BY `pedidos`.`fechaPedido` DESC",QSqlDatabase::database("DB"));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonMeses->isChecked()) {
        modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                               +ui->lineEditCod->text()+"' GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC ",QSqlDatabase::database("DB"));
        qDebug() << modeloCompras.lastError();
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonAnos->isChecked()) {
        modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                +ui->lineEditCod->text()+"' GROUP BY YEAR(pedidos.fechaPedido) DESC",QSqlDatabase::database("DB"));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
}

void Articulos::cargarCodAux()
{
    modeloAux = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modeloAux->setTable("codaux");
    modeloAux->setEditStrategy(QSqlTableModel::OnRowChange);
    modeloAux->setFilter("cod ="+ui->lineEditCod->text());
    modeloAux->select();
    ui->tableViewAux->setModel(modeloAux);
    ui->tableViewAux->hideColumn(0);

}

void Articulos::cargarVentas()
{
    modeloVentas.clear();
    if(ui->radioButtonVentasMes->isChecked()){
//        modeloVentas.setQuery("SELECT `descripcion`, YEAR(`tickets`.`fecha`) , MONTH(`tickets`.`fecha`) ,sum(`cantidad`)FROM `lineasticket` join `tickets` on `nticket` = `tickets`.`ticket` where `cod` = '"
//                              +ui->lineEditCod->text()+"' group by year(`tickets`.`fecha`) desc, month(`tickets`.`fecha`) desc ",QSqlDatabase::database("DB"));
        modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",QSqlDatabase::database("DB"));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0,Qt::Horizontal,"Artculo");
        modeloVentas.setHeaderData(1,Qt::Horizontal,"Año");
        modeloVentas.setHeaderData(2,Qt::Horizontal,"Mes");
        modeloVentas.setHeaderData(3,Qt::Horizontal,"Cantidad");

        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
    }
    if(ui->radioButtonVentasDia->isChecked()){
//        modeloVentas.setQuery("SELECT `descripcion`, `tickets`.`fecha` ,sum(`cantidad`)FROM `lineasticket` join `tickets` on `nticket` = `tickets`.`ticket` where `cod` = '"
//                              +ui->lineEditCod->text()+"' group by `tickets`.`fecha` desc ",QSqlDatabase::database("DB"));
//        qDebug() << modeloVentas.lastError();
//        modeloVentas.setHeaderData(0,Qt::Horizontal,"Artículo");
//        modeloVentas.setHeaderData(1,Qt::Horizontal,"Fecha");
//        modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");

//        ui->tableViewVentas->setModel(&modeloVentas);
//        ui->tableViewVentas->resizeColumnsToContents();
        modeloVentas.setQuery("SELECT descripcion , fecha , sum(cantidad) FROM lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' group by fecha desc",QSqlDatabase::database("DB"));
        qDebug() << modeloVentas.lastError();
        modeloVentas.setHeaderData(0,Qt::Horizontal,"Producto");
        modeloVentas.setHeaderData(1,Qt::Horizontal,"Fecha");
        modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");
        ui->tableViewVentas->setModel(&modeloVentas);
        ui->tableViewVentas->resizeColumnsToContents();
    }
    if(ui->radioButtonVentasAno->isChecked()){
        modeloVentas.clear();
//        modeloVentas.setQuery("SELECT `descripcion`, YEAR(`tickets`.`fecha`) ,sum(`cantidad`)FROM `lineasticket` join `tickets` on `nticket` = `tickets`.`ticket` where `cod` = '"
//                              +ui->lineEditCod->text()+"' group by year(`tickets`.`fecha`) desc",QSqlDatabase::database("DB"));
        modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+ui->lineEditCod->text()+"' GROUP BY YEAR(fecha) desc",QSqlDatabase::database("DB"));

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
            qDebug() << "Event";
            return true;
        }
    }
    return false;
}

void Articulos::borrarFormulario()
{
    foreach (QLineEdit* le, ui->General->findChildren<QLineEdit*>()) {
        le->clear();
        qDebug() << "Borrando line edit";
    }
    ui->dateEditUltimaVenta->setDate(QDate::fromString("2000-01-01"));
    ui->dateEditUltimoPedido->setDate(QDate::fromString("2000-01-01"));
    ui->labelFoto->clear();
    ui->labelNombrePrecio->clear();
}


void Articulos::on_pushButtonAnterior_clicked()
{
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Articulos::on_pushButtonSiguiente_clicked()
{
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
        if (base.modificarArticulo(QSqlDatabase::database("DB") , datos , ui->lineEditCod->text())){
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
    if (base.borrarArticulo(QSqlDatabase::database("DB") , ui->lineEditCod->text())){
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
    consulta = base.buscarEnTabla(QSqlDatabase::database("DB"),"articulos","descripcion", ui->lineEditDesc->text());
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



void Articulos::on_lineEditCod_editingFinished()
{
    for (int i = 0; i < modeloTabla->rowCount(); i++){
        if (modeloTabla->record(i).value("cod").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
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
    consulta = base.consulta_producto(QSqlDatabase::database("DB"), ui->lineEditCod->text());
    if(consulta.numRowsAffected() > 0){
        QMessageBox::warning(this, "ATENCION",
                              "El registro ya existe");
        return;
    }


    QStringList datos = recogerDatosFormulario();
    if (base.insertarArticulo(QSqlDatabase::database("DB"),datos)) {
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
}

void Articulos::on_radioButtonMeses_clicked()
{
    cargarCompras();

}

void Articulos::on_radioButtonAnos_clicked()
{
    cargarCompras();

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
    QModelIndex indice=modeloCompras.index(index.row(),1);

    QString dato=modeloCompras.data(indice,Qt::EditRole).toString();
    ui->labelProveedor->setText(base.nombreProveedor(dato));
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
