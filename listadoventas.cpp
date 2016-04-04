#include "listadoventas.h"
#include "ui_listadoventas.h"
#include <qstandarditemmodel.h>
#include "qtrpt.h"

ListadoVentas::ListadoVentas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListadoVentas)
{
    ui->setupUi(this);
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());

}

ListadoVentas::~ListadoVentas()
{
    delete ui;
}

void ListadoVentas::on_pushButtonVer_clicked()
{
    QString fechaI,fechaF;
    fechaF = ui->dateEditHasta->text();
    fechaI = ui->dateEditDesde->text();
    modeloTabla = new QStandardItemModel(this);
    QSqlQuery ventasA = base->ventasEntreFechas(fechaI,fechaF,"tickets");
    int i = 0;
    while (ventasA.next()) {
        QStandardItem *itemFecha = new QStandardItem(ventasA.value(0).toString());
        QStandardItem *itemVentas = new QStandardItem(ventasA.value(1).toString());
        modeloTabla->setItem(i,0,itemFecha);
        modeloTabla->setItem(i,1,itemVentas);
        ++i;
    }
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        QString fecha = modeloTabla->item(i,0)->text();
        QSqlQuery ventasB = base->ventasEntreFechas(fecha,fecha,"ticketss");
        ventasB.first();
        QStandardItem *itemVentas = new QStandardItem(ventasB.value(1).toString());
        modeloTabla->setItem(i,2,itemVentas);
    }


    modeloTabla->setHeaderData(0,Qt::Horizontal,"FECHA");
    modeloTabla->setHeaderData(1,Qt::Horizontal,"VENTAS");
    modeloTabla->setHeaderData(2,Qt::Horizontal,"VENTAS B");
    ui->tableView->setModel(modeloTabla);
    sumarVentas(modeloTabla);
}

void ListadoVentas::on_tableView_activated(const QModelIndex &index)
{

    QModelIndex indice = modeloTabla->index(index.row(),0);
    QString fecha = modeloTabla->data(indice,Qt::DisplayRole).toString();
    QSqlQuery ventasFormaPago = base->ventas(fecha);

    QStandardItemModel *modeloFPago = new QStandardItemModel;
    int i = 0;
    while (ventasFormaPago.next()) {
        QString fPago = base->nombreFormaPago(ventasFormaPago.value(1).toString());
        QStandardItem *itemFPago = new QStandardItem(fPago);
        modeloFPago->setItem(i,0,itemFPago);
        QStandardItem *itemVentas = new QStandardItem(ventasFormaPago.value(0).toString());
        modeloFPago->setItem(i,1,itemVentas);
        i++;
    }
    modeloFPago->setHeaderData(0,Qt::Horizontal,"Forma de pago");
    modeloFPago->setHeaderData(1,Qt::Horizontal,"Cantidad");
    ui->tableViewFPago->setModel(modeloFPago);

    QStandardItemModel *modeloUsuario = new QStandardItemModel;
    QSqlQuery ventasPorUsusario = base->ventasPorUsusario(fecha);
    i=0;
    while (ventasPorUsusario.next()) {
        QString usuario = base->nombreUsusario(ventasPorUsusario.value(1).toString());
        QStandardItem *itemUsuario = new QStandardItem(usuario);
        modeloUsuario->setItem(i,0,itemUsuario);
        QStandardItem *itemVentasUsusario = new QStandardItem(ventasPorUsusario.value(0).toString());
        modeloUsuario->setItem(i,1,itemVentasUsusario);
        i++;
    }
    modeloUsuario->setHeaderData(0,Qt::Horizontal,"Ususario");
    modeloUsuario->setHeaderData(1,Qt::Horizontal,"Ventas");
    ui->tableViewVendedores->setModel(modeloUsuario);
}

void ListadoVentas::sumarVentas(QStandardItemModel *modelo)
{
    double A = 0;
    double B = 0;
    for (int i = 0; i < modelo->rowCount(); ++i) {
        A += modelo->item(i,1)->text().toDouble();
        B += modelo->item(i,2)->text().toDouble();
    }
    ui->labelTotal->setText(QString::number(A));
    ui->labelTotalB->setText(QString::number(B));
}

void ListadoVentas::on_pushButtonImprimir_clicked()
{
    QtRPT *informe = new QtRPT(this);
    informe->recordCount.append(modeloTabla->rowCount());
    informe->loadReport(":/archivos/ventas.xml");

    connect(informe, &QtRPT::setValue, [&](const int recNo,
            const QString paramName,
            QVariant &paramValue,
            const int reportPage) {
        (void) reportPage;
        if(paramName == "Fecha"){
            paramValue = modeloTabla->item(recNo,0)->text();
        }
        if(paramName == "Cantidad"){
            paramValue = modeloTabla->item(recNo,1)->text();
        }
    });
    informe->printExec();
}
