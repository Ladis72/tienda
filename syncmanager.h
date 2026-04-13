#ifndef SYNCMANAGER_H
#define SYNCMANAGER_H

#include <QObject>
#include <QTimer>
#include <QStringList>
#include <QDateTime>

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

    /// Nombre de la conexión Qt a la BD en la nube
    static const QString CONEXION_NUBE;

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

    /// Ejecuta la sincronización completa (cada 5 minutos si hay conexión)
    void sincronizar();

private:
    QTimer *m_timerPing;   ///< Comprobación de conexión cada 30s
    QTimer *m_timerSync;   ///< Sincronización cada 5 minutos
    bool    m_hayConexion; ///< Estado actual de conexión a la nube

    // --- Tablas maestras que se sincronizan (sin las de solo local) ---
    static const QStringList TABLAS_MAESTRAS;
    
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

    /// Sube los registros de sync_cola pendientes (subido=0) a la nube
    int subirCambios();

    /// Baja los cambios de la nube más recientes que ultima_sync para cada tabla
    int bajarCambios();

    /// Actualiza el campo ultima_sync de sync_control para una tabla
    void actualizarUltimaSync(const QString &tabla, const QDateTime &momento);

    /// Obtiene la última sync de una tabla
    QDateTime ultimaSync(const QString &tabla);

    /// Devuelve el nombre de la clave primaria para una tabla maestra
    QString getPkTabla(const QString &tabla) const;
};

#endif // SYNCMANAGER_H
