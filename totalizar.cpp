#include "totalizar.h"
#include "ui_totalizar.h"

totalizar::totalizar(QString datos, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::totalizar)
{
    ui->setupUi(this);
    ui->lineEditImporte->setText(datos);
    total = ui->lineEditImporte->text().toFloat()*(100-ui->lineEditDescuento->text().toFloat())/100;
    ui->lineEditTotal->setText(QString::number(total));
    descuento = 0;
    fpago = base->fpago(QSqlDatabase::database("DB"));

    while (fpago.next()) {
        ui->comboBox->insertItem(fpago.value(0).toInt(),fpago.value(1).toString());
        ui->comboBox->update();
        }
    efectivo = ui->comboBox->currentText();
    facturacion = "0";

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
    total = ui->lineEditImporte->text().toFloat()*(100-arg1.toFloat())/100;
    ui->lineEditTotal->setText(QString::number(total));
    descuento = ui->lineEditDescuento->text().toFloat();
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

void totalizar::on_comboBox_currentIndexChanged(int index)
{
    qDebug() << index;
}

void totalizar::keyPressEvent(QKeyEvent *e)
{


    switch ( e->key()) {
    case Qt::Key_F1:
        facturacion = "0";
        qDebug() << "F1 pulsada";
        this->setStyleSheet("background-color:#DEDEDE;");
        break;
    case Qt::Key_F2:
        facturacion = "1";
        qDebug() << "F2 pulsada";
        this->setStyleSheet("background-color:#D1AEAB;");
    default:
        qDebug() << "Otra tecla pulsada";
        break;
    }
}
