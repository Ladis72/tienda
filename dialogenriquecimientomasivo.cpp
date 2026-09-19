#include "dialogenriquecimientomasivo.h"
#include "ui_dialogenriquecimientomasivo.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDialog>
#include <QVBoxLayout>
#include <QTextBrowser>
#include <QPushButton>
#include <QRegularExpression>
#include <QDebug>
#include "configuracion.h"

extern Configuracion *conf;

/**
 * @brief Obtiene la conexión activa a la base de datos local del ERP.
 */
static QSqlDatabase obtenerBdLocal()
{
    QString connLocal = conf ? conf->getConexionLocal() : "DB";
    if (connLocal.isEmpty()) connLocal = "DB";
    if (QSqlDatabase::contains(connLocal) && QSqlDatabase::database(connLocal).isOpen()) {
        return QSqlDatabase::database(connLocal);
    }
    if (QSqlDatabase::contains("DB") && QSqlDatabase::database("DB").isOpen()) {
        return QSqlDatabase::database("DB");
    }
    return QSqlDatabase::database();
}

/**
 * @brief Constructor del diálogo de enriquecimiento masivo.
 */
DialogEnriquecimientoMasivo::DialogEnriquecimientoMasivo(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogEnriquecimientoMasivo),
      m_enriquecedor(new EnriquecedorFichasIA(this)),
      m_indiceActual(-1),
      m_ejecutando(false),
      m_contadorOk(0),
      m_contadorRevisar(0),
      m_contadorError(0)
{
    ui->setupUi(this);

    configurarTabla();

    ui->splitterResultados->setSizes(QList<int>() << 360 << 240);
    ui->pushButtonAceptarFicha->setEnabled(false);
    ui->pushButtonRechazarFicha->setEnabled(false);

    connect(m_enriquecedor, &EnriquecedorFichasIA::progreso, this, &DialogEnriquecimientoMasivo::onProductoProgreso);
    connect(m_enriquecedor, &EnriquecedorFichasIA::finalizado, this, &DialogEnriquecimientoMasivo::onProductoFinalizado);

    // Cargar la lista inicial de artículos al abrir
    on_pushButtonCargarLista_clicked();
}

DialogEnriquecimientoMasivo::~DialogEnriquecimientoMasivo()
{
    m_enriquecedor->cancelar();
    delete ui;
}

/**
 * @brief Configura las columnas, cabeceras y modo de selección de la tabla de resultados.
 */
void DialogEnriquecimientoMasivo::configurarTabla()
{
    QStringList cabeceras;
    cabeceras << tr("EAN")
              << tr("Descripción")
              << tr("Fabricante")
              << tr("Familia")
              << tr("Estado IA")
              << tr("Exactitud")
              << tr("Confianza")
              << tr("Guardado en BD")
              << tr("Producto Detectado");

    ui->tableWidgetResultados->setColumnCount(cabeceras.size());
    ui->tableWidgetResultados->setHorizontalHeaderLabels(cabeceras);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(5, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(6, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(7, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->horizontalHeader()->setSectionResizeMode(8, QHeaderView::ResizeToContents);
    ui->tableWidgetResultados->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetResultados->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief Valida si un código de barras cumple con el algoritmo checksum de EAN-13.
 */
bool DialogEnriquecimientoMasivo::esEan13Valido(const QString &cod) const
{
    if (cod.length() != 13) return false;
    QRegularExpression reDigits("^\\d{13}$");
    if (!reDigits.match(cod).hasMatch()) return false;

    // Descartar prefijos internos o comodines
    if (cod.startsWith("000") || cod.startsWith("001") || cod.startsWith("002")) {
        return false;
    }

    int suma = 0;
    for (int i = 0; i < 12; ++i) {
        int d = cod[i].digitValue();
        suma += (i % 2 == 0) ? d : (d * 3);
    }
    int checkCalculado = (10 - (suma % 10)) % 10;
    int checkReal = cod[12].digitValue();

    return checkCalculado == checkReal;
}

/**
 * @brief Consulta la base de datos MariaDB para obtener los artículos a procesar.
 */
void DialogEnriquecimientoMasivo::on_pushButtonCargarLista_clicked()
{
    if (m_ejecutando) return;

    m_colaArticulos.clear();
    m_resultados.clear();
    ui->tableWidgetResultados->setRowCount(0);
    m_contadorOk = 0;
    m_contadorRevisar = 0;
    m_contadorError = 0;
    ui->labelOkValor->setText("0");
    ui->labelRevisarValor->setText("0");
    ui->labelErrorValor->setText("0");

    bool soloSinNotas = ui->checkBoxSoloSinNotas->isChecked();
    bool soloEanValido = ui->checkBoxSoloEanValido->isChecked();
    int limite = ui->spinBoxLimite->value();

    QString queryStr =
        "SELECT a.cod, a.descripcion, COALESCE(b.nombre, a.fabricante, '') AS fabricante, "
        "       COALESCE(c.descripcion, a.familia, '') AS familia, a.formato, a.notas "
        "FROM articulos a "
        "LEFT JOIN fabricantes b ON a.fabricante = b.id "
        "LEFT JOIN familias c ON a.familia = c.id "
        "WHERE a.descripcion IS NOT NULL AND TRIM(a.descripcion) != '' ";

    if (soloSinNotas) {
        queryStr += "AND (a.notas IS NULL OR TRIM(a.notas) = '') ";
    }

    if (ui->checkBoxAleatorio && ui->checkBoxAleatorio->isChecked()) {
        queryStr += "ORDER BY RAND()";
    } else {
        queryStr += "ORDER BY a.cod DESC";
    }

    QSqlDatabase db = obtenerBdLocal();
    if (!db.isOpen()) {
        ui->labelEstadoGeneral->setText(tr("❌ Error: La base de datos no está abierta."));
        return;
    }

    QSqlQuery q(db);
    if (!q.exec(queryStr)) {
        ui->labelEstadoGeneral->setText(tr("❌ Error en consulta: %1").arg(q.lastError().text()));
        return;
    }

    int cargados = 0;

    while (q.next() && cargados < limite) {
        QString cod = q.value(0).toString().trimmed();
        QString desc = q.value(1).toString().trimmed();
        QString fab = q.value(2).toString().trimmed();
        QString fam = q.value(3).toString().trimmed();
        QString formato = q.value(4).toString().trimmed();
        QString notas = q.value(5).toString().trimmed();

        if (soloEanValido && !esEan13Valido(cod)) {
            continue;
        }

        ArticuloLote art;
        art.ean = cod;
        art.descripcion = desc;
        art.fabricante = fab;
        art.familia = fam;
        art.formato = formato;
        art.notas = notas;

        m_colaArticulos.append(art);
        m_resultados.append(ResultadoFicha());

        int row = ui->tableWidgetResultados->rowCount();
        ui->tableWidgetResultados->insertRow(row);
        ui->tableWidgetResultados->setItem(row, 0, new QTableWidgetItem(art.ean));
        ui->tableWidgetResultados->setItem(row, 1, new QTableWidgetItem(art.descripcion));
        ui->tableWidgetResultados->setItem(row, 2, new QTableWidgetItem(art.fabricante));
        ui->tableWidgetResultados->setItem(row, 3, new QTableWidgetItem(art.familia));
        ui->tableWidgetResultados->setItem(row, 4, new QTableWidgetItem(tr("Pendiente")));
        ui->tableWidgetResultados->setItem(row, 5, new QTableWidgetItem("-"));
        ui->tableWidgetResultados->setItem(row, 6, new QTableWidgetItem("-"));
        ui->tableWidgetResultados->setItem(row, 7, new QTableWidgetItem(tr("Pendiente")));
        ui->tableWidgetResultados->setItem(row, 8, new QTableWidgetItem("-"));

        cargados++;
    }

    ui->labelTotalValor->setText(QString::number(m_colaArticulos.size()));
    ui->progressBarLote->setMaximum(m_colaArticulos.size());
    ui->progressBarLote->setValue(0);
    ui->labelEstadoGeneral->setText(tr("%1 artículos cargados y listos para procesar.").arg(m_colaArticulos.size()));
}

/**
 * @brief Inicia la ejecución secuencial de catalogación.
 */
void DialogEnriquecimientoMasivo::on_pushButtonIniciar_clicked()
{
    if (m_colaArticulos.isEmpty()) {
        QMessageBox::information(this, tr("Sin artículos"), tr("No hay artículos cargados en la cola de procesamiento."));
        return;
    }

    m_ejecutando = true;
    m_indiceActual = 0;

    ui->pushButtonIniciar->setEnabled(false);
    ui->pushButtonDetener->setEnabled(true);
    ui->pushButtonCargarLista->setEnabled(false);
    ui->pushButtonExcluir->setEnabled(false);

    procesarSiguiente();
}

/**
 * @brief Excluye de la lista los artículos seleccionados en la tabla para que no se generen.
 */
void DialogEnriquecimientoMasivo::on_pushButtonExcluir_clicked()
{
    if (m_ejecutando) {
        QMessageBox::information(this, tr("Proceso en curso"),
                                tr("Por favor, detén el proceso antes de excluir artículos de la lista."));
        return;
    }

    QModelIndexList selectedRows = ui->tableWidgetResultados->selectionModel()->selectedRows();
    if (selectedRows.isEmpty()) {
        int currentRow = ui->tableWidgetResultados->currentRow();
        if (currentRow >= 0) {
            selectedRows.append(ui->tableWidgetResultados->model()->index(currentRow, 0));
        }
    }

    if (selectedRows.isEmpty()) {
        QMessageBox::information(this, tr("Selección"),
                                tr("Selecciona uno o más artículos en la tabla para excluirlos de la lista."));
        return;
    }

    // Obtener lista única de filas ordenada descendentemente para no desfasar los índices
    QList<int> filas;
    for (const QModelIndex &idx : selectedRows) {
        if (!filas.contains(idx.row())) {
            filas.append(idx.row());
        }
    }
    std::sort(filas.begin(), filas.end(), std::greater<int>());

    for (int r : filas) {
        if (r >= 0 && r < m_colaArticulos.size()) {
            m_colaArticulos.removeAt(r);
        }
        if (r >= 0 && r < m_resultados.size()) {
            m_resultados.removeAt(r);
        }
        ui->tableWidgetResultados->removeRow(r);
    }

    ui->labelTotalValor->setText(QString::number(m_colaArticulos.size()));
    ui->progressBarLote->setMaximum(m_colaArticulos.size());
    ui->labelEstadoGeneral->setText(tr("Se han excluido %1 artículos. Restan %2 en la lista.")
                                   .arg(filas.size()).arg(m_colaArticulos.size()));

    int nuevoRow = ui->tableWidgetResultados->currentRow();
    actualizarPanelPrevisualizacion(nuevoRow);
}

/**
 * @brief Detiene el proceso por lotes.
 */
void DialogEnriquecimientoMasivo::on_pushButtonDetener_clicked()
{
    m_ejecutando = false;
    m_enriquecedor->cancelar();

    ui->pushButtonIniciar->setEnabled(true);
    ui->pushButtonDetener->setEnabled(false);
    ui->pushButtonCargarLista->setEnabled(true);
    ui->pushButtonExcluir->setEnabled(true);

    ui->labelEstadoGeneral->setText(tr("Proceso pausado por el usuario."));
}

/**
 * @brief Procesa el siguiente elemento de la cola.
 */
void DialogEnriquecimientoMasivo::procesarSiguiente()
{
    if (!m_ejecutando || m_indiceActual >= m_colaArticulos.size()) {
        m_ejecutando = false;
        ui->pushButtonIniciar->setEnabled(true);
        ui->pushButtonDetener->setEnabled(false);
        ui->pushButtonCargarLista->setEnabled(true);
        ui->pushButtonExcluir->setEnabled(true);
        ui->labelEstadoGeneral->setText(tr("Catalogación finalizada: %1 OK, %2 Revisar, %3 Errores.")
                                       .arg(m_contadorOk).arg(m_contadorRevisar).arg(m_contadorError));
        return;
    }

    const ArticuloLote &art = m_colaArticulos[m_indiceActual];
    ui->labelEstadoGeneral->setText(tr("Procesando [%1/%2]: %3...")
                                   .arg(QString::number(m_indiceActual + 1),
                                        QString::number(m_colaArticulos.size()),
                                        art.descripcion));

    ui->tableWidgetResultados->selectRow(m_indiceActual);

    QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(m_indiceActual, 4);
    if (itemEstado) {
        itemEstado->setText(tr("⏳ Procesando..."));
        itemEstado->setBackground(QBrush(QColor("#e0f2fe")));
    }

    m_enriquecedor->procesarArticulo(art.ean, art.descripcion, art.fabricante, art.familia, art.formato, art.notas);
}

/**
 * @brief Notifica el avance interno dentro de cada producto.
 */
void DialogEnriquecimientoMasivo::onProductoProgreso(int pasoActual, int totalPasos, const QString &mensaje)
{
    Q_UNUSED(pasoActual);
    Q_UNUSED(totalPasos);
    if (m_ejecutando && m_indiceActual >= 0 && m_indiceActual < m_colaArticulos.size()) {
        ui->labelEstadoGeneral->setText(tr("[%1/%2] %3: %4")
                                       .arg(QString::number(m_indiceActual + 1),
                                            QString::number(m_colaArticulos.size()),
                                            m_colaArticulos[m_indiceActual].descripcion,
                                            mensaje));
    }
}

/**
 * @brief Slot cuando el enriquecedor emite el resultado para el producto actual.
 */
void DialogEnriquecimientoMasivo::onProductoFinalizado(bool exito, const ResultadoFicha &resultado)
{
    if (m_indiceActual < 0 || m_indiceActual >= m_colaArticulos.size()) return;

    m_resultados[m_indiceActual] = resultado;
    const ArticuloLote &art = m_colaArticulos[m_indiceActual];

    QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(m_indiceActual, 4);
    QTableWidgetItem *itemExactitud = ui->tableWidgetResultados->item(m_indiceActual, 5);
    QTableWidgetItem *itemConfianza = ui->tableWidgetResultados->item(m_indiceActual, 6);
    QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(m_indiceActual, 7);
    QTableWidgetItem *itemDetectado = ui->tableWidgetResultados->item(m_indiceActual, 8);

    int pctExactitud = qRound(resultado.exactitud * 100);

    if (exito && resultado.estado == "ok") {
        m_contadorOk++;
        ui->labelOkValor->setText(QString::number(m_contadorOk));

        if (itemEstado) {
            itemEstado->setText(tr("✅ OK"));
            itemEstado->setBackground(QBrush(QColor("#dcfce7")));
            itemEstado->setForeground(QBrush(QColor("#15803d")));
        }
        if (itemExactitud) itemExactitud->setText(QString("%1%").arg(pctExactitud));
        if (itemConfianza) itemConfianza->setText(resultado.confianza.toUpper());
        if (itemDetectado) itemDetectado->setText(resultado.productoDetectado);

        if (itemGuardado) {
            itemGuardado->setText(tr("⏸️ Pendiente aceptar"));
            itemGuardado->setForeground(QBrush(QColor("#0369a1")));
            itemGuardado->setBackground(QBrush(QColor("#e0f2fe")));
        }
    } else if (exito && resultado.estado == "revisar") {
        m_contadorRevisar++;
        ui->labelRevisarValor->setText(QString::number(m_contadorRevisar));

        if (itemEstado) {
            itemEstado->setText(tr("⚠️ REVISAR"));
            itemEstado->setBackground(QBrush(QColor("#fef3c7")));
            itemEstado->setForeground(QBrush(QColor("#b45309")));
        }
        if (itemExactitud) itemExactitud->setText(QString("%1%").arg(pctExactitud));
        if (itemConfianza) itemConfianza->setText(resultado.confianza.toUpper());
        if (itemDetectado) itemDetectado->setText(resultado.productoDetectado.isEmpty() ? tr("No coincide") : resultado.productoDetectado);
        if (itemGuardado) {
            itemGuardado->setText(tr("⚠️ Requiere revisar"));
            itemGuardado->setForeground(QBrush(QColor("#b45309")));
            itemGuardado->setBackground(QBrush(QColor("#fef3c7")));
        }
    } else {
        m_contadorError++;
        ui->labelErrorValor->setText(QString::number(m_contadorError));

        if (itemEstado) {
            itemEstado->setText(tr("❌ ERROR"));
            itemEstado->setBackground(QBrush(QColor("#fee2e2")));
            itemEstado->setForeground(QBrush(QColor("#b91c1c")));
        }
        if (itemExactitud) itemExactitud->setText("0%");
        if (itemConfianza) itemConfianza->setText("BAJA");
        if (itemDetectado) itemDetectado->setText(resultado.error);
        if (itemGuardado) {
            itemGuardado->setText(tr("❌ No guardado"));
            itemGuardado->setForeground(QBrush(QColor("#b91c1c")));
            itemGuardado->setBackground(QBrush(QColor("#fee2e2")));
        }
    }

    ui->progressBarLote->setValue(m_indiceActual + 1);

    // Seleccionar y previsualizar en el panel inferior
    ui->tableWidgetResultados->selectRow(m_indiceActual);
    actualizarPanelPrevisualizacion(m_indiceActual);

    // Si el modo interactivo está activado, dar oportunidad de aceptar o rechazar individualmente
    if (ui->checkBoxModoInteractivo->isChecked() && exito && !resultado.htmlFormateado.isEmpty()) {
        bool continuar = mostrarDialogoRevision(m_indiceActual);
        if (!continuar) {
            m_ejecutando = false;
            ui->pushButtonIniciar->setEnabled(true);
            ui->pushButtonDetener->setEnabled(false);
            ui->pushButtonCargarLista->setEnabled(true);
            ui->labelEstadoGeneral->setText(tr("Proceso pausado tras la revisión individual."));
            return;
        }
    } else if (ui->checkBoxActualizarBD->isChecked() && exito && resultado.estado == "ok") {
        // Modo auto-guardado desatendido
        guardarEnBaseDatos(art.ean, resultado.htmlFormateado);
        if (itemGuardado) {
            itemGuardado->setText(tr("✅ Guardado en BD"));
            itemGuardado->setForeground(QBrush(QColor("#15803d")));
            itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
        }
    }

    m_indiceActual++;
    procesarSiguiente();
}

/**
 * @brief Actualiza los datos mostrados en el panel inferior de previsualización según la fila seleccionada.
 */
void DialogEnriquecimientoMasivo::actualizarPanelPrevisualizacion(int row)
{
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        ui->labelFichaTitulo->setText(tr("Selecciona un artículo en la tabla para revisar su ficha."));
        ui->labelFichaBadge->clear();
        ui->textEditFichaDetalle->clear();
        ui->labelFuentesFicha->clear();
        ui->labelFuentesFicha->setVisible(false);
        ui->pushButtonAceptarFicha->setEnabled(false);
        ui->pushButtonRechazarFicha->setEnabled(false);
        return;
    }

    const ArticuloLote &art = m_colaArticulos[row];
    const ResultadoFicha &res = m_resultados[row];

    ui->labelFichaTitulo->setText(QString("<b>%1</b> | EAN: %2 | Fab: %3 | Fam: %4")
                                  .arg(art.descripcion, art.ean, art.fabricante, art.familia));

    if (res.htmlFormateado.isEmpty() && res.error.isEmpty()) {
        ui->labelFichaBadge->setText(tr("⏳ Pendiente"));
        ui->labelFichaBadge->setStyleSheet("color: #64748b; font-weight: bold;");
        ui->textEditFichaDetalle->clear();
        ui->labelFuentesFicha->clear();
        ui->labelFuentesFicha->setVisible(false);
        ui->pushButtonAceptarFicha->setEnabled(false);
        ui->pushButtonRechazarFicha->setEnabled(false);
        return;
    }

    int pctExactitud = qRound(res.exactitud * 100);

    if (res.estado == "ok") {
        ui->labelFichaBadge->setText(tr("✅ OK (%1% exactitud | Confianza: %2)")
                                    .arg(pctExactitud).arg(res.confianza.toUpper()));
        ui->labelFichaBadge->setStyleSheet("background-color:#dcfce7; color:#15803d; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #a5d6a7;");
    } else if (res.estado == "revisar") {
        ui->labelFichaBadge->setText(tr("⚠️ Requiere Revisión (%1% exactitud | Confianza: %2)")
                                    .arg(pctExactitud).arg(res.confianza.toUpper()));
        ui->labelFichaBadge->setStyleSheet("background-color:#fef3c7; color:#b45309; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #ffcc80;");
    } else if (res.estado == "rechazado") {
        ui->labelFichaBadge->setText(tr("❌ Rechazado"));
        ui->labelFichaBadge->setStyleSheet("background-color:#fee2e2; color:#b91c1c; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #fca5a5;");
    } else {
        ui->labelFichaBadge->setText(tr("❌ Error: %1").arg(res.error));
        ui->labelFichaBadge->setStyleSheet("background-color:#fee2e2; color:#b91c1c; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #ef9a9a;");
    }

    ui->textEditFichaDetalle->setHtml(res.htmlFormateado);

    // Mostrar fuentes consultadas en el panel inferior sin añadirlas a la ficha
    if (!res.fuentes.isEmpty()) {
        QStringList links;
        for (int i = 0; i < res.fuentes.size() && i < 3; ++i) {
            const FuenteWeb &f = res.fuentes[i];
            QString badge = f.oficial ? "<b><span style='color:#166534;'>[Oficial]</span></b>" : "<b><span style='color:#1e40af;'>[Web]</span></b>";
            QString titulo = f.titulo.isEmpty() ? f.url : f.titulo;
            if (titulo.length() > 55) titulo = titulo.left(52) + "...";
            links << QString("%1 <a href=\"%2\" style=\"color:#0284c7; text-decoration:none;\">%3</a>").arg(badge, f.url, titulo);
        }
        ui->labelFuentesFicha->setText(tr("🌐 <b>Fuentes consultadas:</b> %1").arg(links.join(" &nbsp;|&nbsp; ")));
        ui->labelFuentesFicha->setVisible(true);
    } else {
        ui->labelFuentesFicha->clear();
        ui->labelFuentesFicha->setVisible(false);
    }

    ui->pushButtonAceptarFicha->setEnabled(!res.htmlFormateado.isEmpty());
    ui->pushButtonRechazarFicha->setEnabled(true);
}

/**
 * @brief Slot que se dispara al cambiar la fila seleccionada en la tabla.
 */
void DialogEnriquecimientoMasivo::on_tableWidgetResultados_itemSelectionChanged()
{
    int row = ui->tableWidgetResultados->currentRow();
    actualizarPanelPrevisualizacion(row);
}

/**
 * @brief Al marcar el modo interactivo se desmarca el auto-guardado directo.
 */
void DialogEnriquecimientoMasivo::on_checkBoxModoInteractivo_toggled(bool checked)
{
    if (checked && ui->checkBoxActualizarBD->isChecked()) {
        ui->checkBoxActualizarBD->setChecked(false);
    }
}

/**
 * @brief Al marcar el auto-guardado directo se desmarca el modo interactivo.
 */
void DialogEnriquecimientoMasivo::on_checkBoxActualizarBD_toggled(bool checked)
{
    if (checked && ui->checkBoxModoInteractivo->isChecked()) {
        ui->checkBoxModoInteractivo->setChecked(false);
    }
}

/**
 * @brief Actualiza el campo notas en la tabla articulos de MariaDB.
 */
bool DialogEnriquecimientoMasivo::guardarEnBaseDatos(const QString &ean, const QString &htmlNotas)
{
    if (ean.isEmpty() || htmlNotas.isEmpty()) return false;

    QSqlDatabase db = obtenerBdLocal();
    if (!db.isOpen()) return false;

    QSqlQuery q(db);
    q.prepare("UPDATE articulos SET notas = :notas WHERE cod = :cod");
    q.bindValue(":notas", htmlNotas);
    q.bindValue(":cod", ean);

    if (!q.exec()) {
        qDebug() << "[DialogEnriquecimientoMasivo] Error actualizando articulo" << ean << ":" << q.lastError().text();
        return false;
    }
    return true;
}

/**
 * @brief Acepta y guarda en la base de datos la ficha del artículo actualmente seleccionado.
 */
void DialogEnriquecimientoMasivo::on_pushButtonAceptarFicha_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        QMessageBox::information(this, tr("Selección"), tr("Selecciona un artículo en la tabla para aceptar su ficha."));
        return;
    }

    const ArticuloLote &art = m_colaArticulos[row];
    ResultadoFicha &res = m_resultados[row];

    // Tomar el texto directamente del editor por si el usuario lo retocó
    QString htmlActualizado = ui->textEditFichaDetalle->toHtml().trimmed();
    if (htmlActualizado.isEmpty()) {
        htmlActualizado = res.htmlFormateado;
    }

    if (htmlActualizado.isEmpty()) {
        QMessageBox::warning(this, tr("Sin ficha"), tr("Este artículo aún no tiene una ficha técnica generada para aceptar."));
        return;
    }

    if (guardarEnBaseDatos(art.ean, htmlActualizado)) {
        res.htmlFormateado = htmlActualizado;
        res.estado = "ok";
        QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
        if (itemGuardado) {
            itemGuardado->setText(tr("✅ Guardado en BD"));
            itemGuardado->setForeground(QBrush(QColor("#15803d")));
            itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
        }
        actualizarPanelPrevisualizacion(row);
        ui->labelEstadoGeneral->setText(tr("Ficha de '%1' guardada con éxito en la base de datos.").arg(art.descripcion));
    } else {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo guardar la ficha en la base de datos."));
    }
}

/**
 * @brief Marca como rechazada la ficha del artículo actualmente seleccionado.
 */
void DialogEnriquecimientoMasivo::on_pushButtonRechazarFicha_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        QMessageBox::information(this, tr("Selección"), tr("Selecciona un artículo en la tabla para rechazar su ficha."));
        return;
    }

    ResultadoFicha &res = m_resultados[row];
    res.estado = "rechazado";

    QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
    if (itemGuardado) {
        itemGuardado->setText(tr("❌ Rechazado"));
        itemGuardado->setForeground(QBrush(QColor("#b91c1c")));
        itemGuardado->setBackground(QBrush(QColor("#fee2e2")));
    }
    actualizarPanelPrevisualizacion(row);
    ui->labelEstadoGeneral->setText(tr("Ficha de '%1' descartada.").arg(m_colaArticulos[row].descripcion));
}

/**
 * @brief Acepta y guarda en lote todas las fichas en estado OK que estén aún pendientes de aceptar.
 */
void DialogEnriquecimientoMasivo::on_pushButtonAceptarTodasPendientes_clicked()
{
    int guardados = 0;
    for (int i = 0; i < m_colaArticulos.size() && i < m_resultados.size(); ++i) {
        const ResultadoFicha &res = m_resultados[i];
        QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(i, 7);
        QString textoGuardado = itemGuardado ? itemGuardado->text() : "";

        if (res.estado == "ok" && !res.htmlFormateado.isEmpty() && textoGuardado.contains("Pendiente")) {
            if (guardarEnBaseDatos(m_colaArticulos[i].ean, res.htmlFormateado)) {
                if (itemGuardado) {
                    itemGuardado->setText(tr("✅ Guardado en BD"));
                    itemGuardado->setForeground(QBrush(QColor("#15803d")));
                    itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
                }
                guardados++;
            }
        }
    }

    actualizarPanelPrevisualizacion(ui->tableWidgetResultados->currentRow());

    if (guardados > 0) {
        ui->labelEstadoGeneral->setText(tr("Se guardaron en MariaDB %1 fichas pendientes.").arg(guardados));
        QMessageBox::information(this, tr("Fichas Guardadas"),
                                 tr("Se han aceptado y guardado %1 fichas verificadas en la base de datos.").arg(guardados));
    } else {
        QMessageBox::information(this, tr("Sin fichas pendientes"),
                                 tr("No hay fichas en estado OK pendientes de aceptar."));
    }
}

/**
 * @brief Muestra diálogo interactivo modal para aceptar, editar o rechazar la ficha de un artículo.
 * @return true si el proceso debe continuar con el siguiente artículo, false si el usuario detuvo el lote.
 */
bool DialogEnriquecimientoMasivo::mostrarDialogoRevision(int row)
{
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) return true;

    ResultadoFicha &res = m_resultados[row];
    const ArticuloLote &art = m_colaArticulos[row];

    if (res.htmlFormateado.isEmpty() && res.error.isEmpty()) {
        QMessageBox::information(this, tr("Ficha no generada"), tr("Este artículo aún no ha sido procesado."));
        return true;
    }

    QDialog dlg(this);
    dlg.setWindowTitle(tr("Revisión Individual de Ficha — %1 (%2)").arg(art.descripcion, art.ean));
    dlg.resize(750, 580);

    QVBoxLayout *layout = new QVBoxLayout(&dlg);

    // Cabecera con datos del producto
    QLabel *labelHeader = new QLabel(QString("<h3>%1</h3><p><b>EAN:</b> %2 | <b>Fabricante:</b> %3 | <b>Familia:</b> %4</p>")
                                     .arg(art.descripcion, art.ean, art.fabricante, art.familia), &dlg);
    layout->addWidget(labelHeader);

    // Insignia de estado y exactitud
    QLabel *labelBadge = new QLabel(&dlg);
    if (res.estado == "ok") {
        labelBadge->setText(tr("✅ Estado IA: VERIFICADO (OK) | Exactitud léxica: %1% | Confianza: %2")
                            .arg(QString::number(qRound(res.exactitud * 100)), res.confianza.toUpper()));
        labelBadge->setStyleSheet("background-color:#dcfce7; color:#15803d; padding:8px; border-radius:4px; font-weight:bold; border:1px solid #a5d6a7;");
    } else if (res.estado == "revisar") {
        labelBadge->setText(tr("⚠️ Estado IA: REQUIERE REVISIÓN | Exactitud léxica: %1% | Confianza: %2")
                            .arg(QString::number(qRound(res.exactitud * 100)), res.confianza.toUpper()));
        labelBadge->setStyleSheet("background-color:#fef3c7; color:#b45309; padding:8px; border-radius:4px; font-weight:bold; border:1px solid #ffcc80;");
    } else {
        labelBadge->setText(tr("❌ Estado IA: ERROR — %1").arg(res.error));
        labelBadge->setStyleSheet("background-color:#fee2e2; color:#b91c1c; padding:8px; border-radius:4px; font-weight:bold; border:1px solid #ef9a9a;");
    }
    layout->addWidget(labelBadge);

    // Editor de texto enriquecido editable
    QTextEdit *editor = new QTextEdit(&dlg);
    editor->setHtml(res.htmlFormateado);
    layout->addWidget(editor);

    // Botonera de decisión
    QHBoxLayout *btnLayout = new QHBoxLayout();

    QPushButton *btnAceptar = new QPushButton(tr("✅ Aceptar y Guardar en BD"), &dlg);
    btnAceptar->setStyleSheet("background-color: #2e7d32; color: white; font-weight: bold; border-radius: 4px; padding: 7px 14px;");

    QPushButton *btnRechazar = new QPushButton(tr("❌ Rechazar Ficha"), &dlg);
    btnRechazar->setStyleSheet("background-color: #fee2e2; color: #b91c1c; font-weight: bold; border-radius: 4px; padding: 7px 14px; border: 1px solid #fecaca;");

    QPushButton *btnOmitir = new QPushButton(tr("⏭️ Omitir"), &dlg);
    btnOmitir->setToolTip(tr("Pasa al siguiente artículo dejando esta ficha pendiente sin guardar"));

    QPushButton *btnAuto = new QPushButton(tr("⏩ Auto-Aceptar Siguientes OK"), &dlg);
    btnAuto->setToolTip(tr("Acepta esta ficha y continúa el lote procesando automáticamente las siguientes sin volver a preguntar"));

    QPushButton *btnDetener = new QPushButton(tr("⏹️ Detener Lote"), &dlg);

    btnLayout->addWidget(btnAceptar);
    btnLayout->addWidget(btnRechazar);
    btnLayout->addWidget(btnOmitir);
    btnLayout->addWidget(btnAuto);
    btnLayout->addStretch();
    btnLayout->addWidget(btnDetener);
    layout->addLayout(btnLayout);

    bool continuarLote = true;

    connect(btnAceptar, &QPushButton::clicked, [&]() {
        QString htmlActualizado = editor->toHtml().trimmed();
        if (guardarEnBaseDatos(art.ean, htmlActualizado)) {
            res.htmlFormateado = htmlActualizado;
            res.estado = "ok";
            QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
            if (itemGuardado) {
                itemGuardado->setText(tr("✅ Guardado en BD"));
                itemGuardado->setForeground(QBrush(QColor("#15803d")));
                itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
            }
            actualizarPanelPrevisualizacion(row);
            ui->labelEstadoGeneral->setText(tr("Ficha de '%1' aceptada y guardada en BD.").arg(art.descripcion));
            continuarLote = true;
            dlg.accept();
        } else {
            QMessageBox::critical(&dlg, tr("Error"), tr("No se pudo guardar la ficha en la base de datos."));
        }
    });

    connect(btnRechazar, &QPushButton::clicked, [&]() {
        res.estado = "rechazado";
        QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
        if (itemGuardado) {
            itemGuardado->setText(tr("❌ Rechazado"));
            itemGuardado->setForeground(QBrush(QColor("#b91c1c")));
            itemGuardado->setBackground(QBrush(QColor("#fee2e2")));
        }
        actualizarPanelPrevisualizacion(row);
        ui->labelEstadoGeneral->setText(tr("Ficha de '%1' descartada.").arg(art.descripcion));
        continuarLote = true;
        dlg.reject();
    });

    connect(btnOmitir, &QPushButton::clicked, [&]() {
        continuarLote = true;
        dlg.close();
    });

    connect(btnAuto, &QPushButton::clicked, [&]() {
        QString htmlActualizado = editor->toHtml().trimmed();
        if (guardarEnBaseDatos(art.ean, htmlActualizado)) {
            res.htmlFormateado = htmlActualizado;
            res.estado = "ok";
            QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
            if (itemGuardado) {
                itemGuardado->setText(tr("✅ Guardado en BD"));
                itemGuardado->setForeground(QBrush(QColor("#15803d")));
                itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
            }
        }
        ui->checkBoxModoInteractivo->setChecked(false);
        ui->checkBoxActualizarBD->setChecked(true);
        continuarLote = true;
        dlg.accept();
    });

    connect(btnDetener, &QPushButton::clicked, [&]() {
        continuarLote = false;
        dlg.close();
    });

    dlg.exec();
    return continuarLote;
}

/**
 * @brief Abre el visor interactivo de ficha en pantalla completa.
 */
void DialogEnriquecimientoMasivo::on_pushButtonVerFicha_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_resultados.size()) {
        QMessageBox::information(this, tr("Selección"), tr("Selecciona una fila de la tabla para ver su ficha."));
        return;
    }
    mostrarDialogoRevision(row);
}

void DialogEnriquecimientoMasivo::on_tableWidgetResultados_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    mostrarDialogoRevision(row);
}

void DialogEnriquecimientoMasivo::on_pushButtonCerrar_clicked()
{
    if (m_ejecutando) {
        if (QMessageBox::question(this, tr("Proceso en curso"),
                                  tr("Hay un proceso de catalogación en ejecución. ¿Deseas detenerlo y salir?"),
                                  QMessageBox::Yes | QMessageBox::No) == QMessageBox::No) {
            return;
        }
        m_enriquecedor->cancelar();
    }
    accept();
}
