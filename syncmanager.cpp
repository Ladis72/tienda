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
    {"articulos", {"stock", "min", "max", "pendientes_pedido", "ultima_venta", "ultimo_pedido", "encargados", "minimo_pedido"}}
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
    QSqlDatabase db = QSqlDatabase::database("DB");
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
        
        // Evitar duplicar índices: solo añadir si no existe
        q.exec(QString("SELECT COUNT(*) FROM information_schema.statistics "
                       "WHERE table_schema = DATABASE() AND table_name = '%1' "
                       "AND column_name = 'updated_at'").arg(tabla));
        if (q.next() && q.value(0).toInt() == 0) {
            q.exec(QString("ALTER TABLE `%1` ADD INDEX (updated_at)").arg(tabla));
        }
        
        q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES (?)");
        q.addBindValue(tabla);
        q.exec();
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
}

void SyncManager::crearTrigger(const QString &nombreTabla, const QString &clavePrimaria, const QString &evento)
{
    QSqlDatabase db = QSqlDatabase::database("DB");
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

    QSqlQuery q(QSqlDatabase::database("DB"));
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
    
    QSqlQuery q(QSqlDatabase::database("DB"));
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
    QSqlDatabase dbLocal = QSqlDatabase::database("DB");
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
            reg.prepare(QString("SELECT * FROM `%1` WHERE `%2` = ?").arg(tabla, pk));
            reg.addBindValue(idReg);
            if (reg.exec() && reg.first()) {
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
                        if (val.userType() == QMetaType::QDateTime || val.userType() == QMetaType::QDate)
                            vStr = "'" + val.toDateTime().toString("yyyy-MM-dd HH:mm:ss") + "'";
                        else
                            vStr = "'" + val.toString().replace("'", "''") + "'";
                    }
                    campos << "`" + campo + "`"; valores << vStr;
                    updates << QString("`%1` = VALUES(`%1`)").arg(campo);
                }
                QString sql = QString("INSERT INTO `%1` (%2) VALUES (%3) ON DUPLICATE KEY UPDATE %4")
                              .arg(tabla, campos.join(","), valores.join(","), updates.join(","));
                QSqlQuery ins(dbNube);
                ok = ins.exec(sql);
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
    QSqlDatabase dbLocal = QSqlDatabase::database("DB");
    QSqlDatabase dbNube  = QSqlDatabase::database(CONEXION_NUBE);
    int bajados = 0;
    QDateTime ahora = QDateTime::currentDateTime();

    QDateTime maxUpdate = ultimaSync("sync_unificaciones");
    QSqlQuery qNU(dbNube);
    qNU.prepare("SELECT tabla, id_perdedor, id_ganador, fecha FROM sync_unificaciones WHERE fecha > ? ORDER BY fecha ASC");
    qNU.addBindValue(maxUpdate.toString("yyyy-MM-dd HH:mm:ss"));
    
    if (qNU.exec()) {
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
        QDateTime desde = ultimaSyncActual.addSecs(-120); // 2 min overlap for safety
        QSqlQuery qN(dbNube);
        
        qDebug() << "   -> Consultando tabla:" << tabla << "desde" << desde.toString("yyyy-MM-dd HH:mm:ss");

        qN.prepare(QString("SELECT * FROM `%1` WHERE updated_at > ? ORDER BY updated_at ASC").arg(tabla));
        qN.addBindValue(desde.toString("yyyy-MM-dd HH:mm:ss"));
        if (!qN.exec()) continue;

        QString pk = getPkTabla(tabla);
        QStringList excluidos = CAMPOS_EXCLUIDOS.value(tabla);
        QSqlRecord recLoc = dbLocal.record(tabla);
        QStringList avisosPrecios;
        QDateTime maxUpdate = desde;
        int count = 0;

        QSqlQuery(dbLocal).exec("SET @skip_sync = 1");
        while (qN.next()) {
            count++;
            QSqlRecord rec = qN.record();
            QString idReg = rec.value(pk).toString();
            QDateTime recUpd = rec.value("updated_at").toDateTime();

            QStringList campos, valores, updates;
            for (int i = 0; i < rec.count(); ++i) {
                QString campo = rec.fieldName(i);
                if (recLoc.indexOf(campo) == -1 || excluidos.contains(campo.toLower())) continue;
                QVariant val = rec.value(i);
                QString vStr = "NULL";
                if (!val.isNull()) {
                    if (val.userType() == QMetaType::QDateTime || val.userType() == QMetaType::QDate)
                        vStr = "'" + val.toDateTime().toString("yyyy-MM-dd HH:mm:ss") + "'";
                    else
                        vStr = "'" + val.toString().replace("'", "''") + "'";
                }
                campos << "`" + campo + "`"; valores << vStr;
                updates << QString("`%1` = VALUES(`%1`)").arg(campo);
            }

            // Detectar cambio de precio antes de aplicar
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

            QString sql = QString("INSERT INTO `%1` (%2) VALUES (%3) ON DUPLICATE KEY UPDATE %4")
                          .arg(tabla, campos.join(","), valores.join(","), updates.join(","));
            
            QSqlQuery qIns(dbLocal);
            if (qIns.exec(sql)) {
                QDateTime regUpdate = rec.value("updated_at").toDateTime();
                if (regUpdate > maxUpdate) maxUpdate = regUpdate;
                
                // Solo contamos como "bajado" si es realmente más nuevo que nuestra última marca
                if (regUpdate > ultimaSyncActual) {
                    bajados++;
                }
                if (tabla == "articulos" && existeLoc) {
                    double pNew = rec.value("pvp").toDouble();
                    if (qAbs(pOld - pNew) > 0.01) {
                        qDebug() << "   [!] Cambio de precio detectado:" << rec.value("descripcion").toString() << pOld << "->" << pNew;
                        QSqlQuery qE(dbLocal);
                        qE.prepare("SELECT count(*) FROM precios_tienda WHERE cod_articulo = ?");
                        qE.addBindValue(idReg);
                        if (qE.exec() && qE.first() && qE.value(0).toInt() == 0)
                            avisosPrecios << QString("- %1: %2 -> %3").arg(rec.value("descripcion").toString()).arg(pOld).arg(pNew);
                    }
                }
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
    QSqlQuery q(QSqlDatabase::database("DB"));
    q.prepare("UPDATE sync_control SET ultima_sync = ? WHERE tabla = ?");
    q.addBindValue(momento); q.addBindValue(tabla);
    q.exec();
}

void SyncManager::cargarIdTiendaLocal()
{
    QSqlQuery q(QSqlDatabase::database("DB"));
    if (q.exec("SELECT id FROM tiendas WHERE local = 1 LIMIT 1") && q.first())
        m_idTiendaLocal = q.value(0).toInt();
}

QDateTime SyncManager::ultimaSync(const QString &tabla)
{
    QSqlQuery q(QSqlDatabase::database("DB"));
    q.prepare("SELECT ultima_sync FROM sync_control WHERE tabla = ?");
    q.addBindValue(tabla);
    if (q.exec() && q.first()) return q.value(0).toDateTime();
    return QDateTime::fromString("2000-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
}

QString SyncManager::getPkTabla(const QString &tabla) const
{
    static const QMap<QString, QString> m = {
        {"articulos","cod"},{"clientes","idCliente"},{"familias","id"},{"fabricantes","id"},
        {"proveedores","idProveedor"},{"codaux","id"},{"fpago","id"},{"impuestos","tipoIva"},
        {"formatos","idformato"},{"motivosEntrada","idtiposEntrada"},{"usuarios","id"},
        {"permisos","id"},{"vales","idvales"}
    };
    return m.value(tabla, "");
}

void SyncManager::prepararTablasRemotas()
{
    if (!m_hayConexion) return;
    QSqlQuery q(QSqlDatabase::database(CONEXION_NUBE));
    for (const QString &tabla : TABLAS_MAESTRAS) {
        q.exec(QString("ALTER TABLE `%1` ADD COLUMN id_tienda_origen INT DEFAULT NULL").arg(tabla));
        q.exec(QString("ALTER TABLE `%1` ADD COLUMN updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP").arg(tabla));
        q.exec(QString("ALTER TABLE `%1` ADD INDEX (updated_at)").arg(tabla));
    }
}
