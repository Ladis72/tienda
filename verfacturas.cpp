#include "verfacturas.h"
#include "ui_verfacturas.h"

VerFacturas::VerFacturas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerFacturas)
{
    ui->setupUi(this);
}

VerFacturas::~VerFacturas()
{
    delete ui;
}
