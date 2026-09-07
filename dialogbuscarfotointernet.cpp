#include "dialogbuscarfotointernet.h"
#include "ui_dialogbuscarfotointernet.h"
#include "base_datos.h"
#include "configuracion.h"

#include <QDir>
#include <QFileInfo>
#include <QImageReader>
#include <QMessageBox>
#include <QRegularExpression>
#include <QUrlQuery>
#include <QTextDocument>
#include <QDebug>

/**
 * @brief Constructor de DialogBuscarFotoInternet.
 * @param descripcion Nombre o descripción del artículo.
 * @param fabricante Nombre del fabricante o marca.
 * @param codArticulo Código identificador del artículo (EAN).
 * @param parent Widget padre.
 */
DialogBuscarFotoInternet::DialogBuscarFotoInternet(const QString &descripcion,
                                                   const QString &fabricante,
                                                   const QString &codArticulo,
                                                   QWidget *parent)
    : QDialog(parent),
      ui(new Ui::DialogBuscarFotoInternet),
      m_netManager(new QNetworkAccessManager(this)),
      m_nombreProducto(descripcion.trimmed()),
      m_fabricante(fabricante.trimmed()),
      m_codArticulo(codArticulo.trimmed()),
      m_buscando(false)
{
    ui->setupUi(this);

    // Obtener directorio configurado para imágenes
    baseDatos base;
    m_directorioImagenes = base.devolverDirectorio("imagenes");
    if (m_directorioImagenes.isEmpty()) {
        m_directorioImagenes = "./imagenes";
    }
    // Asegurar que el directorio de imágenes exista en el disco
    QDir().mkpath(m_directorioImagenes);

    // Configurar icono y cuadrícula del listWidget
    ui->listWidgetFotos->setIconSize(QSize(120, 120));
    ui->listWidgetFotos->setGridSize(QSize(140, 150));

    // Subtítulo del diálogo
    QString subtitulo = m_nombreProducto;
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido") {
        subtitulo += " | " + m_fabricante;
    }
    if (!m_codArticulo.isEmpty()) {
        subtitulo += " (Cód: " + m_codArticulo + ")";
    }
    ui->labelSubtitulo->setText(subtitulo);

    // Configurar término de búsqueda inicial
    QString terminoInicial = m_nombreProducto;
    if (!m_fabricante.isEmpty() && m_fabricante != "Fabricante desconocido" && !terminoInicial.contains(m_fabricante, Qt::CaseInsensitive)) {
        terminoInicial += " " + m_fabricante;
    }
    ui->lineEditTermino->setText(terminoInicial);

    // Iniciar la búsqueda automáticamente
    iniciarBusqueda();
}

DialogBuscarFotoInternet::~DialogBuscarFotoInternet()
{
    delete ui;
}

/**
 * @brief Devuelve la ruta relativa del archivo guardado.
 */
QString DialogBuscarFotoInternet::getNombreFicheroRelativo() const
{
    return m_ficheroGuardadoRelativo;
}

/**
 * @brief Inicia el proceso de búsqueda de imágenes.
 */
void DialogBuscarFotoInternet::iniciarBusqueda()
{
    QString termino = ui->lineEditTermino->text().trimmed();
    if (termino.isEmpty()) {
        termino = m_nombreProducto;
    }

    m_buscando = true;
    m_candidatas.clear();
    ui->listWidgetFotos->clear();
    ui->labelPreviewGrande->clear();
    ui->labelPreviewGrande->setText(tr("Buscando fotos en internet..."));
    ui->labelDetalleFoto->clear();
    ui->progressBar->setRange(0, 0);
    ui->pushButtonBuscar->setEnabled(false);
    ui->pushButtonAceptar->setEnabled(false);
    ui->labelEstado->setText(tr("🔍 Conectando con el buscador de imágenes..."));

    buscarImagenes(termino);
}

/**
 * @brief Envía la consulta HTTP asíncrona al buscador de imágenes.
 *        Utiliza el endpoint de imágenes sin requerir tokens volátiles que causan errores 403.
 * @param termino Cadena de texto a buscar (nombre de producto, fabricante, etc.).
 */
void DialogBuscarFotoInternet::buscarImagenes(const QString &termino)
{
    QUrl url("https://www.bing.com/images/async");
    QUrlQuery query;
    query.addQueryItem("q", termino);
    query.addQueryItem("async", "1");
    query.addQueryItem("first", "1");
    query.addQueryItem("count", "35");
    url.setQuery(query);

    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");
    req.setRawHeader("Accept", "*/*");
    req.setRawHeader("Accept-Language", "es-ES,es;q=0.9,en;q=0.8");

    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onImagesSearchReplyFinished(reply);
    });
}

/**
 * @brief Procesa el HTML devuelto por el buscador y extrae las imágenes candidatas.
 * @param reply Objeto QNetworkReply con la respuesta HTTP.
 */
void DialogBuscarFotoInternet::onImagesSearchReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();
    m_buscando = false;
    ui->progressBar->setRange(0, 100);
    ui->pushButtonBuscar->setEnabled(true);

    if (reply->error() != QNetworkReply::NoError) {
        ui->labelEstado->setText(tr("❌ Error al consultar imágenes: %1").arg(reply->errorString()));
        return;
    }

    QString htmlContent = QString::fromUtf8(reply->readAll());

    // Localizar tarjetas de imágenes (etiquetas <a> con clase "iusc" y atributo m="...")
    QRegularExpression reCard("<a\\b[^>]*?class=\"[^\"]*?\\biusc\\b[^\"]*?\"[^>]*>");
    QRegularExpressionMatchIterator it = reCard.globalMatch(htmlContent);

    QRegularExpression reM("m=\"([^\"]+)\"");
    QRegularExpression reH("exph=(\\d+)");
    QRegularExpression reW("expw=(\\d+)");

    while (it.hasNext() && m_candidatas.size() < 30) {
        QRegularExpressionMatch matchCard = it.next();
        QString cardTag = matchCard.captured(0);

        QRegularExpressionMatch matchM = reM.match(cardTag);
        if (!matchM.hasMatch()) {
            continue;
        }

        // El atributo m almacena metadatos JSON con comillas escapadas como &quot;
        QString rawM = matchM.captured(1);
        rawM.replace("&quot;", "\"");
        rawM.replace("&amp;", "&");
        rawM.replace("&lt;", "<");
        rawM.replace("&gt;", ">");

        QJsonDocument doc = QJsonDocument::fromJson(rawM.toUtf8());
        if (!doc.isObject()) {
            continue;
        }

        QJsonObject obj = doc.object();
        QString imgUrl = obj.value("murl").toString();
        QString thumbUrl = obj.value("turl").toString();
        QString title = obj.value("t").toString();

        if (imgUrl.isEmpty()) {
            continue;
        }

        // Decodificar entidades HTML residuales del título
        QTextDocument textDoc;
        textDoc.setHtml(title);
        title = textDoc.toPlainText().trimmed();

        FotoCandidata cand;
        cand.urlImagen = imgUrl;
        cand.urlThumbnail = thumbUrl.isEmpty() ? imgUrl : thumbUrl;
        cand.titulo = title;

        // Extraer dimensiones si están presentes en la etiqueta
        QRegularExpressionMatch matchH = reH.match(cardTag);
        if (matchH.hasMatch()) {
            cand.alto = matchH.captured(1).toInt();
        }
        QRegularExpressionMatch matchW = reW.match(cardTag);
        if (matchW.hasMatch()) {
            cand.ancho = matchW.captured(1).toInt();
        }

        m_candidatas.append(cand);

        // Crear item en la lista con icono temporal
        QListWidgetItem *item = new QListWidgetItem();
        item->setText(QString("%1\n%2x%3")
                          .arg(cand.titulo.left(25) + (cand.titulo.length() > 25 ? "..." : ""))
                          .arg(cand.ancho)
                          .arg(cand.alto));
        item->setTextAlignment(Qt::AlignHCenter | Qt::AlignBottom);
        item->setToolTip(cand.titulo + "\n" + cand.urlImagen);
        ui->listWidgetFotos->addItem(item);
    }

    if (m_candidatas.isEmpty()) {
        ui->labelEstado->setText(tr("⚠️ No se encontraron imágenes para este término. Prueba a cambiar el texto."));
        ui->labelPreviewGrande->setText(tr("Sin resultados."));
        return;
    }

    ui->labelEstado->setText(tr("✅ %1 imágenes encontradas. Descargando miniaturas...").arg(m_candidatas.size()));
    descargarMiniaturas();
}

/**
 * @brief Descarga las miniaturas asíncronamente para la galería visual.
 */
void DialogBuscarFotoInternet::descargarMiniaturas()
{
    for (int i = 0; i < m_candidatas.size(); ++i) {
        QString urlThumb = m_candidatas[i].urlThumbnail;
        if (urlThumb.isEmpty()) continue;

        QNetworkRequest req(urlThumb);
        req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
        req.setHeader(QNetworkRequest::UserAgentHeader,
                      "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

        QNetworkReply *reply = m_netManager->get(req);
        connect(reply, &QNetworkReply::finished, this, [this, reply, i]() {
            onThumbnailFinished(reply, i);
        });
    }
}

/**
 * @brief Slot cuando se completa la descarga de una miniatura específica.
 */
void DialogBuscarFotoInternet::onThumbnailFinished(QNetworkReply *reply, int index)
{
    reply->deleteLater();
    if (reply->error() == QNetworkReply::NoError && index >= 0 && index < m_candidatas.size() && index < ui->listWidgetFotos->count()) {
        QByteArray data = reply->readAll();
        QPixmap pix;
        if (pix.loadFromData(data)) {
            m_candidatas[index].thumbnail = pix;
            QIcon icon(pix.scaled(120, 120, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            ui->listWidgetFotos->item(index)->setIcon(icon);

            // Si es el primer elemento y nada está seleccionado, seleccionarlo automáticamente
            if (ui->listWidgetFotos->selectedItems().isEmpty() && index == 0) {
                ui->listWidgetFotos->setCurrentRow(0);
            }
        }
    }
}

/**
 * @brief Actualiza la previsualización grande al cambiar de selección en la galería.
 */
void DialogBuscarFotoInternet::on_listWidgetFotos_itemSelectionChanged()
{
    int row = ui->listWidgetFotos->currentRow();
    if (row < 0 || row >= m_candidatas.size()) {
        ui->pushButtonAceptar->setEnabled(false);
        return;
    }

    const FotoCandidata &cand = m_candidatas[row];
    ui->pushButtonAceptar->setEnabled(true);

    if (!cand.thumbnail.isNull()) {
        QPixmap grande = cand.thumbnail.scaled(ui->labelPreviewGrande->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->labelPreviewGrande->setPixmap(grande);
    } else {
        ui->labelPreviewGrande->setText(tr("Cargando vista previa..."));
    }

    ui->labelDetalleFoto->setText(QString("<b>Dimensiones:</b> %1 x %2 px<br><b>Título:</b> %3")
                                      .arg(cand.ancho)
                                      .arg(cand.alto)
                                      .arg(cand.titulo));
}

/**
 * @brief Doble clic en una imagen para seleccionarla y aceptar inmediatamente.
 */
void DialogBuscarFotoInternet::on_listWidgetFotos_itemDoubleClicked(QListWidgetItem *item)
{
    Q_UNUSED(item);
    on_pushButtonAceptar_clicked();
}

/**
 * @brief Slot para el botón Buscar / Refinar.
 */
void DialogBuscarFotoInternet::on_pushButtonBuscar_clicked()
{
    if (m_buscando) return;
    iniciarBusqueda();
}

/**
 * @brief Descarga la imagen seleccionada en resolución completa y la guarda en disco.
 */
void DialogBuscarFotoInternet::on_pushButtonAceptar_clicked()
{
    int row = ui->listWidgetFotos->currentRow();
    if (row < 0 || row >= m_candidatas.size()) {
        return;
    }

    const FotoCandidata &cand = m_candidatas[row];
    if (cand.urlImagen.isEmpty()) {
        return;
    }

    // Determinar nombre del archivo de destino: <codigo>.jpg (o basado en el código del artículo)
    QString nombreArchivo = m_codArticulo;
    if (nombreArchivo.isEmpty()) {
        nombreArchivo = m_nombreProducto.simplified();
        nombreArchivo.replace(QRegularExpression("[^a-zA-Z0-9_\\-]"), "_");
    }

    // Determinar extensión adecuada
    QString ext = ".jpg";
    if (cand.urlImagen.endsWith(".png", Qt::CaseInsensitive)) {
        ext = ".png";
    } else if (cand.urlImagen.endsWith(".webp", Qt::CaseInsensitive)) {
        ext = ".webp";
    }
    nombreArchivo += ext;

    QDir dirImagenes(m_directorioImagenes);
    QString rutaDestinoAbsoluta = dirImagenes.absoluteFilePath(nombreArchivo);

    ui->labelEstado->setText(tr("💾 Descargando y guardando imagen de alta calidad..."));
    ui->progressBar->setRange(0, 0);
    ui->pushButtonAceptar->setEnabled(false);
    ui->pushButtonCancelar->setEnabled(false);

    descargarYGuardarFoto(cand.urlImagen, rutaDestinoAbsoluta);
}

/**
 * @brief Descarga la foto en resolución completa y la almacena en el directorio configurado.
 */
void DialogBuscarFotoInternet::descargarYGuardarFoto(const QString &url, const QString &rutaDestino)
{
    QNetworkRequest req(url);
    req.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);
    req.setHeader(QNetworkRequest::UserAgentHeader,
                  "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/120.0.0.0 Safari/537.36");

    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply, rutaDestino]() {
        reply->deleteLater();
        ui->progressBar->setRange(0, 100);
        ui->progressBar->setValue(100);
        ui->pushButtonCancelar->setEnabled(true);

        if (reply->error() != QNetworkReply::NoError) {
            QMessageBox::critical(this, tr("Error de descarga"),
                                  tr("No se pudo descargar la imagen seleccionada:\n%1").arg(reply->errorString()));
            ui->pushButtonAceptar->setEnabled(true);
            return;
        }

        QByteArray data = reply->readAll();
        QFile file(rutaDestino);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error al guardar"),
                                  tr("No se pudo guardar la imagen en:\n%1").arg(rutaDestino));
            ui->pushButtonAceptar->setEnabled(true);
            return;
        }

        file.write(data);
        file.close();

        // Obtener ruta relativa respecto al directorio base de imágenes
        QDir dirImagenes(m_directorioImagenes);
        m_ficheroGuardadoRelativo = dirImagenes.relativeFilePath(rutaDestino);

        qDebug() << "Foto guardada exitosamente en:" << rutaDestino
                 << "| Ruta relativa para BD:" << m_ficheroGuardadoRelativo;

        accept();
    });
}

/**
 * @brief Slot para cancelar el diálogo.
 */
void DialogBuscarFotoInternet::on_pushButtonCancelar_clicked()
{
    reject();
}
