#include "dialogasistenteia.h"
#include "ui_dialogasistenteia.h"
#include "dialogconocimientoia.h"
#include <QCoreApplication>
#include <QDate>
#include <QMessageBox>
#include <QScrollBar>
#include <QSettings>

/**
 * @brief Constructor del diálogo DialogAsistenteIA.
 */
DialogAsistenteIA::DialogAsistenteIA(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogAsistenteIA),
      m_asistente(new AsistenteIA(this))
{
    ui->setupUi(this);

    // Permitir que el diálogo esté siempre al frente y accesible desde cualquier ventana o modal
    setWindowFlags(Qt::Window | Qt::WindowStaysOnTopHint);

    // Conectar señales del motor AsistenteIA
    connect(m_asistente, &AsistenteIA::respuestaRecibida, this, &DialogAsistenteIA::slotRespuestaRecibida);
    connect(m_asistente, &AsistenteIA::estadoCambiado, this, &DialogAsistenteIA::slotEstadoCambiado);
    connect(m_asistente, &AsistenteIA::errorOcurrido, this, &DialogAsistenteIA::slotErrorOcurrido);
    connect(m_asistente, &AsistenteIA::herramientaEjecutada, this, &DialogAsistenteIA::slotHerramientaEjecutada);

    // Mostrar el modelo global configurado
    ui->labelModeloActual->setText("Modelo: <b>" + m_asistente->modelo() + "</b>");

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
        ".author { font-weight: bold; margin-bottom: 4px; font-size: 12px; }"
        "table { border-collapse: collapse; width: 100%; margin-top: 6px; }"
        "th, td { border: 1px solid #ddd; padding: 4px 8px; text-align: left; }"
        "th { background-color: #e0e0e0; }"
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
                QString contenido = c.trimmed();
                contenido.replace(regexBold, "<b>\\1</b>");
                contenido.replace(regexItalic, "<i>\\1</i>");
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
            QString contenido = linea.mid(2).trimmed();
            contenido.replace(regexBold, "<b>\\1</b>");
            contenido.replace(regexItalic, "<i>\\1</i>");
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
        QString lineaFmt = linea;
        lineaFmt.replace(regexBold, "<b>\\1</b>");
        lineaFmt.replace(regexItalic, "<i>\\1</i>");
        resultadoHtml += lineaFmt + "<br>";
    }

    if (enTabla) resultadoHtml += "</table>";
    if (enLista) resultadoHtml += "</ul>";

    return resultadoHtml;
}

/**
 * @brief Añade una burbuja de mensaje formateada al chat.
 */
void DialogAsistenteIA::agregarBurbuja(const QString &remitente, const QString &texto,
                                       const QString &colorFondo, const QString &colorBorde, bool esUsuario)
{
    QString clase = esUsuario ? "msg-user" : "msg-assistant";
    QString icono = esUsuario ? "👤" : "🤖";

    QString textoHtml = convertirMarkdownAHtml(texto);

    QString fragmento = QString(
        "<div class='msg-box %1'>"
        "<div class='author'>%2 %3</div>"
        "%4"
        "</div>"
    ).arg(clase, icono, remitente, textoHtml);

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

    // Deshabilitar botón mientras procesa
    ui->pushButtonEnviar->setEnabled(false);

    // Enviar al motor
    m_asistente->enviarMensaje(pregunta);
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

// ─────────────────────────────────────────────────────────────────────────────
// Sugerencias Rápidas
// ─────────────────────────────────────────────────────────────────────────────

void DialogAsistenteIA::on_pushButtonSugerenciaVentas_clicked()
{
    ui->lineEditPregunta->setText("¿Cuánto hemos vendido hoy y cuál es el desglose de efectivo y tarjeta?");
    on_pushButtonEnviar_clicked();
}

void DialogAsistenteIA::on_pushButtonSugerenciaStock_clicked()
{
    ui->lineEditPregunta->setText("¿Qué productos están actualmente bajo mínimos de stock?");
    on_pushButtonEnviar_clicked();
}

void DialogAsistenteIA::on_pushButtonSugerenciaArqueos_clicked()
{
    ui->lineEditPregunta->setText("Muéstrame el resumen de los últimos arqueos de caja.");
    on_pushButtonEnviar_clicked();
}

void DialogAsistenteIA::on_pushButtonCerrar_clicked()
{
    close();
}

// ─────────────────────────────────────────────────────────────────────────────
// Slots del Motor AsistenteIA
// ─────────────────────────────────────────────────────────────────────────────

void DialogAsistenteIA::slotRespuestaRecibida(const QString &respuesta)
{
    ui->pushButtonEnviar->setEnabled(true);
    agregarBurbuja("Asistente IA", respuesta, "#f5f5f5", "#43a047", false);
    ui->lineEditPregunta->setFocus();
}

void DialogAsistenteIA::slotEstadoCambiado(const QString &estado)
{
    ui->labelEstado->setText(estado);
    if (estado == "Listo" || estado == "Error") {
        ui->pushButtonEnviar->setEnabled(true);
    }
}

void DialogAsistenteIA::slotErrorOcurrido(const QString &mensajeError)
{
    ui->pushButtonEnviar->setEnabled(true);
    agregarNotificacionTool("Error: " + mensajeError);
    ui->labelEstado->setText("Error en comunicación");
}

void DialogAsistenteIA::slotHerramientaEjecutada(const QString &nombreHerramienta, const QString &resumen)
{
    Q_UNUSED(nombreHerramienta);
    agregarNotificacionTool(resumen);
}
