#include "tienda.h"
#include "ui_tienda.h"

#include <QDebug>
#include <QMessageBox>


Tienda::Tienda(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tienda)
{
    ui->setupUi(this);


}

Tienda::~Tienda()
{
    delete ui;
}

void Tienda::on_ventasButton_clicked()
{



    T = new Tpv();
    connect(T, SIGNAL(cerrar_tpv()), this, SLOT(activar_btn_tpv()));

    T->show();

    return;

}
void Tienda::activar_btn_tpv()
{

}

void Tienda::on_pushButtonUsuarios_clicked()
{
    U = new Ususarios();

    U->show();
}

void Tienda::on_pushButton_clicked()
{
    A = new Articulos(this);
    A->show();
}

void Tienda::on_pushButtonFamilias_clicked()
{
    F = new Familias(this);
    F->show();
}

void Tienda::on_pushButtonFabricantes_clicked()
{
    Fab = new Fabricantes(this);
    Fab->show();
}

void Tienda::on_pushButtonFormasPago_clicked()
{
    FPago = new FormasPago(this);
    FPago->show();
}

void Tienda::on_pushButton_3_clicked()
{
    Cli = new Clientes(this);
    Cli->show();
}
