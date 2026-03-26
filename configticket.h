#ifndef CONFIGTICKET_H
#define CONFIGTICKET_H

#include <QDialog>
#include <QLabel>
#include "base_datos.h"

namespace Ui {
class ConfigTicket;
}

/**
 * @brief Diálogo de configuración del ticket de venta.
 *
 * Permite configurar la cabecera, el pie, la imagen de cabecera,
 * la imagen promocional (impresa al final del ticket si está activada),
 * la ruta de la impresora, y los códigos de apertura/corte.
 */
class ConfigTicket : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigTicket(QWidget *parent = 0);
    ~ConfigTicket();

private slots:
    /// Slot del botón cancelar: cierra el diálogo sin guardar
    void on_pushButtonCancelar_clicked();

    /// Slot del botón guardar: graba la configuración en base de datos
    void on_pushButtonAceptar_clicked();

    /// Slot del checkbox de ticket promocional: habilita/deshabilita los controles de imagen promo
    void on_checkBoxPromo_toggled(bool checked);

    /// Slot del botón "..." para seleccionar la imagen de cabecera del ticket
    void on_toolButtonImgCabecera_clicked();

    /// Slot del botón "..." para seleccionar la imagen promocional del ticket
    void on_toolButtonImgPromo_clicked();

private:
    Ui::ConfigTicket *ui;
    baseDatos *base;

    /**
     * @brief Convierte una ruta absoluta a ruta relativa respecto al ejecutable.
     * @param directorio Ruta absoluta del archivo seleccionado
     * @return Ruta relativa con prefijo "./" si es necesario
     */
    QString rutaRelativa(QString directorio);

    /**
     * @brief Actualiza la vista previa de una imagen en un QLabel.
     * @param ruta Ruta del archivo de imagen
     * @param label QLabel donde se muestra la vista previa
     */
    void actualizarPreview(const QString &ruta, QLabel *label);
};

#endif // CONFIGTICKET_H
