#include "dialoggenerardescripcionia.h"
#include "ui_dialoggenerardescripcionia.h"

#include <QApplication>
#include <QClipboard>
#include <QMessageBox>

/**
 * @brief Constructor del diálogo DialogGenerarDescripcionIA.
 * @param descripcion Nombre o descripción corta del producto visible en el formulario.
 * @param fabricante Nombre del fabricante o marca.
 * @param familia Categoría o familia asignada al artículo.
 * @param formato Presentación comercial (ej. 100ml, 230 comp, Cápsulas).
 * @param ean Código de barras EAN-13 o identificador único.
 * @param notasPrevias Contenido existente en el campo notas para no perder histórico.
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
      m_enriquecedor(new EnriquecedorFichasIA(this)),
      m_nombreProducto(descripcion.trimmed()),
      m_fabricante(fabricante.trimmed()),
      m_familia(familia.trimmed()),
      m_formato(formato.trimmed()),
      m_ean(ean.trimmed()),
      m_notasPrevias(notasPrevias.trimmed()),
      m_buscando(false),
      m_contadorGeneraciones(0)
{
    ui->setupUi(this);

    // Conectar señales del motor asíncrono de enriquecimiento
    connect(m_enriquecedor, &EnriquecedorFichasIA::progreso, this, &DialogGenerarDescripcionIA::onProgreso);
    connect(m_enriquecedor, &EnriquecedorFichasIA::finalizado, this, &DialogGenerarDescripcionIA::onFinalizado);

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
    m_enriquecedor->cancelar();
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
 * @brief Inicia el ciclo de búsqueda de fuentes, extracción y validación.
 */
void DialogGenerarDescripcionIA::iniciarProceso()
{
    QString termino = ui->lineEditTermino->text().trimmed();
    if (termino.isEmpty()) {
        termino = m_nombreProducto;
    }

    m_contadorGeneraciones++;
    m_buscando = true;
    ui->progressBar->setRange(0, 5);
    ui->progressBar->setValue(0);
    ui->pushButtonRegenerar->setEnabled(false);
    ui->pushButtonAceptar->setEnabled(false);

    ui->labelInsigniaVerificacion->setText(tr("🔍 Buscando fuentes oficiales y contrastando datos..."));
    ui->labelInsigniaVerificacion->setStyleSheet("padding: 4px 8px; border-radius: 4px; background-color: #fff8e1; color: #f57f17; border: 1px solid #ffe082; font-weight: bold;");

    ui->labelEstado->setText(tr("Iniciando análisis documental..."));
    ui->labelFuentes->setText(tr("🌐 Buscando fuentes en internet y contrastando datos..."));

    m_enriquecedor->procesarArticulo(m_ean, termino, m_fabricante, m_familia, m_formato, m_notasPrevias);
}

/**
 * @brief Slot para reflejar el progreso de cada etapa en la interfaz.
 */
void DialogGenerarDescripcionIA::onProgreso(int pasoActual, int totalPasos, const QString &mensaje)
{
    ui->progressBar->setRange(0, totalPasos);
    ui->progressBar->setValue(pasoActual);
    ui->labelEstado->setText(mensaje);
}

/**
 * @brief Slot llamado cuando el motor termina la extracción y validación.
 */
void DialogGenerarDescripcionIA::onFinalizado(bool exito, const ResultadoFicha &resultado)
{
    m_buscando = false;
    ui->progressBar->setValue(ui->progressBar->maximum());
    ui->pushButtonRegenerar->setEnabled(true);
    ui->pushButtonAceptar->setEnabled(true);

    if (!exito || resultado.estado == "error") {
        ui->labelInsigniaVerificacion->setText(tr("❌ No se pudo verificar la composición"));
        ui->labelInsigniaVerificacion->setStyleSheet("padding: 4px 8px; border-radius: 4px; background-color: #ffebee; color: #c62828; border: 1px solid #ef9a9a; font-weight: bold;");
        ui->labelEstado->setText(resultado.error.isEmpty() ? tr("Error generando la ficha.") : resultado.error);
        ui->textEditResultado->setPlainText(resultado.error);
        return;
    }

    // Cargar HTML estructurado en el editor
    ui->textEditResultado->setHtml(resultado.htmlFormateado);

    int pctExactitud = qRound(resultado.exactitud * 100);

    if (resultado.estado == "ok") {
        ui->labelInsigniaVerificacion->setText(
            tr("✅ Composición Verificada (Exactitud: %1% | Confianza: %2)")
            .arg(QString::number(pctExactitud), resultado.confianza.toUpper()));
        ui->labelInsigniaVerificacion->setStyleSheet(
            "padding: 4px 8px; border-radius: 4px; background-color: #e8f5e9; color: #2e7d32; border: 1px solid #a5d6a7; font-weight: bold;");
        ui->labelEstado->setText(tr("✅ Ficha técnica generada con éxito con respaldo literal en fuentes oficiales."));
    } else {
        ui->labelInsigniaVerificacion->setText(
            tr("⚠️ Requiere Revisión Humana (Exactitud léxica: %1% | Posible ambigüedad en formato o datos)")
            .arg(QString::number(pctExactitud)));
        ui->labelInsigniaVerificacion->setStyleSheet(
            "padding: 4px 8px; border-radius: 4px; background-color: #fff3e0; color: #e65100; border: 1px solid #ffcc80; font-weight: bold;");
        ui->labelEstado->setText(tr("⚠️ Revisa la composición y presentación antes de aplicar la ficha."));
    }

    // Mostrar fuentes consultadas en su widget dedicado (sin incorporarlas en el texto editable de la ficha)
    if (!resultado.fuentes.isEmpty()) {
        QStringList links;
        for (int i = 0; i < resultado.fuentes.size() && i < 4; ++i) {
            const FuenteWeb &f = resultado.fuentes[i];
            QString badge = f.oficial ? "<b><span style='color:#166534;'>[Oficial]</span></b>" : "<b><span style='color:#1e40af;'>[Web]</span></b>";
            QString titulo = f.titulo.isEmpty() ? f.url : f.titulo;
            if (titulo.length() > 60) titulo = titulo.left(57) + "...";
            links << QString("%1 <a href=\"%2\" style=\"color:#0284c7; text-decoration:none;\">%3</a>").arg(badge, f.url, titulo);
        }
        ui->labelFuentes->setText(tr("🌐 <b>Fuentes consultadas:</b> %1").arg(links.join(" &nbsp;|&nbsp; ")));
    } else {
        ui->labelFuentes->setText(tr("🌐 <i>No se registraron fuentes externas.</i>"));
    }
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
    m_enriquecedor->cancelar();
    reject();
}
