#include "prestamistas.h"
#include <QMessageBox>
#include "ui_prestamistas.h"

Prestamistas::Prestamistas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Prestamistas)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    modelo->setTable("prestamistas");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
}

Prestamistas::~Prestamistas()
{
    delete ui;
}

void Prestamistas::on_pushButtonAnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void Prestamistas::on_pushButtonModificar_clicked()
{
    if (!modelo->submitAll()) {
        QMessageBox::warning(this,
                             "ATENCION",
                             "No se ha podido modificar el registro" + modelo->lastError().text());
    }
}
