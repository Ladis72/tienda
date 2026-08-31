#include "dialoggenerardescripcionia.h"
#include "ui_dialoggenerardescripcionia.h"

#include <QApplication>
#include <QClipboard>
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
                                                       QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogGenerarDescripcionIA),
      m_netManager(new QNetworkAccessManager(this)),
      m_nombreProducto(descripcion.trimmed()),
      m_fabricante(fabricante.trimmed()),
      m_familia(familia.trimmed()),
      m_formato(formato.trimmed()),
      m_ean(ean.trimmed()),
      m_buscando(false)
{
    ui->setupUi(this);

    // Cargar configuración de Ollama desde tienda.ini
    QSettings settings("tienda.ini", QSettings::IniFormat);
    settings.beginGroup("Ollama");
    m_baseUrlOllama = settings.value("url", "http://localhost:11434").toString().trimmed();
    if (m_baseUrlOllama.endsWith("/")) {
        m_baseUrlOllama.chop(1);
    }
    m_modeloOllama = settings.value("modelo", "llama3.1:8b").toString().trimmed();
    settings.endGroup();

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
    ui->labelEstado->setText(tr("🔍 Buscando información del producto en internet..."));

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
    // Extraer snippets de clases result__snippet de DuckDuckGo
    QRegularExpression reSnippet("<(?:a|div)[^>]*class=\"[^\"]*result__snippet[^\"]*\"[^>]*>(.*?)</(?:a|div)>",
                                 QRegularExpression::DotMatchesEverythingOption);
    QRegularExpressionMatchIterator it = reSnippet.globalMatch(html);

    int count = 0;
    while (it.hasNext() && count < 6) {
        QRegularExpressionMatch match = it.next();
        QString snippet = match.captured(1);

        // Limpiar etiquetas HTML internas
        snippet.remove(QRegularExpression("<[^>]*>"));
        snippet = snippet.simplified();

        if (!snippet.isEmpty() && snippet.length() > 20) {
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

    // Prompt de sistema que fija estrictamente el formato tradicional de las fichas de productos
    QString promptSistema =
        "Eres un especialista farmacéutico, herborista y redactor técnico de productos de dietética y parafarmacia.\n"
        "Tu misión es redactar una ficha técnica clara, profesional y estructurada para el producto indicado.\n\n"
        "DEBES estructurar OBLIGATORIAMENTE la ficha con las siguientes 4 secciones:\n\n"
        "Indicación:\n"
        "[Explica para qué sirve el producto, sus propiedades activas, beneficios y a quién va destinado]\n\n"
        "Precauciones:\n"
        "[Indica advertencias, contraindicaciones, embarazo o 'No se han descrito.' si no hay especiales]\n\n"
        "Composición:\n"
        "[Lista de ingredientes activos, extractos de plantas, vitaminas o componentes]\n\n"
        "Posología:\n"
        "[Modo de empleo, dosis diaria recomendada y forma de administración o aplicación]\n\n"
        "REGLAS CRÍTICAS:\n"
        "- NO incluyas introducciones, títulos generales, saludos ni despedidas.\n"
        "- Responde directamente comenzando con 'Indicación:'.\n"
        "- Mantén una redacción profesional, en español neutro de España.";

    // Mensaje de usuario con los datos recopilados
    QString promptUsuario = QString("PRODUCTO: %1\n").arg(m_nombreProducto);
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido") {
        promptUsuario += QString("MARCA / FABRICANTE: %1\n").arg(m_fabricante);
    }
    if (!m_familia.isEmpty() && m_familia != "Sin famila asignada") {
        promptUsuario += QString("FAMILIA / CATEGORÍA: %1\n").arg(m_familia);
    }
    if (!m_formato.isEmpty()) {
        promptUsuario += QString("PRESENTACIÓN: %1\n").arg(m_formato);
    }
    if (!contextoInternet.isEmpty()) {
        promptUsuario += QString("\nINFORMACIÓN ENCONTRADA EN INTERNET:\n%1\n").arg(contextoInternet);
    }

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

    QJsonObject options;
    options["temperature"] = 0.3; // Baja temperatura para mayor fidelidad técnica
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
            linea.startsWith("Composición", Qt::CaseInsensitive) ||
            linea.startsWith("Composicion", Qt::CaseInsensitive) ||
            linea.startsWith("Posología", Qt::CaseInsensitive) ||
            linea.startsWith("Posologia", Qt::CaseInsensitive) ||
            linea.startsWith("Modo de empleo", Qt::CaseInsensitive))
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
