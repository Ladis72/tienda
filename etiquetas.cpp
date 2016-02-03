#include "etiquetas.h"
#include "ui_etiquetas.h"
#include <qtrpt.h>
#include <QPdfWriter>
#include <QPainter>

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
    int paginas = floor(filas/10)+1;
    QPdfWriter etiquetasPDF("./etiquetas.pdf");
    QPainter painter(&etiquetasPDF);
    for (int p = 0; p < paginas; ++p){
        for (int i = 0; i < filas; ++i) {
            int y = 300+2000*i;
            for (int j = 0; j < 3; ++j) {
                int x = 300+2900*j;
                if(i*3+j < etiquetas){
                    painter.setPen(Qt::black);
                    painter.setFont(QFont("Liberation",15));
                    painter.drawText(x,y,modelo->item(i*3+j,0)->text());
                    painter.setPen(Qt::blue);
                    painter.setFont(QFont("Arial",50));
                    painter.drawText(x+200,y+1000,modelo->item(i*3+j,1)->text());
                    }
                }
            }
        if (p > paginas -1) etiquetasPDF.newPage();
    }

    painter.end();
}

void Etiquetas::on_lineEditCod_returnPressed()
{
    if (base->existeDatoEnTabla(QSqlDatabase::database("DB"),"articulos","cod",ui->lineEditCod->text())) {
        base->insertarEtiqueta(ui->lineEditCod->text());
        llenarModelo();
    }
}

QString Etiquetas::formatearCadena(QString cadena)
{

}

void Etiquetas::llenarModelo()
{
    QSqlQuery listaEtiquetas = base->ejecutarSentencia("SELECT * FROM etiquetas");
    listaEtiquetas.first();
    for (int i = 0; i < listaEtiquetas.numRowsAffected(); ++i) {
        qDebug() << "COD: " << listaEtiquetas.record().value(0).toString();
        QSqlQuery producto = base->consulta_producto(QSqlDatabase::database("DB"),listaEtiquetas.record().value(0).toString());
        producto.first();
        double pvp = producto.value(2).toDouble();
        QStandardItem *nombre = new QStandardItem(producto.value(1).toString());
        QStandardItem *precio = new QStandardItem(QString::number(pvp,'f',2));
        modelo->setItem(i,0,nombre);
        modelo->setItem(i,1,precio);
        listaEtiquetas.next();

    }
    ui->tableView->setModel(modelo);
}
