#include "base_datos.h"
#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include "qprocess.h"

baseDatos::baseDatos() {}

bool baseDatos::conectar(
    QString host, QString puerto, QString baseDatos, QString usuario, QString clave)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "DB");
    db.setHostName(host);
    db.setDatabaseName(baseDatos);
    db.setUserName(usuario);
    db.setPassword(clave);
    db.setPort(puerto.toInt());
    if (!db.open()) {
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar. " + db.lastError().text());
        mensaje.setWindowTitle("Error");
        mensaje.exec();
        return false;
    }

    return true;
}

bool baseDatos::guardarDatosConexion(
    QString host, QString puerto, QString baseDatos, QString usuario, QString clave)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configBase SET servidor =? , puerto =?, baseDatos =? , usuario =? , "
                     "clave =? WHERE id=1");
    consulta.bindValue(0, host);
    consulta.bindValue(1, puerto.toInt());
    consulta.bindValue(2, baseDatos);
    consulta.bindValue(3, usuario);
    consulta.bindValue(4, clave);
    if (!consulta.exec()) {
        QMessageBox mensaje;
        mensaje.setText("No se han podido guardar los datos" + consulta.lastError().text());
        mensaje.setWindowTitle("Error");
        mensaje.exec();
        return false;
    }
    return true;
}

QStringList baseDatos::datosConexion()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM configBase");
    consulta.first();
    QStringList datos;
    datos.clear();
    qDebug() << consulta.size();
    for (int i = 1; i < 6; ++i) {
        datos.append(consulta.value(i).toString());
    }
    return datos;
}
bool baseDatos::guardarDatosConexionMaster(
    QString host, QString puerto, QString baseDatos, QString usuario, QString clave)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configMaster SET servidor =? , puerto =?, baseDatos =? , usuario =? , "
                     "clave =? WHERE id=1");
    consulta.bindValue(0, host);
    consulta.bindValue(1, puerto.toInt());
    consulta.bindValue(2, baseDatos);
    consulta.bindValue(3, usuario);
    consulta.bindValue(4, clave);
    if (!consulta.exec()) {
        QMessageBox mensaje;
        mensaje.setText("No se han podido guardar los datos" + consulta.lastError().text());
        mensaje.setWindowTitle("Error");
        mensaje.exec();
        return false;
    }
    return true;
}

QStringList baseDatos::datosConexionMaster()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM configMaster");
    consulta.first();
    QStringList datos;
    datos.clear();
    qDebug() << consulta.size();
    for (int i = 1; i < 6; ++i) {
        datos.append(consulta.value(i).toString());
    }
    return datos;
}

QStringList baseDatos::datosConexionLocal()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT nombre , ip , usuario , password FROM tiendas WHERE local = 1");
    consulta.first();
    QStringList datos;
    datos.clear();
    if (!consulta.isValid()) {
        return datos;
    }

    qDebug() << consulta.size();
    for (int i = 0; i < 4; ++i) {
        datos.append(consulta.value(i).toString());
    }
    return datos;
}
bool baseDatos::base_datos_abierta()
{
    return false;
}

QSqlQuery baseDatos::usuarios(QSqlDatabase db)
{
    if (db.isOpen()) {
        QSqlQuery consulta(db);
        consulta.exec("SELECT * FROM usuarios");
        return consulta;
    }

    return QSqlQuery();
}

QSqlQuery baseDatos::fpago(QSqlDatabase db)
{
    if (db.isOpen()) {
        QSqlQuery consulta(db);
        consulta.exec("SELECT * FROM fpago");
        return consulta;
    }
    qDebug() << "Formas de pago no accesibles";
    return QSqlQuery();
}

QSqlDatabase baseDatos::conexion()
{
    if (db.isValid()) {
        return db;
    }
    qDebug() << "Sin conexion";

    return QSqlDatabase();
}

QSqlQuery baseDatos::consulta_producto(QString nombreConnexion, QString cod)
{
    qDebug() << "Nombre conexion" << nombreConnexion;
    if (QSqlDatabase::database(nombreConnexion).isOpen()) {
        QSqlQuery consulta(QSqlDatabase::database(nombreConnexion));
        consulta.exec("SELECT * FROM articulos WHERE cod LIKE '" + cod + "'");
        return consulta;
    }
    return QSqlQuery();
}

QSqlQuery baseDatos::buscarProducto(QSqlDatabase db, QString tabla, QString nombre)
{
    if (db.isOpen()) {
        QSqlQuery consulta(db);
        //consulta.exec("SELECT * FROM "+tabla+" WHERE descripcion LIKE '%"+nombre+"%' ORDER BY descripcion");
        consulta.exec("SELECT * , SUM(lotes.cantidad) FROM articulos LEFT JOIN lotes ON "
                      "articulos.cod=lotes.ean WHERE articulos.descripcion LIKE '%"
                      + nombre + "%' GROUP BY articulos.cod");
        return consulta;
    }
    return QSqlQuery();
}

bool baseDatos::insertarUsuario(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO usuarios (id , nombre , apellido , direccion , localidad , "
                     "provincia , cp , tlfn , nif , mail , usuario , clave , notas , rol , foto) "
                     "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    consulta.bindValue(0, datos.at(0).toInt());
    qDebug() << datos.at(0);

    consulta.bindValue(1, datos.at(1));
    qDebug() << datos[1];

    consulta.bindValue(2, datos.at(2));
    qDebug() << datos[2];
    consulta.bindValue(3, datos.at(3));
    qDebug() << datos[3];
    consulta.bindValue(4, datos.at(4));
    consulta.bindValue(5, datos.at(5));
    consulta.bindValue(6, datos.at(6));
    consulta.bindValue(7, datos.at(7));
    consulta.bindValue(8, datos.at(8));
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11));
    consulta.bindValue(12, datos.at(12));
    consulta.bindValue(13, datos.at(13).toInt());
    consulta.bindValue(14, datos.at(14));
    qDebug() << db.lastError().text();
    qDebug() << consulta.lastError();
    if (!consulta.exec()) {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
        db.rollback();
        //QSqlDatabase::database().rollback();
        return false;

    } else {
        db.commit(); //QSqlDatabase::database().commit();
        return true;
    }
}

/*Funcion para consultar una tabla
 *recibe la base de datos
 *       el nombre de una tabla
 *      el nombre el campo a consultar
 *      el dato a consultar
 *
 *y devuelve el registro 0 de la tabla
 *normalmente el "id" o codigo desde
 *una tabla del formulario
 *buscarproducto.ui
 *
 */

QSqlQuery baseDatos::buscarEnTabla(QSqlDatabase db, QString tabla, QString campo, QString dato)
{
    if (db.isOpen()) {
        QSqlQuery consulta(db);
        consulta.exec("SELECT * FROM " + tabla + " WHERE " + campo + " LIKE '%" + dato + "%'");
        qDebug() << consulta.lastError().text();

        return consulta;
    }
    qDebug() << consulta.lastError().text();

    return QSqlQuery();
}

bool baseDatos::modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("UPDATE usuarios SET id =?, nombre=? , apellido=? , direccion=? , localidad=? "
                     ", provincia=? , cp=? , tlfn=? , nif=? , mail=? , usuario=? , clave=? , "
                     "notas=? , rol=? , foto=?  WHERE id=?");
    consulta.bindValue(0, datos.at(0).toInt());
    qDebug() << dato.at(0);

    consulta.bindValue(1, datos.at(1));
    qDebug() << dato[1];

    consulta.bindValue(2, datos.at(2));
    qDebug() << dato[2];
    consulta.bindValue(3, datos.at(3));
    qDebug() << dato[3];
    consulta.bindValue(4, datos.at(4));
    consulta.bindValue(5, datos.at(5));
    consulta.bindValue(6, datos.at(6));
    consulta.bindValue(7, datos.at(7));
    consulta.bindValue(8, datos.at(8));
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11));
    consulta.bindValue(12, datos.at(12));
    consulta.bindValue(13, datos.at(13).toInt());
    consulta.bindValue(14, datos.at(14));
    consulta.bindValue(15, dato.toInt());

    qDebug() << db.lastError().text();
    qDebug() << consulta.lastError();
    if (!consulta.exec()) {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
        db.rollback();
        //QSqlDatabase::database().rollback();
        return false;

    } else {
        db.commit(); //QSqlDatabase::database().commit();
        return true;
    }
}

bool baseDatos::modificarFotoUsusario(QString foto, int id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE usuarios SET foto=? WHERE id =?");
    consulta.bindValue(0, foto);
    consulta.bindValue(1, id);
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError();
    return false;
}

bool baseDatos::modificarArticulo(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare(
        "UPDATE articulos SET cod = ? , descripcion = ? , pvp = ? , iva = ? , stock = ? , min = ? "
        ", max = ? , pendientes_pedido = ? , encargados = ? , ultima_venta = ? , ultimo_pedido = ? "
        ", familia = ? , precio_compra = ? , fabricante = ? , foto = ? , notas = ? , formato = ? , "
        "cantformato = ? WHERE cod = ?");
    consulta.bindValue(0, datos.at(0));
    consulta.bindValue(1, datos.at(1));
    consulta.bindValue(2, datos.at(2).toDouble());
    consulta.bindValue(3, datos.at(3).toInt());
    consulta.bindValue(4, datos.at(4).toInt());
    consulta.bindValue(5, datos.at(5).toInt());
    consulta.bindValue(6, datos.at(6).toInt());
    consulta.bindValue(7, datos.at(7).toInt());
    consulta.bindValue(8, datos.at(8).toInt());
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11).toInt());
    consulta.bindValue(12, datos.at(12).toDouble());
    consulta.bindValue(13, datos.at(13).toInt());
    consulta.bindValue(14, datos.at(14));
    consulta.bindValue(15, datos.at(15));
    consulta.bindValue(16, datos.at(16));
    consulta.bindValue(17, datos.at(17).toDouble());
    consulta.bindValue(18, dato);

    qDebug() << db.lastError().text();
    qDebug() << consulta.lastError();
    if (!consulta.exec()) {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
        db.rollback();
        //QSqlDatabase::database().rollback();
        return false;

    } else {
        db.commit(); //QSqlDatabase::database().commit();
        return true;
    }
}

bool baseDatos::insertarArticulo(QSqlDatabase db, QStringList datos)
{
    qDebug() << datos;
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO articulos VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (!consulta.exec()) {
        db.rollback();
        qDebug() << consulta.lastError();
        return false;
    } else {
        db.commit();
        qDebug() << "Articulo insertado";
        return true;
    }
}

bool baseDatos::descontarArticulo(QString cod, int uds)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));

    consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '" + cod + "' ORDER BY fecha ASC");
    consulta.first();
    if (consulta.isValid()) {
        if (consulta.value("cantidad").toInt() > uds) {
            QString registro = consulta.value("id").toString();
            consulta.prepare("UPDATE lotes SET cantidad = cantidad - ? WHERE id = ?");
            consulta.bindValue(0, uds);
            consulta.bindValue(1, registro);
            if (consulta.exec()) {
                return true;
            } else {
                qDebug() << consulta.lastError().text();
                return false;
            }
        } else if (consulta.value("cantidad").toInt() <= uds) {
            QString registro = consulta.value("id").toString();
            int resta = uds - consulta.value("cantidad").toInt();
            if (resta == 0) {
                consulta.exec("DELETE FROM lotes WHERE id = '" + registro + "'");
                qDebug() << consulta.lastError().text();
                return true;
            } else {
                consulta.exec("DELETE FROM lotes WHERE id = '" + registro + "'");
                descontarArticulo(cod, resta);
                qDebug() << consulta.lastError().text();
                return true;
            }
        }
    } else {
        qDebug() << "No hay lotes de ese artículo";
        crearLote(conf->getConexionLocal(), cod, "", "2000-01-01", QString::number(0 - uds));
    }
    return true;
}

bool baseDatos::actualizarFechaVentaArticulo(QString cod, QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET ultima_venta = ? WHERE cod LIKE ?");
    consulta.bindValue(0, fecha);
    consulta.bindValue(1, cod);
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::actualizarArticulosDesdeCompras(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET descripcion = ? , pvp = ? , iva = ? , precio_compra = ? "
                     ", ultimo_pedido = ?  WHERE cod = ?");
    consulta.bindValue(0, datos.at(1));
    consulta.bindValue(1, datos.at(2));
    consulta.bindValue(2, datos.at(3));
    consulta.bindValue(3, datos.at(4));
    consulta.bindValue(4, datos.at(5));
    consulta.bindValue(5, datos.at(0));
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::borrarUsusario(QSqlDatabase db, int dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM usuarios WHERE id = ?");
    consulta.bindValue(0, dato);
    if (!consulta.exec()) {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
        db.rollback();
        //QSqlDatabase::database().rollback();
        return false;

    } else {
        db.commit(); //QSqlDatabase::database().commit();
        return true;
    }
}

bool baseDatos::borrarArticulo(QSqlDatabase db, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM articulos WHERE cod LIKE ?");
    consulta.bindValue(0, dato);
    if (!consulta.exec()) {
        return false;
    } else {
        db.commit();
        return true;
    }
}

bool baseDatos::modificarFotoArticulo(QString foto, QString dato)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET foto=? WHERE cod LIKE ?");
    consulta.bindValue(0, foto);
    consulta.bindValue(1, dato);
    if (consulta.exec()) {
        return true;
    } else {
        qDebug() << consulta.lastError().text();
        return false;
    }
}

QSqlQuery baseDatos::ventasClientes(QString nombreConexion, QDate fechaI, QDate fechaF)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("SELECT cliente , sum(total) FROM tickets WHERE fecha BETWEEN '"
                  + fechaI.toString("yyyy-MM-01") + "' and '"
                  + fechaF.toString("yyyy-MM-" + QString::number(fechaF.daysInMonth()))
                  + "' GROUP BY cliente");
    qDebug() << fechaI.toString("yyyy-MM-01");
    //consulta.bindValue(0,fechaI.toString("yyyy-MM-01"));
    qDebug() << fechaF.toString("yyyy-MM-" + QString::number(fechaF.daysInMonth()));
    //consulta.bindValue(1,fechaF.toString("yyyy-MM-"+QString::number(fechaF.daysInMonth())));
    //if (consulta.exec()) {
    qDebug() << consulta.numRowsAffected();
    return consulta;
    //}
    //qDebug() << consulta.lastError();
    //return consulta;
}

double baseDatos::valeCliente(QString nombreConexion, QString idCLiente)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("SELECT cantidad FROM vales WHERE idCLiente ='" + idCLiente + "' AND estado = 1");
    qDebug() << consulta.lastError();
    if (consulta.numRowsAffected() > 0) {
        consulta.first();
        return consulta.value(0).toDouble();
    }
    return 0;
}

bool baseDatos::caducarVales(QString nombreConexion)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("UPDATE vales SET estado = 0 WHERE estado = 1");
    if (consulta.isValid()) {
        return true;
    }
    return false;
}

int baseDatos::idVale(QString nombreConexion, QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("SELECT idVales FROM vales WHERE idCliente='" + idCliente + "' AND estado = 1");
    if (consulta.numRowsAffected() > 0) {
        consulta.first();
        return consulta.value(0).toInt();
    }
    return 0;
}

bool baseDatos::usarVale(QString nombreConexion, int idVale)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.prepare("UPDATE vales SET estado = 2 WHERE idvales = ?");
    consulta.bindValue(0, idVale);
    if (consulta.exec()) {
        return true;
    }
    return false;
}

bool baseDatos::valesPendientesMarcar(QString nombreConexion, QString tienda, int idVale)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.prepare("INSERT INTO valesPendientesMarcar (idVale , tienda) VALUES (?,?)");
    consulta.bindValue(0, idVale);
    consulta.bindValue(1, tienda);
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError();
    return false;
}

bool baseDatos::hayValesPendientesMarcar(QString nombreConexion)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("SELECT * FROM valesPendientesMarcar");
    if (consulta.numRowsAffected() > 0) {
        return true;
    }
    return false;
}

QSqlQuery baseDatos::valesPendientes(QString nombreConexion)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.exec("SELECT * FROM valesPendientesMarcar");
    return consulta;
}

bool baseDatos::borrarValePendiente(QString nombreConexion, int vale)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.prepare("DELETE FROM valesPendientesMarcar WHERE idVale LIKE ?");
    consulta.bindValue(0, vale);
    if (!consulta.exec()) {
        return false;
    }
    return true;
}

QSqlQuery baseDatos::tickesPorCLiente(QString nombreConexion,
                                      QString fechaI,
                                      QString fechaF,
                                      QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));

    if (consulta.exec("SELECT * FROM tickets WHERE cliente = '" + idCliente
                      + "' AND fecha BETWEEN '" + fechaI + "' AND '" + fechaF + "'")) {
        qDebug() << consulta.lastQuery() << consulta.boundValue(0).toString()
                 << consulta.boundValue(1).toString() << consulta.boundValue(2).toString();
        consulta.first();
        return consulta;
    }
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::productosPorClienteCantidad(QString nombreConexion, QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    if (!consulta.exec("SELECT cod , descripcion , SUM(cantidad) FROM lineasticket JOIN tickets ON "
                       "lineasticket.nticket = tickets.ticket "
                       "AND tickets.cliente = '"
                       + idCliente + "' GROUP BY cod ORDER BY SUM(cantidad) DESC ")) {
        qDebug() << consulta.lastError();
    }
    consulta.first();
    qDebug() << consulta.numRowsAffected();
    return consulta;
}

QSqlQuery baseDatos::productosPorClienteFecha(QString nombreConexion, QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    if (!consulta.exec("SELECT cod , descripcion , cantidad , lineasticket.fecha FROM lineasticket "
                       "JOIN tickets ON lineasticket.nticket = tickets.ticket"
                       " AND tickets.cliente = '"
                       + idCliente + "' ORDER BY tickets.fecha DESC  ")) {
        qDebug() << consulta.lastError();
    }
    consulta.first();
    qDebug() << consulta.numRowsAffected();
    return consulta;
}

QString baseDatos::nombreFamilia(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM familias WHERE id LIKE ?");
    consulta.bindValue(0, id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();
    }
    return "Sin familia asignada";
}

QString baseDatos::nombreFabricante(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM fabricantes WHERE id LIKE ?");
    consulta.bindValue(0, id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();
    }
    return "Sin asignar";
}

QString baseDatos::nombreUsusario(QString id, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT * FROM usuarios WHERE id LIKE ?");
    consulta.bindValue(0, id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();
    }
    return "Sin asignar";
}

QString baseDatos::nombreCliente(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
    consulta.bindValue(0, id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString() + " " + consulta.value(2).toString();
    }
    return "Sin asignar";
}

QString baseDatos::etiquetaCliente(QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
    consulta.bindValue(0, idCliente);
    consulta.exec();
    if (consulta.first() == true) {
        QString cliente = "";
        cliente += consulta.value(1).toString() + " " + consulta.value(2).toString() + "<br>";
        cliente += consulta.value(3).toString() + "<br>";
        cliente += consulta.value(4).toString() + "  " + consulta.value(5).toString() + "<br>";
        cliente += consulta.value(6).toString() + "<br>";
        cliente += "N.I.F: " + consulta.value(7).toString();
        return cliente;
    }
    return "Sin asignar";
}

QString baseDatos::nombreFormaPago(QString id, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT * FROM fpago WHERE id LIKE ?");
    consulta.bindValue(0, id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();
    }
    return "Sin forma de pago asignada";
}

QString baseDatos::idFormaPago(QString fpago, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT id FROM fpago WHERE tipo LIKE ?");
    consulta.bindValue(0, fpago);
    if (consulta.exec() == true) {
        consulta.first();
        return consulta.value(0).toString();
    }
    return nullptr;
}

bool baseDatos::insertarEtiqueta(QString base, QString etiqueta)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO etiquetas (cod) VALUES (?)");
    consulta.bindValue(0, etiqueta);
    if (consulta.exec() == true) {
        qDebug() << "Codigo insertado con exito";
        return true;
    }
    qDebug() << consulta.lastError();
    return false;
}

bool baseDatos::modificarTienda(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare(
        "UPDATE tiendas SET nombre = ? , direccion = ? , ciudad = ? , telefono = ? , whatsapp = ? "
        ", email = ? , ip = ? , usuario = ? , password = ? , master = ? , local = ? WHERE id = ?");
    consulta.bindValue(0, datos.at(1));
    consulta.bindValue(1, datos.at(2));
    consulta.bindValue(2, datos.at(3));
    consulta.bindValue(3, datos.at(4));
    consulta.bindValue(4, datos.at(5));
    consulta.bindValue(5, datos.at(6));
    consulta.bindValue(6, datos.at(7));
    consulta.bindValue(7, datos.at(8));
    consulta.bindValue(8, datos.at(9));
    consulta.bindValue(9, datos.at(10));
    consulta.bindValue(10, datos.at(11));
    consulta.bindValue(11, datos.at(0));
    if (consulta.exec()) {
        return true;
    }
    return false;
}

bool baseDatos::borrarTienda(QString dato)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("DELETE FROM tiendas where id = ?");
    consulta.bindValue(0, dato);
    if (consulta.exec()) {
        return true;
    }
    return false;
}

bool baseDatos::crearTienda(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare(
        "INSERT INTO tiendas (id, nombre, direccion, ciudad, telefono, whatsapp, email, ip, "
        "usuario, password, master) VALUES (? , ? , ? , ? , ? , ? , ? , ? , ? ,?,?)");
    qDebug() << datos.length();
    for (int i = 0; i < datos.length(); i++) {
        consulta.bindValue(i, datos.at(i));
        qDebug() << i;
        //qDebug() << datos.at(i);
    }
    if (consulta.exec()) {
        return true;
    }
    return false;
}

QSqlQuery baseDatos::tiendas(QSqlDatabase db)
{
    QSqlQuery consulta(db);
    if (!consulta.exec("SELECT * FROM tiendas WHERE local != '1'")) {
        qDebug() << consulta.lastError();
    }
    return consulta;
}

int baseDatos::idTiendaDesdeNombre(QSqlDatabase db, QString nombreTienda)
{
    QSqlQuery consulta(db);
    if (!consulta.exec("SELECT id FROM tiendas WHERE nombre = '" + nombreTienda + "'")) {
        qDebug() << consulta.lastError();
    }
    consulta.first();
    return consulta.value(0).toInt();
}

QString baseDatos::nombreProveedor(QString id, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT nombre FROM proveedores WHERE idProveedor LIKE ?");
    consulta.bindValue(0, id);
    if (consulta.exec() == true) {
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

QString baseDatos::idProveedor(QString nombre, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT idProveedor FROM proveedores WHERE nombre LIKE ?");
    consulta.bindValue(0, nombre);
    if (consulta.exec() == true) {
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

bool baseDatos::modificarProveedor(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    qDebug() << datos;
    consulta.prepare(
        "UPDATE proveedores SET nombre = ?, nif = ? , direccion = ?, cp = ?, localidad= ?, "
        "provincia = ?, representante = ?, telefonor = ?, mailr = ?, telefono = ?, mail = ?, "
        "descuento = ?, formapago = ?, notas = ? WHERE idProveedor = ?");
    //consulta.bindValue(0,datos.at(0).toInt());
    consulta.bindValue(0, datos.at(1));
    consulta.bindValue(1, datos.at(2));
    consulta.bindValue(2, datos.at(3));
    consulta.bindValue(3, datos.at(4));
    consulta.bindValue(4, datos.at(5));
    consulta.bindValue(5, datos.at(6));
    consulta.bindValue(6, datos.at(7));
    consulta.bindValue(7, datos.at(8));
    consulta.bindValue(8, datos.at(9));
    consulta.bindValue(9, datos.at(10));
    consulta.bindValue(10, datos.at(11));
    consulta.bindValue(11, datos.at(12).toDouble());
    consulta.bindValue(12, datos.at(14).toInt());
    consulta.bindValue(13, datos.at(15));
    consulta.bindValue(14, dato.toInt());
    if (!consulta.exec()) {
        db.rollback();
        QMessageBox msgBox;
        msgBox.setText("Error base de datos");
        msgBox.setInformativeText(consulta.lastError().text());
        msgBox.exec();
        qDebug() << consulta.lastError();
        return false;
    } else {
        db.commit();
        qDebug() << "Articulo insertado";
        return true;
    }
}

bool baseDatos::borrarProveedor(QSqlDatabase db, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM proveedores WHERE idProveedor LIKE ?");
    consulta.bindValue(0, dato);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError();

        return false;
    } else {
        db.commit();
        return true;
    }
}

QString baseDatos::descuentoProveedor(QString proveedor)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT descuento FROM proveedores WHERE nombre LIKE ?");
    consulta.bindValue(0, proveedor);
    if (consulta.exec() == true) {
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

QStringList baseDatos::listadoProveedores()
{
    QStringList proveedores;
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT nombre FROM proveedores ORDER BY nombre ASC");
    while (consulta.next()) {
        proveedores << consulta.value("nombre").toString();
    }
    return proveedores;
}

QString baseDatos::codigoParaNuevoProveedor()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT max(idProveedor) FROM proveedores");
    consulta.first();
    int id = consulta.value(0).toInt() + 1;
    return QString::number(id);
}

QString baseDatos::codigoDesdeAux(QString base, QString aux)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT cod FROM codaux WHERE aux = '" + aux + "'");
    consulta.first();
    return consulta.value(0).toString();
}

QSqlQuery baseDatos::datosTicket(QString base, QString nTicket)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT * FROM tickets WHERE ticket = '" + nTicket + "'");
    consulta.first();
    return consulta;
}

QSqlQuery baseDatos::consultarLineasTicket(QString base, QString nTicket)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT * FROM lineasticket WHERE nticket = '" + nTicket + "'");
    return consulta;
}

double baseDatos::obtenerNumeroUltimoTicket(QSqlDatabase db)
{
    QSqlQuery consulta(db);
    consulta.exec("SELECT max(ticket) FROM tickets");
    consulta.first();
    return consulta.value(0).toDouble();
}

QSqlQuery baseDatos::tcketsPendientes(QSqlDatabase db)
{
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM ticket_tmp");
    // consulta.first();

    return consulta;
}

int baseDatos::maxTicketPendiente(QSqlDatabase db)
{
    QSqlQuery consulta(db);

    if (consulta.exec("SELECT max(orden) FROM ticket_tmp")) {
        consulta.first();
        if (consulta.numRowsAffected() == 0) {
            return 0;
        }
    }
    return consulta.value(0).toInt();
}

bool baseDatos::nuevoTicketTmp(int orden, int cliente, int vendedor)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO ticket_tmp VALUES (?,?,?)");
    consulta.bindValue(0, orden);
    consulta.bindValue(1, cliente);
    consulta.bindValue(2, vendedor);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError().text();
        return false;
    } else {
        return true;
    }
}

bool baseDatos::grabarTicket(QString base , QString serie, QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO " + serie + " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
    //consulta.bindValue(0,serie);
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::grabarLineaTicket(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO lineasticket VALUES (NULL,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text() << "en grabarlineaticket";
    return false;
}

bool baseDatos::borrarTicketTmp(int ticket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("DELETE FROM ticket_tmp WHERE orden = ?");
    consulta.bindValue(0, ticket);
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::modificarCliente(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare(
        "UPDATE clientes SET idCliente = ? , nombre = ? , apellidos = ? , direccion = ? , cp = ? , "
        "localidad = ? , provincia = ? , nif = ? , telefono = ? , telefono2 = ? , mail = ? , "
        "descuento = ? , fechaAlta = ? , notas = ? WHERE idCliente = ?");
    consulta.bindValue(0, datos.at(0).toInt());
    consulta.bindValue(1, datos.at(1));
    consulta.bindValue(2, datos.at(2));
    consulta.bindValue(3, datos.at(3));
    consulta.bindValue(4, datos.at(4));
    consulta.bindValue(5, datos.at(5));
    consulta.bindValue(6, datos.at(6));
    consulta.bindValue(7, datos.at(7));
    consulta.bindValue(8, datos.at(8));
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11).toFloat());
    consulta.bindValue(12, datos.at(12));
    consulta.bindValue(13, datos.at(13));
    consulta.bindValue(14, dato.toInt());
    QString texto
        = QString("UPDATE clientes SET idCliente = '%1' , nombre = '%2' , apellidos = '%3' , "
                  "direccion = '%4' , cp = '%5' , "
                  "localidad = '%6' , provincia = '%7' , nif = '%8' , telefono = '%9' , telefono2 "
                  "= '%10' , mail = '%11' , descuento = '%12' , fechaAlta = '%13' , notas = '%14'"
                  " WHERE idCliente = '%15'")
              .arg(datos.at(0))
              .arg(datos.at(1))
              .arg(datos.at(2))
              .arg(datos.at(3))
              .arg(datos.at(4))
              .arg(datos.at(5))
              .arg(datos.at(6))
              .arg(datos.at(7))
              .arg(datos.at(8))
              .arg(datos.at(9))
              .arg(datos.at(10))
              .arg(datos.at(11))
              .arg(datos.at(12))
              .arg(datos.at(13))
              .arg(dato);
    qDebug() << texto;
    if (!consulta.exec()) {
        db.rollback();
        return false;
    } else {
        db.commit();
        return true;
    }
}

bool baseDatos::borrarCliente(QSqlDatabase db, int idCliente)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM clientes WHERE idCliente = ?");
    consulta.bindValue(0, idCliente);
    if (!consulta.exec()) {
        return false;
    } else {
        db.commit();
        return true;
    }
}

bool baseDatos::crearCliente(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO clientes VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    consulta.bindValue(0, datos.at(0).toInt());
    consulta.bindValue(1, datos.at(1));
    consulta.bindValue(2, datos.at(2));
    consulta.bindValue(3, datos.at(3));
    consulta.bindValue(4, datos.at(4));
    consulta.bindValue(5, datos.at(5));
    consulta.bindValue(6, datos.at(6));
    consulta.bindValue(7, datos.at(7));
    consulta.bindValue(8, datos.at(8));
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11).toFloat());
    consulta.bindValue(12, datos.at(12));
    consulta.bindValue(13, datos.at(13));
    if (!consulta.exec()) {
        db.rollback();
        qDebug() << consulta.lastError();
        return false;
    } else {
        db.commit();
        qDebug() << consulta.executedQuery();

        qDebug() << "Articulo insertado";
        return true;
    }
}

double baseDatos::descuentoCliente(QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT descuento FROM clientes WHERE idCliente = '" + idCliente + "'");
    consulta.first();
    if (consulta.isValid()) {
        return consulta.record().value("descuento").toDouble();
    }
    return 0;
}

bool baseDatos::existeDatoEnTabla(QSqlDatabase db, QString tabla, QString columna, QString dato)
{
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM " + tabla + " WHERE " + columna + " LIKE '" + dato + "'");
    if (consulta.numRowsAffected() > 0) {
        return true;
    } else {
        return false;
    }
}

void baseDatos::insertarEnTabla(QSqlDatabase db, QString tabla, QStringList datos)
{
    QSqlQuery consulta(db);
    QString cadenaDatos;
    for (int i = 0; i < datos.length() - 1; ++i) {
        cadenaDatos += "?,";
    }
    cadenaDatos += "?";
    consulta.prepare("INSERT INTO " + tabla + " VALUES(NULL," + cadenaDatos + ")");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec()) {
        return;
    }
    qDebug() << consulta.lastError();
}

void baseDatos::vaciarTabla(QString tabla, QSqlDatabase db)
{
    QSqlQuery consulta(db);
    consulta.exec("TRUNCATE " + tabla);
}

bool baseDatos::crearProveedor(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO proveedores VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    consulta.bindValue(0, datos.at(0).toInt());
    consulta.bindValue(1, datos.at(1));
    consulta.bindValue(2, datos.at(2));
    consulta.bindValue(3, datos.at(3));
    consulta.bindValue(4, datos.at(4));
    consulta.bindValue(5, datos.at(5));
    consulta.bindValue(6, datos.at(6));
    consulta.bindValue(7, datos.at(7));
    consulta.bindValue(8, datos.at(8));
    consulta.bindValue(9, datos.at(9));
    consulta.bindValue(10, datos.at(10));
    consulta.bindValue(11, datos.at(11));
    consulta.bindValue(12, datos.at(12).toFloat());
    consulta.bindValue(13, datos.at(13));
    consulta.bindValue(14, datos.at(14).toInt());
    consulta.bindValue(15, datos.at(15));
    if (!consulta.exec()) {
        db.rollback();
        QMessageBox msgBox;
        msgBox.setText("Error base de datos");
        msgBox.setInformativeText(consulta.lastError().text());
        msgBox.exec();
        qDebug() << consulta.lastError();
        return false;
    } else {
        db.commit();
        qDebug() << "Articulo insertado";
        return true;
    }
}

double baseDatos::sumarColumna(
    QString base, QString tabla, QString campo, QString campoCondicion, QString condicion)
{
    if (campoCondicion.isNull())
        campoCondicion = campo;
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(" + campo + ") FROM " + tabla + " WHERE " + campoCondicion + " = '"
                  + condicion + "'");
    qDebug() << consulta.lastError().text();
    consulta.first();
    return consulta.value(0).toDouble();
}

int baseDatos::contarLineas(QString tabla, QString base, QString campoCondicion, QString condicion)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT * FROM " + tabla + " WHERE " + campoCondicion + " = '" + condicion + "'");
    qDebug() << consulta.lastError().text();
    consulta.first();
    return consulta.numRowsAffected();
}

bool baseDatos::insertarES(QStringList datos, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO entradasSalidas VALUES (NULL,?,?,?,?,?)");
    consulta.bindValue(0, datos.at(0));
    consulta.bindValue(1, datos.at(1));
    consulta.bindValue(2, datos.at(2));
    consulta.bindValue(3, datos.at(3));
    consulta.bindValue(4, datos.at(4));
    if (consulta.exec())
        return true;
    return false;
}

QStringList baseDatos::recuperarConfigTicket()
{
    QStringList configTicket;
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
    if (!consulta.exec("SELECT * FROM configTicket")) {
        qDebug() << consulta.lastError();
        return configTicket;
    }
    consulta.first();
    configTicket << consulta.value("cabecera").toString();
    configTicket << consulta.value("pie").toString();
    configTicket << consulta.value("promocion").toString();
    configTicket << consulta.value("ruta").toString();
    configTicket << consulta.value("codApertura").toString();
    configTicket << consulta.value("codCorte").toString();
    return configTicket;
}

bool baseDatos::ticketPromo(QString base)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    if (!consulta.exec("SELECT boolPromocion FROM tienda.configTicket"))
        return false;
    consulta.first();
    return consulta.value("boolPromocion").toBool();
}

bool baseDatos::grabarConfiguracionTicket(QStringList configTicket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configTicket SET cabecera = ? , pie = ? , promocion = ? , "
                     "boolPromocion = ? , ruta = ? , codApertura = ? , codCorte = ? WHERE id = 1");
    consulta.bindValue(0, configTicket.at(0));
    consulta.bindValue(1, configTicket.at(1));
    consulta.bindValue(2, configTicket.at(2));
    //    if (configTicket.at(3) == "0") {
    //        consulta.bindValue(3,"FALSE");
    //    } else {consulta.bindValue(3,"TRUE");
    //    }
    consulta.bindValue(3, configTicket.at(3));
    consulta.bindValue(4, configTicket.at(4));
    consulta.bindValue(5, configTicket.at(5));
    consulta.bindValue(6, configTicket.at(6));
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::crearPedido(QString proveedor, QString nPedido, QString fecha, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare(
        "INSERT INTO albaranes_tmp (idProveedor , npedido, fechaPedido) VALUES (?,?,?)");
    consulta.bindValue(0, proveedor);
    consulta.bindValue(1, nPedido);
    consulta.bindValue(2, fecha);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError();
        return false;
    } else {
        qDebug() << "Articulo insertado";
        return true;
    }
}

bool baseDatos::borrarPedido(QString base, QString numeroPedido)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec(QString("SELECT id FROM albaranes_tmp WHERE id = '%1'").arg(numeroPedido));
    consulta.first();
    if (consulta.numRowsAffected() == 1) {
        consulta.exec(
            QString("DELETE FROM lineaspedido_tmp WHERE idPedido = '%1'").arg(numeroPedido));
        consulta.exec(QString("DELETE FROM albaranes_tmp WHERE id = '%1'").arg(numeroPedido));

        return true;
    } else {
        //Buscar los pedidos que coincidan y mostrarlos
        return false;
    }
}

bool baseDatos::grabarLineaPedido(QString base, QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO lineaspedido_tmp VALUES (NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}
bool baseDatos::modificarLineaPedido(QString base, QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("UPDATE lineaspedido_tmp SET idPedido = ? ,"
                     "cod = ? ,descripcion = ? , cantidad = ? , bonificacion = ? , lote = ? , "
                     "fc = ? , costo = ? , descuento1 = ? ,base = ? , tipoIva = ? ,totalbase = ? ,"
                     "iva= ? , re = ? , pvp = ? WHERE id= ? ");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

QStringList baseDatos::listadoPrestamistas()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    QStringList prestamistas;
    consulta.exec("SELECT nombre FROM prestamistas");
    while (consulta.next()) {
        prestamistas << consulta.value("nombre").toString();
    }
    return prestamistas;
}

QStringList baseDatos::datosFactura(QSqlDatabase db, QString nFactura)
{
    QStringList datos;
    datos.clear();
    QSqlQuery consulta(db);
    consulta.prepare("SELECT * FROM facturas WHERE nFactura = ?");
    consulta.bindValue(0, nFactura);
    if (!consulta.exec()) {
        datos.append("ERROR");
        return datos;
    }
    consulta.first();
    QSqlRecord resultado = consulta.record();
    for (int i = 1; i < resultado.count(); i++) {
        datos << resultado.value(i).toString();
    }
    return datos;
}

float baseDatos::sumarIvasPedido(QString base, QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(iva) FROM lineaspedido_tmp WHERE idPedido = '" + idPedido
                  + "' AND tipoIva = '" + tipoIva + "'");
    consulta.first();
    return consulta.value(0).toFloat();
}

float baseDatos::sumarRePedido(QString base, QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(re) FROM lineaspedido_tmp WHERE idPedido = '" + idPedido
                  + "' AND tipoIva = '" + tipoIva + "'");
    consulta.first();
    return consulta.value(0).toFloat();
}

float baseDatos::sumarBasesPedido(QString base, QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(totalbase) FROM lineaspedido_tmp WHERE idPedido = '" + idPedido
                  + "' AND tipoIva = '" + tipoIva + "'");
    consulta.first();
    return consulta.value(0).toFloat();
}

bool baseDatos::borrarLineaPedido(QString base, QString idLinea)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    if (consulta.exec("DELETE FROM `lineaspedido_tmp` WHERE `lineaspedido_tmp`.`id` = '"
                      + idLinea + "'")) {
        qDebug() << "CORRECTO" << consulta.lastError();

        return true;
    }
    qDebug() << "ERROR" << consulta.lastError();

    return false;
}

bool baseDatos::contabilizarPedido(QString base, QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare(
        "INSERT INTO pedidos VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::grabarFactura(QString base , QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    qDebug() << datos;
    consulta.prepare("INSERT INTO facturas VALUES(NULL,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec()){
        qDebug() << "GRABANDO FACTURA";
        return true;
    }
    qDebug() << " ERROR GRABANDO FACTURA" << consulta.lastError().text();
    return false;
}

bool baseDatos::grabarAlbaran(QString base , QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO albaranes VALUES(NULL,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::borrarAlbaranTmp(QString idAlbaran)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("DELETE FROM albaranes_tmp WHERE id = ?");
    consulta.bindValue(0, idAlbaran);
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::pasarLineaPedidoAHistorico(QString base, QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO lineaspedido VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec())
        return true;
    qDebug() << consulta.lastError().text();
    return false;
}

QSqlQuery baseDatos::ventas(QString fecha, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(total) , fpago FROM tickets WHERE fecha = '" + fecha
                  + "' group by fecha , fpago");
    return consulta;
}

QSqlQuery baseDatos::ventasPorUsusario(QString fecha, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(total) , usuario FROM tickets WHERE fecha = '" + fecha
                  + "' group by fecha , usuario");
    return consulta;
}

QSqlQuery baseDatos::ventasDesdeUltimoArqueo(QString fechaI,
                                             QString horaI,
                                             QString tabla,
                                             QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(total) , fpago FROM " + tabla
                  + " WHERE concat_ws('/',fecha , hora) >= '" + fechaI + "/" + horaI
                  + "' group by fpago");
    consulta.first();
    return consulta;
}
QSqlQuery baseDatos::recuperarDatosUltimoArqueo(QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT * FROM arqueos");
    return consulta;
}

bool baseDatos::grabarArqueo(QStringList datos, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO arqueos VALUES (NULL,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); i++) {
        consulta.bindValue(i, datos.at(i));
    }
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

QSqlQuery baseDatos::ventasEntreFechas(QString fechaI, QString FechaF, QString tabla, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT fecha , SUM(total) FROM " + tabla
                     + " WHERE fecha >= ? AND fecha <= ? GROUP BY fecha");
    consulta.bindValue(0, fechaI);
    consulta.bindValue(1, FechaF);
    consulta.exec();
    return consulta;
}

int baseDatos::nTarjetasDesdeUltimoArqueo(QString fechaI, QString horaI, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT count(total) FROM tickets WHERE concat_ws('/',fecha , hora) >= '" + fechaI
                  + "/" + horaI + "' group by fpago");
    consulta.first();
    return consulta.value(0).toInt();
}

QSqlQuery baseDatos::devolverTablaCompleta(QString base , QString nombreTabla)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM " + nombreTabla);
    return consulta;
}

QSqlQuery baseDatos::ejecutarSentencia(QString sentencia, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec(sentencia);
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::ejecutarSentencia(QString sentencia, QSqlDatabase db)
{
    QSqlQuery consulta(db);
    consulta.exec(sentencia);
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::recuperarPedidos(QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT * FROM albaranes_tmp");
    if (!consulta.isValid()) {
        qDebug() << consulta.lastError().text();
    }
    return consulta;
}
double baseDatos::ESdesdeFecha(QString fecha, QString hora, QString base)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT sum(cantidad) FROM entradasSalidas WHERE concat_ws('/',fecha,hora) >= '"
                  + fecha + "/" + hora + "'");
    consulta.first();
    return consulta.value(0).toDouble();
}

QString baseDatos::idLote(QString base, QString cod, QString lote, QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT id FROM lotes WHERE ean = '" + cod + "' AND lote = '" + lote
                  + "' AND fecha = '" + fecha + "'");
    consulta.first();
    if (consulta.isValid())
        return consulta.value(0).toString();
    return "0";
}

int baseDatos::unidadesLote(QString base, QString idLote)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT cantidad FROM lotes WHERE id ='" + idLote + "'");
    consulta.first();
    if (consulta.isValid()) {
        return consulta.value(0).toInt();
    }
    return 0;
}

bool baseDatos::borrarLotesArticulo(QString nombreConexion, QString codigo)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
    consulta.prepare("DELETE * FROM lotes WHERE ean = ?");
    consulta.bindValue(0, codigo);
    if (consulta.exec()) {
        return true;
    }
    qDebug() << consulta.lastError();
    return false;
}

void baseDatos::aumentarLote(QString base, QString idLote, int uds)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("UPDATE lotes SET cantidad = cantidad + " + QString::number(uds) + " WHERE id = '"
                  + idLote + "'");
    if (consulta.numRowsAffected() == 1) {
        qDebug() << "Aumentar lote";

    } else {
        qDebug() << consulta.lastError();
    }
}

void baseDatos::disminuirLote(QString cod, QString fecha, int uds)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '" + cod + "' AND fecha = '" + fecha
                  + "'");
    consulta.exec();
    qDebug() << consulta.numRowsAffected();
    if (consulta.numRowsAffected() == 0) {
        qDebug() << "Pasa por filas = 0";
        QString udsString = QString::number(uds);
        consulta.exec("INSERT INTO lotes (ean , lote ,fecha , cantidad) VALUES ('" + cod + "','','"
                      + fecha + "','-" + udsString + "')");
        qDebug() << consulta.lastError() << "Sin lotes";
        return;
    }

    consulta.first();
    QString id = consulta.record().value(0).toString();
    qDebug() << id;
    if (consulta.record().value(1).toInt() == uds) {
        consulta.exec("DELETE FROM lotes WHERE id LIKE '" + id + "'");
        qDebug() << consulta.lastError() << "== borrando";
        consulta.exec("UPDATE articulos SET stock= (SELECT sum(cantidad) FROM lotes WHERE ean = '"
                      + cod + "') where articulos.cod = '" + cod + "'");
        qDebug() << consulta.lastError() << "==";
        return;
    }
    if (consulta.record().value(1).toInt() < uds) {
        int resto = uds - consulta.record().value(1).toInt();
        consulta.exec("DELETE FROM lotes WHERE id = '" + id + "'");
        qDebug() << consulta.lastError() << "1<";

        consulta.exec("UPDATE articulos SET stock = (SELECT sum(cantidad) FROM lotes WHERE ean = '"
                      + cod + "') where articulos.cod = '" + cod + "'");
        qDebug() << consulta.lastError() << "2<";
        disminuirLote(cod, fecha, resto);
        return;
    }
    int descontarUds = consulta.record().value(1).toInt();
    QString resto = QString::number(descontarUds - uds);
    consulta.exec("UPDATE lotes SET cantidad = " + resto + " WHERE id = '" + id + "'");
    return;
}

void baseDatos::crearLote(QString base, QString ean, QString lote, QString fecha, QString uds)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("INSERT INTO lotes VALUES (NULL,?,?,?,?)");
    consulta.bindValue(0, ean);
    consulta.bindValue(1, lote);
    consulta.bindValue(2, fecha);
    consulta.bindValue(3, uds);
    consulta.exec();
    qDebug() << consulta.lastError().text();
}

QSqlQuery baseDatos::lotesProducto(QString cod, QString nombreConnexion)
{
    //Devuelve los diferentes lotes de un producto
    QSqlQuery consulta(QSqlDatabase::database(nombreConnexion));
    consulta.prepare("SELECT * FROM lotes WHERE ean = ? group by fecha");
    consulta.bindValue(0, cod);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError();
        return consulta;
    }
    return consulta;
}

QString baseDatos::sumarStockArticulo(QString id, QString nombreConnexion)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreConnexion));
    consulta.prepare("SELECT sum(cantidad) FROM lotes WHERE ean = ?");
    consulta.bindValue(0, id);
    if (consulta.exec()) {
        consulta.first();
        return consulta.value(0).toString();
    }
    return "Desconocido";
}

QString baseDatos::ticketCercanoFecha(QString tabla, QString fecha, QString cuando)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    if (cuando == "minimo") {
        consulta.exec("SELECT min(ticket) FROM " + tabla + " WHERE concat_ws('/',fecha,hora) >= '"
                      + fecha + "'");
    } else {
        consulta.exec("SELECT max(ticket) FROM " + tabla + " WHERE concat_ws('/',fecha,hora) <= '"
                      + fecha + "'");
    }
    //consulta.bindValue(0,tabla);
    //consulta.bindValue(1,fecha);
    if (consulta.exec()) {
        consulta.first();
        return consulta.value(0).toString();
    }
    qDebug() << consulta.lastError();
    return 0;
}

QSqlQuery baseDatos::estadisticasVentaProductos(QString nPrimerTicket,
                                                QString nUltimoTicket,
                                                QString nPrimerTicketB,
                                                QString nUltimoTicketB)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT descripcion , sum(cantidad) FROM lineasticket WHERE cast(nticket as "
                  "unsigned) between '"
                  + nPrimerTicket + "' and '" + nUltimoTicket + "' or nticket between '"
                  + nPrimerTicketB + "' and '" + nUltimoTicketB
                  + "' group by cod order by descripcion asc");
    consulta.first();
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::listadoVentaArticulos(QString inicio, QString final, QString nombreDB)
{
    QSqlQuery consulta(QSqlDatabase::database(nombreDB));
    consulta.prepare(
        "SELECT descripcion , sum(cantidad) FROM lineasticket WHERE concat_ws('/',fecha,hora) >= ? "
        "AND concat_ws('/',fecha,hora) <= ? group by cod asc");
    consulta.bindValue(0, inicio);
    consulta.bindValue(1, final);
    consulta.exec();
    //consulta.first();
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::listadoMovimientosEfectivo(QString db , QString inicio, QString final)
{
    QSqlQuery consulta(QSqlDatabase::database(db));
    consulta.prepare(
        "SELECT fecha , hora , cantidad , motivosEntrada.descripcion , entradasSalidas.descripcion "
        "FROM entradasSalidas left join motivosEntrada on entradasSalidas.idTiposRentrada = "
        "motivosEntrada.idtiposEntrada WHERE fecha >= ? AND fecha <= ?");
    consulta.bindValue(0, inicio);
    consulta.bindValue(1, final);
    consulta.exec();
    //consulta.first();
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::listadoCaducados(QString base, QString desde, QString hasta)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("SELECT * FROM caducados WHERE fecha >= ? AND fecha <= ?");
    consulta.bindValue(0, desde);
    consulta.bindValue(1, hasta);
    if (!consulta.exec())
        qDebug() << consulta.lastError();
    return consulta;
}

QString baseDatos::leerConfiguracion()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM configuracion");
    consulta.first();
    return consulta.value("recargoeq").toString();
}

bool baseDatos::GuardarConfiguracion(int datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configuracion SET recargoeq = ? WHERE idconfiguracion = 0");
    consulta.bindValue(0, datos);
    qDebug() << consulta.lastError();
    if (consulta.exec()) {
        return true;
    }
    return false;
}

bool baseDatos::guardarDirectorios(QString base, QStringList directorios)
{
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.prepare("UPDATE directorios SET directorio = ? WHERE id = ?");
    for (int i = 1; i < directorios.count() + 1; i++) {
        consulta.bindValue(0, directorios.at(i - 1));
        consulta.bindValue(1, i);
        if (!consulta.exec()) {
            return false;
        }
    }
    return true;
}

QStringList baseDatos::cargarDirectorios(QString base)
{
    QStringList resultado;
    QSqlQuery consulta(QSqlDatabase::database(base));
    consulta.exec("SELECT directorio FROM directorios");
    consulta.first();
    for (int i = 0; i <= consulta.numRowsAffected(); i++) {
        resultado.append(consulta.record().value(0).toString());
        consulta.next();
    }
    return resultado;
}

QString baseDatos::devolverDirectorio(QString tipo)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT directorio FROM directorios WHERE nombre = ?");
    consulta.bindValue(0, tipo);
    consulta.exec();
    qDebug() << consulta.lastError();
    consulta.first();
    return consulta.record().value(0).toString();
}

QString baseDatos::nombreConexionMaster()
{
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
    consulta.exec("SELECT * FROM tiendas where master = '1'");
    if (consulta.numRowsAffected() < 1) {
        return "";
    }
    consulta.first();
    return consulta.record().value("nombre").toString();
}

QString baseDatos::nombreConexionLocal()
{
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
    consulta.exec("SELECT * FROM tiendas where local = '1'");
    if (consulta.numRowsAffected() < 1) {
        return "DB";
    }
    consulta.first();
    return consulta.record().value("nombre").toString();
}

bool baseDatos::copiaSeguridad(QString base, QString nombre)
{
    qDebug() << "Funcion copia de segurudad activada con el nombre: " + nombre;
    QStringList argumentos;
    argumentos << "--host=" + QSqlDatabase::database(base).hostName()
               << "--port=" + QString::number(QSqlDatabase::database(base).port())
               << "--user=" + QSqlDatabase::database(base).userName()
               << "--password=" + QSqlDatabase::database(base).password()
               << QSqlDatabase::database(base).databaseName();
    qDebug() << argumentos;
    QProcess *process = new QProcess();
    process->setStandardOutputFile(nombre);
    process->start("mysqldump", argumentos);
    if (process->waitForFinished(-1)) {
        qDebug() << "Compretada sin errores el backup de: " << nombre;
        return true;
    } else {
        qDebug() << "Backup error";
        qDebug() << process->errorString();
    }

    return false;
}

QStringList baseDatos::datosTiendaLocal(QString db)
{
    QStringList datos;
    QSqlQuery consulta(QSqlDatabase::database(db));
    consulta.exec("SELECT * FROM tiendas WHERE local = '1'");
    consulta.first();
    if (consulta.numRowsAffected() < 1) {
        QMessageBox msg;
        msg.setText("Error");
        msg.setInformativeText("No se ha podido recuperar los datos de la tienda");
        msg.exec();
        return datos;
    }
    QSqlRecord resultado = consulta.record();
    for (int i = 0; i < resultado.count(); i++) {
        datos << resultado.value(i).toString();
    }
    return datos;
}
