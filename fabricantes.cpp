#include "fabricantes.h"
#include "ui_fabricantes.h"

#include <QMessageBox>

Fabricantes::Fabricantes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Fabricantes)
{
    ui->setupUi(this);

    modelo = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modelo->setTable("fabricantes");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
    ui->lineEdit->setFocus();

}

Fabricantes::~Fabricantes()
{
    delete ui;
}
void Fabricantes::on_pushButtonAnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void Fabricantes::on_pushButtonModificar_clicked()
{
    if(!modelo->submitAll()){
        QMessageBox::warning(this, "ATENCION",
                             "No se ha podido modificar el registro"+modelo->lastError().text());
    }
}

void Fabricantes::on_pushButtonDeshacer_clicked()
{
    modelo->revertAll();
}

void Fabricantes::on_pushButtonBorrar_clicked()
{
    modelo->removeRow(Qt::EditRole);
}

void Fabricantes::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(),0);
    resultado = modelo->data(indice).toString();
    qDebug() << resultado;
}


void Fabricantes::on_lineEdit_textChanged(const QString &arg1)
{
    modelo->setFilter("nombre LIKE '%"+arg1+"%'");

}
