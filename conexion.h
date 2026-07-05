#ifndef CONEXION_H
#define CONEXION_H

#endif // CONEXION_H
#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/*
    SEC-01: Funciones de ayuda para abrir conexiones a la base de datos.
    Las credenciales se leen del fichero tienda.ini (excluido de Git).
*/

//! [0]
/**
 * @brief Crea la conexión por defecto leyendo credenciales de tienda.ini.
 *
 * El fichero tienda.ini debe estar junto al ejecutable con el formato:
 * @code
 *   [BaseDatos]
 *   host=localhost
 *   puerto=3306
 *   baseDatos=tiendaNueva
 *   usuario=root
 *   clave=miContraseña
 * @endcode
 *
 * @return true si la conexión se abre correctamente.
 */
[[maybe_unused]] static bool createConnection()
{
    // SEC-01: Leer credenciales desde fichero externo (no hardcodeadas)
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("BaseDatos");

    QString host     = settings.value("host", "localhost").toString();
    int     puerto   = settings.value("puerto", 3306).toInt();
    QString baseDatos= settings.value("baseDatos", "tiendaNueva").toString();
    QString usuario  = settings.value("usuario", "").toString();
    QString clave    = settings.value("clave", "").toString();
    settings.endGroup();

    if (usuario.isEmpty()) {
        QMessageBox::critical(nullptr, "Error de configuración",
            "No se ha encontrado el fichero de configuración tienda.ini\n"
            "o falta el campo 'usuario' en la sección [BaseDatos].\n\n"
            "Ruta esperada: " + iniPath);
        return false;
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "DB");
    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");
    db.setHostName(host);
    db.setDatabaseName(baseDatos);
    db.setUserName(usuario);
    db.setPassword(clave);
    db.setPort(puerto);
    if (!db.open()) {
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar" + db.lastError().text());
        mensaje.setWindowTitle("Error");
        mensaje.exec();
        return false;
    }

    // Asegurar de forma eficiente que las columnas de notas existen en albaranes_tmp y pedidos
    QSqlQuery checkQuery(db);
    
    // Comprobar si existe la columna en 'albaranes_tmp'
    checkQuery.prepare("SELECT COUNT(*) FROM information_schema.columns "
                       "WHERE table_schema = DATABASE() AND table_name = 'albaranes_tmp' AND column_name = 'notas'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QSqlQuery queryAlter(db);
        queryAlter.exec("ALTER TABLE albaranes_tmp ADD COLUMN notas TEXT NULL");
    }

    // Comprobar si existe la columna en 'pedidos'
    checkQuery.prepare("SELECT COUNT(*) FROM information_schema.columns "
                       "WHERE table_schema = DATABASE() AND table_name = 'pedidos' AND column_name = 'notas'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QSqlQuery queryAlter(db);
        queryAlter.exec("ALTER TABLE pedidos ADD COLUMN notas TEXT NULL");
    }

    // Comprobar si existe la columna en 'facturas'
    checkQuery.prepare("SELECT COUNT(*) FROM information_schema.columns "
                       "WHERE table_schema = DATABASE() AND table_name = 'facturas' AND column_name = 'notas'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QSqlQuery queryAlter(db);
        queryAlter.exec("ALTER TABLE facturas ADD COLUMN notas TEXT NULL");
    }

    // Comprobar si existe la columna en 'albaranes'
    checkQuery.prepare("SELECT COUNT(*) FROM information_schema.columns "
                       "WHERE table_schema = DATABASE() AND table_name = 'albaranes' AND column_name = 'notas'");
    if (checkQuery.exec() && checkQuery.next() && checkQuery.value(0).toInt() == 0) {
        QSqlQuery queryAlter(db);
        queryAlter.exec("ALTER TABLE albaranes ADD COLUMN notas TEXT NULL");
    }

    return true;
}
[[maybe_unused]] static bool createConnection(QString host,
                             QString puerto,
                             QString baseDatos,
                             QString usuario,
                             QString clave,
                             QString nombreConexion,
                             QString sslCa = QString())
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", nombreConexion);
    db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");
    db.setHostName(host);
    db.setDatabaseName(baseDatos);
    db.setUserName(usuario);
    db.setPassword(clave);
    db.setPort(puerto.toInt());

    // Si se indica un fichero CA, activar SSL para MySQL/MariaDB
    if (!sslCa.isEmpty()) {
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3;SSL_CA=" + sslCa);
    }

    if (!db.open()) {
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar" + db.lastError().text());
        mensaje.setWindowTitle("Error");
        mensaje.exec();
        return false;
    }

    return true;
}
