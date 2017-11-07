#include "prestamos.h"
#include "ui_prestamos.h"

Prestamos::Prestamos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prestamos)
{
    ui->setupUi(this);
}

Prestamos::~Prestamos()
{
    delete ui;
}
