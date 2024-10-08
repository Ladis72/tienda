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
    QString getConexionMaster();
    void setConexionMaster(QString conn);
    QString getConexionLocal();
    void setConexionLocal(QString conn);
    QString getUsuario();
    void setUsuario(QString user);
    int getRol();
    void setRol(int setRol);

private:
    QStringList nombreconexiones;
    QStringList nombreconexionesActivas;
    QString conexionMaster;
    QString conexionLocal;
    int rol;
};

#endif // CONFIGURACION_H
