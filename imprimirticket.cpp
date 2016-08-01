#include "imprimirticket.h"
#include <QFile>
#include <QDate>



ImprimirTicket::ImprimirTicket(QString nTicket)
{
    consulta = base.datosTicket(nTicket);
    ticket = consulta.value(0).toString();
    fecha = consulta.value(3).toString();
    hora = consulta.value(4).toString();
    total = consulta.value(12).toString();
    fPago = base.nombreFormaPago(consulta.value(13).toString());
    QFile impresora("ticket.txt");
    impresora.open(QIODevice::WriteOnly);
    QTextStream texto(&impresora);
    texto << "\n";
    texto << "HERBOLARIO EMEICJAC\n";
    texto << "C/Perines 14 bajo\n";
    texto << "Tlfn: 942-37-20-27\n";
    texto << "N.I.F.: 20196639-V\n";
    texto << "E-mail: emeicjac@emeicjac.com\n";
    texto << "Web: emeicjac.com\n\n";
    texto << QDate::fromString(fecha,"yyyy-MM-dd").toString("dd-MMM-yyyy") + "  " + hora + "    " + "Ticket: "+ticket;
    texto <<"\n";
    texto << "UDS|  Producto            |Prec.|Dto|Total\n";
    texto << "------------------------------------------\n";

    consulta = base.consultarLineasTicket(ticket);
    while(consulta.next()) {
        uds = consulta.value(4).toString();
        producto = consulta.value(3).toString();
        precio = consulta.value(6).toString();
        dto = consulta.value(7).toString();
        totalLinea = consulta.value(8).toString();
        texto << formatearCadena(uds,3);
        texto << formatearCadena(producto,24)+" ";
        texto << formatearCadena(precio,6);
        texto << formatearCadena(dto,2);
        texto << formatearCadena(totalLinea,6);
    }
    texto << "\n\n";
    texto << "Total: "+total+"\n";
    texto << "Forma de pago. "+fPago;
    texto << "\n\n\n     GRACIAS POR SU VISITA\n";
    texto << "\n\n\n\n";
    texto << char(0x1D) << char(0x56) << char(0x30);
    impresora.close();
    system("less ./ticket.txt >> /dev/lp0");
}

QString ImprimirTicket::formatearCadena(QString cadena, int tamano)
{
    if (cadena.length() >= tamano) {
        cadena.resize(tamano);
       return cadena;
    }
    int diferencia = tamano-cadena.length();
    for (int i = 0; i < diferencia; ++i) {
        cadena += " ";
    }
    return cadena;
}
