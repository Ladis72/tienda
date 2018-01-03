#include "configuracionotros.h"
#include "ui_configuracionotros.h"

ConfiguracionOtros::ConfiguracionOtros(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfiguracionOtros)
{
    ui->setupUi(this);
    if (base->leerConfiguracion() == "1") {
        ui->checkBoxRE->setChecked(true);
    }else {
        ui->checkBoxRE->setChecked(false);
    }
}

ConfiguracionOtros::~ConfiguracionOtros()
{
    delete ui;
}

void ConfiguracionOtros::on_pushButtonAceptar_clicked()
{
    if (ui->checkBoxRE->isChecked()) {
        base->GuardarConfiguracion(1);
    } else {base->GuardarConfiguracion(0);
    }
    emit accept();
}
