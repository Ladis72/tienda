#include "totalizar.h"
#include "ui_totalizar.h"

totalizar::totalizar(QString datos, double vale, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::totalizar)
{
    ui->setupUi(this);
    valeUsado = false;
    ui->lineEditImporte->setText(datos);
    total = ui->lineEditImporte->text().toDouble()*(100-ui->lineEditDescuento->text().toDouble())/100;
    ui->lineEditTotal->setText(QString::number(total));
    descuento = 0;
    fpago = base->fpago(QSqlDatabase::database("DB"));

    while (fpago.next()) {
        ui->comboBox->insertItem(fpago.value(0).toInt(),fpago.value(1).toString());
        ui->comboBox->update();
        }
    efectivo = ui->comboBox->currentText();
    facturacion = "0";
    ticket = false;
    factura = false;
    entrega = 0;
    cambio = 0;
    cantVale = vale;
    qDebug() << cantVale;
    if (cantVale > 0) {
        ui->checkBoxVale->setChecked(true);
        ui->lineEditVale->setText(QString::number(cantVale));
    }
}

totalizar::~totalizar()
{
    delete ui;
}

void totalizar::on_pushButtonTicket_clicked()
{
    ticket = true;
    factura = false;
    emit accept();
}

void totalizar::on_lineEditDescuento_textChanged(const QString &arg1)
{
    total = ui->lineEditImporte->text().toDouble()*(100-arg1.toDouble())/100;
    ui->lineEditTotal->setText(QString::number(total));
    descuento = ui->lineEditDescuento->text().toDouble();
    emit on_lineEditEntrega_textChanged(ui->lineEditEntrega->text());
}

void totalizar::on_lineEditEntrega_textChanged(const QString &arg1)
{
    double vuelta = arg1.toDouble()-ui->lineEditTotal->text().toDouble();
    ui->lineEditCambio->setText(QString::number(vuelta));
    entrega = ui->lineEditEntrega->text().toDouble();
    cambio = vuelta;
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
        emit on_pushButtonCobrar_clicked();
        break;
    case Qt::Key_F8:
        emit on_pushButtonCobrar_clicked();
        break;
    case Qt::Key_F9:
        emit on_pushButtonTicket_clicked();
        break;
    default:
        qDebug() << "Otra tecla pulsada";
        break;
    }
}

void totalizar::on_pushButtonCobrar_clicked()
{
    ticket = false;
    factura =false;
    emit accept();
}

void totalizar::on_pushButtonFactura_clicked()
{
    ticket = true;
    factura = true;
    emit accept();
}

void totalizar::on_checkBoxVale_stateChanged(int arg1)
{
    if (arg1 > 0) {
        total = ui->lineEditImporte->text().toDouble()*(100-ui->lineEditDescuento->text().toDouble())/100-cantVale;
        ui->lineEditTotal->setText(QString::number(total));
        valeUsado = true;
    }else{
        total = ui->lineEditImporte->text().toDouble()*(100-ui->lineEditDescuento->text().toDouble())/100;
        ui->lineEditTotal->setText(QString::number(total));
        valeUsado = false;
    }
    qDebug() << arg1;
}
