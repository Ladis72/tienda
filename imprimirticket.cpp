#include "imprimirticket.h"
#include <QDate>
#include <QFile>

ImprimirTicket::ImprimirTicket(QString nTicket, QString formato, bool noTicketRegalo, QObject *parent)
    : QObject(parent)
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


