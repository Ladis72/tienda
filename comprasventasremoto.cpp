#include "comprasventasremoto.h"
#include "ui_comprasventasremoto.h"

comprasVentasRemoto::comprasVentasRemoto(QSqlDatabase base, QString ean, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::comprasVentasRemoto)
{
    ui->setupUi(this);
    base.open();
    db = base;
    codigo = ean;
    emit on_radioButtonVentaMes_clicked();
    emit on_radioButtonComprasMes_clicked();


}

comprasVentasRemoto::~comprasVentasRemoto()
{
    delete ui;
}



void comprasVentasRemoto::on_radioButtonVentasDia_clicked()
{
    modeloVentas.clear();

    modeloVentas.setQuery("SELECT descripcion , fecha , sum(cantidad) FROM lineasticket WHERE cod = '"+codigo+"'GROUP BY fecha DESC",db);
    modeloVentas.setHeaderData(0,Qt::Horizontal,"Producto");
    modeloVentas.setHeaderData(1,Qt::Horizontal,"Fecha");
    modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");
    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonVentaMes_clicked()
{
    modeloVentas.clear();

    modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+codigo+"' GROUP BY YEAR(fecha) desc , MONTH(fecha) desc",db);
    modeloVentas.setHeaderData(0,Qt::Horizontal,"Artculo");
    modeloVentas.setHeaderData(1,Qt::Horizontal,"Año");
    modeloVentas.setHeaderData(2,Qt::Horizontal,"Mes");
    modeloVentas.setHeaderData(3,Qt::Horizontal,"Cantidad");
    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonVentasAno_clicked()
{
    modeloVentas.clear();

    modeloVentas.setQuery("SELECT descripcion , YEAR(fecha) , sum(cantidad) from lineasticket WHERE cod = '"+codigo+"' GROUP BY YEAR(fecha) desc",db);
    modeloVentas.setHeaderData(0,Qt::Horizontal,"Artculo");
    modeloVentas.setHeaderData(1,Qt::Horizontal,"Año");
    modeloVentas.setHeaderData(2,Qt::Horizontal,"Cantidad");

    ui->tableViewVentas->setModel(&modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasDia_clicked()
{
    modeloCompras.clear();
    modeloCompras.setQuery("SELECT `nDocumento` , `pedidos`.`idProveedor` , `cantidad` , `bonificacion` , `costo` , `descuento1`, `pedidos`.`fechaPedido` FROM `lineaspedido` JOIN `pedidos` on `nDocumento` = `pedidos`.`npedido` WHERE `cod` = '"
                           +codigo+"' ORDER BY `pedidos`.`fechaPedido` DESC",db);
    qDebug() << modeloCompras.lastError();
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasMes_clicked()
{
    modeloCompras.clear();
    modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
                           +codigo+"' GROUP BY YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC ",db);
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasano_clicked()
{
    modeloCompras.clear();
    modeloCompras.setQuery("SELECT YEAR(pedidos.fechaPedido) , sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = pedidos.npedido WHERE cod = '"
            +codigo+"' GROUP BY YEAR(pedidos.fechaPedido) DESC",db);
    ui->tableViewCompras->setModel(&modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}
