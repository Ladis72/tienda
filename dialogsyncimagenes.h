#ifndef DIALOGSYNCIMAGENES_H
#define DIALOGSYNCIMAGENES_H

#include "base_datos.h"
#include <QDialog>
#include <QList>
#include <QProcess>

namespace Ui {
class DialogSyncImagenes;
}

/**
 * @brief Diálogo para sincronizar la carpeta de imágenes del TPV entre todos los equipos de la red (Tailscale / SSH).
 *
 * Incorpora de forma nativa y visual la funcionalidad del script 'sincronizar-imagenes-tpv':
 *  - Paso 1: Recoge imágenes nuevas o actualizadas de los demás equipos hacia la carpeta local (punto de unión).
 *  - Paso 2: Envía el catálogo completo unificado desde la carpeta local al resto de equipos disponibles.
 */
class DialogSyncImagenes : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSyncImagenes(QWidget *parent = nullptr);
    ~DialogSyncImagenes() override;

private slots:
    /// Examina y selecciona manualmente el directorio local de imágenes
    void on_toolButtonExaminarLocal_clicked();

    /// Añade una fila nueva a la tabla de equipos remotos
    void on_pushButtonAnadirEquipo_clicked();

    /// Elimina la fila seleccionada de la tabla de equipos remotos
    void on_pushButtonQuitarEquipo_clicked();

    /// Restaura la lista original de los 5 equipos predeterminados
    void on_pushButtonRestaurarEquipos_clicked();

    /// Guarda la lista actual de equipos en el archivo tienda.ini global
    void on_pushButtonGuardarEquipos_clicked();

    /// Inicia el proceso completo de sincronización en 2 pasos (Pull + Push)
    void on_pushButtonSincronizar_clicked();

    /// Cancela la sincronización en curso
    void on_pushButtonDetener_clicked();

    /// Cierra el diálogo
    void on_pushButtonCerrar_clicked();

    /// Procesa la salida estándar y de error del proceso SSH/rsync activo
    void onProcesoOutputListo();

    /// Avanza a la siguiente tarea de la cola cuando termina un comando SSH/rsync
    void onProcesoTerminado(int exitCode, QProcess::ExitStatus exitStatus);

private:
    /// Representa una fase individual dentro de la cola de sincronización
    struct TareaSync {
        enum Tipo {
            PullCheckSSH,
            PullRsync,
            PushMkdirSSH,
            PushRsync
        };
        Tipo tipo;
        int filaTabla;
        QString nombre;
        QString host;
        QString rutaRemota;
    };

    Ui::DialogSyncImagenes *ui;
    baseDatos m_base;
    QProcess *m_proceso;
    QList<TareaSync> m_colaTareas;
    int m_totalTareas;
    int m_tareasCompletadas;
    bool m_cancelado;
    int m_archivosTransferidosActual;

    /// Determina la ruta local de imágenes según la configuración y el hostname del equipo
    QString detectarDirectorioLocal() const;

    /// Devuelve la lista predeterminada de equipos del clúster ("Nombre|usuario@IP|ruta_remota")
    static QStringList equiposPredeterminados();

    /// Carga la lista de equipos desde tienda.ini (o los predeterminados) en la tabla
    void cargarEquiposEnTabla(const QStringList &entradas);

    /// Añade una fila a la tabla de equipos
    void agregarFilaEquipo(const QString &nombre, const QString &host, const QString &rutaRemota);

    /// Comprueba si una entrada corresponde a la máquina local donde se ejecuta el programa
    bool esEquipoLocal(const QString &hostUsuarioIp) const;

    /// Cuenta y actualiza en la interfaz el número total de archivos en la carpeta local
    int actualizarContadorArchivosLocales();

    /// Ejecuta la siguiente tarea pendiente de m_colaTareas
    void ejecutarSiguienteTarea();

    /// Añade una línea de texto a la consola de registro y hace scroll automático
    void agregarLog(const QString &texto);

    /// Asegura que una ruta de directorio termine en '/' para el comportamiento correcto de rsync
    static QString normalizarRutaRsync(const QString &ruta);
};

#endif // DIALOGSYNCIMAGENES_H
