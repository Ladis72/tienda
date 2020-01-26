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

private:
    Ui::GenerarVales *ui;
    baseDatos base;
    QStringList tiendas;
    QString master , fechaI , fechaF;
};

#endif // GENERARVALES_H
