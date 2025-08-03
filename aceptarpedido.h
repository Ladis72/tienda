#ifndef ACEPTARPEDIDO_H
#define ACEPTARPEDIDO_H

#include <QDialog>
#include "base_datos.h"
#include "cambiodatosproducto.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class AceptarPedido;
}

class AceptarPedido : public QDialog
{
    Q_OBJECT

public:
    explicit AceptarPedido(
        QString pedido, QString proveedor, QString nDoc, QString fechaPedido, QWidget *parent = 0);
    ~AceptarPedido();
    bool correcto;

    double descuentoReal;

private slots:

    void on_doubleSpinBox_valueChanged(double arg1);

    void on_pushButton_clicked();

    void on_lineEditNDoc_textChanged(const QString &arg1);

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::AceptarPedido *ui;

    baseDatos base;
    CambioDatosProducto *cdProducto;
    QSqlQuery consulta;
    QString idPedido, fecha, nDoc, idProveedor;
    double descuento;
    void llenarTabla(QString idPedido, double desc);
    QSqlQueryModel *modeloPedido;
    bool procesarPedido(QSqlQueryModel *modelo);
};

#endif // ACEPTARPEDIDO_H
