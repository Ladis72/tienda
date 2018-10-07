#include "stock.h"
#include "ui_stock.h"

Stock::Stock(QString cod ,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stock)
{
    DateEditDelegate *dateDelegate =new DateEditDelegate;
    ui->setupUi(this);
    QSqlQuery producto(base->consulta_producto(QSqlDatabase::database("DB"),cod));
    producto.first();
    codProducto = producto.value(0).toString();
    ui->label->setText(producto.value(1).toString());
    modeloLotes = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modeloLotes->setTable("lotes");
    modeloLotes->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloLotes->setFilter("ean = '"+codProducto+"'");
    modeloLotes->select();
    ui->tableView->setModel(modeloLotes);
    ui->tableView->hideColumn(0);
    //ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->setItemDelegateForColumn(3,dateDelegate);


}

Stock::~Stock()
{
    delete ui;
}

void Stock::on_pushButtonActualizar_clicked()
{
    modeloLotes->submitAll();
    qDebug() << modeloLotes->lastError();
}

void Stock::on_pushButton_clicked()
{
    modeloLotes->insertRow(0);
    QSqlRecord record = modeloLotes->record();
    record.setValue(1,codProducto);
    qDebug() << codProducto;
    record.setValue(2,"");

    modeloLotes->setRecord(0,record);

}

void Stock::on_pushButtonBorrar_clicked()
{
    modeloLotes->removeRow(filaSeleccionada);
}

void Stock::on_tableView_clicked(const QModelIndex &index)
{
    filaSeleccionada = index.row();
}

void Stock::keyPressEvent(QKeyEvent *e)
{
    if(e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_F11){
        ui->pushButton->setEnabled(true);
        ui->pushButtonActualizar->setEnabled(true);
        ui->pushButtonBorrar->setEnabled(true);
    }

}
