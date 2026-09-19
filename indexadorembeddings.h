/**
 * @file indexadorembeddings.h
 * @brief Indexación semántica, persistencia y búsqueda vectorial para el catálogo y áreas de conocimiento.
 * 
 * Gestiona la generación de embeddings mediante Ollama (nomic-embed-text), el almacenamiento
 * en la base de datos (nube MySQL o local SQLite), una caché binaria rápida en disco/RAM
 * y la recuperación mediante similitud de coseno (producto escalar de vectores normalizados).
 */

#ifndef INDEXADOREMBEDDINGS_H
#define INDEXADOREMBEDDINGS_H

#include <QObject>
#include <QString>
#include <QVector>
#include <QByteArray>
#include <QDateTime>
#include <QSqlDatabase>
#include <QMutex>
#include <QThread>

/**
 * @brief Representa un vector embedding asociado a un código y tipo de elemento.
 */
struct ItemEmbedding {
    QString cod;            ///< Código del artículo o identificador del área
    QString tipo;           ///< 'articulo' o 'area'
    QVector<float> vector;  ///< Vector de dimensiones (768) normalizado L2
};

/**
 * @brief Resultado ordenado de una búsqueda por similitud.
 */
struct ResultadoSimilitud {
    QString cod;            ///< Código del artículo o identificador de área
    QString tipo;           ///< 'articulo' o 'area'
    float score;            ///< Puntuación de similitud coseno (entre -1.0 y 1.0, típicamente > 0.3)
};

/**
 * @brief Hilo de trabajo para ejecutar la indexación en segundo plano sin bloquear la interfaz.
 */
class WorkerIndexador : public QThread {
    Q_OBJECT
public:
    explicit WorkerIndexador(QObject *parent = nullptr);
    void solicitarCancelacion();

    /// @brief Configura las credenciales y parámetros de conexión para el hilo de trabajo.
    void configurarConexion(const QString &driver,
                            const QString &host,
                            int port,
                            const QString &databaseName,
                            const QString &userName,
                            const QString &password,
                            const QString &connectOptions);

signals:
    void progreso(int actual, int total, const QString &mensaje);
    void finalizado(bool exito, const QString &resumen);

protected:
    void run() override;

private:
    bool m_cancelar;
    QMutex m_mutexCancel;

    QString m_driver;
    QString m_host;
    int m_port;
    QString m_databaseName;
    QString m_userName;
    QString m_password;
    QString m_connectOptions;
};

/**
 * @brief Gestor centralizado de embeddings y búsqueda semántica híbrida.
 */
class IndexadorEmbeddings : public QObject {
    Q_OBJECT

public:
    /// @brief Obtiene la instancia singleton del indexador.
    static IndexadorEmbeddings* instancia();

    /// @brief Destructor.
    ~IndexadorEmbeddings() override;

    /// @brief Asegura que la tabla ia_embeddings existe en la base de datos dada (MySQL o SQLite).
    static bool asegurarTabla(QSqlDatabase &db);

    /// @brief Carga la caché de embeddings en memoria RAM desde el archivo local o desde la BD si no existe.
    /// @param forzarRecarga Si es true, ignora la caché local y recarga desde la base de datos.
    bool cargarCache(bool forzarRecarga = false);

    /// @brief Guarda la memoria de embeddings en el archivo local de caché binario.
    bool guardarCacheEnDisco();

    /// @brief Obtiene el número de embeddings actualmente cargados en memoria.
    int totalEnCache() const;

    /// @brief Obtiene la fecha/hora de última actualización del índice en memoria.
    QDateTime fechaUltimaActualizacion() const;

    /// @brief Realiza una búsqueda semántica de los artículos más similares al texto consultado.
    /// @param query Texto o síntoma consultado por el usuario.
    /// @param topK Número máximo de resultados a devolver.
    /// @param umbralMinimo Score mínimo de similitud coseno aceptado (por defecto 0.35).
    QList<ResultadoSimilitud> buscarArticulosSimilares(const QString &query, int topK = 50, float umbralMinimo = 0.35f);

    /// @brief Busca las áreas de conocimiento de fitoterapia activadas por el síntoma o consulta.
    /// @param query Texto o síntoma consultado.
    /// @param umbralMinimo Score mínimo de similitud para activar el área.
    QList<ResultadoSimilitud> buscarAreasSimilares(const QString &query, float umbralMinimo = 0.40f);

    /// @brief Obtiene el embedding vectorial de un texto único mediante Ollama.
    /// @param texto Contenido a convertir en vector.
    /// @param esQuery Si es true añade el prefijo 'search_query: ', si es false añade 'search_document: '.
    /// @param timeoutMs Tiempo máximo de espera en milisegundos.
    QVector<float> obtenerEmbedding(const QString &texto, bool esQuery = true, int timeoutMs = 3500);

    /// @brief Inicia el proceso asíncrono de reindexación total o incremental.
    void iniciarReindexacionAsync();

    /// @brief Cancela la reindexación en segundo plano si está en curso.
    void cancelarReindexacion();

    /// @brief Comprueba si el indexador está ejecutando una tarea en segundo plano.
    bool estaIndexando() const;

    /// @brief Limpia texto HTML y normaliza espacios para embedding.
    static QString limpiarTextoParaEmbedding(const QString &texto, int maxLongitud = 6000);

    /// @brief Normaliza un vector a norma Euclidiana (L2) unitaria.
    static void normalizarL2(QVector<float> &vector);

    /// @brief Calcula el producto escalar (similitud coseno de vectores normalizados L2).
    static float productoEscalar(const QVector<float> &v1, const QVector<float> &v2);

signals:
    /// @brief Señal emitida para actualizar barras de progreso de la UI.
    void progreso(int actual, int total, const QString &mensaje);

    /// @brief Señal emitida cuando concluye la indexación en segundo plano.
    void indexacionFinalizada(bool exito, const QString &resumen);

    /// @brief Señal emitida cuando la caché en RAM ha sido actualizada.
    void cacheActualizada();

private:
    explicit IndexadorEmbeddings(QObject *parent = nullptr);

    /// @brief Obtiene la URL base de Ollama configurada en la aplicación.
    static QString obtenerUrlOllama();

    /// @brief Obtiene el nombre del modelo de embeddings configurado.
    static QString obtenerModeloEmbeddings();

    /// @brief Ruta absoluta al fichero de caché local en disco.
    static QString rutaFicheroCache();

    mutable QMutex m_mutexCache;
    QList<ItemEmbedding> m_cacheMemoria;
    QDateTime m_fechaMaxActualizacion;
    WorkerIndexador *m_worker;

    friend class WorkerIndexador;
};

#endif // INDEXADOREMBEDDINGS_H
