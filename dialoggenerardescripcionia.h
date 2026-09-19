#ifndef DIALOGGENERARDESCRIPCIONIA_H
#define DIALOGGENERARDESCRIPCIONIA_H

#include <QDialog>
#include "enriquecedorfichasia.h"

namespace Ui {
class DialogGenerarDescripcionIA;
}

/**
 * @brief Diálogo modal para generar y revisar fichas de productos con IA y fuentes web verificadas.
 *
 * Flujo de ejecución:
 *  1. Búsqueda de fuentes verificables por EAN (Open Food/Beauty Facts) y Google (Serper API).
 *  2. Descarga y extracción limpia del texto de fichas oficiales de fabricantes y parafarmacias.
 *  3. Inferencia rigurosa con modelo de IA local (Ollama) a temperatura 0 (cero alucinación).
 *  4. Validación algorítmica de solapamiento léxico de ingredientes (exactitud >= 60%).
 *  5. Visualización de estado de verificación, fuentes oficiales y edición antes de aplicar a la ficha.
 */
class DialogGenerarDescripcionIA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogGenerarDescripcionIA(const QString &descripcion,
                                        const QString &fabricante = QString(),
                                        const QString &familia = QString(),
                                        const QString &formato = QString(),
                                        const QString &ean = QString(),
                                        const QString &notasPrevias = QString(),
                                        QWidget *parent = nullptr);
    ~DialogGenerarDescripcionIA();

    /// @brief Devuelve la descripción final generada (HTML o texto enriquecido)
    QString getDescripcionGenerada() const;

private slots:
    /// @brief Slot cuando el motor de enriquecimiento notifica un avance
    void onProgreso(int pasoActual, int totalPasos, const QString &mensaje);

    /// @brief Slot cuando el motor de enriquecimiento termina el análisis
    void onFinalizado(bool exito, const ResultadoFicha &resultado);

    /// @brief Regenera la descripción a partir del término ingresado
    void on_pushButtonRegenerar_clicked();

    /// @brief Copia el contenido del editor al portapapeles
    void on_pushButtonCopiar_clicked();

    /// @brief Acepta y cierra el diálogo retornando QDialog::Accepted
    void on_pushButtonAceptar_clicked();

    /// @brief Cancela y cierra el diálogo retornando QDialog::Rejected
    void on_pushButtonRechazar_clicked();

private:
    Ui::DialogGenerarDescripcionIA *ui;
    EnriquecedorFichasIA *m_enriquecedor;

    // Metadatos del producto
    QString m_nombreProducto;
    QString m_fabricante;
    QString m_familia;
    QString m_formato;
    QString m_ean;
    QString m_notasPrevias;

    // Estado del proceso
    bool m_buscando;
    int m_contadorGeneraciones;

    /// @brief Inicializa y lanza el proceso de enriquecimiento
    void iniciarProceso();
};

#endif // DIALOGGENERARDESCRIPCIONIA_H
