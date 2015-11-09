#include "listadoventas.h"
#include "ui_listadoventas.h"
#include <qstandarditemmodel.h>

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
    modeloTabla = new QSqlQueryModel(this);
    modeloTabla->setQuery(base->ventasEntreFechas(fechaI,fechaF));
    modeloTabla->setHeaderData(0,Qt::Horizontal,"FECHA");
    modeloTabla->setHeaderData(1,Qt::Horizontal,"VENTAS");
    ui->tableView->setModel(modeloTabla);
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
