#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QDialog>
#include "base_datos.h"


namespace Ui {
class ConfigBase;
}

class ConfigBase : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigBase(QWidget *parent = 0);
    ~ConfigBase();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ConfigBase *ui;
    baseDatos *base;
    QString host;
    QString puerto;
    QString nombreBaseDatos;
    QString usuario;
    QString clave;

    void actualizarDatos();
};

#endif // CONFIGBASE_H
