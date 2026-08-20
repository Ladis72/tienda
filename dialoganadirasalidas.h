#ifndef DIALOGANADIRASALIDAS_H
#define DIALOGANADIRASALIDAS_H

#include <QDialog>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class DialogAnadirASalidas;
}

/**
 * @brief Diálogo para añadir un artículo a la lista temporal de salidas de mercancía / traspasos.
 *
 * Permite especificar la tienda de destino, cantidad a traspasar, fecha de caducidad/lote y precio,
 * utilizando la fecha actual del sistema para la salida.
 */
class DialogAnadirASalidas : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAnadirASalidas(const QString &codArticulo, QWidget *parent = nullptr);
    ~DialogAnadirASalidas();

private slots:
    void on_comboBoxLotes_currentIndexChanged(int index);
    void on_pushButtonAceptar_clicked();

private:
    Ui::DialogAnadirASalidas *ui;
    QString m_codArticulo;
    baseDatos base;

    void cargarDatosArticulo();
    void llenarComboTiendas();
    void llenarComboLotes();
};

#endif // DIALOGANADIRASALIDAS_H
