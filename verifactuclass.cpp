#include "verifactuclass.h"
#include <QCoreApplication>
#include <QSettings>
#include <QCryptographicHash>
#include <QXmlStreamWriter>
#include <QFile>
#include <QSslKey>
#include <QSslCertificate>
#include <QSslConfiguration>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QPainter>
#include <QDir>
#include <QDebug>
#include <qrencode.h> // Biblioteca del sistema instalada
#include <QThread>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>

verifactuClass::verifactuClass(QObject *parent)
    : QObject(parent)
{
}

/**
 * @brief Carga la configuración de VeriFactu desde tienda.ini (SEC-01).
 * Si no está configurado, devuelve valores desactivados por defecto.
 */
VeriFactuConfig verifactuClass::cargarConfiguracion()
{
    VeriFactuConfig config;
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    
    settings.beginGroup("VeriFactu");
    config.modo = settings.value("modo", 0).toInt(); // 0 = Desactivado, 1 = VeriFactu, 2 = No VeriFactu
    config.entorno = settings.value("entorno", 0).toInt(); // 0 = Pruebas, 1 = Prod
    config.emisorNif = settings.value("emisor_nif", "").toString().trimmed().toUpper();
    config.emisorNombre = settings.value("emisor_nombre", "").toString().trimmed();
    config.certificadoRuta = settings.value("certificado_ruta", "").toString().trimmed();
    config.certificadoClave = settings.value("certificado_clave", "").toString();
    settings.endGroup();
    
    return config;
}

/**
 * @brief Formatea importes y decimales según las directrices de la AEAT.
 * Elimina ceros finales redundantes y el punto decimal si no hay decimales.
 */
QString verifactuClass::formatearDecimalAEAT(double valor)
{
    // Redondear a dos decimales de precisión
    QString s = QString::number(valor, 'f', 2);
    if (s.endsWith(".00")) {
        s.chop(3); // Eliminar ".00"
    } else if (s.endsWith("0")) {
        s.chop(1); // Eliminar "0" final en el caso de ".X0"
    }
    return s;
}

/**
 * @brief Genera la huella (hash SHA-256) encadenada obligatoria de un registro de facturación de alta.
 * La cadena de entrada se forma concatenando los campos en formato similar a query string de URL.
 */
QString verifactuClass::calcularHuellaAlta(const QString &nifEmisor,
                                          const QString &numSerieFactura,
                                          const QString &fechaExpedicion,
                                          const QString &tipoFactura,
                                          const QString &cuotaTotal,
                                          const QString &importeTotal,
                                          const QString &huellaAnterior,
                                          const QString &fechaHoraGen)
{
    // Para el primer registro de la serie no existe huella anterior: la AEAT
    // exige el campo Huella vacío en la cadena (no 64 ceros). Se normalizan
    // tanto la cadena vacía como el antiguo sentinela de 64 ceros.
    QString huellaPrevia = huellaAnterior.trimmed();
    if (huellaPrevia == QString(64, '0'))
        huellaPrevia.clear();

    // Construir la cadena de concatenación ordenada según exige la AEAT
    QString cadena = QString("IDEmisorFactura=%1&NumSerieFactura=%2&FechaExpedicionFactura=%3&TipoFactura=%4&CuotaTotal=%5&ImporteTotal=%6&Huella=%7&FechaHoraHusoGenRegistro=%8")
        .arg(nifEmisor.trimmed().toUpper())
        .arg(numSerieFactura.trimmed())
        .arg(fechaExpedicion.trimmed()) // Esperado "DD-MM-YYYY"
        .arg(tipoFactura.trimmed())      // "F1" (Ordinaria) o "F2" (Simplificada/Ticket)
        .arg(cuotaTotal)                 // Ya formateado sin ceros a la derecha
        .arg(importeTotal)               // Ya formateado sin ceros a la derecha
        .arg(huellaPrevia)               // Huella del registro anterior (vacía en el primero)
        .arg(fechaHoraGen.trimmed());    // ISO-8601 (ej: "2026-06-06T21:45:00+02:00")

    // Calcular hash SHA-256
    QByteArray hash = QCryptographicHash::hash(cadena.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex()).toUpper();
}

/**
 * @brief Genera el XML correspondiente al registro de alta de facturación.
 * El documento XML resultante cumple estrictamente con el esquema XSD de la AEAT.
 */
QString verifactuClass::generarXmlAlta(const VeriFactuConfig &config,
                                      const QString &numSerieFactura,
                                      const QString &fechaExpedicion,
                                      const QString &horaExpedicion,
                                      double totalBase,
                                      double totalIva,
                                      double totalImporte,
                                      const QString &tipoFactura,
                                      const QString &huellaAnterior,
                                      const QString &hashActual,
                                      const QString &fechaHoraGen,
                                      const QString &numSerieAnterior,
                                      const QString &fechaExpedicionAnterior)
{
    QString xmlOutput;
    QXmlStreamWriter writer(&xmlOutput);
    writer.setAutoFormatting(true);

    // Escribir cabecera SOAP
    writer.writeStartDocument();
    writer.writeStartElement("soapenv:Envelope");
    writer.writeAttribute("xmlns:soapenv", "http://schemas.xmlsoap.org/soap/envelope/");
    // Definimos namespaces correctos oficiales de la AEAT para VeriFactu
    writer.writeAttribute("xmlns:sum", "https://www2.agenciatributaria.gob.es/static_files/common/internet/dep/aplicaciones/es/aeat/tike/cont/ws/SuministroLR.xsd");
    writer.writeAttribute("xmlns:sum1", "https://www2.agenciatributaria.gob.es/static_files/common/internet/dep/aplicaciones/es/aeat/tike/cont/ws/SuministroInformacion.xsd");

    writer.writeStartElement("soapenv:Header");
    writer.writeEndElement(); // soapenv:Header

    writer.writeStartElement("soapenv:Body");
    // Elemento raíz del mensaje para VeriFactu: RegFactuSistemaFacturacion
    writer.writeStartElement("sum:RegFactuSistemaFacturacion");

    // Cabecera VeriFactu
    writer.writeStartElement("sum:Cabecera");
    writer.writeStartElement("sum1:ObligadoEmision");
    writer.writeTextElement("sum1:NombreRazon", config.emisorNombre);
    writer.writeTextElement("sum1:NIF", config.emisorNif);
    writer.writeEndElement(); // sum1:ObligadoEmision
    writer.writeEndElement(); // sum:Cabecera

    // Registro de Facturación de VeriFactu
    writer.writeStartElement("sum:RegistroFactura");
    writer.writeStartElement("sum1:RegistroAlta");

    // Versión del esquema
    writer.writeTextElement("sum1:IDVersion", "1.0");

    // Identificador de la Factura (IDFactura)
    writer.writeStartElement("sum1:IDFactura");
    writer.writeTextElement("sum1:IDEmisorFactura", config.emisorNif);
    writer.writeTextElement("sum1:NumSerieFactura", numSerieFactura);
    writer.writeTextElement("sum1:FechaExpedicionFactura", fechaExpedicion); // Formato DD-MM-YYYY
    writer.writeEndElement(); // sum1:IDFactura

    // Nombre o Razón Social del Emisor
    writer.writeTextElement("sum1:NombreRazonEmisor", config.emisorNombre);

    // Tipo de Factura (ej: F2 para ticket simplificado)
    writer.writeTextElement("sum1:TipoFactura", tipoFactura);

    // Descripción obligatoria de la operación
    writer.writeTextElement("sum1:DescripcionOperacion", "Venta de productos");

    // Desglose de impuestos (Desglose)
    writer.writeStartElement("sum1:Desglose");
    writer.writeStartElement("sum1:DetalleDesglose");
    writer.writeTextElement("sum1:Impuesto", "01"); // 01 = IVA
    writer.writeTextElement("sum1:ClaveRegimen", "01"); // 01 = Régimen común
    writer.writeTextElement("sum1:CalificacionOperacion", "S1"); // S1 = Sujeta y no exenta, sin inversión
    writer.writeTextElement("sum1:TipoImpositivo", "21"); // IVA estándar 21%
    writer.writeTextElement("sum1:BaseImponibleOimporteNoSujeto", formatearDecimalAEAT(totalBase));
    writer.writeTextElement("sum1:CuotaRepercutida", formatearDecimalAEAT(totalIva));
    writer.writeEndElement(); // sum1:DetalleDesglose
    writer.writeEndElement(); // sum1:Desglose

    // Totales de impuestos e importe
    writer.writeTextElement("sum1:CuotaTotal", formatearDecimalAEAT(totalIva));
    writer.writeTextElement("sum1:ImporteTotal", formatearDecimalAEAT(totalImporte));

    // Criterio de Encadenamiento criptográfico
    writer.writeStartElement("sum1:Encadenamiento");
    // Si la huella anterior está vacía, es el primer registro de la serie.
    // (obtenerUltimoHash* devuelve cadena vacía cuando no hay registros).
    if (huellaAnterior.isEmpty()) {
        writer.writeTextElement("sum1:PrimerRegistro", "S");
    } else {
        writer.writeStartElement("sum1:RegistroAnterior");
        writer.writeTextElement("sum1:IDEmisorFactura", config.emisorNif);
        // F1.4: RegistroAnterior debe referenciar la serie y fecha del registro
        // PREVIO, no las del registro actual.
        writer.writeTextElement("sum1:NumSerieFactura", numSerieAnterior);
        writer.writeTextElement("sum1:FechaExpedicionFactura", fechaExpedicionAnterior);
        writer.writeTextElement("sum1:Huella", huellaAnterior);
        writer.writeEndElement(); // sum1:RegistroAnterior
    }
    writer.writeEndElement(); // sum1:Encadenamiento

    // Datos del Sistema Informático de Facturación (SIF)
    writer.writeStartElement("sum1:SistemaInformatico");
    writer.writeTextElement("sum1:NombreRazon", config.emisorNombre); // Usamos los datos del emisor
    writer.writeTextElement("sum1:NIF", config.emisorNif);
    writer.writeTextElement("sum1:NombreSistemaInformatico", "Tienda TPV SIF");
    writer.writeTextElement("sum1:IdSistemaInformatico", "01");
    writer.writeTextElement("sum1:Version", "1.0");
    writer.writeTextElement("sum1:NumeroInstalacion", "01");
    writer.writeTextElement("sum1:TipoUsoPosibleSoloVerifactu", "N");
    writer.writeTextElement("sum1:TipoUsoPosibleMultiOT", "N");
    writer.writeTextElement("sum1:IndicadorMultiplesOT", "N");
    writer.writeEndElement(); // sum1:SistemaInformatico

    // Fecha y hora de generación con huso horario
    writer.writeTextElement("sum1:FechaHoraHusoGenRegistro", fechaHoraGen);

    // Tipo de huella y huella hash de este registro
    writer.writeTextElement("sum1:TipoHuella", "01"); // 01 = SHA-256
    writer.writeTextElement("sum1:Huella", hashActual);

    writer.writeEndElement(); // sum1:RegistroAlta
    writer.writeEndElement(); // sum:RegistroFactura
    writer.writeEndElement(); // sum:RegFactuSistemaFacturacion
    writer.writeEndElement(); // soapenv:Body
    writer.writeEndElement(); // soapenv:Envelope
    writer.writeEndDocument();

    return xmlOutput;
}

/**
 * @brief Realiza el envío del XML firmado/generado a la AEAT utilizando SOAP sobre HTTPS.
 * Utiliza QEventLoop para bloquear síncronamente durante la remisión y reportar errores inmediatos.
 */
bool verifactuClass::enviarAEAT(const QString &xmlContent, const VeriFactuConfig &config, QString &errStr)
{
    // Validar parámetros del certificado
    if (config.certificadoRuta.isEmpty()) {
        errStr = tr("Ruta del certificado digital no configurada.");
        return false;
    }

    QFile certFile(config.certificadoRuta);
    if (!certFile.open(QIODevice::ReadOnly)) {
        errStr = tr("No se pudo abrir el archivo de certificado en: %1").arg(config.certificadoRuta);
        return false;
    }

    QSslKey privateKey;
    QSslCertificate localCert;
    QList<QSslCertificate> caCerts;
    
    // Importar el certificado PKCS#12 (.p12/.pfx) usando la contraseña
    bool ok = QSslCertificate::importPkcs12(&certFile, &privateKey, &localCert, &caCerts, config.certificadoClave.toUtf8());
    certFile.close();

    if (!ok) {
        errStr = tr("Error al importar el certificado PKCS#12 (compruebe la contraseña).");
        return false;
    }

    // Configurar protocolo TLS seguro con el certificado cliente
    QSslConfiguration sslConfig = QSslConfiguration::defaultConfiguration();
    sslConfig.setLocalCertificate(localCert);
    sslConfig.setPrivateKey(privateKey);
    // Para entornos de pre-producción/pruebas, AEAT a veces usa certificados autofirmados
    if (config.entorno == 0) {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
    } else {
        sslConfig.setPeerVerifyMode(QSslSocket::VerifyPeer);
    }

    // URLs oficiales de AEAT para VeriFactu (Sistemas Informáticos de Facturación)
    QString urlStr = (config.entorno == 0)
        ? "https://prewww1.aeat.es/wlpl/TIKE-CONT/ws/SistemaFacturacion/VerifactuSOAP"
        : "https://www1.aeat.es/wlpl/TIKE-CONT/ws/SistemaFacturacion/VerifactuSOAP";

    QNetworkAccessManager manager;
    QNetworkRequest request((QUrl(urlStr)));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    request.setRawHeader("SOAPAction", "\"\"");
    request.setSslConfiguration(sslConfig);

    // Enviar POST SOAP
    QNetworkReply *reply = manager.post(request, xmlContent.toUtf8());

    // Bloqueo síncrono mediante QEventLoop local para transacciones seguras del TPV
    QEventLoop loop;
    connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
    loop.exec();

    bool success = false;
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        // Buscar confirmación de aceptación en la respuesta XML de la AEAT
        if (response.contains("Correcto") ||
            response.contains("AceptadoConErrores") ||
            response.contains("AceptadaConErrores")) {
            success = true;
        } else {
            errStr = tr("El servidor AEAT retornó errores: %1").arg(QString::fromUtf8(response));
        }
    } else {
        errStr = tr("Error de conexión telemática (%1): %2").arg(reply->error()).arg(reply->errorString());
    }

    reply->deleteLater();
    return success;
}

/**
 * @brief Genera la URL de cotejo que la AEAT requiere para incrustar en el código QR.
 */
QString verifactuClass::generarUrlQR(const QString &nifEmisor,
                                    const QString &numSerieFactura,
                                    const QString &fechaExpedicion,
                                    double importeTotal,
                                    int entorno)
{
    // URL base de cotejo de la AEAT (entorno de pruebas vs producción)
    QString baseUrl = (entorno == 0)
        ? "https://prewww2.aeat.es/wlpl/TIKE-CONT/ValidarQR"
        : "https://www2.agenciatributaria.gob.es/wlpl/TIKE-CONT/ValidarQR";
    
    // Parámetros formateados según AEAT: fecha como DD-MM-YYYY
    // Los importes se formatean sin ceros redundantes a la derecha
    QString url = QString("%1?nif=%2&numserie=%3&fecha=%4&importe=%5")
        .arg(baseUrl)
        .arg(nifEmisor.trimmed().toUpper())
        .arg(QUrl::toPercentEncoding(numSerieFactura.trimmed()))
        .arg(fechaExpedicion)
        .arg(formatearDecimalAEAT(importeTotal));

    return url;
}

/**
 * @brief Genera un código QR y lo devuelve como un objeto QImage listo para imprimir.
 * Utiliza la biblioteca del sistema libqrencode de forma directa en C++.
 */
QImage verifactuClass::generarCodigoQR(const QString &url)
{
    // Generar el código QR con el nivel de corrección M requerido
    QRcode *qrcode = QRcode_encodeString(url.toUtf8().constData(), 0, QR_ECLEVEL_M, QR_MODE_8, 1);
    if (!qrcode) {
        return QImage();
    }

    // Escalar el tamaño de píxeles para mejorar la nitidez al imprimir
    int scale = 4;
    int width = qrcode->width * scale;
    
    // Usamos Format_RGB32 en lugar de Format_Mono para garantizar compatibilidad total
    // con QPainter, la exportación a PNG y la conversión en PrinterManager sin problemas de color.
    QImage qrImage(width, width, QImage::Format_RGB32);
    qrImage.fill(Qt::white);

    QPainter painter(&qrImage);
    painter.setBrush(Qt::black);
    painter.setPen(Qt::NoPen);

    // Renderizar los módulos de datos del QR en la imagen
    for (int y = 0; y < qrcode->width; ++y) {
        for (int x = 0; x < qrcode->width; ++x) {
            unsigned char b = qrcode->data[y * qrcode->width + x];
            if (b & 0x01) {
                painter.drawRect(x * scale, y * scale, scale, scale);
            }
        }
    }

    // Liberar estructura dinámica de libqrencode
    QRcode_free(qrcode);
    
    return qrImage;
}

/**
 * @brief Genera la fecha/hora actual en formato ISO-8601 completo incluyendo el huso horario local.
 * Esto es obligatorio para cumplir con el esquema dateTime de la AEAT y evitar el error 1244.
 */
QString verifactuClass::obtenerFechaHoraHusoActual()
{
    QDateTime now = QDateTime::currentDateTime();
    int offset = now.offsetFromUtc();
    int hours = qAbs(offset) / 3600;
    int minutes = (qAbs(offset) % 3600) / 60;
    QString offsetStr = QString("%1%2:%3")
        .arg(offset >= 0 ? "+" : "-")
        .arg(hours, 2, 10, QChar('0'))
        .arg(minutes, 2, 10, QChar('0'));
    return now.toString("yyyy-MM-ddTHH:mm:ss") + offsetStr;
}

/**
 * @brief Reenvía de forma automática los registros pendientes (estado_envio = 0)
 *        al servidor SOAP de la AEAT en un hilo secundario de forma segura.
 * @param conexionOriginal Nombre de la conexión de base de datos activa del hilo principal.
 */
void verifactuClass::procesarEnviosPendientes(const QString &conexionOriginal)
{
    // Generar un nombre único de conexión para evitar colisiones entre hilos
    QString nombreConexion = QString("VeriFactuRetryConnection_%1")
                                 .arg(quintptr(QThread::currentThreadId()));
    {
        QSqlDatabase dbOriginal = QSqlDatabase::database(conexionOriginal);
        if (!dbOriginal.isOpen()) {
            return;
        }

        // Clonamos la conexión para que este hilo secundario acceda de forma segura
        QSqlDatabase db = QSqlDatabase::cloneDatabase(dbOriginal, nombreConexion);
        if (!db.open()) {
            qWarning() << "No se pudo abrir la conexión a la base de datos de reintentos VeriFactu:"
                       << db.lastError().text();
            return;
        }

        VeriFactuConfig config = cargarConfiguracion();
        // Si no estamos en modo remisión a la AEAT, no tiene sentido el reenvío
        if (config.modo != 1) {
            db.close();
            return;
        }

        QSqlQuery query(db);
        // Buscamos los logs pendientes en orden cronológico ascendente para no romper la cadena
        QString sql = "SELECT id, id_factura, cadena_firmada FROM verifactu_logs "
                      "WHERE estado_envio = 0 ORDER BY id ASC";

        if (query.exec(sql)) {
            while (query.next()) {
                int logId = query.value("id").toInt();
                QString numSerie = query.value("id_factura").toString();
                QString xmlContent = query.value("cadena_firmada").toString();

                QString errStr;
                qDebug() << "Reintentando enviar ticket pendiente" << numSerie << "a la AEAT...";
                bool ok = enviarAEAT(xmlContent, config, errStr);

                if (ok) {
                    QSqlQuery updateQuery(db);
                    updateQuery.prepare("UPDATE verifactu_logs SET estado_envio = 1 WHERE id = :id");
                    updateQuery.bindValue(":id", logId);
                    if (updateQuery.exec()) {
                        qDebug() << "Reenvío exitoso para ticket pendiente" << numSerie;
                    } else {
                        qWarning() << "Fallo al actualizar el estado de envío para ticket" << numSerie
                                   << updateQuery.lastError().text();
                    }
                } else {
                    qWarning() << "Reenvío fallido para ticket" << numSerie
                               << ". Deteniendo cola de reintentos. Motivo:" << errStr;
                    // Si falla un envío (por ejemplo, sigue sin haber conexión), detenemos el bucle
                    break;
                }
            }
        } else {
            qWarning() << "Error al consultar la lista de envíos de VeriFactu pendientes:"
                       << query.lastError().text();
        }

        db.close();
    }
    
    // Eliminamos del registro global la conexión temporal una vez destruidos todos los objetos SQL
    QSqlDatabase::removeDatabase(nombreConexion);
}
