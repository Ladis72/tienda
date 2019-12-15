#ifndef CONEXION_H
#define CONEXION_H

#endif // CONEXION_H
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    This file defines a helper function to open a connection to an
    in-memory SQLITE database and to create a test table.

    If you want to use another database, simply modify the code
    below. All the examples in this directory use this function to
    connect to a database.
*/
//! [0]
static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","DB");
    db.setHostName("localhost");
    db.setDatabaseName("tienda");
    db.setUserName("root");
    db.setPassword("meganizado");
    db.setPort(3306);
        if(!db.open()){
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar"+db.lastError().text());
        mensaje.setWindowTitle ("Error");
        mensaje.exec();
        return false;
        }

return true;



}
static bool createConnection(QString host , QString puerto , QString baseDatos , QString usuario , QString clave , QString nombreConexion)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL",nombreConexion);
    db.setHostName(host);
    db.setDatabaseName(baseDatos);
    db.setUserName(usuario);
    db.setPassword(clave);
    db.setPort(puerto.toInt());
        if(!db.open()){
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar"+db.lastError().text());
        mensaje.setWindowTitle ("Error");
        mensaje.exec();
        return false;
        }

return true;

}
