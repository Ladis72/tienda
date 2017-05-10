#include "etiquetas.h"
#include "ui_etiquetas.h"
#include <qtrpt.h>
#include <QPdfWriter>
#include <QPainter>
#include <QMessageBox>

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
    int etiquetas = modelo->rowCount();
    int filas = floor(modelo->rowCount()/3)+1;
    int paginas = floor(filas/7)+1;
    qDebug() << etiquetas << filas << paginas;
    QPdfWriter etiquetasPDF("./etiquetas.pdf");
    QPainter painter(&etiquetasPDF);
    for (int p = 0; p < paginas; ++p){
        for (int i = 0; i < 7; ++i) {
            int y = 300+1800*i;
            for (int j = 0; j < 3; ++j) {
                int x = 300+3100*j;
                if((21*p)+i*3+j < etiquetas){
                    painter.setPen(Qt::black);
                    painter.setFont(QFont("Liberation",15));
                    painter.drawText(QRect(x,y,3000,600),modelo->item((21*p)+i*3+j,1)->text());
                    painter.setPen(Qt::blue);
                    painter.setFont(QFont("Arial",50));
                    painter.drawText(x+200,y+1200,modelo->item((21*p)+i*3+j,2)->text());
                    }
                }
            }
        if (p < paginas -1) etiquetasPDF.newPage();
    }

    painter.end();
    system("okular ./etiquetas.pdf");
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
        modelo->setItem(i,0,cod);
        modelo->setItem(i,1,nombre);
        modelo->setItem(i,2,precio);
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
