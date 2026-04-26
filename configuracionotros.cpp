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

    // Poblar combos de vendedores
    QSqlQuery q = base->usuarios(QSqlDatabase::database("DB"));
    ui->comboBoxF1->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF2->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF3->addItem(tr("Ninguno"), QVariant());
    ui->comboBoxF4->addItem(tr("Ninguno"), QVariant());
    
    while (q.next()) {
        QString id = q.value("id").toString();
        QString nombre = q.value("nombre").toString();
        QString itemText = id + "-" + nombre;
        ui->comboBoxF1->addItem(itemText, id);
        ui->comboBoxF2->addItem(itemText, id);
        ui->comboBoxF3->addItem(itemText, id);
        ui->comboBoxF4->addItem(itemText, id);
    }

    // Cargar valores actuales
    ui->comboBoxF1->setCurrentIndex(ui->comboBoxF1->findData(config.value("vendedor_f1").toString()));
    ui->comboBoxF2->setCurrentIndex(ui->comboBoxF2->findData(config.value("vendedor_f2").toString()));
    ui->comboBoxF3->setCurrentIndex(ui->comboBoxF3->findData(config.value("vendedor_f3").toString()));
    ui->comboBoxF4->setCurrentIndex(ui->comboBoxF4->findData(config.value("vendedor_f4").toString()));
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

    config["vendedor_f1"] = ui->comboBoxF1->currentData();
    config["vendedor_f2"] = ui->comboBoxF2->currentData();
    config["vendedor_f3"] = ui->comboBoxF3->currentData();
    config["vendedor_f4"] = ui->comboBoxF4->currentData();
    
    base->GuardarConfiguracion(config);
    conf->setUsarPreciosLocales(ui->checkBoxNube->isChecked());
    emit accept();
}
