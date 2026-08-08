#include "conexionesremotas.h"
#include "conexion.h"
conexionesRemotas::conexionesRemotas(QObject *parent)
    : QObject(parent)
{}

QStringList conexionesRemotas::crear(const QStringList &filtroNombres)
{
    listaConexionesRemotas.clear();
    if (!base) return listaConexionesRemotas;
    QSqlQuery conexiones = base->tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (conexiones.next()) {
        QString nombreConexion = conexiones.value("nombre").toString();
        if (!filtroNombres.isEmpty() && !filtroNombres.contains(nombreConexion)) {
            continue;
        }
        QString host = conexiones.value("ip").toString();
        // Leer el puerto desde la BD; si está vacío o es 0, usar 3306 por defecto
        QString puerto = conexiones.value("puerto").toString();
        if (puerto.isEmpty() || puerto == "0") {
            puerto = "3306";
        }
        QString baseDatos = conexiones.value("baseDatos").toString();
        if (baseDatos.isEmpty()) {
            baseDatos = "tiendaNueva";
        }
        QString usuario = conexiones.value("usuario").toString();
        QString constrasena = conexiones.value("password").toString();
        // nombreConexion ya está declarada al inicio del bucle
        // Leer el certificado CA para SSL (vacío = sin SSL)
        QString sslCa = conexiones.value("ssl_ca").toString();
        if (createConnection(host, puerto, baseDatos, usuario, constrasena, nombreConexion, sslCa)) {
            qDebug() << "conexion creada: " << nombreConexion;
            listaConexionesRemotas << nombreConexion << "1";
        } else {
            listaConexionesRemotas << nombreConexion << "0";
        }
    }

    qDebug() << "Lista online" << listaConexionesRemotas.size() / 2;
    return listaConexionesRemotas;
}

QStringList conexionesRemotas::lista()
{
    listaOrdenadoresRemotos.clear();
    if (!base) return listaOrdenadoresRemotos;
    QSqlQuery tiendas = base->tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (tiendas.next()) {
        listaOrdenadoresRemotos.append(tiendas.value("nombre").toString());
    }
    qDebug() << "Lista ordenadores remotos :" << listaOrdenadoresRemotos;
    return listaOrdenadoresRemotos;
}

QStringList conexionesRemotas::listaOnLine()
{
    listaConexionesActivas.clear();
    if (!base) return listaConexionesActivas;
    QSqlQuery conexiones = base->tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (conexiones.next()) {
        QString host = conexiones.value("ip").toString();
        // Leer el puerto desde la BD; si está vacío o es 0, usar 3306 por defecto
        QString puerto = conexiones.value("puerto").toString();
        if (puerto.isEmpty() || puerto == "0") {
            puerto = "3306";
        }
        QString baseDatos = conexiones.value("baseDatos").toString();
        if (baseDatos.isEmpty()) {
            baseDatos = "tiendaNueva";
        }
        QString usuario = conexiones.value("usuario").toString();
        QString constrasena = conexiones.value("password").toString();
        QString nombreConexion = conexiones.value("nombre").toString();
        // Leer el certificado CA para SSL (vacío = sin SSL)
        QString sslCa = conexiones.value("ssl_ca").toString();
        if (createConnection(host, puerto, baseDatos, usuario, constrasena, nombreConexion, sslCa)) {
            qDebug() << "conexion creada: " << nombreConexion;
            listaConexionesActivas << nombreConexion;
        }
    }
    return listaConexionesActivas;
}

QString conexionesRemotas::conexionMaster()
{
    QString nombreConexionMaster = base->nombreConexionMaster();
    return nombreConexionMaster;
}
