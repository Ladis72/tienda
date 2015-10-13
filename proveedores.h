#ifndef PROVEEDORES_H
#define PROVEEDORES_H
#include "base_datos.h"
#include "buscarproveedor.h"
#include "formaspago.h"

#include <QDialog>

namespace Ui {
class Proveedores;
}

class Proveedores : public QDialog
{
    Q_OBJECT

public:
    explicit Proveedores(QWidget *parent = 0);
    ~Proveedores();

private slots:
    void on_pushButtonNuevo_clicked();

    void on_pushButtonAnterior_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_lineEditNombre_returnPressed();

    void on_pushButtonModificar_clicked();

    void on_lineEditFormaPago_textChanged(const QString &arg1);

    void on_pushButtonFPago_clicked();

private:
    Ui::Proveedores *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;
    BuscarProveedor *proveedor;
    FormasPago *fpago;

    void recargarTabla();
    void borrarFormulario();
    bool eventFilter(QObject *obj, QEvent *event);
    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();
};

#endif // PROVEEDORES_H
