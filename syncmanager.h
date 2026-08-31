#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QTimer>
#include <QStringList>
#include <QDateTime>
#include <QFuture>
#include <atomic>

/**
 * @brief Gestiona la sincronización bidireccional entre la BD local y el servidor en la nube.
 *
 * Arquitectura:
 *  - La BD local es la fuente de trabajo del TPV (siempre disponible, incluso sin internet).
 *  - La tabla sync_cola registra automáticamente cada cambio en las tablas maestras (via triggers).
 *  - Este manager sube los cambios pendientes a la nube y baja los cambios remotos cada 5 minutos.
 *  - Si no hay conexión a la nube, reintenta silenciosamente sin interrumpir el TPV.
 *
 * Tablas maestras sincronizadas (bidireccional):
 *   articulos, clientes, familias, fabricantes, proveedores,
 *   codaux, fpago, impuestos, formatos, motivosEntrada, usuarios, permisos
 */
class SyncManager : public QObject
{
    Q_OBJECT

public:
    explicit SyncManager(QObject *parent = nullptr);
    ~SyncManager();

    /// Inicia los timers de ping y sincronización
    void iniciar();

    /// Devuelve true si la última comprobación de conexión fue exitosa
    bool hayConexionNube() const { return m_hayConexion; }

    /// Instancia global del SyncManager
    static SyncManager* instance() { return s_instance; }

    /// Nombre de la conexión Qt a la BD en la nube
    static const QString CONEXION_NUBE;

    /// Resincroniza limpiamente todo el stock de las tiendas conectadas a la tabla stock_tiendas_nube
    static void resincronizarTodoElStockNube();

public slots:
    /// Ejecuta la sincronización completa (inmediata o programada)
    void sincronizar();

signals:
    /// Emitido cuando se recupera la conexión a la nube
    void conexionRecuperada();

    /// Emitido cuando se pierde la conexión a la nube
    void conexionPerdida();

    /// Emitido al completar una sincronización con el número de registros procesados
    void syncCompletado(int subidos, int bajados);

private slots:
    /// Comprueba si la nube está accesible (cada 30 segundos)
    void comprobarConexion();

private:
    static SyncManager *s_instance;
    QTimer *m_timerPing;   ///< Comprobación de conexión cada 30s
    QTimer *m_timerSync;   ///< Sincronización cada 5 minutos
    bool    m_hayConexion; ///< Estado actual de conexión a la nube

    /// Tarea asíncrona del último ciclo de sincronización (hilo del pool)
    QFuture<void> m_futuroSync;

    /// Evita que dos ciclos de sincronización se solapen (timer + ping)
    std::atomic<bool> m_syncActivo;

    // --- Tablas maestras que se sincronizan bidireccionalmente entre tiendas ---
    static const QStringList TABLAS_MAESTRAS;

    // --- Tablas transaccionales locales que se consolidan en la nube (unidireccional: local -> nube) ---
    static const QStringList TABLAS_TRANSACCIONALES;
    
    // --- Campos operativos que no deben sincronizarse para evitar corrupción entre tiendas ---
    static const QMap<QString, QStringList> CAMPOS_EXCLUIDOS;

    // --- ID de la tienda local para trazabilidad ---
    int m_idTiendaLocal;
    void cargarIdTiendaLocal();

    // --- Inicialización de infraestructura SQL ---

    /// Crea la tabla sync_cola y sync_control en la BD local si no existen
    void crearTablasSyncLocal();

    /// Crea los triggers en la BD local para las tablas maestras
    void crearTriggers();

    /// Crea un trigger individual (INSERT o UPDATE) para una tabla
    void crearTrigger(const QString &nombreTabla,
                      const QString &clavePrimaria,
                      const QString &evento); // "INSERT" o "UPDATE"

    // --- Conexión a la nube ---

    /// Abre la conexión a la nube usando los datos de config_nube
    bool conectarNube();

    /// Cierra y elimina la conexión a la nube
    void desconectarNube();

    // --- Operaciones de sync ---
    // Reciben los nombres de las conexiones Qt (local y nube) a usar, porque
    // el ciclo pesado se ejecuta en un hilo del pool con conexiones clonadas.

    /// Sube los registros de sync_cola pendientes (subido=0) a la nube
    int subirCambios(const QString &connLocal, const QString &connNube, const QString &usuario);

    /// Sube las unificaciones de maestros pendientes (sync_unificaciones) a la nube
    int subirUnificaciones(const QString &connLocal, const QString &connNube, const QString &usuario);

    /// Baja los cambios de la nube más recientes que ultima_sync para cada tabla
    int bajarCambios(const QString &connLocal, const QString &connNube, const QString &usuario);

    /// Baja los borrados (tombstones) de la nube y los aplica localmente
    int bajarBorrados(const QString &connLocal, const QString &connNube, const QString &usuario);

    /// Realiza un volcado inicial de lotes a stock_tiendas_nube si no se ha realizado previamente
    void sincronizarLotesInicial(const QString &connLocal, const QString &connNube, const QString &usuario);

    /// Purga registros antiguos ya subidos de sync_cola y sync_unificaciones
    void purgarCola(const QString &connLocal, const QString &usuario);

    /// Escribe un mensaje en la tabla logs usando la conexión local indicada.
    /// Funciona con las conexiones clonadas del hilo de pool (connLocal).
    void registrarLog(const QString &connLocal, const QString &categoria,
                      const QString &usuario, const QString &mensaje);

    /// Actualiza el campo ultima_sync de sync_control para una tabla
    void actualizarUltimaSync(const QString &tabla, const QDateTime &momento,
                              const QString &connLocal);

    /// Obtiene la última sync de una tabla
    QDateTime ultimaSync(const QString &tabla, const QString &connLocal, const QString &usuario);

    /// Asegura que las tablas maestras en la nube tengan las columnas id_tienda_origen y updated_at
    void prepararTablasRemotas();

    /// Devuelve el nombre de la clave primaria para una tabla maestra
    QString getPkTabla(const QString &tabla) const;
};

#endif // SYNCMANAGER_H
