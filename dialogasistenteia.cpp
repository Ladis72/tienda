#include "dialogasistenteia.h"
#include "ui_dialogasistenteia.h"
#include "dialogconocimientoia.h"
#include "dialoglogsia.h"
#include "articulos.h"
#include "clientes.h"
#include "configuracion.h"
#include <QCoreApplication>
#include <QDate>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>
#include <QInputDialog>
#include <QUrl>
#include <QCompleter>
#include <QSqlQuery>
#include <QClipboard>
#include <QGuiApplication>

extern Configuracion *conf;

/**
 * @brief Constructor del diálogo DialogAsistenteIA.
 */
DialogAsistenteIA::DialogAsistenteIA(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogAsistenteIA),
      m_asistente(new AsistenteIA(this))
{
    ui->setupUi(this);

    // Configurar como ventana independiente estándar (no bloquea el TPV ni compite por foco)
    setWindowFlags(Qt::Window);

    // Conectar señales del motor AsistenteIA
    connect(m_asistente, &AsistenteIA::respuestaRecibida, this, &DialogAsistenteIA::slotRespuestaRecibida);
    connect(m_asistente, &AsistenteIA::estadoCambiado, this, &DialogAsistenteIA::slotEstadoCambiado);
    connect(m_asistente, &AsistenteIA::errorOcurrido, this, &DialogAsistenteIA::slotErrorOcurrido);
    connect(m_asistente, &AsistenteIA::herramientaEjecutada, this, &DialogAsistenteIA::slotHerramientaEjecutada);

    // Conectar clics de feedback interactivo y enlaces en el chat
    ui->textBrowserChat->setOpenLinks(false);
    connect(ui->textBrowserChat, &QTextBrowser::anchorClicked, this, &DialogAsistenteIA::slotAnchorClicked);

    // Mostrar el modelo global configurado
    ui->labelModeloActual->setText("Modelo: <b>" + m_asistente->modelo() + "</b>");

    // Inicializar autocompletado en el campo de entrada
    inicializarAutocompletado();

    // Estado inicial de botones
    ui->pushButtonDetener->setEnabled(false);

    // Inicializar la vista del chat con mensaje de bienvenida
    inicializarChat();
}

/**
 * @brief Destructor del diálogo.
 */
DialogAsistenteIA::~DialogAsistenteIA()
{
    delete ui;
}

/**
 * @brief Inicializa el contenido del visor de chat con un mensaje de bienvenida.
 */
void DialogAsistenteIA::inicializarChat()
{
    m_htmlChat =
        "<html><head><style>"
        "body { font-family: 'Segoe UI', Arial, sans-serif; font-size: 13px; margin: 8px; }"
        ".msg-box { margin-bottom: 12px; padding: 10px 14px; border-radius: 8px; line-height: 1.4; }"
        ".msg-user { background-color: #e3f2fd; border-left: 4px solid #1976d2; color: #0d47a1; margin-left: 40px; }"
        ".msg-assistant { background-color: #f5f5f5; border-left: 4px solid #43a047; color: #212121; margin-right: 40px; }"
        ".msg-tool { background-color: #fff8e1; border: 1px dashed #ffa000; color: #e65100; font-size: 11px; padding: 4px 8px; border-radius: 4px; margin: 4px 20px; }"
        ".feedback-bar { margin-top: 8px; padding-top: 6px; border-top: 1px dashed #ccc; font-size: 11px; color: #555; }"
        ".feedback-btn { text-decoration: none; padding: 2px 6px; background-color: #e2e8f0; border-radius: 4px; color: #1e293b; font-weight: bold; margin-left: 4px; }"
        ".author { font-weight: bold; margin-bottom: 4px; font-size: 12px; }"
        "table { border-collapse: collapse; width: 100%; margin-top: 6px; }"
        "th, td { border: 1px solid #ddd; padding: 4px 8px; text-align: left; }"
        "th { background-color: #e0e0e0; }"
        "a { color: #1565c0; text-decoration: none; font-weight: bold; }"
        "a:hover { text-decoration: underline; color: #0d47a1; }"
        "</style></head><body>";

    m_htmlChat +=
        "<div class='msg-box msg-assistant'>"
        "<div class='author'>🤖 Asistente TPV</div>"
        "¡Hola! Soy tu asistente inteligente local. Puedo ayudarte a consultar el <b>stock</b> de productos, "
        "revisar las <b>ventas del día</b>, comprobar <b>arqueos</b>, buscar <b>clientes</b> o localizar "
        "productos para indicaciones específicas.<br><br>"
        "<i>¿En qué te puedo ayudar hoy?</i>"
        "</div>";

    ui->textBrowserChat->setHtml(m_htmlChat + "</body></html>");
}

/**
 * @brief Inicializa el autocompletado en el campo de texto con descripciones de la base de datos local.
 */
void DialogAsistenteIA::inicializarAutocompletado()
{
    if (!conf) return;

    QString conexion = conf->getConexionLocal();
    QSqlDatabase db = QSqlDatabase::database(conexion);
    if (!db.isOpen()) return;

    QStringList listaCompletar;
    QSqlQuery q(db);
    if (q.exec("SELECT DISTINCT `desc` FROM articulos WHERE `desc` IS NOT NULL AND `desc` != '' ORDER BY `desc` ASC")) {
        while (q.next()) {
            QString desc = q.value(0).toString().trimmed();
            if (!desc.isEmpty()) {
                listaCompletar << desc;
            }
        }
    }

    if (!listaCompletar.isEmpty()) {
        QCompleter *completer = new QCompleter(listaCompletar, this);
        completer->setCaseSensitivity(Qt::CaseInsensitive);
        completer->setFilterMode(Qt::MatchStartsWith);
        ui->lineEditPregunta->setCompleter(completer);
    }
}

/**
 * @brief Convierte texto con sintaxis Markdown (tablas, negritas, listas) a HTML renderizable.
 */
static QString convertirMarkdownAHtml(const QString &markdown)
{
    QStringList lineas = markdown.split("\n");
    QString resultadoHtml;
    bool enTabla = false;
    bool primeraFilaTabla = true;
    bool enLista = false;

    static const QRegularExpression regexBold("\\*\\*(.*?)\\*\\*");
    static const QRegularExpression regexItalic("\\*(.*?)\\*");
    static const QRegularExpression regexLink("\\[(.*?)\\]\\((.*?)\\)");
    static const QRegularExpression regexArtDirect("\\[(?:art|articulo):\\s*([a-zA-Z0-9_-]+)\\]", QRegularExpression::CaseInsensitiveOption);

    auto formatearTexto = [&](QString txt) -> QString {
        txt.replace(regexBold, "<b>\\1</b>");
        txt.replace(regexItalic, "<i>\\1</i>");
        txt.replace(regexLink, "<a href='\\2' style='color:#1565c0;font-weight:bold;text-decoration:underline;'>\\1</a>");
        txt.replace(regexArtDirect, "<a href='articulo://\\1' style='color:#1565c0;font-weight:bold;text-decoration:underline;'>📦 \\1</a>");
        return txt;
    };

    for (int i = 0; i < lineas.size(); ++i) {
        QString linea = lineas[i].trimmed();

        // Detectar si es fila de tabla Markdown (ej. | Col1 | Col2 |)
        if (linea.startsWith("|") && linea.endsWith("|")) {
            // Ignorar fila de guiones separadores |---|---|
            if (linea.contains("---")) {
                continue;
            }

            if (!enTabla) {
                if (enLista) { resultadoHtml += "</ul>"; enLista = false; }
                resultadoHtml += "<table border='1' cellspacing='0' cellpadding='5' style='border-collapse:collapse;width:100%;margin:8px 0;font-size:12px;border:1px solid #ccc;'>";
                enTabla = true;
                primeraFilaTabla = true;
            }

            QStringList celdas = linea.split("|");
            if (!celdas.isEmpty() && celdas.first().isEmpty()) celdas.removeFirst();
            if (!celdas.isEmpty() && celdas.last().isEmpty()) celdas.removeLast();

            resultadoHtml += "<tr>";
            for (const QString &c : celdas) {
                QString contenido = formatearTexto(c.trimmed());
                if (primeraFilaTabla) {
                    resultadoHtml += "<th style='background-color:#e2e8f0;color:#1e293b;padding:6px;border:1px solid #cbd5e1;text-align:left;font-weight:bold;'>" + contenido + "</th>";
                } else {
                    resultadoHtml += "<td style='padding:5px 6px;border:1px solid #e2e8f0;background-color:#ffffff;'>" + contenido + "</td>";
                }
            }
            resultadoHtml += "</tr>";
            primeraFilaTabla = false;
            continue;
        } else if (enTabla) {
            resultadoHtml += "</table>";
            enTabla = false;
        }

        // Detectar elementos de lista (- item o * item)
        if (linea.startsWith("- ") || linea.startsWith("* ")) {
            if (!enLista) {
                resultadoHtml += "<ul style='margin:4px 0 6px 18px;padding:0;'>";
                enLista = true;
            }
            QString contenido = formatearTexto(linea.mid(2).trimmed());
            resultadoHtml += "<li style='margin-bottom:3px;'>" + contenido + "</li>";
            continue;
        } else if (enLista) {
            resultadoHtml += "</ul>";
            enLista = false;
        }

        if (linea.isEmpty()) {
            resultadoHtml += "<br>";
            continue;
        }

        // Línea normal de texto
        QString lineaFmt = formatearTexto(linea);
        resultadoHtml += lineaFmt + "<br>";
    }

    if (enTabla) resultadoHtml += "</table>";
    if (enLista) resultadoHtml += "</ul>";

    return resultadoHtml;
}

/**
 * @brief Añade una burbuja de mensaje formateada al chat con enlaces opcionales de feedback.
 */
void DialogAsistenteIA::agregarBurbuja(const QString &remitente, const QString &texto,
                                       const QString &colorFondo, const QString &colorBorde,
                                       bool esUsuario, qint64 idLog)
{
    Q_UNUSED(colorFondo);
    Q_UNUSED(colorBorde);

    QString clase = esUsuario ? "msg-user" : "msg-assistant";
    QString icono = esUsuario ? "👤" : "🤖";

    QString textoHtml = convertirMarkdownAHtml(texto);

    QString feedbackHtml;
    if (!esUsuario && idLog > 0) {
        feedbackHtml = QString(
            "<div class='feedback-bar'>"
            "¿Fue útil y correcta esta respuesta? "
            "<a class='feedback-btn' href='feedback://correcta/%1'>👍 Correcta</a>"
            "<a class='feedback-btn' href='feedback://incorrecta/%1'>👎 Incorrecta / Mejorable</a>"
            "</div>"
        ).arg(idLog);
    }

    QString fragmento = QString(
        "<div class='msg-box %1'>"
        "<div class='author'>%2 %3</div>"
        "%4"
        "%5"
        "</div>"
    ).arg(clase, icono, remitente, textoHtml, feedbackHtml);

    m_htmlChat += fragmento;
    ui->textBrowserChat->setHtml(m_htmlChat + "</body></html>");
    desplazarChatAlFinal();
}

/**
 * @brief Añade una notificación visual de herramienta ejecutada.
 */
void DialogAsistenteIA::agregarNotificacionTool(const QString &resumen)
{
    QString fragmento = QString(
        "<div class='msg-tool'>⚙️ <i>%1</i></div>"
    ).arg(resumen);

    m_htmlChat += fragmento;
    ui->textBrowserChat->setHtml(m_htmlChat + "</body></html>");
    desplazarChatAlFinal();
}

/**
 * @brief Desplaza el scroll del chat automáticamente hacia el último mensaje.
 */
void DialogAsistenteIA::desplazarChatAlFinal()
{
    QScrollBar *sb = ui->textBrowserChat->verticalScrollBar();
    if (sb) {
        sb->setValue(sb->maximum());
    }
}

/**
 * @brief Envía la consulta escrita por el usuario.
 */
void DialogAsistenteIA::on_pushButtonEnviar_clicked()
{
    QString pregunta = ui->lineEditPregunta->text().trimmed();
    if (pregunta.isEmpty()) return;

    if (m_asistente->estaProcesando()) {
        QMessageBox::information(this, "En proceso", "El asistente está respondiendo a la consulta anterior. Espera un momento.");
        return;
    }

    // Añadir mensaje del usuario a la vista
    agregarBurbuja("Tú", pregunta, "#e3f2fd", "#1976d2", true);

    // Limpiar campo de texto
    ui->lineEditPregunta->clear();

    // Deshabilitar botón enviar y activar detener mientras procesa
    ui->pushButtonEnviar->setEnabled(false);
    ui->pushButtonDetener->setEnabled(true);

    // Enviar al motor
    m_asistente->enviarMensaje(pregunta);
}

/**
 * @brief Detiene o cancela la petición HTTP en curso hacia la IA.
 */
void DialogAsistenteIA::on_pushButtonDetener_clicked()
{
    m_asistente->cancelarConsulta();
    ui->pushButtonDetener->setEnabled(false);
    ui->pushButtonEnviar->setEnabled(true);
    ui->labelEstado->setText("Consulta cancelada por el usuario.");
}

/**
 * @brief Copia al portapapeles la última respuesta del asistente o todo el historial.
 */
void DialogAsistenteIA::on_pushButtonCopiar_clicked()
{
    QString textoACopiar = m_ultimaRespuestaIA.trimmed();
    if (textoACopiar.isEmpty()) {
        textoACopiar = ui->textBrowserChat->toPlainText().trimmed();
    }

    if (!textoACopiar.isEmpty()) {
        QClipboard *clipboard = QGuiApplication::clipboard();
        if (clipboard) {
            clipboard->setText(textoACopiar);
            ui->labelEstado->setText("📋 Copiado al portapapeles con éxito.");
        }
    } else {
        ui->labelEstado->setText("No hay texto para copiar.");
    }
}

void DialogAsistenteIA::on_lineEditPregunta_returnPressed()
{
    on_pushButtonEnviar_clicked();
}

/**
 * @brief Limpia la conversación en pantalla y en la memoria del motor.
 */
void DialogAsistenteIA::on_pushButtonLimpiar_clicked()
{
    m_asistente->limpiarHistorial();
    inicializarChat();
    ui->labelEstado->setText("Historial reiniciado.");
}

/**
 * @brief Abre el editor de Base de Conocimiento de Fitoterapia y Salud.
 */
void DialogAsistenteIA::on_pushButtonConocimiento_clicked()
{
    DialogConocimientoIA dlg(this);
    connect(&dlg, &DialogConocimientoIA::conocimientoModificado, m_asistente, &AsistenteIA::recargarConocimiento);
    dlg.exec();
}

/**
 * @brief Abre el diálogo de Historial de Consultas, Auditoría y Feedback de IA.
 */
void DialogAsistenteIA::on_pushButtonVerLogs_clicked()
{
    DialogLogsIA dlg(this);
    dlg.exec();
}

// ─────────────────────────────────────────────────────────────────────────────
// Sugerencias Rápidas
// ─────────────────────────────────────────────────────────────────────────────

void DialogAsistenteIA::on_pushButtonSugerenciaVentas_clicked()
{
    ui->lineEditPregunta->setText("¿Cuánto hemos vendido hoy y cuál es el desglose de efectivo y tarjeta?");
    on_pushButtonEnviar_clicked();
}

/**
 * @brief Envía la consulta sobre la venta de artículos y productos realizada hoy.
 */
void DialogAsistenteIA::on_pushButtonSugerenciaArticulosHoy_clicked()
{
    ui->lineEditPregunta->setText("¿Cuáles son los artículos y productos más vendidos hoy? Muéstrame el listado de ventas de hoy con unidades y facturación.");
    on_pushButtonEnviar_clicked();
}

/**
 * @brief Envía una consulta para obtener los datos estadísticos del producto indicado previamente en el lineEdit.
 */
void DialogAsistenteIA::on_pushButtonSugerenciaEstadisticasProducto_clicked()
{
    QString producto = ui->lineEditPregunta->text().trimmed();
    if (producto.isEmpty()) {
        QMessageBox::information(this, "Producto no indicado",
                                 "Escribe primero en el campo de texto el nombre o código del producto para consultar sus estadísticas.");
        ui->lineEditPregunta->setFocus();
        return;
    }

    // Formular la pregunta con el producto ingresado previamente
    QString pregunta = QString("Muéstrame todos los datos estadísticos del producto '%1': histórico de ventas, unidades vendidas, evolución mensual y cobertura de stock.").arg(producto);
    ui->lineEditPregunta->setText(pregunta);
    on_pushButtonEnviar_clicked();
}

void DialogAsistenteIA::on_pushButtonCerrar_clicked()
{
    close();
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots del Motor AsistenteIA y Manejo de Enlaces
// ─────────────────────────────────────────────────────────────────────────────

void DialogAsistenteIA::slotRespuestaRecibida(const QString &respuesta, qint64 idLog)
{
    m_ultimaRespuestaIA = respuesta;
    ui->pushButtonEnviar->setEnabled(true);
    ui->pushButtonDetener->setEnabled(false);
    agregarBurbuja("Asistente IA", respuesta, "#f5f5f5", "#43a047", false, idLog);
    ui->lineEditPregunta->setFocus();
}

void DialogAsistenteIA::slotEstadoCambiado(const QString &estado)
{
    ui->labelEstado->setText(estado);
    if (estado == "Listo" || estado == "Error" || estado.contains("cancelada", Qt::CaseInsensitive)) {
        ui->pushButtonEnviar->setEnabled(true);
        ui->pushButtonDetener->setEnabled(false);
    }
}

void DialogAsistenteIA::slotErrorOcurrido(const QString &mensajeError)
{
    ui->pushButtonEnviar->setEnabled(true);
    ui->pushButtonDetener->setEnabled(false);
    agregarNotificacionTool("Error: " + mensajeError);
    ui->labelEstado->setText("Error en comunicación");
}

void DialogAsistenteIA::slotHerramientaEjecutada(const QString &nombreHerramienta, const QString &resumen)
{
    Q_UNUSED(nombreHerramienta);
    agregarNotificacionTool(resumen);
}

/**
 * @brief Procesa los clics en los enlaces interactivos (artículos, clientes, feedback) embebidos en el chat.
 */
void DialogAsistenteIA::slotAnchorClicked(const QUrl &url)
{
    if (url.scheme() == "articulo") {
        QString codigo = url.host().isEmpty() ? url.path() : url.host();
        while (codigo.startsWith("/")) codigo.remove(0, 1);
        codigo = codigo.trimmed();
        if (!codigo.isEmpty()) {
            Articulos *artDlg = new Articulos(this);
            artDlg->cargarArticuloPorCodigo(codigo);
            artDlg->exec();
            delete artDlg;
        }
    } else if (url.scheme() == "cliente") {
        QString codCliente = url.host().isEmpty() ? url.path() : url.host();
        while (codCliente.startsWith("/")) codCliente.remove(0, 1);
        codCliente = codCliente.trimmed();
        if (!codCliente.isEmpty()) {
            Clientes *cliDlg = new Clientes(this, codCliente);
            cliDlg->exec();
            delete cliDlg;
        }
    } else if (url.scheme() == "feedback") {
        QString host = url.host();
        QString path = url.path();
        if (path.startsWith("/")) path.remove(0, 1);
        qint64 idLog = path.toLongLong();

        if (idLog <= 0) return;

        if (host == "correcta") {
            AsistenteIA::registrarFeedback(idLog, 1, "Evaluada como correcta desde el chat");
            ui->labelEstado->setText(QString("✅ Interacción #%1 registrada como correcta.").arg(idLog));
        } else if (host == "incorrecta") {
            bool ok = false;
            QString motivo = QInputDialog::getText(this, "Evaluar Respuesta como Incorrecta",
                                                   "Describe qué falló o qué herramienta nueva se necesita:",
                                                   QLineEdit::Normal, "", &ok);
            if (ok && !motivo.trimmed().isEmpty()) {
                AsistenteIA::registrarFeedback(idLog, -1, motivo.trimmed());
                ui->labelEstado->setText(QString("⚠️ Interacción #%1 registrada con feedback.").arg(idLog));
            }
        }
    }
}
