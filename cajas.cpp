#include "cajas.h"
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include "ui_cajas.h"

Cajas::Cajas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Cajas)
{
    ui->setupUi(this);
    ui->label_21->hide();
    ui->label_ventasB->hide();
    recuperarDatosUltimoArqueo();
    ventas();
    ES();
    actualizarEfectivo();
}

Cajas::~Cajas()
{
    delete ui;
}

void Cajas::on_spinBox20_valueChanged(int arg1)
{
    ui->label20->setText(QString::number(20 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox500_valueChanged(int arg1)
{
    ui->label500->setText(QString::number(500 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox200_valueChanged(int arg1)
{
    ui->label200->setText(QString::number(200 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox100_valueChanged(int arg1)
{
    ui->label100->setText(QString::number(100 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox50_valueChanged(int arg1)
{
    ui->label50->setText(QString::number(50 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox10_valueChanged(int arg1)
{
    ui->label10->setText(QString::number(10 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox5_valueChanged(int arg1)
{
    ui->label5->setText(QString::number(5 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox2_valueChanged(int arg1)
{
    ui->label2->setText(QString::number(2 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox1_valueChanged(int arg1)
{
    ui->label1->setText(QString::number(arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox50c_valueChanged(int arg1)
{
    ui->label50c->setText(QString::number(0.5 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox20c_valueChanged(int arg1)
{
    ui->label20c->setText(QString::number(0.2 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox10c_valueChanged(int arg1)
{
    ui->label10c->setText(QString::number(0.10 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox5c_valueChanged(int arg1)
{
    ui->label5c->setText(QString::number(0.05 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox2c_valueChanged(int arg1)
{
    ui->label2c->setText(QString::number(0.02 * arg1));
    actualizarEfectivo();
}

void Cajas::on_spinBox1c_valueChanged(int arg1)
{
    ui->label1c->setText(QString::number(0.01 * arg1));
    actualizarEfectivo();
}

void Cajas::on_doubleSpinBoxCambios_valueChanged(double arg1)
{
    actualizarEfectivo();
}

void Cajas::on_doubleSpinBoxCajaF_valueChanged(const QString &arg1)
{
    actualizarEfectivo();
}

void Cajas::actualizarEfectivo()
{
    efectivoReal = ui->label500->text().toDouble() + ui->label200->text().toDouble()
                   + ui->label100->text().toDouble() + ui->label50->text().toDouble()
                   + ui->label20->text().toDouble() + ui->label10->text().toDouble()
                   + ui->label5->text().toDouble() + ui->label2->text().toDouble()
                   + ui->label1->text().toDouble() + ui->label50c->text().toDouble()
                   + ui->label20c->text().toDouble() + ui->label10c->text().toDouble()
                   + ui->label5c->text().toDouble() + ui->label2c->text().toDouble()
                   + ui->label1c->text().toDouble() + ui->doubleSpinBoxCambios->value()
                   + ui->doubleSpinBoxCajaF->value();
    ui->labelEfectivoR->setText(QString::number(efectivoReal));
    totalEfectivo = saldoAnterior + ventasEfectivo + salidas + ventasB;
    ui->labelEfectivoTotal->setText(QString::number(totalEfectivo));
    descuadre = efectivoReal - totalEfectivo;
    if (descuadre < 0) {
        ui->labelDescuadre->setStyleSheet("color:red");
    } else {
        ui->labelDescuadre->setStyleSheet("color:green");
    }
    ui->labelDescuadre->setText(QString::number(descuadre));
}

bool Cajas::recuperarDatosUltimoArqueo()
{
    QSqlQuery resultado = base->recuperarDatosUltimoArqueo(conf->getConexionLocal());
    resultado.last();
    fechaUltimoArqueo = resultado.value("fecha").toString();
    horaUltimoArqueo = resultado.value("hora").toString();
    saldoAnterior = resultado.value("efectivoReal").toDouble();
    ui->labelFUArqueo->setText(fechaUltimoArqueo + " " + horaUltimoArqueo);
    ui->labelSaldoAnterior->setText(QString::number(saldoAnterior));
    return true;
}

void Cajas::ventas()
{
    QSqlQuery resultado = base->ventasDesdeUltimoArqueo(fechaUltimoArqueo,
                                                        horaUltimoArqueo,
                                                        "tickets",
                                                        conf->getConexionLocal());
    resultado.first();
    ventasEfectivo = 0;
    if (resultado.value("fpago") == 1) {
        ventasEfectivo = resultado.value(0).toDouble();
        resultado.next();
    }

    ventasTarjeta = resultado.value(0).toDouble();
    ui->label_ventasTarjeta->setText(QString::number(ventasTarjeta));
    nTarjetas = base->nTarjetasDesdeUltimoArqueo(fechaUltimoArqueo,
                                                 horaUltimoArqueo,
                                                 conf->getConexionLocal());
    ui->labelNumeroTarjetas->setText(QString::number(nTarjetas));
    resultado = base->ventasDesdeUltimoArqueo(fechaUltimoArqueo,
                                              horaUltimoArqueo,
                                              "ticketss",
                                              conf->getConexionLocal());
    resultado.first();
    ventasB = resultado.value(0).toDouble();
    ui->label_ventasB->setText(QString::number(ventasB));
    //ventasEfectivo += resultado.value(0).toDouble();
    ui->label_ventasEfectivo->setText(QString::number(ventasEfectivo));
    ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ventasTarjeta));
}

void Cajas::ES()
{
    salidas = base->ESdesdeFecha(fechaUltimoArqueo, horaUltimoArqueo, conf->getConexionLocal());
    ui->labelEntradas->setText(QString::number(salidas));
    if (salidas < 0) {
        ui->labelEntradas->setStyleSheet("color:red");
    } else {
        ui->labelEntradas->setStyleSheet("color:blue");
    }
}

void Cajas::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
    case Qt::Key_F2:
        ui->label_21->show();
        ui->label_ventasB->show();
        ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ventasTarjeta + ventasB));
        break;
    default:
        ui->label_21->hide();
        ui->label_ventasB->hide();
        ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ventasTarjeta));

        break;
    }
}

void Cajas::on_pushButtonRetirarDinero_clicked()
{
    entSal = new EntradaSalida;
    entSal->exec();
    ES();
    actualizarEfectivo();
}

void Cajas::on_pushButtonAceptar_clicked()
{
    actualizarEfectivo();
    QStringList datos;
    QMessageBox *MsgBox = new QMessageBox;
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(QTime::currentTime().toString("hh:mm:ss"));
    datos.append(QString::number(ventasEfectivo));
    datos.append(QString::number(ventasTarjeta));
    datos.append(QString::number(salidas));
    datos.append(QString::number(totalEfectivo));
    datos.append(QString::number(descuadre));
    MsgBox->setText("Confirmación");
    MsgBox->setInformativeText("¿Quiere guardar el arqueo?.\nEsta operación no se puede deshacer");
    MsgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    MsgBox->setDefaultButton(QMessageBox::Cancel);
    int resp = MsgBox->exec();
    switch (resp) {
    case QMessageBox::Save:
        if (!base->grabarArqueo(datos, conf->getConexionLocal())) {
            MsgBox->setText("ERROR");
            MsgBox->setInformativeText("No se ha podido grabar la información.");

            break;
        default:
            break;
        }
    }
    close();
}
