#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include <QString>
#include <QStringList>

class Configuracion
{
public:
    Configuracion();
    ~Configuracion();
    QString usuario;
    QStringList getNombreConexiones();
    void setNombreconexiones(QStringList conn);
    QStringList getNombreConexionesActivas();
    void setNombreConexionesActivas(QStringList conn);
private:
    QStringList nombreconexiones;
    QStringList nombreconexionesActivas;
};

#endif // CONFIGURACION_H
