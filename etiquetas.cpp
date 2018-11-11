#include "etiquetas.h"
#include "ui_etiquetas.h"
#include <qtrpt.h>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>
#include "qtrpt.h"

Etiquetas::Etiquetas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Etiquetas)
{
    ui->setupUi(this);
    modelo = new QStandardItemModel;
    llenarModelo();

}

Etiquetas::~Etiquetas()
{
    delete ui;
}

void Etiquetas::on_pushButtonImprimir_clicked()
{

QStandardItemModel *modeloImpresion = new QStandardItemModel();
modeloImpresion->clear();
int contadorfila = 0;
int contadorcolumna = 0;
for (int i = 0;i < modelo->rowCount();i++) {

    QString textoFormateado = modelo->item(i,1)->text();
    if(textoFormateado.length() > 25){
        textoFormateado.insert(textoFormateado.indexOf(" ",20),"\n");
    }
    QStandardItem *nombreImp = new QStandardItem(textoFormateado);
    modeloImpresion->setItem(contadorfila,contadorcolumna,nombreImp);
    contadorcolumna++;

    QStandardItem *PVPImp = new QStandardItem(modelo->item(i,2)->text());
    modeloImpresion->setItem(contadorfila,contadorcolumna,PVPImp);
    contadorcolumna++;

    QString formato = modelo->item(i,3)->text();
    QString formatoEtiqueta;
    if(formato == "Uds"){
        formatoEtiqueta = "Precio / ud \n";
        double pvp = classFormatear.redondear(modelo->item(i,2)->text().toDouble()/modelo->item(i,4)->text().toDouble(),3);
        formatoEtiqueta += QString::number(pvp);
    }
    if(formato == "Peso"){
        formatoEtiqueta = "Precio / Kg \n";
        double pvp = classFormatear.redondear(modelo->item(i,2)->text().toDouble()*1000/modelo->item(i,4)->text().toDouble(),3);
        formatoEtiqueta += QString::number(pvp);
    }
    if(formato == "Volumen"){
        formatoEtiqueta = "Precio / L \n";
        double pvp = classFormatear.redondear(modelo->item(i,2)->text().toDouble()*1000/modelo->item(i,4)->text().toDouble(),3);
        formatoEtiqueta += QString::number(pvp);
    }
    if(formato == "No definido"){
        formatoEtiqueta = "--- / --- \n";
    }


    QStandardItem *formatoImp = new QStandardItem(formatoEtiqueta);
    modeloImpresion->setItem(contadorfila,contadorcolumna,formatoImp);
    contadorcolumna++;

    if (contadorcolumna == 9){
        contadorcolumna=0;
        contadorfila++;
    }

}
while(contadorcolumna < 9){
    QStandardItem *vacio = new QStandardItem("");
    modeloImpresion->setItem(contadorfila,contadorcolumna,vacio);

    contadorcolumna++;
}


QtRPT *informe = new QtRPT();
informe->recordCount.append(modeloImpresion->rowCount());
QString informeDir = base->devolverDirectorio("etiquetas");
informe->loadReport(informeDir);
connect(informe, &QtRPT::setValue, [&](const int recNo,
        const QString paramName,
        QVariant &paramValue,
        const int reportPage) {
    (void) reportPage;
    if(paramName == "producto1"){
        paramValue = modeloImpresion->item(recNo,0)->text();
    }
    if(paramName == "pvp1"){
        paramValue = modeloImpresion->item(recNo,1)->text();
    }if(paramName == "producto2"){
        paramValue = modeloImpresion->item(recNo,3)->text();
    }
    if(paramName == "pvp2"){
        paramValue = modeloImpresion->item(recNo,4)->text();
    }if(paramName == "producto3"){
        paramValue = modeloImpresion->item(recNo,6)->text();
    }
    if(paramName == "pvp3"){
        paramValue = modeloImpresion->item(recNo,7)->text();
    }
    if(paramName == "peso1"){
        paramValue = modeloImpresion->item(recNo,2)->text();
    }
    if(paramName == "peso2"){
        paramValue = modeloImpresion->item(recNo,5)->text();
    }
    if(paramName == "peso3"){
        paramValue = modeloImpresion->item(recNo,8)->text();
    }


});
informe->printExec();
}

void Etiquetas::on_lineEditCod_returnPressed()
{
    if (base->existeDatoEnTabla(QSqlDatabase::database("DB"),"articulos","cod",ui->lineEditCod->text())) {
        base->insertarEtiqueta(ui->lineEditCod->text());
        llenarModelo();
    }
}

void Etiquetas::llenarModelo()
{
    modelo->clear();
    QSqlQuery listaEtiquetas = base->ejecutarSentencia("SELECT * FROM etiquetas");
    listaEtiquetas.first();
    for (int i = 0; i < listaEtiquetas.numRowsAffected(); ++i) {
        qDebug() << "COD: " << listaEtiquetas.record().value(0).toString();
        QSqlQuery producto = base->consulta_producto(QSqlDatabase::database("DB"),listaEtiquetas.record().value(0).toString());
        producto.first();
        double pvp = producto.value(2).toDouble();
        QStandardItem *cod = new QStandardItem(producto.value(0).toString());
        QStandardItem *nombre = new QStandardItem(producto.value(1).toString());
        QStandardItem *precio = new QStandardItem(QString::number(pvp,'f',2));
        QStandardItem *formato = new QStandardItem(producto.value("formato").toString());
        QStandardItem *cantidad = new QStandardItem(producto.value("cantformato").toString());
        modelo->setItem(i,0,cod);
        modelo->setItem(i,1,nombre);
        modelo->setItem(i,2,precio);
        modelo->setItem(i,3,formato);
        modelo->setItem(i,4,cantidad);
        listaEtiquetas.next();

    }
    ui->tableView->setModel(modelo);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditCod->setFocus();
}

void Etiquetas::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database("DB"),"articulos",ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscarProducto = new BuscarProducto(this,consulta);
    buscarProducto->exec();
    ui->lineEditCod->setText(buscarProducto->resultado);
    emit on_lineEditCod_returnPressed();
}

void Etiquetas::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice=modelo->index(index.row(),0);

    QVariant dato = modelo->data(indice,Qt::EditRole);

    articuloSeleccionado = dato.toString();
}

void Etiquetas::on_pushButton_2_clicked()
{
    if (articuloSeleccionado.isEmpty()) {
        QMessageBox *aviso = new QMessageBox(this);
        aviso->setWindowTitle("Aviso");
        aviso->setText("No se ha seleccionado ningún artículo para borrar");
        aviso->exec();
    } else {
        base->ejecutarSentencia("DELETE FROM etiquetas WHERE cod = '"+articuloSeleccionado+"'");
        llenarModelo();
    }
}

void Etiquetas::on_pushButton_3_clicked()
{
    QMessageBox *pregunta = new QMessageBox(this);
    pregunta->setWindowTitle("Borrar");
    pregunta->setText("¿Está seguro que quiere borrar todas las etiquetas?");
    pregunta->setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    pregunta->setDefaultButton(QMessageBox::Cancel);
    if (pregunta->exec() == QMessageBox::Yes) {
        qDebug() << "aceptado";
        base->ejecutarSentencia("TRUNCATE etiquetas");
        llenarModelo();
    }
    return;
}
