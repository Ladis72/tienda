#include "base_datos.h"
#include <QDebug>
#include <QMessageBox>

baseDatos::baseDatos()
{

}

QSqlDatabase baseDatos::conectar(){



        //db = QSqlDatabase::database();

            db = QSqlDatabase::addDatabase("QMYSQL","DB");
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
                return QSqlDatabase();
                }

return db;

}

bool baseDatos::base_datos_abierta(){


    return false;
}

QSqlQuery baseDatos::usuarios(QSqlDatabase db){

    if(db.isOpen()){
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM usuarios");
    return consulta;
    }

    return QSqlQuery();
}

QSqlQuery baseDatos::fpago(QSqlDatabase db)
{
    if(db.isOpen()){
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM fpago");
    return consulta;
    }
    qDebug() << "Formas de pago no accesibles";
    return QSqlQuery();
}

QSqlDatabase baseDatos::conexion()
{
    if (db.isValid()){
        return db;
    }
    qDebug() << "Sin conexion";

    return QSqlDatabase();
}

QSqlQuery baseDatos::consulta_producto(QSqlDatabase db, QString cod)
{
    if(db.isOpen()){
        QSqlQuery consulta(db);
        consulta.exec("SELECT * FROM articulos WHERE cod LIKE '"+cod+"'");
        return consulta;
    }
    return QSqlQuery();
}

QSqlQuery baseDatos::buscarProducto(QSqlDatabase db, QString tabla, QString nombre)
{
    if (db.isOpen()) {
        QSqlQuery consulta(db);
        consulta.exec("SELECT * FROM "+tabla+" WHERE descripcion LIKE '%"+nombre+"%' ORDER BY descripcion");
        return consulta;

    }
    return QSqlQuery();
}

bool baseDatos::insertarUsuario(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
       consulta.prepare("INSERT INTO usuarios (id , nombre , apellido , direccion , localidad , provincia , cp , tlfn , nif , mail , usuario , clave , notas , rol , foto) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
       consulta.bindValue(0,datos.at(0).toInt());
       qDebug() << datos.at(0);

       consulta.bindValue(1,datos.at(1));
       qDebug() << datos[1];

       consulta.bindValue(2,datos.at(2));
       qDebug() << datos[2];
       consulta.bindValue(3,datos.at(3));
       qDebug() << datos[3];
       consulta.bindValue(4,datos.at(4));
       consulta.bindValue(5,datos.at(5));
       consulta.bindValue(6,datos.at(6));
       consulta.bindValue(7,datos.at(7));
       consulta.bindValue(8,datos.at(8));
       consulta.bindValue(9,datos.at(9));
       consulta.bindValue(10,datos.at(10));
       consulta.bindValue(11,datos.at(11));
       consulta.bindValue(12,datos.at(12));
       consulta.bindValue(13,datos.at(13).toInt());
       consulta.bindValue(14,datos.at(14));
       qDebug() << db.lastError().text();
       qDebug() << consulta.lastError();
       if(!consulta.exec())
           {
           //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
               db.rollback();
           //QSqlDatabase::database().rollback();
               return false;

           } else {
                   db.commit();//QSqlDatabase::database().commit();
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
        consulta.exec("SELECT * FROM "+tabla+" WHERE "+campo+" LIKE '%"+dato+"%'");
        return consulta;

    }
    return QSqlQuery();
}

bool baseDatos::modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("UPDATE usuarios SET id =?, nombre=? , apellido=? , direccion=? , localidad=? , provincia=? , cp=? , tlfn=? , nif=? , mail=? , usuario=? , clave=? , notas=? , rol=? , foto=?  WHERE id=?");
    consulta.bindValue(0,datos.at(0).toInt());
    qDebug() << dato.at(0);

    consulta.bindValue(1,datos.at(1));
    qDebug() << dato[1];

    consulta.bindValue(2,datos.at(2));
    qDebug() << dato[2];
    consulta.bindValue(3,datos.at(3));
    qDebug() << dato[3];
    consulta.bindValue(4,datos.at(4));
    consulta.bindValue(5,datos.at(5));
    consulta.bindValue(6,datos.at(6));
    consulta.bindValue(7,datos.at(7));
    consulta.bindValue(8,datos.at(8));
    consulta.bindValue(9,datos.at(9));
    consulta.bindValue(10,datos.at(10));
    consulta.bindValue(11,datos.at(11));
    consulta.bindValue(12,datos.at(12));
    consulta.bindValue(13,datos.at(13).toInt());
    consulta.bindValue(14,datos.at(14));
    consulta.bindValue(15,dato.toInt());

    qDebug() << db.lastError().text();
    qDebug() << consulta.lastError();
    if(!consulta.exec())
        {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
            db.rollback();
        //QSqlDatabase::database().rollback();
            return false;

        } else {
                db.commit();//QSqlDatabase::database().commit();
                return true;
    }
}

bool baseDatos::modificarFotoUsusario(QString foto, int id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE usuarios SET foto=? WHERE id =?");
    consulta.bindValue(0,foto);
    consulta.bindValue(1,id);
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError();
    return false;
}

bool baseDatos::modificarArticulo(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("UPDATE articulos SET cod = ? , descripcion = ? , pvp = ? , iva = ? , stock = ? , min = ? , max = ? , pendientes_pedido = ? , ultima_venta = ? , ultimo_pedido = ? , familia = ? , precio_compra = ? , fabricante = ? , foto = ? WHERE cod = ?");
    consulta.bindValue(0,datos.at(0));
    consulta.bindValue(1,datos.at(1));
    consulta.bindValue(2,datos.at(2).toDouble());
    consulta.bindValue(3,datos.at(3).toInt());
    consulta.bindValue(4,datos.at(4).toInt());
    consulta.bindValue(5,datos.at(5).toInt());
    consulta.bindValue(6,datos.at(6).toInt());
    consulta.bindValue(7,datos.at(7).toInt());
    consulta.bindValue(8,datos.at(8));
    consulta.bindValue(9,datos.at(9));
    consulta.bindValue(10,datos.at(10).toInt());
    consulta.bindValue(11,datos.at(11).toDouble());
    consulta.bindValue(12,datos.at(12).toInt());
    consulta.bindValue(13,datos.at(13));
    consulta.bindValue(14,dato);

    qDebug() << db.lastError().text();
    qDebug() << consulta.lastError();
    if(!consulta.exec())
        {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
            db.rollback();
        //QSqlDatabase::database().rollback();
            return false;

        } else {
                db.commit();//QSqlDatabase::database().commit();
                return true;
    }
}

bool baseDatos::insertarArticulo(QSqlDatabase db, QStringList datos)
{
    qDebug() << datos;
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO articulos VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    consulta.bindValue(0,datos.at(0));
    consulta.bindValue(1,datos.at(1));
    consulta.bindValue(2,datos.at(2).toDouble());
    consulta.bindValue(3,datos.at(3).toInt());
    consulta.bindValue(4,datos.at(4).toInt());
    consulta.bindValue(5,datos.at(5).toInt());
    consulta.bindValue(6,datos.at(6).toInt());
    consulta.bindValue(7,datos.at(7).toInt());
    consulta.bindValue(8,datos.at(8).toInt());
    consulta.bindValue(9,datos.at(9));
    consulta.bindValue(10,datos.at(10));
    consulta.bindValue(11,datos.at(11).toInt());
    consulta.bindValue(12,datos.at(12).toDouble());
    consulta.bindValue(13,datos.at(13).toInt());
    consulta.bindValue(14,datos.at(14));
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

bool baseDatos::borrarUsusario(QSqlDatabase db, int dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM usuarios WHERE id = ?");
    consulta.bindValue(0,dato);
    if(!consulta.exec())
        {
        //QMessageBox::warning(this,"Advertencia!","No se pudo realizar la operacion solicitada \n"+db.lastError().text());
            db.rollback();
        //QSqlDatabase::database().rollback();
            return false;

        } else {
                db.commit();//QSqlDatabase::database().commit();
                return true;
    }

}

bool baseDatos::borrarArticulo(QSqlDatabase db, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("DELETE FROM articulos WHERE cod LIKE ?");
    consulta.bindValue(0,dato);
    if (!consulta.exec()) {
        return false;
    }else{
        db.commit();
        return true;
    }
}

bool baseDatos::modificarFotoArticulo(QString foto, QString dato)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET foto=? WHERE cod LIKE ?");
    consulta.bindValue(0,foto);
    consulta.bindValue(1,dato);
    if (consulta.exec()) {
        return true;
    }else{
        qDebug() << consulta.lastError().text();
        return false;
    }
}

QString baseDatos::nombreFamilia(QString id)
{

    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM familias WHERE id LIKE ?");
    consulta.bindValue(0,id);
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
    consulta.bindValue(0,id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();

    }
    return "Sin asignar";
}

QString baseDatos::nombreUsusario(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM usuarios WHERE id LIKE ?");
    consulta.bindValue(0,id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();

    }
    return "Sin asignar";
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

bool baseDatos::modificarCliente(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    consulta.prepare("UPDATE clientes SET idCliente = ? , nombre = ? , apellidos = ? , direccion = ? , cp = ? , localidad = ? , provincia = ? , nif = ? , telefono = ? , telefono2 = ? , mail = ? , descuento = ? , fechaAlta = ? , notas = ? WHERE idCliente = ?");
    consulta.bindValue(0,datos.at(0).toInt());
    consulta.bindValue(1,datos.at(1));
    consulta.bindValue(2,datos.at(2));
    consulta.bindValue(3,datos.at(3));
    consulta.bindValue(4,datos.at(4));
    consulta.bindValue(5,datos.at(5));
    consulta.bindValue(6,datos.at(6));
    consulta.bindValue(7,datos.at(7));
    consulta.bindValue(8,datos.at(8));
    consulta.bindValue(9,datos.at(9));
    consulta.bindValue(10,datos.at(10));
    consulta.bindValue(11,datos.at(11).toFloat());
    consulta.bindValue(12,datos.at(12));
    consulta.bindValue(13,datos.at(13));
    consulta.bindValue(14,dato.toInt());

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
    consulta.bindValue(0,idCliente);
    if(!consulta.exec()){
        return false;
    }else{
        db.commit();
        return true;
    }
}

