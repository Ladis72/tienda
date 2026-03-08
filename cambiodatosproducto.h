#ifndef CAMBIODATOSPRODUCTO_H
#define CAMBIODATOSPRODUCTO_H

#include <QDialog>

namespace Ui {
class CambioDatosProducto;
}

class CambioDatosProducto : public QDialog
{
    Q_OBJECT

public:
    explicit CambioDatosProducto(QString nombrePedido,
                                 QString nombreAlmacen,
                                 QString pvpPedido,
                                 QString pvpAlmacen,
                                 QWidget *parent);

    ~CambioDatosProducto();
    int tipoActualizacion;
    QString nombre, pvp;

private slots:
    void on_radioButtonNombreAlmacen_clicked();

    void on_radioButtonNombrePedido_clicked();

    void on_radioButtonNombrePersonal_clicked();

    void on_radioButtonPVPPedido_clicked();

    void on_radioButtonPVPAlmacen_clicked();

    void on_radioButtonPVPPersonal_clicked();

    void on_lineEditNombrePersonal_editingFinished();

    void on_lineEditPVPPersonal_editingFinished();

    void on_radioButtonDatosPedido_clicked();

    void on_radioButtonDatosAlmacen_clicked();

    void on_radioButton_clicked();

private:
    Ui::CambioDatosProducto *ui;
};

#endif // CAMBIODATOSPRODUCTO_H
