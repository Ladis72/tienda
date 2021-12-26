#include "configticket.h"
#include "ui_configticket.h"
#include <QPrintDialog>
#include <QPrinter>

ConfigTicket::ConfigTicket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConfigTicket)
{
    ui->setupUi(this);
    QStringList configuracionTicket = base->recuperarConfigTicket();
    ui->plainTextEditCabecera->setPlainText(configuracionTicket.at(0));
    ui->plainTextEditPie->setPlainText(configuracionTicket.at(1));
    ui->plainTextEditPromo->setPlainText(configuracionTicket.at(2));
    if (base->ticketPromo()) {
        ui->checkBoxPromo->setChecked(true);
        ui->plainTextEditPromo->setEnabled(true);
    }else{
        ui->checkBoxPromo->setChecked(false);
        ui->plainTextEditPromo->setEnabled(false);
    }
    ui->lineEditCola->setText(configuracionTicket.at(3));
    ui->lineEditCodApertura->setText(configuracionTicket.at(4));
    ui->lineEditCodCorte->setText(configuracionTicket.at(5));

}

ConfigTicket::~ConfigTicket()
{
    delete ui;
}

void ConfigTicket::on_pushButtonCancelar_clicked()
{
    close();
}

void ConfigTicket::on_pushButtonAceptar_clicked()
{
    QStringList configuracionTicket;
    configuracionTicket.append(ui->plainTextEditCabecera->toPlainText());
    configuracionTicket.append(ui->plainTextEditPie->toPlainText());
    configuracionTicket.append(ui->plainTextEditPromo->toPlainText());
    if (ui->checkBoxPromo->isChecked()) {
        configuracionTicket.append("1");
    } else {configuracionTicket.append("0");
    }
    configuracionTicket.append(ui->lineEditCola->text());
    configuracionTicket.append(ui->lineEditCodApertura->text());
    configuracionTicket.append(ui->lineEditCodCorte->text());

    base->grabarConfiguracionTicket(configuracionTicket);
}

void ConfigTicket::on_checkBoxPromo_toggled(bool checked)
{
    ui->plainTextEditPromo->setEnabled(checked);
}

void ConfigTicket::on_pushButton_clicked()
{
    QPrinter impresora;
    QPrintDialog dlgImpresora(&impresora,this);
    if (!dlgImpresora.exec()) {
        return;
    }
    ui->lineEditCola->setText(impresora.printerName());
}

