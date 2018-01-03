#include "base_datos.h"
#include <QDebug>
#include <QMessageBox>

baseDatos::baseDatos()
{

}

bool baseDatos::conectar(QString host , QString puerto , QString baseDatos , QString usuario , QString clave){


    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL","DB");
    db.setHostName(host);
    db.setDatabaseName(baseDatos);
    db.setUserName(usuario);
    db.setPassword(clave);
    db.setPort(puerto.toInt());
        if(!db.open()){
        QMessageBox mensaje;
        mensaje.setText("No se puede continuar. "+db.lastError().text());
        mensaje.setWindowTitle ("Error");
        mensaje.exec();
        return false;
        }

return true;

}

bool baseDatos::guardarDatosConexion(QString host, QString puerto, QString baseDatos, QString usuario, QString clave)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configBase SET servidor =? , puerto =?, baseDatos =? , usuario =? , clave =? WHERE id=1");
    consulta.bindValue(0,host);
    consulta.bindValue(1,puerto.toInt());
    consulta.bindValue(2,baseDatos);
    consulta.bindValue(3,usuario);
    consulta.bindValue(4,clave);
    if (!consulta.exec()) {
        QMessageBox mensaje;
        mensaje.setText("No se han podido guardar los datos"+consulta.lastError().text());
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
        qDebug() << consulta.lastError().text();

        return consulta;

    }
    qDebug() << consulta.lastError().text();

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
    consulta.prepare("UPDATE articulos SET cod = ? , descripcion = ? , pvp = ? , iva = ? , stock = ? , min = ? , max = ? , pendientes_pedido = ? , encargados = ? , ultima_venta = ? , ultimo_pedido = ? , familia = ? , precio_compra = ? , fabricante = ? , foto = ? , notas = ? WHERE cod = ?");
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
    consulta.bindValue(15,datos.at(15));
    consulta.bindValue(16,dato);

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
    consulta.prepare("INSERT INTO articulos VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
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

    consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '"+cod+"' ORDER BY fecha ASC");
    consulta.first();
    if(consulta.isValid()){

    if (consulta.value("cantidad").toInt() > uds) {
        QString registro = consulta.value("id").toString();
        consulta.prepare("UPDATE lotes SET cantidad = cantidad - ? WHERE id = ?");
        consulta.bindValue(0,uds);
        consulta.bindValue(1,registro);
        if(consulta.exec()){
            return true;
        }else{
            qDebug() << consulta.lastError().text();
            return false;
        }
    }else if (consulta.value("cantidad").toInt() <= uds) {
        QString registro = consulta.value("id").toString();
        int resta = uds - consulta.value("cantidad").toInt();
        if (resta == 0) {
            consulta.exec("DELETE FROM lotes WHERE id = '"+registro+"'");
            qDebug() << consulta.lastError().text();
            return true;
        } else {
            consulta.exec("DELETE FROM lotes WHERE id = '"+registro+"'");
            descontarArticulo(cod,resta);
            qDebug() << consulta.lastError().text();
            return true;
        }
    }
    }else {
        qDebug() << "No hay lotes de ese artículo";
        crearLote(cod,"","2000-01-01",QString::number(0-uds));
    }
}


bool baseDatos::actualizarFechaVentaArticulo(QString cod, QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET ultima_venta = ? WHERE cod LIKE ?");
    consulta.bindValue(0,fecha);
    consulta.bindValue(1,cod);
    if(consulta.exec()){
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::actualizarArticulosDesdeCompras(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE articulos SET descripcion = ? , pvp = ? , iva = ? , precio_compra = ? , ultimo_pedido = ?  WHERE cod = ?");
    consulta.bindValue(0,datos.at(1));
    consulta.bindValue(1,datos.at(2));
    consulta.bindValue(2,datos.at(3));
    consulta.bindValue(3,datos.at(4));
    consulta.bindValue(4,datos.at(5));
    consulta.bindValue(5,datos.at(0));
    if(consulta.exec()){
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
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

QString baseDatos::nombreCliente(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
    consulta.bindValue(0,id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString()+" "+consulta.value(2).toString();

    }
    return "Sin asignar";
}

QString baseDatos::etiquetaCliente(QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
    consulta.bindValue(0,idCliente);
    consulta.exec();
    if (consulta.first() == true) {
        QString cliente="";
        cliente += consulta.value(1).toString()+" "+consulta.value(2).toString()+"\n";
        cliente += consulta.value(3).toString()+"\n";
        cliente += consulta.value(4).toString()+"  "+consulta.value(5).toString()+"\n";
        cliente += consulta.value(6).toString()+"\n";
        cliente += "N.I.F: "+consulta.value(7).toString();
        return cliente;

    }
    return "Sin asignar";
}



QString baseDatos::nombreFormaPago(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM fpago WHERE id LIKE ?");
    consulta.bindValue(0,id);
    consulta.exec();
    if (consulta.first() == true) {
        return consulta.value(1).toString();

    }
    return "Sin forma de pago asignada";
}

QString baseDatos::idFormaPago(QString fpago)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT id FROM fpago WHERE tipo LIKE ?");
    consulta.bindValue(0,fpago);
    if(consulta.exec() == true){
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

bool baseDatos::insertarEtiqueta(QString etiqueta)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO etiquetas (cod) VALUES (?)");
    consulta.bindValue(0,etiqueta);
    if (consulta.exec() == true) {
        qDebug() << "Codigo insertado con exito";
        return true;
    }
    qDebug() << consulta.lastError();
    return false;
}

QString baseDatos::nombreProveedor(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT nombre FROM proveedores WHERE idProveedor LIKE ?");
    consulta.bindValue(0,id);
    if(consulta.exec() == true){
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

QString baseDatos::idProveedor(QString nombre)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT idProveedor FROM proveedores WHERE nombre LIKE ?");
    consulta.bindValue(0,nombre);
    if(consulta.exec() == true){
        consulta.first();
        return consulta.value(0).toString();
    }
    return 0;
}

bool baseDatos::modificarProveedor(QSqlDatabase db, QStringList datos, QString dato)
{
    QSqlQuery consulta(db);
    qDebug() << datos;
    consulta.prepare("UPDATE proveedores SET nombre = ?, nif = ? , direccion = ?, cp = ?, localidad= ?, provincia = ?, representante = ?, telefonor = ?, mailr = ?, telefono = ?, mail = ?, descuento = ?, formapago = ?, notas = ? WHERE idProveedor = ?");
    //consulta.bindValue(0,datos.at(0).toInt());
    consulta.bindValue(0,datos.at(1));
    consulta.bindValue(1,datos.at(2));
    consulta.bindValue(2,datos.at(3));
    consulta.bindValue(3,datos.at(4));
    consulta.bindValue(4,datos.at(5));
    consulta.bindValue(5,datos.at(6));
    consulta.bindValue(6,datos.at(7));
    consulta.bindValue(7,datos.at(8));
    consulta.bindValue(8,datos.at(9));
    consulta.bindValue(9,datos.at(10));
    consulta.bindValue(10,datos.at(11));
    consulta.bindValue(11,datos.at(12).toDouble());
    consulta.bindValue(12,datos.at(14).toInt());
    consulta.bindValue(13,datos.at(15));
    consulta.bindValue(14,dato.toInt());
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

QString baseDatos::descuentoProveedor(QString proveedor)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT descuento FROM proveedores WHERE nombre LIKE ?");
    consulta.bindValue(0,proveedor);
    if(consulta.exec() == true){
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
    int id = consulta.value(0).toInt()+1;
    return QString::number(id);
}

QString baseDatos::codigoDesdeAux(QString aux)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT cod FROM codaux WHERE aux = '"+aux+"'");
    consulta.first();
    return consulta.value(0).toString();

}

QSqlQuery baseDatos::datosTicket(QString nTicket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM tickets WHERE ticket = '"+nTicket+"'");
    consulta.first();
    return consulta;
}

QSqlQuery baseDatos::consultarLineasTicket(QString nTicket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM lineasticket WHERE nticket = '"+nTicket+"'");
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
        if(consulta.numRowsAffected() == 0){
            return 0;
        }
    }
        return consulta.value(0).toInt();

}

bool baseDatos::nuevoTicketTmp(int orden, int cliente, int vendedor)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO ticket_tmp VALUES (?,?,?)");
    consulta.bindValue(0,orden);
    consulta.bindValue(1,cliente);
    consulta.bindValue(2,vendedor);
    if(!consulta.exec()){
        qDebug() << consulta.lastError().text();
        return false;
    }else{
        return true;
    }
}

bool baseDatos::grabarTicket(QString serie , QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO "+serie+" VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    //consulta.bindValue(0,serie);
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()){
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
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()){
        return true;
    }
    qDebug() << consulta.lastError().text() << "en grabarlineaticket";
    return false;
}

bool baseDatos::borrarTicketTmp(int ticket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("DELETE FROM ticket_tmp WHERE orden = ?");
    consulta.bindValue(0,ticket);
    if(consulta.exec()){
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
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

bool baseDatos::crearCliente(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO clientes VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
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

double baseDatos::descuentoCliente(QString idCliente)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT descuento FROM clientes WHERE idCliente = '"+idCliente+"'");
    consulta.first();
    if(consulta.isValid()){
        return consulta.record().value("descuento").toDouble();
    }
    return 0;
}

bool baseDatos::existeDatoEnTabla(QSqlDatabase db, QString tabla,QString columna, QString dato)
{
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM "+tabla+" WHERE "+columna+" LIKE '"+dato+"'");
    if(consulta.numRowsAffected() > 0 ){
        return true;
    }else{
        return false;
    }
}

void baseDatos::insertarEnTabla(QSqlDatabase db, QString tabla, QStringList datos)
{
    QSqlQuery consulta(db);
    QString cadenaDatos;
    for (int i = 0; i < datos.length()-1; ++i) {
        cadenaDatos +="?,";
    }
    cadenaDatos +="?";
    consulta.prepare("INSERT INTO "+tabla+" VALUES(NULL,"+cadenaDatos+")");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if (consulta.exec()) {
        return;
    }
    qDebug() << consulta.lastError();
}

void baseDatos::vaciarTabla(QString tabla)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("TRUNCATE "+tabla);
}

bool baseDatos::crearProveedor(QSqlDatabase db, QStringList datos)
{
    QSqlQuery consulta(db);
    consulta.prepare("INSERT INTO proveedores VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
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
    consulta.bindValue(11,datos.at(11));
    consulta.bindValue(12,datos.at(12).toFloat());
    consulta.bindValue(13,datos.at(13));
    consulta.bindValue(14,datos.at(14).toInt());
    consulta.bindValue(15,datos.at(15));
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

double baseDatos::sumarColumna(QString tabla, QString campo, QString campoCondicion, QString condicion)
{
    if(campoCondicion.isNull()) campoCondicion = campo;
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum("+campo+") FROM "+tabla+" WHERE "+campoCondicion+" = '"+condicion+"'");
    qDebug() << consulta.lastError().text();
    consulta.first();
    return consulta.value(0).toDouble();

}

int baseDatos::contarLineas(QString tabla, QString campoCondicion, QString condicion)
{

    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM "+tabla+" WHERE "+campoCondicion+" = '"+condicion+"'");
    qDebug() << consulta.lastError().text();
    consulta.first();
    return consulta.numRowsAffected();
}

bool baseDatos::insertarES(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO entradasSalidas VALUES (NULL,?,?,?,?,?)");
    consulta.bindValue(0,datos.at(0));
    consulta.bindValue(1,datos.at(1));
    consulta.bindValue(2,datos.at(2));
    consulta.bindValue(3,datos.at(3));
    consulta.bindValue(4,datos.at(4));
    if(consulta.exec()) return true;
    return false;
}


QStringList baseDatos::recuperarConfigTicket()
{
    QStringList configTicket;
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    if(!consulta.exec("SELECT * FROM tienda.configTicket")){
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

bool baseDatos::ticketPromo()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    if(!consulta.exec("SELECT boolPromocion FROM tienda.configTicket"))
        return false;
    consulta.first();
    return consulta.value("boolPromocion").toBool();
}

bool baseDatos::grabarConfiguracionTicket(QStringList configTicket)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE configTicket SET cabecera = ? , pie = ? , promocion = ? , boolPromocion = ? , ruta = ? , codApertura = ? , codCorte = ? WHERE id = 1");
    consulta.bindValue(0,configTicket.at(0));
    consulta.bindValue(1,configTicket.at(1));
    consulta.bindValue(2,configTicket.at(2));
//    if (configTicket.at(3) == "0") {
//        consulta.bindValue(3,"FALSE");
//    } else {consulta.bindValue(3,"TRUE");
//    }
    consulta.bindValue(3,configTicket.at(3));
    consulta.bindValue(4,configTicket.at(4));
    consulta.bindValue(5,configTicket.at(5));
    consulta.bindValue(6,configTicket.at(6));
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;

}

bool baseDatos::crearPedido(QString proveedor, QString nPedido, QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO albaranes_tmp (idProveedor , npedido, fechaPedido) VALUES (?,?,?)");
    consulta.bindValue(0,proveedor);
    consulta.bindValue(1,nPedido);
    consulta.bindValue(2,fecha);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError();
        return false;
    } else {
        qDebug() << "Articulo insertado";
        return true;
    }
}

bool baseDatos::borrarPedido(QString numeroPedido)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec(QString("SELECT id FROM albaranes_tmp WHERE id = '%1'").arg(numeroPedido));
    consulta.first();
    if(consulta.numRowsAffected() == 1){
        consulta.exec(QString("DELETE FROM lineaspedido_tmp WHERE idPedido = '%1'").arg(numeroPedido));
        consulta.exec(QString("DELETE FROM albaranes_tmp WHERE id = '%1'").arg(numeroPedido));

        return true;
    }else{
        //Buscar los pedidos que coincidan y mostrarlos
        return false;
    }

}

bool baseDatos::grabarLineaPedido(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO lineaspedido_tmp VALUES (NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;
}
bool baseDatos::modificarLineaPedido(QStringList datos){
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("UPDATE lineaspedido_tmp SET idPedido = ? ,"
                     "cod = ? ,descripcion = ? , cantidad = ? , bonificacion = ? , lote = ? , "
                     "fc = ? , costo = ? , descuento1 = ? ,base = ? , tipoIva = ? ,totalbase = ? ,"
                     "iva= ? , re = ? , pvp = ? WHERE id= ? ");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;
}

float baseDatos::sumarIvasPedido(QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(iva) FROM lineaspedido_tmp WHERE idPedido = '"+idPedido+"' AND tipoIva = '"+tipoIva+"'");
    consulta.first();
    return consulta.value(0).toFloat();
}

float baseDatos::sumarRePedido(QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(re) FROM lineaspedido_tmp WHERE idPedido = '"+idPedido+"' AND tipoIva = '"+tipoIva+"'");
    consulta.first();
    return consulta.value(0).toFloat();
}

float baseDatos::sumarBasesPedido(QString idPedido, QString tipoIva)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(totalbase) FROM lineaspedido_tmp WHERE idPedido = '"+idPedido+"' AND tipoIva = '"+tipoIva+"'");
    consulta.first();
    return consulta.value(0).toFloat();
}

bool baseDatos::borrarLineaPedido(QString idLinea)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    if(consulta.exec("DELETE FROM `tienda`.`lineaspedido_tmp` WHERE `lineaspedido_tmp`.`id` = '"+idLinea+"'")){
        return true;
    }
    return false;
}

bool baseDatos::contabilizarPedido(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO pedidos VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::grabarFactura(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO facturas VALUES(NULL,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;
}

bool baseDatos::borrarAlbaranTmp(QString idAlbaran)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("DELETE FROM albaranes_tmp WHERE id = ?");
    consulta.bindValue(0,idAlbaran);
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;

}

bool baseDatos::pasarLineaPedidoAHistorico(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO lineaspedido VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
    for (int i = 0; i < datos.length(); ++i) {
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()) return true;
    qDebug() << consulta.lastError().text();
    return false;
}

QSqlQuery baseDatos::ventas(QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(total) , fpago FROM tickets WHERE fecha = '"+fecha+"' group by fecha , fpago");
    return consulta;
}

QSqlQuery baseDatos::ventasPorUsusario(QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(total) , usuario FROM tickets WHERE fecha = '"+fecha+"' group by fecha , usuario");
    return consulta;
}


QSqlQuery baseDatos::ventasDesdeUltimoArqueo(QString fechaI, QString horaI, QString tabla)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(total) , fpago FROM "+tabla+" WHERE concat_ws('/',fecha , hora) >= '"+fechaI+"/"+horaI+"' group by fpago");
    consulta.first();
    return consulta;
}
QSqlQuery baseDatos::recuperarDatosUltimoArqueo()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM arqueos");
    return consulta;
}

bool baseDatos::grabarArqueo(QStringList datos)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO arqueos VALUES (NULL,?,?,?,?,?,?,?)");
    for(int i = 0; i < datos.length(); i++){
        consulta.bindValue(i,datos.at(i));
    }
    if(consulta.exec()){
        return true;
    }
    qDebug() << consulta.lastError().text();
    return false;
}

QSqlQuery baseDatos::ventasEntreFechas(QString fechaI, QString FechaF, QString tabla)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT fecha , SUM(total) FROM "+tabla+" WHERE fecha >= ? AND fecha <= ? GROUP BY fecha");
    consulta.bindValue(0,fechaI);
    consulta.bindValue(1,FechaF);
    consulta.exec();
    return consulta;
}

int baseDatos::nTarjetasDesdeUltimoArqueo(QString fechaI , QString horaI)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT count(total) FROM tickets WHERE concat_ws('/',fecha , hora) >= '"+fechaI+"/"+horaI+"' group by fpago");
    consulta.first();
    return consulta.value(0).toInt();
}

QSqlQuery baseDatos::devolverTablaCompleta(QString nombreTabla)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM "+nombreTabla);
    return consulta;
}

QSqlQuery baseDatos::ejecutarSentencia(QString sentencia)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec(sentencia);
    return consulta;
}

QSqlQuery baseDatos::recuperarPedidos()
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT * FROM albaranes_tmp");
    if (consulta.isValid()) {
        return consulta;
    }
    qDebug() << consulta.lastError().text();
    
}
double baseDatos::ESdesdeFecha(QString fecha, QString hora){
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT sum(cantidad) FROM entradasSalidas WHERE concat_ws('/',fecha,hora) >= '"+fecha+"/"+hora+"'");
    consulta.first();
    return consulta.value(0).toDouble();
}

QString baseDatos::idLote(QString cod, QString lote, QString fecha)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT id FROM lotes WHERE ean = '"+cod+"' AND lote = '"+lote+"' AND fecha = '"+fecha+"'");
    consulta.first();
    if(consulta.isValid()) return consulta.value(0).toString();
    return "0";
}

int baseDatos::unidadesLote(QString idLote){
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT cantidad FROM lotes WHERE id ='"+idLote+"'");
    consulta.first();
    if (consulta.isValid()) {
        return consulta.value(0).toInt();
    }
    return 0;
}

void baseDatos::aumentarLote(QString idLote, int uds)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("UPDATE lotes SET cantidad = cantidad + "+QString::number(uds)+" WHERE id = '"+idLote+"'");
    if(consulta.numRowsAffected() == 1) {
        qDebug() << "Aumentar lote";

    }else{
    qDebug() << consulta.lastError();
    }

}

void baseDatos::disminuirLote(QString cod, QString fecha, int uds)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '"+cod+"' AND fecha = '"+fecha+"'");
    consulta.exec();
    qDebug() << consulta.numRowsAffected();
    if(consulta.numRowsAffected() == 0){
        qDebug() << "Pasa por filas = 0";
        return;
    }

    consulta.first();
    QString id = consulta.record().value(0).toString();
    qDebug() << id;
    if (consulta.record().value(1).toInt() <= uds) {
        consulta.exec("DELETE FROM lotes WHERE id = '"+id+"'");
        consulta.exec("UPDATE articulos SET stock= (SELECT sum(cantidad) FROM lotes WHERE ean = '"+cod+"') where articulos.cod = '"+cod+"'");
        consulta.lastError();
        return;
    }
    int descontarUds = consulta.record().value(1).toInt();
    QString resto = QString::number(descontarUds-uds);
    consulta.exec("UPDATE lotes SET cantidad = "+resto+" WHERE id = '"+id+"'");
    return;
}

void baseDatos::crearLote(QString ean, QString lote, QString fecha, QString uds)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("INSERT INTO lotes VALUES (NULL,?,?,?,?)");
    consulta.bindValue(0,ean);
    consulta.bindValue(1,lote);
    consulta.bindValue(2,fecha);
    consulta.bindValue(3,uds);
    consulta.exec();
    qDebug() << consulta.lastError().text();

}

QSqlQuery baseDatos::lotesProducto(QString cod)
{
    //Devuelve los diferentes lotes de un producto
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT * FROM lotes WHERE ean = ? group by fecha");
    consulta.bindValue(0,cod);
    if (!consulta.exec()) {
        qDebug() << consulta.lastError();
        return consulta;
    }
    return consulta;

}

QString baseDatos::sumarStockArticulo(QString id)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT sum(cantidad) FROM lotes WHERE ean = ?");
    consulta.bindValue(0,id);
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
        consulta.exec("SELECT min(ticket) FROM "+tabla+" WHERE concat_ws('/',fecha,hora) >= '"+fecha+"'");
    }else{ consulta.exec("SELECT max(ticket) FROM "+tabla+" WHERE concat_ws('/',fecha,hora) <= '"+fecha+"'");
    }
    //consulta.bindValue(0,tabla);
    //consulta.bindValue(1,fecha);
    if(consulta.exec()){
        consulta.first();
        return consulta.value(0).toString();
    }
    qDebug() << consulta.lastError();
    return 0;
}

QSqlQuery baseDatos::estadisticasVentaProductos(QString nPrimerTicket , QString nUltimoTicket, QString nPrimerTicketB , QString nUltimoTicketB)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.exec("SELECT descripcion , sum(cantidad) FROM lineasticket WHERE cast(nticket as unsigned) between '"+nPrimerTicket+"' and '"+nUltimoTicket+"' or nticket between '"+nPrimerTicketB+"' and '"+nUltimoTicketB+"' group by cod order by descripcion asc");
    consulta.first();
    qDebug() << consulta.lastError();
    return consulta;
}

QSqlQuery baseDatos::listadoVentaArticulos(QString inicio, QString final)
{
    QSqlQuery consulta(QSqlDatabase::database("DB"));
    consulta.prepare("SELECT descripcion , sum(cantidad) FROM lineasticket WHERE concat_ws('/',fecha,hora) >= ? AND concat_ws('/',fecha,hora) <= ? group by descripcion asc");
    consulta.bindValue(0,inicio);
    consulta.bindValue(1,final);
    consulta.exec();
    //consulta.first();
    qDebug() << consulta.lastError();
    return consulta;
}





