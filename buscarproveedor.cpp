#include "buscarproveedor.h"
#include "ui_buscarproveedor.h"


BuscarProveedor::BuscarProveedor(QWidget *parent,QString nombre) :
    QDialog(parent),
    ui(new Ui::BuscarProveedor)
{
    ui->setupUi(this);

    QSqlQuery query = base->buscarEnTabla(QSqlDatabase::database("DB"),"proveedores","nombre",nombre);
    //query.exec("SELECT * FROM proveedores WHERE nombre LIKE '%"+nombre+"%'");
    query.first();
    modelo.setQuery(query);
    ui->tableView->hideColumn(0);
    ui->tableView->setModel(&modelo);
}

BuscarProveedor::~BuscarProveedor()
{
    delete ui;
}

void BuscarProveedor::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice=modelo.index(index.row(),0);
    QVariant dato = modelo.data(indice,Qt::EditRole);
    resultado = dato.toString();
    qDebug() << resultado;

}

void BuscarProveedor::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex indice=modelo.index(index.row(),0);
    QVariant dato = modelo.data(indice,Qt::EditRole);
    resultado = dato.toString();
}
