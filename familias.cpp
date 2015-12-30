#include "familias.h"
#include "ui_familias.h"
#include <QMessageBox>

Familias::Familias(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Familias)
{
    ui->setupUi(this);

    modelo = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modelo->setTable("familias");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->lineEdit->setFocus();


}

Familias::~Familias()
{
    delete ui;
}

void Familias::on_pushButtonAnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void Familias::on_pushButtonModificar_clicked()
{
    if(!modelo->submitAll()){
        QMessageBox::warning(this, "ATENCION",
                             "No se ha podido modificar el registro"+modelo->lastError().text());
    }
}

void Familias::on_pushButtonDeshacer_clicked()
{
    modelo->revertAll();
}

void Familias::on_pushButtonBorrar_clicked()
{
    modelo->removeRow(Qt::EditRole);
}

void Familias::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(),0);
    resultado = modelo->data(indice).toString();
    qDebug() << resultado;
}

void Familias::on_lineEdit_textChanged(const QString &arg1)
{
    modelo->setFilter("descripcion LIKE '%"+ui->lineEdit->text()+"%'");
}
