#include "configuracionotros.h"
#include "ui_configuracionotros.h"

ConfiguracionOtros::ConfiguracionOtros(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfiguracionOtros)
{
    ui->setupUi(this);
    QMap<QString, QVariant> config = base->leerConfiguracion();
    ui->checkBoxRE->setChecked(config.value("recargoeq").toBool());
    
    if (ui->checkBoxNube) {
        ui->checkBoxNube->setChecked(config.value("precios_locales").toBool());
    }
}

ConfiguracionOtros::~ConfiguracionOtros()
{
    delete ui;
}

void ConfiguracionOtros::on_pushButtonAceptar_clicked()
{
    QMap<QString, QVariant> config;
    config["recargoeq"] = ui->checkBoxRE->isChecked() ? 1 : 0;
    
    if (ui->checkBoxNube) {
        config["precios_locales"] = ui->checkBoxNube->isChecked() ? 1 : 0;
    }
    
    base->GuardarConfiguracion(config);
    conf->setUsarPreciosLocales(ui->checkBoxNube->isChecked());
    emit accept();
}
