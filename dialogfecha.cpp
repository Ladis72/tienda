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
    lote = ui->lineEdit->text();
}

void DialogFecha::on_pushButton_clicked()
{
    if (fecha.isValid() && fecha > QDate::currentDate()) {
        close();
    } else {
        QMessageBox::information(this, "ERROR", "La fecha no es correcta");
        return;
    }
}
