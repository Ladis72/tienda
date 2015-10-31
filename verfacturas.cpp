#include "verfacturas.h"
#include "ui_verfacturas.h"

VerFacturas::VerFacturas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerFacturas)
{
    ui->setupUi(this);
    llenarProveedores();
}

VerFacturas::~VerFacturas()
{
    delete ui;
}

void VerFacturas::llenarProveedores()
{
    ui->comboBoxProceedores->addItems(base.listadoProveedores());
}

void VerFacturas::on_checkBoxTodosProveedores_clicked(bool checked)
{
    if (checked == false) {
        ui->comboBoxProceedores->setEnabled(true);
    } else {ui->comboBoxProceedores->setDisabled(true);
    }
}
