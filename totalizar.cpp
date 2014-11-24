#include "totalizar.h"
#include "ui_totalizar.h"

totalizar::totalizar(QStringList datos, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::totalizar)
{
    ui->setupUi(this);
    ui->lineEditImporte->setText(datos.at(0));
    float total = ui->lineEditImporte->text().toFloat()*(100-ui->lineEditDescuento->text().toFloat())/100;
    ui->lineEditTotal->setText(QString::number(total));

    fpago = base->fpago(QSqlDatabase::database("DB"));

    while (fpago.next()) {
        ui->comboBox->insertItem(fpago.value(0).toInt(),fpago.value(1).toString());
        ui->comboBox->update();
        }
    efectivo = ui->comboBox->currentText();


}

totalizar::~totalizar()
{
    delete ui;
}

void totalizar::on_pushButtonTicket_clicked()
{
    emit accept();
}

void totalizar::on_lineEditDescuento_textChanged(const QString &arg1)
{
    float total = ui->lineEditImporte->text().toFloat()*(100-arg1.toFloat())/100;
    ui->lineEditTotal->setText(QString::number(total));
}

void totalizar::on_lineEditEntrega_textChanged(const QString &arg1)
{
    float vuelta = arg1.toFloat()-ui->lineEditTotal->text().toFloat();
    ui->lineEditCambio->setText(QString::number(vuelta));

}


void totalizar::on_comboBox_currentTextChanged(const QString &arg1)
{
    efectivo = arg1;
    qDebug() << efectivo;
}
