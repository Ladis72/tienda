#include "cambiodatosproducto.h"
#include "ui_cambiodatosproducto.h"
#include <QMessageBox>
#include <QDebug>
CambioDatosProducto::CambioDatosProducto(QString nombrePedido, QString nombreAlmacen,
                                                     QString pvpPedido, QString pvpAlmacen, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CambioDatosProducto)
{
    ui->setupUi(this);
    ui->radioButtonNombrePedido->setText(nombrePedido);
    ui->radioButtonNombreAlmacen->setText(nombreAlmacen);
    ui->radioButtonPVPPedido->setText(pvpPedido);
    ui->radioButtonPVPAlmacen->setText(pvpAlmacen);
    nombre=ui->radioButtonNombrePedido->text();
    pvp=ui->radioButtonPVPPedido->text();
    ui->lineEditNombrePersonal->setText(nombre);
    ui->lineEditPVPPersonal->setText(pvp);
    //datosAlmacen = ui->radioButtonDatosAlmacen->isChecked();
    //datosPedido = ui->radioButtonDatosPedido->isChecked();
    tipoActualizacion = 0;

}

CambioDatosProducto::~CambioDatosProducto()
{
    delete ui;
}

void CambioDatosProducto::on_radioButtonNombreAlmacen_clicked()
{
    nombre = ui->radioButtonNombreAlmacen->text();
}

void CambioDatosProducto::on_radioButtonNombrePedido_clicked()
{
    nombre = ui->radioButtonNombrePedido->text();
}

void CambioDatosProducto::on_radioButtonNombrePersonal_clicked()
{
    nombre = ui->lineEditNombrePersonal->text();
}

void CambioDatosProducto::on_radioButtonPVPPedido_clicked()
{
    pvp = ui->radioButtonPVPPedido->text();
}

void CambioDatosProducto::on_radioButtonPVPAlmacen_clicked()
{
    pvp = ui->radioButtonPVPAlmacen->text();
}

void CambioDatosProducto::on_radioButtonPVPPersonal_clicked()
{
    pvp = ui->radioButtonPVPPersonal->text();
}

void CambioDatosProducto::on_lineEditNombrePersonal_editingFinished()
{
    if (ui->lineEditNombrePersonal->text().isEmpty() && ui->radioButtonNombrePersonal->isChecked()) {
        QMessageBox mensaje;
        mensaje.setText("Corrija los datos");
        mensaje.setInformativeText("El campo nombre personalizado no puede estar vacio si elige esta opción");
        mensaje.exec();
        ui->radioButtonNombrePedido->setChecked(true);
        emit on_radioButtonNombrePedido_clicked();
    }else{
        nombre = ui->lineEditNombrePersonal->text();
    }
    qDebug() << nombre;


}

void CambioDatosProducto::on_lineEditPVPPersonal_editingFinished()
{
    if (ui->lineEditPVPPersonal->text().isEmpty() && ui->radioButtonPVPPersonal->isChecked()) {
        QMessageBox mensaje;
        mensaje.setText("Corrija los datos");
        mensaje.setInformativeText("El campo nombre personalizado no puede estar vacio si elige esta opción");
        mensaje.exec();
        ui->radioButtonPVPPedido->setChecked(true);
        emit on_radioButtonPVPPedido_clicked();
    } else {
        pvp = ui->lineEditPVPPersonal->text();
    }
}

void CambioDatosProducto::on_radioButtonDatosPedido_clicked()
{
    tipoActualizacion = 1;
}

void CambioDatosProducto::on_radioButtonDatosAlmacen_clicked()
{
    tipoActualizacion = 2;

}

void CambioDatosProducto::on_radioButton_clicked()
{
    tipoActualizacion = 0;

}
