/**
 * @file indexadorembeddings.cpp
 * @brief Implementación del indexador de embeddings semánticos y caché vectorial.
 */

#include "indexadorembeddings.h"
#include "base_datos.h"
#include "syncmanager.h"
#include "configuracion.h"

#include <cmath>
#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDataStream>
#include <QDateTime>
#include <QDebug>
#include <QDir>
#include <QEventLoop>
#include <QFile>
#include <QFileInfo>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QMutexLocker>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QRegularExpression>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QTimer>
#include <QUuid>
#include <algorithm>

extern Configuracion *conf;

// Identificador mágico para el archivo de caché binario
static const char CACHE_MAGIC[7] = {'I', 'A', 'E', 'M', 'B', '0', '1'};
static const qint32 CACHE_VERSION = 1;

/* ========================================================================= */
/*                         MÉTODOS AUXILIARES Y MATEMÁTICOS                  */
/* ========================================================================= */

/**
 * @brief Normaliza un vector a norma Euclidiana unitaria (L2 = 1.0).
 */
void IndexadorEmbeddings::normalizarL2(QVector<float> &vec)
{
    if (vec.isEmpty()) return;
    double sumaCuadrados = 0.0;
    const int n = vec.size();
    const float *datos = vec.constData();
    for (int i = 0; i < n; ++i) {
        sumaCuadrados += static_cast<double>(datos[i]) * static_cast<double>(datos[i]);
    }
    double norma = std::sqrt(sumaCuadrados);
    if (norma > 1e-7) {
        float factor = static_cast<float>(1.0 / norma);
        float *modificable = vec.data();
        for (int i = 0; i < n; ++i) {
            modificable[i] *= factor;
        }
    }
}

/**
 * @brief Calcula el producto escalar entre dos vectores ya normalizados L2.
 */
float IndexadorEmbeddings::productoEscalar(const QVector<float> &v1, const QVector<float> &v2)
{
    if (v1.size() != v2.size() || v1.isEmpty()) return 0.0f;
    float dot = 0.0f;
    const float *d1 = v1.constData();
    const float *d2 = v2.constData();
    const int n = v1.size();
    for (int i = 0; i < n; ++i) {
        dot += d1[i] * d2[i];
    }
    return dot;
}

/**
 * @brief Limpia texto HTML, decodifica entidades básicas y trunca a longitud máxima.
 */
QString IndexadorEmbeddings::limpiarTextoParaEmbedding(const QString &texto, int maxLongitud)
{
    if (texto.isEmpty()) return QString();

    QString resultado = texto;
    // Eliminar etiquetas HTML
    resultado.remove(QRegularExpression("<[^>]*>"));
    // Decodificar entidades HTML habituales
    resultado.replace("&nbsp;", " ");
    resultado.replace("&amp;", "&");
    resultado.replace("&lt;", "<");
    resultado.replace("&gt;", ">");
    resultado.replace("&quot;", "\"");
    resultado.replace("&#39;", "'");
    // Normalizar espacios múltiples y saltos de línea
    resultado.replace(QRegularExpression("\\s+"), " ");
    resultado = resultado.trimmed();

    if (resultado.length() > maxLongitud) {
        int corte = resultado.lastIndexOf(' ', maxLongitud);
        if (corte > maxLongitud / 2) {
            resultado = resultado.left(corte);
        } else {
            resultado = resultado.left(maxLongitud);
        }
    }
    return resultado;
}

/**
 * @brief Obtiene la URL configurada para el servidor Ollama.
 */
QString IndexadorEmbeddings::obtenerUrlOllama()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("Ollama");
    QString url = settings.value("url", "").toString().trimmed();
    settings.endGroup();

    if (url.isEmpty()) {
        QSettings settingsFallback("Ladis", "tienda");
        url = settingsFallback.value("url", "http://localhost:11434").toString().trimmed();
    }
    if (url.isEmpty()) url = "http://localhost:11434";
    if (url.endsWith("/")) url.chop(1);
    return url;
}

/**
 * @brief Obtiene el nombre del modelo configurado para embeddings (por defecto nomic-embed-text).
 */
QString IndexadorEmbeddings::obtenerModeloEmbeddings()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("Ollama");
    QString modelo = settings.value("modelo_embedding", "").toString().trimmed();
    settings.endGroup();

    if (modelo.isEmpty()) {
        QSettings settingsFallback("Ladis", "tienda");
        modelo = settingsFallback.value("modelo_embedding", "nomic-embed-text").toString().trimmed();
    }
    if (modelo.isEmpty()) modelo = "nomic-embed-text";
    return modelo;
}

/**
 * @brief Ruta local del archivo binario de caché.
 */
QString IndexadorEmbeddings::rutaFicheroCache()
{
    return QCoreApplication::applicationDirPath() + "/ia_embeddings.cache";
}

/* ========================================================================= */
/*                         CONSTRUCTOR Y SINGLETON                           */
/* ========================================================================= */

IndexadorEmbeddings::IndexadorEmbeddings(QObject *parent)
    : QObject(parent),
      m_worker(nullptr)
{
}

IndexadorEmbeddings::~IndexadorEmbeddings()
{
    cancelarReindexacion();
}

IndexadorEmbeddings* IndexadorEmbeddings::instancia()
{
    static IndexadorEmbeddings s_instancia;
    return &s_instancia;
}

/* ========================================================================= */
/*                         PERSISTENCIA Y TABLA SQL                          */
/* ========================================================================= */

/**
 * @brief Asegura la creación de la tabla ia_embeddings en la base de datos indicada.
 */
bool IndexadorEmbeddings::asegurarTabla(QSqlDatabase &db)
{
    if (!db.isOpen()) return false;
    return baseDatos::crearTablasIA(db);
}

/* ========================================================================= */
/*                         GESTIÓN DE CACHÉ (DISCO / RAM)                    */
/* ========================================================================= */

/**
 * @brief Carga la caché de embeddings en memoria RAM.
 */
bool IndexadorEmbeddings::cargarCache(bool forzarRecarga)
{
    QMutexLocker locker(&m_mutexCache);
    QString ruta = rutaFicheroCache();

    // 1. Intentar cargar desde el archivo binario local si existe y no se fuerza recarga
    if (!forzarRecarga && QFile::exists(ruta)) {
        QFile file(ruta);
        if (file.open(QIODevice::ReadOnly)) {
            QDataStream stream(&file);
            char magic[7];
            if (stream.readRawData(magic, 7) == 7 && memcmp(magic, CACHE_MAGIC, 7) == 0) {
                qint32 version;
                stream >> version;
                if (version == CACHE_VERSION) {
                    qint32 total;
                    qint64 msecsEpoch;
                    stream >> total >> msecsEpoch;

                    // Sólo aceptar la caché en disco si contiene al menos un elemento
                    if (total > 0) {
                        m_fechaMaxActualizacion = QDateTime::fromMSecsSinceEpoch(msecsEpoch);
                        m_cacheMemoria.clear();
                        m_cacheMemoria.reserve(total);

                        bool errorLectura = false;
                        for (int i = 0; i < total; ++i) {
                            ItemEmbedding item;
                            stream >> item.cod >> item.tipo;
                            quint16 dims;
                            stream >> dims;
                            if (dims > 0 && dims <= 4096) {
                                item.vector.resize(dims);
                                int bytes = dims * sizeof(float);
                                if (stream.readRawData(reinterpret_cast<char*>(item.vector.data()), bytes) != bytes) {
                                    errorLectura = true;
                                    break;
                                }
                                m_cacheMemoria.append(item);
                            } else {
                                errorLectura = true;
                                break;
                            }
                        }

                        if (!errorLectura && m_cacheMemoria.size() == total) {
                            file.close();
                            emit cacheActualizada();
                            return true;
                        }
                    }
                }
            }
            file.close();
        }
    }

    // 2. Si no hay archivo válido con datos o se forzó recarga, leer desde la base de datos (priorizando local offline-first)
    QString connLocal = conf ? conf->getConexionLocal() : "DB";
    if (connLocal.isEmpty()) connLocal = "DB";
    QSqlDatabase db;
    if (QSqlDatabase::contains(connLocal) && QSqlDatabase::database(connLocal).isOpen()) {
        db = QSqlDatabase::database(connLocal);
    } else if (QSqlDatabase::contains("DB") && QSqlDatabase::database("DB").isOpen()) {
        db = QSqlDatabase::database("DB");
    } else if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    } else {
        db = QSqlDatabase::database();
    }

    if (!db.isOpen()) {
        qWarning() << "IndexadorEmbeddings::cargarCache: No hay conexión a base de datos abierta para cargar embeddings.";
        return false;
    }
    asegurarTabla(db);

    QSqlQuery q(db);
    if (!q.exec("SELECT cod, tipo, embedding, updated_at FROM ia_embeddings")) {
        qWarning() << "IndexadorEmbeddings::cargarCache: Error al consultar ia_embeddings:" << q.lastError().text();
        return false;
    }

    m_cacheMemoria.clear();
    QDateTime maxFecha;

    while (q.next()) {
        ItemEmbedding item;
        item.cod = q.value("cod").toString();
        item.tipo = q.value("tipo").toString();
        QByteArray blob = q.value("embedding").toByteArray();

        if (blob.size() >= static_cast<int>(sizeof(float))) {
            int numFloats = blob.size() / sizeof(float);
            item.vector.resize(numFloats);
            memcpy(item.vector.data(), blob.constData(), blob.size());
            normalizarL2(item.vector);
            m_cacheMemoria.append(item);
        }

        QDateTime f = q.value("updated_at").toDateTime();
        if (f.isValid() && (!maxFecha.isValid() || f > maxFecha)) {
            maxFecha = f;
        }
    }

    m_fechaMaxActualizacion = maxFecha;
    int totalCargados = m_cacheMemoria.size();
    locker.unlock();

    if (totalCargados > 0) {
        guardarCacheEnDisco();
    } else {
        qWarning() << "IndexadorEmbeddings::cargarCache: La tabla ia_embeddings está vacía. Es necesario regenerar el índice.";
    }

    emit cacheActualizada();
    return (totalCargados > 0);
}

/**
 * @brief Guarda la memoria de embeddings en el archivo binario local.
 */
bool IndexadorEmbeddings::guardarCacheEnDisco()
{
    QMutexLocker locker(&m_mutexCache);
    if (m_cacheMemoria.isEmpty()) {
        // No guardar un archivo de caché vacío para no enmascarar datos de la base de datos
        return false;
    }

    QString ruta = rutaFicheroCache();
    QString rutaTmp = ruta + ".tmp";

    QFile file(rutaTmp);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }

    QDataStream stream(&file);
    stream.writeRawData(CACHE_MAGIC, 7);
    stream << CACHE_VERSION;
    stream << static_cast<qint32>(m_cacheMemoria.size());
    stream << static_cast<qint64>(m_fechaMaxActualizacion.isValid() ? m_fechaMaxActualizacion.toMSecsSinceEpoch() : 0);

    for (const ItemEmbedding &item : m_cacheMemoria) {
        stream << item.cod << item.tipo;
        quint16 dims = static_cast<quint16>(item.vector.size());
        stream << dims;
        if (dims > 0) {
            int bytes = dims * sizeof(float);
            stream.writeRawData(reinterpret_cast<const char*>(item.vector.constData()), bytes);
        }
    }

    file.close();

    // Reemplazo atómico del archivo
    if (QFile::exists(ruta)) {
        QFile::remove(ruta);
    }
    return file.rename(rutaTmp, ruta);
}

int IndexadorEmbeddings::totalEnCache() const
{
    QMutexLocker locker(&m_mutexCache);
    return m_cacheMemoria.size();
}

QDateTime IndexadorEmbeddings::fechaUltimaActualizacion() const
{
    QMutexLocker locker(&m_mutexCache);
    return m_fechaMaxActualizacion;
}

/* ========================================================================= */
/*                         OBTENCIÓN DE EMBEDDINGS (OLLAMA)                  */
/* ========================================================================= */

/**
 * @brief Obtiene el embedding vectorial de un texto único llamando a Ollama.
 */
QVector<float> IndexadorEmbeddings::obtenerEmbedding(const QString &texto, bool esQuery, int timeoutMs)
{
    if (texto.trimmed().isEmpty()) return QVector<float>();

    QString textoConPrefijo = (esQuery ? "search_query: " : "search_document: ") + texto.trimmed();
    QString urlOllama = obtenerUrlOllama();
    QString modelo = obtenerModeloEmbeddings();

    QNetworkAccessManager nam;
    QNetworkRequest request(QUrl(urlOllama + "/api/embed"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject payload;
    payload["model"] = modelo;
    payload["input"] = textoConPrefijo;

    QNetworkReply *reply = nam.post(request, QJsonDocument(payload).toJson());
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);

    timer.start(timeoutMs);
    loop.exec();

    QVector<float> resultado;

    if (timer.isActive() && reply->error() == QNetworkReply::NoError) {
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        QJsonObject root = doc.object();

        // En /api/embed el resultado es "embeddings": [[...]]
        if (root.contains("embeddings") && root["embeddings"].isArray()) {
            QJsonArray embeddings = root["embeddings"].toArray();
            if (!embeddings.isEmpty() && embeddings[0].isArray()) {
                QJsonArray vectorArray = embeddings[0].toArray();
                resultado.reserve(vectorArray.size());
                for (const QJsonValue &v : vectorArray) {
                    resultado.append(static_cast<float>(v.toDouble()));
                }
            }
        }
    } else if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
        // Fallback a /api/embeddings si la versión de Ollama fuese anterior a /api/embed
        reply->deleteLater();
        QNetworkRequest reqLegacy(QUrl(urlOllama + "/api/embeddings"));
        reqLegacy.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
        QJsonObject payLegacy;
        payLegacy["model"] = modelo;
        payLegacy["prompt"] = textoConPrefijo;

        QNetworkReply *repLegacy = nam.post(reqLegacy, QJsonDocument(payLegacy).toJson());
        QEventLoop loopLegacy;
        QTimer timerLegacy;
        timerLegacy.setSingleShot(true);
        connect(&timerLegacy, &QTimer::timeout, &loopLegacy, &QEventLoop::quit);
        connect(repLegacy, &QNetworkReply::finished, &loopLegacy, &QEventLoop::quit);
        timerLegacy.start(timeoutMs);
        loopLegacy.exec();

        if (timerLegacy.isActive() && repLegacy->error() == QNetworkReply::NoError) {
            QJsonDocument doc = QJsonDocument::fromJson(repLegacy->readAll());
            QJsonObject root = doc.object();
            if (root.contains("embedding") && root["embedding"].isArray()) {
                QJsonArray vectorArray = root["embedding"].toArray();
                resultado.reserve(vectorArray.size());
                for (const QJsonValue &v : vectorArray) {
                    resultado.append(static_cast<float>(v.toDouble()));
                }
            }
        }
        repLegacy->deleteLater();
        if (!resultado.isEmpty()) {
            normalizarL2(resultado);
        }
        return resultado;
    }

    reply->deleteLater();
    if (!resultado.isEmpty()) {
        normalizarL2(resultado);
    } else {
        qWarning() << "IndexadorEmbeddings::obtenerEmbedding: Fallo al obtener vector embedding de Ollama:"
                   << (timer.isActive() ? reply->errorString() : "Timeout agotado");
    }
    return resultado;
}

/* ========================================================================= */
/*                         BÚSQUEDA SEMÁNTICA VECTORIAL                      */
/* ========================================================================= */

/**
 * @brief Busca los artículos más afines semánticamente a la consulta.
 */
QList<ResultadoSimilitud> IndexadorEmbeddings::buscarArticulosSimilares(const QString &query, int topK, float umbralMinimo)
{
    QList<ResultadoSimilitud> resultados;
    if (query.trimmed().isEmpty()) return resultados;

    // Asegurar que la caché esté cargada en memoria
    if (totalEnCache() == 0) {
        cargarCache();
    }

    // Si el índice sigue vacío, no hacer petición innecesaria a Ollama ni búsqueda
    if (totalEnCache() == 0) {
        qWarning() << "IndexadorEmbeddings::buscarArticulosSimilares: Índice semántico en memoria vacío. Se omite búsqueda vectorial.";
        return resultados;
    }

    QVector<float> queryVec = obtenerEmbedding(query, true, 3000);
    if (queryVec.isEmpty()) {
        qWarning() << "IndexadorEmbeddings::buscarArticulosSimilares: No se pudo generar vector embedding para la consulta.";
        return resultados;
    }

    QMutexLocker locker(&m_mutexCache);
    for (const ItemEmbedding &item : m_cacheMemoria) {
        if (item.tipo != "articulo") continue;

        float score = productoEscalar(queryVec, item.vector);
        if (score >= umbralMinimo) {
            ResultadoSimilitud res;
            res.cod = item.cod;
            res.tipo = item.tipo;
            res.score = score;
            resultados.append(res);
        }
    }

    // Ordenar de mayor a menor puntuación
    std::sort(resultados.begin(), resultados.end(), [](const ResultadoSimilitud &a, const ResultadoSimilitud &b) {
        return a.score > b.score;
    });

    if (resultados.size() > topK) {
        resultados = resultados.mid(0, topK);
    }
    return resultados;
}

/**
 * @brief Identifica qué áreas de fitoterapia coinciden conceptualmente con la consulta.
 */
QList<ResultadoSimilitud> IndexadorEmbeddings::buscarAreasSimilares(const QString &query, float umbralMinimo)
{
    QList<ResultadoSimilitud> resultados;
    if (query.trimmed().isEmpty()) return resultados;

    if (totalEnCache() == 0) {
        cargarCache();
    }

    // Si el índice sigue vacío, no hacer petición innecesaria a Ollama ni búsqueda
    if (totalEnCache() == 0) {
        qWarning() << "IndexadorEmbeddings::buscarAreasSimilares: Índice semántico en memoria vacío. Se omite búsqueda de áreas.";
        return resultados;
    }

    QVector<float> queryVec = obtenerEmbedding(query, true, 3000);
    if (queryVec.isEmpty()) {
        qWarning() << "IndexadorEmbeddings::buscarAreasSimilares: No se pudo generar vector embedding para la consulta.";
        return resultados;
    }

    QMutexLocker locker(&m_mutexCache);
    for (const ItemEmbedding &item : m_cacheMemoria) {
        if (item.tipo != "area") continue;

        float score = productoEscalar(queryVec, item.vector);
        if (score >= umbralMinimo) {
            ResultadoSimilitud res;
            res.cod = item.cod;
            res.tipo = item.tipo;
            res.score = score;
            resultados.append(res);
        }
    }

    std::sort(resultados.begin(), resultados.end(), [](const ResultadoSimilitud &a, const ResultadoSimilitud &b) {
        return a.score > b.score;
    });

    return resultados;
}

/* ========================================================================= */
/*                         HILO DE TRABAJO (WORKER)                          */
/* ========================================================================= */

WorkerIndexador::WorkerIndexador(QObject *parent)
    : QThread(parent),
      m_cancelar(false),
      m_port(0)
{
}

void WorkerIndexador::solicitarCancelacion()
{
    QMutexLocker locker(&m_mutexCancel);
    m_cancelar = true;
}

void WorkerIndexador::configurarConexion(const QString &driver,
                                        const QString &host,
                                        int port,
                                        const QString &databaseName,
                                        const QString &userName,
                                        const QString &password,
                                        const QString &connectOptions)
{
    m_driver = driver;
    m_host = host;
    m_port = port;
    m_databaseName = databaseName;
    m_userName = userName;
    m_password = password;
    m_connectOptions = connectOptions;
}

void WorkerIndexador::run()
{
    m_cancelar = false;

    if (m_databaseName.isEmpty()) {
        emit finalizado(false, "No hay configuración de base de datos disponible para el hilo de indexación.");
        return;
    }

    // Conexión independiente y exclusiva para este hilo de trabajo
    const QString nombreConexion = "WorkerIdx_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
    bool finalizadoOk = false;
    QString mensajeFinal;

    {
        QSqlDatabase dbWorker = QSqlDatabase::addDatabase(m_driver.isEmpty() ? "QMYSQL" : m_driver, nombreConexion);
        dbWorker.setHostName(m_host);
        if (m_port > 0) {
            dbWorker.setPort(m_port);
        }
        dbWorker.setDatabaseName(m_databaseName);
        dbWorker.setUserName(m_userName);
        dbWorker.setPassword(m_password);
        if (!m_connectOptions.isEmpty()) {
            dbWorker.setConnectOptions(m_connectOptions);
        }

        if (!dbWorker.open()) {
            emit finalizado(false, "Error al abrir conexión SQL para el hilo de indexación: " + dbWorker.lastError().text());
            dbWorker = QSqlDatabase();
            QSqlDatabase::removeDatabase(nombreConexion);
            return;
        }

        IndexadorEmbeddings::asegurarTabla(dbWorker);

        emit progreso(0, 100, "Comprobando catálogo e índice existente...");

        // 1. Cargar hashes existentes para saltar elementos sin cambios (Indexación Incremental)
        QHash<QString, QString> hashesExistentes; // clave: "cod_tipo" -> hash
        {
            QSqlQuery qHash(dbWorker);
            if (qHash.exec("SELECT cod, tipo, texto_hash FROM ia_embeddings")) {
                while (qHash.next()) {
                    QString k = qHash.value("cod").toString() + "_" + qHash.value("tipo").toString();
                    hashesExistentes.insert(k, qHash.value("texto_hash").toString());
                }
            } else {
                qWarning() << "WorkerIndexador: Error al consultar hashes de ia_embeddings:" << qHash.lastError().text();
            }
        }

        // 2. Extraer artículos de la base de datos
        struct TareaItem {
            QString cod;
            QString tipo; // "articulo" o "area"
            QString textoIndexable;
            QString hash;
        };
        QList<TareaItem> itemsParaIndexar;
        int totalArticulosLeidos = 0;

        {
            QSqlQuery qArt(dbWorker);
            QString sqlArt = "SELECT a.cod, a.descripcion, "
                             "COALESCE(b.nombre, a.fabricante, '') AS fabricante_nom, "
                             "COALESCE(c.descripcion, a.familia, '') AS familia_nom, "
                             "a.formato, a.notas "
                             "FROM articulos a "
                             "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                             "LEFT JOIN familias c ON a.familia = c.id "
                             "WHERE a.descripcion IS NOT NULL AND TRIM(a.descripcion) != ''";

            if (!qArt.exec(sqlArt)) {
                qWarning() << "WorkerIndexador: Fallo en consulta avanzada de articulos:" << qArt.lastError().text()
                           << ". Intentando consulta básica...";
                sqlArt = "SELECT cod, descripcion, notas, formato FROM articulos WHERE descripcion IS NOT NULL AND TRIM(descripcion) != ''";
                if (!qArt.exec(sqlArt)) {
                    qCritical() << "WorkerIndexador: Error crítico al consultar articulos:" << qArt.lastError().text();
                    dbWorker.close();
                    emit finalizado(false, "Error al consultar la tabla articulos: " + qArt.lastError().text());
                    return;
                }
            }

            while (qArt.next()) {
                totalArticulosLeidos++;
                QString cod = qArt.value("cod").toString().trimmed();
                if (cod.isEmpty()) continue;

                QString descrip = qArt.value("descripcion").toString().trimmed();
                QString fabricante = qArt.value("fabricante_nom").toString().trimmed();
                QString familia = qArt.value("familia_nom").toString().trimmed();
                QString formato = qArt.value("formato").toString().trimmed();
                QString notas = qArt.value("notas").toString().trimmed();

                QStringList partesTexto;
                partesTexto << QString("Producto: %1").arg(descrip);
                if (!fabricante.isEmpty()) partesTexto << QString("Marca: %1").arg(fabricante);
                if (!familia.isEmpty()) partesTexto << QString("Familia: %1").arg(familia);
                if (!formato.isEmpty()) partesTexto << QString("Formato: %1").arg(formato);
                if (!notas.isEmpty()) partesTexto << QString("Información: %1").arg(notas);

                QString textoCompleto = partesTexto.join(". ");
                QString textoLimpio = IndexadorEmbeddings::limpiarTextoParaEmbedding(textoCompleto, 4000);
                if (textoLimpio.isEmpty()) continue;

                QString hash = QString::fromLatin1(QCryptographicHash::hash(textoLimpio.toUtf8(), QCryptographicHash::Sha256).toHex());
                QString k = cod + "_articulo";

                if (!hashesExistentes.contains(k) || hashesExistentes.value(k) != hash) {
                    TareaItem it;
                    it.cod = cod;
                    it.tipo = "articulo";
                    it.textoIndexable = textoLimpio;
                    it.hash = hash;
                    itemsParaIndexar.append(it);
                }
            }
        }

        // 3. Extraer áreas de conocimiento
        int totalAreasLeidas = 0;
        {
            QSqlQuery qArea(dbWorker);
            QString sqlArea = "SELECT id, categoria, detonantes, terminos_clave FROM ia_conocimiento WHERE activo = 1";
            if (!qArea.exec(sqlArea)) {
                qWarning() << "WorkerIndexador: Aviso al consultar ia_conocimiento:" << qArea.lastError().text();
            } else {
                while (qArea.next()) {
                    totalAreasLeidas++;
                    int areaId = qArea.value("id").toInt();
                    if (areaId <= 0) continue;

                    QString cod = QString("AREA_%1").arg(areaId);
                    QString categoria = qArea.value("categoria").toString().trimmed();
                    QString detonantes = qArea.value("detonantes").toString().trimmed();
                    QString terminos = qArea.value("terminos_clave").toString().trimmed();

                    QString texto = QString("Área de salud: %1. Síntomas detonantes: %2. Términos clave: %3")
                                        .arg(categoria, detonantes, terminos);
                    texto = IndexadorEmbeddings::limpiarTextoParaEmbedding(texto, 6000);
                    if (texto.isEmpty()) continue;

                    QString hash = QString::fromLatin1(QCryptographicHash::hash(texto.toUtf8(), QCryptographicHash::Sha256).toHex());
                    QString k = cod + "_area";

                    if (!hashesExistentes.contains(k) || hashesExistentes.value(k) != hash) {
                        TareaItem it;
                        it.cod = cod;
                        it.tipo = "area";
                        it.textoIndexable = texto;
                        it.hash = hash;
                        itemsParaIndexar.append(it);
                    }
                }
            }
        }

        int totalPendientes = itemsParaIndexar.size();
        if (totalPendientes == 0) {
            dbWorker.close();
            finalizadoOk = true;
            if (hashesExistentes.isEmpty() && totalArticulosLeidos == 0 && totalAreasLeidas == 0) {
                mensajeFinal = "No se encontraron artículos ni áreas de conocimiento en la base de datos para indexar.";
                emit progreso(0, 0, mensajeFinal);
            } else {
                mensajeFinal = QString("Índice al día. Todos los elementos (%1) ya estaban indexados y sin cambios.")
                                   .arg(hashesExistentes.size());
                emit progreso(100, 100, "El índice semántico ya está completamente actualizado.");
            }
        } else {
            emit progreso(0, totalPendientes, QString("Indexando %1 elementos modificados o nuevos...").arg(totalPendientes));

            QString urlOllama = IndexadorEmbeddings::obtenerUrlOllama();
            QString modelo = IndexadorEmbeddings::obtenerModeloEmbeddings();
            bool esSqlite = dbWorker.driverName().contains("SQLITE", Qt::CaseInsensitive);

            const int TAMANO_LOTE = 64;
            int procesados = 0;
            bool cancelado = false;

            QNetworkAccessManager nam;

            for (int i = 0; i < totalPendientes; i += TAMANO_LOTE) {
                {
                    QMutexLocker locker(&m_mutexCancel);
                    if (m_cancelar) {
                        cancelado = true;
                        break;
                    }
                }

                int fin = qMin(i + TAMANO_LOTE, totalPendientes);
                QJsonArray inputLote;
                for (int k = i; k < fin; ++k) {
                    inputLote.append("search_document: " + itemsParaIndexar[k].textoIndexable);
                }

                // Petición a /api/embed
                QNetworkRequest request(QUrl(urlOllama + "/api/embed"));
                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

                QJsonObject rootObj;
                rootObj["model"] = modelo;
                rootObj["input"] = inputLote;

                QEventLoop loop;
                QTimer timer;
                timer.setSingleShot(true);
                QNetworkReply *reply = nam.post(request, QJsonDocument(rootObj).toJson(QJsonDocument::Compact));

                connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
                connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);
                timer.start(45000); // 45 segundos para el lote de 64 en GPU/CPU

                loop.exec();

                if (timer.isActive()) {
                    timer.stop();
                } else {
                    reply->abort();
                    dbWorker.close();
                    finalizadoOk = false;
                    mensajeFinal = QString("Timeout al generar embeddings para el lote %1 - %2.").arg(i).arg(fin);
                    reply->deleteLater();
                    break;
                }

                if (reply->error() != QNetworkReply::NoError) {
                    QString errStr = reply->errorString();
                    QByteArray errBody = reply->readAll();
                    reply->deleteLater();
                    dbWorker.close();
                    finalizadoOk = false;

                    QJsonDocument docErr = QJsonDocument::fromJson(errBody);
                    QString detalleOllama = docErr.object().value("error").toString();
                    if (!detalleOllama.isEmpty()) {
                        mensajeFinal = QString("Error en Ollama (%1): %2").arg(modelo, detalleOllama);
                    } else {
                        mensajeFinal = QString("Error en Ollama al generar lote (%1): %2").arg(modelo, errStr);
                    }
                    qWarning() << "WorkerIndexador:" << mensajeFinal;
                    break;
                }

                QByteArray respData = reply->readAll();
                reply->deleteLater();

                QJsonDocument doc = QJsonDocument::fromJson(respData);
                QJsonArray embeddingsArr = doc.object().value("embeddings").toArray();

                if (embeddingsArr.size() != (fin - i)) {
                    dbWorker.close();
                    finalizadoOk = false;
                    mensajeFinal = QString("Respuesta inesperada de Ollama: se enviaron %1 textos y se recibieron %2 vectores.")
                                      .arg(fin - i).arg(embeddingsArr.size());
                    qWarning() << "WorkerIndexador:" << mensajeFinal;
                    break;
                }

                // Guardar el lote en base de datos usando transacción
                dbWorker.transaction();
                bool hayErrorUpsert = false;
                QString errorDetalleUpsert;
                for (int k = 0; k < embeddingsArr.size(); ++k) {
                    const TareaItem &tarea = itemsParaIndexar[i + k];
                    QJsonArray vArr = embeddingsArr[k].toArray();

                    QVector<float> vec;
                    vec.reserve(vArr.size());
                    for (const QJsonValue &val : vArr) {
                        vec.append(static_cast<float>(val.toDouble()));
                    }
                    IndexadorEmbeddings::normalizarL2(vec);

                    QByteArray blob;
                    blob.resize(static_cast<int>(vec.size() * sizeof(float)));
                    memcpy(blob.data(), vec.constData(), blob.size());

                    QSqlQuery qUpsert(dbWorker);
                    if (esSqlite) {
                        qUpsert.prepare("INSERT INTO ia_embeddings (cod, tipo, embedding, dimensiones, texto_hash, updated_at) "
                                        "VALUES (:cod, :tipo, :emb, :dim, :hash, :upd) "
                                        "ON CONFLICT(cod, tipo) DO UPDATE SET "
                                        "embedding = excluded.embedding, "
                                        "dimensiones = excluded.dimensiones, "
                                        "texto_hash = excluded.texto_hash, "
                                        "updated_at = excluded.updated_at");
                    } else {
                        qUpsert.prepare("INSERT INTO ia_embeddings (cod, tipo, embedding, dimensiones, texto_hash, updated_at) "
                                        "VALUES (:cod, :tipo, :emb, :dim, :hash, :upd) "
                                        "ON DUPLICATE KEY UPDATE "
                                        "embedding = VALUES(embedding), "
                                        "dimensiones = VALUES(dimensiones), "
                                        "texto_hash = VALUES(texto_hash), "
                                        "updated_at = VALUES(updated_at)");
                    }

                    qUpsert.bindValue(":cod", tarea.cod);
                    qUpsert.bindValue(":tipo", tarea.tipo);
                    qUpsert.bindValue(":emb", blob);
                    qUpsert.bindValue(":dim", vec.size());
                    qUpsert.bindValue(":hash", tarea.hash);
                    if (esSqlite) {
                        qUpsert.bindValue(":upd", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
                    } else {
                        qUpsert.bindValue(":upd", QDateTime::currentDateTime());
                    }

                    if (!qUpsert.exec()) {
                        errorDetalleUpsert = qUpsert.lastError().text();
                        qWarning() << "WorkerIndexador: Error al insertar embedding para" << tarea.cod << ":" << errorDetalleUpsert;
                        hayErrorUpsert = true;
                        break;
                    }
                }

                if (hayErrorUpsert) {
                    dbWorker.rollback();
                    dbWorker.close();
                    finalizadoOk = false;
                    mensajeFinal = QString("Error al guardar los embeddings en la base de datos: %1").arg(errorDetalleUpsert);
                    break;
                }

                if (!dbWorker.commit()) {
                    QString errCommit = dbWorker.lastError().text();
                    qCritical() << "WorkerIndexador: Error al confirmar commit de embeddings:" << errCommit;
                    dbWorker.rollback();
                    dbWorker.close();
                    finalizadoOk = false;
                    mensajeFinal = QString("Error al confirmar la transacción de embeddings en la base de datos: %1").arg(errCommit);
                    break;
                }

                procesados += (fin - i);
                emit progreso(procesados, totalPendientes, QString("Indexados %1 de %2 elementos...").arg(procesados).arg(totalPendientes));
            }

            if (cancelado) {
                dbWorker.close();
                finalizadoOk = false;
                mensajeFinal = "Indexación cancelada por el usuario.";
            } else if (procesados == totalPendientes) {
                dbWorker.close();
                finalizadoOk = true;
                mensajeFinal = QString("Indexación completada correctamente: %1 elementos procesados.").arg(procesados);
                emit progreso(totalPendientes, totalPendientes, "Índice semántico actualizado con éxito.");
            }
        }

        dbWorker.close();
    } // Aquí dbWorker se destruye por completo antes de removeDatabase

    QSqlDatabase::removeDatabase(nombreConexion);

    emit finalizado(finalizadoOk, mensajeFinal);
}

/* ========================================================================= */
/*                         GESTIÓN DEL HILO WORKER                           */
/* ========================================================================= */

void IndexadorEmbeddings::iniciarReindexacionAsync()
{
    if (estaIndexando()) return;

    // Obtener la conexión abierta en el hilo principal (priorizando local offline-first)
    QString connLocal = conf ? conf->getConexionLocal() : "DB";
    if (connLocal.isEmpty()) connLocal = "DB";
    QSqlDatabase dbGui;
    if (QSqlDatabase::contains(connLocal) && QSqlDatabase::database(connLocal).isOpen()) {
        dbGui = QSqlDatabase::database(connLocal);
    } else if (QSqlDatabase::contains("DB") && QSqlDatabase::database("DB").isOpen()) {
        dbGui = QSqlDatabase::database("DB");
    } else if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        dbGui = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    } else {
        dbGui = QSqlDatabase::database();
    }

    if (!dbGui.isOpen()) {
        emit indexacionFinalizada(false, "No hay conexión abierta a la base de datos en la aplicación.");
        return;
    }

    if (!m_worker) {
        m_worker = new WorkerIndexador(this);
        connect(m_worker, &WorkerIndexador::progreso, this, &IndexadorEmbeddings::progreso);
        connect(m_worker, &WorkerIndexador::finalizado, this, [this](bool exito, const QString &resumen) {
            if (exito) {
                // Al finalizar con éxito, recargar la memoria caché desde la base de datos en el hilo principal
                cargarCache(true);
            }
            emit indexacionFinalizada(exito, resumen);
        });
    }

    m_worker->configurarConexion(dbGui.driverName(),
                                 dbGui.hostName(),
                                 dbGui.port(),
                                 dbGui.databaseName(),
                                 dbGui.userName(),
                                 dbGui.password(),
                                 dbGui.connectOptions());

    m_worker->start();
}

void IndexadorEmbeddings::cancelarReindexacion()
{
    if (m_worker && m_worker->isRunning()) {
        m_worker->solicitarCancelacion();
        m_worker->wait(3000);
    }
}

bool IndexadorEmbeddings::estaIndexando() const
{
    return (m_worker && m_worker->isRunning());
}
