#ifndef ASISTENTEIA_H
#define ASISTENTEIA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include <QDateTime>
#include <QSqlDatabase>

/**
 * @brief Motor de Asistente de IA local para el TPV (usando Ollama REST API).
 *
 * Características:
 *  - Comunicación asíncrona mediante QNetworkAccessManager con Ollama (/api/chat y /api/tags).
 *  - Soporte de Function Calling (Tool Calling) para consultar de forma segura
 *    el stock, ventas, clientes, arqueos y recomendaciones de productos.
 *  - Control de permisos mediante GestorPermisos según el rol del usuario conectado.
 *  - Formateo estricto de fechas en formato "yyyy-MM-dd".
 *  - Conservación del historial de conversación para respuestas contextuales.
 */
class AsistenteIA : public QObject
{
    Q_OBJECT

public:
    explicit AsistenteIA(QObject *parent = nullptr);
    ~AsistenteIA();

    /// @brief Configura la URL base del servidor Ollama (ej. "http://localhost:11434")
    void setBaseUrl(const QString &url);
    QString baseUrl() const;

    /// @brief Configura el modelo a utilizar (ej. "llama3.1:8b", "qwen2.5:7b").
    /// Si ya había un modelo cargado en memoria distinto, lo descarga de la VRAM antes de cambiar.
    void setModelo(const QString &modelo);
    QString modelo() const;

    /// @brief Descarga explícitamente un modelo de la memoria VRAM/RAM de Ollama (keep_alive = 0)
    void descargarModelo(const QString &nombreModelo);

    /// @brief Descarga todos los modelos actualmente cargados en memoria en Ollama (/api/ps)
    void descargarModelosEnMemoria();

    /// @brief Consulta la lista de modelos instalados en Ollama (/api/tags)
    void consultarModelosDisponibles();

    /// @brief Envía un mensaje del usuario a la IA y procesa posibles llamadas a herramientas
    void enviarMensaje(const QString &mensajeUsuario);

    /// @brief Reinicia el historial de la conversación
    void limpiarHistorial();

    /// @brief Recarga la base de conocimiento semántico desde la base de datos
    void recargarConocimiento();

    /// @brief Devuelve true si la IA está procesando una consulta actualmente
    bool estaProcesando() const { return m_procesando; }

signals:
    /// @brief Emitido cuando la IA genera una respuesta final de texto para el usuario
    void respuestaRecibida(const QString &respuesta);

    /// @brief Notifica cambios en el estado del motor (ej. "Pensando...", "Consultando stock...")
    void estadoCambiado(const QString &estado);

    /// @brief Emitido si ocurre un error de red, de Ollama o de procesamiento
    void errorOcurrido(const QString &mensajeError);

    /// @brief Emitido con la lista de modelos locales encontrados en Ollama
    void modelosRecibidos(const QStringList &modelos);

    /// @brief Notifica la ejecución de una herramienta para mostrar un indicador visual en el chat
    void herramientaEjecutada(const QString &nombreHerramienta, const QString &resumen);

private slots:
    /// @brief Procesa la respuesta de /api/tags
    void onTagsReplyFinished(QNetworkReply *reply);

    /// @brief Procesa la respuesta de /api/chat
    void onChatReplyFinished(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_netManager;
    QString m_baseUrl;
    QString m_modelo;
    bool m_procesando;
    int m_profundidadToolCalls;
    QJsonArray m_historial;

    /// @brief Construye el mensaje inicial de sistema con contexto de la tienda y permisos
    QJsonObject construirMensajeSistema();

    /// @brief Construye el esquema JSON de herramientas disponibles para Ollama
    QJsonArray construirDefinicionHerramientas();

    /// @brief Envía la petición HTTP a /api/chat con el historial y herramientas actuales
    void enviarPeticionChat();

    /// @brief Ejecuta una herramienta solicitada por la IA tras validar permisos
    QJsonObject ejecutarHerramienta(const QString &nombre, const QJsonObject &argumentos);

    // --- Herramientas de negocio ---
    QSqlDatabase obtenerBaseDatos();
    QJsonObject toolConsultarStock(const QJsonObject &args);
    QJsonObject toolArticulosBajoMinimo(const QJsonObject &args);
    QJsonObject toolResumenVentas(const QJsonObject &args);
    QJsonObject toolUltimosArqueos(const QJsonObject &args);
    QJsonObject toolBuscarClientes(const QJsonObject &args);
    QJsonObject toolUltimasComprasCliente(const QJsonObject &args);
    QJsonObject toolBuscarPorSintoma(const QJsonObject &args);
    QJsonObject toolProductosMasVendidos(const QJsonObject &args);
    QJsonObject toolComparativaVentas(const QJsonObject &args);
    QJsonObject toolPrevisionCoberturaStock(const QJsonObject &args);
    QJsonObject toolConsultarCaducidades(const QJsonObject &args);
    QJsonObject toolConsultarComprasProveedor(const QJsonObject &args);
    QJsonObject toolFacturacionPorHoras(const QJsonObject &args);
    QJsonObject toolConsultarSalidasTiendas(const QJsonObject &args);
    QJsonObject toolConsultarPedidos(const QJsonObject &args);
    QJsonObject toolEjecutarConsultaSql(const QJsonObject &args);
};

#endif // ASISTENTEIA_H
