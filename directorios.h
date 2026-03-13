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

    void on_toolButtonCseg_clicked();

    void on_toolButtonLogo_clicked();

    void on_toolButtonLogoFactura_clicked();

    void on_toolButton_clicked();

private:
    Ui::Directorios *ui;
    baseDatos *base;
    QStringList listaDatos;

    void llenarListaBase(QStringList lista);
    void cargarListaLineEdit();
    void cargalListaBase();
    QString rutaRelativa(QString directorio);
};

#endif // DIRECTORIOS_H
