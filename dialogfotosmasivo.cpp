#include "dialogfotosmasivo.h"
#include "ui_dialogfotosmasivo.h"
#include "base_datos.h"
#include "configuracion.h"
#include "visorimagenes.h"

#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QDir>
#include <QFileInfo>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QTextDocument>
#include <QEventLoop>
#include <QTimer>
#include <QDebug>

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
 * @brief Constructor del diálogo DialogFotosMasivo.
 */
DialogFotosMasivo::DialogFotosMasivo(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogFotosMasivo),
      m_netManager(new QNetworkAccessManager(this)),
      m_indiceActual(-1),
      m_ejecutando(false),
      m_contadorOk(0),
      m_contadorSinFoto(0),
      m_contadorError(0)
{
    ui->setupUi(this);

    // Obtener y asegurar el directorio configurado para imágenes del ERP
    baseDatos base;
    m_directorioImagenes = base.devolverDirectorio("imagenes");
    if (m_directorioImagenes.isEmpty()) {
        m_directorioImagenes = "./imagenes";
    }
    QDir().mkpath(m_directorioImagenes);

    // Configurar galería de miniaturas
    ui->listWidgetCandidatas->setIconSize(QSize(100, 100));
    ui->listWidgetCandidatas->setGridSize(QSize(115, 120));

    // Configurar proporciones del divisor y tabla
    ui->splitterResultados->setSizes(QList<int>() << 380 << 260);
    configurarTabla();

    ui->pushButtonAceptarFoto->setEnabled(false);
    ui->pushButtonRechazarFoto->setEnabled(false);
    ui->pushButtonVerGrande->setEnabled(false);

    // Cargar artículos inicialmente al abrir
    on_pushButtonCargarLista_clicked();
}

DialogFotosMasivo::~DialogFotosMasivo()
{
    m_ejecutando = false;
    delete ui;
}

/**
 * @brief Configura las columnas, cabeceras y modo de selección de la tabla.
 */
void DialogFotosMasivo::configurarTabla()
{
    QStringList cabeceras;
    cabeceras << tr("EAN")
              << tr("Descripción")
              << tr("Fabricante")
              << tr("Familia")
              << tr("Estado Búsqueda")
              << tr("Candidatas")
              << tr("Resolución")
              << tr("Guardado en BD");

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

    ui->tableWidgetResultados->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetResultados->setSelectionMode(QAbstractItemView::ExtendedSelection);
}

/**
 * @brief Valida si un código de barras cumple con el algoritmo checksum de EAN-13.
 */
bool DialogFotosMasivo::esEan13Valido(const QString &cod) const
{
    if (cod.length() != 13) return false;
    for (const QChar &c : cod) {
        if (!c.isDigit()) return false;
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
 * @brief Consulta la base de datos para cargar los artículos a procesar.
 */
void DialogFotosMasivo::on_pushButtonCargarLista_clicked()
{
    if (m_ejecutando) return;

    m_colaArticulos.clear();
    m_resultados.clear();
    ui->tableWidgetResultados->setRowCount(0);
    m_contadorOk = 0;
    m_contadorSinFoto = 0;
    m_contadorError = 0;
    actualizarMetricas();

    bool soloSinFoto = ui->checkBoxSoloSinFoto->isChecked();
    bool aleatorio = ui->checkBoxAleatorio->isChecked();
    bool soloEanValido = ui->checkBoxSoloEanValido->isChecked();
    int limite = ui->spinBoxLimite->value();

    QString queryStr =
        "SELECT a.cod, a.descripcion, COALESCE(b.nombre, a.fabricante, '') AS fabricante, "
        "       COALESCE(c.descripcion, a.familia, '') AS familia, a.formato, a.foto "
        "FROM articulos a "
        "LEFT JOIN fabricantes b ON a.fabricante = b.id "
        "LEFT JOIN familias c ON a.familia = c.id "
        "WHERE a.descripcion IS NOT NULL AND TRIM(a.descripcion) != '' ";

    if (soloSinFoto) {
        queryStr += "AND (a.foto IS NULL OR TRIM(a.foto) = '') ";
    }

    if (aleatorio) {
        queryStr += "ORDER BY RAND() ";
    } else {
        queryStr += "ORDER BY a.cod DESC ";
    }

    QSqlDatabase db = obtenerBdLocal();
    if (!db.isOpen()) {
        ui->labelEstadoGeneral->setText(tr("❌ Error: La base de datos no está disponible."));
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
        QString foto = q.value(5).toString().trimmed();

        if (soloEanValido && !esEan13Valido(cod)) {
            continue;
        }

        ArticuloFotoLote art;
        art.ean = cod;
        art.descripcion = desc;
        art.fabricante = fab;
        art.familia = fam;
        art.formato = formato;
        art.fotoActual = foto;

        m_colaArticulos.append(art);
        m_resultados.append(ResultadoFotoArticulo());

        int row = ui->tableWidgetResultados->rowCount();
        ui->tableWidgetResultados->insertRow(row);
        ui->tableWidgetResultados->setItem(row, 0, new QTableWidgetItem(art.ean));
        ui->tableWidgetResultados->setItem(row, 1, new QTableWidgetItem(art.descripcion));
        ui->tableWidgetResultados->setItem(row, 2, new QTableWidgetItem(art.fabricante));
        ui->tableWidgetResultados->setItem(row, 3, new QTableWidgetItem(art.familia));
        ui->tableWidgetResultados->setItem(row, 4, new QTableWidgetItem(tr("Pendiente")));
        ui->tableWidgetResultados->setItem(row, 5, new QTableWidgetItem("-"));
        ui->tableWidgetResultados->setItem(row, 6, new QTableWidgetItem("-"));
        ui->tableWidgetResultados->setItem(row, 7, new QTableWidgetItem(art.fotoActual.isEmpty() ? tr("Sin foto") : tr("Foto existente")));

        cargados++;
    }

    ui->labelTotalValor->setText(QString::number(m_colaArticulos.size()));
    ui->progressBarLote->setMaximum(m_colaArticulos.size());
    ui->progressBarLote->setValue(0);
    ui->labelEstadoGeneral->setText(tr("%1 artículos cargados y listos para buscar fotos.").arg(m_colaArticulos.size()));
}

/**
 * @brief Excluye de la lista los artículos seleccionados en la tabla.
 */
void DialogFotosMasivo::on_pushButtonExcluir_clicked()
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

    // Obtener lista única de filas ordenada descendentemente
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
 * @brief Inicia la búsqueda secuencial por lotes de fotos en internet.
 */
void DialogFotosMasivo::on_pushButtonIniciar_clicked()
{
    if (m_colaArticulos.isEmpty()) {
        QMessageBox::information(this, tr("Sin artículos"), tr("No hay artículos en la lista para buscar fotos."));
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
 * @brief Detiene la ejecución en curso.
 */
void DialogFotosMasivo::on_pushButtonDetener_clicked()
{
    m_ejecutando = false;

    ui->pushButtonIniciar->setEnabled(true);
    ui->pushButtonDetener->setEnabled(false);
    ui->pushButtonCargarLista->setEnabled(true);
    ui->pushButtonExcluir->setEnabled(true);

    ui->labelEstadoGeneral->setText(tr("Búsqueda pausada por el usuario."));
}

/**
 * @brief Procesa el siguiente artículo en la cola de búsqueda.
 */
void DialogFotosMasivo::procesarSiguiente()
{
    if (!m_ejecutando || m_indiceActual >= m_colaArticulos.size()) {
        m_ejecutando = false;
        ui->pushButtonIniciar->setEnabled(true);
        ui->pushButtonDetener->setEnabled(false);
        ui->pushButtonCargarLista->setEnabled(true);
        ui->pushButtonExcluir->setEnabled(true);
        ui->labelEstadoGeneral->setText(tr("Búsqueda finalizada: %1 con fotos, %2 sin fotos, %3 errores.")
                                       .arg(m_contadorOk).arg(m_contadorSinFoto).arg(m_contadorError));
        return;
    }

    const ArticuloFotoLote &art = m_colaArticulos[m_indiceActual];
    ui->labelEstadoGeneral->setText(tr("Buscando fotos [%1/%2]: %3...")
                                   .arg(QString::number(m_indiceActual + 1),
                                        QString::number(m_colaArticulos.size()),
                                        art.descripcion));

    ui->tableWidgetResultados->selectRow(m_indiceActual);

    QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(m_indiceActual, 4);
    if (itemEstado) {
        itemEstado->setText(tr("⏳ Buscando fotos..."));
        itemEstado->setBackground(QBrush(QColor("#e0f2fe")));
    }

    buscarFotosArticulo(m_indiceActual);
}

/**
 * @brief Envía la consulta asíncrona de búsqueda de imágenes a Bing Images.
 */
void DialogFotosMasivo::buscarFotosArticulo(int index)
{
    if (index < 0 || index >= m_colaArticulos.size()) return;

    const ArticuloFotoLote &art = m_colaArticulos[index];

    QString termino = art.descripcion;
    if (!art.fabricante.isEmpty() && !termino.contains(art.fabricante, Qt::CaseInsensitive)) {
        termino += " " + art.fabricante;
    }

    QUrl url("https://www.bing.com/images/async");
    QUrlQuery query;
    query.addQueryItem("q", termino);
    query.addQueryItem("async", "1");
    query.addQueryItem("first", "1");
    query.addQueryItem("count", "30");
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
    req.setRawHeader("Accept", "*/*");
    req.setRawHeader("Accept-Language", "es-ES,es;q=0.9,en;q=0.8");

    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, index]() {
        onRespuestaBusquedaTerminada(reply, index);
    });
}

/**
 * @brief Procesa el HTML de imágenes y extrae las tarjetas candidatas.
 */
void DialogFotosMasivo::onRespuestaBusquedaTerminada(QNetworkReply *reply, int index)
{
    reply->deleteLater();

    if (index < 0 || index >= m_colaArticulos.size() || index >= m_resultados.size()) {
        return;
    }

    ResultadoFotoArticulo &res = m_resultados[index];
    res.candidatas.clear();

    if (reply->error() != QNetworkReply::NoError) {
        res.estado = "error";
        res.error = reply->errorString();
        m_contadorError++;
        actualizarMetricas();

        QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(index, 4);
        if (itemEstado) {
            itemEstado->setText(tr("❌ Error"));
            itemEstado->setBackground(QBrush(QColor("#fee2e2")));
            itemEstado->setForeground(QBrush(QColor("#b91c1c")));
        }

        ui->progressBarLote->setValue(index + 1);
        if (m_ejecutando) {
            m_indiceActual++;
            procesarSiguiente();
        }
        return;
    }

    QString htmlContent = QString::fromUtf8(reply->readAll());

    QRegularExpression reCard("<a\\b[^>]*?class=\"[^\"]*?\\biusc\\b[^\"]*?\"[^>]*>");
    QRegularExpressionMatchIterator it = reCard.globalMatch(htmlContent);

    QRegularExpression reM("m=\"([^\"]+)\"");
    QRegularExpression reH("exph=(\\d+)");
    QRegularExpression reW("expw=(\\d+)");

    while (it.hasNext() && res.candidatas.size() < 24) {
        QRegularExpressionMatch matchCard = it.next();
        QString cardTag = matchCard.captured(0);

        QRegularExpressionMatch matchM = reM.match(cardTag);
        if (!matchM.hasMatch()) continue;

        QString rawM = matchM.captured(1);
        rawM.replace("&quot;", "\"");
        rawM.replace("&amp;", "&");
        rawM.replace("&lt;", "<");
        rawM.replace("&gt;", ">");

        QJsonDocument doc = QJsonDocument::fromJson(rawM.toUtf8());
        if (!doc.isObject()) continue;

        QJsonObject obj = doc.object();
        QString imgUrl = obj.value("murl").toString();
        QString thumbUrl = obj.value("turl").toString();
        QString title = obj.value("t").toString();

        if (imgUrl.isEmpty()) continue;

        QTextDocument textDoc;
        textDoc.setHtml(title);
        title = textDoc.toPlainText().trimmed();

        FotoCandidata cand;
        cand.urlImagen = imgUrl;
        cand.urlThumbnail = thumbUrl.isEmpty() ? imgUrl : thumbUrl;
        cand.titulo = title;

        QRegularExpressionMatch matchH = reH.match(cardTag);
        if (matchH.hasMatch()) cand.alto = matchH.captured(1).toInt();

        QRegularExpressionMatch matchW = reW.match(cardTag);
        if (matchW.hasMatch()) cand.ancho = matchW.captured(1).toInt();

        res.candidatas.append(cand);
    }

    if (res.candidatas.isEmpty()) {
        res.estado = "sin_foto";
        m_contadorSinFoto++;
        actualizarMetricas();

        QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(index, 4);
        if (itemEstado) {
            itemEstado->setText(tr("⚠️ Sin fotos"));
            itemEstado->setBackground(QBrush(QColor("#fef3c7")));
            itemEstado->setForeground(QBrush(QColor("#b45309")));
        }

        ui->progressBarLote->setValue(index + 1);
        if (m_ejecutando) {
            m_indiceActual++;
            procesarSiguiente();
        }
        return;
    }

    // Fotos encontradas con éxito
    res.estado = "ok";
    res.candidatoSeleccionado = 0;
    m_contadorOk++;
    actualizarMetricas();

    QTableWidgetItem *itemEstado = ui->tableWidgetResultados->item(index, 4);
    if (itemEstado) {
        itemEstado->setText(tr("✅ %1 fotos").arg(res.candidatas.size()));
        itemEstado->setBackground(QBrush(QColor("#dcfce7")));
        itemEstado->setForeground(QBrush(QColor("#15803d")));
    }

    QTableWidgetItem *itemCand = ui->tableWidgetResultados->item(index, 5);
    if (itemCand) {
        itemCand->setText(QString::number(res.candidatas.size()));
    }

    QTableWidgetItem *itemRes = ui->tableWidgetResultados->item(index, 6);
    if (itemRes && !res.candidatas.isEmpty()) {
        itemRes->setText(QString("%1x%2").arg(res.candidatas[0].ancho).arg(res.candidatas[0].alto));
    }

    ui->progressBarLote->setValue(index + 1);

    // Descargar miniaturas para la galería
    descargarMiniaturas(index);

    // Modo automático: descargar y guardar automáticamente la primera foto
    if (ui->checkBoxActualizarBD->isChecked()) {
        descargarYAsignarFoto(index, 0);
        if (m_ejecutando) {
            m_indiceActual++;
            procesarSiguiente();
        }
    } else {
        // Modo interactivo: pausar para que el usuario examine las fotos
        actualizarPanelPrevisualizacion(index);
    }
}

/**
 * @brief Descarga las miniaturas de las imágenes candidatas en segundo plano.
 */
void DialogFotosMasivo::descargarMiniaturas(int index)
{
    if (index < 0 || index >= m_resultados.size()) return;

    ResultadoFotoArticulo &res = m_resultados[index];
    int limiteMiniaturas = qMin(res.candidatas.size(), 12);

    for (int c = 0; c < limiteMiniaturas; ++c) {
        if (!res.candidatas[c].thumbnail.isNull()) continue;

        QString urlThumb = res.candidatas[c].urlThumbnail;
        QNetworkRequest reqThumb;
        reqThumb.setUrl(QUrl(urlThumb));
        reqThumb.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        reqThumb.setHeader(QNetworkRequest::UserAgentHeader,
                           "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

        QNetworkReply *replyThumb = m_netManager->get(reqThumb);
        connect(replyThumb, &QNetworkReply::finished, this, [this, replyThumb, index, c]() {
            replyThumb->deleteLater();
            if (index < 0 || index >= m_resultados.size()) return;
            ResultadoFotoArticulo &resRef = m_resultados[index];
            if (c < 0 || c >= resRef.candidatas.size()) return;

            if (replyThumb->error() == QNetworkReply::NoError) {
                QPixmap pix;
                pix.loadFromData(replyThumb->readAll());
                if (!pix.isNull()) {
                    resRef.candidatas[c].thumbnail = pix;
                    // Si el artículo actualmente mostrado en el panel es este, refrescar la galería
                    if (ui->tableWidgetResultados->currentRow() == index) {
                        actualizarPanelPrevisualizacion(index);
                    }
                }
            }
        });
    }
}

/**
 * @brief Descarga la foto en resolución completa, la guarda en disco y actualiza la BD.
 */
bool DialogFotosMasivo::descargarYAsignarFoto(int rowArticulo, int indexCandidato)
{
    if (rowArticulo < 0 || rowArticulo >= m_colaArticulos.size() || rowArticulo >= m_resultados.size()) {
        return false;
    }

    const ArticuloFotoLote &art = m_colaArticulos[rowArticulo];
    ResultadoFotoArticulo &res = m_resultados[rowArticulo];

    if (indexCandidato < 0 || indexCandidato >= res.candidatas.size()) {
        return false;
    }

    const FotoCandidata &cand = res.candidatas[indexCandidato];

    // Determinar extensión del archivo
    QString ext = ".jpg";
    QString urlLower = cand.urlImagen.toLower();
    if (urlLower.endsWith(".png")) ext = ".png";
    else if (urlLower.endsWith(".webp")) ext = ".webp";
    else if (urlLower.endsWith(".jpeg")) ext = ".jpeg";

    QString eanLimpio = art.ean.trimmed();
    QString nombreArchivo = eanLimpio.isEmpty() ? "prod_" + QString::number(QDateTime::currentMSecsSinceEpoch()) : eanLimpio;
    nombreArchivo += ext;

    QDir dirImagenes(m_directorioImagenes);
    QString rutaDestinoAbsoluta = dirImagenes.absoluteFilePath(nombreArchivo);

    QNetworkRequest req(cand.urlImagen);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

    // Descarga sincrónica controlada con event loop local y timeout de 10s
    QNetworkReply *reply = m_netManager->get(req);
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);
    QObject::connect(&timer, &QTimer::timeout, &loop, [&loop, reply]() {
        if (reply && reply->isRunning()) {
            reply->abort();
        }
        loop.quit();
    });
    QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    timer.start(10000);
    loop.exec();

    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "[DialogFotosMasivo] Error descargando imagen de alta calidad:" << reply->errorString();
        return false;
    }

    QByteArray data = reply->readAll();
    QFile file(rutaDestinoAbsoluta);
    if (!file.open(QIODevice::WriteOnly)) {
        qDebug() << "[DialogFotosMasivo] Error abriendo archivo para escritura:" << rutaDestinoAbsoluta;
        return false;
    }
    file.write(data);
    file.close();

    QString relativePath = dirImagenes.relativeFilePath(rutaDestinoAbsoluta);
    res.fotoGuardada = relativePath;
    res.guardadoEnBd = true;

    // Actualizar en base de datos local y nube
    baseDatos base;
    base.modificarFotoArticulo(relativePath, art.ean);

    // Actualizar estado en la tabla
    QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(rowArticulo, 7);
    if (itemGuardado) {
        itemGuardado->setText(tr("✅ Guardado en BD"));
        itemGuardado->setForeground(QBrush(QColor("#15803d")));
        itemGuardado->setBackground(QBrush(QColor("#dcfce7")));
    }

    actualizarPanelPrevisualizacion(rowArticulo);
    return true;
}

/**
 * @brief Actualiza la previsualización inferior al seleccionar una fila.
 */
void DialogFotosMasivo::actualizarPanelPrevisualizacion(int row)
{
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        ui->labelFichaTitulo->setText(tr("Selecciona un artículo en la tabla para revisar sus fotos candidatas."));
        ui->labelFichaBadge->clear();
        ui->labelFotoPreviewGrande->setText(tr("Sin foto seleccionada"));
        ui->labelFotoPreviewGrande->setPixmap(QPixmap());
        ui->listWidgetCandidatas->clear();
        ui->pushButtonAceptarFoto->setEnabled(false);
        ui->pushButtonRechazarFoto->setEnabled(false);
        ui->pushButtonVerGrande->setEnabled(false);
        return;
    }

    const ArticuloFotoLote &art = m_colaArticulos[row];
    const ResultadoFotoArticulo &res = m_resultados[row];

    ui->labelFichaTitulo->setText(QString("<b>%1</b> | EAN: %2 | Fab: %3 | Fam: %4")
                                  .arg(art.descripcion, art.ean, art.fabricante, art.familia));

    if (res.guardadoEnBd) {
        ui->labelFichaBadge->setText(tr("✅ Foto asignada y guardada en BD"));
        ui->labelFichaBadge->setStyleSheet("background-color:#dcfce7; color:#15803d; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #a5d6a7;");
    } else if (res.estado == "ok") {
        ui->labelFichaBadge->setText(tr("🖼️ %1 candidatas encontradas (elige una)").arg(res.candidatas.size()));
        ui->labelFichaBadge->setStyleSheet("background-color:#e0f2fe; color:#0369a1; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #bae6fd;");
    } else if (res.estado == "rechazado") {
        ui->labelFichaBadge->setText(tr("❌ Rechazado"));
        ui->labelFichaBadge->setStyleSheet("background-color:#fee2e2; color:#b91c1c; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #fca5a5;");
    } else if (res.estado == "sin_foto") {
        ui->labelFichaBadge->setText(tr("⚠️ Sin fotos encontradas en internet"));
        ui->labelFichaBadge->setStyleSheet("background-color:#fef3c7; color:#b45309; padding:4px 8px; border-radius:4px; font-weight:bold; border:1px solid #ffcc80;");
    } else {
        ui->labelFichaBadge->setText(tr("⏳ Pendiente de búsqueda"));
        ui->labelFichaBadge->setStyleSheet("color: #64748b; font-weight: bold;");
    }

    // Poblar galería de miniaturas
    ui->listWidgetCandidatas->blockSignals(true);
    ui->listWidgetCandidatas->clear();

    for (int i = 0; i < res.candidatas.size(); ++i) {
        const FotoCandidata &cand = res.candidatas[i];
        QListWidgetItem *item = new QListWidgetItem();
        if (!cand.thumbnail.isNull()) {
            item->setIcon(QIcon(cand.thumbnail.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
        } else {
            // Icono provisional mientras descarga
            QPixmap pix(100, 100);
            pix.fill(QColor("#e2e8f0"));
            item->setIcon(QIcon(pix));
        }
        item->setText(QString("%1x%2").arg(cand.ancho).arg(cand.alto));
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        item->setToolTip(cand.titulo + "\n" + cand.urlImagen);
        ui->listWidgetCandidatas->addItem(item);
    }

    int selIdx = res.candidatoSeleccionado;
    if (selIdx >= 0 && selIdx < ui->listWidgetCandidatas->count()) {
        ui->listWidgetCandidatas->setCurrentRow(selIdx);
    }
    ui->listWidgetCandidatas->blockSignals(false);

    // Previsualización grande
    if (res.guardadoEnBd && !res.fotoGuardada.isEmpty()) {
        QDir dirImg(m_directorioImagenes);
        QString absPath = dirImg.absoluteFilePath(res.fotoGuardada);
        QPixmap pixGuardada(absPath);
        if (!pixGuardada.isNull()) {
            ui->labelFotoPreviewGrande->setPixmap(pixGuardada.scaled(ui->labelFotoPreviewGrande->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->pushButtonVerGrande->setEnabled(true);
        } else {
            ui->labelFotoPreviewGrande->setText(tr("Foto guardada:\n%1").arg(res.fotoGuardada));
            ui->pushButtonVerGrande->setEnabled(false);
        }
    } else if (selIdx >= 0 && selIdx < res.candidatas.size()) {
        const FotoCandidata &cand = res.candidatas[selIdx];
        if (!cand.thumbnail.isNull()) {
            ui->labelFotoPreviewGrande->setPixmap(cand.thumbnail.scaled(ui->labelFotoPreviewGrande->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->pushButtonVerGrande->setEnabled(true);
        } else {
            ui->labelFotoPreviewGrande->setText(tr("Cargando imagen...\n%1x%2").arg(cand.ancho).arg(cand.alto));
            ui->pushButtonVerGrande->setEnabled(false);
        }
    } else {
        ui->labelFotoPreviewGrande->setText(tr("Sin fotos disponibles"));
        ui->labelFotoPreviewGrande->setPixmap(QPixmap());
        ui->pushButtonVerGrande->setEnabled(false);
    }

    ui->pushButtonAceptarFoto->setEnabled(!res.candidatas.isEmpty());
    ui->pushButtonRechazarFoto->setEnabled(true);
}

/**
 * @brief Cambia la foto activa al seleccionar otra miniatura en la galería.
 */
void DialogFotosMasivo::on_listWidgetCandidatas_itemSelectionChanged()
{
    int rowArticulo = ui->tableWidgetResultados->currentRow();
    int rowCandidata = ui->listWidgetCandidatas->currentRow();

    if (rowArticulo >= 0 && rowArticulo < m_resultados.size() && rowCandidata >= 0) {
        ResultadoFotoArticulo &res = m_resultados[rowArticulo];
        res.candidatoSeleccionado = rowCandidata;

        if (rowCandidata < res.candidatas.size()) {
            const FotoCandidata &cand = res.candidatas[rowCandidata];
            if (!cand.thumbnail.isNull()) {
                ui->labelFotoPreviewGrande->setPixmap(cand.thumbnail.scaled(ui->labelFotoPreviewGrande->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
                ui->pushButtonVerGrande->setEnabled(true);
            }
        }
    }
}

/**
 * @brief Doble clic en una foto candidata para guardarla directamente.
 */
void DialogFotosMasivo::on_listWidgetCandidatas_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    on_pushButtonAceptarFoto_clicked();
}

/**
 * @brief Doble clic en la tabla de artículos.
 */
void DialogFotosMasivo::on_tableWidgetResultados_cellDoubleClicked(int row, int column)
{
    Q_UNUSED(column);
    actualizarPanelPrevisualizacion(row);
}

/**
 * @brief Acepta y asigna la foto candidata seleccionada.
 */
void DialogFotosMasivo::on_pushButtonAceptarFoto_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        QMessageBox::information(this, tr("Selección"), tr("Selecciona un artículo en la tabla para asignar su foto."));
        return;
    }

    ResultadoFotoArticulo &res = m_resultados[row];
    int candIdx = ui->listWidgetCandidatas->currentRow();
    if (candIdx < 0) candIdx = res.candidatoSeleccionado;
    if (candIdx < 0 && !res.candidatas.isEmpty()) candIdx = 0;

    if (candIdx < 0 || candIdx >= res.candidatas.size()) {
        QMessageBox::warning(this, tr("Sin foto"), tr("No hay fotos candidatas disponibles para asignar a este artículo."));
        return;
    }

    if (descargarYAsignarFoto(row, candIdx)) {
        ui->labelEstadoGeneral->setText(tr("✅ Foto asignada con éxito al artículo '%1'.").arg(m_colaArticulos[row].descripcion));

        // Si estábamos en ejecución interactiva, pasar al siguiente producto automáticamente
        if (m_ejecutando && row == m_indiceActual) {
            m_indiceActual++;
            procesarSiguiente();
        }
    } else {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo descargar o guardar la foto seleccionada."));
    }
}

/**
 * @brief Rechaza u omite la foto del artículo seleccionado.
 */
void DialogFotosMasivo::on_pushButtonRechazarFoto_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_colaArticulos.size() || row >= m_resultados.size()) {
        return;
    }

    ResultadoFotoArticulo &res = m_resultados[row];
    res.estado = "rechazado";

    QTableWidgetItem *itemGuardado = ui->tableWidgetResultados->item(row, 7);
    if (itemGuardado) {
        itemGuardado->setText(tr("❌ Omitido"));
        itemGuardado->setForeground(QBrush(QColor("#b91c1c")));
        itemGuardado->setBackground(QBrush(QColor("#fee2e2")));
    }

    actualizarPanelPrevisualizacion(row);
    ui->labelEstadoGeneral->setText(tr("Foto omitida para '%1'.").arg(m_colaArticulos[row].descripcion));

    // Si estábamos en ejecución interactiva, avanzar al siguiente
    if (m_ejecutando && row == m_indiceActual) {
        m_indiceActual++;
        procesarSiguiente();
    }
}

/**
 * @brief Guarda en lote la primera foto candidata para todos los artículos que la tengan disponible.
 */
void DialogFotosMasivo::on_pushButtonAceptarTodasPendientes_clicked()
{
    int guardados = 0;
    for (int i = 0; i < m_resultados.size(); ++i) {
        ResultadoFotoArticulo &res = m_resultados[i];
        if (!res.guardadoEnBd && res.estado == "ok" && !res.candidatas.isEmpty()) {
            if (descargarYAsignarFoto(i, 0)) {
                guardados++;
            }
        }
    }

    QMessageBox::information(this, tr("Guardado en lote"),
                             tr("Se han asignado y guardado %1 fotos en la base de datos con éxito.").arg(guardados));
    actualizarPanelPrevisualizacion(ui->tableWidgetResultados->currentRow());
}

/**
 * @brief Abre la foto seleccionada a tamaño completo.
 */
void DialogFotosMasivo::on_pushButtonVerGrande_clicked()
{
    int row = ui->tableWidgetResultados->currentRow();
    if (row < 0 || row >= m_resultados.size()) return;

    const ResultadoFotoArticulo &res = m_resultados[row];

    if (res.guardadoEnBd && !res.fotoGuardada.isEmpty()) {
        QDir dirImg(m_directorioImagenes);
        QString absPath = dirImg.absoluteFilePath(res.fotoGuardada);
        VisorImagenes visor(absPath, this);
        visor.exec();
    } else {
        int candIdx = ui->listWidgetCandidatas->currentRow();
        if (candIdx < 0) candIdx = res.candidatoSeleccionado;
        if (candIdx >= 0 && candIdx < res.candidatas.size()) {
            const FotoCandidata &cand = res.candidatas[candIdx];
            if (!cand.thumbnail.isNull()) {
                QDialog dlg(this);
                dlg.setWindowTitle(cand.titulo);
                QVBoxLayout *lay = new QVBoxLayout(&dlg);
                QLabel *lbl = new QLabel(&dlg);
                lbl->setPixmap(cand.thumbnail.scaled(600, 600, Qt::KeepAspectRatio, Qt::SmoothTransformation));
                lbl->setAlignment(Qt::AlignCenter);
                lay->addWidget(lbl);
                QPushButton *btnCerrar = new QPushButton(tr("Cerrar"), &dlg);
                connect(btnCerrar, &QPushButton::clicked, &dlg, &QDialog::accept);
                lay->addWidget(btnCerrar);
                dlg.exec();
            }
        }
    }
}

/**
 * @brief Slot al cambiar la selección en la tabla.
 */
void DialogFotosMasivo::on_tableWidgetResultados_itemSelectionChanged()
{
    int row = ui->tableWidgetResultados->currentRow();
    actualizarPanelPrevisualizacion(row);
}

/**
 * @brief Al marcar el modo interactivo se desmarca el auto-guardado directo.
 */
void DialogFotosMasivo::on_checkBoxModoInteractivo_toggled(bool checked)
{
    if (checked && ui->checkBoxActualizarBD->isChecked()) {
        ui->checkBoxActualizarBD->setChecked(false);
    }
}

/**
 * @brief Al marcar el auto-guardado directo se desmarca el modo interactivo.
 */
void DialogFotosMasivo::on_checkBoxActualizarBD_toggled(bool checked)
{
    if (checked && ui->checkBoxModoInteractivo->isChecked()) {
        ui->checkBoxModoInteractivo->setChecked(false);
    }
}

/**
 * @brief Actualiza los contadores de la barra de métricas.
 */
void DialogFotosMasivo::actualizarMetricas()
{
    ui->labelOkValor->setText(QString::number(m_contadorOk));
    ui->labelRevisarValor->setText(QString::number(m_contadorSinFoto));
    ui->labelErrorValor->setText(QString::number(m_contadorError));
}

void DialogFotosMasivo::on_pushButtonCerrar_clicked()
{
    m_ejecutando = false;
    close();
}
