#ifndef CONEXIONESREMOTAS_H
#define CONEXIONESREMOTAS_H

#include "base_datos.h"

#include <QObject>

class conexionesRemotas : public QObject
{
    Q_OBJECT
public:
    explicit conexionesRemotas(QObject *parent = nullptr);
    QStringList crear();
    QStringList lista();
    QStringList listaOnLine();

    QStringList listaOrdenadoresRemotos;
    QStringList listaConexionesRemotas;
    QStringList listaConexionesActivas;
    baseDatos *base;
    QSqlQuery tiendas;
    QSqlQuery conexiones;

signals:

public slots:
};

#endif // CONEXIONESREMOTAS_H
