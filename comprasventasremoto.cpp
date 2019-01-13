#include "comprasventasremoto.h"
#include "ui_comprasventasremoto.h"

comprasVentasRemoto::comprasVentasRemoto(QSqlDatabase base, QString ean, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::comprasVentasRemoto)
{
    ui->setupUi(this);
    base.open();
    QSqlQueryModel *modeloVentas = new QSqlQueryModel();
    modeloVentas->setQuery("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+ean+"' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",base);
    modeloVentas->setHeaderData(0,Qt::Horizontal,"Artculo");
    modeloVentas->setHeaderData(1,Qt::Horizontal,"Año");
    modeloVentas->setHeaderData(2,Qt::Horizontal,"Mes");
    modeloVentas->setHeaderData(3,Qt::Horizontal,"Cantidad");
    ui->tableViewVentas->setModel(modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();

    modeloCompras = new QSqlQueryModel;
    modeloCompras->setQuery("SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                           +ean+"' GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC ",base);
    ui->tableViewCompras->setModel(modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}

comprasVentasRemoto::~comprasVentasRemoto()
{
    delete ui;
}
