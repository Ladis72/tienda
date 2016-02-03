#include "salidas.h"
#include "ui_salidas.h"

Salidas::Salidas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Salidas)
{
    ui->setupUi(this);
    mTablaSalidas = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    mTablaSalidas->setTable("salidaGenero_tmp");
    actualizarTabla();
}

Salidas::~Salidas()
{
    delete ui;
}

void Salidas::on_lineEditCod_returnPressed()
{
    consulta = base->consulta_producto(QSqlDatabase::database("DB"),ui->lineEditCod->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base->codigoDesdeAux(ui->lineEditCod->text());
        consulta = base->consulta_producto(QSqlDatabase::database("DB"),cod);
        consulta.first();
    }
    if (consulta.numRowsAffected() == 1) {
        ui->lineEditDesc->setText(consulta.value(1).toString());
        ui->groupBox->setTitle(consulta.value(1).toString());
    }
    QSqlQuery consulta2 = base->lotesProducto(consulta.value(0).toString());
    consulta2.first();
    QList<QLabel *> fechas;
    QList<QLabel *> cantidad;
    QList<QLineEdit *> cantidadSalida;

}

void Salidas::actualizarTabla()
{
    mTablaSalidas->select();
    mTablaSalidas->setSort(3,Qt::AscendingOrder);
    ui->tableView->setModel(mTablaSalidas);
}
