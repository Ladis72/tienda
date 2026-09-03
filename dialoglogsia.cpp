#include "dialoglogsia.h"
#include "ui_dialoglogsia.h"
#include "asistenteia.h"
#include <QDate>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QHeaderView>
#include <QInputDialog>
#include <QClipboard>
#include <QGuiApplication>

/**
 * @brief Constructor del diálogo DialogLogsIA.
 * Inicializa las fechas con formato "yyyy-MM-dd", configura las columnas de la tabla y carga los datos.
 */
DialogLogsIA::DialogLogsIA(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogLogsIA),
      m_idLogSeleccionado(0)
{
    ui->setupUi(this);

    // Configurar fechas por defecto: Últimos 30 días hasta hoy en formato "yyyy-MM-dd"
    QDate hoy = QDate::currentDate();
    QDate hace30Dias = hoy.addDays(-30);
    ui->dateEditInicio->setDate(hace30Dias);
    ui->dateEditFin->setDate(hoy);

    // Ajustar encabezados y proporciones de la tabla de interacciones
    QHeaderView *header = ui->tableWidgetLogs->horizontalHeader();
    header->setSectionResizeMode(0, QHeaderView::ResizeToContents); // ID
    header->setSectionResizeMode(1, QHeaderView::ResizeToContents); // Fecha
    header->setSectionResizeMode(2, QHeaderView::ResizeToContents); // Hora
    header->setSectionResizeMode(3, QHeaderView::ResizeToContents); // Tienda
    header->setSectionResizeMode(4, QHeaderView::ResizeToContents); // Usuario
    header->setSectionResizeMode(5, QHeaderView::Stretch);          // Petición
    header->setSectionResizeMode(6, QHeaderView::ResizeToContents); // Herramientas
    header->setSectionResizeMode(7, QHeaderView::ResizeToContents); // Tiempo ms
    header->setSectionResizeMode(8, QHeaderView::ResizeToContents); // Evaluación
    header->setSectionResizeMode(9, QHeaderView::ResizeToContents); // Feedback

    // Cargar los primeros registros
    cargarLogs();
}

/**
 * @brief Destructor del diálogo.
 */
DialogLogsIA::~DialogLogsIA()
{
    delete ui;
}

/**
 * @brief Carga las peticiones registradas desde la base de datos aplicando los filtros de estado, texto y fechas.
 */
void DialogLogsIA::cargarLogs()
{
    int comboIdx = ui->comboBoxFiltroEstado->currentIndex();
    int filtroEval = 99; // 99 = Todos
    if (comboIdx == 1) filtroEval = -1; // Incorrectas
    else if (comboIdx == 2) filtroEval = 1; // Correctas
    else if (comboIdx == 3) filtroEval = 0; // Sin evaluar

    QString texto = ui->lineEditBuscar->text().trimmed();
    QString fechaI = ui->dateEditInicio->date().toString("yyyy-MM-dd");
    QString fechaF = ui->dateEditFin->date().toString("yyyy-MM-dd");

    m_logsActuales = AsistenteIA::obtenerLogs(filtroEval, texto, fechaI, fechaF, 300);

    ui->tableWidgetLogs->setRowCount(0);
    ui->tableWidgetLogs->setRowCount(m_logsActuales.size());

    for (int i = 0; i < m_logsActuales.size(); ++i) {
        const QVariantMap &l = m_logsActuales[i];
        qint64 id = l.value("id").toLongLong();
        QString fecha = l.value("fecha").toString();
        QString hora = l.value("hora").toString();
        int idTienda = l.value("id_tienda").toInt();
        QString usuario = l.value("usuario").toString();
        QString peticion = l.value("peticion").toString();
        QString tools = l.value("herramientas_usadas").toString();
        int ms = l.value("tiempo_ms").toInt();
        int eval = l.value("es_correcta").toInt();
        QString feedback = l.value("comentario_feedback").toString();

        QString strEval = "⚪ Sin evaluar";
        if (eval == 1) strEval = "✅ Correcta";
        else if (eval == -1) strEval = "❌ Incorrecta / Mejorar";

        ui->tableWidgetLogs->setItem(i, 0, new QTableWidgetItem(QString::number(id)));
        ui->tableWidgetLogs->setItem(i, 1, new QTableWidgetItem(fecha));
        ui->tableWidgetLogs->setItem(i, 2, new QTableWidgetItem(hora));
        ui->tableWidgetLogs->setItem(i, 3, new QTableWidgetItem(QString("Tienda %1").arg(idTienda)));
        ui->tableWidgetLogs->setItem(i, 4, new QTableWidgetItem(usuario));
        ui->tableWidgetLogs->setItem(i, 5, new QTableWidgetItem(peticion));
        ui->tableWidgetLogs->setItem(i, 6, new QTableWidgetItem(tools.isEmpty() ? "-" : tools));
        ui->tableWidgetLogs->setItem(i, 7, new QTableWidgetItem(QString("%1 ms").arg(ms)));
        ui->tableWidgetLogs->setItem(i, 8, new QTableWidgetItem(strEval));
        ui->tableWidgetLogs->setItem(i, 9, new QTableWidgetItem(feedback));
    }

    ui->labelContador->setText(QString("Total de consultas registradas: %1").arg(m_logsActuales.size()));

    // Limpiar panel de detalle si no hay registros
    if (m_logsActuales.isEmpty()) {
        m_idLogSeleccionado = 0;
        ui->textEditPregunta->clear();
        ui->textEditRespuesta->clear();
        ui->lineEditFeedback->clear();
    } else {
        ui->tableWidgetLogs->selectRow(0);
        mostrarDetalleLog(0);
    }
}

/**
 * @brief Muestra en el panel inferior los datos completos de la interacción seleccionada.
 */
void DialogLogsIA::mostrarDetalleLog(int fila)
{
    if (fila < 0 || fila >= m_logsActuales.size()) return;

    const QVariantMap &l = m_logsActuales[fila];
    m_idLogSeleccionado = l.value("id").toLongLong();

    ui->textEditPregunta->setPlainText(l.value("peticion").toString());
    ui->textEditRespuesta->setPlainText(l.value("respuesta").toString());
    ui->lineEditFeedback->setText(l.value("comentario_feedback").toString());
}

/**
 * @brief Slot llamado cuando cambia la selección en la tabla.
 */
void DialogLogsIA::on_tableWidgetLogs_itemSelectionChanged()
{
    int fila = ui->tableWidgetLogs->currentRow();
    mostrarDetalleLog(fila);
}

/**
 * @brief Slot para el botón Filtrar.
 */
void DialogLogsIA::on_pushButtonFiltrar_clicked()
{
    cargarLogs();
}

/**
 * @brief Slot cuando cambia el selector de estado.
 */
void DialogLogsIA::on_comboBoxFiltroEstado_currentIndexChanged(int /*index*/)
{
    cargarLogs();
}

/**
 * @brief Actualiza la evaluación en base de datos y refresca la fila correspondiente.
 */
void DialogLogsIA::actualizarEvaluacion(int evaluacion, const QString &comentario)
{
    if (m_idLogSeleccionado <= 0) {
        QMessageBox::warning(this, "Aviso", "Selecciona primero una interacción de la tabla.");
        return;
    }

    bool ok = AsistenteIA::registrarFeedback(m_idLogSeleccionado, evaluacion, comentario);
    if (ok) {
        int fila = ui->tableWidgetLogs->currentRow();
        if (fila >= 0 && fila < m_logsActuales.size()) {
            m_logsActuales[fila]["es_correcta"] = evaluacion;
            m_logsActuales[fila]["comentario_feedback"] = comentario;

            QString strEval = (evaluacion == 1) ? "✅ Correcta" : "❌ Incorrecta / Mejorar";
            ui->tableWidgetLogs->setItem(fila, 8, new QTableWidgetItem(strEval));
            ui->tableWidgetLogs->setItem(fila, 9, new QTableWidgetItem(comentario));
        }
        QMessageBox::information(this, "Guardado", "Evaluación y feedback registrados correctamente.");
    } else {
        QMessageBox::critical(this, "Error", "No se pudo actualizar el registro en la base de datos.");
    }
}

/**
 * @brief Marca la interacción seleccionada como Correcta (1).
 */
void DialogLogsIA::on_pushButtonMarcarCorrecta_clicked()
{
    QString com = ui->lineEditFeedback->text().trimmed();
    if (com.isEmpty()) com = "Respuesta correcta y precisa";
    ui->lineEditFeedback->setText(com);
    actualizarEvaluacion(1, com);
}

/**
 * @brief Marca la interacción seleccionada como Incorrecta (-1) solicitando el motivo.
 */
void DialogLogsIA::on_pushButtonMarcarIncorrecta_clicked()
{
    QString comActual = ui->lineEditFeedback->text().trimmed();
    bool ok = false;
    QString motivo = QInputDialog::getText(this, "Evaluar como Incorrecta / Mejorable",
                                           "Describe qué falló en la respuesta o qué herramienta nueva se necesita:",
                                           QLineEdit::Normal, comActual, &ok);
    if (ok) {
        ui->lineEditFeedback->setText(motivo);
        actualizarEvaluacion(-1, motivo);
    }
}

/**
 * @brief Guarda el comentario escrito en el campo de texto.
 */
void DialogLogsIA::on_pushButtonGuardarFeedback_clicked()
{
    int fila = ui->tableWidgetLogs->currentRow();
    int eval = 0;
    if (fila >= 0 && fila < m_logsActuales.size()) {
        eval = m_logsActuales[fila].value("es_correcta").toInt();
    }
    actualizarEvaluacion(eval, ui->lineEditFeedback->text().trimmed());
}

/**
 * @brief Copia el reporte estructurado de mejoras al portapapeles para pegarlo en Antigravity.
 */
void DialogLogsIA::on_pushButtonCopiarPortapapeles_clicked()
{
    int comboIdx = ui->comboBoxFiltroEstado->currentIndex();
    int filtroEval = -1; // Por defecto exportar fallos y mejoras
    if (comboIdx == 0) filtroEval = 99; // Todo
    else if (comboIdx == 2) filtroEval = 1;

    QString reporte = AsistenteIA::exportarReporteMejoras(filtroEval);

    QClipboard *clipboard = QGuiApplication::clipboard();
    if (clipboard) {
        clipboard->setText(reporte);
        QMessageBox::information(this, "Copiado al Portapapeles",
            "📋 El reporte de auditoría y diagnóstico se ha copiado al portapapeles.\n\n"
            "Pégalo directamente en el chat con Antigravity para que analice los fallos y cree las nuevas herramientas o mejoras necesarias.");
    }
}

/**
 * @brief Exporta el reporte estructurado de fallos y mejoras a un archivo Markdown.
 */
void DialogLogsIA::on_pushButtonExportar_clicked()
{
    QString rutaDefecto = QString("reporte_analisis_ia_%1.md").arg(QDate::currentDate().toString("yyyy-MM-dd"));
    QString ruta = QFileDialog::getSaveFileName(this, "Guardar Reporte de Análisis de IA", rutaDefecto, "Documentos Markdown (*.md);;Archivos de texto (*.txt)");
    if (ruta.isEmpty()) return;

    int comboIdx = ui->comboBoxFiltroEstado->currentIndex();
    int filtroEval = -1; // Por defecto exportar fallos y mejoras
    if (comboIdx == 0) filtroEval = 99; // Todo
    else if (comboIdx == 2) filtroEval = 1;

    QString reporte = AsistenteIA::exportarReporteMejoras(filtroEval);

    QFile archivo(ruta);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo de reporte en la ruta seleccionada.");
        return;
    }

    QTextStream out(&archivo);
    out << reporte;
    archivo.close();

    QMessageBox::information(this, "Reporte Exportado", QString("El reporte de análisis se guardó con éxito en:\n%1").arg(ruta));
}

/**
 * @brief Cierra el diálogo.
 */
void DialogLogsIA::on_pushButtonCerrar_clicked()
{
    accept();
}
