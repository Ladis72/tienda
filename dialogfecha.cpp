#include "dialogfecha.h"
#include <QMessageBox>
#include "ui_dialogfecha.h"

DialogFecha::DialogFecha(QString producto, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogFecha)
{
    ui->setupUi(this);
    ui->label->setText(producto);
    lote = "";
    ui->dateEdit->setDate(QDate::currentDate());
    fecha = ui->dateEdit->date();
}

DialogFecha::~DialogFecha()
{
    delete ui;
}

//void DialogFecha::on_buttonBox_accepted()
//{

//}

void DialogFecha::on_dateEdit_userDateChanged(const QDate &date)
{
    fecha = date;
    lote = ui->lineEdit->text().trimmed();
}

void DialogFecha::on_pushButton_clicked()
{
    // Recoger siempre la fecha y el lote actuales al pulsar aceptar,
    // por si el usuario escribió el lote después de seleccionar la fecha.
    fecha = ui->dateEdit->date();
    lote = ui->lineEdit->text().trimmed();

    if (fecha.isValid() && fecha > QDate::currentDate()) {
        accept();
    } else {
        QMessageBox::information(this, "ERROR", "La fecha no es correcta");
        return;
    }
}
