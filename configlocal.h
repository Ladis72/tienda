#ifndef CONFIGLOCAL_H
#define CONFIGLOCAL_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class ConfigLocal;
}

/**
 * @brief Diálogo para configurar la conexión a la base de datos local mediante tienda.ini.
 *
 * Utiliza QSettings para cargar y guardar la dirección, puerto, usuario, base de datos
 * y clave en el archivo local 'tienda.ini'. Cumple con las directrices de seguridad (SEC-01).
 */
class ConfigLocal : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigLocal(QWidget *parent = nullptr);
    ~ConfigLocal();

private slots:
    /// Guarda los datos en el archivo tienda.ini mediante QSettings y cierra el diálogo
    void on_pushButtonGuardar_clicked();

    /// Cierra el diálogo sin guardar ningún cambio
    void on_pushButtonCancelar_clicked();

    /// Prueba la conexión temporal con los datos del formulario antes de guardar
    void on_pushButtonTest_clicked();

private:
    Ui::ConfigLocal *ui;
    baseDatos base;

    /// Carga en el formulario los parámetros de conexión guardados en tienda.ini
    void cargarDatos();
};

#endif // CONFIGLOCAL_H
