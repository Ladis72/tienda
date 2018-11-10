#ifndef DIRECTORIOS_H
#define DIRECTORIOS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class Directorios;
}

class Directorios : public QDialog
{
    Q_OBJECT

public:
    explicit Directorios(QWidget *parent = nullptr);
    ~Directorios();

private slots:
    void on_pushButtonAceptar_clicked();

    void on_toolButtonFactura_clicked();

    void on_toolButtonVentas_clicked();

    void on_toolButtonEtiquetas_clicked();

    void on_toolButtonCaducados_clicked();

    void on_toolButtonArqueos_clicked();

    void on_toolButtonMovimientos_clicked();

private:
    Ui::Directorios *ui;
    baseDatos *base;
    QStringList listaDatos;

    void llenarListaBase(QStringList lista);
    void cargarListaLineEdit();
    void cargalListaBase();
};

#endif // DIRECTORIOS_H
