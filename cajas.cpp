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
    aplicarPermisos();
}

Cajas::~Cajas()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones del módulo de Cajas.
 */
void Cajas::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonAceptar->setEnabled(conf->permisos()->tiene("cajas.cerrar"));
    ui->pushButtonRetirarDinero->setEnabled(conf->permisos()->tiene("cajas.retirar"));
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

void Cajas::on_doubleSpinBoxCambios_valueChanged(double)
{
    actualizarEfectivo();
}

void Cajas::on_doubleSpinBoxCajaF_textChanged(const QString &)
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
        ui->labelDescuadre->setStyleSheet("color: #ef5350; font-weight: bold;");
    } else {
        ui->labelDescuadre->setStyleSheet("color: #7cb342; font-weight: bold;");
    }
    ui->labelDescuadre->setText(QString::number(descuadre));
}

bool Cajas::recuperarDatosUltimoArqueo()
{
    QSqlQuery resultado = base->recuperarDatosUltimoArqueo(conf->getConexionLocal());
    resultado.first();

    // En Qt 6.11 + MariaDB 12.x, el campo DATE llega como QDate.
    // toString() sin formato usa el locale del sistema (p.ej. "27/06/2026" en español)
    // lo que rompe la consulta SQL posterior que espera "yyyy-MM-dd".
    QVariant fechaVar = resultado.value("fecha");
    fechaUltimoArqueo = fechaVar.toDate().isValid()
                            ? fechaVar.toDate().toString("yyyy-MM-dd")
                            : fechaVar.toString();

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

    // Sumar los anticipos de encargos en tarjeta/no-efectivo recibidos desde el último arqueo
    double anticiposTarjeta = base->anticiposNoEfectivoDesdeUltimoArqueo(fechaUltimoArqueo,
                                                                          horaUltimoArqueo,
                                                                          conf->getConexionLocal());
    ventasTarjeta += anticiposTarjeta;

    ui->labelVentasTarjeta->setText(QString::number(ventasTarjeta, 'f', 2));
    ui->lineEditVentasTarjeta->setText(QString::number(ventasTarjeta, 'f', 2));

    nTarjetas = base->nTarjetasDesdeUltimoArqueo(fechaUltimoArqueo,
                                                 horaUltimoArqueo,
                                                 conf->getConexionLocal());

    // Sumar el número de operaciones de anticipo por tarjeta/no-efectivo
    int nAnticiposTarjeta = base->nAnticiposNoEfectivoDesdeUltimoArqueo(fechaUltimoArqueo,
                                                                         horaUltimoArqueo,
                                                                         conf->getConexionLocal());
    nTarjetas += nAnticiposTarjeta;

    ui->labelNumeroTarjetas->setText(QString::number(nTarjetas));
    resultado = base->ventasDesdeUltimoArqueo(fechaUltimoArqueo,
                                              horaUltimoArqueo,
                                              "ticketss",
                                              conf->getConexionLocal());
    resultado.first();
    ventasB = resultado.value(0).toDouble();
    ui->label_ventasB->setText(QString::number(ventasB));
    ui->label_ventasB->setVisible(false); // Mantener oculto por defecto como en el original

    //ventasEfectivo += resultado.value(0).toDouble();
    ui->label_ventasEfectivo->setText(QString::number(ventasEfectivo));
    // El total de ventas usa el valor MANUAL/APUNTADO de tarjeta para detectar descuadres si se desea
    ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ui->lineEditVentasTarjeta->text().toDouble(), 'f', 2));
}

void Cajas::ES()
{
    salidas = base->ESdesdeFecha(fechaUltimoArqueo, horaUltimoArqueo, conf->getConexionLocal());
    ui->labelEntradas->setText(QString::number(salidas));
    if (salidas < 0) {
        ui->labelEntradas->setStyleSheet("color: #ef5350; font-weight: bold;");
    } else {
        ui->labelEntradas->setStyleSheet("color: #0288d1; font-weight: bold;");
    }
}

void Cajas::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F2) {
        if (ui->label_21->isVisible()) {
            ui->label_21->hide();
            ui->label_ventasB->hide();
            ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ui->lineEditVentasTarjeta->text().toDouble(), 'f', 2));
        } else {
            ui->label_21->show();
            ui->label_ventasB->show();
            ui->label_ventasTotales->setText(QString::number(ventasEfectivo + ui->lineEditVentasTarjeta->text().toDouble() + ventasB, 'f', 2));
        }
    } else {
        QDialog::keyPressEvent(e);
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
    datos.append(QString::number(ui->lineEditVentasTarjeta->text().toDouble(), 'f', 2));
    datos.append(QString::number(salidas));
    datos.append(QString::number(totalEfectivo));
    datos.append(QString::number(descuadre));
    datos.append(QString::number(efectivoReal));     // efectivoContado
    datos.append(conf->getUsuario());                 // usuario

    // Construir desglose de denominaciones
    QMap<double, int> desglose;
    desglose.insert(500, ui->spinBox500->value());
    desglose.insert(200, ui->spinBox200->value());
    desglose.insert(100, ui->spinBox100->value());
    desglose.insert(50, ui->spinBox50->value());
    desglose.insert(20, ui->spinBox20->value());
    desglose.insert(10, ui->spinBox10->value());
    desglose.insert(5, ui->spinBox5->value());
    desglose.insert(2, ui->spinBox2->value());
    desglose.insert(1, ui->spinBox1->value());
    desglose.insert(0.50, ui->spinBox50c->value());
    desglose.insert(0.20, ui->spinBox20c->value());
    desglose.insert(0.10, ui->spinBox10c->value());
    desglose.insert(0.05, ui->spinBox5c->value());
    desglose.insert(0.02, ui->spinBox2c->value());
    desglose.insert(0.01, ui->spinBox1c->value());

    MsgBox->setText("Confirmación");
    MsgBox->setInformativeText("¿Quiere guardar el arqueo?.\nEsta operación no se puede deshacer");
    MsgBox->setStandardButtons(QMessageBox::Save | QMessageBox::Cancel);
    MsgBox->setDefaultButton(QMessageBox::Cancel);
    int resp = MsgBox->exec();
    switch (resp) {
    case QMessageBox::Save:
        if (!base->grabarArqueo(datos, conf->getConexionLocal(), desglose)) {
            MsgBox->setText("ERROR");
            MsgBox->setInformativeText("No se ha podido grabar la información.");
            MsgBox->exec();
        }
        break;
    default:
        break;
    }
    close();
}

void Cajas::on_pushButtonCerrar_clicked()
{
    reject();
}

void Cajas::on_lineEditVentasTarjeta_textChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    double card = ui->lineEditVentasTarjeta->text().toDouble();
    if (ui->label_21->isVisible()) {
        ui->label_ventasTotales->setText(QString::number(ventasEfectivo + card + ventasB, 'f', 2));
    } else {
        ui->label_ventasTotales->setText(QString::number(ventasEfectivo + card, 'f', 2));
    }
}
