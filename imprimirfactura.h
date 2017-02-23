#ifndef IMPRIMIRFACTURA_H
#define IMPRIMIRFACTURA_H

#include <qstandarditemmodel.h>
#include "base_datos.h"

class ImprimirFactura : public QObject
{
    Q_OBJECT
public:
    explicit ImprimirFactura(QString nTicket , QObject *parent = 0);
private:
    QStandardItemModel *modeloTabla;
    QSqlQuery consulta;
    baseDatos base;
    QString fecha,hora,ticket,total,fPago,idCliente, idVendedor , vendedor , cliente;
    QString uds,producto,precio,dto,totalLinea;
    QString base21 , iva21 , base10 , iva10 , base4 , iva4 , totalFactura ;
signals:

public slots:
};

#endif // IMPRIMIRFACTURA_H
