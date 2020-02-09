#ifndef GENERARVALES_H
#define GENERARVALES_H

#include <QDialog>

#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class GenerarVales;
}

class GenerarVales : public QDialog
{
    Q_OBJECT

public:
    explicit GenerarVales(QWidget *parent = nullptr);
    ~GenerarVales();

private slots:
    void on_pushButtonGenerar_clicked();

    void on_spinBox_valueChanged(int arg1);

private:
    Ui::GenerarVales *ui;
    baseDatos base;
    QStringList tiendas;
    QString master , fechaI , fechaF;
    int descuento;
};

#endif // GENERARVALES_H
