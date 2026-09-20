#include "enriquecedorfichasia.h"
#include "asistenteia.h"

#include <QCoreApplication>
#include <QSettings>
#include <QUrl>
#include <QUrlQuery>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QRegularExpression>
#include <QTextDocument>
#include <QDebug>
#include <algorithm>

namespace {
// Palabras clave en español para puntuar la relevancia de composición y dosificación
const QStringList KEYWORDS_COMPOSICION = {
    "composici", "ingrediente", "por cápsula", "por capsula",
    "por comprimido", "por dosis", "modo de empleo", "posología",
    "dosis diaria", "información nutricional", "lista de ingredientes",
    "inci", "principios activos"
};
}

/**
 * @brief Constructor de la clase EnriquecedorFichasIA.
 * @param parent Objeto padre en el árbol de Qt.
 */
EnriquecedorFichasIA::EnriquecedorFichasIA(QObject *parent)
    : QObject(parent),
      m_netManager(new QNetworkAccessManager(this)),
      m_cancelado(false),
      m_pasoActual(0),
      m_openFactsPendientes(0),
      m_serperPendientes(0),
      m_paginasPendientes(0)
{
    // Cargar configuración de Ollama desde tienda.ini
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("Ollama");
    m_baseUrlOllama = settings.value("url", "http://localhost:11434").toString().trimmed();
    if (m_baseUrlOllama.isEmpty()) {
        m_baseUrlOllama = "http://localhost:11434";
    }
    if (m_baseUrlOllama.endsWith("/")) {
        m_baseUrlOllama.chop(1);
    }
    settings.endGroup();

    // Obtener modelo de Ollama centralizado (vía AsistenteIA)
    m_modeloOllama = AsistenteIA::obtenerModeloCentralizado();

    // Cargar clave de Serper API
    m_apiKeySerper = obtenerClaveSerper();
}

EnriquecedorFichasIA::~EnriquecedorFichasIA()
{
    cancelar();
}

/**
 * @brief Obtiene la clave de Serper API guardada en tienda.ini o variables de entorno.
 */
QString EnriquecedorFichasIA::obtenerClaveSerper()
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);

    // Intentar primero en sección [Serper]
    settings.beginGroup("Serper");
    QString key = settings.value("apiKey", "").toString().trimmed();
    settings.endGroup();

    // Si no está, buscar en [IA]
    if (key.isEmpty()) {
        settings.beginGroup("IA");
        key = settings.value("serperKey", "").toString().trimmed();
        settings.endGroup();
    }

    // Fallback a variable de entorno SERPER_KEY
    if (key.isEmpty()) {
        key = qEnvironmentVariable("SERPER_KEY").trimmed();
    }

    return key;
}

/**
 * @brief Guarda la clave de Serper API en tienda.ini.
 */
void EnriquecedorFichasIA::guardarClaveSerper(const QString &apiKey)
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("Serper");
    settings.setValue("apiKey", apiKey.trimmed());
    settings.endGroup();
    settings.sync();
}

/**
 * @brief Cancela cualquier operación de red en curso.
 */
void EnriquecedorFichasIA::cancelar()
{
    m_cancelado = true;
}

/**
 * @brief Inicia el pipeline completo de enriquecimiento para un artículo.
 */
void EnriquecedorFichasIA::procesarArticulo(const QString &ean,
                                            const QString &descripcion,
                                            const QString &fabricante,
                                            const QString &familia,
                                            const QString &formato,
                                            const QString &notasPrevias)
{
    m_cancelado = false;
    m_ean = ean.trimmed();
    m_descripcion = descripcion.trimmed();
    m_fabricante = fabricante.trimmed();
    m_familia = familia.trimmed();
    m_formato = formato.trimmed();
    m_notasPrevias = notasPrevias.trimmed();
    m_fuentes.clear();
    m_pasoActual = 1;

    // Actualizar configuración en tiempo de ejecución
    m_modeloOllama = AsistenteIA::obtenerModeloCentralizado();
    m_apiKeySerper = obtenerClaveSerper();

    emit progreso(1, 5, tr("Consultando bases de datos de productos (Open Facts por EAN)..."));
    paso1_consultarOpenFacts();
}

/**
 * @brief Paso 1: Consulta Open Food Facts y Open Beauty Facts por EAN.
 */
void EnriquecedorFichasIA::paso1_consultarOpenFacts()
{
    if (m_cancelado) return;

    if (m_ean.isEmpty() || m_ean.length() < 8) {
        // Sin EAN válido, pasar directamente a la búsqueda web
        paso2_consultarSerper();
        return;
    }

    QStringList endpoints = {
        QString("https://world.openfoodfacts.org/api/v2/product/%1.json").arg(m_ean),
        QString("https://world.openbeautyfacts.org/api/v2/product/%1.json").arg(m_ean)
    };

    m_openFactsPendientes = endpoints.size();

    for (const QString &urlStr : endpoints) {
        QUrl url(urlStr);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::UserAgentHeader, "TiendaHerbolario/2.0 (Linux)");
        req.setTransferTimeout(10000); // 10 segundos timeout

        QNetworkReply *reply = m_netManager->get(req);
        bool esBeauty = urlStr.contains("openbeautyfacts");

        connect(reply, &QNetworkReply::finished, this, [this, reply, esBeauty]() {
            reply->deleteLater();
            m_openFactsPendientes--;

            if (reply->error() == QNetworkReply::NoError) {
                QByteArray data = reply->readAll();
                QJsonDocument doc = QJsonDocument::fromJson(data);
                if (doc.isObject()) {
                    QJsonObject root = doc.object();
                    if (root.value("status").toInt() == 1) {
                        QJsonObject prod = root.value("product").toObject();
                        QStringList campos;

                        auto agregarCampo = [&](const QString &clave, const QString &etiqueta) {
                            if (prod.contains(clave) && !prod.value(clave).toString().trimmed().isEmpty()) {
                                campos << QString("%1: %2").arg(etiqueta, prod.value(clave).toString().trimmed());
                            }
                        };

                        agregarCampo("product_name_es", "Nombre");
                        agregarCampo("product_name", "Nombre");
                        agregarCampo("generic_name_es", "Descripción genérica");
                        agregarCampo("brands", "Marca");
                        agregarCampo("quantity", "Cantidad");
                        agregarCampo("ingredients_text_es", "Ingredientes");
                        agregarCampo("ingredients_text", "Ingredientes");
                        agregarCampo("serving_size", "Porción/Dosis");
                        agregarCampo("allergens_text", "Alérgenos");

                        // Extraer cantidades detalladas y porcentajes de ingredientes si están estructurados
                        if (prod.contains("ingredients") && prod.value("ingredients").isArray()) {
                            QJsonArray ingsArr = prod.value("ingredients").toArray();
                            QStringList ingsDetallados;
                            for (const QJsonValue &iv : ingsArr) {
                                if (iv.isObject()) {
                                    QJsonObject io = iv.toObject();
                                    QString txt = io.value("text").toString().trimmed();
                                    QString q = io.value("quantity").toString().trimmed();
                                    if (q.isEmpty() && io.contains("percent_estimate")) {
                                        double pct = io.value("percent_estimate").toDouble();
                                        if (pct > 0.0) q = QString("%1%").arg(QString::number(pct, 'f', 1));
                                    }
                                    if (!txt.isEmpty()) {
                                        ingsDetallados << (q.isEmpty() ? txt : QString("%1 %2").arg(txt, q));
                                    }
                                }
                            }
                            if (!ingsDetallados.isEmpty()) {
                                campos << QString("Composición con cantidades por dosis: %1").arg(ingsDetallados.join(", "));
                            }
                        }

                        QString textoUnido = campos.join("\n");
                        if (textoUnido.length() >= 40) {
                            FuenteWeb f;
                            f.tipo = esBeauty ? "open_beauty_facts" : "open_food_facts";
                            f.url = QString("https://world.%1.org/product/%2")
                                        .arg(esBeauty ? "openbeautyfacts" : "openfoodfacts", m_ean);
                            f.titulo = prod.value("product_name").toString(QString("Open Facts %1").arg(m_ean));
                            f.texto = textoUnido;
                            f.oficial = false;
                            f.puntuacionKeywords = puntuacionKeywords(textoUnido);
                            m_fuentes.append(f);
                        }
                    }
                }
            }

            if (m_openFactsPendientes <= 0) {
                onOpenFactsTerminado();
            }
        });
    }
}

void EnriquecedorFichasIA::onOpenFactsTerminado()
{
    if (m_cancelado) return;
    paso2_consultarSerper();
}

/**
 * @brief Paso 2: Búsqueda en Google vía Serper API optimizada (1 sola consulta por producto para ahorrar créditos).
 */
void EnriquecedorFichasIA::paso2_consultarSerper()
{
    if (m_cancelado) return;

    if (m_apiKeySerper.isEmpty()) {
        qDebug() << "[EnriquecedorFichasIA] Sin SERPER_KEY, saltando búsqueda web.";
        paso3_descargarPaginasWeb();
        return;
    }

    emit progreso(2, 5, tr("Buscando fichas oficiales y composición en internet (Google Serper)..."));

    QString base = limpiarTextoDesc(m_descripcion);
    if (base.isEmpty()) base = m_descripcion;

    QString slugFab = slugMarca(m_fabricante);
    QStringList fmts = extraerTokensFormato(m_descripcion);
    if (fmts.isEmpty() && !m_formato.isEmpty()) {
        fmts = extraerTokensFormato(m_formato);
    }

    // Generar consulta unificada optimizada (1 sola búsqueda por producto = 1 crédito consumido)
    QString consulta = base;
    if (!m_fabricante.isEmpty() && m_fabricante != "0" && m_fabricante.toLower() != "fabricante desconocido") {
        consulta = QString("%1 %2").arg(m_fabricante, base);
    }
    if (!fmts.isEmpty()) {
        consulta += QString(" %1").arg(fmts.first());
    }
    consulta += " composición ingredientes dosis";
    consulta = consulta.simplified();

    m_serperPendientes = 1;

    QUrl url("https://google.serper.dev/search");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setRawHeader("X-API-KEY", m_apiKeySerper.toUtf8());
    req.setTransferTimeout(20000);

    QJsonObject body;
    body["q"] = consulta;
    body["gl"] = "es";
    body["hl"] = "es";
    body["num"] = 6;

    QByteArray jsonData = QJsonDocument(body).toJson();
    QNetworkReply *reply = m_netManager->post(req, jsonData);

    connect(reply, &QNetworkReply::finished, this, [this, reply, slugFab]() {
        reply->deleteLater();
        m_serperPendientes--;

        if (reply->error() == QNetworkReply::NoError) {
            QByteArray data = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(data);
            if (doc.isObject()) {
                QJsonArray organic = doc.object().value("organic").toArray();
                for (const QJsonValue &val : organic) {
                    QJsonObject item = val.toObject();
                    QString link = item.value("link").toString().trimmed();
                    QString title = item.value("title").toString().trimmed();
                    QString snippet = item.value("snippet").toString().trimmed();

                    if (link.isEmpty() || title.isEmpty()) continue;

                    // Evitar URLs duplicadas
                    bool yaExiste = false;
                    for (const FuenteWeb &f : m_fuentes) {
                        if (f.url.compare(link, Qt::CaseInsensitive) == 0) {
                            yaExiste = true;
                            break;
                        }
                    }
                    if (yaExiste) continue;

                    QUrl urlObj(link);
                    QString host = urlObj.host().toLower();
                    bool oficial = (!slugFab.isEmpty() && host.contains(slugFab));

                    FuenteWeb f;
                    f.url = link;
                    f.titulo = title;
                    f.snippet = snippet;
                    // f.texto se deja vacío para que paso3_descargarPaginasWeb descargue y limpie el HTML completo
                    f.texto = "";
                    f.tipo = oficial ? "oficial" : "web";
                    f.oficial = oficial;
                    f.puntuacionKeywords = puntuacionKeywords(snippet);
                    m_fuentes.append(f);
                }
            }
        } else {
            qDebug() << "[EnriquecedorFichasIA] Error Serper:" << reply->errorString();
        }

        if (m_serperPendientes <= 0) {
            onSerperTerminado();
        }
    });
}

void EnriquecedorFichasIA::onSerperTerminado()
{
    if (m_cancelado) return;
    paso3_descargarPaginasWeb();
}

/**
 * @brief Paso 3: Descarga y limpieza del contenido HTML de las fuentes más prometedoras.
 */
void EnriquecedorFichasIA::paso3_descargarPaginasWeb()
{
    if (m_cancelado) return;

    emit progreso(3, 5, tr("Descargando fichas técnicas y limpiando datos..."));

    // Ordenar fuentes dando prioridad a fichas oficiales y con mayor coincidencia de palabras clave
    std::sort(m_fuentes.begin(), m_fuentes.end(), [](const FuenteWeb &a, const FuenteWeb &b) {
        if (a.oficial != b.oficial) {
            return a.oficial > b.oficial;
        }
        return a.puntuacionKeywords > b.puntuacionKeywords;
    });

    // Seleccionar hasta 4 fuentes para descargar su texto completo
    QList<int> indicesParaDescargar;
    for (int i = 0; i < m_fuentes.size() && indicesParaDescargar.size() < 4; ++i) {
        if (m_fuentes[i].texto.isEmpty()) {
            indicesParaDescargar.append(i);
        }
    }

    if (indicesParaDescargar.isEmpty()) {
        paso4_extraerConOllama();
        return;
    }

    m_paginasPendientes = indicesParaDescargar.size();

    for (int idx : indicesParaDescargar) {
        QUrl url(m_fuentes[idx].url);
        QNetworkRequest req(url);
        req.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (X11; Linux x86_64; rv:126.0) Gecko/20100101 Firefox/126.0");
        req.setRawHeader("Accept-Language", "es-ES,es;q=0.9");
        req.setTransferTimeout(12000); // 12s máximo por web

        QNetworkReply *reply = m_netManager->get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply, idx]() {
            onPaginaWebDescargada(reply, idx);
        });
    }
}

void EnriquecedorFichasIA::onPaginaWebDescargada(QNetworkReply *reply, int indiceFuente)
{
    reply->deleteLater();
    m_paginasPendientes--;

    if (indiceFuente >= 0 && indiceFuente < m_fuentes.size()) {
        if (reply->error() == QNetworkReply::NoError) {
            QByteArray htmlData = reply->readAll();
            QString htmlStr = QString::fromUtf8(htmlData);
            QString textoLimpio = limpiarHtml(htmlStr);

            if (textoLimpio.length() > 50) {
                m_fuentes[indiceFuente].texto = textoLimpio;
                m_fuentes[indiceFuente].puntuacionKeywords = puntuacionKeywords(textoLimpio);
            } else if (!m_fuentes[indiceFuente].snippet.isEmpty()) {
                m_fuentes[indiceFuente].texto = m_fuentes[indiceFuente].titulo + "\n" + m_fuentes[indiceFuente].snippet;
                m_fuentes[indiceFuente].tipo += "_snippet";
            }
        } else {
            // Si la web no se puede descargar, usar el snippet de Google
            if (!m_fuentes[indiceFuente].snippet.isEmpty()) {
                m_fuentes[indiceFuente].texto = m_fuentes[indiceFuente].titulo + "\n" + m_fuentes[indiceFuente].snippet;
                m_fuentes[indiceFuente].tipo += "_snippet";
            }
        }
    }

    if (m_paginasPendientes <= 0) {
        if (!m_cancelado) {
            paso4_extraerConOllama();
        }
    }
}

/**
 * @brief Paso 4: Extracción rigurosa anti-alucinación con Ollama (JSON estricto, temperature=0).
 */
void EnriquecedorFichasIA::paso4_extraerConOllama()
{
    if (m_cancelado) return;

    emit progreso(4, 5, tr("Analizando composición y redactando ficha técnica con IA (%1)...").arg(m_modeloOllama));

    // Filtrar fuentes con texto
    QList<FuenteWeb> fuentesConTexto;
    for (const FuenteWeb &f : m_fuentes) {
        if (!f.texto.trimmed().isEmpty()) {
            fuentesConTexto.append(f);
        }
    }

    if (fuentesConTexto.isEmpty()) {
        ResultadoFicha res;
        res.estado = "error";
        res.confianza = "baja";
        res.error = tr("No se encontraron fuentes de información fiables en internet ni en Open Facts.");
        emit finalizado(false, res);
        return;
    }

    // Ordenar fuentes por tokens de formato y oficialidad
    QStringList fmts = extraerTokensFormato(m_descripcion);
    for (FuenteWeb &f : fuentesConTexto) {
        f.puntuacionFormato = 0;
        for (const QString &fmt : fmts) {
            if (f.texto.contains(fmt, Qt::CaseInsensitive)) {
                f.puntuacionFormato++;
            }
        }
    }

    std::sort(fuentesConTexto.begin(), fuentesConTexto.end(), [](const FuenteWeb &a, const FuenteWeb &b) {
        if (a.puntuacionFormato != b.puntuacionFormato) {
            return a.puntuacionFormato > b.puntuacionFormato;
        }
        if (a.oficial != b.oficial) {
            return a.oficial > b.oficial;
        }
        return a.puntuacionKeywords > b.puntuacionKeywords;
    });

    // Guardar las fuentes ordenadas finales
    m_fuentes = fuentesConTexto;

    // Construir texto de las fuentes para el prompt (máximo 16.000 caracteres)
    QStringList bloquesFuentes;
    int maxFuentes = qMin(fuentesConTexto.size(), 4);
    for (int i = 0; i < maxFuentes; ++i) {
        const FuenteWeb &f = fuentesConTexto[i];
        bloquesFuentes << QString("--- FUENTE %1 [%2] %3 ---\n%4")
                              .arg(QString::number(i + 1), f.tipo, f.url, f.texto);
    }
    QString textoFuentes = bloquesFuentes.join("\n\n");
    if (textoFuentes.length() > 16000) {
        textoFuentes = textoFuentes.left(16000) + "\n[...texto recortado por longitud...]";
    }

    // Prompt del sistema estricto anti-alucinación con exigencia de cantidades
    QString promptSistema =
        "Eres un extractor de datos farmacéutico y nutricional riguroso. Te doy textos reales descargados de internet "
        "(fichas de producto de un complemento alimenticio o cosmética de herbolario).\n"
        "DEBES extraer la información ÚNICAMENTE de ese texto. Reglas estrictas:\n"
        "- NUNCA inventes, infieras ni completes datos que no aparezcan literalmente en el texto.\n"
        "- Si un campo no aparece en el texto, devuelve null (no lo rellenes con tu conocimiento).\n"
        "- COMPOSICIÓN Y CANTIDADES (OBLIGATORIO Y PRIORITARIO):\n"
        "  * Incluye SIEMPRE las CANTIDADES, miligramos (mg), gramos (g), microgramos (mcg/µg), porcentajes (%) o unidades por dosis de cada ingrediente siempre que aparezcan en el texto descargado.\n"
        "  * Si el texto indica las cantidades por toma o dosis (ej: 'Por 2 cápsulas:', 'Por 1 vial:', 'Por 100g:', 'Por dosis diaria recomendada:'), INCLUYE SIEMPRE dicho encabezado con el desglose exacto de cantidades de cada principio activo.\n"
        "  * FORMATO DE COMPOSICIÓN (OBLIGATORIO): Coloca CADA INGREDIENTE EN UNA LÍNEA INDEPENDIENTE (un ingrediente por línea con su cantidad al lado, por ejemplo:\n"
        "    - Ingrediente A: 250 mg\n"
        "    - Ingrediente B: 100 mg\n"
        "    o similar con saltos de línea). NO los juntes en un solo párrafo continuo.\n"
        "  * INGREDIENTES SIN CANTIDAD: Cuando en el texto no se indique la cantidad de un ingrediente o se desconozca, pon ÚNICAMENTE el nombre del ingrediente sin ninguna cifra (ej: '- Aceite de oliva', '- Cera de abejas'). En NINGÚN caso escribas 'null', 'desconocido' ni ': null' junto al ingrediente.\n"
        "  * Si la fuente incluye tabla nutricional o lista de principios activos con sus dosis numéricas, extráela con sus cifras exactas. No pongas solo los nombres de las plantas si las cantidades numéricas están disponibles.\n"
        "- No uses conocimiento previo: si el texto no menciona un ingrediente o cantidad, no lo añadas.\n"
        "- Responde SIEMPRE en español.\n"
        "- Devuelve exclusivamente un objeto JSON válido, sin comentarios ni texto extra.";

    QString schemaHint =
        "\nFormato JSON obligatorio de salida:\n"
        "{\n"
        "  \"producto_detectado\": \"nombre del producto tal como aparece en el texto, o null\",\n"
        "  \"coincide\": true,\n"
        "  \"indicacion\": \"qué es, propiedades y para qué sirve según el texto (máx 2-3 frases), o null\",\n"
        "  \"composicion\": \"LISTA DE INGREDIENTES CON UN INGREDIENTE POR LÍNEA y sus cantidades exactas si constan (sin poner 'null' si falta cantidad), o null\",\n"
        "  \"posologia\": \"cómo se toma/usa, dosis diaria recomendada y pauta de administración, o null\",\n"
        "  \"contraindicaciones\": \"contraindicaciones, advertencias especiales, embarazo/lactancia o alérgenos si los tuviese, o null\"\n"
        "}";

    QString promptUsuario = QString(
        "Producto buscado en el ERP de la herboristería: \"%1\" (fabricante: %2).\n"
        "Formato/medida indicado en el ERP: %3.\n\n"
        "Texto de las fuentes descargadas:\n\n%4\n\n"
        "Extrae los campos. REQUISITOS CLAVE:\n"
        "1) Usa SOLO el texto de las fuentes descargadas.\n"
        "2) composicion DEBE incluir obligatoriamente las CANTIDADES numéricas (mg, g, %, mcg) cuando estén disponibles y formatearse con UN INGREDIENTE POR LÍNEA. Si de un ingrediente no se indica cantidad en el texto, pon solo su nombre (ej: '- Extracto de gayuba') y NUNCA escribas la palabra 'null'.\n"
        "3) COMPRUEBA EL FORMATO: si la fuente describe otra presentación distinta (ej. perlas en vez de aceite líquido o número de unidades diferente), pon coincide=false.\n"
        "4) Si trata de otro producto, indícalo en producto_detectado y coincide=false.\n%5")
        .arg(m_descripcion,
             m_fabricante.isEmpty() ? "desconocido" : m_fabricante,
             fmts.isEmpty() ? "no indicado" : fmts.join(", "),
             textoFuentes,
             schemaHint);

    QJsonObject msgSystem;
    msgSystem["role"] = "system";
    msgSystem["content"] = promptSistema;

    QJsonObject msgUser;
    msgUser["role"] = "user";
    msgUser["content"] = promptUsuario;

    QJsonArray messages;
    messages.append(msgSystem);
    messages.append(msgUser);

    QJsonObject payload;
    payload["model"] = m_modeloOllama;
    payload["messages"] = messages;
    payload["stream"] = false;
    payload["format"] = "json";
    payload["keep_alive"] = "24h";

    QJsonObject options;
    options["temperature"] = 0.0; // Determinista, cero creatividad
    options["num_predict"] = 1500;
    options["num_ctx"] = AsistenteIA::obtenerNumCtxCentralizado();
    payload["options"] = options;

    QUrl url(m_baseUrlOllama + "/api/chat");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setTransferTimeout(180000); // 3 minutos timeout

    QNetworkReply *reply = m_netManager->post(req, QJsonDocument(payload).toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onOllamaTerminado(reply);
    });
}

void EnriquecedorFichasIA::onOllamaTerminado(QNetworkReply *reply)
{
    reply->deleteLater();

    if (m_cancelado) return;

    if (reply->error() != QNetworkReply::NoError) {
        ResultadoFicha res;
        res.estado = "error";
        res.confianza = "baja";
        res.error = tr("Error de conexión con Ollama (%1): %2").arg(m_baseUrlOllama, reply->errorString());
        emit finalizado(false, res);
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        ResultadoFicha res;
        res.estado = "error";
        res.error = tr("Respuesta inválida devuelta por Ollama.");
        emit finalizado(false, res);
        return;
    }

    QJsonObject root = doc.object();
    QString contenidoRaw;
    if (root.contains("message")) {
        contenidoRaw = root.value("message").toObject().value("content").toString().trimmed();
    } else if (root.contains("response")) {
        contenidoRaw = root.value("response").toString().trimmed();
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(contenidoRaw.toUtf8());
    if (!jsonDoc.isObject()) {
        // Si viene envuelto en markdown ```json ... ```
        QRegularExpression jsonBlockRe("```(?:json)?\\s*([\\s\\S]*?)\\s*```");
        QRegularExpressionMatch match = jsonBlockRe.match(contenidoRaw);
        if (match.hasMatch()) {
            jsonDoc = QJsonDocument::fromJson(match.captured(1).toUtf8());
        }
    }

    if (!jsonDoc.isObject()) {
        ResultadoFicha res;
        res.estado = "error";
        res.error = tr("La IA no devolvió un JSON estructurado válido:\n%1").arg(contenidoRaw);
        emit finalizado(false, res);
        return;
    }

    paso5_validarYFinalizar(jsonDoc.object());
}

/**
 * @brief Paso 5: Validación léxica algorítmica (token overlap) y formateo final.
 */
void EnriquecedorFichasIA::paso5_validarYFinalizar(const QJsonObject &jsonExtraccion)
{
    emit progreso(5, 5, tr("Validando exactitud de ingredientes y emitiendo ficha..."));

    ResultadoFicha res;
    res.productoDetectado = jsonExtraccion.value("producto_detectado").toString().trimmed();
    res.coincide = jsonExtraccion.value("coincide").toBool(false);
    res.composicion = jsonExtraccion.value("composicion").toString().trimmed();
    if (res.composicion.compare("null", Qt::CaseInsensitive) == 0) {
        res.composicion.clear();
    } else {
        // Limpiar cualquier residuo de 'null' generado para ingredientes sin cantidad
        res.composicion.replace(QRegularExpression(R"([:\s-]+\bnull\b(\s*(?:mg|g|mcg|µg|ml|%))?)", QRegularExpression::CaseInsensitiveOption), "");
        res.composicion.replace(QRegularExpression(R"(\(\s*null\s*\))", QRegularExpression::CaseInsensitiveOption), "");
        res.composicion.replace(QRegularExpression(R"(\bnull\b)", QRegularExpression::CaseInsensitiveOption), "");
        res.composicion.replace(QRegularExpression(R"(:\s*(\n|$))"), "\n");
        res.composicion = res.composicion.trimmed();
    }
    res.descripcion = jsonExtraccion.contains("indicacion") ? jsonExtraccion.value("indicacion").toString().trimmed()
                                                            : jsonExtraccion.value("descripcion").toString().trimmed();
    res.modoEmpleo = jsonExtraccion.contains("posologia") ? jsonExtraccion.value("posologia").toString().trimmed()
                                                          : jsonExtraccion.value("modo_empleo").toString().trimmed();
    res.advertencias = jsonExtraccion.contains("contraindicaciones") ? jsonExtraccion.value("contraindicaciones").toString().trimmed()
                                                                    : jsonExtraccion.value("advertencias").toString().trimmed();
    res.fuentes = m_fuentes;

    // Extraer textos de las fuentes
    QStringList textosFuentes;
    bool hayOficial = false;
    for (const FuenteWeb &f : m_fuentes) {
        if (!f.texto.isEmpty()) {
            textosFuentes << f.texto;
        }
        if (f.oficial) {
            hayOficial = true;
        }
    }

    // Calcular solapamiento léxico de tokens
    res.exactitud = calcularSolapamientoTokens(res.composicion, textosFuentes);

    // Determinar estado de verificación
    if (m_fuentes.isEmpty() || res.composicion.isEmpty() || !res.coincide || res.productoDetectado.isEmpty()) {
        res.estado = "revisar";
        res.confianza = "baja";
    } else if (res.exactitud < 0.60) {
        // Si menos del 60% de los tokens de composición aparecen en las fuentes, marcar para revisión humana
        res.estado = "revisar";
        res.confianza = "baja";
    } else {
        res.estado = "ok";
        res.confianza = hayOficial ? "alta" : "media";
    }

    // Formatear en HTML estructurado
    res.htmlFormateado = formatearResultadoHtml(res);

    emit finalizado(true, res);
}

/**
 * @brief Extrae tokens de medidas o formatos (ej: 100ml, 230 comp, 14 viales).
 */
QStringList EnriquecedorFichasIA::extraerTokensFormato(const QString &texto) const
{
    QStringList tokens;
    QRegularExpression re(
        "\\b\\d+\\s*(?:ml|l|cl|gr|g|kg|mg|t|tabs|caps?|cápsulas?|comps?|comprimidos?|uds|sobres?|viales?|ampollas?|perlas?)\\b",
        QRegularExpression::CaseInsensitiveOption);

    QRegularExpressionMatchIterator it = re.globalMatch(texto);
    while (it.hasNext()) {
        tokens << it.next().captured(0).trimmed();
    }
    return tokens;
}

QString EnriquecedorFichasIA::limpiarTextoDesc(const QString &desc) const
{
    QString d = desc;
    QRegularExpression reUnit("\\b\\d+\\s*(?:ml|gr|g|mg|t|tabs|cap|comp|uds)\\b", QRegularExpression::CaseInsensitiveOption);
    d.replace(reUnit, "");
    d = d.simplified();
    return d;
}

QString EnriquecedorFichasIA::slugMarca(const QString &marca) const
{
    QString s = marca.toLower();
    s.replace(QRegularExpression("[^a-z0-9]+"), "");
    return s;
}

int EnriquecedorFichasIA::puntuacionKeywords(const QString &texto) const
{
    QString t = texto.toLower();
    int score = 0;
    for (const QString &k : KEYWORDS_COMPOSICION) {
        if (t.contains(k)) {
            score++;
        }
    }
    return score;
}

/**
 * @brief Limpia el HTML eliminando scripts, estilos y etiquetas, retornando texto plano limpio.
 */
QString EnriquecedorFichasIA::limpiarHtml(const QString &htmlBruto) const
{
    QString h = htmlBruto;

    // Eliminar etiquetas de script, style, svg, noscript y comentarios HTML
    h.replace(QRegularExpression("<script[\\s\\S]*?</script>", QRegularExpression::CaseInsensitiveOption), " ");
    h.replace(QRegularExpression("<style[\\s\\S]*?</style>", QRegularExpression::CaseInsensitiveOption), " ");
    h.replace(QRegularExpression("<svg[\\s\\S]*?</svg>", QRegularExpression::CaseInsensitiveOption), " ");
    h.replace(QRegularExpression("<noscript[\\s\\S]*?</noscript>", QRegularExpression::CaseInsensitiveOption), " ");
    h.replace(QRegularExpression("<!--[\\s\\S]*?-->"), " ");

    // Preservar estructura de tablas y listas para que las cantidades no se peguen a los ingredientes
    h.replace(QRegularExpression("<tr[^>]*>", QRegularExpression::CaseInsensitiveOption), "\n");
    h.replace(QRegularExpression("</t[dh]>", QRegularExpression::CaseInsensitiveOption), " : ");
    h.replace(QRegularExpression("<br\\s*/?>", QRegularExpression::CaseInsensitiveOption), "\n");
    h.replace(QRegularExpression("</p>", QRegularExpression::CaseInsensitiveOption), "\n\n");
    h.replace(QRegularExpression("</li>", QRegularExpression::CaseInsensitiveOption), "\n");

    // Convertir a texto plano utilizando QTextDocument
    QTextDocument doc;
    doc.setHtml(h);
    QString texto = doc.toPlainText();

    // Normalizar saltos de línea y espacios repetitivos
    texto.replace(QRegularExpression("[ \\t]+"), " ");
    texto.replace(QRegularExpression("\\n{3,}"), "\n\n");
    texto = texto.trimmed();

    if (texto.length() > 14000) {
        texto = texto.left(14000);
    }
    return texto;
}

/**
 * @brief Calcula el solapamiento léxico de tokens de la composición respecto a las fuentes.
 */
double EnriquecedorFichasIA::calcularSolapamientoTokens(const QString &composicion, const QStringList &textosFuentes) const
{
    if (composicion.isEmpty()) return 0.0;

    QRegularExpression reTokens("[a-záéíóúüñ0-9.,/%-]+", QRegularExpression::CaseInsensitiveOption);

    // Extraer tokens significativos de la composición (longitud >= 3)
    QSet<QString> tokensComposicion;
    QRegularExpressionMatchIterator itComp = reTokens.globalMatch(composicion.toLower());
    while (itComp.hasNext()) {
        QString tok = itComp.next().captured(0);
        if (tok.length() >= 3) {
            tokensComposicion.insert(tok);
        }
    }

    if (tokensComposicion.isEmpty()) return 1.0;

    // Extraer todos los tokens de las fuentes
    QSet<QString> tokensFuentes;
    for (const QString &txt : textosFuentes) {
        QRegularExpressionMatchIterator itSrc = reTokens.globalMatch(txt.toLower());
        while (itSrc.hasNext()) {
            tokensFuentes.insert(itSrc.next().captured(0));
        }
    }

    int encontrados = 0;
    for (const QString &tok : tokensComposicion) {
        if (tokensFuentes.contains(tok)) {
            encontrados++;
        }
    }

    return static_cast<double>(encontrados) / static_cast<double>(tokensComposicion.size());
}

/**
 * @brief Genera el bloque formateado en HTML para visualización y guardado en articulos.notas.
 */
QString EnriquecedorFichasIA::formatearResultadoHtml(const ResultadoFicha &res) const
{
    QString html;

    // 1. Indicación
    if (!res.descripcion.isEmpty() && res.descripcion != "null") {
        QString ind = res.descripcion.trimmed();
        ind.replace("\r\n", "\n");
        if (!ind.contains("<br", Qt::CaseInsensitive) && !ind.contains("<p", Qt::CaseInsensitive)) {
            ind.replace("\n", "<br>");
        }
        html += QString("<b>Indicación:</b><br>%1<br><br>").arg(ind);
    }

    // 2. Composición (un ingrediente por línea)
    if (!res.composicion.isEmpty() && res.composicion.compare("null", Qt::CaseInsensitive) != 0) {
        QString comp = res.composicion.trimmed();
        comp.replace("\r\n", "\n");
        // Asegurar limpieza de cualquier 'null' residual o dos puntos sueltos
        comp.replace(QRegularExpression(R"([:\s-]+\bnull\b(\s*(?:mg|g|mcg|µg|ml|%))?)", QRegularExpression::CaseInsensitiveOption), "");
        comp.replace(QRegularExpression(R"(\(\s*null\s*\))", QRegularExpression::CaseInsensitiveOption), "");
        comp.replace(QRegularExpression(R"(\bnull\b)", QRegularExpression::CaseInsensitiveOption), "");

        // Asegurar que cada ingrediente tenga su salto de línea <br> en HTML
        if (!comp.contains("<br", Qt::CaseInsensitive) && !comp.contains("<p", Qt::CaseInsensitive) && !comp.contains("<li", Qt::CaseInsensitive)) {
            QStringList lineas = comp.split('\n', Qt::SkipEmptyParts);
            for (QString &l : lineas) {
                l = l.trimmed();
                if (l.endsWith(':')) l.chop(1);
                l = l.trimmed();
            }
            comp = lineas.join("<br>");
        }
        html += QString("<b>Composición:</b><br>%1<br><br>").arg(comp);
    }

    // 3. Posología
    if (!res.modoEmpleo.isEmpty() && res.modoEmpleo != "null") {
        QString pos = res.modoEmpleo.trimmed();
        pos.replace("\r\n", "\n");
        if (!pos.contains("<br", Qt::CaseInsensitive) && !pos.contains("<p", Qt::CaseInsensitive)) {
            pos.replace("\n", "<br>");
        }
        html += QString("<b>Posología:</b><br>%1<br><br>").arg(pos);
    }

    // 4. Contraindicaciones (si las tuviese)
    if (!res.advertencias.isEmpty() && res.advertencias != "null") {
        QString contra = res.advertencias.trimmed();
        if (!contra.isEmpty() && contra.compare("null", Qt::CaseInsensitive) != 0) {
            contra.replace("\r\n", "\n");
            if (!contra.contains("<br", Qt::CaseInsensitive) && !contra.contains("<p", Qt::CaseInsensitive)) {
                contra.replace("\n", "<br>");
            }
            html += QString("<b>Contraindicaciones:</b><br>%1<br><br>").arg(contra);
        }
    }

    // NOTA: Las fuentes consultadas se preservan en el atributo res.fuentes para que la
    // interfaz de usuario las muestre aparte de forma interactiva, sin añadirlas al texto de la ficha.

    return html;
}
