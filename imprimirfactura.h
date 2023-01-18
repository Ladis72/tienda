#ifndef IMPRIMIRFACTURA_H
#define IMPRIMIRFACTURA_H

#include <qstandarditemmodel.h>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

class ImprimirFactura : public QObject
{
    Q_OBJECT
public:
    explicit ImprimirFactura(QString nTicket , QObject *parent = nullptr);
private:
    QStandardItemModel *modeloTabla;
    QSqlQuery consulta;
    baseDatos base;
    QString fecha,hora,ticket,total,fPago,idCliente, idVendedor , vendedor , cliente;
    QString uds,producto,precio,dto,totalLinea;
    QString base21 , iva21 , base10 , iva10 , base4 , iva4 , base5 , iva5 , base0 , iva0 , totalFactura ;
signals:

public slots:
};

#endif // IMPRIMIRFACTURA_H
