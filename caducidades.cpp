#include "caducidades.h"
#include "ui_caducidades.h"

Caducidades::Caducidades(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caducidades)
{
    ui->setupUi(this);
    modelo = new QSqlRelationalTableModel(this,QSqlDatabase::database("DB"));
    modelo->setTable("lotes");
    modelo->setRelation(1,QSqlRelation("articulos","cod","descripcion"));
    modelo->setSort(3,Qt::AscendingOrder);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
}

Caducidades::~Caducidades()
{
    delete ui;
}
