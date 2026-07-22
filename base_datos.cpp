#include "base_datos.h"
#include "hashutil.h"
#include "qprocess.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>


baseDatos::baseDatos() {}

bool baseDatos::conectar(QString host, QString puerto, QString baseDatos,
                         QString usuario, QString clave) {
  QSqlDatabase db;
  if (QSqlDatabase::contains("DB")) {
    db = QSqlDatabase::database(conf->getConexionLocal());
  } else {
    db = QSqlDatabase::addDatabase("QMYSQL", "DB");
  }
  db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=3");
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

  return true;
}

bool baseDatos::guardarDatosConexion(QString host, QString puerto,
                                     QString baseDatos, QString usuario,
                                     QString clave) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("UPDATE configBase SET servidor =? , puerto =?, baseDatos "
                   "=? , usuario =? , "
                   "clave =? WHERE id=1");
  consulta.bindValue(0, host);
  consulta.bindValue(1, puerto.toInt());
  consulta.bindValue(2, baseDatos);
  consulta.bindValue(3, usuario);
  consulta.bindValue(4, clave);
  if (!consulta.exec()) {
    QMessageBox mensaje;
    mensaje.setText("No se han podido guardar los datos" +
                    consulta.lastError().text());
    mensaje.setWindowTitle("Error");
    mensaje.exec();
    return false;
  }
  return true;
}

QStringList baseDatos::datosConexion() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
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
bool baseDatos::guardarDatosConexionMaster(QString host, QString puerto,
                                           QString baseDatos, QString usuario,
                                           QString clave) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("UPDATE configMaster SET servidor =? , puerto =?, baseDatos "
                   "=? , usuario =? , "
                   "clave =? WHERE id=1");
  consulta.bindValue(0, host);
  consulta.bindValue(1, puerto.toInt());
  consulta.bindValue(2, baseDatos);
  consulta.bindValue(3, usuario);
  consulta.bindValue(4, clave);
  if (!consulta.exec()) {
    QMessageBox mensaje;
    mensaje.setText("No se han podido guardar los datos" +
                    consulta.lastError().text());
    mensaje.setWindowTitle("Error");
    mensaje.exec();
    return false;
  }
  return true;
}

QStringList baseDatos::datosConexionMaster() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
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

QStringList baseDatos::datosConexionLocal() {
  // Recupera los datos de conexión de la tienda marcada como local.
  // El QStringList devuelto tiene el siguiente orden:
  //   [0] nombre, [1] ip, [2] usuario, [3] password, [4] baseDatos, [5] puerto
  // Primero busca por la bandera local=1; si no, busca por IP de loopback como
  // fallback.
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT nombre , ip , usuario , password , baseDatos , "
                "COALESCE(puerto, 3306) AS puerto FROM tiendas "
                "WHERE local = 1 OR ip IN ('localhost','127.0.0.1','::1') "
                "ORDER BY local DESC LIMIT 1");
  consulta.first();
  QStringList datos;
  datos.clear();
  if (!consulta.isValid()) {
    return datos;
  }

  qDebug() << consulta.size();
  for (int i = 0; i < 6; ++i) {
    datos.append(consulta.value(i).toString());
  }
  return datos;
}
bool baseDatos::base_datos_abierta() { return false; }

QSqlQuery baseDatos::usuarios(QSqlDatabase db) {
  if (db.isOpen()) {
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM usuarios");
    return consulta;
  }

  return QSqlQuery();
}

QSqlQuery baseDatos::fpago(QSqlDatabase db) {
  if (db.isOpen()) {
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM fpago");
    return consulta;
  }
  qDebug() << "Formas de pago no accesibles";
  return QSqlQuery();
}

QSqlDatabase baseDatos::conexion() {
  if (db.isValid()) {
    return db;
  }
  qDebug() << "Sin conexion";

  return QSqlDatabase();
}

QSqlRecord baseDatos::consulta_producto(QString nombreConnexion, QString cod) {
  QSqlRecord registro;
  QSqlDatabase dbQuery = QSqlDatabase::database(nombreConnexion);
  
  if (dbQuery.isOpen()) {
    QSqlQuery consulta(dbQuery);
    consulta.prepare("SELECT * FROM articulos WHERE cod = ?");
    consulta.bindValue(0, cod);
    if (consulta.exec() && consulta.first()) {
      registro = consulta.record();
      
      // Si se usan precios locales, buscar excepción en la conexión "DB" (precios_tienda no se sincroniza)
      if (conf->getUsarPreciosLocales()) {
          QSqlQuery qLocal(QSqlDatabase::database(conf->getConexionLocal()));
          qLocal.prepare("SELECT pvp, precio_venta FROM precios_tienda WHERE cod_articulo = ?");
          qLocal.bindValue(0, cod);
          if (qLocal.exec() && qLocal.next()) {
            registro.setValue("pvp", qLocal.value("pvp"));
            registro.setValue("precio_venta", qLocal.value("precio_venta"));
          }
      }
    }
  }
  return registro;
}

QSqlQuery baseDatos::buscarProducto(QSqlDatabase db, QString tabla,
                                    QString nombre) {
  if (db.isOpen()) {
    QSqlQuery consulta(db);
    // consulta.exec("SELECT * FROM "+tabla+" WHERE descripcion LIKE
    // '%"+nombre+"%' ORDER BY descripcion");
    QString pvpQuery = conf->getUsarPreciosLocales() 
        ? "IF(pt.pvp IS NOT NULL, pt.pvp, articulos.pvp)" 
        : "articulos.pvp";

    QString sql = QString(
        "SELECT articulos.cod, articulos.descripcion, %1 as pvp, articulos.iva, "
        "0 as stock, articulos.min, articulos.max, 0 as pendientes_pedido, "
        "0 as encargados, articulos.ultima_venta, articulos.ultimo_pedido, "
        "articulos.familia, articulos.precio_compra, articulos.fabricante, articulos.foto, "
        "articulos.notas, articulos.formato, articulos.cantformato, "
        "SUM(lotes.cantidad) as stock_total, MIN(lotes.fecha) as fecha_caducidad "
        "FROM articulos "
        "LEFT JOIN lotes ON articulos.cod = lotes.ean "
        "LEFT JOIN precios_tienda pt ON articulos.cod = pt.cod_articulo "
        "WHERE articulos.descripcion LIKE '%%2%' "
        "GROUP BY articulos.cod").arg(pvpQuery).arg(nombre);

    consulta.exec(sql);
    return consulta;
  }
  return QSqlQuery();
}

// Realiza una búsqueda inteligente multitérmino priorizando el campo notas y la descripción del artículo
// Optimizada para alto rendimiento en equipos de bajos recursos (elimina GROUP BY masivo y añade LIMIT)
QSqlQuery baseDatos::buscarPorNotas(QSqlDatabase db, QString texto) {
  if (db.isOpen()) {
    QSqlQuery consulta(db);
    QString pvpQuery = conf->getUsarPreciosLocales() 
        ? "IF(pt.pvp IS NOT NULL, pt.pvp, articulos.pvp)" 
        : "articulos.pvp";

    // Dividir el texto introducido por el usuario en palabras clave separadas por espacios
    QStringList palabras = texto.trimmed().split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
    QStringList condiciones;

    // Generar la cláusula WHERE requiriendo que cada palabra clave coincida en notas, descripción o código
    for (QString palabra : palabras) {
      // Escapar caracteres comilla y barra invertida para la seguridad del SQL
      palabra.replace("'", "''");
      palabra.replace("\\", "\\\\");
      condiciones.append(QString("(articulos.notas LIKE '%%1%' OR articulos.descripcion LIKE '%%1%' OR articulos.cod LIKE '%%1%')").arg(palabra));
    }

    QString clFiltro = condiciones.isEmpty() ? "" : "WHERE " + condiciones.join(" AND ");

    // Optimización SQL: Se reemplaza el LEFT JOIN + GROUP BY masivo por subconsultas directas indexadas sobre la tabla lotes,
    // ejecutadas únicamente sobre las filas filtradas, y se limita el resultado a 150 registros para velocidad óptima.
    QString sql = QString(
        "SELECT articulos.cod, articulos.descripcion, %1 as pvp, articulos.iva, "
        "0 as stock, articulos.min, articulos.max, 0 as pendientes_pedido, "
        "0 as encargados, articulos.ultima_venta, articulos.ultimo_pedido, "
        "articulos.familia, articulos.precio_compra, articulos.fabricante, articulos.foto, "
        "articulos.notas, articulos.formato, articulos.cantformato, "
        "(SELECT COALESCE(SUM(cantidad), 0) FROM lotes WHERE ean = articulos.cod) as stock_total, "
        "(SELECT MIN(fecha) FROM lotes WHERE ean = articulos.cod) as fecha_caducidad "
        "FROM articulos "
        "LEFT JOIN precios_tienda pt ON articulos.cod = pt.cod_articulo "
        "%2 "
        "ORDER BY articulos.descripcion "
        "LIMIT 150").arg(pvpQuery, clFiltro);

    consulta.exec(sql);
    return consulta;
  }
  return QSqlQuery();
}

bool baseDatos::insertarUsuario(QSqlDatabase db, QStringList datos) {
  QSqlQuery consulta(db);

  // SEC-02: Comprobar de forma dinámica si la columna 'salt' existe en la tabla usuarios.
  bool tieneSalt = false;
  QSqlQuery checkCol(db);
  if (checkCol.exec("SHOW COLUMNS FROM usuarios LIKE 'salt'")) {
      tieneSalt = checkCol.next();
  }

  QString claveFinal = datos.at(11);
  QString saltFinal = "";

  if (tieneSalt) {
      // Generar un salt seguro de 16 bytes y hashear la clave.
      saltFinal = HashUtil::generarSalt();
      claveFinal = HashUtil::hashPassword(claveFinal, saltFinal);
  }

  if (tieneSalt) {
      consulta.prepare(
          "INSERT INTO usuarios (id , nombre , apellido , direccion , localidad , "
          "provincia , cp , tlfn , nif , mail , usuario , clave , notas , rol , "
          "foto, salt) "
          "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
  } else {
      consulta.prepare(
          "INSERT INTO usuarios (id , nombre , apellido , direccion , localidad , "
          "provincia , cp , tlfn , nif , mail , usuario , clave , notas , rol , "
          "foto) "
          "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
  }

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
  consulta.bindValue(11, claveFinal);
  consulta.bindValue(12, datos.at(12));
  consulta.bindValue(13, datos.at(13).toInt());
  consulta.bindValue(14, datos.at(14));

  if (tieneSalt) {
      consulta.bindValue(15, saltFinal);
  }

  if (!consulta.exec()) {
    db.rollback();
    return false;
  } else {
    db.commit();
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

QSqlQuery baseDatos::buscarEnTabla(QSqlDatabase db, QString tabla,
                                   QString campo, QString dato) {
  if (db.isOpen()) {
    QSqlQuery consulta(db);
    consulta.exec("SELECT * FROM " + tabla + " WHERE " + campo + " LIKE '%" +
                  dato + "%'");
    qDebug() << consulta.lastError().text();

    return consulta;
  }
  qDebug() << consulta.lastError().text();

  return QSqlQuery();
}

bool baseDatos::modificarUsuaruio(QSqlDatabase db, QStringList datos,
                                  QString dato) {
  QSqlQuery consulta(db);

  // SEC-02: Comprobar de forma dinámica si la columna 'salt' existe en la tabla usuarios.
  bool tieneSalt = false;
  QSqlQuery checkCol(db);
  if (checkCol.exec("SHOW COLUMNS FROM usuarios LIKE 'salt'")) {
      tieneSalt = checkCol.next();
  }

  QString claveFinal = datos.at(11);
  QString saltFinal = "";
  bool actualizarSalt = false;

  if (tieneSalt) {
      // Si la clave NO es un hash SHA-256 válido, significa que es texto plano y se está modificando.
      if (!HashUtil::esHashSHA256(claveFinal)) {
          saltFinal = HashUtil::generarSalt();
          claveFinal = HashUtil::hashPassword(claveFinal, saltFinal);
          actualizarSalt = true;
      }
  }

  if (tieneSalt && actualizarSalt) {
      consulta.prepare(
          "UPDATE usuarios SET id =?, nombre=? , apellido=? , direccion=? , "
          "localidad=? "
          ", provincia=? , cp=? , tlfn=? , nif=? , mail=? , usuario=? , clave=? , "
          "notas=? , rol=? , foto=? , salt=?  WHERE id=?");
  } else {
      consulta.prepare(
          "UPDATE usuarios SET id =?, nombre=? , apellido=? , direccion=? , "
          "localidad=? "
          ", provincia=? , cp=? , tlfn=? , nif=? , mail=? , usuario=? , clave=? , "
          "notas=? , rol=? , foto=?  WHERE id=?");
  }

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
  consulta.bindValue(11, claveFinal);
  consulta.bindValue(12, datos.at(12));
  consulta.bindValue(13, datos.at(13).toInt());
  consulta.bindValue(14, datos.at(14));

  if (tieneSalt && actualizarSalt) {
      consulta.bindValue(15, saltFinal);
      consulta.bindValue(16, dato.toInt());
  } else {
      consulta.bindValue(15, dato.toInt());
  }

  if (!consulta.exec()) {
    db.rollback();
    return false;
  } else {
    db.commit();
    return true;
  }
}

bool baseDatos::modificarFotoUsusario(QString foto, int id) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("UPDATE usuarios SET foto=? WHERE id =?");
  consulta.bindValue(0, foto);
  consulta.bindValue(1, id);
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError();
  return false;
}

bool baseDatos::modificarArticulo(QSqlDatabase db, QStringList datos,
                                  QString dato) {
  QSqlQuery consulta(db);
  consulta.prepare("UPDATE articulos SET cod = ? , descripcion = ? , pvp = ? , "
                   "iva = ? , min = ? , max = ? , "
                   "ultima_venta = ? , ultimo_pedido = ? , "
                   "familia = ? , precio_compra = ? , fabricante = ? , foto "
                   "= ? , notas = ? , formato = ? , "
                   "cantformato = ? WHERE cod = ?");
  consulta.bindValue(0, datos.at(0));
  consulta.bindValue(1, datos.at(1));
  consulta.bindValue(2, datos.at(2).toDouble());
  consulta.bindValue(3, datos.at(3).toInt());
  consulta.bindValue(4, datos.at(5).toInt()); // min (antes era 5)
  consulta.bindValue(5, datos.at(6).toInt()); // max (antes era 6)
  consulta.bindValue(6, datos.at(9));         // ultima_venta (antes era 9)
  consulta.bindValue(7, datos.at(10));        // ultimo_pedido (antes era 10)
  consulta.bindValue(8, datos.at(11).toInt());
  consulta.bindValue(9, datos.at(12).toDouble());
  consulta.bindValue(10, datos.at(13).toInt());
  consulta.bindValue(11, datos.at(14));
  consulta.bindValue(12, datos.at(15));
  consulta.bindValue(13, datos.at(16));
  consulta.bindValue(14, datos.at(17).toDouble());
  consulta.bindValue(15, dato);

  qDebug() << db.lastError().text();
  qDebug() << consulta.lastError();
  if (!consulta.exec()) {
    // QMessageBox::warning(this,"Advertencia!","No se pudo realizar la
    // operacion solicitada \n"+db.lastError().text());
    db.rollback();
    // QSqlDatabase::database().rollback();
    return false;

  } else {
    db.commit(); // QSqlDatabase::database().commit();
    return true;
  }
}

bool baseDatos::insertarArticulo(QSqlDatabase db, QStringList datos) {
  qDebug() << datos;
  QSqlQuery consulta(db);
  consulta.prepare(
      "INSERT INTO articulos (cod, descripcion, pvp, iva, min, max, "
      "ultima_venta, ultimo_pedido, familia, "
      "precio_compra, fabricante, foto, notas, formato, cantformato) "
      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");

  consulta.bindValue(0, datos.at(0));
  consulta.bindValue(1, datos.at(1));
  consulta.bindValue(2, datos.at(2));
  consulta.bindValue(3, datos.at(3));
  consulta.bindValue(4, datos.at(5));  // min
  consulta.bindValue(5, datos.at(6));  // max
  consulta.bindValue(6, datos.at(9));  // ultima_venta
  consulta.bindValue(7, datos.at(10)); // ultimo_pedido
  consulta.bindValue(8, datos.at(11));
  consulta.bindValue(9, datos.at(12));
  consulta.bindValue(10, datos.at(13));
  consulta.bindValue(11, datos.at(14));
  consulta.bindValue(12, datos.at(15));
  consulta.bindValue(13, datos.at(16));
  consulta.bindValue(14, datos.at(17));
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

bool baseDatos::descontarArticulo(QString db, QString cod, int uds) {
  QSqlQuery consulta(QSqlDatabase::database(db));

  // No sincronizar cambios realizados desde el proceso de venta (TPV)
  consulta.exec("SET @skip_sync = 1");

  consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '" + cod +
                "' ORDER BY fecha ASC");
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
        descontarArticulo(db, cod, resta);
        qDebug() << consulta.lastError().text();
        return true;
      }
    }
  } else {
    qDebug() << "No hay lotes de ese artículo";
    crearLote(conf->getConexionLocal(), cod, "", "2000-01-01",
              QString::number(0 - uds));
  }
  
  consulta.exec("SET @skip_sync = 0");
  return true;
}

bool baseDatos::actualizarFechaVentaArticulo(QString nombreConexion,
                                             QString cod, QString fecha) {
  QSqlDatabase db = QSqlDatabase::database(nombreConexion);

  // No sincronizar cambios realizados desde el proceso de venta (TPV)
  QSqlQuery qSkip(db);
  qSkip.exec("SET @skip_sync = 1");

  // 2. Iniciar la transacción
  if (!db.transaction()) {
    qDebug() << "Error al iniciar la transacción:" << db.lastError().text();
    return false;
  }

  QSqlQuery consulta(db);
  consulta.prepare("UPDATE articulos SET ultima_venta = ? WHERE cod LIKE ?");
  consulta.bindValue(0, fecha);
  consulta.bindValue(1, cod);

  if (consulta.exec()) {
    // 3. Si la consulta fue exitosa, confirmar los cambios
    if (db.commit()) {
      qSkip.exec("SET @skip_sync = 0");
      return true;
    } else {
      // Error al confirmar (ej. problemas de red, disco lleno, etc.)
      qDebug() << "Error al confirmar la transacción:" << db.lastError().text();
      // Intenta hacer un rollback si el commit falla
      db.rollback();
      return false;
    }
  } else {
    // 4. Si la consulta falló, mostrar el error y revertir
    qDebug() << "Error al ejecutar la consulta UPDATE:"
             << consulta.lastError().text();
    db.rollback();
    return false;
  }
}

bool baseDatos::actualizarArticulosDesdeCompras(QString base,
                                                QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("UPDATE articulos SET descripcion = ? , pvp = ? , iva = ? , "
                   "precio_compra = ? "
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

bool baseDatos::borrarUsusario(QSqlDatabase db, int dato) {
  QSqlQuery consulta(db);
  consulta.prepare("DELETE FROM usuarios WHERE id = ?");
  consulta.bindValue(0, dato);
  if (!consulta.exec()) {
    // QMessageBox::warning(this,"Advertencia!","No se pudo realizar la
    // operacion solicitada \n"+db.lastError().text());
    db.rollback();
    // QSqlDatabase::database().rollback();
    return false;

  } else {
    db.commit(); // QSqlDatabase::database().commit();
    return true;
  }
}

bool baseDatos::borrarArticulo(QSqlDatabase db, QString dato) {
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

bool baseDatos::modificarFotoArticulo(QString foto, QString dato) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
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

bool baseDatos::propagarCambioCodigoArticulo(QString oldCod, QString newCod) {
  QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
  if (!dbLocal.transaction()) {
    qDebug() << "Error al iniciar transaccion para propagar cambio de codigo";
    return false;
  }

  QSqlQuery q(dbLocal);

  // 1. Obtener la descripción actual del artículo antes de cambiarlo
  QString descripcion;
  q.prepare("SELECT descripcion FROM articulos WHERE cod = ?");
  q.addBindValue(oldCod);
  if (q.exec() && q.next()) {
    descripcion = q.value(0).toString();
  } else {
    qDebug() << "Atención: No se pudo obtener la descripción original para " << oldCod;
  }

  // Desactivar temporalmente las restricciones de claves foráneas para poder modificar la PK
  q.exec("SET FOREIGN_KEY_CHECKS = 0;");

  // Estructura de tablas y sus columnas que necesitan actualización
  struct Dependencia {
    QString tabla;
    QString colCod;
    QString colDesc;
  };

  const QList<Dependencia> dependencias = {
      {"articulos", "cod", ""}, // PK principal, se actualiza solo el código
      {"lineasticket", "cod", "descripcion"},
      {"lineaspedido", "cod", "descripcion"},
      {"lineaspedido_tmp", "cod", "descripcion"},
      {"encargos", "cod_articulo", ""},
      {"precios_tienda", "cod_articulo", ""},
      {"lotes", "ean", ""},
      {"etiquetas", "cod", ""},
      {"codaux", "cod", ""},
      {"entradaGenero_tmp", "cod", "descripcion"},
      {"salidaGenero_tmp", "cod", "descripcion"},
      {"historico_stock", "ean", ""},
      {"lineasticket_tmp", "cod", "descripcion"},
      {"salidaGenero", "cod", "descripcion"}};

  for (const auto &dep : dependencias) {
    if (dep.colDesc.isEmpty()) {
      // Solo actualizar código (PK o FK simple)
      q.prepare(QString("UPDATE %1 SET %2 = ? WHERE %2 = ?").arg(dep.tabla, dep.colCod));
      q.addBindValue(newCod);
      q.addBindValue(oldCod);
    } else {
      // Actualizar código y descripción (historico desnormalizado)
      q.prepare(QString("UPDATE %1 SET %2 = ?, %3 = ? WHERE %2 = ?").arg(dep.tabla, dep.colCod, dep.colDesc));
      q.addBindValue(newCod);
      q.addBindValue(descripcion);
      q.addBindValue(oldCod);
    }

    if (!q.exec()) {
      qDebug() << "Error actualizando" << dep.tabla << ":" << q.lastError().text();
      q.exec("SET FOREIGN_KEY_CHECKS = 1;");
      dbLocal.rollback();
      return false;
    }
  }

  q.exec("SET FOREIGN_KEY_CHECKS = 1;");

  if (dbLocal.commit()) {
    qDebug() << "Cambio de codigo y descripcion propagado de" << oldCod << "a" << newCod;
    return true;
  } else {
    qDebug() << "Error al hacer commit del cambio de codigo";
    dbLocal.rollback();
    return false;
  }
}

bool baseDatos::existeArticuloEnNube(QString cod) {
  // Intentar usar la conexión existente "NUBE" creada por SyncManager
  QSqlDatabase dbNube = QSqlDatabase::database("NUBE");
  if (!dbNube.isOpen()) {
    qDebug() << "Nube no conectada, no se puede verificar duplicidad remota.";
    return false;
  }

  QSqlQuery q(dbNube);
  q.prepare("SELECT cod FROM articulos WHERE cod = ?");
  q.addBindValue(cod);
  if (q.exec() && q.next()) {
    return true;
  }
  return false;
}

QSqlRecord baseDatos::getArticuloNube(QString cod) {
  QSqlDatabase dbNube = QSqlDatabase::database("NUBE");
  if (!dbNube.isOpen()) return QSqlRecord();

  QSqlQuery q(dbNube);
  q.prepare("SELECT * FROM articulos WHERE cod = ?");
  q.addBindValue(cod);
  if (q.exec() && q.next()) {
    return q.record();
  }
  return QSqlRecord();
}

bool baseDatos::actualizarArticuloDesdeRecord(QString cod, const QSqlRecord &record) {
  QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
  QSqlQuery q(dbLocal);

  // Campos a sincronizar desde la nube al local (ignorando stock)
  q.prepare("UPDATE articulos SET descripcion = ?, pvp = ?, iva = ?, familia = ?, "
            "fabricante = ?, precio_venta = ? WHERE cod = ?");

  q.addBindValue(record.value("descripcion"));
  q.addBindValue(record.value("pvp"));
  q.addBindValue(record.value("iva"));
  q.addBindValue(record.value("familia"));
  q.addBindValue(record.value("fabricante"));
  q.addBindValue(record.value("precio_venta"));
  q.addBindValue(cod);

  if (!q.exec()) {
    qDebug() << "Error actualizando articulo desde nube:" << q.lastError().text();
    return false;
  }
  return true;
}

QSqlQuery baseDatos::ventasClientes(QString nombreConexion, QDate fechaI,
                                    QDate fechaF) {
  QSqlDatabase db = QSqlDatabase::database(nombreConexion);
  QSqlQuery consulta(db);
  
  QString desde = fechaI.toString("yyyy-MM-01");
  QString hasta = fechaF.toString("yyyy-MM-" + QString::number(fechaF.daysInMonth()));
  
  QString sql;
  // Si existe la tabla histórica ticketss, unificamos con tickets
  if (db.tables().contains("ticketss")) {
      sql = QString("SELECT cliente, SUM(total) FROM ("
                    "  SELECT cliente, total FROM tickets WHERE fecha BETWEEN '%1' AND '%2' "
                    "  UNION ALL "
                    "  SELECT cliente, total FROM ticketss WHERE fecha BETWEEN '%1' AND '%2' "
                    ") AS todas_ventas WHERE cliente > 0 GROUP BY cliente").arg(desde, hasta);
  } else {
      sql = QString("SELECT cliente, SUM(total) FROM tickets "
                    "WHERE fecha BETWEEN '%1' AND '%2' AND cliente > 0 GROUP BY cliente").arg(desde, hasta);
  }

  if (!consulta.exec(sql)) {
      qDebug() << "Error en ventasClientes (" << nombreConexion << "):" << consulta.lastError().text();
  }
  
  return consulta;
}

double baseDatos::valeCliente(QString nombreConexion, QString idCLiente) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  // NOTA: numRowsAffected() no funciona con SELECT (siempre devuelve -1 o 0).
  // Se usa consulta.next() para comprobar si hay resultado.
  consulta.prepare("SELECT cantidad FROM vales WHERE idCLiente = ? "
                   "AND estado = 1 ORDER BY fechaEmision DESC, vale_uuid DESC LIMIT 1");
  consulta.bindValue(0, idCLiente);
  if (consulta.exec() && consulta.next()) {
    return consulta.value(0).toDouble();
  }
  return 0;
}

bool baseDatos::caducarVales(QString nombreConexion) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  // Marcar todos los vales activos como caducados (estado 1 → 0)
  // Solo se ejecuta en local; el SyncManager propaga el cambio a la nube
  if (consulta.exec("UPDATE vales SET estado = 0 WHERE estado = 1")) {
    qDebug() << "Vales caducados correctamente en" << nombreConexion
             << "(" << consulta.numRowsAffected() << "registros)";
    return true;
  }
  qDebug() << "Error al caducar vales:" << consulta.lastError();
  return false;
}

int baseDatos::idVale(QString nombreConexion, QString idCliente) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  // NOTA: numRowsAffected() no funciona con SELECT (siempre devuelve -1 o 0).
  // Se usa consulta.next() para comprobar si hay resultado.
  consulta.prepare("SELECT idVales FROM vales WHERE idCliente= ? "
                   "AND estado = 1 ORDER BY fechaEmision DESC, vale_uuid DESC LIMIT 1");
  consulta.bindValue(0, idCliente);
  if (consulta.exec() && consulta.next()) {
    return consulta.value(0).toInt();
  }
  return 0;
}

bool baseDatos::usarVale(QString nombreConexion, int idVale) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  consulta.prepare("UPDATE vales SET estado = 2, fechaUso = CURRENT_DATE() "
                   "WHERE idvales = ?");
  consulta.bindValue(0, idVale);
  if (consulta.exec()) {
    return true;
  }
  return false;
}

// NOTA: valesPendientesMarcar, hayValesPendientesMarcar, valesPendientes y
// borrarValePendiente han sido eliminados (2026-04-11).
// La propagación del estado de los vales se hace ahora vía nube
// (SyncManager):
//   - usarVale() marca en local + nube directamente
//   - Si la nube no está disponible, sync_cola propaga el cambio
//   automáticamente

QSqlQuery baseDatos::tickesPorCLiente(QString nombreConexion, QString fechaI,
                                      QString fechaF, QString idCliente) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  consulta.prepare("SELECT * FROM tickets WHERE cliente = ? "
                   "AND fecha BETWEEN ? AND ?");
  consulta.bindValue(0, idCliente);
  consulta.bindValue(1, fechaI);
  consulta.bindValue(2, fechaF);

  if (consulta.exec()) {
    qDebug() << consulta.lastQuery() << consulta.boundValue(0).toString()
             << consulta.boundValue(1).toString()
             << consulta.boundValue(2).toString();
    consulta.first();
    return consulta;
  }
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::productosPorClienteCantidad(QString nombreConexion,
                                                 QString idCliente,
                                                 QDate fechaI, QDate fechaF, bool consolidado) {
  QSqlDatabase db = QSqlDatabase::database(nombreConexion);
  QSqlQuery consulta(db);
  
  QString queryStr;
  if (consolidado && db.tables().contains("lineasticketss")) {
      QString subQuery = "SELECT l.cod, l.descripcion, l.cantidad FROM lineasticket l "
                         "JOIN tickets t ON l.nticket = t.ticket "
                         "WHERE t.cliente = '" + idCliente + "' "
                         "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "'";
      
      subQuery += " UNION ALL SELECT l.cod, l.descripcion, l.cantidad FROM lineasticketss l "
                  "JOIN ticketss t ON l.nticket = t.ticket "
                  "WHERE t.cliente = '" + idCliente + "' "
                  "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "'";

      queryStr = "SELECT cod, descripcion, SUM(cantidad) as total FROM (" + subQuery + ") as sub "
                 "GROUP BY cod, descripcion ORDER BY total DESC";
  } else {
      queryStr = "SELECT cod, descripcion, SUM(cantidad) as total FROM lineasticket l "
                 "JOIN tickets t ON l.nticket = t.ticket "
                 "WHERE t.cliente = '" + idCliente + "' "
                 "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "' "
                 "GROUP BY cod, descripcion ORDER BY total DESC";
  }

  if (!consulta.exec(queryStr)) {
    qDebug() << consulta.lastError() << "en productosPorClienteCantidad";
    qDebug() << "Query:" << queryStr;
  }
  consulta.first();
  return consulta;
}

QSqlQuery baseDatos::productosPorClienteFecha(QString nombreConexion,
                                              QString idCliente, QDate fechaI,
                                              QDate fechaF, bool consolidado) {
  QSqlDatabase db = QSqlDatabase::database(nombreConexion);
  QSqlQuery consulta(db);
  
  QString queryStr;
  if (consolidado && db.tables().contains("lineasticketss")) {
      QString subQuery = "SELECT l.cod, l.descripcion, l.cantidad, t.fecha FROM lineasticket l "
                         "JOIN tickets t ON l.nticket = t.ticket "
                         "WHERE t.cliente = '" + idCliente + "' "
                         "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "'";
      
      subQuery += " UNION ALL SELECT l.cod, l.descripcion, l.cantidad, t.fecha FROM lineasticketss l "
                  "JOIN ticketss t ON l.nticket = t.ticket "
                  "WHERE t.cliente = '" + idCliente + "' "
                  "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "'";

      queryStr = "SELECT cod, descripcion, cantidad, fecha FROM (" + subQuery + ") as sub "
                 "ORDER BY fecha DESC";
  } else {
      queryStr = "SELECT l.cod, l.descripcion, l.cantidad, t.fecha FROM lineasticket l "
                 "JOIN tickets t ON l.nticket = t.ticket "
                 "WHERE t.cliente = '" + idCliente + "' "
                 "AND t.fecha BETWEEN '" + fechaI.toString("yyyy-MM-dd") + "' AND '" + fechaF.toString("yyyy-MM-dd") + "' "
                 "ORDER BY t.fecha DESC";
  }

  if (!consulta.exec(queryStr)) {
    qDebug() << consulta.lastError() << "en productosPorClienteFecha";
    qDebug() << "Query:" << queryStr;
  }
  consulta.first();
  return consulta;
}

QString baseDatos::nombreFamilia(QString id) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT * FROM familias WHERE id LIKE ?");
  consulta.bindValue(0, id);
  consulta.exec();
  if (consulta.first() == true) {
    return consulta.value(1).toString();
  }
  return "Sin familia asignada";
}

QString baseDatos::nombreFabricante(QString id) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT * FROM fabricantes WHERE id LIKE ?");
  consulta.bindValue(0, id);
  consulta.exec();
  if (consulta.first() == true) {
    return consulta.value(1).toString();
  }
  return "Sin asignar";
}

QString baseDatos::nombreUsusario(QString id, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT * FROM usuarios WHERE id LIKE ?");
  consulta.bindValue(0, id);
  consulta.exec();
  if (consulta.first() == true) {
    return consulta.value(1).toString();
  }
  return "Sin asignar";
}

QString baseDatos::nombreCliente(QString id) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
  consulta.bindValue(0, id);
  consulta.exec();
  if (consulta.first() == true) {
    return consulta.value(1).toString() + " " + consulta.value(2).toString();
  }
  return "Sin asignar";
}

QString baseDatos::etiquetaCliente(QString idCliente) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT * FROM clientes WHERE idCliente LIKE ?");
  consulta.bindValue(0, idCliente);
  consulta.exec();
  if (consulta.first() == true) {
    QString cliente = "";
    cliente += consulta.value(1).toString() + " " +
               consulta.value(2).toString() + "<br>";
    cliente += consulta.value(3).toString() + "<br>";
    cliente += consulta.value(4).toString() + "  " +
               consulta.value(5).toString() + "<br>";
    cliente += consulta.value(6).toString() + "<br>";
    cliente += "N.I.F: " + consulta.value(7).toString();
    return cliente;
  }
  return "Sin asignar";
}

QString baseDatos::nombreFormaPago(QString id, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  
  // 1. Intentar buscar por ID (numérico)
  consulta.prepare("SELECT tipo FROM fpago WHERE id = ?");
  consulta.bindValue(0, id.toInt());
  if (consulta.exec() && consulta.first()) {
    return consulta.value(0).toString();
  }
  
  // 2. Fallback: Si no es un número o no se encontró, intentar buscar por el nombre directamente
  // (útil si hay registros antiguos que guardaron el texto en lugar del ID)
  consulta.prepare("SELECT tipo FROM fpago WHERE tipo LIKE ?");
  consulta.bindValue(0, id);
  if (consulta.exec() && consulta.first()) {
    return consulta.value(0).toString();
  }

  return "Sin forma de pago asignada";
}

QString baseDatos::idFormaPago(QString fpago, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT id FROM fpago WHERE tipo LIKE ?");
  consulta.bindValue(0, fpago);
  if (consulta.exec() == true) {
    consulta.first();
    return consulta.value(0).toString();
  }
  return nullptr;
}

bool baseDatos::insertarEtiqueta(QString base, QString etiqueta) {
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

bool baseDatos::modificarTienda(QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare(
      "UPDATE tiendas SET nombre = ? , direccion = ? , ciudad = ? , telefono = "
      "? , whatsapp = ? "
      ", email = ? , ip = ? , usuario = ? , password = ? , master = ? , local "
      "= ?, baseDatos = ?, puerto = ?, ssl_ca = ? WHERE id = ?");
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
  consulta.bindValue(11, datos.at(12));
  consulta.bindValue(12, datos.at(13).toInt());
  consulta.bindValue(13, datos.at(14)); // ssl_ca
  consulta.bindValue(14, datos.at(0));  // id
  if (consulta.exec()) {
    return true;
  }
  m_lastError = consulta.lastError().text();
  return false;
}

bool baseDatos::borrarTienda(QString dato) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("DELETE FROM tiendas where id = ?");
  consulta.bindValue(0, dato);
  if (consulta.exec()) {
    return true;
  }
  return false;
}

bool baseDatos::crearTienda(QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("INSERT INTO tiendas (nombre, direccion, ciudad, telefono, "
                   "whatsapp, email, ip, "
                   "usuario, password, master, local, baseDatos, puerto, "
                   "ssl_ca) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");

  // Comprobar duplicados por nombre o IP
  QSqlQuery check(QSqlDatabase::database(conf->getConexionLocal()));
  check.prepare("SELECT id FROM tiendas WHERE nombre = ? OR ip = ?");
  check.bindValue(0, datos.at(1));
  check.bindValue(1, datos.at(7));
  if (check.exec() && check.next()) {
      m_lastError = "Ya existe una tienda con ese nombre o IP.";
      return false;
  }

  if (consulta.exec()) {
    return true;
  }
  m_lastError = consulta.lastError().text();
  qDebug() << "Error en crearTienda:" << m_lastError;
  return false;
}

QSqlQuery baseDatos::tiendas(QSqlDatabase db) {
  // Devuelve las tiendas remotas (excluye la marcada como local y las IP de
  // loopback)
  QSqlQuery consulta(db);
  if (!consulta.exec("SELECT * FROM tiendas WHERE local = 0 ")) {
    qDebug() << consulta.lastError();
  }
  return consulta;
}

int baseDatos::idTiendaDesdeNombre(QSqlDatabase db, QString nombreTienda) {
  QSqlQuery consulta(db);
  consulta.prepare("SELECT id FROM tiendas WHERE nombre = ?");
  consulta.bindValue(0, nombreTienda);
  if (!consulta.exec()) {
    qDebug() << consulta.lastError();
  }
  consulta.first();
  return consulta.value(0).toInt();
}

QString baseDatos::nombreProveedor(QString id, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT nombre FROM proveedores WHERE idProveedor LIKE ?");
  consulta.bindValue(0, id);
  if (consulta.exec() == true) {
    consulta.first();
    return consulta.value(0).toString();
  }
  return 0;
}

QString baseDatos::idProveedor(QString nombre, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  // Limpieza de espacios para mayor robustez
  consulta.prepare("SELECT idProveedor FROM proveedores WHERE TRIM(nombre) = "
                   "TRIM(?) LIMIT 1");
  consulta.bindValue(0, nombre);
  if (consulta.exec() && consulta.first()) {
    return consulta.value(0).toString();
  }
  return QString();
}

bool baseDatos::modificarProveedor(QSqlDatabase db, QStringList datos,
                                   QString dato) {
  QSqlQuery consulta(db);
  qDebug() << datos;
  consulta.prepare(
      "UPDATE proveedores SET nombre = ?, nif = ? , direccion = ?, cp = ?, "
      "localidad= ?, "
      "provincia = ?, representante = ?, telefonor = ?, mailr = ?, telefono = "
      "?, mail = ?, "
      "descuento = ?, formapago = ?, notas = ? WHERE idProveedor = ?");
  // consulta.bindValue(0,datos.at(0).toInt());
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

bool baseDatos::borrarProveedor(QSqlDatabase db, QString dato) {
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

QString baseDatos::descuentoProveedor(QString proveedor) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT descuento FROM proveedores WHERE nombre LIKE ?");
  consulta.bindValue(0, proveedor);
  if (consulta.exec() == true) {
    consulta.first();
    return consulta.value(0).toString();
  }
  return 0;
}

QStringList baseDatos::listadoProveedores(QString db) {
  QStringList proveedores;
  QSqlQuery consulta(QSqlDatabase::database(db));
  consulta.exec("SELECT nombre FROM proveedores ORDER BY nombre ASC");
  while (consulta.next()) {
    proveedores << consulta.value("nombre").toString();
  }
  return proveedores;
}

QString baseDatos::codigoParaNuevoProveedor() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT max(idProveedor) FROM proveedores");
  consulta.first();
  int id = consulta.value(0).toInt() + 1;
  return QString::number(id);
}

QString baseDatos::codigoDesdeAux(QString base, QString aux) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT cod FROM codaux WHERE aux = ?");
  consulta.bindValue(0, aux);
  consulta.exec();
  consulta.first();
  return consulta.value(0).toString();
}

QSqlQuery baseDatos::datosTicket(QString base, QString nTicket) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT * FROM tickets WHERE ticket = ?");
  consulta.bindValue(0, nTicket);
  consulta.exec();
  consulta.first();
  return consulta;
}

QSqlQuery baseDatos::consultarLineasTicket(QString base, QString nTicket) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT * FROM lineasticket WHERE nticket = ?");
  consulta.bindValue(0, nTicket);
  consulta.exec();
  return consulta;
}

double baseDatos::obtenerNumeroUltimoTicket(QSqlDatabase db) {
  QSqlQuery consulta(db);
  consulta.exec("SELECT max(ticket) FROM tickets");
  consulta.first();
  return consulta.value(0).toDouble();
}

/**
 * @brief Obtiene el siguiente número de ticket de forma atómica.
 *
 * Utiliza FOR UPDATE para bloquear la lectura del máximo ticket,
 * garantizando que dos puestos no obtengan el mismo número.
 * Consulta tanto la tabla 'tickets' como 'ticketss' para evitar colisiones.
 *
 * @warning DEBE llamarse dentro de una transacción activa (db.transaction()).
 * @param db Conexión a la base de datos (debe tener una transacción abierta).
 * @return El siguiente número de ticket disponible.
 */
int baseDatos::obtenerSiguienteTicketAtomico(QSqlDatabase db) {
  QSqlQuery consulta(db);
  // Obtenemos el máximo de ambas tablas con bloqueo FOR UPDATE
  // para que otro puesto que intente lo mismo espere a que esta
  // transacción termine (commit/rollback).
  bool ok = consulta.exec(
      "SELECT GREATEST("
      "  COALESCE((SELECT MAX(ticket) FROM tickets FOR UPDATE), 0),"
      "  COALESCE((SELECT MAX(ticket) FROM ticketss FOR UPDATE), 0)"
      ") + 1 AS siguiente");
  if (ok && consulta.first()) {
    return consulta.value(0).toInt();
  }
  // Fallback: si falla la consulta atómica, usar el método clásico
  qWarning() << "obtenerSiguienteTicketAtomico: fallback al método no-atómico"
             << consulta.lastError().text();
  return static_cast<int>(obtenerNumeroUltimoTicket(db)) + 1;
}

QSqlQuery baseDatos::tcketsPendientes(QSqlDatabase db) {
  QSqlQuery consulta(db);
  consulta.exec("SELECT * FROM ticket_tmp");
  // consulta.first();

  return consulta;
}

int baseDatos::maxTicketPendiente(QSqlDatabase db) {
  QSqlQuery consulta(db);

  if (consulta.exec("SELECT max(orden) FROM ticket_tmp")) {
    consulta.first();
    if (consulta.numRowsAffected() == 0) {
      return 0;
    }
  }
  return consulta.value(0).toInt();
}

bool baseDatos::nuevoTicketTmp(int orden, int cliente, int vendedor) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
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

bool baseDatos::grabarTicket(QString base, QString serie, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("INSERT INTO " + serie +
                   " VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?)");
  // consulta.bindValue(0,serie);
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec()) {
    return true;
  }
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::grabarLineaTicket(QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare(
      "INSERT INTO lineasticket VALUES (NULL,?,?,?,?,?,?,?,?,?,?)");
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec()) {
    return true;
  }
  qDebug() << consulta.lastError().text() << "en grabarlineaticket";
  return false;
}

bool baseDatos::borrarTicketTmp(int ticket) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("DELETE FROM ticket_tmp WHERE orden = ?");
  consulta.bindValue(0, ticket);
  if (consulta.exec()) {
    return true;
  }
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::modificarCliente(QSqlDatabase db, QStringList datos,
                                 QString dato) {
  QSqlQuery consulta(db);
  consulta.prepare(
      "UPDATE clientes SET idCliente = ? , nombre = ? , apellidos = ? , "
      "direccion = ? , cp = ? , "
      "localidad = ? , provincia = ? , nif = ? , telefono = ? , telefono2 = ? "
      ", mail = ? , "
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
  QString texto = QString("UPDATE clientes SET idCliente = '%1' , nombre = "
                          "'%2' , apellidos = '%3' , "
                          "direccion = '%4' , cp = '%5' , "
                          "localidad = '%6' , provincia = '%7' , nif = '%8' , "
                          "telefono = '%9' , telefono2 "
                          "= '%10' , mail = '%11' , descuento = '%12' , "
                          "fechaAlta = '%13' , notas = '%14'"
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

bool baseDatos::borrarCliente(QSqlDatabase db, int idCliente) {
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

bool baseDatos::crearCliente(QSqlDatabase db, QStringList datos) {
  QSqlQuery consulta(db);
  consulta.prepare(
      "INSERT INTO clientes (idCliente, nombre, apellidos, direccion, cp, "
      "localidad, provincia, nif, telefono, telefono2, mail, descuento, "
      "fechaAlta, notas) VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
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

double baseDatos::descuentoCliente(QString idCliente) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  
  consulta.prepare("SELECT descuento FROM clientes WHERE idCliente = :idcliente");
  consulta.bindValue(":idcliente", idCliente);
  
  if (consulta.exec() && consulta.first()) {
    return consulta.value("descuento").toDouble();
  }
  return 0;
}

bool baseDatos::existeDatoEnTabla(QSqlDatabase db, QString tabla,
                                   QString columna, QString dato) {
  QSqlQuery consulta(db);
  
  consulta.prepare("SELECT * FROM " + tabla + " WHERE " + columna + " = :dato");
  consulta.bindValue(":dato", dato);
  
  return consulta.exec() && consulta.next();
}

void baseDatos::insertarEnTabla(QSqlDatabase db, QString tabla,
                                QStringList datos) {
  QSqlQuery consulta(db);
  QString cadenaDatos;
  for (int i = 0; i < datos.length() - 1; ++i) {
    cadenaDatos += "?,";
  }
  cadenaDatos += "?";
  consulta.prepare("INSERT INTO " + tabla + " VALUES(NULL," + cadenaDatos +
                   ")");
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec()) {
    return;
  }
  qDebug() << consulta.lastError();
}

void baseDatos::vaciarTabla(QString tabla, QSqlDatabase db) {
  QSqlQuery consulta(db);
  consulta.exec("TRUNCATE " + tabla);
}

bool baseDatos::crearProveedor(QSqlDatabase db, QStringList datos) {
  QSqlQuery consulta(db);
  consulta.prepare(
      "INSERT INTO proveedores (idProveedor, nombre, nif, direccion, cp, "
      "localidad, provincia, representante, telefonoR, mailR, telefono, mail, "
      "descuento, fechaUltimaCompra, formaPago, notas) "
      "VALUES (?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
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

double baseDatos::sumarColumna(QString base, QString tabla, QString campo,
                               QString campoCondicion, QString condicion) {
  if (campoCondicion.isNull())
    campoCondicion = campo;
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(" + campo + ") FROM " + tabla + " WHERE " +
                campoCondicion + " = '" + condicion + "'");
  qDebug() << consulta.lastError().text();
  consulta.first();
  return consulta.value(0).toDouble();
}

int baseDatos::contarLineas(QString tabla, QString base, QString campoCondicion,
                            QString condicion) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT * FROM " + tabla + " WHERE " + campoCondicion + " = '" +
                condicion + "'");
  qDebug() << consulta.lastError().text();
  consulta.first();
  return consulta.numRowsAffected();
}

bool baseDatos::insertarES(QStringList datos, QString base, QString usuario) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  // Se añade un parámetro más para el campo 'usuario'
  consulta.prepare("INSERT INTO entradasSalidas VALUES (NULL,?,?,?,?,?,?)");
  consulta.bindValue(0, datos.at(0)); // fecha
  consulta.bindValue(1, datos.at(1)); // hora
  consulta.bindValue(2, datos.at(2)); // cantidad
  consulta.bindValue(3, datos.at(3)); // idTiposRentrada
  consulta.bindValue(4, datos.at(4)); // descripcion
  consulta.bindValue(5, usuario);     // usuario
  if (consulta.exec())
    return true;
  return false;
}

/**
 * @brief Recupera la configuración del ticket desde la base de datos.
 *
 * Devuelve una lista con los siguientes campos en orden:
 * 0 - cabecera (texto de cabecera del ticket)
 * 1 - pie (texto del pie del ticket)
 * 2 - promocion (ruta de la imagen de cabecera del ticket)
 * 3 - ruta (ruta de la impresora de tickets)
 * 4 - codApertura (código de apertura del cajón)
 * 5 - codCorte (código de corte del papel)
 * 6 - imagenPromo (ruta de la imagen promocional al final del ticket)
 *
 * @return QStringList con los valores de configuración
 */
QStringList baseDatos::recuperarConfigTicket(QString base) {
  QStringList configTicket;
  QSqlQuery consulta(QSqlDatabase::database(base));
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
  // Nuevo campo: ruta de la imagen promocional que se imprime al final del
  // ticket
  configTicket << consulta.value("imagenPromo").toString();
  configTicket << consulta.value("boolPromocion").toString();
  return configTicket;
}

bool baseDatos::ticketPromo(QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  if (!consulta.exec("SELECT boolPromocion FROM tienda.configTicket"))
    return false;
  consulta.first();
  return true;
}

/**
 * @brief Graba la configuración del ticket en la base de datos.
 *
 * Recibe una lista con los campos en el siguiente orden:
 * 0 - cabecera, 1 - pie, 2 - promocion (imagen cabecera),
 * 3 - boolPromocion, 4 - ruta impresora, 5 - codApertura,
 * 6 - codCorte, 7 - imagenPromo (imagen promocional)
 *
 * @param configTicket Lista de valores a grabar
 * @return true si se grabó correctamente, false en caso de error
 */
bool baseDatos::grabarConfiguracionTicket(QStringList configTicket) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare(
      "UPDATE configTicket SET cabecera = ? , pie = ? , promocion = ? , "
      "boolPromocion = ? , ruta = ? , codApertura = ? , codCorte = ? , "
      "imagenPromo = ? WHERE id = 1");
  consulta.bindValue(0, configTicket.at(0));
  consulta.bindValue(1, configTicket.at(1));
  consulta.bindValue(2, configTicket.at(2));
  consulta.bindValue(3, configTicket.at(3));
  consulta.bindValue(4, configTicket.at(4));
  consulta.bindValue(5, configTicket.at(5));
  consulta.bindValue(6, configTicket.at(6));
  // Nuevo campo: ruta de la imagen promocional
  consulta.bindValue(7, configTicket.at(7));
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::crearPedido(QString proveedor, QString nPedido, QString fecha,
                            QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("INSERT INTO albaranes_tmp (idProveedor , npedido, "
                   "fechaPedido) VALUES (?,?,?)");
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

bool baseDatos::borrarPedido(QString base, QString numeroPedido) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec(QString("SELECT id FROM albaranes_tmp WHERE id = '%1'")
                    .arg(numeroPedido));
  consulta.first();
  if (consulta.numRowsAffected() == 1) {
    consulta.exec(QString("DELETE FROM lineaspedido_tmp WHERE idPedido = '%1'")
                      .arg(numeroPedido));
    consulta.exec(
        QString("DELETE FROM albaranes_tmp WHERE id = '%1'").arg(numeroPedido));

    return true;
  } else {
    // Buscar los pedidos que coincidan y mostrarlos
    return false;
  }
}

bool baseDatos::grabarLineaPedido(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("INSERT INTO lineaspedido_tmp VALUES "
                   "(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}
bool baseDatos::modificarLineaPedido(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare(
      "UPDATE lineaspedido_tmp SET idPedido = ? ,"
      "cod = ? ,descripcion = ? , cantidad = ? , bonificacion = ? , lote = ? , "
      "fc = ? , costo = ? , descuento1 = ? ,base = ? , tipoIva = ? ,totalbase "
      "= ? ,"
      "iva= ? , re = ? , pvp = ? WHERE id= ? ");
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

QStringList baseDatos::listadoPrestamistas() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  QStringList prestamistas;
  consulta.exec("SELECT nombre FROM prestamistas");
  while (consulta.next()) {
    prestamistas << consulta.value("nombre").toString();
  }
  return prestamistas;
}

QStringList baseDatos::datosFactura(QSqlDatabase db, QString nFactura) {
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

float baseDatos::sumarIvasPedido(QString base, QString idPedido,
                                 QString tipoIva) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(iva) FROM lineaspedido_tmp WHERE idPedido = '" +
                idPedido + "' AND tipoIva = '" + tipoIva + "'");
  consulta.first();
  return consulta.value(0).toFloat();
}

float baseDatos::sumarRePedido(QString base, QString idPedido,
                               QString tipoIva) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(re) FROM lineaspedido_tmp WHERE idPedido = '" +
                idPedido + "' AND tipoIva = '" + tipoIva + "'");
  consulta.first();
  return consulta.value(0).toFloat();
}

float baseDatos::sumarBasesPedido(QString base, QString idPedido,
                                  QString tipoIva) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec(
      "SELECT sum(totalbase) FROM lineaspedido_tmp WHERE idPedido = '" +
      idPedido + "' AND tipoIva = '" + tipoIva + "'");
  consulta.first();
  return consulta.value(0).toFloat();
}

bool baseDatos::borrarLineaPedido(QString base, QString idLinea) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  if (consulta.exec(
          "DELETE FROM `lineaspedido_tmp` WHERE `lineaspedido_tmp`.`id` = '" +
          idLinea + "'")) {
    qDebug() << "CORRECTO" << consulta.lastError();

    return true;
  }
  qDebug() << "ERROR" << consulta.lastError();

  return false;
}

bool baseDatos::contabilizarPedido(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  
  // Utilizar campos explícitos para soportar de forma robusta la columna 'notas' opcional
  if (datos.length() == 12) {
    consulta.prepare("INSERT INTO pedidos (idProveedor, npedido, fechaPedido, nLineas, nArticulos, descuento, totalbase, totaliva, totalre, total, nFactura, notas) VALUES (?,?,?,?,?,?,?,?,?,?,?,?)");
  } else {
    consulta.prepare("INSERT INTO pedidos (idProveedor, npedido, fechaPedido, nLineas, nArticulos, descuento, totalbase, totaliva, totalre, total, nFactura) VALUES (?,?,?,?,?,?,?,?,?,?,?)");
  }

  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::grabarFactura(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  qDebug() << datos;
  
  // Utilizar campos explícitos para soportar de forma robusta la columna 'notas' opcional
  if (datos.length() == 10) {
    consulta.prepare("INSERT INTO facturas (nFactura, fechaFactura, idProveedor, totalBase, totalIva, totalRe, total, vencimiento, pagada, notas) VALUES (?,?,?,?,?,?,?,?,?,?)");
  } else {
    consulta.prepare("INSERT INTO facturas (nFactura, fechaFactura, idProveedor, totalBase, totalIva, totalRe, total, vencimiento, pagada) VALUES (?,?,?,?,?,?,?,?,?)");
  }

  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec()) {
    qDebug() << "GRABANDO FACTURA";
    return true;
  }
  qDebug() << " ERROR GRABANDO FACTURA" << consulta.lastError().text();
  return false;
}

bool baseDatos::grabarAlbaran(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  
  // Utilizar campos explícitos para soportar de forma robusta la columna 'notas' opcional
  if (datos.length() == 10) {
    consulta.prepare("INSERT INTO albaranes (nFactura, fechaFactura, idProveedor, totalBase, totalIva, totalRe, total, facturada, idFactura, notas) VALUES (?,?,?,?,?,?,?,?,?,?)");
  } else {
    consulta.prepare("INSERT INTO albaranes (nFactura, fechaFactura, idProveedor, totalBase, totalIva, totalRe, total, facturada, idFactura) VALUES (?,?,?,?,?,?,?,?,?)");
  }

  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::borrarAlbaranTmp(QString idAlbaran) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("DELETE FROM albaranes_tmp WHERE id = ?");
  consulta.bindValue(0, idAlbaran);
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::pasarLineaPedidoAHistorico(QString base, QStringList datos) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare(
      "INSERT INTO lineaspedido VALUES(NULL,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?,?)");
  for (int i = 0; i < datos.length(); ++i) {
    consulta.bindValue(i, datos.at(i));
  }
  if (consulta.exec())
    return true;
  qDebug() << consulta.lastError().text();
  return false;
}

QSqlQuery baseDatos::ventas(QString fecha, QString tabla, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(total) , fpago FROM " + tabla + " WHERE fecha = '" +
                fecha + "' group by fecha , fpago");
  return consulta;
}

QSqlQuery baseDatos::ventasPorUsusario(QString fecha, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(total) , usuario FROM tickets WHERE fecha = '" +
                fecha + "' group by fecha , usuario");
  return consulta;
}

QSqlQuery baseDatos::ventasDesdeUltimoArqueo(QString fechaI, QString horaI,
                                             QString tabla, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(total) , fpago FROM " + tabla +
                " WHERE concat_ws('/',fecha , hora) >= '" + fechaI + "/" +
                horaI + "' group by fpago");
  consulta.first();
  return consulta;
}
QSqlQuery baseDatos::recuperarDatosUltimoArqueo(QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT * FROM arqueos ORDER BY id DESC LIMIT 1");
  return consulta;
}

bool baseDatos::grabarArqueo(QStringList datos, QString base,
                             const QMap<double, int> &desglose) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("INSERT INTO arqueos VALUES (NULL,?,?,?,?,?,?,?,?,?)");
  for (int i = 0; i < datos.length(); i++) {
    consulta.bindValue(i, datos.at(i));
  }
  if (!consulta.exec()) {
    qDebug() << consulta.lastError().text();
    return false;
  }

  // Guardar desglose de denominaciones si se proporcionó
  if (!desglose.isEmpty()) {
    int idArqueo = consulta.lastInsertId().toInt();
    QSqlQuery detalle(QSqlDatabase::database(base));
    detalle.prepare(
        "INSERT INTO arqueos_detalle (idArqueo, denominacion, cantidad) "
        "VALUES (?, ?, ?)");
    QMapIterator<double, int> it(desglose);
    while (it.hasNext()) {
      it.next();
      if (it.value() > 0) {
        detalle.bindValue(0, idArqueo);
        detalle.bindValue(1, it.value() > 0 ? it.key() : 0);
        detalle.bindValue(2, it.value());
        if (!detalle.exec()) {
          qDebug() << "Error guardando detalle:" << detalle.lastError().text();
        }
      }
    }
  }
  return true;
}

QSqlQuery baseDatos::ventasEntreFechas(QString fechaI, QString FechaF,
                                       QString tabla, QString base) {
  // Se usa DATE_FORMAT para que la fecha llegue como VARCHAR "yyyy-MM-dd" al driver
  // Qt 6.11 + MariaDB 12.x. Sin el format, el driver devuelve QDate y toString()
  // sin formato usa el locale del sistema (vacío o "dd/MM/yyyy" en español).
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT DATE_FORMAT(fecha,'%Y-%m-%d') , SUM(total) FROM " + tabla +
                   " WHERE fecha >= ? AND fecha <= ? GROUP BY fecha");
  consulta.bindValue(0, fechaI);
  consulta.bindValue(1, FechaF);
  consulta.exec();
  return consulta;
}

int baseDatos::nTarjetasDesdeUltimoArqueo(QString fechaI, QString horaI,
                                          QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT count(*) FROM tickets t JOIN fpago f ON t.fpago = f.id WHERE ((t.fecha > ?) OR (t.fecha "
                   "= ? AND t.hora >= ?)) AND f.efectivo = 0");
  consulta.bindValue(0, fechaI);
  consulta.bindValue(1, fechaI);
  consulta.bindValue(2, horaI);
  consulta.exec();
  consulta.first();
  return consulta.value(0).toInt();
}

QSqlQuery baseDatos::devolverTablaCompleta(QString base, QString nombreTabla) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT * FROM " + nombreTabla);
  return consulta;
}

QSqlQuery baseDatos::ejecutarSentencia(QString sentencia, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec(sentencia);
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::ejecutarSentencia(QString sentencia, QSqlDatabase db) {
  QSqlQuery consulta(db);
  consulta.exec(sentencia);
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::recuperarPedidos(QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT * FROM albaranes_tmp");
  if (!consulta.isValid()) {
    qDebug() << consulta.lastError().text();
  }
  return consulta;
}
double baseDatos::ESdesdeFecha(QString fecha, QString hora, QString base) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.exec("SELECT sum(cantidad) FROM entradasSalidas WHERE "
                "concat_ws('/',fecha,hora) >= '" +
                fecha + "/" + hora + "'");
  consulta.first();
  return consulta.value(0).toDouble();
}

QString baseDatos::idLote(QString base, QString cod, QString lote,
                          QString fecha) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT id FROM lotes WHERE ean = :id_producto AND lote = "
                   ":lote AND fecha = :fecha");
  consulta.bindValue(":id_producto", cod);
  consulta.bindValue(":lote", lote);
  consulta.bindValue(":fecha", fecha);
  if (consulta.exec() && consulta.first()) {
    return consulta.value(0).toString();
  }
  return "0";
}

int baseDatos::unidadesLote(QString base, QString idLote) {
  if (idLote.isEmpty())
    return 0;

  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("SELECT cantidad FROM lotes WHERE id = :id_lote");
  consulta.bindValue(":id_lote", idLote);
  if (consulta.exec() && consulta.first()) {
    return consulta.value(0).toInt();
  }
  return 0;
}

bool baseDatos::borrarLotesArticulo(QString nombreConexion, QString codigo) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConexion));
  consulta.prepare("DELETE FROM lotes WHERE ean = ?");
  consulta.bindValue(0, codigo);
  if (consulta.exec()) {
    return true;
  }
  qDebug() << consulta.lastError();
  return false;
}

void baseDatos::aumentarLote(QString base, QString idLote, int uds) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  
  // No sincronizar cambios realizados desde el proceso de venta (TPV)
  consulta.exec("SET @skip_sync = 1");

  consulta.exec("UPDATE lotes SET cantidad = cantidad + " +
                QString::number(uds) + " WHERE id = '" + idLote + "'");
  if (consulta.numRowsAffected() == 1) {
    qDebug() << "Aumentar lote";

  } else {
    qDebug() << consulta.lastError();
  }
  
  consulta.exec("SET @skip_sync = 0");
}

void baseDatos::disminuirLote(QString cod, QString fecha, int uds) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  
  // No sincronizar cambios realizados desde el proceso de venta (TPV)
  consulta.exec("SET @skip_sync = 1");

  consulta.exec("SELECT id , cantidad FROM lotes WHERE ean = '" + cod +
                "' AND fecha = '" + fecha + "'");
  consulta.exec();
  qDebug() << consulta.numRowsAffected();
  if (consulta.numRowsAffected() == 0) {
    qDebug() << "Pasa por filas = 0";
    QString udsString = QString::number(uds);
    consulta.exec("INSERT INTO lotes (ean , lote ,fecha , cantidad) VALUES ('" +
                  cod + "','','" + fecha + "','-" + udsString + "')");
    qDebug() << consulta.lastError() << "Sin lotes";
    consulta.exec("SET @skip_sync = 0");
    return;
  }

  consulta.first();
  QString id = consulta.record().value(0).toString();
  qDebug() << id;
  if (consulta.record().value(1).toInt() == uds) {
    consulta.exec("DELETE FROM lotes WHERE id LIKE '" + id + "'");
    qDebug() << consulta.lastError() << "== borrando";
    consulta.exec("SET @skip_sync = 0");
    return;
  }
  if (consulta.record().value(1).toInt() < uds) {
    int resto = uds - consulta.record().value(1).toInt();
    consulta.exec("DELETE FROM lotes WHERE id = '" + id + "'");
    qDebug() << consulta.lastError() << "1<";

    disminuirLote(cod, fecha, resto);
    consulta.exec("SET @skip_sync = 0");
    return;
  }
  int descontarUds = consulta.record().value(1).toInt();
  QString resto = QString::number(descontarUds - uds);
  consulta.exec("UPDATE lotes SET cantidad = " + resto + " WHERE id = '" + id +
                "'");
  
  consulta.exec("SET @skip_sync = 0");
  return;
}

void baseDatos::crearLote(QString base, QString ean, QString lote,
                          QString fecha, QString uds) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  consulta.prepare("INSERT INTO lotes VALUES (NULL,?,?,?,?)");
  consulta.bindValue(0, ean);
  consulta.bindValue(1, lote);
  consulta.bindValue(2, fecha);
  consulta.bindValue(3, uds);
  consulta.exec();
  qDebug() << consulta.lastError().text();
}

QSqlQuery baseDatos::lotesProducto(QString cod, QString nombreConnexion) {
  // Devuelve los diferentes lotes de un producto.
  // Usamos DATE_FORMAT para que la fecha llegue como QString "yyyy-MM-dd" y no
  // como QDate, evitando que Qt 6 la convierta con el locale del sistema al
  // llamar a toString() sin formato explícito.
  QSqlQuery consulta(QSqlDatabase::database(nombreConnexion));
  consulta.prepare("SELECT id, ean, lote, DATE_FORMAT(fecha,'%Y-%m-%d') AS fecha, cantidad "
                   "FROM lotes WHERE ean = ? GROUP BY fecha");
  consulta.bindValue(0, cod);
  if (!consulta.exec()) {
    qDebug() << consulta.lastError();
    return consulta;
  }
  return consulta;
}

QString baseDatos::sumarStockArticulo(QString id, QString nombreConnexion) {
  QSqlQuery consulta(QSqlDatabase::database(nombreConnexion));
  consulta.prepare("SELECT sum(cantidad) FROM lotes WHERE ean = ?");
  consulta.bindValue(0, id);
  if (consulta.exec()) {
    consulta.first();
    return consulta.value(0).toString();
  }
  return "Desconocido";
}

QString baseDatos::ticketCercanoFecha(QString tabla, QString fecha,
                                      QString cuando) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  if (cuando == "minimo") {
    consulta.exec("SELECT min(ticket) FROM " + tabla +
                  " WHERE concat_ws('/',fecha,hora) >= '" + fecha + "'");
  } else {
    consulta.exec("SELECT max(ticket) FROM " + tabla +
                  " WHERE concat_ws('/',fecha,hora) <= '" + fecha + "'");
  }
  // consulta.bindValue(0,tabla);
  // consulta.bindValue(1,fecha);
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
                                                QString nUltimoTicketB) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT descripcion , sum(cantidad) FROM lineasticket WHERE "
                "cast(nticket as "
                "unsigned) between '" +
                nPrimerTicket + "' and '" + nUltimoTicket +
                "' or nticket between '" + nPrimerTicketB + "' and '" +
                nUltimoTicketB + "' group by cod order by descripcion asc");
  consulta.first();
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::listadoVentaArticulos(QString inicio, QString final,
                                           QString nombreDB) {
  QSqlQuery consulta(QSqlDatabase::database(nombreDB));
  consulta.prepare("SELECT descripcion , sum(cantidad) FROM lineasticket WHERE "
                   "concat_ws('/',fecha,hora) >= ? "
                   "AND concat_ws('/',fecha,hora) <= ? group by cod asc");
  consulta.bindValue(0, inicio);
  consulta.bindValue(1, final);
  consulta.exec();
  // consulta.first();
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::listadoMovimientosEfectivo(QString db, QString inicio,
                                                QString final, int idTipo) {
  QSqlQuery consulta(QSqlDatabase::database(db));
  QString sql =
      // DATE_FORMAT: evita que Qt 6.11 devuelva QDate con toString() vacío en locale español
      "SELECT DATE_FORMAT(fecha,'%Y-%m-%d') AS fecha , hora , cantidad , motivosEntrada.descripcion , "
      "entradasSalidas.descripcion "
      "FROM entradasSalidas left join motivosEntrada on "
      "entradasSalidas.idTiposRentrada = "
      "motivosEntrada.idtiposEntrada WHERE fecha >= :inicio AND fecha <= :final";

  if (idTipo != -1) {
    sql += " AND idTiposRentrada = :idTipo";
  }

  consulta.prepare(sql);
  consulta.bindValue(":inicio", inicio);
  consulta.bindValue(":final", final);
  if (idTipo != -1) {
    consulta.bindValue(":idTipo", idTipo);
  }
  consulta.exec();
  // consulta.first();
  qDebug() << consulta.lastError();
  return consulta;
}

QSqlQuery baseDatos::listadoCaducados(QString base, QString desde,
                                      QString hasta) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  // Unimos con artículos para obtener descripción y PVP. 
  // Filtramos por el motivo 'Cadicidad' para mantener la coherencia con los informes anteriores.
  consulta.prepare("SELECT h.id, h.ean, ABS(h.stock_new - h.stock_ant), a.descripcion, "
                   "DATE(h.fecha_hora), a.pvp, h.fecha_caducidad_new "
                   "FROM historico_stock h "
                   "JOIN articulos a ON h.ean = a.cod "
                   "WHERE (h.motivo = 'Cadicidad' OR h.motivo = 'Caducidad') "
                   "AND DATE(h.fecha_hora) >= ? AND DATE(h.fecha_hora) <= ? "
                   "ORDER BY h.fecha_hora DESC");
  consulta.bindValue(0, desde);
  consulta.bindValue(1, hasta);
  if (!consulta.exec())
    qDebug() << consulta.lastError();
  return consulta;
}

QMap<QString, QVariant> baseDatos::leerConfiguracion() {
  QMap<QString, QVariant> config;
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  if (consulta.exec("SELECT * FROM configuracion") && consulta.first()) {
    config["recargoeq"] = consulta.value("recargoeq");
    config["precios_locales"] = consulta.value("precios_locales");
    config["vendedor_f1"] = consulta.value("vendedor_f1");
    config["vendedor_f2"] = consulta.value("vendedor_f2");
    config["vendedor_f3"] = consulta.value("vendedor_f3");
    config["vendedor_f4"] = consulta.value("vendedor_f4");
  }
  return config;
}

bool baseDatos::GuardarConfiguracion(QMap<QString, QVariant> datos) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("UPDATE configuracion SET recargoeq = ?, precios_locales = ?, "
                   "vendedor_f1 = ?, vendedor_f2 = ?, vendedor_f3 = ?, vendedor_f4 = ? "
                   "WHERE idconfiguracion = 0");
  consulta.bindValue(0, datos.value("recargoeq", 0));
  consulta.bindValue(1, datos.value("precios_locales", 0));
  consulta.bindValue(2, datos.value("vendedor_f1"));
  consulta.bindValue(3, datos.value("vendedor_f2"));
  consulta.bindValue(4, datos.value("vendedor_f3"));
  consulta.bindValue(5, datos.value("vendedor_f4"));
  if (consulta.exec()) {
    return true;
  }
  qDebug() << consulta.lastError().text();
  return false;
}

bool baseDatos::guardarDirectorios(QString base,
                                   QMap<QString, QString> directorios) {
  QSqlQuery consulta(QSqlDatabase::database(base));
  
  // Usamos una transacción para asegurar integridad si hay varios cambios
  QSqlDatabase::database(base).transaction();
  
  QMapIterator<QString, QString> it(directorios);
  while (it.hasNext()) {
    it.next();
    // Usamos INSERT ... ON DUPLICATE KEY UPDATE porque hemos añadido un índice UNIQUE en 'nombre'
    // Esto asegura que si el nombre ya existe, se actualiza la ruta; si no, se crea.
    consulta.prepare("INSERT INTO directorios (nombre, directorio) VALUES (?, ?) "
                     "ON DUPLICATE KEY UPDATE directorio = VALUES(directorio)");
    consulta.bindValue(0, it.key());
    consulta.bindValue(1, it.value());
    
    if (!consulta.exec()) {
      qWarning() << "Error al guardar directorio '" + it.key() + "':"
                 << consulta.lastError().text();
      QSqlDatabase::database(base).rollback();
      return false;
    }
  }
  
  return QSqlDatabase::database(base).commit();
}

QMap<QString, QString> baseDatos::cargarDirectorios(QString base) {
  QMap<QString, QString> resultado;
  QSqlQuery consulta(QSqlDatabase::database(base));
  if (!consulta.exec("SELECT nombre, directorio FROM directorios")) {
    qWarning() << "Error al cargar directorios:" << consulta.lastError().text();
    return resultado;
  }
  while (consulta.next()) {
    resultado.insert(consulta.value(0).toString(),
                     consulta.value(1).toString());
  }
  return resultado;
}

QString baseDatos::devolverDirectorio(QString tipo) {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.prepare("SELECT directorio FROM directorios WHERE nombre = ?");
  consulta.bindValue(0, tipo);
  consulta.exec();
  qDebug() << consulta.lastError();
  consulta.first();
  return consulta.record().value(0).toString();
}

QString baseDatos::resolverRutaImagen(QString nombreFoto) {
    if (nombreFoto.isEmpty()) return "";
    
    // Si la ruta ya es absoluta y el fichero existe, usarla tal cual
    if (QFileInfo(nombreFoto).isAbsolute() && QFile::exists(nombreFoto)) {
        return nombreFoto;
    }
    
    // Obtener el directorio base de imágenes (ej: ./imagenes o /ruta/absoluta/imagenes)
    QString baseDir = devolverDirectorio("imagenes");
    QDir d(baseDir);
    
    // Devolver la ruta absoluta combinando el directorio base y el nombre del fichero
    return d.absoluteFilePath(nombreFoto);
}

QString baseDatos::nombreConexionMaster() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT * FROM tiendas where master = '1'");
  if (consulta.numRowsAffected() < 1) {
    return "";
  }
  consulta.first();
  return consulta.record().value("nombre").toString();
}

QString baseDatos::nombreConexionLocal() {
  QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
  consulta.exec("SELECT * FROM tiendas where local = '1'");
  if (consulta.numRowsAffected() < 1) {
    return "DB";
  }
  consulta.first();
  return consulta.record().value("nombre").toString();
}

bool baseDatos::copiaSeguridad(QString base, QString nombre) {
  qDebug() << "Funcion copia de segurudad activada con el nombre: " + nombre;
  QStringList argumentos;
  argumentos << "--host=" + QSqlDatabase::database(base).hostName()
             << "--port=" + QString::number(QSqlDatabase::database(base).port())
             << "--user=" + QSqlDatabase::database(base).userName()
             << QSqlDatabase::database(base).databaseName();
  
  // Impresión en logs de depuración ocultando la contraseña para mayor seguridad (SEC-04)
  qDebug() << argumentos << "(--password=********)";

  // SEC-10: Objeto local en pila en vez de new para evitar memory leak
  QProcess process;

  // Se inyecta la contraseña de forma segura en las variables de entorno del proceso hijo (SEC-04)
  QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
  env.insert("MYSQL_PWD", QSqlDatabase::database(base).password());
  process.setProcessEnvironment(env);

  process.setStandardOutputFile(nombre);
  process.start("mysqldump", argumentos);
  if (process.waitForFinished(-1)) {
    qDebug() << "Compretada sin errores el backup de: " << nombre;
    return true;
  } else {
    qDebug() << "Backup error";
    qDebug() << process.errorString();
  }

  return false;
}

bool baseDatos::insertarLog(const QString db, const QString &categoria,
                            const QString &usuario, const QString &mensaje) {
  if (!QSqlDatabase::database(db).isOpen()) {
    qWarning() << "No se puede registrar log: Base de datos no conectada.";
    return false;
  }

  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare("INSERT INTO logs (categoria, usuario, mensaje) VALUES "
                "(:categoria, :usuario, :mensaje)");
  query.bindValue(":categoria", categoria);
  query.bindValue(":usuario", usuario);
  query.bindValue(":mensaje", mensaje);

  if (!query.exec()) {
    qWarning() << "Error al insertar log:" << query.lastError().text();
    return false;
  }
  return true;
}

QSqlQueryModel *baseDatos::consultarLog(const QString db,
                                        const QString &categoria,
                                        const QString &usuario,
                                        const QDateTime &desde,
                                        const QDateTime &hasta) {
  QString sql =
      "SELECT id, timestamp, categoria, usuario, mensaje FROM logs WHERE 1=1";

  if (!categoria.isEmpty())
    sql += " AND categoria = :categoria";

  if (!usuario.isEmpty())
    sql += " AND usuario = :usuario";

  if (desde.isValid())
    sql += " AND timestamp >= :desde";

  if (hasta.isValid())
    sql += " AND timestamp <= :hasta";

  sql += " ORDER BY timestamp DESC";

  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare(sql);

  if (!categoria.isEmpty())
    query.bindValue(":categoria", categoria);
  if (!usuario.isEmpty())
    query.bindValue(":usuario", usuario);
  if (desde.isValid())
    query.bindValue(":desde", desde.toString("yyyy-MM-dd HH:mm:ss"));
  if (hasta.isValid())
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd HH:mm:ss"));

  if (!query.exec()) {
    qWarning() << "Error al consultar logs:" << query.lastError().text();
    return nullptr;
  }

  QSqlQueryModel *model = new QSqlQueryModel();
  model->setQuery(query);
  model->setHeaderData(0, Qt::Horizontal, "ID");
  model->setHeaderData(1, Qt::Horizontal, "Fecha");
  model->setHeaderData(2, Qt::Horizontal, "Categoría");
  model->setHeaderData(3, Qt::Horizontal, "Usuario");
  model->setHeaderData(4, Qt::Horizontal, "Mensaje");

  return model;
}

QString baseDatos::obtenerUltimoHash(const QString db) {
  QSqlQuery query(QSqlDatabase::database(db));
  if (query.exec(
          "SELECT hash_actual FROM verifactu_logs ORDER BY id DESC LIMIT 1")) {
    if (query.next())
      return query.value(0).toString();
  }
  // Si no hay registros, devolver 64 ceros
  return QString(64, '0');
}

/**
 * @brief Obtiene el último hash VeriFactu con bloqueo FOR UPDATE.
 *
 * Garantiza que dos puestos no lean el mismo hash antes de grabar
 * su ticket, lo que rompería la cadena de integridad VeriFactu.
 *
 * @warning DEBE llamarse dentro de una transacción activa (db.transaction()).
 * @param db Conexión a la base de datos (debe tener una transacción abierta).
 * @return El último hash almacenado, o 64 ceros si no hay registros.
 */
QString baseDatos::obtenerUltimoHashAtomico(QSqlDatabase db) {
  QSqlQuery query(db);
  // FOR UPDATE bloquea la fila del último registro para que otro puesto
  // que intente leerla espere a que esta transacción termine.
  if (query.exec(
          "SELECT hash_actual FROM verifactu_logs ORDER BY id DESC LIMIT 1 FOR UPDATE")) {
    if (query.next())
      return query.value(0).toString();
  }
  // Si no hay registros, devolver 64 ceros
  return QString(64, '0');
}

QString baseDatos::registrarTickeckVerifactu(
    const QString db, const int ticket, const QString fecha, const QString hora,
    const QString hashActual, const QString hashAnterior,
    const QString datosFactura, const int ususario, const int estadoEnvio) {
  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare("INSERT INTO verifactu_logs (id_factura, fecha_hora, "
                "hash_actual, hash_anterior, "
                "cadena_firmada, usuario, estado_envio) "
                "VALUES (:ticket, :fecha_hora, :hash_actual, :hash_anterior, "
                ":cadena_firmada, :usuario, :estado_envio)");
  query.bindValue(":ticket", ticket);
  query.bindValue(":fecha_hora", fecha + " " + hora);
  query.bindValue(":hash_actual", hashActual);
  query.bindValue(":hash_anterior", hashAnterior);
  query.bindValue(":cadena_firmada", datosFactura);
  query.bindValue(":usuario", ususario); // campo usuario
  query.bindValue(":estado_envio", estadoEnvio);

  if (!query.exec()) {
    qWarning() << "Error al insertar en verifactu_logs:"
               << query.lastError().text();
    return "";
  }

  return hashActual;
}

QStringList baseDatos::datosTiendaLocal(QString db) {
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

// ============================================================
// FUNCIONES NOTAS
// ============================================================

bool baseDatos::crearNota(const QString &db, const QString &titulo,
                          const QString &descripcion, const QString &usuario,
                          const QString &fechaLimite,
                          const QString &prioridad) {
  if (!QSqlDatabase::database(db).isOpen())
    return false;
  QSqlQuery q(QSqlDatabase::database(db));
  q.prepare("INSERT INTO notas (titulo, descripcion, usuario, fecha_limite, "
            "estado, prioridad) "
            "VALUES (:titulo, :descripcion, :usuario, :fechaLimite, "
            "'Pendiente', :prioridad)");
  q.bindValue(":titulo", titulo);
  q.bindValue(":descripcion", descripcion);
  q.bindValue(":usuario", usuario);
  q.bindValue(":fechaLimite", fechaLimite.isEmpty() ? QVariant() : QVariant(fechaLimite));
  q.bindValue(":prioridad", prioridad.isEmpty() ? "Normal" : prioridad);
  if (!q.exec()) {
    qWarning() << "Error al crear nota:" << q.lastError().text();
    return false;
  }
  return true;
}

bool baseDatos::actualizarEstadoNota(const QString &db, int idNota,
                                     const QString &estado) {
  if (!QSqlDatabase::database(db).isOpen())
    return false;
  QSqlQuery q(QSqlDatabase::database(db));
  q.prepare("UPDATE notas SET estado = :estado WHERE id = :id");
  q.bindValue(":estado", estado);
  q.bindValue(":id", idNota);
  if (!q.exec()) {
    qWarning() << "Error al actualizar estado nota:" << q.lastError().text();
    return false;
  }
  return true;
}

bool baseDatos::actualizarPrioridadNota(const QString &db, int idNota,
                                        const QString &prioridad) {
  if (!QSqlDatabase::database(db).isOpen())
    return false;
  QSqlQuery q(QSqlDatabase::database(db));
  q.prepare("UPDATE notas SET prioridad = :prioridad WHERE id = :id");
  q.bindValue(":prioridad", prioridad);
  q.bindValue(":id", idNota);
  if (!q.exec()) {
    qWarning() << "Error al actualizar prioridad nota:" << q.lastError().text();
    return false;
  }
  return true;
}

bool baseDatos::editarNota(const QString &db, int idNota, const QString &titulo,
                           const QString &descripcion,
                           const QString &fechaLimite,
                           const QString &prioridad) {
  if (!QSqlDatabase::database(db).isOpen())
    return false;
  QSqlQuery q(QSqlDatabase::database(db));
  q.prepare(
      "UPDATE notas SET titulo = :titulo, descripcion = :descripcion, "
      "fecha_limite = :fechaLimite, prioridad = :prioridad WHERE id = :id");
  q.bindValue(":titulo", titulo);
  q.bindValue(":descripcion", descripcion);
  q.bindValue(":fechaLimite", fechaLimite.isEmpty() ? QVariant() : QVariant(fechaLimite));
  q.bindValue(":prioridad", prioridad);
  q.bindValue(":id", idNota);
  if (!q.exec()) {
    qWarning() << "Error al editar nota:" << q.lastError().text();
    return false;
  }
  return true;
}

bool baseDatos::eliminarNota(const QString &db, int idNota) {
  if (!QSqlDatabase::database(db).isOpen())
    return false;
  QSqlQuery q(QSqlDatabase::database(db));
  q.prepare("DELETE FROM notas WHERE id = :id");
  q.bindValue(":id", idNota);
  if (!q.exec()) {
    qWarning() << "Error al eliminar nota:" << q.lastError().text();
    return false;
  }
  return true;
}

QSqlQueryModel *baseDatos::consultarNotas(const QString &db,
                                          const QString &filtroEstado) {
  QString sql =
      "SELECT id, prioridad, titulo, usuario, "
      "DATE_FORMAT(fecha_creacion, '%d/%m/%Y %H:%i') AS fecha_creacion, "
      "IFNULL(DATE_FORMAT(fecha_limite, '%d/%m/%Y'), '') AS fecha_limite, "
      "estado, descripcion "
      "FROM notas";
  if (!filtroEstado.isEmpty() && filtroEstado != "Todas")
    sql += " WHERE estado = '" + filtroEstado + "'";
  sql +=
      " ORDER BY FIELD(prioridad,'Alta','Normal','Baja'), fecha_creacion DESC";

  QSqlQuery query(QSqlDatabase::database(db));
  query.exec(sql);
  if (query.lastError().isValid())
    qWarning() << "Error al consultar notas:" << query.lastError().text();

  QSqlQueryModel *model = new QSqlQueryModel();
  model->setQuery(query);
  model->setHeaderData(0, Qt::Horizontal, "ID");
  model->setHeaderData(1, Qt::Horizontal, "Prioridad");
  model->setHeaderData(2, Qt::Horizontal, "Título");
  model->setHeaderData(3, Qt::Horizontal, "Usuario");
  model->setHeaderData(4, Qt::Horizontal, "Creada");
  model->setHeaderData(5, Qt::Horizontal, "Límite");
  model->setHeaderData(6, Qt::Horizontal, "Estado");
  model->setHeaderData(7, Qt::Horizontal, "Descripción");
  return model;
}

int baseDatos::contarNotasPendientes(const QString &db) {
  if (!QSqlDatabase::database(db).isOpen())
    return 0;
  QSqlQuery q(QSqlDatabase::database(db));
  q.exec("SELECT COUNT(*) FROM notas WHERE estado = 'Pendiente'");
  if (q.first())
    return q.value(0).toInt();
  return 0;
}

// ============================================================
// FUNCIONES ESTADÍSTICAS
// ============================================================

QSqlQuery baseDatos::estadisticasVentasPorPeriodo(const QString &db,
                                                  const QDate &desde,
                                                  const QDate &hasta,
                                                  const QString &agrupacion,
                                                  bool consolidado) {
  QString formatoFechaSQL;
  if (agrupacion == "dia")
    formatoFechaSQL = "DATE(fecha)";
  else if (agrupacion == "mes")
    formatoFechaSQL = "DATE_FORMAT(fecha, '%Y-%m')";
  else if (agrupacion == "anio")
    formatoFechaSQL = "YEAR(fecha)";
  else
    formatoFechaSQL = "DATE(fecha)";

  // En modo consolidado se combinan tickets y ticketss con UNION ALL
  // y se agrupa el resultado en una subconsulta para sumar ambas fuentes
  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  QString fuenteSQL;
  if (reallyConsolidado) {
    fuenteSQL = QString("(SELECT fecha, total FROM tickets WHERE fecha BETWEEN "
                        ":desde AND :hasta "
                        " UNION ALL "
                        " SELECT fecha, total FROM ticketss WHERE fecha "
                        "BETWEEN :desde2 AND :hasta2) AS t_union");
  } else {
    fuenteSQL = "tickets WHERE fecha BETWEEN :desde AND :hasta";
  }

  QString sql;
  if (reallyConsolidado) {
    sql = QString("SELECT %1 AS periodo, SUM(total) AS total_ventas "
                  "FROM %2 "
                  "GROUP BY periodo "
                  "ORDER BY periodo ASC")
              .arg(formatoFechaSQL, fuenteSQL);
  } else {
    sql = QString("SELECT %1 AS periodo, SUM(total) AS total_ventas "
                  "FROM %2 "
                  "GROUP BY periodo "
                  "ORDER BY periodo ASC")
              .arg(formatoFechaSQL, fuenteSQL);
  }

  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasVentasPorPeriodo:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasVentasPorUsuario(const QString &db,
                                                  const QDate &desde,
                                                  const QDate &hasta,
                                                  bool consolidado) {
  QSqlQuery query(QSqlDatabase::database(db));

  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  // En modo consolidado se unen tickets y ticketss antes de agrupar por usuario
  QString sql;
  if (reallyConsolidado) {
    sql = "SELECT u.nombre, SUM(t.total) AS total "
          "FROM (SELECT usuario, total FROM tickets WHERE fecha BETWEEN :desde "
          "AND :hasta "
          "      UNION ALL "
          "      SELECT usuario, total FROM ticketss WHERE fecha BETWEEN "
          ":desde2 AND :hasta2) AS t "
          "JOIN usuarios u ON t.usuario = u.id "
          "GROUP BY u.nombre ORDER BY total DESC";
  } else {
    sql = "SELECT u.nombre, SUM(t.total) AS total "
          "FROM tickets t JOIN usuarios u ON t.usuario = u.id "
          "WHERE t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY u.nombre ORDER BY total DESC";
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasVentasPorUsuario:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasVentasPorFormaPago(const QString &db,
                                                    const QDate &desde,
                                                    const QDate &hasta,
                                                    bool consolidado) {
  QSqlQuery query(QSqlDatabase::database(db));

  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  // En modo consolidado se unen tickets y ticketss antes de agrupar por forma
  // de pago
  QString sql;
  if (reallyConsolidado) {
    sql = "SELECT fp.id , fp.tipo, SUM(t.total) AS total "
          "FROM (SELECT fpago, total FROM tickets WHERE fecha BETWEEN :desde "
          "AND :hasta "
          "      UNION ALL "
          "      SELECT fpago, total FROM ticketss WHERE fecha BETWEEN :desde2 "
          "AND :hasta2) AS t "
          "JOIN fpago fp ON t.fpago = fp.id "
          "GROUP BY fp.tipo ORDER BY total DESC";
  } else {
    sql = "SELECT fp.id , fp.tipo, SUM(t.total) AS total "
          "FROM tickets t JOIN fpago fp ON t.fpago = fp.id "
          "WHERE t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY fp.tipo ORDER BY total DESC";
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasVentasPorFormaPago:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasVentasPorFamilia(const QString &dbName,
                                                  const QDate &desde,
                                                  const QDate &hasta,
                                                  bool consolidado) {
  QSqlDatabase db = QSqlDatabase::database(dbName);
  QSqlQuery query(db);

  // Verificamos si realmente podemos usar el modo consolidado (si existen las tablas históricas)
  bool reallyConsolidado = consolidado && db.tables().contains("lineasticketss") && 
                           db.tables().contains("ticketss");

  QString sql;
  if (reallyConsolidado) {
    sql = "SELECT COALESCE(f.id, 0) AS id_fam, "
          "COALESCE(f.descripcion, 'SIN FAMILIA') AS nombre_fam, "
          "ROUND(SUM(COALESCE(ltu.cantidad, 0) * COALESCE(ltu.precio, 0)), 2) AS "
          "total "
          "FROM ("
          "  SELECT lt.cod, lt.cantidad, lt.precio FROM lineasticket lt "
          "  JOIN tickets t ON lt.nticket = t.ticket "
          "  WHERE t.fecha BETWEEN :desde AND :hasta "
          "  UNION ALL "
          "  SELECT lt2.cod, lt2.cantidad, lt2.precio FROM lineasticketss lt2 "
          "  JOIN ticketss t2 ON lt2.nticket = t2.ticket "
          "  WHERE t2.fecha BETWEEN :desde2 AND :hasta2 "
          ") AS ltu "
          "JOIN articulos a ON ltu.cod = a.cod "
          "LEFT JOIN familias f ON a.familia = f.id "
          "GROUP BY id_fam, nombre_fam ORDER BY total DESC";
  } else {
    sql = "SELECT COALESCE(f.id, 0) AS id_fam, "
          "COALESCE(f.descripcion, 'SIN FAMILIA') AS nombre_fam, "
          "ROUND(SUM(COALESCE(lt.cantidad, 0) * COALESCE(lt.precio, 0)), 2) AS "
          "total "
          "FROM lineasticket lt "
          "JOIN tickets t ON lt.nticket = t.ticket "
          "JOIN articulos a ON lt.cod = a.cod "
          "LEFT JOIN familias f ON a.familia = f.id "
          "WHERE t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY id_fam, nombre_fam ORDER BY total DESC";
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasVentasPorFamilia:"
             << query.lastError().text();
  return query;
}


QSqlQuery baseDatos::estadisticasTopArticulosVendidos(const QString &db,
                                                      const QDate &desde,
                                                      const QDate &hasta,
                                                      int limite,
                                                      bool consolidado) {
  // NOTA: MySQL no permite LIMIT con parámetros vinculados (bindValue),
  // por lo que se inyecta el valor como literal en la cadena SQL.
  QSqlQuery query(QSqlDatabase::database(db));
  QString sql;

  bool reallyConsolidado = consolidado && 
                           QSqlDatabase::database(db).tables().contains("ticketss") &&
                           QSqlDatabase::database(db).tables().contains("lineasticketss");

  if (reallyConsolidado) {
      sql = QString(
                "SELECT lt.cod, lt.descripcion, SUM(lt.cantidad) AS cantidad_total "
                "FROM (SELECT nticket, cod, descripcion, cantidad FROM lineasticket "
                "      UNION ALL "
                "      SELECT nticket, cod, descripcion, cantidad FROM lineasticketss) lt "
                "JOIN (SELECT ticket, fecha FROM tickets WHERE fecha BETWEEN :desde AND :hasta "
                "      UNION ALL "
                "      SELECT ticket, fecha FROM ticketss WHERE fecha BETWEEN :desde2 AND :hasta2) t "
                "ON lt.nticket = t.ticket "
                "GROUP BY lt.cod, lt.descripcion ORDER BY cantidad_total DESC "
                "LIMIT %1")
                .arg(limite);
  } else {
      sql = QString(
                "SELECT lt.cod, lt.descripcion, SUM(lt.cantidad) AS cantidad_total "
                "FROM lineasticket lt "
                "JOIN tickets t ON lt.nticket = t.ticket "
                "WHERE t.fecha BETWEEN :desde AND :hasta "
                "GROUP BY lt.cod, lt.descripcion ORDER BY cantidad_total DESC "
                "LIMIT %1")
                .arg(limite);
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasTopArticulosVendidos:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasTopArticulosRentables(const QString &db,
                                                       const QDate &desde,
                                                       const QDate &hasta,
                                                       int limite,
                                                       bool consolidado) {
  // NOTA: MySQL no permite LIMIT con parámetros vinculados (bindValue),
  // por lo que se inyecta el valor como literal en la cadena SQL.
  QSqlQuery query(QSqlDatabase::database(db));
  QString sql;

  bool reallyConsolidado = consolidado && 
                           QSqlDatabase::database(db).tables().contains("ticketss") &&
                           QSqlDatabase::database(db).tables().contains("lineasticketss");

  if (reallyConsolidado) {
      sql = QString(
          "SELECT lt.cod, lt.descripcion, ROUND(SUM(lt.cantidad * (lt.precio - "
          "COALESCE(a.precio_compra,0))), 2) AS rentabilidad "
          "FROM (SELECT nticket, cod, descripcion, cantidad, precio FROM lineasticket "
          "      UNION ALL "
          "      SELECT nticket, cod, descripcion, cantidad, precio FROM lineasticketss) lt "
          "JOIN (SELECT ticket, fecha FROM tickets WHERE fecha BETWEEN :desde AND :hasta "
          "      UNION ALL "
          "      SELECT ticket, fecha FROM ticketss WHERE fecha BETWEEN :desde2 AND :hasta2) t "
          "ON lt.nticket = t.ticket "
          "LEFT JOIN articulos a ON lt.cod = a.cod "
          "GROUP BY lt.cod, lt.descripcion ORDER BY rentabilidad DESC LIMIT %1")
          .arg(limite);
  } else {
      sql = QString(
          "SELECT lt.cod, lt.descripcion, ROUND(SUM(lt.cantidad * (lt.precio - "
          "COALESCE(a.precio_compra,0))), 2) AS rentabilidad "
          "FROM lineasticket lt JOIN tickets t ON lt.nticket = t.ticket "
          "LEFT JOIN articulos a ON lt.cod = a.cod "
          "WHERE t.fecha BETWEEN :desde AND :hasta "
          "GROUP BY lt.cod, lt.descripcion ORDER BY rentabilidad DESC LIMIT %1")
          .arg(limite);
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasTopArticulosRentables:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasArticulosSinMovimiento(const QString &db,
                                                        int diasSinVenta) {
  // Devuelve los artículos que no se han vendido en los últimos X días o nunca
  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare("SELECT cod, descripcion, stock, ultima_venta "
                "FROM articulos "
                "WHERE ultima_venta IS NULL OR DATEDIFF(CURRENT_DATE(), "
                "ultima_venta) >= :dias "
                "ORDER BY ultima_venta ASC, stock DESC");
  query.bindValue(":dias", diasSinVenta);
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasArticulosSinMovimiento:"
             << query.lastError().text();
  return query;
}

QSqlQuery baseDatos::estadisticasMejoresClientes(const QString &db,
                                                 const QDate &desde,
                                                 const QDate &hasta, int limite,
                                                 bool consolidado) {
  // NOTA: MySQL no permite LIMIT con parámetros vinculados (bindValue),
  // por lo que se inyecta el valor como literal en la cadena SQL.
  QSqlQuery query(QSqlDatabase::database(db));
  QString sql;

  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  // En modo consolidado se suman las compras de tickets y ticketss por cliente
  if (reallyConsolidado) {
    sql = QString("SELECT c.nif, CONCAT(c.nombre, ' ', c.apellidos) AS "
                  "nombre_completo, SUM(t.total) AS total_compras "
                  "FROM ("
                  "  SELECT cliente, total FROM tickets "
                  "  WHERE fecha BETWEEN :desde AND :hasta AND cliente != 0 "
                  "AND cliente IS NOT NULL "
                  "  UNION ALL "
                  "  SELECT cliente, total FROM ticketss "
                  "  WHERE fecha BETWEEN :desde2 AND :hasta2 AND cliente != 0 "
                  "AND cliente IS NOT NULL "
                  ") AS t "
                  "JOIN clientes c ON t.cliente = c.idCliente "
                  "GROUP BY c.idCliente ORDER BY total_compras DESC LIMIT %1")
              .arg(limite);
  } else {
    sql = QString("SELECT c.nif, CONCAT(c.nombre, ' ', c.apellidos) AS "
                  "nombre_completo, SUM(t.total) AS total_compras "
                  "FROM tickets t JOIN clientes c ON t.cliente = c.idCliente "
                  "WHERE t.fecha BETWEEN :desde AND :hasta AND t.cliente != 0 "
                  "AND t.cliente IS NOT NULL "
                  "GROUP BY c.idCliente ORDER BY total_compras DESC LIMIT %1")
              .arg(limite);
  }

  query.prepare(sql);
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (reallyConsolidado) {
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  }
  query.exec();
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasMejoresClientes:"
             << query.lastError().text();
  return query;
}

double baseDatos::estadisticasTotalVentas(const QString &db, const QDate &desde,
                                          const QDate &hasta,
                                          bool consolidado) {
  QSqlQuery query(QSqlDatabase::database(db));

  // En modo consolidado sumamos el total de tickets y ticketss
  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  if (reallyConsolidado) {
    query.prepare(
        "SELECT COALESCE(SUM(total), 0) FROM "
        "(SELECT total FROM tickets WHERE fecha BETWEEN :desde AND :hasta "
        " UNION ALL "
        " SELECT total FROM ticketss WHERE fecha BETWEEN :desde2 AND :hasta2) "
        "AS t_union");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  } else {
    query.prepare("SELECT COALESCE(SUM(total), 0) FROM tickets WHERE fecha "
                  "BETWEEN :desde AND :hasta");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  }

  if (query.exec() && query.first()) {
    return query.value(0).toDouble();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasTotalVentas:" << query.lastError().text();
  return 0.0;
}

int baseDatos::estadisticasNumeroTickets(const QString &db, const QDate &desde,
                                         const QDate &hasta, bool consolidado) {
  QSqlQuery query(QSqlDatabase::database(db));

  // En modo consolidado contamos tickets y ticketss
  bool reallyConsolidado = consolidado && QSqlDatabase::database(db).tables().contains("ticketss");

  if (reallyConsolidado) {
    query.prepare(
        "SELECT COUNT(*) FROM "
        "(SELECT ticket FROM tickets WHERE fecha BETWEEN :desde AND :hasta "
        " UNION ALL "
        " SELECT ticket FROM ticketss WHERE fecha BETWEEN :desde2 AND :hasta2) "
        "AS t_union");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  } else {
    query.prepare(
        "SELECT COUNT(*) FROM tickets WHERE fecha BETWEEN :desde AND :hasta");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  }

  if (query.exec() && query.first()) {
    return query.value(0).toInt();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasNumeroTickets:"
             << query.lastError().text();
  return 0;
}

int baseDatos::estadisticasTotalArticulosStock(const QString &db) {
  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare(
      "SELECT COALESCE(SUM(cantidad), 0) FROM lotes"); // El stock real es la
                                                       // suma de lotes
  if (query.exec() && query.first()) {
    return query.value(0).toInt();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasTotalArticulosStock:"
             << query.lastError().text();
  return 0;
}

int baseDatos::estadisticasClientesActivos(const QString &db,
                                           const QDate &desde,
                                           const QDate &hasta,
                                           bool consolidado) {
  QSqlQuery query(QSqlDatabase::database(db));

  // En modo consolidado cuenta los clientes distintos de tickets y ticketss
  if (consolidado) {
    query.prepare(
        "SELECT COUNT(DISTINCT cliente) FROM "
        "(SELECT cliente FROM tickets WHERE fecha BETWEEN :desde AND :hasta "
        "AND cliente IS NOT NULL AND cliente > 0 "
        " UNION ALL "
        " SELECT cliente FROM ticketss WHERE fecha BETWEEN :desde2 AND :hasta2 "
        "AND cliente IS NOT NULL AND cliente > 0) AS t_union");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
    query.bindValue(":desde2", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta2", hasta.toString("yyyy-MM-dd"));
  } else {
    query.prepare(
        "SELECT COUNT(DISTINCT cliente) FROM tickets WHERE fecha BETWEEN "
        ":desde AND :hasta AND cliente IS NOT NULL AND cliente > 0");
    query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
    query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  }

  if (query.exec() && query.first()) {
    return query.value(0).toInt();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasClientesActivos:"
             << query.lastError().text();
  return 0;
}

double baseDatos::estadisticasTotalCompras(const QString &db,
                                           const QDate &desde,
                                           const QDate &hasta) {
  QSqlQuery query(QSqlDatabase::database(db));
  // Asumiendo que albaranes tiene total o algo similar. Usamos
  // base_datos.sumarBasesPedido Si la tabla facturas o pedidos tiene el total,
  // lo sumaremos de ahí. Verifiquemos si existe 'total' en albaranes o facturas
  query.prepare("SELECT COALESCE(SUM(total), 0) FROM facturas WHERE "
                "fechaFactura BETWEEN :desde AND :hasta");
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (query.exec() && query.first()) {
    return query.value(0).toDouble();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasTotalCompras:"
             << query.lastError().text();
  return 0.0;
}

int baseDatos::estadisticasNumeroPedidos(const QString &db, const QDate &desde,
                                         const QDate &hasta) {
  QSqlQuery query(QSqlDatabase::database(db));
  query.prepare("SELECT COUNT(*) FROM pedidos WHERE fechaPedido BETWEEN :desde "
                "AND :hasta");
  query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
  query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
  if (query.exec() && query.first()) {
    return query.value(0).toInt();
  }
  if (query.lastError().isValid())
    qDebug() << "Error en estadisticasNumeroPedidos:"
             << query.lastError().text();
  return 0;
}
