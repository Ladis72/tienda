#include "tiposentradassalidas.h"
#include "ui_tiposentradassalidas.h"

TiposEntradasSalidas::TiposEntradasSalidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TiposEntradasSalidas)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    modelo->setTable("motivosEntrada");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
}

TiposEntradasSalidas::~TiposEntradasSalidas()
{
    delete ui;
}

void TiposEntradasSalidas::on_pushButton_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void TiposEntradasSalidas::on_pushButton_2_clicked()
{
    if (!modelo->submitAll()) {
        QMessageBox::warning(this,
                             "ATENCION",
                             "No se ha podido modificar el registro" + modelo->lastError().text());
    }
}

void TiposEntradasSalidas::on_lineEdit_textChanged(const QString &arg1)
{
    modelo->setFilter("descripcion LIKE '%" + arg1 + "%'");
}
