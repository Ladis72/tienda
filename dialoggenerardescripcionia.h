#ifndef DIALOGGENERARDESCRIPCIONIA_H
#define DIALOGGENERARDESCRIPCIONIA_H

#include <QDialog>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QSettings>

namespace Ui {
class DialogGenerarDescripcionIA;
}

/**
 * @brief Diálogo modal para generar y revisar fichas de productos con IA y búsqueda en internet.
 *
 * Flujo de ejecución:
 *  1. Búsqueda web de información, composición e indicaciones del producto (DuckDuckGo).
 *  2. Síntesis y estructuración mediante el modelo de IA local de Ollama.
 *  3. Visualización y edición en texto enriquecido para que el usuario pueda aceptarlo o rechazarlo.
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
                                        QWidget *parent = nullptr);
    ~DialogGenerarDescripcionIA();

    /// @brief Devuelve la descripción final generada (HTML o texto enriquecido)
    QString getDescripcionGenerada() const;

private slots:
    /// @brief Slot cuando finaliza la petición de búsqueda en internet
    void onSearchReplyFinished(QNetworkReply *reply);

    /// @brief Slot cuando finaliza la petición a la API de Ollama
    void onOllamaReplyFinished(QNetworkReply *reply);

    /// @brief Regenera la descripción a partir del término de búsqueda ingresado
    void on_pushButtonRegenerar_clicked();

    /// @brief Copia el contenido del editor al portapapeles
    void on_pushButtonCopiar_clicked();

    /// @brief Acepta y cierra el diálogo retornando QDialog::Accepted
    void on_pushButtonAceptar_clicked();

    /// @brief Cancela y cierra el diálogo retornando QDialog::Rejected
    void on_pushButtonRechazar_clicked();

private:
    Ui::DialogGenerarDescripcionIA *ui;
    QNetworkAccessManager *m_netManager;

    // Metadatos del producto
    QString m_nombreProducto;
    QString m_fabricante;
    QString m_familia;
    QString m_formato;
    QString m_ean;

    // Configuración de Ollama
    QString m_baseUrlOllama;
    QString m_modeloOllama;

    // Estado del proceso
    QString m_snippetsInternet;
    bool m_buscando;

    /// @brief Inicializa y lanza el proceso de búsqueda e inferencia
    void iniciarProceso();

    /// @brief Realiza la consulta web asíncrona a través de DuckDuckGo
    void buscarEnInternet(const QString &termino);

    /// @brief Extrae texto útil y snippets limpios del HTML devuelto por el buscador
    QString extraerSnippetsDeHtml(const QString &html);

    /// @brief Envía el contexto del producto y la web a Ollama
    void consultarOllama(const QString &contextoInternet);

    /// @brief Aplica estilo visual HTML estándar a la ficha
    QString formatearFichaComoHtml(const QString &textoRaw);
};

#endif // DIALOGGENERARDESCRIPCIONIA_H
