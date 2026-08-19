#include "imprimirticket.h"
#include <QDate>
#include <QFile>
#include "verifactuclass.h"

ImprimirTicket::ImprimirTicket(QString nTicket, QString formato, bool noTicketRegalo, bool esSerieB, QObject *parent)
    : QObject(parent)
    , esSerieB(esSerieB)
    , printer(new PrinterManager(this))
{
    consulta = base.datosTicket(conf->getConexionLocal(), nTicket);
    if (!consulta.first()) {
        qDebug() << "Ticket no encontrado: " << nTicket;
        return;
    }
    ticket = consulta.value(0).toString();
    fecha = consulta.value(3).toString();
    hora = consulta.value(4).toString();
    total = consulta.value(8).toString();
    fPago = base.nombreFormaPago(consulta.value(9).toString(), conf->getConexionLocal());
    entrega = consulta.value(11).toString();
    cambio = consulta.value(12).toString();
    confTicket = base.recuperarConfigTicket(conf->getConexionLocal());
    if (formato == "ticket") {

        qDebug() << "Imprimiendo ticket" << ticket << "directamente a impresora...";

        // 1. Imprimir logo si existe
        imprimirLogo();

        // 2. Imprimir cabecera
        imprimirCabecera();

        // 3. Imprimir línea de separación
        printer->imprimirLineaSeparadora('-', 48);

        // 4. Imprimir encabezado de columnas
        printer->imprimirLinea("UDS  Producto                  Prec.  %  Total");
        printer->imprimirLineaSeparadora('-', 48);

        // 5. Imprimir líneas de productos
        imprimirLineasProductos();

        // 6. Imprimir pie
        imprimirPie();

        // 7. Imprimir mensaje final
        printer->imprimirLineaCentrada(confTicket.at(1));

        // 8. Imprimir promoción si corresponde
        if (confTicket.at(7) == "1" && !noTicketRegalo) {
            cortarPapel();
            ImprimirTicketRegalo(confTicket.at(6));
            printer->alimentarLineas(1);
        }

        // 9. Espacios y corte
        printer->alimentarLineas(3);

        // 10. Corte de papel
        cortarPapel();

        // 11. Cerrar impresora
        printer->cerrarImpresora();

        qDebug() << "Ticket" << ticket << "impreso correctamente";
    }
}

ImprimirTicket::~ImprimirTicket() {}

bool ImprimirTicket::ImprimirTicketRegalo(QString ruta)
{
    if (!ruta.isEmpty() && QFile::exists(ruta)) {
        if (printer->imprimirImagen(ruta,true)) {
            return true;
        }
        return false;
    }
    return false;
}

bool ImprimirTicket::cortarPapel()
{
    QString codCorte = confTicket.at(5);
    if (!codCorte.isEmpty()) {
        // Si hay configuración de corte específica
        QStringList cadaCodCorte = codCorte.split(",");
        QByteArray comandoCorte;
        for (int i = 0; i < cadaCodCorte.size(); ++i) {
            comandoCorte.append(static_cast<char>(cadaCodCorte.at(i).toInt()));
        }
        printer->enviarComando(comandoCorte);
        qDebug() << "Corte de configuración";
    } else {
        // Corte estándar
        printer->cortarPapel(true);
        qDebug() << "Corte standar";
    }
    return true;
}

QString ImprimirTicket::formatearCadena(QString cadena, int tamano)
{
    if (cadena.length() >= tamano) {
        cadena.resize(tamano);
        return cadena;
    }
    int diferencia = tamano - cadena.length();
    for (int i = 0; i < diferencia; ++i) {
        cadena += " ";
    }
    return cadena;
}

bool ImprimirTicket::imprimirCabecera()
{
    QStringList confTicket = base.recuperarConfigTicket(conf->getConexionLocal());

    // Nombre de empresa
    if (!confTicket.isEmpty() && !confTicket.at(0).isEmpty()) {
        printer->setTamanio(2, 1);
        printer->imprimirLineaCentrada(confTicket.at(0));
        printer->setTamanio(1, 1);
    }

    printer->alimentarLineas(1);

    // Fecha, hora y número de ticket
    QString fechaStr = QDate::fromString(fecha, "yyyy-MM-dd").toString("dd-MMM-yyyy");
    QString infoLinea = fechaStr + "  " + hora + "    Ticket: " + ticket;
    printer->imprimirLineaCentrada(infoLinea);

    printer->alimentarLineas(1);

    return true;
}

bool ImprimirTicket::imprimirLineasProductos()
{
    consulta = base.consultarLineasTicket(conf->getConexionLocal(), ticket);

    while (consulta.next()) {
        uds = consulta.value(4).toString();
        producto = consulta.value(3).toString();
        precio = consulta.value(6).toString();
        dto = consulta.value(7).toString();
        totalLinea = consulta.value(8).toString();

        // Formatear línea para impresión
        QString linea = formatearCadena(uds, 3) + " " + formatearCadena(producto, 24) + "  "
                        + formatearCadena(precio, 5) + "  " + formatearCadena(dto, 3) + "  "
                        + formatearCadena(totalLinea, 6);

        printer->imprimirLinea(linea);
    }

    printer->alimentarLineas(1);
    printer->imprimirLineaSeparadora('=', 48);

    return true;
}

bool ImprimirTicket::imprimirPie()
{
    printer->alimentarLineas(1);

    // Total
    printer->setNegrita(true);
    printer->imprimirLineaDerecha("TOTAL: " + total);
    printer->setNegrita(false);

    printer->alimentarLineas(1);

    // Forma de pago
    printer->imprimirLinea("Forma de pago: " + fPago);

    // Entrega y cambio
    if (!entrega.isEmpty() && entrega != "0.00") {
        printer->imprimirLinea("Entregado: " + entrega);
    }

    if (!cambio.isEmpty() && cambio != "0.00") {
        printer->imprimirLinea("Cambio: " + cambio);
    }

    // --- Código QR Tributario y Leyendas de VeriFactu ---
    // SEC/F1.4: los tickets serie B (ventas especiales, tabla 'ticketss') no se
    // registran en VeriFactu, así que no deben imprimir el QR tributario.
    VeriFactuConfig vfConfig = verifactuClass::cargarConfiguracion();
    if (vfConfig.modo != 0 && !esSerieB) { // Si VeriFactu o Firma Local están habilitados
        printer->alimentarLineas(1);
        
        // 1. Leyenda superior del QR
        printer->imprimirLineaCentrada("QR tributario");
        
        // 2. Formatear la fecha como DD-MM-YYYY
        QDate dateExp = QDate::fromString(fecha, "yyyy-MM-dd");
        QString fechaExpAEAT = dateExp.isValid() ? dateExp.toString("dd-MM-yyyy") : QDate::currentDate().toString("dd-MM-yyyy");
        
        // 3. Generar la URL de cotejo de la AEAT
        QString urlCotejo = verifactuClass::generarUrlQR(
            vfConfig.emisorNif,
            ticket, // número de ticket / serie
            fechaExpAEAT,
            total.toDouble(),
            vfConfig.entorno
        );
        
        // 4. Renderizar la imagen QR
        QImage qrImage = verifactuClass::generarCodigoQR(urlCotejo);
        
        // 5. Imprimir la imagen del QR centrada
        if (!qrImage.isNull()) {
            printer->imprimirImagen(qrImage, true);
        }
        
        // 6. Leyenda inferior reglamentaria
        if (vfConfig.modo == 1) {
            printer->imprimirLineaCentrada("Factura verificable en la sede");
            printer->imprimirLineaCentrada("electronica de la AEAT - VERI*FACTU");
        } else {
            printer->imprimirLineaCentrada("Factura simplificada");
        }
        
        printer->alimentarLineas(1);
    }

    printer->alimentarLineas(2);

    return true;
}

bool ImprimirTicket::imprimirLogo()
{

    QString logoTicket = base.recuperarConfigTicket(conf->getConexionLocal()).at(2);
    if (!logoTicket.isEmpty() && QFile::exists(logoTicket)) {
        if (printer->imprimirImagen(logoTicket, true)) {
            return true;
        }
    }
    // Buscar logo de empresa
    QStringList posiblesLogos = {
        "logo.png",
        "logo.jpg",
        "logo.bmp",
        "img/logo.png",
        "img/logo.jpg",
        "/usr/share/miempresa/logo.png",
    };

    for (const QString &logoPath : posiblesLogos) {
        if (QFile::exists(logoPath)) {
            if (printer->imprimirImagen(logoPath, true)) {
                //printer->alimentarLineas(1);
                return true;
            }
        }
    }

    // Si no hay logo, imprimir nombre de empresa centrado
    QStringList confTicket = base.recuperarConfigTicket(conf->getConexionLocal());
    if (!confTicket.isEmpty() && !confTicket.at(0).isEmpty()) {
        printer->setTamanio(2, 2);
        printer->imprimirLineaCentrada(confTicket.at(0));
        printer->setTamanio(1, 1);
        printer->alimentarLineas(1);
    }

    return true;
}

/**
 * @brief Imprime un comprobante de encargo en la impresora térmica de tickets.
 * 
 * Genera y envía el ticket de comprobante de encargo a la impresora térmica configurada,
 * incluyendo información sobre la empresa, datos del cliente, producto encargados, anticipo y notas.
 */
bool ImprimirTicket::imprimirComprobanteEncargo(int idEncargo,
                                               const QString &codCliente,
                                               const QString &codArticulo,
                                               const QString &descArticulo,
                                               int cantidad,
                                               double anticipo,
                                               const QString &formaPago,
                                               const QString &notas,
                                               const QString &empleado,
                                               const QString &fechaStr)
{
    baseDatos base;
    PrinterManager printer;

    // Recuperar configuración de la impresora y del ticket desde la base de datos
    QStringList confTicket = base.recuperarConfigTicket(conf->getConexionLocal());
    if (confTicket.size() > 3 && !confTicket.at(3).isEmpty()) {
        printer.setDevicePath(confTicket.at(3));
    }

    if (!printer.abrirImpresora()) {
        qDebug() << "ImprimirTicket::imprimirComprobanteEncargo: No se pudo abrir la impresora:" << printer.devicePath();
        return false;
    }

    // 1. Imprimir logo de la empresa si está configurado y existe el archivo
    if (confTicket.size() > 2 && !confTicket.at(2).isEmpty() && QFile::exists(confTicket.at(2))) {
        printer.imprimirLogoEmpresa(confTicket.at(2));
    }

    // 2. Cabecera (Nombre de la empresa)
    if (confTicket.size() > 0 && !confTicket.at(0).isEmpty()) {
        printer.setTamanio(2, 1);
        printer.imprimirLineaCentrada(confTicket.at(0));
        printer.setTamanio(1, 1);
    }
    printer.alimentarLineas(1);

    // 3. Título del comprobante en negrita y tamaño destacado
    printer.setNegrita(true);
    printer.setTamanio(1, 2);
    printer.imprimirLineaCentrada("*** COMPROBANTE DE ENCARGO ***");
    printer.setTamanio(1, 1);
    printer.setNegrita(false);
    printer.alimentarLineas(1);

    // 4. Datos del encargo: ID de encargo, Fecha y Hora
    if (idEncargo > 0) {
        printer.imprimirLinea(QString("No Encargo: #%1").arg(idEncargo));
    }

    // Formato estricto de fecha "yyyy-MM-dd" según las normas globales del sistema
    QString fechaFinal = fechaStr;
    if (fechaFinal.isEmpty()) {
        fechaFinal = QDate::currentDate().toString("yyyy-MM-dd");
    } else {
        QDate f = QDate::fromString(fechaStr, "yyyy-MM-dd");
        if (!f.isValid()) {
            f = QDate::fromString(fechaStr, Qt::ISODate);
        }
        if (f.isValid()) {
            fechaFinal = f.toString("yyyy-MM-dd");
        }
    }
    QString horaFinal = QTime::currentTime().toString("hh:mm:ss");
    printer.imprimirLinea(QString("Fecha: %1  Hora: %2").arg(fechaFinal, horaFinal));

    // Obtener nombre completo del cliente si está registrado
    QString clienteInfo = codCliente;
    if (!codCliente.isEmpty() && codCliente != "0") {
        QSqlQuery qCli(QSqlDatabase::database(conf->getConexionLocal()));
        qCli.prepare("SELECT nombre FROM clientes WHERE id = ?");
        qCli.bindValue(0, codCliente.toInt());
        if (qCli.exec() && qCli.next() && !qCli.value(0).toString().isEmpty()) {
            clienteInfo += " - " + qCli.value(0).toString();
        }
    } else {
        clienteInfo = "Cliente de Contado";
    }
    printer.imprimirLinea("Cliente: " + clienteInfo);

    if (!empleado.isEmpty()) {
        printer.imprimirLinea("Atendido por: " + empleado);
    }

    printer.imprimirLineaSeparadora('-', 48);

    // 5. Detalles de los artículos encargados y anticipo a cuenta
    bool desglosadoMultiproducto = false;
    if (idEncargo > 0) {
        QSqlQuery qLineas(QSqlDatabase::database(conf->getConexionLocal()));
        qLineas.prepare("SELECT cod_articulo, descripcion, cantidad, pvp FROM encargos_lineas WHERE id_encargo = ?");
        qLineas.bindValue(0, idEncargo);
        if (qLineas.exec() && qLineas.next()) {
            desglosadoMultiproducto = true;
            printer.setNegrita(true);
            printer.imprimirLinea("PRODUCTOS ENCARGADOS:");
            printer.setNegrita(false);
            
            double totalProductos = 0.0;
            do {
                QString cArt = qLineas.value("cod_articulo").toString();
                QString dArt = qLineas.value("descripcion").toString();
                int cantArt = qLineas.value("cantidad").toInt();
                double pvpArt = qLineas.value("pvp").toDouble();
                double subTotal = cantArt * pvpArt;
                totalProductos += subTotal;

                printer.imprimirLinea(QString("- [%1] %2").arg(cArt, dArt));
                printer.imprimirLinea(QString("  Cant: %1  PVP: %2 EUR  Subtotal: %3 EUR")
                                      .arg(QString::number(cantArt),
                                           QString::number(pvpArt, 'f', 2),
                                           QString::number(subTotal, 'f', 2)));
            } while (qLineas.next());

            printer.setNegrita(true);
            printer.imprimirLinea(QString("TOTAL PRODUCTOS: %1 EUR").arg(QString::number(totalProductos, 'f', 2)));
            printer.setNegrita(false);
        }
    }

    if (!desglosadoMultiproducto) {
        // Modo legado de producto único
        printer.setNegrita(true);
        printer.imprimirLinea("CODIGO: " + codArticulo);
        printer.imprimirLinea("ARTICULO: " + descArticulo);
        printer.setNegrita(false);
        printer.imprimirLinea("CANTIDAD ENCARGADA: " + QString::number(cantidad));
    }

    printer.imprimirLinea(QString("ANTICIPO A CUENTA: %1 EUR").arg(QString::number(anticipo, 'f', 2)));
    printer.imprimirLinea("FORMA PAGO ANTICIPO: " + formaPago);

    printer.imprimirLineaSeparadora('-', 48);

    // 6. Observaciones / Notas si no están vacías
    if (!notas.trimmed().isEmpty()) {
        printer.imprimirLinea("NOTAS / OBSERVACIONES:");
        printer.imprimirLinea(notas);
        printer.imprimirLineaSeparadora('-', 48);
    }

    // 7. Pie del ticket configurado
    if (confTicket.size() > 1 && !confTicket.at(1).isEmpty()) {
        printer.imprimirLineaCentrada(confTicket.at(1));
    }

    // 8. Alimentar líneas y cortar el papel
    printer.alimentarLineas(3);

    if (confTicket.size() > 5 && !confTicket.at(5).isEmpty()) {
        QString codCorte = confTicket.at(5);
        QStringList cadaCodCorte = codCorte.split(",");
        QByteArray comandoCorte;
        for (int i = 0; i < cadaCodCorte.size(); ++i) {
            comandoCorte.append(static_cast<char>(cadaCodCorte.at(i).toInt()));
        }
        printer.enviarComando(comandoCorte);
    } else {
        printer.cortarPapel(true);
    }

    printer.cerrarImpresora();
    qDebug() << "ImprimirTicket::imprimirComprobanteEncargo: Comprobante impreso correctamente.";
    return true;
}


