#ifndef IMPRIMIRTICKET_H
#define IMPRIMIRTICKET_H

#include <QDebug>
#include <QString>
#include "base_datos.h"


class ImprimirTicket
{
public:
    ImprimirTicket(QString nTicket);

private:
    QString formatearCadena(QString cadena, int tamano);

    baseDatos base;
    QSqlQuery consulta;

    QString fecha,hora,ticket,total,fPago;
    QString uds,producto,precio,dto,totalLinea;
};

#endif // IMPRIMIRTICKET_H
