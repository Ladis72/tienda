#ifndef PEDIDONUEVO_H
#define PEDIDONUEVO_H

#include <QDialog>
#include "base_datos.h"
#include "buscarproveedor.h"
#include "configuracion.h"

extern Configuracion *conf;


namespace Ui {
class PedidoNuevo;
}

class PedidoNuevo : public QDialog
{
    Q_OBJECT

public:
    explicit PedidoNuevo(QWidget *parent = 0);
    ~PedidoNuevo();

private slots:
    void on_lineEditIdProveedor_editingFinished();

    void on_lineEditProveedor_returnPressed();

    void on_pushButtonAceptar_clicked();

private:
    Ui::PedidoNuevo *ui;
    baseDatos *base;
    BuscarProveedor *buscProveedor;
};

#endif // PEDIDONUEVO_H
