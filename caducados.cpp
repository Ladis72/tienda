#include "caducados.h"
#include "ui_caducados.h"

Caducados::Caducados(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caducados)
{
    ui->setupUi(this);
}

Caducados::~Caducados()
{
    delete ui;
}
