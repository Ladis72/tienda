#ifndef IMPRIMIRFACTURAPROVEEDOR_H
#define IMPRIMIRFACTURAPROVEEDOR_H

#include <QObject>
#include <QStandardItemModel>
#include "base_datos.h"

class imprimirFacturaProveedor
{
public:
    imprimirFacturaProveedor(QString tienda, QStringList cabecera , QString idFactura);

private:
    baseDatos *base;
};

#endif // IMPRIMIRFACTURAPROVEEDOR_H
