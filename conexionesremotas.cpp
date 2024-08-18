#include "conexionesremotas.h"
#include "conexion.h"
conexionesRemotas::conexionesRemotas(QObject *parent)
    : QObject(parent)
{}

QStringList conexionesRemotas::crear()
{
    listaConexionesRemotas.clear();
    QSqlQuery conexiones = base->tiendas(QSqlDatabase::database("DB"));
    conexiones.first();
    qDebug() << conexiones.numRowsAffected();
    for (int i = 0; i < conexiones.numRowsAffected(); ++i) {
        QString host = conexiones.value("ip").toString();
        QString puerto = "3306";
        QString baseDatos = "tienda";
        QString usuario = conexiones.value("usuario").toString();
        QString constrasena = conexiones.value("password").toString();
        QString nombreConexion = conexiones.value("nombre").toString();
        if (createConnection(host, puerto, baseDatos, usuario, constrasena, nombreConexion)) {
            qDebug() << "conexion creada: " << nombreConexion;
            listaConexionesRemotas << nombreConexion << "1";
        } else {
            listaConexionesRemotas << nombreConexion << "0";
        }

        conexiones.next();
    }

    qDebug() << "Lista online" << listaConexionesRemotas.size();
    return listaConexionesRemotas;
}

QStringList conexionesRemotas::lista()
{
    listaOrdenadoresRemotos.clear();
    QSqlQuery tiendas = base->tiendas(QSqlDatabase::database("DB"));
    tiendas.first();
    qDebug() << tiendas.numRowsAffected();
    qDebug() << tiendas.lastError();
    for (int i = 0; i < tiendas.numRowsAffected(); ++i) {
        listaOrdenadoresRemotos.append(tiendas.value("nombre").toString());
        tiendas.next();
    }
    qDebug() << "Lista ordenadores remotos :" << listaOrdenadoresRemotos;
    return listaOrdenadoresRemotos;
}

QStringList conexionesRemotas::listaOnLine()
{
    listaConexionesActivas.clear();
    QSqlQuery conexiones = base->tiendas(QSqlDatabase::database("DB"));
    conexiones.first();
    qDebug() << conexiones.numRowsAffected();
    for (int i = 0; i < conexiones.numRowsAffected(); ++i) {
        QString host = conexiones.value("ip").toString();
        QString puerto = "3306";
        QString baseDatos = "tienda";
        QString usuario = conexiones.value("usuario").toString();
        QString constrasena = conexiones.value("password").toString();
        QString nombreConexion = conexiones.value("nombre").toString();
        if (createConnection(host, puerto, baseDatos, usuario, constrasena, nombreConexion)) {
            qDebug() << "conexion creada: " << nombreConexion;
            listaConexionesActivas << nombreConexion;
        }

        conexiones.next();
    }
    return listaConexionesActivas;
}

QString conexionesRemotas::conexionMaster()
{
    QString nombreConexionMaster = base->nombreConexionMaster();
    return nombreConexionMaster;
}
