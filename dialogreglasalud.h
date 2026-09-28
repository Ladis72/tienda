#ifndef DIALOGREGLASALUD_H
#define DIALOGREGLASALUD_H

#include <QDialog>
#include <QString>

namespace Ui {
class DialogReglaSalud;
}

/**
 * @brief Diálogo modal para la creación y edición de reglas de conocimiento de salud para la IA.
 *
 * Utiliza un archivo .ui diseñado en Qt Creator para mantener coherencia visual y
 * facilitar el mantenimiento gráfico de la interfaz.
 */
class DialogReglaSalud : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor de DialogReglaSalud.
     * @param parent Widget padre (opcional).
     * @param esEdicion True si se está editando una regla existente, false si es una nueva regla.
     */
    explicit DialogReglaSalud(QWidget *parent = nullptr, bool esEdicion = false);
    ~DialogReglaSalud();

    /// @brief Configura los valores iniciales de la regla en el formulario
    void setDatos(const QString &categoria, const QString &detonantes, const QString &terminos);

    /// @brief Devuelve la categoría o dolencia introducida
    QString categoria() const;

    /// @brief Devuelve las palabras detonantes introducidas
    QString detonantes() const;

    /// @brief Devuelve los términos clave y principios activos introducidos
    QString terminos() const;

private slots:
    /// @brief Valida que la categoría no esté vacía antes de confirmar
    void onAceptar();

private:
    Ui::DialogReglaSalud *ui;
};

#endif // DIALOGREGLASALUD_H
