#ifndef ACTUALIZARCLIENTES_H
#define ACTUALIZARCLIENTES_H

#include <QDialog>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class ActualizarClientes;
}

class ActualizarClientes : public QDialog
{
    Q_OBJECT

public:
    explicit ActualizarClientes(QWidget *parent = nullptr);
    ~ActualizarClientes();

private slots:
    void on_pushButtonActualizar_clicked();

private:
    Ui::ActualizarClientes *ui;

    baseDatos base;
    QStringList conexiones;
};

#endif // ACTUALIZARCLIENTES_H
