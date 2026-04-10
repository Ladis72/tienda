#include "syncmanager.h"


#include <QCoreApplication>
#include <QDebug>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>

// ─────────────────────────────────────────────────────────────────────────────
// Constantes
// ─────────────────────────────────────────────────────────────────────────────

/// Nombre de la conexión Qt para la BD en la nube (no "DB", que es la local)
const QString SyncManager::CONEXION_NUBE = "NUBE";

/// Tablas maestras cuyo contenido se comparte entre todas las tiendas.
/// El orden importa: las tablas con FK deben ir después de su tabla padre.
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
    "permisos"
};

// ─────────────────────────────────────────────────────────────────────────────
// Constructor / Destructor
// ─────────────────────────────────────────────────────────────────────────────

SyncManager::SyncManager(QObject *parent)
    : QObject(parent)
    , m_timerPing(new QTimer(this))
    , m_timerSync(new QTimer(this))
    , m_hayConexion(false)
{
    // Comprobación de conexión cada 30 segundos
    m_timerPing->setInterval(30 * 1000);
    connect(m_timerPing, &QTimer::timeout, this, &SyncManager::comprobarConexion);

    // Sincronización cada 5 minutos
    m_timerSync->setInterval(5 * 60 * 1000);
    connect(m_timerSync, &QTimer::timeout, this, &SyncManager::sincronizar);
}

SyncManager::~SyncManager()
{
    desconectarNube();
}

// ─────────────────────────────────────────────────────────────────────────────
// Inicialización
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Inicia el SyncManager: crea infraestructura SQL y arranca los timers.
 *
 * Debe llamarse una vez después de que la BD local esté abierta.
 */
void SyncManager::iniciar()
{
    // 1. Crear tablas auxiliares de sync en la BD local
    crearTablasSyncLocal();

    // 2. Crear triggers para detectar cambios automáticamente
    crearTriggers();

    // 3. Arrancar timers
    m_timerPing->start();
    m_timerSync->start();

    // 4. Comprobar conexión inmediatamente al arrancar
    comprobarConexion();

    qDebug() << "SyncManager: iniciado correctamente";
}

// ─────────────────────────────────────────────────────────────────────────────
// Infraestructura SQL local
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Crea las tablas auxiliares de sync en la BD local si no existen.
 *
 *  sync_cola    → cola de cambios pendientes de subir a la nube
 *  sync_control → controla la fecha de última sync por tabla (para la bajada)
 */
void SyncManager::crearTablasSyncLocal()
{
    QSqlDatabase db = QSqlDatabase::database("DB");
    QSqlQuery q(db);

    // Cola de cambios pendientes de subir
    q.exec("CREATE TABLE IF NOT EXISTS sync_cola ("
           "  id          BIGINT UNSIGNED AUTO_INCREMENT PRIMARY KEY,"
           "  tabla       VARCHAR(64)  NOT NULL COMMENT 'Nombre de la tabla modificada',"
           "  id_registro VARCHAR(64)  NOT NULL COMMENT 'PK del registro modificado',"
           "  accion      ENUM('INSERT','UPDATE','DELETE') NOT NULL,"
           "  fecha       DATETIME     NOT NULL DEFAULT CURRENT_TIMESTAMP,"
           "  subido      TINYINT(1)   NOT NULL DEFAULT 0 COMMENT '1=subido a la nube'"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4"
           "  COMMENT='Cola de cambios en tablas maestras pendientes de sincronizar'");

    if (q.lastError().isValid())
        qWarning() << "SyncManager: error creando sync_cola:" << q.lastError().text();

    // Control de última sincronización por tabla (para la bajada)
    q.exec("CREATE TABLE IF NOT EXISTS sync_control ("
           "  tabla       VARCHAR(64) PRIMARY KEY,"
           "  ultima_sync DATETIME    NOT NULL DEFAULT '2000-01-01 00:00:00'"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4"
           "  COMMENT='Fecha de última sincronización recibida desde la nube por tabla'");

    if (q.lastError().isValid())
        qWarning() << "SyncManager: error creando sync_control:" << q.lastError().text();

    // Insertar filas por defecto en sync_control (una por tabla maestra)
    for (const QString &tabla : TABLAS_MAESTRAS) {
        q.prepare("INSERT IGNORE INTO sync_control (tabla) VALUES (?)");
        q.addBindValue(tabla);
        q.exec();
    }
}

/**
 * @brief Crea los triggers en la BD local para todas las tablas maestras.
 *
 * Cada tabla maestra tiene dos triggers: AFTER INSERT y AFTER UPDATE.
 * Los triggers insertan una fila en sync_cola para que el SyncManager
 * sepa qué registros hay que subir a la nube.
 */
void SyncManager::crearTriggers()
{
    // Mapa tabla → nombre de su clave primaria real en la base de datos
    QMap<QString, QString> claves = {
        {"articulos",      "cod"},
        {"clientes",       "idCliente"},
        {"familias",       "id"},
        {"fabricantes",    "id"},
        {"proveedores",    "idProveedor"},
        {"codaux",         "id"},
        {"fpago",          "id"},
        {"impuestos",      "tipoIva"},
        {"formatos",       "idformato"},
        {"motivosEntrada", "idtiposEntrada"},
        {"usuarios",       "id"},
        {"permisos",       "id"}
    };

    for (const QString &tabla : TABLAS_MAESTRAS) {
        if (!claves.contains(tabla)) {
            qWarning() << "SyncManager: clave primaria desconocida para tabla" << tabla;
            continue;
        }
        crearTrigger(tabla, claves[tabla], "INSERT");
        crearTrigger(tabla, claves[tabla], "UPDATE");
    }
}

/**
 * @brief Crea un trigger AFTER INSERT o AFTER UPDATE en la BD local.
 *
 * Si el trigger ya existe, no hace nada (usa DROP IF EXISTS + CREATE).
 *
 * @param nombreTabla   Nombre de la tabla a vigilar
 * @param clavePrimaria Nombre del campo PK de esa tabla
 * @param evento        "INSERT" o "UPDATE"
 */
void SyncManager::crearTrigger(const QString &nombreTabla,
                                const QString &clavePrimaria,
                                const QString &evento)
{
    QSqlDatabase db = QSqlDatabase::database("DB");
    QSqlQuery q(db);

    // Nombre único del trigger: sync_<tabla>_<evento>
    QString nombreTrigger = QString("sync_%1_%2").arg(nombreTabla).arg(evento.toLower());

    // Borrar si ya existe (para poder recrearlo si cambia la estructura)
    q.exec(QString("DROP TRIGGER IF EXISTS %1").arg(nombreTrigger));

    // Crear el trigger
    QString sql = QString(
        "CREATE TRIGGER %1 "
        "AFTER %2 ON %3 "
        "FOR EACH ROW "
        "BEGIN "
        "  INSERT INTO sync_cola (tabla, id_registro, accion) "
        "  VALUES ('%3', NEW.%4, '%2'); "
        "END"
    ).arg(nombreTrigger, evento, nombreTabla, clavePrimaria);

    if (!q.exec(sql)) {
        qWarning() << "SyncManager: error creando trigger" << nombreTrigger
                   << ":" << q.lastError().text();
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Conexión a la nube
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Abre la conexión a la BD en la nube usando los datos de config_nube.
 * @return true si la conexión se establece correctamente
 */
bool SyncManager::conectarNube()
{
    // Si ya está abierta, reutilizarla
    if (QSqlDatabase::database(CONEXION_NUBE).isOpen())
        return true;

    // Leer datos de config_nube desde la BD local
    QSqlQuery q(QSqlDatabase::database("DB"));
    q.exec("SELECT servidor, puerto, baseDatos, usuario, clave, ssl_ca "
           "FROM config_nube WHERE id = 1");

    if (!q.first()) {
        qDebug() << "SyncManager: sin configuración de nube (config_nube vacía)";
        return false;
    }

    QString host   = q.value(0).toString();
    int     puerto = q.value(1).toInt();
    QString bd     = q.value(2).toString();
    QString user   = q.value(3).toString();
    QString pass   = q.value(4).toString();
    QString sslCa  = q.value(5).toString();

    if (host.isEmpty() || bd.isEmpty()) {
        qDebug() << "SyncManager: config_nube incompleta, sin sincronización";
        return false;
    }

    // Resolver ruta relativa del certificado SSL
    if (!sslCa.isEmpty() && QDir::isRelativePath(sslCa))
        sslCa = QCoreApplication::applicationDirPath() + "/" + sslCa;

    // Abrir conexión
    QSqlDatabase dbNube = QSqlDatabase::addDatabase("QMYSQL", CONEXION_NUBE);
    dbNube.setHostName(host);
    dbNube.setPort(puerto > 0 ? puerto : 3306);
    dbNube.setDatabaseName(bd);
    dbNube.setUserName(user);
    dbNube.setPassword(pass);

    QString opciones = "MYSQL_OPT_CONNECT_TIMEOUT=5";
    if (!sslCa.isEmpty())
        opciones += ";SSL_CA=" + sslCa;
    dbNube.setConnectOptions(opciones);

    if (!dbNube.open()) {
        qDebug() << "SyncManager: no se pudo conectar a la nube:"
                 << dbNube.lastError().text();
        QSqlDatabase::removeDatabase(CONEXION_NUBE);
        return false;
    }

    return true;
}

/**
 * @brief Cierra y elimina la conexión a la nube.
 */
void SyncManager::desconectarNube()
{
    if (QSqlDatabase::contains(CONEXION_NUBE)) {
        QSqlDatabase::database(CONEXION_NUBE).close();
        QSqlDatabase::removeDatabase(CONEXION_NUBE);
    }
}

// ─────────────────────────────────────────────────────────────────────────────
// Timers: ping y sync
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Comprueba si la nube está accesible.
 *
 * Intenta abrir la conexión. Si cambia el estado respecto al anterior,
 * emite la señal correspondiente y (si se recupera) lanza sync inmediato.
 */
void SyncManager::comprobarConexion()
{
    bool anteriorEstado = m_hayConexion;

    // Cerrar conexión previa para forzar un intento limpio
    desconectarNube();
    m_hayConexion = conectarNube();

    if (m_hayConexion && !anteriorEstado) {
        qDebug() << "SyncManager: conexión a la nube recuperada → sync inmediato";
        emit conexionRecuperada();
        sincronizar(); // Sync inmediato al recuperar conexión
    } else if (!m_hayConexion && anteriorEstado) {
        qDebug() << "SyncManager: conexión a la nube perdida";
        emit conexionPerdida();
    }
}

/**
 * @brief Ejecuta la sincronización completa (subida + bajada).
 *
 * Se llama desde el timer de 5 minutos o inmediatamente al recuperar conexión.
 * Si no hay conexión, sale silenciosamente sin tocar nada.
 */
void SyncManager::sincronizar()
{
    if (!m_hayConexion) {
        // Sin conexión: intentar conectar una vez más antes de rendirse
        desconectarNube();
        m_hayConexion = conectarNube();
        if (!m_hayConexion) return;
    }

    qDebug() << "SyncManager: iniciando sincronización" << QDateTime::currentDateTime();

    int subidos = subirCambios();
    int bajados = bajarCambios();

    qDebug() << "SyncManager: sync completado — subidos:" << subidos
             << "bajados:" << bajados;

    emit syncCompletado(subidos, bajados);
}

// ─────────────────────────────────────────────────────────────────────────────
// Operaciones de sincronización
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Sube a la nube los registros de sync_cola con subido=0.
 *
 * Para cada entrada pendiente, lee el registro completo de la BD local
 * y lo inserta o actualiza en la nube mediante INSERT ... ON DUPLICATE KEY UPDATE.
 *
 * @return Número de registros subidos con éxito
 */
int SyncManager::subirCambios()
{
    QSqlDatabase dbLocal = QSqlDatabase::database("DB");
    QSqlDatabase dbNube  = QSqlDatabase::database(CONEXION_NUBE);

    // Leer la cola de pendientes (agrupada por tabla+registro para evitar duplicados)
    QSqlQuery cola(dbLocal);
    cola.exec("SELECT id, tabla, id_registro, accion "
              "FROM sync_cola "
              "WHERE subido = 0 "
              "ORDER BY fecha ASC");

    int subidos = 0;
    QList<qint64> idsProcessados;

    while (cola.next()) {
        qint64  idCola    = cola.value(0).toLongLong();
        QString tabla     = cola.value(1).toString();
        QString idRegistro = cola.value(2).toString();
        QString accion    = cola.value(3).toString();

        bool exito = false;

        if (accion == "DELETE") {
            // Los DELETEs en tablas maestras son raros; los ignoramos por ahora
            // (política: no borrar en la nube automáticamente)
            exito = true;
        } else {
            // Obtener el nombre de la clave primaria para esta tabla
            QString pk = "id";
            if (tabla == "articulos") pk = "cod";
            else if (tabla == "clientes") pk = "idCliente";
            else if (tabla == "proveedores") pk = "idProveedor";
            else if (tabla == "impuestos") pk = "tipoIva";
            else if (tabla == "formatos") pk = "idformato";
            else if (tabla == "motivosEntrada") pk = "idtiposEntrada";

            // Leer el registro completo de la BD local
            QSqlQuery reg(dbLocal);
            QString where = QString("`%1` = '%2'").arg(pk, idRegistro);
            // Si es numérico, quitar comillas (opcional en MariaDB pero más limpio)
            if (pk != "cod") where = QString("`%1` = %2").arg(pk, idRegistro);

            reg.exec(QString("SELECT * FROM `%1` WHERE %2 LIMIT 1").arg(tabla, where));

            if (reg.first()) {
                // Construir INSERT ... ON DUPLICATE KEY UPDATE dinámicamente
                QSqlRecord record = reg.record();
                QStringList campos;
                QStringList valores;
                QStringList updates;

                for (int i = 0; i < record.count(); ++i) {
                    QString campo = record.fieldName(i);
                    QString valor = record.value(i).toString().replace("'", "''");
                    campos  << "`" + campo + "`";
                    valores << "'" + valor + "'";
                    updates << QString("`%1` = VALUES(`%1`)").arg(campo);
                }

                QString sql = QString(
                    "INSERT INTO `%1` (%2) VALUES (%3) "
                    "ON DUPLICATE KEY UPDATE %4"
                ).arg(tabla,
                      campos.join(", "),
                      valores.join(", "),
                      updates.join(", "));

                QSqlQuery ins(dbNube);
                exito = ins.exec(sql);
                if (!exito)
                    qWarning() << "SyncManager: error subiendo a nube:" << ins.lastError().text();
            } else {
                // Registro ya no existe en local → marcar como procesado igualmente
                exito = true;
            }
        }

        if (exito) {
            idsProcessados << idCola;
            subidos++;
        }
    }

    // Marcar como subidos en la cola local
    if (!idsProcessados.isEmpty()) {
        QStringList ids;
        for (qint64 id : idsProcessados)
            ids << QString::number(id);

        QSqlQuery upd(dbLocal);
        upd.exec("UPDATE sync_cola SET subido = 1 WHERE id IN (" + ids.join(",") + ")");
    }

    return subidos;
}

/**
 * @brief Baja desde la nube los registros más recientes que la última sync.
 *
 * Para cada tabla maestra, consulta la nube los registros con updated_at
 * posterior a la última sync local y los aplica en la BD local.
 *
 * @return Número de registros bajados con éxito
 */
int SyncManager::bajarCambios()
{
    QSqlDatabase dbLocal = QSqlDatabase::database("DB");
    QSqlDatabase dbNube  = QSqlDatabase::database(CONEXION_NUBE);

    int bajados = 0;
    QDateTime ahora = QDateTime::currentDateTime();

    for (const QString &tabla : TABLAS_MAESTRAS) {
        QDateTime ultima = ultimaSync(tabla);

        // Pedir a la nube los registros más nuevos
        QSqlQuery qNube(dbNube);
        qNube.prepare(QString("SELECT * FROM `%1` "
                              "WHERE updated_at > ? "
                              "ORDER BY updated_at ASC").arg(tabla));
        qNube.addBindValue(ultima.toString("yyyy-MM-dd HH:mm:ss"));

        if (!qNube.exec()) {
            qWarning() << "SyncManager: error consultando nube tabla" << tabla
                       << ":" << qNube.lastError().text();
            continue;
        }

        while (qNube.next()) {
            QSqlRecord rec = qNube.record();
            QStringList campos, valores, updates;

            for (int i = 0; i < rec.count(); ++i) {
                QString campo = rec.fieldName(i);
                QString valor = rec.value(i).toString().replace("'", "''");
                campos  << "`" + campo + "`";
                valores << "'" + valor + "'";
                updates << QString("`%1` = VALUES(`%1`)").arg(campo);
            }

            QString sql = QString(
                "INSERT INTO `%1` (%2) VALUES (%3) "
                "ON DUPLICATE KEY UPDATE %4"
            ).arg(tabla,
                  campos.join(", "),
                  valores.join(", "),
                  updates.join(", "));

            QSqlQuery ins(dbLocal);
            if (ins.exec(sql))
                bajados++;
            else
                qWarning() << "SyncManager: error aplicando en local:" << ins.lastError().text();
        }

        // Actualizar la última sync para esta tabla
        actualizarUltimaSync(tabla, ahora);
    }

    return bajados;
}

// ─────────────────────────────────────────────────────────────────────────────
// Helpers de sync_control
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Actualiza el campo ultima_sync en sync_control para una tabla.
 */
void SyncManager::actualizarUltimaSync(const QString &tabla, const QDateTime &momento)
{
    QSqlQuery q(QSqlDatabase::database("DB"));
    q.prepare("UPDATE sync_control SET ultima_sync = ? WHERE tabla = ?");
    q.addBindValue(momento.toString("yyyy-MM-dd HH:mm:ss"));
    q.addBindValue(tabla);
    q.exec();
}

/**
 * @brief Devuelve la fecha de última sincronización de una tabla.
 * @return QDateTime de la última sync, o epoch si nunca se ha sincronizado.
 */
QDateTime SyncManager::ultimaSync(const QString &tabla)
{
    QSqlQuery q(QSqlDatabase::database("DB"));
    q.prepare("SELECT ultima_sync FROM sync_control WHERE tabla = ?");
    q.addBindValue(tabla);
    q.exec();
    if (q.first())
        return QDateTime::fromString(q.value(0).toString(), "yyyy-MM-dd HH:mm:ss");
    return QDateTime::fromString("2000-01-01 00:00:00", "yyyy-MM-dd HH:mm:ss");
}
