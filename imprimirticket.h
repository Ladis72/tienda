#ifndef IMPRIMIRTICKET_H
#define IMPRIMIRTICKET_H

#include <QDebug>
#include <QString>
#include "base_datos.h"
#include "configuracion.h"
#include "printermanager.h"
#include <qstandarditemmodel.h>

extern Configuracion *conf;

class ImprimirTicket : public QObject
{
public:
    explicit ImprimirTicket(QString nTicket, QString formato, QObject *parent = nullptr);
    ~ImprimirTicket();

    void ImprimirTicketRegalo();

private:
    baseDatos base;
    QSqlQuery consulta;

    QString fecha, hora, ticket, total, fPago;
    QString uds, producto, precio, dto, totalLinea, entrega, cambio;
    QString formatearCadena(QString cadena, int tamano);
    QStringList confTicket;

    PrinterManager *printer;

    bool imprimirCabecera();
    bool imprimirLineasProductos();
    bool imprimirPie();
    bool imprimirLogo();
};

#endif // IMPRIMIRTICKET_H
