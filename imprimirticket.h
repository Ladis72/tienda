#ifndef IMPRIMIRTICKET_H
#define IMPRIMIRTICKET_H

#include <QDebug>
#include <QString>
#include "base_datos.h"
#include "configuracion.h"
#include <qstandarditemmodel.h>

extern Configuracion *conf;

class ImprimirTicket
{
public:
    ImprimirTicket(QString nTicket, QString formato);
    void ImprimirTicketRegalo();

private:
    QString formatearCadena(QString cadena, int tamano);

    baseDatos base;
    QSqlQuery consulta;

    QString fecha, hora, ticket, total, fPago;
    QString uds, producto, precio, dto, totalLinea, entrega, cambio;
};

#endif // IMPRIMIRTICKET_H
