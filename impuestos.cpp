#include "impuestos.h"
#include "ui_impuestos.h"
#include<QMessageBox>

impuestos::impuestos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::impuestos)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this,QSqlDatabase::database(conf->getConexionLocal()));
    modelo->setTable("impuestos");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->hideColumn(0);
}

impuestos::~impuestos()
{
    delete ui;
}

void impuestos::on_pushButtonAgregar_clicked()
{
    int row = modelo->rowCount();
    modelo->insertRow(row);
    ui->tableView->selectRow(row);
}


void impuestos::on_pushButtonElimiar_clicked()
{
    QModelIndexList seleccion = ui->tableView->selectionModel()->selectedRows();
    if (!seleccion.isEmpty()) {
        modelo->removeRow(seleccion.first().row());
    }else{
        QMessageBox::warning(this,"Eliminar IVA","Selecciones una fila primero");
    }
}


void impuestos::on_pushButtonGuardar_clicked()
{
    if (modelo->submitAll()) {
        QMessageBox::information(this,"Guardar","Los cambios se han guardado");
    }else{
        QMessageBox::critical(this, "Guardar", "No se han podido guardar los cambios");
    }
}

