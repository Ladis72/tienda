#include "formaspago.h"
#include "ui_formaspago.h"



FormasPago::FormasPago(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormasPago)
{
    ui->setupUi(this);

    modelolista = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modelolista->setTable("fpago");
    modelolista->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelolista->select();
    ui->tableView->setModel(modelolista);
    ui->tableView->hideColumn(0);
}

FormasPago::~FormasPago()
{
    delete ui;

}

void FormasPago::on_pushButtonModificar_clicked()
{
    modelolista->submitAll();
}
