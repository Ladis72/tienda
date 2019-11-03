#include "configuracion.h"

Configuracion::Configuracion()
{
    usuario ="Ladis";
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

