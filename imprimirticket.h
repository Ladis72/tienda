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
    explicit ImprimirTicket(QString nTicket, QString formato, bool noTicketRegalo = false, bool esSerieB = false, QObject *parent = nullptr);
    ~ImprimirTicket();

    /**
     * @brief Imprime un comprobante de encargo en la impresora térmica de tickets.
     * @param idEncargo ID del encargo (0 si es recién creado y no se dispone del ID)
     * @param codCliente Código o ID del cliente
     * @param codArticulo Código del artículo encargado
     * @param descArticulo Descripción del artículo encargado
     * @param cantidad Cantidad del encargo
     * @param anticipo Importe adelantado a cuenta
     * @param formaPago Forma de pago del anticipo
     * @param notas Observaciones o notas adicionales
     * @param empleado Nombre del empleado responsable
     * @param fechaStr Fecha en formato "yyyy-MM-dd" (si es vacía, usa la fecha actual)
     * @return true si el proceso de impresión se completó con éxito
     */
    static bool imprimirComprobanteEncargo(int idEncargo,
                                           const QString &codCliente,
                                           const QString &codArticulo,
                                           const QString &descArticulo,
                                           int cantidad,
                                           double anticipo,
                                           const QString &formaPago,
                                           const QString &notas,
                                           const QString &empleado,
                                           const QString &fechaStr = "");


private:
    baseDatos base;
    QSqlQuery consulta;

    QString fecha, hora, ticket, total, fPago;
    QString uds, producto, precio, dto, totalLinea, entrega, cambio;
    QString formatearCadena(QString cadena, int tamano);
    QStringList confTicket;
    bool esSerieB;

    PrinterManager *printer;

    bool imprimirCabecera();
    bool imprimirLineasProductos();
    bool imprimirPie();
    bool imprimirLogo();
    bool ImprimirTicketRegalo(QString ruta);
    bool cortarPapel();

};

#endif // IMPRIMIRTICKET_H
