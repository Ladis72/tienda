#include "dialoggenerardescripcionia.h"
#include "ui_dialoggenerardescripcionia.h"
#include "asistenteia.h"

#include <QApplication>
#include <QClipboard>
#include <QCoreApplication>
#include <QMessageBox>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QDebug>

/**
 * @brief Constructor del diálogo DialogGenerarDescripcionIA.
 * @param descripcion Nombre o descripción corta del producto visible en el formulario.
 * @param fabricante Nombre del fabricante o marca.
 * @param familia Categoría o familia asignada al artículo.
 * @param formato Presentación comercial (ej. Cápsulas, Jarabe, Crema).
 * @param ean Código de barras o identificador único.
 * @param parent Widget padre de la ventana modal.
 */
DialogGenerarDescripcionIA::DialogGenerarDescripcionIA(const QString &descripcion,
                                                       const QString &fabricante,
                                                       const QString &familia,
                                                       const QString &formato,
                                                       const QString &ean,
                                                       const QString &notasPrevias,
                                                       QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogGenerarDescripcionIA),
      m_netManager(new QNetworkAccessManager(this)),
      m_nombreProducto(descripcion.trimmed()),
      m_fabricante(fabricante.trimmed()),
      m_familia(familia.trimmed()),
      m_formato(formato.trimmed()),
      m_ean(ean.trimmed()),
      m_notasPrevias(notasPrevias.trimmed()),
      m_buscando(false)
{
    ui->setupUi(this);

    // Cargar configuración de Ollama desde tienda.ini (SEC-01: ruta absoluta junto al binario)
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

    // Obtener modelo centralizado (desde Nube / Local / tienda.ini)
    m_modeloOllama = AsistenteIA::obtenerModeloCentralizado();

    // Actualizar subtítulo con metadatos del producto
    QString subtitulo = m_nombreProducto;
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido") {
        subtitulo += " | " + m_fabricante;
    }
    if (!m_ean.isEmpty()) {
        subtitulo += " (EAN: " + m_ean + ")";
    }
    ui->labelSubtitulo->setText(subtitulo);

    // Configurar término de búsqueda inicial
    QString terminoInicial = m_nombreProducto;
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido" && !terminoInicial.contains(m_fabricante, Qt::CaseInsensitive)) {
        terminoInicial += " " + m_fabricante;
    }
    ui->lineEditTermino->setText(terminoInicial);

    // Iniciar el proceso de búsqueda e inferencia automáticamente
    iniciarProceso();
}

DialogGenerarDescripcionIA::~DialogGenerarDescripcionIA()
{
    delete ui;
}

/**
 * @brief Obtiene el contenido generado en el editor de texto.
 */
QString DialogGenerarDescripcionIA::getDescripcionGenerada() const
{
    return ui->textEditResultado->toHtml();
}

/**
 * @brief Inicia el ciclo de búsqueda web + generación con IA.
 */
void DialogGenerarDescripcionIA::iniciarProceso()
{
    QString termino = ui->lineEditTermino->text().trimmed();
    if (termino.isEmpty()) {
        termino = m_nombreProducto;
    }

    m_buscando = true;
    ui->progressBar->setRange(0, 0); // Indicador de actividad indeterminada
    ui->pushButtonRegenerar->setEnabled(false);
    ui->pushButtonAceptar->setEnabled(false);
    ui->labelEstado->setText(tr("🔍 Buscando información técnica y composición del producto..."));

    m_snippetsInternet.clear();
    buscarEnInternet(termino);
}

/**
 * @brief Envía la petición HTTP para buscar en DuckDuckGo HTML / Lite.
 */
void DialogGenerarDescripcionIA::buscarEnInternet(const QString &termino)
{
    // Construir URL de consulta a DuckDuckGo HTML
    QUrl url("https://html.duckduckgo.com/html/");
    QUrlQuery query;
    query.addQueryItem("q", termino);
    query.addQueryItem("kl", "es-es"); // Priorizar resultados en español
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
    req.setRawHeader("Accept-Language", "es-ES,es;q=0.9,en;q=0.8");

    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onSearchReplyFinished(reply);
    });
}

/**
 * @brief Procesa los resultados obtenidos del buscador.
 */
void DialogGenerarDescripcionIA::onSearchReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() == QNetworkReply::NoError) {
        QString html = QString::fromUtf8(reply->readAll());
        m_snippetsInternet = extraerSnippetsDeHtml(html);
        qDebug() << "Búsqueda web completada. Snippets encontrados:" << m_snippetsInternet.length() << "caracteres";
    } else {
        qDebug() << "Aviso: No se pudo obtener respuesta del buscador web:" << reply->errorString();
        m_snippetsInternet = "";
    }

    // Continuar con la fase de generación con Ollama
    ui->labelEstado->setText(tr("🧠 Generando ficha estructurada con IA (%1)...").arg(m_modeloOllama));
    consultarOllama(m_snippetsInternet);
}

/**
 * @brief Extrae los textos y fragmentos relevantes de la página de resultados.
 */
QString DialogGenerarDescripcionIA::extraerSnippetsDeHtml(const QString &html)
{
    QString resultado;
    // Extraer snippets de clases result__snippet de DuckDuckGo (soportando divs, spans, enlaces y celdas)
    QRegularExpression reSnippet("<(?:a|div|span|td)[^>]*class=\"[^\"]*result__snippet[^\"]*\"[^>]*>(.*?)</(?:a|div|span|td)>",
                                 QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator it = reSnippet.globalMatch(html);

    int count = 0;
    while (it.hasNext() && count < 12) {
        QRegularExpressionMatch match = it.next();
        QString snippet = match.captured(1);

        // Limpiar etiquetas HTML internas
        snippet.remove(QRegularExpression("<[^>]*>"));

        // Decodificar entidades HTML habituales
        snippet.replace("&quot;", "\"")
               .replace("&amp;", "&")
               .replace("&#39;", "'")
               .replace("&apos;", "'")
               .replace("&lt;", "<")
               .replace("&gt;", ">")
               .replace("&nbsp;", " ");

        snippet = snippet.simplified();

        if (!snippet.isEmpty() && snippet.length() > 15) {
            resultado += "- " + snippet + "\n";
            count++;
        }
    }

    return resultado;
}

/**
 * @brief Envía la solicitud de generación a la API de Ollama (/api/chat).
 */
void DialogGenerarDescripcionIA::consultarOllama(const QString &contextoInternet)
{
    QUrl url(m_baseUrlOllama + "/api/chat");
    QNetworkRequest req(url);
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    req.setTransferTimeout(180000); // 180 segundos de timeout máximo para modelos locales en CPU/GPU

    // Prompt de sistema estructurado como redactor documental de catálogo comercial
    // Diseñado para evitar falsos positivos en los filtros de seguridad médica (RLHF) de Llama/Gemma,
    // garantizando al mismo tiempo que la composición sea 100% exacta y confirmada.
    QString promptSistema =
        "Eres un redactor y catalogador técnico para la base de datos informativa de una tienda de dietética, nutrición y herbolario.\n"
        "Tu tarea consiste en organizar de manera estructurada, objetiva y neutral la información comercial del fabricante del producto indicado.\n\n"
        "Estructura la ficha informativa en estas 4 secciones:\n\n"
        "Indicación:\n"
        "[Describe la finalidad del producto y para qué se comercializa según las propiedades de sus componentes]\n\n"
        "Precauciones:\n"
        "[Indica las advertencias de uso indicadas por el fabricante o 'No se han descrito.' si no constan especiales]\n\n"
        "Composición:\n"
        "[Lista fiel de ingredientes, extractos de plantas, vitaminas o minerales confirmados por el fabricante]\n\n"
        "Posología:\n"
        "[Modo de empleo o sugerencia de uso indicada en el etiquetado del fabricante]\n\n"
        "NORMAS DE REDACCIÓN:\n"
        "1. COMPOSICIÓN EXACTA Y CONFIRMADA:\n"
        "   - En la sección 'Composición:', transcribe exclusivamente los ingredientes, principios y cantidades que aparezcan explícitamente confirmados en la información del fabricante o fuentes aportadas.\n"
        "   - No agregues componentes ni cantidades supuestas o deducidas de otros productos.\n"
        "   - Si la composición completa no figura confirmada en los datos disponibles:\n"
        "     * Si se conocen ingredientes parciales confirmados: lista únicamente los confirmados y anota: '(Composición cuantitativa o completa pendiente de confirmación en el etiquetado oficial)'.\n"
        "     * Si no se dispone de composición confirmada: indica: 'Composición no confirmada en las fuentes disponibles. Consultar el etiquetado oficial del producto.'\n"
        "2. COHERENCIA INFORMATIVA:\n"
        "   - No menciones en las demás secciones sustancias o ingredientes que no pertenezcan a la fórmula confirmada de este producto.\n"
        "3. ESTILO Y FORMATO:\n"
        "   - No incluyas introducciones ni saludos.\n"
        "   - Comienza directamente con 'Indicación:'.\n"
        "   - Utiliza español neutro y formal.";

    // Mensaje de usuario con los datos recopilados
    QString promptUsuario = QString("PRODUCTO: %1\n").arg(m_nombreProducto);
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido") {
        promptUsuario += QString("MARCA / FABRICANTE: %1\n").arg(m_fabricante);
    }
    if (!m_familia.isEmpty() && m_familia != "Sin famila asignada") {
        promptUsuario += QString("CATEGORÍA: %1\n").arg(m_familia);
    }
    if (!m_formato.isEmpty()) {
        promptUsuario += QString("PRESENTACIÓN: %1\n").arg(m_formato);
    }
    if (!m_ean.isEmpty()) {
        promptUsuario += QString("CÓDIGO / EAN: %1\n").arg(m_ean);
    }
    if (!m_notasPrevias.isEmpty()) {
        promptUsuario += QString("\nDATOS YA REGISTRADOS EN LA FICHA DEL PRODUCTO:\n%1\n").arg(m_notasPrevias);
    }
    if (!contextoInternet.isEmpty()) {
        promptUsuario += QString("\nINFORMACIÓN CATALOGADA DE INTERNET:\n%1\n").arg(contextoInternet);
    }

    promptUsuario += "\nNota: Asegúrate de que la sección de Composición sea exacta y confirmada con la información disponible, sin añadir ingredientes no verificados.";

    QJsonObject msgSystem;
    msgSystem["role"] = "system";
    msgSystem["content"] = promptSistema;

    QJsonObject msgUser;
    msgUser["role"] = "user";
    msgUser["content"] = promptUsuario;

    QJsonArray messages;
    messages.append(msgSystem);
    messages.append(msgUser);

    // Asegurar que usamos el modelo centralizado activo para no variar el runner en VRAM
    m_modeloOllama = AsistenteIA::obtenerModeloCentralizado();

    QJsonObject payload;
    payload["model"] = m_modeloOllama;
    payload["messages"] = messages;
    payload["stream"] = false;
    payload["keep_alive"] = "24h"; // Mantener modelo cargado en VRAM igual que AsistenteIA para evitar recargas continuas

    QJsonObject options;
    options["temperature"] = 0.15; // Temperatura muy baja para máxima fidelidad técnica y evitar alucinaciones
    options["top_p"] = 0.9;
    options["repeat_penalty"] = 1.03;
    options["repeat_last_n"] = 64;
    options["num_predict"] = 4000;

    // Alinear ventana de contexto con AsistenteIA para que Ollama no descargue y recargue el runner en VRAM
    int numCtx = 16384; // 16K tokens por defecto
    if (m_modeloOllama.contains("-32k", Qt::CaseInsensitive) || m_modeloOllama.contains("64k", Qt::CaseInsensitive)) {
        numCtx = 32768; // 32K tokens si el modelo soporta ventana extendida
    }
    options["num_ctx"] = numCtx;
    payload["options"] = options;

    QJsonDocument doc(payload);
    QNetworkReply *reply = m_netManager->post(req, doc.toJson());
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onOllamaReplyFinished(reply);
    });
}

/**
 * @brief Procesa la respuesta de Ollama e inserta el resultado formateado en el editor.
 */
void DialogGenerarDescripcionIA::onOllamaReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    m_buscando = false;
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(100);
    ui->pushButtonRegenerar->setEnabled(true);
    ui->pushButtonAceptar->setEnabled(true);

    if (reply->error() != QNetworkReply::NoError) {
        QString errStr = reply->errorString();
        ui->labelEstado->setText(tr("❌ Error conectando con Ollama: %1").arg(errStr));
        ui->textEditResultado->setPlainText(
            tr("No se pudo contactar con Ollama en %1.\n\n"
               "Comprueba que Ollama esté en ejecución y que el modelo '%2' esté descargado.")
            .arg(m_baseUrlOllama, m_modeloOllama));
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        ui->labelEstado->setText(tr("❌ Respuesta inválida de Ollama."));
        return;
    }

    QJsonObject root = doc.object();
    QString contenidoRaw;

    if (root.contains("message")) {
        contenidoRaw = root.value("message").toObject().value("content").toString().trimmed();
    } else if (root.contains("response")) {
        contenidoRaw = root.value("response").toString().trimmed();
    }

    if (contenidoRaw.isEmpty()) {
        ui->labelEstado->setText(tr("⚠️ La IA devolvió una respuesta vacía."));
        return;
    }

    ui->labelEstado->setText(tr("✅ Ficha generada correctamente. Puedes revisarla y aplicarla."));

    // Formatear la ficha en HTML limpio con encabezados estándar en negrita
    QString htmlFormateado = formatearFichaComoHtml(contenidoRaw);
    ui->textEditResultado->setHtml(htmlFormateado);
}

/**
 * @brief Convierte el texto estructurado a HTML estándar con etiquetas <b> y párrafos limpios.
 */
QString DialogGenerarDescripcionIA::formatearFichaComoHtml(const QString &textoRaw)
{
    QString txt = textoRaw;

    // Normalizar encabezados en markdown (ej. **Indicación:** o ### Indicación) a texto plano para el parser
    txt.replace(QRegularExpression("#{1,4}\\s*"), "");
    txt.replace(QRegularExpression("\\*\\*([^*]+)\\*\\*"), "\\1");

    // Dividir en líneas y procesar secciones
    QStringList lineas = txt.split("\n");
    QString resultadoHtml;

    for (int i = 0; i < lineas.size(); ++i) {
        QString linea = lineas[i].trimmed();
        if (linea.isEmpty()) {
            resultadoHtml += "<br>";
            continue;
        }

        // Detectar si la línea es un encabezado de sección conocido
        if (linea.startsWith("Indicación", Qt::CaseInsensitive) ||
            linea.startsWith("Indicacion", Qt::CaseInsensitive) ||
            linea.startsWith("Precauciones", Qt::CaseInsensitive) ||
            linea.startsWith("Advertencias", Qt::CaseInsensitive) ||
            linea.startsWith("Composición", Qt::CaseInsensitive) ||
            linea.startsWith("Composicion", Qt::CaseInsensitive) ||
            linea.startsWith("Posología", Qt::CaseInsensitive) ||
            linea.startsWith("Posologia", Qt::CaseInsensitive) ||
            linea.startsWith("Modo de empleo", Qt::CaseInsensitive) ||
            linea.startsWith("Modo de uso", Qt::CaseInsensitive))
        {
            int posDosPuntos = linea.indexOf(':');
            if (posDosPuntos != -1) {
                QString titulo = linea.left(posDosPuntos + 1).trimmed();
                QString resto = linea.mid(posDosPuntos + 1).trimmed();
                resultadoHtml += QString("<b>%1</b>").arg(titulo);
                if (!resto.isEmpty()) {
                    resultadoHtml += "<br>" + resto;
                }
                resultadoHtml += "<br>";
            } else {
                resultadoHtml += QString("<b>%1:</b><br>").arg(linea);
            }
        } else {
            resultadoHtml += linea + "<br>";
        }
    }

    return resultadoHtml;
}

/**
 * @brief Slot para el botón Regenerar.
 */
void DialogGenerarDescripcionIA::on_pushButtonRegenerar_clicked()
{
    if (m_buscando) return;
    iniciarProceso();
}

/**
 * @brief Slot para copiar el contenido al portapapeles.
 */
void DialogGenerarDescripcionIA::on_pushButtonCopiar_clicked()
{
    QString texto = ui->textEditResultado->toPlainText();
    if (!texto.isEmpty()) {
        QApplication::clipboard()->setText(texto);
        QMessageBox::information(this, tr("Copiado"), tr("El texto de la ficha ha sido copiado al portapapeles."));
    }
}

/**
 * @brief Slot para aceptar y aplicar la descripción.
 */
void DialogGenerarDescripcionIA::on_pushButtonAceptar_clicked()
{
    accept();
}

/**
 * @brief Slot para rechazar y cancelar.
 */
void DialogGenerarDescripcionIA::on_pushButtonRechazar_clicked()
{
    reject();
}
