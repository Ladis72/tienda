#include "pedidonuevo.h"
#include "ui_pedidonuevo.h"

PedidoNuevo::PedidoNuevo(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PedidoNuevo)
{
    ui->setupUi(this);
    ui->dateEditDocumento->setDate(QDate::currentDate());
}

PedidoNuevo::~PedidoNuevo()
{
    delete ui;
}

void PedidoNuevo::on_lineEditIdProveedor_editingFinished()
{
    QString proveedor = base->nombreProveedor(ui->lineEditIdProveedor->text(),
                                              conf->getConexionLocal());
    if (!proveedor.isEmpty())
        ui->lineEditProveedor->setText(proveedor);
    return;
}

void PedidoNuevo::on_lineEditProveedor_returnPressed()
{
    BuscarProveedor *proveedor = new BuscarProveedor(this, ui->lineEditProveedor->text());
    proveedor->exec();
    ui->lineEditIdProveedor->setText(proveedor->resultado);
    emit on_lineEditIdProveedor_editingFinished();
}

void PedidoNuevo::on_pushButtonAceptar_clicked()
{
    if (ui->lineEditDocumento->text().isEmpty())
        return;
    if (base->nombreProveedor(ui->lineEditIdProveedor->text(), conf->getConexionLocal()).isNull())
        return;
    if (base->crearPedido(ui->lineEditIdProveedor->text(),
                          ui->lineEditDocumento->text(),
                          ui->dateEditDocumento->text(),
                          conf->getConexionLocal()))
        ;
    qDebug() << "todo correcto";
}
