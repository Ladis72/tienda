#ifndef INFORMES1_H
#define INFORMES1_H

#include "configuracion.h"

#include <QDebug>
#include <QObject>
#include <QStandardItemModel>
extern Configuracion *conf;


class informes1
{
public:
    informes1(QString tienda, QString fechaI, QString fechaF, QStandardItemModel *tabla);
    bool imprimir(QString tienda, QString fechaI, QString fechaF, QStandardItemModel *tabla);
};

#endif // INFORMES1_H
