#include "imprimirticket.h"
#include <QDate>
#include <QFile>

ImprimirTicket::ImprimirTicket(QString nTicket, QString formato)
{
    consulta = base.datosTicket(conf->getConexionLocal(), nTicket);
    ticket = consulta.value(0).toString();
    fecha = consulta.value(3).toString();
    hora = consulta.value(4).toString();
    total = consulta.value(8).toString();
    fPago = base.nombreFormaPago(consulta.value(9).toString(), conf->getConexionLocal());
    entrega = consulta.value(11).toString();
    cambio = consulta.value(12).toString();
    QStringList confTicket = base.recuperarConfigTicket();
    if (formato == "ticket") {
        QFile impresora("ticket.txt");
        impresora.open(QIODevice::WriteOnly);
        QTextStream texto(&impresora);

        texto << confTicket.at(0) + "\n\n";
        texto << QDate::fromString(fecha, "yyyy-MM-dd").toString("dd-MMM-yyyy") + "  " + hora
                     + "    " + "Ticket: " + ticket;
        texto << "\n";
        texto << "UDS|  Producto            |Prec.|Dto|Total\n";
        texto << "------------------------------------------\n";

        consulta = base.consultarLineasTicket(conf->getConexionLocal(), ticket);
        while (consulta.next()) {
            uds = consulta.value(4).toString();
            producto = consulta.value(3).toString();
            precio = consulta.value(6).toString();
            dto = consulta.value(7).toString();
            totalLinea = consulta.value(8).toString();
            texto << formatearCadena(uds, 3);
            texto << formatearCadena(producto, 24) + " ";
            texto << formatearCadena(precio, 6);
            texto << formatearCadena(dto, 2);
            texto << formatearCadena(totalLinea, 6);
            texto << "\n";
        }
        texto << "\n\n";
        texto << "Total: " + total + "\n";
        texto << "Forma de pago. " + fPago;
        texto << "\n";
        texto << "Entrega: " + entrega + "\n";
        texto << "Cambio:  " + cambio;
        texto << "\n\n\n";
        texto << confTicket.at(1);
        texto << "\n\n\n\n";
        //    texto << char(0x1D) << char(0x56) << char(0x30);
        QString codCorte = confTicket.at(5);
        qDebug() << codCorte;
        QStringList cadaCodCorte = codCorte.split(",");
        for (int i = 0; i < cadaCodCorte.size(); ++i) {
            texto << char(cadaCodCorte.at(i).toInt());
        }
        //texto << confTicket.at(4);
        texto << "\n\n";
        impresora.close();
        QString imprimir = "cat ./ticket.txt >> " + confTicket.at(3);
        const char *ch = imprimir.toLocal8Bit().constData();
        system(ch);
        return;
    }
}

void ImprimirTicket::ImprimirTicketRegalo() {}

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
