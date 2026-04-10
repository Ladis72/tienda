#ifndef CONFIGBASE_H
#define CONFIGBASE_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class ConfigBase;
}

/**
 * @brief Diálogo para configurar la conexión al servidor MariaDB en la nube.
 *
 * Lee y guarda los datos en la tabla `config_nube` de la BD local.
 * Permite probar la conexión antes de guardar.
 */
class ConfigBase : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigBase(QWidget *parent = nullptr);
    ~ConfigBase();

private slots:
    /// Guarda los datos en la BD local y cierra el diálogo
    void on_pushButton_clicked();

    /// Prueba la conexión con los datos introducidos sin guardar
    void on_pushButtonTest_clicked();

    /// Abre un selector de archivo para elegir el certificado CA y guarda ruta relativa
    void on_pushButtonBuscarCa_clicked();

private:
    Ui::ConfigBase *ui;
    baseDatos base;

    /// Carga en el formulario los datos guardados en config_nube
    void cargarDatos();

    /// Asegura que la tabla config_nube existe en la BD local
    void crearTablaConfigNube();
};

#endif // CONFIGBASE_H
