#include "configuracion.h"

Configuracion::Configuracion()
{
}

Configuracion::~Configuracion()
{

}

QStringList Configuracion::getNombreConexiones()
{
    return nombreconexiones;
}

void Configuracion::setNombreconexiones(QStringList conn)
{
    nombreconexiones = conn;
}

QStringList Configuracion::getNombreConexionesActivas()
{
    return nombreconexionesActivas;
}

void Configuracion::setNombreConexionesActivas(QStringList conn)
{
    nombreconexionesActivas = conn;
}

QString Configuracion::getConexionMaster()
{
    return conexionMaster;
}

void Configuracion::setConexionMaster(QString conn)
{
    conexionMaster = conn;
}

QString Configuracion::getConexionLocal()
{
    return conexionLocal;
}

void Configuracion::setConexionLocal(QString conn)
{
    conexionLocal = conn;
}

QString Configuracion::getUsuario()
{
    return usuario;
}

void Configuracion::setUsuario(QString user)
{
    usuario = user;
}

int Configuracion::getRol()
{
    return rol;
}

void Configuracion::setRol(int setRol)
{
    rol = setRol;
}

