#ifndef DIALOGBUSCARFOTOINTERNET_H
#define DIALOGBUSCARFOTOINTERNET_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QListWidgetItem>
#include <QPixmap>

namespace Ui {
class DialogBuscarFotoInternet;
}

/**
 * @brief Estructura que almacena la información de cada imagen candidata encontrada en internet.
 */
struct FotoCandidata {
    QString urlImagen;
    QString urlThumbnail;
    QString titulo;
    int ancho = 0;
    int alto = 0;
    QPixmap thumbnail;
};

/**
 * @brief Diálogo modal para buscar fotos de artículos en internet, previsualizarlas
 *        y guardarlas directamente en el directorio configurado para imágenes del programa.
 */
class DialogBuscarFotoInternet : public QDialog
{
    Q_OBJECT

public:
    explicit DialogBuscarFotoInternet(const QString &descripcion,
                                      const QString &fabricante = QString(),
                                      const QString &codArticulo = QString(),
                                      QWidget *parent = nullptr);
    ~DialogBuscarFotoInternet();

    /// @brief Devuelve la ruta relativa del archivo de imagen guardado
    QString getNombreFicheroRelativo() const;

private slots:
    /// @brief Procesa el token VQD para la búsqueda de imágenes
    void onVqdReplyFinished(QNetworkReply *reply, const QString &termino);

    /// @brief Procesa la lista JSON de imágenes devuelta por el buscador
    void onImagesSearchReplyFinished(QNetworkReply *reply);

    /// @brief Slot invocado cuando se completa la descarga de una miniatura
    void onThumbnailFinished(QNetworkReply *reply, int index);

    /// @brief Actualiza la previsualización grande al cambiar de elemento en la galería
    void on_listWidgetFotos_itemSelectionChanged();

    /// @brief Doble clic en una imagen para seleccionarla y aceptar directamente
    void on_listWidgetFotos_itemDoubleClicked(QListWidgetItem *item);

    /// @brief Slot para el botón Buscar / Regenerar búsqueda
    void on_pushButtonBuscar_clicked();

    /// @brief Slot para aceptar, descargar la imagen en alta calidad y guardarla
    void on_pushButtonAceptar_clicked();

    /// @brief Slot para cancelar el diálogo
    void on_pushButtonCancelar_clicked();

private:
    Ui::DialogBuscarFotoInternet *ui;
    QNetworkAccessManager *m_netManager;

    QString m_nombreProducto;
    QString m_fabricante;
    QString m_codArticulo;
    QString m_directorioImagenes;
    QString m_ficheroGuardadoRelativo;

    QList<FotoCandidata> m_candidatas;
    bool m_buscando;

    /// @brief Inicia el ciclo completo de búsqueda de fotos
    void iniciarBusqueda();

    /// @brief Obtiene el token VQD necesario para consultar imágenes en DuckDuckGo
    void obtenerVqdYBuscar(const QString &termino);

    /// @brief Realiza la petición de catálogo de imágenes con el token VQD
    void buscarImagenesConVqd(const QString &vqd, const QString &termino);

    /// @brief Descarga las miniaturas de las imágenes candidatas en segundo plano
    void descargarMiniaturas();

    /// @brief Descarga la foto en resolución completa y la almacena en el directorio de imágenes
    void descargarYGuardarFoto(const QString &url, const QString &rutaDestino);
};

#endif // DIALOGBUSCARFOTOINTERNET_H
