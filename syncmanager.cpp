#include "syncmanager.h"


#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QTimer>
#include <QDateTime>
#include "unificarmaestros.h"
#include <QTcpSocket>
#include "configuracion.h"

extern Configuracion *conf;

// ─────────────────────────────────────────────────────────────────────────────
// Constantes
// ─────────────────────────────────────────────────────────────────────────────

/// Nombre de la conexión Qt para la BD en la nube (no "DB", que es la local)
const QString SyncManager::CONEXION_NUBE = "NUBE";

/// Tablas maestras cuyo contenido se comparte entre todas las tiendas.
const QStringList SyncManager::TABLAS_MAESTRAS = {
    "familias",
    "fabricantes",
    "proveedores",
    "fpago",
    "impuestos",
    "formatos",
    "motivosEntrada",
    "articulos",    // depende de familias y fabricantes
    "clientes",
    "codaux",       // depende de articulos
    "usuarios",
    "permisos",
    "vales"         // vales de fidelidad (estado se propaga via nube)
};

const QMap<QString, QStringList> SyncManager::CAMPOS_EXCLUIDOS = {
    {"articulos", {"stock", "min", "max", "pendientes_pedido", "ultima_venta", "ultimo_pedido", "encargados", "minimo_pedido"}},
    {"vales", {"idvales"}}
};

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

SyncManager::SyncManager(QObject *parent)
    : QObject(parent), m_timerPing(new QTimer(this)), m_timerSync(new QTimer(this)), 
      m_hayConexion(false), m_idTiendaLocal(0)
{
    // Comprobación de conexión cada 30 segundos
    m_timerPing->setInterval(30 * 1000);
    connect(m_timerPing, &QTimer::timeout, this, &SyncManager::comprobarConexion);

    // Sincronización cada 5 minutos
    m_timerSync->setInterval(5 * 60 * 1000);
    connect(m_timerSync, &QTimer::timeout, this, &SyncManager::sincronizar);

    // Intentar cargar el ID de la tienda local al inicio
    cargarIdTiendaLocal();
}

SyncManager::~SyncManager()
{
    desconectarNube();
}

void SyncManager::iniciar()
{
    crearTablasSyncLocal();
    crearTriggers();
    m_timerPing->start();
    m_timerSync->start();
    comprobarConexion();
    qDebug() << "SyncManager: iniciado correctamente";
}

void SyncManager::crearTablasSyncLocal()
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery q(db);

    q.exec("CREATE TABLE IF NOT EXISTS sync_cola ("
           "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
           "  tabla       VARCHAR(64)  NOT NULL,"
           "  id_registro VARCHAR(64)  NOT NULL,"
           "  accion      ENUM('INSERT','UPDATE','DELETE') NOT NULL,"
           "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,"
           "  subido      TINYINT(1)   NOT NULL DEFAULT 0"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

    q.exec("CREATE TABLE IF NOT EXISTS sync_control ("
           "  tabla       VARCHAR(64) PRIMARY KEY,"
           "  ultima_sync DATETIME    NOT NULL DEFAULT '2000-01-01 00:00:00'"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

    for (const QString &tabla : TABLAS_MAESTRAS) {
        q.exec(QString("ALTER TABLE `%1` ADD COLUMN IF NOT EXISTS id_tienda_origen INT DEFAULT NULL").arg(tabla));
        q.exec(QString("ALTER TABLE `%1` ADD COLUMN IF NOT EXISTS updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP "
                       "ON UPDATE CURRENT_TIMESTAMP").arg(tabla));
        
        // Comprobar por nombre de índice exacto para evitar duplicados.
        // Buscar por index_name (no por column_name) para no confundirse con
        // otros índices que usen la misma columna (p.ej. PRIMARY, UNIQUE).
        QString checkIdx = QString("SELECT COUNT(*) FROM information_schema.statistics "
                                   "WHERE table_schema = (SELECT DATABASE()) AND table_name = '%1' "
                                   "AND index_name = 'idx_%1_updated'").arg(tabla);
        if (q.exec(checkIdx) && q.next() && q.value(0).toInt() == 0) {
            q.exec(QString("ALTER TABLE `%1` ADD INDEX idx_%1_updated (updated_at)").arg(tabla));
        }
        
        q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES (?)");
        q.addBindValue(tabla);
        q.exec();
    }

    // ── Migración: vale_uuid como identificador global único para vales ──────
    // Evita colisiones de idvales AUTO_INCREMENT entre tiendas al sincronizar.
    q.exec("ALTER TABLE `vales` ADD COLUMN IF NOT EXISTS "
           "vale_uuid CHAR(36) NOT NULL DEFAULT ''");
    // Rellenar UUID en los vales existentes que lo tengan vacío
    q.exec("UPDATE vales SET vale_uuid = UUID() WHERE vale_uuid = '' OR vale_uuid IS NULL");
    // Índice único para garantizar unicidad en la nube
    q.exec("SELECT COUNT(*) FROM information_schema.statistics "
           "WHERE table_schema = DATABASE() AND table_name = 'vales' "
           "AND index_name = 'uq_vale_uuid'");
    if (q.next() && q.value(0).toInt() == 0) {
        q.exec("ALTER TABLE vales ADD UNIQUE INDEX uq_vale_uuid (vale_uuid)");
    }

    q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES ('sync_unificaciones')");
    q.exec();

    q.exec("CREATE TABLE IF NOT EXISTS sync_unificaciones ("
           "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
           "  tabla       VARCHAR(64)  NOT NULL,"
           "  id_perdedor VARCHAR(64)  NOT NULL,"
           "  id_ganador  VARCHAR(64)  NOT NULL,"
           "  subido      TINYINT(1)   NOT NULL DEFAULT 0,"
           "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");
}

void SyncManager::crearTriggers()
{
    for (const QString &tabla : TABLAS_MAESTRAS) {
        QString pk = getPkTabla(tabla);
        if (pk.isEmpty()) continue;
        crearTrigger(tabla, pk, "INSERT");
        crearTrigger(tabla, pk, "UPDATE");
        crearTrigger(tabla, pk, "DELETE");
    }
    
    // Limpieza: eliminar triggers de tablas que ya no se sincronizan (como 'directorios')
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.exec("DROP TRIGGER IF EXISTS sync_directorios_insert");
    q.exec("DROP TRIGGER IF EXISTS sync_directorios_update");
    q.exec("DROP TRIGGER IF EXISTS sync_directorios_delete");
}

void SyncManager::crearTrigger(const QString &nombreTabla, const QString &clavePrimaria, const QString &evento)
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery q(db);
    QString nombreTrigger = QString("sync_%1_%2").arg(nombreTabla).arg(evento.toLower());
    q.exec(QString("DROP TRIGGER IF EXISTS %1").arg(nombreTrigger));

    QString prefijo = (evento == "DELETE") ? "OLD" : "NEW";
    QString sql = QString(
        "CREATE TRIGGER %1 AFTER %2 ON %3 FOR EACH ROW BEGIN "
        "  IF @skip_sync IS NULL OR @skip_sync = 0 THEN "
        "    INSERT INTO sync_cola (tabla, id_registro, accion) VALUES ('%3', %4.%5, '%2'); "
        "  END IF; "
        "END"
    ).arg(nombreTrigger, evento, nombreTabla, prefijo, clavePrimaria);
    q.exec(sql);
}

bool SyncManager::conectarNube()
{
    if (QSqlDatabase::database(CONEXION_NUBE).isOpen()) return true;

    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.exec("SELECT servidor, puerto, baseDatos, usuario, clave, ssl_ca FROM config_nube WHERE id = 1");
    if (!q.first()) return false;

    QSqlDatabase dbNube = QSqlDatabase::addDatabase("QMYSQL", CONEXION_NUBE);
    dbNube.setHostName(q.value(0).toString());
    dbNube.setPort(q.value(1).toInt() > 0 ? q.value(1).toInt() : 3306);
    dbNube.setDatabaseName(q.value(2).toString());
    dbNube.setUserName(q.value(3).toString());
    dbNube.setPassword(q.value(4).toString());

    QString sslCa = q.value(5).toString();
    if (!sslCa.isEmpty()) {
        if (QDir::isRelativePath(sslCa)) sslCa = QCoreApplication::applicationDirPath() + "/" + sslCa;
        dbNube.setConnectOptions("SSL_CA=" + sslCa + ";MYSQL_OPT_CONNECT_TIMEOUT=2");
    } else {
        dbNube.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=2");
    }

    if (!dbNube.open()) {
        QSqlDatabase::removeDatabase(CONEXION_NUBE);
        return false;
    }
    
    // Forzar zona horaria UTC para que los updated_at sean universales
    QSqlQuery qTZ(dbNube);
    qTZ.exec("SET time_zone = '+00:00'");
    
    // Verificar conexión real
    qTZ.exec("SELECT DATABASE()");
    if (qTZ.next()) {
        qDebug() << "SyncManager: Conectado a" << dbNube.hostName() << "Base de Datos:" << qTZ.value(0).toString();
    }
    
    m_hayConexion = true;
    return true;
}

void SyncManager::desconectarNube()
{
    if (QSqlDatabase::contains(CONEXION_NUBE)) {
        QSqlDatabase::database(CONEXION_NUBE).close();
        QSqlDatabase::removeDatabase(CONEXION_NUBE);
    }
    m_hayConexion = false;
}

void SyncManager::comprobarConexion()
{
    bool anterior = m_hayConexion;
    
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.exec("SELECT servidor, puerto FROM config_nube WHERE id = 1");
    if (!q.first()) {
        if (anterior) emit conexionPerdida();
        desconectarNube();
        return;
    }
    
    QString host = q.value(0).toString();
    int port = q.value(1).toInt() > 0 ? q.value(1).toInt() : 3306;

    if (!anterior) {
        // Solo hacemos el ping TCP si no estamos conectados, para evitar bloqueos largos de la UI
        // si la BD está apagada. Si este ping tiene éxito, conectarNube() establecerá
        // una conexión real que reseteará el contador de errores de conexión (max_connect_errors) en MariaDB.
        QTcpSocket socket;
        socket.connectToHost(host, port);
        if (!socket.waitForConnected(1500)) {
            // Sigue sin haber conexión o el puerto está cerrado, no bloqueamos la UI
            return;
        }
        socket.disconnectFromHost();

        desconectarNube();
        if (conectarNube()) {
            prepararTablasRemotas();
            emit conexionRecuperada();
            // Ejecutamos la primera sincronizacion despues de recuperar
            sincronizar();
        }
    } else {
        // Ya estabamos conectados; hacer ping rápido a la BD a través de la conexión existente.
        // Esto NO incrementa el contador de errores de conexión de MariaDB.
        QSqlDatabase dbNube = QSqlDatabase::database(CONEXION_NUBE);
        if (dbNube.isOpen()) {
            QSqlQuery qPing(dbNube);
            if (!qPing.exec("SELECT 1")) {
                desconectarNube();
                if (!conectarNube()) {
                    emit conexionPerdida();
                } else {
                    prepararTablasRemotas();
                }
            }
        } else {
            if (!conectarNube()) {
                emit conexionPerdida();
            } else {
                prepararTablasRemotas();
            }
        }
    }
}

void SyncManager::sincronizar()
{
    if (!m_hayConexion && !conectarNube()) {
        qDebug() << "SyncManager: Sincronización cancelada (sin conexión a la nube)";
        return;
    }

    qDebug() << "SyncManager: Iniciando ciclo de sincronización..." << QDateTime::currentDateTime().toString("HH:mm:ss");
    
    int subidos = subirCambios();
    int bajados = bajarCambios();
    
    qDebug() << "SyncManager: Ciclo completado — Registros subidos:" << subidos << "| Registros bajados:" << bajados;
    emit syncCompletado(subidos, bajados);
}

int SyncManager::subirCambios()
{
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    QSqlDatabase dbNube  = QSqlDatabase::database(CONEXION_NUBE);
    QSqlQuery cola(dbLocal);
    cola.exec("SELECT id, tabla, id_registro, accion FROM sync_cola WHERE subido = 0 ORDER BY fecha ASC");

    int subidos = 0;
    QList<qint64> procesados;
    while (cola.next()) {
        qint64 idCola = cola.value(0).toLongLong();
        QString tabla = cola.value(1).toString();
        QString idReg = cola.value(2).toString();
        QString accion = cola.value(3).toString();
        bool ok = false;

        if (accion == "DELETE") {
            QString pk = getPkTabla(tabla);
            QSqlQuery del(dbNube);
            del.prepare(QString("DELETE FROM `%1` WHERE `%2` = ?").arg(tabla, pk));
            del.addBindValue(idReg);
            ok = del.exec();
        } else {
            QString pk = getPkTabla(tabla);
            QSqlQuery reg(dbLocal);
            // Usar consulta directa para evitar el bug del protocolo binario de MySQL/MariaDB
            // en Qt 6, el cual lee los campos de fecha como QDateTime(Invalid) al usar prepare().
            QString sqlReg = QString("SELECT * FROM `%1` WHERE `%2` = '%3'")
                             .arg(tabla, pk, QString(idReg).replace("'", "''"));
            if (reg.exec(sqlReg) && reg.first()) {
                QSqlRecord rec = reg.record();
                QStringList campos, valores, updates;
                QStringList excluidos = CAMPOS_EXCLUIDOS.value(tabla);
                for (int i = 0; i < rec.count(); ++i) {
                    QString campo = rec.fieldName(i);
                    if (excluidos.contains(campo.toLower())) continue;
                    if (campo.toLower() == "id_tienda_origen" && (rec.value(i).isNull() || rec.value(i).toInt() == 0)) {
                        campos << "`id_tienda_origen`"; valores << QString::number(m_idTiendaLocal);
                        updates << "`id_tienda_origen` = VALUES(`id_tienda_origen`)"; continue;
                    }
                    QVariant val = rec.value(i);
                    QString vStr = "NULL";
                    if (!val.isNull()) {
                        if (val.userType() == QMetaType::QDateTime || val.userType() == QMetaType::QDate) {
                            QDateTime dt = val.toDateTime();
                            // Si la fecha no es válida (p.ej. '0000-00-00'), la enviamos como NULL
                            // para evitar errores de sintaxis/formato incorrecto en la base de datos.
                            if (dt.isValid()) {
                                vStr = "'" + dt.toString("yyyy-MM-dd HH:mm:ss") + "'";
                            } else {
                                vStr = "NULL";
                            }
                        } else {
                            vStr = "'" + val.toString().replace("'", "''") + "'";
                        }
                    }
                    campos << "`" + campo + "`"; valores << vStr;
                    updates << QString("`%1` = VALUES(`%1`)").arg(campo);
                }
                QString sql = QString("INSERT INTO `%1` (%2) VALUES (%3) ON DUPLICATE KEY UPDATE %4")
                              .arg(tabla, campos.join(","), valores.join(","), updates.join(","));
                QSqlQuery ins(dbNube);
                ok = ins.exec(sql);
                if (!ok) {
                    qDebug() << "SyncManager: Error subiendo a" << tabla << ":" << ins.lastError().text();
                    qDebug() << "SQL fallido:" << sql;
                }
            } else ok = true;
        }
        if (ok) { procesados << idCola; subidos++; }
    }
    if (!procesados.isEmpty()) {
        QStringList sIds; for (qint64 id : procesados) sIds << QString::number(id);
        QSqlQuery(dbLocal).exec("UPDATE sync_cola SET subido = 1 WHERE id IN (" + sIds.join(",") + ")");
    }
    return subidos;
}

int SyncManager::bajarCambios()
{
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    QSqlDatabase dbNube  = QSqlDatabase::database(CONEXION_NUBE);
    int bajados = 0;
    QDateTime ahora = QDateTime::currentDateTime();

    QDateTime maxUpdate = ultimaSync("sync_unificaciones");
    QSqlQuery qNU(dbNube);
    // Usar consulta directa para evitar el bug del protocolo binario con QDateTime(Invalid) en prepared statements.
    QString sqlU = QString("SELECT tabla, id_perdedor, id_ganador, fecha FROM sync_unificaciones WHERE fecha > '%1' ORDER BY fecha ASC")
                   .arg(maxUpdate.toString("yyyy-MM-dd HH:mm:ss"));
    
    if (qNU.exec(sqlU)) {
        while (qNU.next()) {
            QString t = qNU.value(0).toString();
            QString p = qNU.value(1).toString();
            QString g = qNU.value(2).toString();
            QDateTime f = qNU.value(3).toDateTime();

            UnificarMaestrosConfig cfg = UnificarMaestrosConfig::configParaTabla(t);
            if (cfg.tablaMaestra.isEmpty()) continue;

            dbLocal.transaction();
            QSqlQuery qL(dbLocal);
            bool ok = true;
            for (const auto &dep : cfg.dependencias) {
                QString sqlU = QString("UPDATE %1 SET %2 = '%3' WHERE %2 = '%4'").arg(dep.tabla, dep.campo, g, p);
                if (!qL.exec(sqlU)) { ok = false; break; }
            }
            if (ok) ok = qL.exec(QString("DELETE FROM %1 WHERE %2 = '%3'").arg(cfg.tablaMaestra, cfg.campoId, p));

            if (ok) dbLocal.commit();
            else dbLocal.rollback();
            
            if (f > maxUpdate) maxUpdate = f;
        }
        actualizarUltimaSync("sync_unificaciones", maxUpdate);
    }

    for (const QString &tabla : TABLAS_MAESTRAS) {
        QDateTime ultimaSyncActual = ultimaSync(tabla);
        QDateTime desde = ultimaSyncActual.addSecs(-120); // 2 min overlap
        QSqlQuery qN(dbNube);
        
        qDebug() << "   -> Consultando tabla:" << tabla << "desde" << desde.toString("yyyy-MM-dd HH:mm:ss");

        // Usar consulta directa para evitar el bug del protocolo binario de MariaDB que devuelve QDateTime(Invalid)
        QString sqlN = QString("SELECT * FROM `%1` WHERE updated_at > '%2' ORDER BY updated_at ASC")
                       .arg(tabla, desde.toUTC().toString("yyyy-MM-dd HH:mm:ss"));
        if (!qN.exec(sqlN)) {
            qDebug() << "SyncManager: Error consultando tabla" << tabla << "en la nube:" << qN.lastError().text();
            continue;
        }

        QString pk = getPkTabla(tabla);
        QStringList excluidos = CAMPOS_EXCLUIDOS.value(tabla);
        QSqlRecord recLoc = dbLocal.record(tabla);
        QStringList avisosPrecios;
        QDateTime maxUpdate = desde;

        QSqlQuery(dbLocal).exec("SET @skip_sync = 1");
        while (qN.next()) {
            QSqlRecord rec = qN.record();
            QString idReg = rec.value(pk).toString();
            QDateTime recUpd = rec.value("updated_at").toDateTime();

            // Variables para alerta de precios
            double pOld = 0;
            bool existeLoc = false;
            if (tabla == "articulos") {
                QSqlQuery qP(dbLocal);
                qP.prepare("SELECT pvp FROM articulos WHERE cod = ?");
                qP.addBindValue(idReg);
                if (qP.exec() && qP.first()) {
                    pOld = qP.value(0).toDouble();
                    existeLoc = true;
                }
            }

            QStringList campos, valores, updates;
            for (int i = 0; i < rec.count(); ++i) {
                QString campo = rec.fieldName(i);
                
                // NUNCA sincronizar el ID físico local ni la PK aquí (se añade luego)
                if (campo == pk || campo.toLower() == "id" + tabla.toLower()) continue;
                if (recLoc.indexOf(campo) == -1 || excluidos.contains(campo.toLower())) continue;
                
                QVariant val = rec.value(i);
                QString vStr = "NULL";
                if (!val.isNull()) {
                    if (val.userType() == QMetaType::QDateTime || val.userType() == QMetaType::QDate) {
                        QDateTime dt = val.toDateTime();
                        // Evitar que fechas no válidas (como '0000-00-00') generen un string vacío
                        // y provoquen un error de "Incorrect date value: ''" al insertar en la base de datos local.
                        if (dt.isValid()) {
                            vStr = "'" + dt.toString("yyyy-MM-dd HH:mm:ss") + "'";
                        } else {
                            vStr = "NULL";
                        }
                    } else {
                        vStr = "'" + val.toString().replace("'", "''") + "'";
                    }
                }
                campos << "`" + campo + "`"; valores << vStr;
                updates << QString("`%1` = VALUES(`%1`)").arg(campo);
            }
            
            campos << "`" + pk + "`";
            valores << "'" + idReg.replace("'", "''") + "'";

            QString sql = QString("INSERT INTO `%1` (%2) VALUES (%3) ON DUPLICATE KEY UPDATE %4")
                          .arg(tabla, campos.join(","), valores.join(","), updates.join(","));
            
            QSqlQuery qIns(dbLocal);
            if (qIns.exec(sql)) {
                if (recUpd > maxUpdate) maxUpdate = recUpd;
                if (recUpd > ultimaSyncActual) {
                    bajados++;
                }
                if (tabla == "articulos" && existeLoc) {
                    double pNew = rec.value("pvp").toDouble();
                    if (qAbs(pOld - pNew) > 0.01) {
                        int idOrigen = rec.value("id_tienda_origen").toInt();
                        QString tiendaName = "Desconocida";
                        if (idOrigen > 0) {
                            QSqlQuery qTienda(dbLocal);
                            qTienda.prepare("SELECT nombre FROM tiendas WHERE id = ?");
                            qTienda.addBindValue(idOrigen);
                            if (qTienda.exec() && qTienda.next()) {
                                tiendaName = qTienda.value(0).toString();
                            } else {
                                tiendaName = QString("Tienda %1").arg(idOrigen);
                            }
                        }
                        avisosPrecios << QString("- %1: %2 -> %3 (desde %4)")
                                             .arg(rec.value("descripcion").toString())
                                             .arg(pOld)
                                             .arg(pNew)
                                             .arg(tiendaName);
                    }
                }
            } else {
                qDebug() << "SyncManager: ERROR bajando" << tabla << ":" << qIns.lastError().text();
            }
        }
        if (!avisosPrecios.isEmpty()) {
            qDebug() << "   [i] Creando nota de aviso para" << avisosPrecios.size() << "cambios de precio...";
            QSqlQuery qNot(dbLocal);
            // El esquema real usa enum('Alta','Normal','Baja') para prioridad
            // y enum('Pendiente','Completada') para estado.
            qNot.prepare("INSERT INTO notas (titulo, descripcion, usuario, fecha_limite, prioridad, estado) VALUES (?,?,?,?,?,?)");
            qNot.addBindValue("Cambio de precios nube");
            qNot.addBindValue("Nuevos precios:\n" + avisosPrecios.join("\n"));
            qNot.addBindValue("SISTEMA"); 
            qNot.addBindValue(ahora.toString("yyyy-MM-dd")); // fecha_limite es DATE
            qNot.addBindValue("Alta"); 
            qNot.addBindValue("Pendiente");
            
            if (!qNot.exec()) {
                qWarning() << "   [!] Error al crear nota de precios:" << qNot.lastError().text();
            } else {
                qDebug() << "   [+] Nota de aviso creada correctamente.";
                avisosPrecios.clear(); // Limpiar para que no se repita en la siguiente tabla si hubiera errores
            }
        }
        if (bajados > 0) actualizarUltimaSync(tabla, maxUpdate);
        QSqlQuery(dbLocal).exec("SET @skip_sync = 0");
    }
    return bajados;
}

void SyncManager::actualizarUltimaSync(const QString &tabla, const QDateTime &momento)
{
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    // Usar consulta directa para evitar el bug del protocolo binario con QDateTime(Invalid)
    QString sql = QString("UPDATE sync_control SET ultima_sync = '%1' WHERE tabla = '%2'")
                  .arg(momento.toString("yyyy-MM-dd HH:mm:ss"), tabla);
    q.exec(sql);
}

void SyncManager::cargarIdTiendaLocal()
{
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    if (q.exec("SELECT id FROM tiendas WHERE local = 1 LIMIT 1") && q.first())
        m_idTiendaLocal = q.value(0).toInt();
}

QDateTime SyncManager::ultimaSync(const QString &tabla)
{
    QString connName = conf->getConexionLocal();
    QSqlDatabase db = QSqlDatabase::database(connName);
    QSqlQuery q(db);
    // Usar consulta directa para evitar el bug del protocolo binario de MySQL/MariaDB en Qt 6,
    // que retorna QDateTime(Invalid) en campos de fecha/hora cuando se preparan consultas (prepare()).
    QString sql = QString("SELECT ultima_sync FROM sync_control WHERE tabla = '%1'").arg(tabla);
    if (!q.exec(sql)) {
        qWarning() << "SyncManager: Error en ultimaSync para" << tabla 
                   << "usando conexion" << connName 
                   << "abierta:" << db.isOpen() 
                   << "error:" << q.lastError().text();
    } else if (q.first()) {
        return q.value(0).toDateTime();
    }
    return QDateTime::fromString("2000-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
}

QString SyncManager::getPkTabla(const QString &tabla) const
{
    // Para 'vales' se usa vale_uuid como clave de sincronización global
    // porque idvales es AUTO_INCREMENT local y puede colisionar entre tiendas.
    static const QMap<QString, QString> m = {
        {"articulos","cod"},{"clientes","idCliente"},{"familias","id"},{"fabricantes","id"},
        {"proveedores","idProveedor"},{"codaux","id"},{"fpago","id"},{"impuestos","tipoIva"},
        {"formatos","idformato"},{"motivosEntrada","idtiposEntrada"},{"usuarios","id"},
        {"permisos","id"},{"vales","vale_uuid"}
    };
    return m.value(tabla, "");
}

void SyncManager::prepararTablasRemotas()
{
    if (!m_hayConexion) return;
    
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    QSqlDatabase dbNube = QSqlDatabase::database(CONEXION_NUBE);
    QSqlQuery qN(dbNube);
    QSqlQuery qL(dbLocal);

    for (const QString &tabla : TABLAS_MAESTRAS) {
        // 0. Asegurar que la tabla existe en la nube (copiando esquema local si falta)
        qN.exec(QString("SHOW TABLES LIKE '%1'").arg(tabla));
        if (!qN.next()) {
            qDebug() << "SyncManager: La tabla" << tabla << "no existe en la nube. Intentando crearla...";
            if (qL.exec(QString("SHOW CREATE TABLE `%1`").arg(tabla)) && qL.next()) {
                QString createSql = qL.value(1).toString();
                // Ejecutamos el CREATE TABLE tal cual viene del local
                if (qN.exec(createSql)) {
                    qDebug() << "SyncManager: Tabla" << tabla << "creada con éxito en la nube.";
                } else {
                    qWarning() << "SyncManager: Error al crear tabla" << tabla << "en la nube:" << qN.lastError().text();
                    continue; // No podemos seguir con esta tabla si no se pudo crear
                }
            } else {
                qWarning() << "SyncManager: No se pudo obtener el esquema local para la tabla" << tabla;
                continue;
            }
        }

        // 1. Asegurar columnas de sincronización (por si acaso la tabla ya existía pero era antigua)
        qN.exec(QString("ALTER TABLE `%1` ADD COLUMN IF NOT EXISTS id_tienda_origen INT DEFAULT NULL").arg(tabla));
        qN.exec(QString("ALTER TABLE `%1` ADD COLUMN IF NOT EXISTS updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP").arg(tabla));
        
        // 2. Asegurar Clave Primaria en la nube si falta
        QString pk = getPkTabla(tabla);
        if (!pk.isEmpty() && pk != "vale_uuid") { // vale_uuid se maneja aparte abajo
             qN.exec(QString("SELECT COUNT(*) FROM information_schema.statistics "
                            "WHERE table_schema = (SELECT DATABASE()) AND table_name = '%1' "
                            "AND index_name = 'PRIMARY'").arg(tabla));
             if (qN.next() && qN.value(0).toInt() == 0) {
                 qDebug() << "SyncManager: Añadiendo PK faltante a" << tabla << "en la nube";
                 if (!qN.exec(QString("ALTER TABLE `%1` ADD PRIMARY KEY (`%2`)").arg(tabla, pk))) {
                     qWarning() << "SyncManager: Error al añadir PK a" << tabla << "en la nube:" << qN.lastError().text();
                 }
             }
        }

        // 3. Comprobar por nombre de índice exacto para evitar duplicados.
        // Buscar por index_name (no por column_name) evita confundirse con
        // PRIMARY KEY u otros índices que incluyan la columna updated_at.
        qN.exec(QString("SELECT COUNT(*) FROM information_schema.statistics "
                       "WHERE table_schema = (SELECT DATABASE()) AND table_name = '%1' "
                       "AND index_name = 'idx_%1_updated'").arg(tabla));
        if (qN.next() && qN.value(0).toInt() == 0) {
            qN.exec(QString("ALTER TABLE `%1` ADD INDEX idx_%1_updated (updated_at)").arg(tabla));
        }
    }

    // Migración vale_uuid en la nube (caso especial para vales)
    qN.exec("ALTER TABLE `vales` ADD COLUMN IF NOT EXISTS "
           "vale_uuid CHAR(36) NOT NULL DEFAULT ''");
    qN.exec("UPDATE vales SET vale_uuid = UUID() WHERE vale_uuid = '' OR vale_uuid IS NULL");
    qN.exec("SELECT COUNT(*) FROM information_schema.statistics "
           "WHERE table_schema = DATABASE() AND table_name = 'vales' "
           "AND index_name = 'uq_vale_uuid'");
    if (qN.next() && qN.value(0).toInt() == 0)
        qN.exec("ALTER TABLE vales ADD UNIQUE INDEX uq_vale_uuid (vale_uuid)");

    // SEC-02: Asegurar columna salt y ampliar el tamaño de la clave en la tabla usuarios de la nube
    // Esto evita que los hashes SHA-256 se trunquen o que falle la inserción de salt en la nube.
    qN.exec("ALTER TABLE `usuarios` ADD COLUMN IF NOT EXISTS `salt` VARCHAR(64) DEFAULT NULL");
    qN.exec("ALTER TABLE `usuarios` MODIFY COLUMN `clave` VARCHAR(64) NOT NULL");
}
