#ifndef IMPRIMIRPEDIDO_H
#define IMPRIMIRPEDIDO_H

#include <QObject>
#include <QSqlQueryModel>

class imprimirPedido
{
public:
    imprimirPedido(QString tienda, QStringList cabecera, QSqlQueryModel *modelo);
};

#endif // IMPRIMIRPEDIDO_H
