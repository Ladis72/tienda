#ifndef DIALOGFOTOSMASIVO_H
#define DIALOGFOTOSMASIVO_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QPixmap>
#include <QListWidgetItem>
#include "dialogbuscarfotointernet.h" // Para FotoCandidata si procede o definirla

namespace Ui {
class DialogFotosMasivo;
}

/**
 * @brief Estructura que representa un artículo cargado en la cola para búsqueda de fotos.
 */
struct ArticuloFotoLote {
    QString ean;
    QString descripcion;
    QString fabricante;
    QString familia;
    QString formato;
    QString fotoActual;
};

/**
 * @brief Estructura con los resultados y fotos candidatas encontradas para un artículo.
 */
struct ResultadoFotoArticulo {
    QString estado = "pendiente"; // "pendiente", "buscando", "ok", "sin_foto", "error", "rechazado"
    QString fotoGuardada;
    QList<FotoCandidata> candidatas;
    int candidatoSeleccionado = -1;
    QString error;
    bool guardadoEnBd = false;
};

/**
 * @brief Diálogo modal para la búsqueda, revisión y asignación masiva de fotos a artículos.
 */
class DialogFotosMasivo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFotosMasivo(QWidget *parent = nullptr);
    ~DialogFotosMasivo();

private slots:
    /// @brief Carga la lista de artículos según los filtros configurados
    void on_pushButtonCargarLista_clicked();

    /// @brief Excluye de la lista los artículos seleccionados en la tabla
    void on_pushButtonExcluir_clicked();

    /// @brief Inicia el procesamiento por lotes
    void on_pushButtonIniciar_clicked();

    /// @brief Detiene el procesamiento en curso
    void on_pushButtonDetener_clicked();

    /// @brief Cierra el diálogo
    void on_pushButtonCerrar_clicked();

    /// @brief Acepta y asigna la foto seleccionada para el artículo actual
    void on_pushButtonAceptarFoto_clicked();

    /// @brief Rechaza u omite la asignación de foto para el artículo actual
    void on_pushButtonRechazarFoto_clicked();

    /// @brief Acepta y guarda la primera foto candidata para todos los artículos que la tengan
    void on_pushButtonAceptarTodasPendientes_clicked();

    /// @brief Abre la foto en resolución completa en un visor
    void on_pushButtonVerGrande_clicked();

    /// @brief Actualiza la previsualización al cambiar de fila seleccionada en la tabla
    void on_tableWidgetResultados_itemSelectionChanged();

    /// @brief Cambia la foto candidata activa en la previsualización grande
    void on_listWidgetCandidatas_itemSelectionChanged();

    /// @brief Doble clic en una miniatura candidata para seleccionarla y guardarla directamente
    void on_listWidgetCandidatas_itemDoubleClicked(QListWidgetItem *item);

    /// @brief Doble clic en una fila de la tabla para ver la foto
    void on_tableWidgetResultados_cellDoubleClicked(int row, int column);

    /// @brief Desmarca auto-guardado si se activa el modo interactivo
    void on_checkBoxModoInteractivo_toggled(bool checked);

    /// @brief Desmarca modo interactivo si se activa el auto-guardado
    void on_checkBoxActualizarBD_toggled(bool checked);

private:
    Ui::DialogFotosMasivo *ui;
    QNetworkAccessManager *m_netManager;

    QString m_directorioImagenes;
    QList<ArticuloFotoLote> m_colaArticulos;
    QList<ResultadoFotoArticulo> m_resultados;

    int m_indiceActual;
    bool m_ejecutando;

    int m_contadorOk;
    int m_contadorSinFoto;
    int m_contadorError;

    /// @brief Configura las cabeceras y modo de selección de la tabla
    void configurarTabla();

    /// @brief Valida si un código de barras es un EAN-13 numérico válido
    bool esEan13Valido(const QString &cod) const;

    /// @brief Procesa el siguiente artículo en la cola
    void procesarSiguiente();

    /// @brief Envía la petición HTTP de búsqueda de fotos a Bing Images para un artículo
    void buscarFotosArticulo(int index);

    /// @brief Procesa la respuesta HTML de imágenes y extrae las candidatas
    void onRespuestaBusquedaTerminada(QNetworkReply *reply, int index);

    /// @brief Descarga las miniaturas de las fotos candidatas para la galería
    void descargarMiniaturas(int index);

    /// @brief Guarda la foto seleccionada en el disco y en la base de datos
    bool descargarYAsignarFoto(int rowArticulo, int indexCandidato);

    /// @brief Actualiza los datos y la galería del panel inferior de previsualización
    void actualizarPanelPrevisualizacion(int row);

    /// @brief Actualiza los contadores de métricas en la interfaz
    void actualizarMetricas();
};

#endif // DIALOGFOTOSMASIVO_H
