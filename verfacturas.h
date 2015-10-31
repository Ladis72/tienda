#ifndef VERFACTURAS_H
#define VERFACTURAS_H

#include <QDialog>
#include "base_datos.h"


namespace Ui {
class VerFacturas;
}

class VerFacturas : public QDialog
{
    Q_OBJECT

public:
    explicit VerFacturas(QWidget *parent = 0);
    ~VerFacturas();

private slots:
    void on_checkBoxTodosProveedores_clicked(bool checked);

private:
    void llenarProveedores();


    Ui::VerFacturas *ui;
    baseDatos base;
    int idProveedor;
    //QDate fechaInicial, fechaFinal;
};

#endif // VERFACTURAS_H
