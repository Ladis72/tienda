#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>
#include <QSqlRecord>

#include <QDebug>

class baseDatos
{
public:
    baseDatos();
    bool base_datos_abierta();
    QSqlDatabase conectar();
    QSqlQuery usuarios(QSqlDatabase db);
    QSqlQuery fpago(QSqlDatabase db);
    QSqlDatabase conexion();
    QSqlQuery consulta_producto(QSqlDatabase db, QString cod);
    QSqlQuery buscarProducto (QSqlDatabase db, QString tabla, QString nombre);
    bool insertarUsuario(QSqlDatabase db, QStringList datos);
    QSqlQuery buscarEnTabla(QSqlDatabase db, QString tabla, QString campo, QString dato);
    bool modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato);
    bool modificarFotoUsusario(QString foto, int id);
    bool modificarArticulo(QSqlDatabase db, QStringList datos, QString dato);
    bool insertarArticulo(QSqlDatabase db, QStringList datos);
    bool borrarUsusario(QSqlDatabase db, int dato);
    bool borrarArticulo(QSqlDatabase db, QString dato);
    bool modificarFotoArticulo(QString foto, QString dato);
    QString nombreFamilia(QString id);
    QString nombreFabricante(QString id);
    QString nombreUsusario(QString id);
    double obtenerNumeroUltimoTicket(QSqlDatabase db);
    QSqlQuery tcketsPendientes(QSqlDatabase db);
    bool modificarCliente(QSqlDatabase db, QStringList datos, QString dato);
    bool borrarCliente(QSqlDatabase db, int idCliente);
private:

    QSqlDatabase db;
    QSqlQuery consulta;
    QSqlQueryModel *modelo;
    QString cod;
};

#endif // BASE_DATOS_H
