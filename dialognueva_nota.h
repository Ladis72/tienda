#ifndef DIALOGNUEVA_NOTA_H
#define DIALOGNUEVA_NOTA_H

#include <QDialog>
#include <QString>
#include <QDate>

namespace Ui {
class DialogNuevaNota;
}

/**
 * @brief Diálogo modal para la creación y edición de notas de la aplicación.
 *
 * Utiliza el archivo .ui diseñado en Qt Creator para mantener una interfaz gráfica
 * coherente con el estilo global de la aplicación y un formateo de fechas estricto ("yyyy-MM-dd").
 */
class DialogNuevaNota : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de DialogNuevaNota.
     * @param parent Widget padre (opcional).
     * @param esEdicion Indica si se trata de la edición de una nota existente (true) o creación (false).
     */
    explicit DialogNuevaNota(QWidget *parent = nullptr, bool esEdicion = false);
    ~DialogNuevaNota();

    /// @brief Configura los datos iniciales de la nota a editar
    void setDatos(const QString &titulo, const QString &descripcion,
                  const QString &fechaLimite, const QString &prioridad);

    /// @brief Obtiene el título ingresado por el usuario
    QString titulo() const;

    /// @brief Obtiene la descripción de la nota
    QString descripcion() const;

    /// @brief Obtiene la fecha límite en formato estricto "yyyy-MM-dd" o cadena vacía si no tiene
    QString fechaLimite() const;

    /// @brief Obtiene el nivel de prioridad seleccionado ("Alta", "Normal", "Baja")
    QString prioridad() const;

private slots:
    /// @brief Gestiona la activación/desactivación del selector de fecha según el checkbox
    void on_checkBoxSinFecha_toggled(bool checked);

    /// @brief Valida que los campos obligatorios estén completos antes de aceptar
    void onAceptar();

private:
    Ui::DialogNuevaNota *ui;
};

#endif // DIALOGNUEVA_NOTA_H
