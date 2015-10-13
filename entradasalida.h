#ifndef ENTRADASALIDA_H
#define ENTRADASALIDA_H

#include <QDialog>
#include "base_datos.h"
#include "tiposentradassalidas.h"

namespace Ui {
class EntradaSalida;
}

class EntradaSalida : public QDialog
{
    Q_OBJECT

public:
    explicit EntradaSalida(QWidget *parent = 0);
    ~EntradaSalida();

private slots:
    void on_pushButtonAceptar_clicked();

    void on_toolButton_clicked();

private:
    Ui::EntradaSalida *ui;
    baseDatos *base;
    QSqlQuery consulta;
    TiposEntradasSalidas *anadirTipo;

    void llenarCombo();
};

#endif // ENTRADASALIDA_H
