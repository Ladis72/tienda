#include "buscarproducto.h"
#include <QSqlRecord>
#include "ui_buscarproducto.h"

BuscarProducto::BuscarProducto(QWidget *parent, QSqlQuery query)
    : QDialog(parent)
    , ui(new Ui::BuscarProducto)
{
    ui->setupUi(this);
    // QSqlQueryModel *modelo = new QSqlQueryModel;
    modelo.setQuery(query);
    ui->tableView->setModel(&modelo);
    //ui->tableView->hideColumn(3);
    for (int i = 3; i < 21; ++i)
        ui->tableView->hideColumn(i);
    ui->tableView->hideColumn(22);
    ui->tableView->setColumnWidth(1, 600);
}

BuscarProducto::~BuscarProducto()
{
    delete ui;
}

void BuscarProducto::on_tableView_clicked(const QModelIndex index)
{
    QModelIndex indice = modelo.index(index.row(), 0);

    QVariant dato = modelo.data(indice, Qt::EditRole);

    resultado = dato.toString();
    qDebug() << resultado;
}

void BuscarProducto::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex indice = modelo.index(index.row(), 0);

    QVariant dato = modelo.data(indice, Qt::EditRole);

    resultado = dato.toString();
}
