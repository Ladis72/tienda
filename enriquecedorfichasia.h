#ifndef ENRIQUECEDORFICHASIA_H
#define ENRIQUECEDORFICHASIA_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QList>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>

/**
 * @brief Representa una fuente de información web contrastada.
 */
struct FuenteWeb {
    QString url;
    QString titulo;
    QString snippet;
    QString texto;
    QString tipo;       // "open_facts", "oficial", "web", "snippet"
    bool oficial = false;
    int puntuacionFormato = 0;
    int puntuacionKeywords = 0;
};

/**
 * @brief Representa el resultado completo del análisis y redacción técnica de la ficha.
 */
struct ResultadoFicha {
    QString estado;          // "ok", "revisar", "error"
    QString confianza;       // "alta", "media", "baja"
    QString productoDetectado;
    bool coincide = false;
    QString composicion;
    QString descripcion;
    QString modoEmpleo;
    QString advertencias;
    QList<FuenteWeb> fuentes;
    double exactitud = 0.0;  // Solapamiento léxico de tokens (0.0 a 1.0)
    QString error;
    QString htmlFormateado;
};

/**
 * @brief Motor nativo C++/Qt para enriquecimiento automático y verificación
 * de fichas de producto herbolario/dietética mediante Open Facts, Google Serper API,
 * web scraping limpio e inferencia local con Ollama.
 */
class EnriquecedorFichasIA : public QObject
{
    Q_OBJECT

public:
    explicit EnriquecedorFichasIA(QObject *parent = nullptr);
    ~EnriquecedorFichasIA();

    /// @brief Inicia el proceso de enriquecimiento para un artículo
    void procesarArticulo(const QString &ean,
                          const QString &descripcion,
                          const QString &fabricante = QString(),
                          const QString &familia = QString(),
                          const QString &formato = QString(),
                          const QString &notasPrevias = QString());

    /// @brief Cancela cualquier petición en curso
    void cancelar();

    /// @brief Obtiene la clave de Serper API guardada en tienda.ini o variables de entorno
    static QString obtenerClaveSerper();

    /// @brief Guarda la clave de Serper API en tienda.ini
    static void guardarClaveSerper(const QString &apiKey);

signals:
    /// @brief Notifica el avance en los pasos del pipeline
    void progreso(int pasoActual, int totalPasos, const QString &mensaje);

    /// @brief Notifica la finalización del proceso
    void finalizado(bool exito, const ResultadoFicha &resultado);

private slots:
    void onOpenFactsTerminado();
    void onSerperTerminado();
    void onPaginaWebDescargada(QNetworkReply *reply, int indiceFuente);
    void onOllamaTerminado(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_netManager;

    // Datos del artículo actual
    QString m_ean;
    QString m_descripcion;
    QString m_fabricante;
    QString m_familia;
    QString m_formato;
    QString m_notasPrevias;

    // Configuración
    QString m_baseUrlOllama;
    QString m_modeloOllama;
    QString m_apiKeySerper;

    // Estado del pipeline
    bool m_cancelado;
    int m_pasoActual;
    QList<FuenteWeb> m_fuentes;
    int m_openFactsPendientes;
    int m_serperPendientes;
    int m_paginasPendientes;

    // Métodos auxiliares
    void paso1_consultarOpenFacts();
    void paso2_consultarSerper();
    void paso3_descargarPaginasWeb();
    void paso4_extraerConOllama();
    void paso5_validarYFinalizar(const QJsonObject &jsonExtraccion);

    // Utilidades de texto y filtrado
    QStringList extraerTokensFormato(const QString &texto) const;
    QString limpiarTextoDesc(const QString &desc) const;
    QString slugMarca(const QString &marca) const;
    int puntuacionKeywords(const QString &texto) const;
    QString limpiarHtml(const QString &htmlBruto) const;
    double calcularSolapamientoTokens(const QString &composicion, const QStringList &textosFuentes) const;
    QString formatearResultadoHtml(const ResultadoFicha &res) const;
};

#endif // ENRIQUECEDORFICHASIA_H
