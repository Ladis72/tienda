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
#include <QtConcurrent>
#include <QUuid>
#include <QPointer>
#include "unificarmaestros.h"
#include <QTcpSocket>
#include "configuracion.h"
#include "skip_sync_guard.h"

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
    "tiendas",
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

    // Índice para la purga periódica de registros ya subidos (subido=1, fecha antigua).
    // Comprobar por nombre de índice exacto para evitar duplicados.
    q.exec("SELECT COUNT(*) FROM information_schema.statistics "
           "WHERE table_schema = (SELECT DATABASE()) AND table_name = 'sync_cola' "
           "AND index_name = 'idx_sync_cola_purga'");
    if (q.next() && q.value(0).toInt() == 0) {
        q.exec("ALTER TABLE sync_cola ADD INDEX idx_sync_cola_purga (subido, fecha)");
    }

    // Watermark para controlar cuándo se hizo la última purga (máx. una vez al día)
    q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES ('sync_cola_purga')");
    q.exec();

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

    // Índice único para que la subida de unificaciones sea idempotente (ON DUPLICATE KEY)
    q.exec("ALTER TABLE sync_unificaciones ADD UNIQUE INDEX IF NOT EXISTS uq_unif (tabla, id_perdedor)");

    // Tabla de tombstones: registra qué filas se han borrado en la nube para
    // poder propagar el borrado a las demás tiendas (el download no ve filas borradas).
    q.exec("CREATE TABLE IF NOT EXISTS sync_borrados ("
           "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
           "  tabla       VARCHAR(64)  NOT NULL,"
           "  id_registro VARCHAR(64)  NOT NULL,"
           "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,"
           "  UNIQUE KEY uq_borrado (tabla, id_registro)"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

    q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES ('sync_borrados')");
    q.exec();
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
    if (!q.exec(sql)) {
        qDebug() << "SyncManager: Error creando trigger" << nombreTrigger << ":" << q.lastError().text();
    }
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
        dbNube.setConnectOptions("SSL_CA=" + sslCa + ";MYSQL_OPT_CONNECT_TIMEOUT=2;MYSQL_OPT_READ_TIMEOUT=5;MYSQL_OPT_WRITE_TIMEOUT=5");
    } else {
        dbNube.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=2;MYSQL_OPT_READ_TIMEOUT=5;MYSQL_OPT_WRITE_TIMEOUT=5");
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
    if (m_syncActivo.exchange(true)) {
        qDebug() << "SyncManager: Sincronización ya en curso, se omite este ciclo";
        return;
    }

    if (!m_hayConexion && !conectarNube()) {
        m_syncActivo = false;
        qDebug() << "SyncManager: Sincronización cancelada (sin conexión a la nube)";
        return;
    }

    qDebug() << "SyncManager: Iniciando ciclo de sincronización..." << QDateTime::currentDateTime().toString("HH:mm:ss");

    // Capturar los parámetros de la conexión local y de la nube en el hilo GUI.
    // QSqlDatabase no se puede compartir entre hilos, así que el hilo del pool
    // abrirá sus propias conexiones clonadas con estos parámetros.
    QSqlDatabase dbLocalGui = QSqlDatabase::database(conf->getConexionLocal());
    const QString driverLocal = dbLocalGui.driverName();
    const QString hostLocal = dbLocalGui.hostName();
    const int portLocal = dbLocalGui.port();
    const QString baseLocal = dbLocalGui.databaseName();
    const QString userLocal = dbLocalGui.userName();
    const QString passLocal = dbLocalGui.password();

    QSqlDatabase dbNubeGui = QSqlDatabase::database(CONEXION_NUBE);
    const QString hostNube = dbNubeGui.hostName();
    const int portNube = dbNubeGui.port();
    const QString baseNube = dbNubeGui.databaseName();
    const QString userNube = dbNubeGui.userName();
    const QString passNube = dbNubeGui.password();
    const QString optsNube = dbNubeGui.connectOptions();

    const QString usuario = conf->getUsuario(); // capturado en el hilo GUI (seguro para el hilo de pool)
    QPointer<SyncManager> self = this;

    m_futuroSync = QtConcurrent::run([=]() {
        if (!self) return;

        QString connLocalClone = "SyncLocal_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
        QString connNubeClone  = "SyncNube_"  + QUuid::createUuid().toString(QUuid::WithoutBraces);

        QSqlDatabase dbLocal = QSqlDatabase::addDatabase(driverLocal, connLocalClone);
        dbLocal.setHostName(hostLocal);
        if (portLocal > 0) dbLocal.setPort(portLocal);
        dbLocal.setDatabaseName(baseLocal);
        dbLocal.setUserName(userLocal);
        dbLocal.setPassword(passLocal);
        bool okLocal = dbLocal.open();

        QSqlDatabase dbNube;
        bool okNube = false;
        if (okLocal) {
            dbNube = QSqlDatabase::addDatabase("QMYSQL", connNubeClone);
            dbNube.setHostName(hostNube);
            if (portNube > 0) dbNube.setPort(portNube);
            dbNube.setDatabaseName(baseNube);
            dbNube.setUserName(userNube);
            dbNube.setPassword(passNube);
            if (!optsNube.isEmpty()) dbNube.setConnectOptions(optsNube);
            okNube = dbNube.open();
        }

        int subidos = 0, subUnif = 0, bajados = 0;
        if (okLocal && okNube) {
            subidos = self->subirCambios(connLocalClone, connNubeClone, usuario);
            subUnif = self->subirUnificaciones(connLocalClone, connNubeClone, usuario);
            bajados = self->bajarCambios(connLocalClone, connNubeClone, usuario);
            qDebug() << "SyncManager: Ciclo completado — Registros subidos:" << subidos
                     << "| Unificaciones subidas:" << subUnif
                     << "| Registros bajados:" << bajados;
        } else {
            qWarning() << "SyncManager: No se pudieron abrir las conexiones del ciclo de sync"
                       << "(local:" << okLocal << ", nube:" << okNube << ")";
        }

        // Purga periódica de sync_cola (máx. una vez al día) usando la conexión local clonada
        if (okLocal) self->purgarCola(connLocalClone, usuario);

        if (dbNube.isOpen()) dbNube.close();
        if (dbLocal.isOpen()) dbLocal.close();
        if (QSqlDatabase::contains(connNubeClone)) QSqlDatabase::removeDatabase(connNubeClone);
        if (QSqlDatabase::contains(connLocalClone)) QSqlDatabase::removeDatabase(connLocalClone);

        // Volver al hilo GUI para actualizar estado y emitir la señal
        QMetaObject::invokeMethod(self, [self, subidos, bajados, okNube]() {
            if (!self) return;
            self->m_syncActivo = false;
            if (!okNube && self->m_hayConexion) {
                self->m_hayConexion = false;
                emit self->conexionPerdida();
            }
            emit self->syncCompletado(subidos, bajados);
        }, Qt::QueuedConnection);
    });
}

int SyncManager::subirCambios(const QString &connLocal, const QString &connNube, const QString &usuario)
{
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);
    QSqlDatabase dbNube  = QSqlDatabase::database(connNube);
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
                    if (tabla == "tiendas") {
                        bool esLocal = (rec.value("local").toInt() == 1 || rec.value("id").toInt() == m_idTiendaLocal);
                        if (esLocal) {
                            if (campo.toLower() == "local") {
                                vStr = "0"; // En la nube todas las tiendas deben tener local = 0
                            } else {
                                QStringList camposLocales = {"ip", "usuario", "password", "baseDatos", "puerto", "ssl_ca"};
                                if (camposLocales.contains(campo.toLower())) {
                                    continue; // Evitar subir credenciales y configuración locales
                                }
                            }
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
                    QString err = QString("Error subiendo a %1 (id %2): %3")
                                  .arg(tabla, idReg, ins.lastError().text());
                    qDebug() << "SyncManager:" << err;
                    qDebug() << "SQL fallido:" << sql;
                    registrarLog(connLocal, "SyncError", usuario, err);
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

int SyncManager::subirUnificaciones(const QString &connLocal, const QString &connNube, const QString &usuario)
{
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);
    QSqlDatabase dbNube  = QSqlDatabase::database(connNube);
    QSqlQuery cola(dbLocal);
    cola.exec("SELECT id, tabla, id_perdedor, id_ganador FROM sync_unificaciones WHERE subido = 0 ORDER BY fecha ASC");

    int subidos = 0;
    QList<qint64> procesados;
    while (cola.next()) {
        qint64 idCola = cola.value(0).toLongLong();
        QString tabla = cola.value(1).toString();
        QString perdedor = cola.value(2).toString();
        QString ganador = cola.value(3).toString();

        QSqlQuery ins(dbNube);
        ins.prepare("INSERT INTO sync_unificaciones (tabla, id_perdedor, id_ganador) "
                    "VALUES (?, ?, ?) "
                    "ON DUPLICATE KEY UPDATE id_ganador = VALUES(id_ganador), fecha = CURRENT_TIMESTAMP");
        ins.addBindValue(tabla);
        ins.addBindValue(perdedor);
        ins.addBindValue(ganador);
        if (ins.exec()) {
            procesados << idCola; subidos++;
        } else {
            QString err = QString("Error subiendo unificación %1 (%2 -> %3): %4")
                          .arg(tabla, perdedor, ganador, ins.lastError().text());
            qDebug() << "SyncManager:" << err;
            registrarLog(connLocal, "SyncError", usuario, err);
        }
    }
    if (!procesados.isEmpty()) {
        QStringList sIds; for (qint64 id : procesados) sIds << QString::number(id);
        QSqlQuery(dbLocal).exec("UPDATE sync_unificaciones SET subido = 1 WHERE id IN (" + sIds.join(",") + ")");
    }
    return subidos;
}

int SyncManager::bajarBorrados(const QString &connLocal, const QString &connNube, const QString &usuario)
{
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);
    QSqlDatabase dbNube  = QSqlDatabase::database(connNube);

    QDateTime watermark = ultimaSync("sync_borrados", connLocal, usuario);
    QSqlQuery qN(dbNube);
    QString sql = QString("SELECT tabla, id_registro, UNIX_TIMESTAMP(fecha) AS f_ts "
                          "FROM sync_borrados WHERE fecha > '%1' ORDER BY fecha ASC")
                  .arg(watermark.toUTC().toString("yyyy-MM-dd HH:mm:ss"));
    if (!qN.exec(sql)) {
        QString err = "Error consultando sync_borrados: " + qN.lastError().text();
        qDebug() << "SyncManager:" << err;
        registrarLog(connLocal, "SyncError", usuario, err);
        return 0;
    }

    struct Borrado { QString tabla; QString idReg; qint64 ts; };
    QList<Borrado> borradosPendientes;
    qint64 maxTs = watermark.toUTC().toSecsSinceEpoch();
    while (qN.next()) {
        QString t = qN.value(0).toString();
        QString idReg = qN.value(1).toString();
        qint64 fTs = qN.value(2).toLongLong();
        if (getPkTabla(t).isEmpty()) continue;
        borradosPendientes << Borrado{t, idReg, fTs};
        if (fTs > maxTs) maxTs = fTs;
    }

    int borrados = 0;
    int conservados = 0;
    if (!borradosPendientes.isEmpty()) {
        // SkipSyncGuard evita que el DELETE local re-encuele en sync_cola
        // (el borrado ya está registrado como tombstone en la nube).
        SkipSyncGuard guard(connLocal);

        QSqlQuery qTsLocal(dbLocal);
        QSqlQuery qDel(dbLocal);
        QSqlQuery qPurge(dbLocal);
        qPurge.prepare("DELETE FROM sync_cola WHERE tabla = ? AND id_registro = ? AND subido = 0");

        for (const Borrado &b : borradosPendientes) {
            QString pk = getPkTabla(b.tabla);

            // ── Resolución de conflictos: edición local más nueva gana ──
            // Si la fila local fue modificada después del borrado en la nube,
            // se conserva: la edición local pendiente en sync_cola se re-subirá
            // y recreará la fila en la nube (filosofía "lo más nuevo gana").
            qTsLocal.prepare(QString("SELECT UNIX_TIMESTAMP(updated_at) FROM `%1` WHERE `%2` = ?").arg(b.tabla, pk));
            qTsLocal.addBindValue(b.idReg);
            bool localMasNuevo = false;
            if (qTsLocal.exec() && qTsLocal.first() && !qTsLocal.value(0).isNull()) {
                if (qTsLocal.value(0).toLongLong() > b.ts) localMasNuevo = true;
            }
            if (localMasNuevo) {
                conservados++;
                continue;
            }

            qDel.prepare(QString("DELETE FROM `%1` WHERE `%2` = ?").arg(b.tabla, pk));
            qDel.addBindValue(b.idReg);
            if (qDel.exec()) {
                // Purga de pendientes obsoletos: evita que un INSERT/UPDATE que
                // quedara sin subir re-cree la fila borrada en la nube.
                qPurge.bindValue(0, b.tabla);
                qPurge.bindValue(1, b.idReg);
                qPurge.exec();
                if (qDel.numRowsAffected() > 0) borrados++;
            } else {
                QString err = QString("Error aplicando borrado en %1 (id %2): %3")
                              .arg(b.tabla, b.idReg, qDel.lastError().text());
                qDebug() << "SyncManager:" << err;
                registrarLog(connLocal, "SyncError", usuario, err);
            }
        }
    }

    actualizarUltimaSync("sync_borrados", QDateTime::fromSecsSinceEpoch(maxTs, Qt::UTC), connLocal);
    if (borrados > 0)
        qDebug() << "SyncManager: Borrados aplicados localmente:" << borrados;
    if (conservados > 0)
        qDebug() << "SyncManager: Borrados conservados por edición local más nueva:" << conservados;
    return borrados;
}

void SyncManager::purgarCola(const QString &connLocal, const QString &usuario)
{
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);

    // Solo purgar una vez al día: el watermark evita recorrer/borrar la tabla en cada ciclo (5 min).
    QDateTime ultimaPurga = ultimaSync("sync_cola_purga", connLocal, usuario);
    if (ultimaPurga.isValid() && ultimaPurga.secsTo(QDateTime::currentDateTime()) < 24 * 3600) return;

    // Borrar registros ya subidos (subido=1) con más de 7 días de antigüedad.
    // Los pendientes (subido=0) nunca se tocan: aún deben subirse a la nube.
    QSqlQuery q(dbLocal);
    int borradosCola = 0;
    if (q.exec("DELETE FROM sync_cola WHERE subido = 1 AND fecha < DATE_SUB(NOW(), INTERVAL 7 DAY)")) {
        borradosCola = q.numRowsAffected();
        if (borradosCola > 0)
            qDebug() << "SyncManager: Purga de sync_cola:" << borradosCola << "registros eliminados";
    } else {
        QString err = "Error purgando sync_cola: " + q.lastError().text();
        qWarning() << "SyncManager:" << err;
        registrarLog(connLocal, "SyncError", usuario, err);
    }

    // Idem para las unificaciones de maestros ya propagadas
    int borradosUnif = 0;
    if (q.exec("DELETE FROM sync_unificaciones WHERE subido = 1 AND fecha < DATE_SUB(NOW(), INTERVAL 7 DAY)")) {
        borradosUnif = q.numRowsAffected();
        if (borradosUnif > 0)
            qDebug() << "SyncManager: Purga de sync_unificaciones:" << borradosUnif << "registros eliminados";
    } else {
        QString err = "Error purgando sync_unificaciones: " + q.lastError().text();
        qWarning() << "SyncManager:" << err;
        registrarLog(connLocal, "SyncError", usuario, err);
    }

    actualizarUltimaSync("sync_cola_purga", QDateTime::currentDateTime(), connLocal);
}

void SyncManager::registrarLog(const QString &connLocal, const QString &categoria,
                               const QString &usuario, const QString &mensaje)
{
    QSqlDatabase db = QSqlDatabase::database(connLocal);
    if (!db.isOpen()) return;

    QSqlQuery q(db);
    q.prepare("INSERT INTO logs (categoria, usuario, mensaje) VALUES (?, ?, ?)");
    q.addBindValue(categoria);
    q.addBindValue(usuario);
    q.addBindValue(mensaje);
    if (!q.exec()) {
        qWarning() << "SyncManager: Error al insertar log:" << q.lastError().text();
    }
}

int SyncManager::bajarCambios(const QString &connLocal, const QString &connNube, const QString &usuario)
{
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);
    QSqlDatabase dbNube  = QSqlDatabase::database(connNube);
    int bajados = 0;
    QDateTime ahora = QDateTime::currentDateTime();

    // Aplicar primero los borrados (tombstones) para que las filas re-creadas
    // se vuelvan a insertar después por el loop de tablas.
    int borrados = bajarBorrados(connLocal, connNube, usuario);

    QDateTime maxUpdate = ultimaSync("sync_unificaciones", connLocal, usuario);
    QSqlQuery qNU(dbNube);
    // Usar consulta directa para evitar el bug del protocolo binario con QDateTime(Invalid) en prepared statements.
    QString sqlU = QString("SELECT tabla, id_perdedor, id_ganador, fecha FROM sync_unificaciones WHERE fecha > '%1' ORDER BY fecha ASC")
                   .arg(maxUpdate.toString("yyyy-MM-dd HH:mm:ss"));
    
    if (qNU.exec(sqlU)) {
        // SkipSyncGuard evita que los UPDATE/DELETE locales de la unificación
        // re-encuelen en sync_cola (el cambio ya se ha propagado desde la nube).
        SkipSyncGuard guardUnif(connLocal);
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
        actualizarUltimaSync("sync_unificaciones", maxUpdate, connLocal);
    }

    for (const QString &tabla : TABLAS_MAESTRAS) {
        QDateTime ultimaSyncActual = ultimaSync(tabla, connLocal, usuario);
        QDateTime desde = ultimaSyncActual.addSecs(-120); // 2 min overlap
        QSqlQuery qN(dbNube);
        
        qDebug() << "   -> Consultando tabla:" << tabla << "desde" << desde.toString("yyyy-MM-dd HH:mm:ss");

        // Usar consulta directa para evitar el bug del protocolo binario de MariaDB que devuelve QDateTime(Invalid)
        // UNIX_TIMESTAMP(updated_at) devuelve un entero, inmune a ese bug, y permite
        // comparar local vs nube de forma independiente de la zona horaria.
        QString sqlN = QString("SELECT *, UNIX_TIMESTAMP(updated_at) AS up_ts FROM `%1` WHERE updated_at > '%2' ORDER BY updated_at ASC")
                       .arg(tabla, desde.toUTC().toString("yyyy-MM-dd HH:mm:ss"));
        if (!qN.exec(sqlN)) {
            QString err = QString("Error consultando tabla %1 en la nube: %2")
                          .arg(tabla, qN.lastError().text());
            qDebug() << "SyncManager:" << err;
            registrarLog(connLocal, "SyncError", usuario, err);
            continue;
        }

        QString pk = getPkTabla(tabla);
        QStringList excluidos = CAMPOS_EXCLUIDOS.value(tabla);
        QSqlRecord recLoc = dbLocal.record(tabla);
        QStringList avisosPrecios;
        QDateTime maxUpdate = desde;
        int bajadosTabla = 0;

        // Consultas preparadas para la resolución de conflictos local.
        // Devuelven enteros (UNIX_TIMESTAMP y COUNT), inmunes al bug de
        // parseo de fechas de los prepared statements.
        QSqlQuery qTsLocal(dbLocal);
        qTsLocal.prepare(QString("SELECT UNIX_TIMESTAMP(updated_at) FROM `%1` WHERE `%2` = ?").arg(tabla, pk));
        QSqlQuery qPendiente(dbLocal);
        qPendiente.prepare("SELECT COUNT(*) FROM sync_cola WHERE tabla = ? AND id_registro = ? AND subido = 0");

        {
            // SkipSyncGuard desactiva los triggers de sync durante la escritura local
            // y garantiza el reset de @skip_sync en todos los caminos (RAII).
            SkipSyncGuard guard(connLocal);
            while (qN.next()) {
                QSqlRecord rec = qN.record();
                QString idReg = rec.value(pk).toString();
                QDateTime recUpd = rec.value("updated_at").toDateTime();
                qint64 recUpdTs = rec.value("up_ts").toLongLong();

                // ── Resolución de conflictos por updated_at ──
                // Solo se evita la sobrescritura cuando existe una edición local
                // PENDIENTE (sync_cola subido=0) y la local no es más antigua que
                // la remota: gana lo local y la edición pendiente se re-subirá sola.
                // Sin edición pendiente, se aplica el remoto siempre (permite
                // refrescar filas con el mismo updated_at dentro del mismo segundo).
                qTsLocal.bindValue(0, idReg);
                bool tieneLocal = (qTsLocal.exec() && qTsLocal.first() && !qTsLocal.value(0).isNull());
                if (tieneLocal) {
                    qint64 localTs = qTsLocal.value(0).toLongLong();
                    if (localTs >= recUpdTs) {
                        qPendiente.bindValue(0, tabla);
                        qPendiente.bindValue(1, idReg);
                        bool pendiente = (qPendiente.exec() && qPendiente.first() && qPendiente.value(0).toInt() > 0);
                        if (pendiente) continue; // la edición local pendiente ganará al re-subirse
                    } else {
                        // Remoto más nuevo: se aplica y se descartan las ediciones
                        // locales obsoletas que aún estuvieran pendientes en sync_cola,
                        // para que no se re-suban datos antiguos encima de los nuevos.
                        qPendiente.bindValue(0, tabla);
                        qPendiente.bindValue(1, idReg);
                        if (qPendiente.exec() && qPendiente.first() && qPendiente.value(0).toInt() > 0) {
                            QSqlQuery qLimpiar(dbLocal);
                            qLimpiar.prepare("DELETE FROM sync_cola WHERE tabla = ? AND id_registro = ? AND subido = 0");
                            qLimpiar.addBindValue(tabla);
                            qLimpiar.addBindValue(idReg);
                            qLimpiar.exec();
                        }
                    }
                }

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

                    // up_ts es una columna sintética añadida por nosotros, no existe en la tabla.
                    if (campo == "up_ts") continue;
                    // NUNCA sincronizar el ID físico local ni la PK aquí (se añade luego)
                    if (campo == pk || campo.toLower() == "id" + tabla.toLower()) continue;
                    if (recLoc.indexOf(campo) == -1 || excluidos.contains(campo.toLower())) continue;
                    if (tabla == "tiendas") {
                        bool esLocal = (idReg.toInt() == m_idTiendaLocal || rec.value("local").toInt() == 1);
                        if (esLocal) {
                            QStringList camposLocales = {"ip", "local", "usuario", "password", "baseDatos", "puerto", "ssl_ca"};
                            if (camposLocales.contains(campo.toLower())) {
                                continue;
                            }
                        }
                    }
                    
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
                        bajadosTabla++;
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
                    QString err = QString("ERROR bajando %1 (id %2): %3")
                                  .arg(tabla, idReg, qIns.lastError().text());
                    qDebug() << "SyncManager:" << err;
                    registrarLog(connLocal, "SyncError", usuario, err);
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
                QString err = "Error al crear nota de precios: " + qNot.lastError().text();
                qWarning() << "   [!]" << err;
                registrarLog(connLocal, "SyncError", usuario, err);
            } else {
                qDebug() << "   [+] Nota de aviso creada correctamente.";
                avisosPrecios.clear(); // Limpiar para que no se repita en la siguiente tabla si hubiera errores
            }
        }

        // 2.4: watermark por tabla, solo avanza si esta tabla bajó registros nuevos.
        // El contador es por tabla (no global) para evitar que una tabla activa
        // arrastre el watermark de las inactivas hacia atrás.
        if (bajadosTabla > 0) {
            // Salvaguarda anti-retroceso: el watermark nunca debe ser menor que el actual.
            if (maxUpdate < ultimaSyncActual) maxUpdate = ultimaSyncActual;
            actualizarUltimaSync(tabla, maxUpdate, connLocal);
        }
        bajados += bajadosTabla;
    }
    qDebug() << "SyncManager: bajarCambios — bajados:" << bajados << "| borrados:" << borrados;
    return bajados;
}

void SyncManager::actualizarUltimaSync(const QString &tabla, const QDateTime &momento,
                                       const QString &connLocal)
{
    QSqlQuery q(QSqlDatabase::database(connLocal));
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

QDateTime SyncManager::ultimaSync(const QString &tabla, const QString &connLocal, const QString &usuario)
{
    QString connName = connLocal;
    QSqlDatabase db = QSqlDatabase::database(connName);
    QSqlQuery q(db);
    // Usar consulta directa para evitar el bug del protocolo binario de MySQL/MariaDB en Qt 6,
    // que retorna QDateTime(Invalid) en campos de fecha/hora cuando se preparan consultas (prepare()).
    QString sql = QString("SELECT ultima_sync FROM sync_control WHERE tabla = '%1'").arg(tabla);
    if (!q.exec(sql)) {
        QString err = QString("Error en ultimaSync para %1 (conexión %2, abierta: %3): %4")
                      .arg(tabla, connName, db.isOpen() ? "sí" : "no", q.lastError().text());
        qWarning() << "SyncManager:" << err;
        registrarLog(connLocal, "SyncError", usuario, err);
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
        {"formatos","idformato"},{"motivosEntrada","idtiposEntrada"},{"tiendas","id"},{"usuarios","id"},
        {"permisos","id"},{"vales","vale_uuid"}
    };
    return m.value(tabla, "");
}

void SyncManager::prepararTablasRemotas()
{
    if (!m_hayConexion) return;
    
    const QString connLocal = conf->getConexionLocal();
    QSqlDatabase dbLocal = QSqlDatabase::database(connLocal);
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
                    QString err = QString("Error al crear tabla %1 en la nube: %2")
                                  .arg(tabla, qN.lastError().text());
                    qWarning() << "SyncManager:" << err;
                    registrarLog(connLocal, "SyncError", conf->getUsuario(), err);
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
                     QString err = QString("Error al añadir PK a %1 en la nube: %2")
                                   .arg(tabla, qN.lastError().text());
                     qWarning() << "SyncManager:" << err;
                     registrarLog(connLocal, "SyncError", conf->getUsuario(), err);
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

    // ── Tablas de sincronización en la nube ────────────────────────────────
    // sync_unificaciones: recibe las fusiones de maestros de cualquier tienda.
    qN.exec("CREATE TABLE IF NOT EXISTS sync_unificaciones ("
            "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
            "  tabla       VARCHAR(64)  NOT NULL,"
            "  id_perdedor VARCHAR(64)  NOT NULL,"
            "  id_ganador  VARCHAR(64)  NOT NULL,"
            "  subido      TINYINT(1)   NOT NULL DEFAULT 0,"
            "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "  UNIQUE KEY uq_unif (tabla, id_perdedor)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

    // sync_borrados: tombstones que permiten propagar los borrados a otras tiendas.
    qN.exec("CREATE TABLE IF NOT EXISTS sync_borrados ("
            "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
            "  tabla       VARCHAR(64)  NOT NULL,"
            "  id_registro VARCHAR(64)  NOT NULL,"
            "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,"
            "  UNIQUE KEY uq_borrado (tabla, id_registro)"
            ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

    // Watermarks para la descarga de tombstones.
    QSqlQuery qControl(dbNube);
    qControl.exec("CREATE TABLE IF NOT EXISTS sync_control ("
                  "  tabla       VARCHAR(64) PRIMARY KEY,"
                  "  ultima_sync DATETIME    NOT NULL DEFAULT '2000-01-01 00:00:00'"
                  ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");
    qControl.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES (?)");
    qControl.addBindValue("sync_borrados");
    qControl.exec();

    // Triggers de borrado en la nube: cada DELETE en una tabla maestra deja un
    // tombstone que las demás tiendas bajarán en su siguiente sincronización.
    for (const QString &tabla : TABLAS_MAESTRAS) {
        QString pk = getPkTabla(tabla);
        if (pk.isEmpty()) continue;
        QString nombreTrigger = QString("sync_borrados_%1").arg(tabla);
        qN.exec(QString("DROP TRIGGER IF EXISTS %1").arg(nombreTrigger));
        QString sql = QString(
            "CREATE TRIGGER %1 AFTER DELETE ON `%2` FOR EACH ROW "
            "INSERT INTO sync_borrados (tabla, id_registro) VALUES ('%2', OLD.`%3`) "
            "ON DUPLICATE KEY UPDATE fecha = CURRENT_TIMESTAMP"
        ).arg(nombreTrigger, tabla, pk);
        if (!qN.exec(sql)) {
            QString err = QString("Error creando trigger de borrados para %1 en la nube: %2")
                          .arg(tabla, qN.lastError().text());
            qWarning() << "SyncManager:" << err;
            registrarLog(connLocal, "SyncError", conf->getUsuario(), err);
        }
    }
}
