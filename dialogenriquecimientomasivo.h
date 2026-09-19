#ifndef DIALOGENRIQUECIMIENTOMASIVO_H
#define DIALOGENRIQUECIMIENTOMASIVO_H

#include <QDialog>
#include <QList>
#include "enriquecedorfichasia.h"

namespace Ui {
class DialogEnriquecimientoMasivo;
}

/**
 * @brief Estructura que representa un artículo candidato para catalogación masiva.
 */
struct ArticuloLote {
    QString ean;
    QString descripcion;
    QString fabricante;
    QString familia;
    QString formato;
    QString notas;
};

/**
 * @brief Diálogo modal para la catalogación, enriquecimiento y verificación
 * masiva de fichas de producto en el ERP mediante IA y fuentes web.
 */
class DialogEnriquecimientoMasivo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogEnriquecimientoMasivo(QWidget *parent = nullptr);
    ~DialogEnriquecimientoMasivo();

private slots:
    /// @brief Carga desde MariaDB los artículos que cumplen los filtros seleccionados
    void on_pushButtonCargarLista_clicked();

    /// @brief Inicia el procesamiento secuencial por lotes
    void on_pushButtonIniciar_clicked();

    /// @brief Excluye de la lista los artículos seleccionados por el usuario
    void on_pushButtonExcluir_clicked();

    /// @brief Detiene la ejecución en curso
    void on_pushButtonDetener_clicked();

    /// @brief Cierra el diálogo cancelando operaciones pendientes
    void on_pushButtonCerrar_clicked();

    /// @brief Muestra el detalle de la ficha del artículo seleccionado en la tabla
    void on_pushButtonVerFicha_clicked();

    /// @brief Acepta y guarda en MariaDB la ficha del artículo seleccionado
    void on_pushButtonAceptarFicha_clicked();

    /// @brief Rechaza la ficha generada para el artículo seleccionado
    void on_pushButtonRechazarFicha_clicked();

    /// @brief Acepta y guarda en lote todas las fichas en estado OK que estén pendientes
    void on_pushButtonAceptarTodasPendientes_clicked();

    /// @brief Actualiza la previsualización inferior al cambiar la selección en la tabla
    void on_tableWidgetResultados_itemSelectionChanged();

    /// @brief Desmarca auto-guardado si se activa el modo interactivo
    void on_checkBoxModoInteractivo_toggled(bool checked);

    /// @brief Desmarca modo interactivo si se activa el auto-guardado
    void on_checkBoxActualizarBD_toggled(bool checked);

    /// @brief Doble clic en una fila de la tabla para abrir el visor de ficha
    void on_tableWidgetResultados_cellDoubleClicked(int row, int column);

    /// @brief Slot cuando el motor de enriquecimiento emite el resultado de un producto
    void onProductoFinalizado(bool exito, const ResultadoFicha &resultado);

    /// @brief Slot de avance interno de cada producto
    void onProductoProgreso(int pasoActual, int totalPasos, const QString &mensaje);

private:
    Ui::DialogEnriquecimientoMasivo *ui;
    EnriquecedorFichasIA *m_enriquecedor;

    QList<ArticuloLote> m_colaArticulos;
    QList<ResultadoFicha> m_resultados;
    int m_indiceActual;
    bool m_ejecutando;

    int m_contadorOk;
    int m_contadorRevisar;
    int m_contadorError;

    /// @brief Configura las columnas y cabeceras de la tabla
    void configurarTabla();

    /// @brief Procesa el siguiente artículo en la cola
    void procesarSiguiente();

    /// @brief Actualiza los datos mostrados en el panel inferior de previsualización
    void actualizarPanelPrevisualizacion(int row);

    /// @brief Muestra diálogo interactivo para aceptar, editar o rechazar la ficha de un producto
    /// @return true si el proceso debe continuar con el siguiente artículo, false si se detuvo
    bool mostrarDialogoRevision(int row);

    /// @brief Actualiza la base de datos MariaDB para un artículo verificado
    bool guardarEnBaseDatos(const QString &ean, const QString &htmlNotas);

    /// @brief Valida si una cadena cumple el algoritmo de checksum EAN-13
    bool esEan13Valido(const QString &cod) const;
};

#endif // DIALOGENRIQUECIMIENTOMASIVO_H
