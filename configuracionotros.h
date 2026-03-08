#ifndef CONFIGURACIONOTROS_H
#define CONFIGURACIONOTROS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class ConfiguracionOtros;
}

class ConfiguracionOtros : public QDialog
{
    Q_OBJECT

public:
    explicit ConfiguracionOtros(QWidget *parent = 0);
    ~ConfiguracionOtros();

private slots:
    void on_pushButtonAceptar_clicked();

private:
    Ui::ConfiguracionOtros *ui;
    baseDatos *base;
};

#endif // CONFIGURACIONOTROS_H
