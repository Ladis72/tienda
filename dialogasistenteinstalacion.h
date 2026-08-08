#ifndef DIALOGASISTENTEINSTALACION_H
#define DIALOGASISTENTEINSTALACION_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class DialogAsistenteInstalacion;
}

/**
 * @class DialogAsistenteInstalacion
 * @brief Asistente interactivo de instalación y primer despliegue para Tienda 2.0.
 *
 * Permite al usuario seleccionar entre 3 modos de instalación:
 * 1. Nueva Empresa / Tienda Principal (Instalación Matriz inicial).
 * 2. Unirse a un Grupo de Tiendas Existente (Nueva sucursal / Tienda remota con sincronización Nube).
 * 3. Añadir Punto de Venta (TPV Secundario) a una tienda ya existente (red local compartiendo la misma base de datos).
 */
class DialogAsistenteInstalacion : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del asistente de instalación.
     * @param parent Widget padre opcional.
     */
    explicit DialogAsistenteInstalacion(QWidget *parent = nullptr);
    ~DialogAsistenteInstalacion();

private slots:
    /// Slot para cambiar de página según el modo seleccionado en los radio buttons
    void on_radioNuevaEmpresa_toggled(bool checked);
    void on_radioUnirGrupo_toggled(bool checked);
    void on_radioNuevoTPV_toggled(bool checked);

    /// Slot para probar la conexión según los parámetros de la página activa
    void on_pushButtonTest_clicked();

    /// Slot para validar, guardar la configuración en tienda.ini e iniciar el sistema
    void on_pushButtonGuardar_clicked();

    /// Slot para cancelar la instalación y cerrar la aplicación
    void on_pushButtonCancelar_clicked();

private:
    Ui::DialogAsistenteInstalacion *ui; ///< Interfaz gráfica de Qt Creator (.ui)
    baseDatos m_base;                   ///< Objeto de acceso a la base de datos

    /**
     * @brief Realiza la prueba de conexión a MariaDB según los parámetros pasados.
     * @param host Host o IP del servidor.
     * @param puerto Puerto de conexión.
     * @param bd Nombre de la base de datos.
     * @param usuario Usuario de acceso.
     * @param clave Contraseña de acceso.
     * @return true si la conexión tuvo éxito, false en caso contrario.
     */
    bool probarConexion(const QString &host, const QString &puerto, const QString &bd,
                        const QString &usuario, const QString &clave);

    /**
     * @brief Guarda las credenciales principales en el archivo tienda.ini.
     */
    void guardarConfiguracionIni(const QString &host, const QString &puerto,
                                 const QString &bd, const QString &usuario,
                                 const QString &clave, const QString &nombreTienda,
                                 const QString &hostNube = "", const QString &bdNube = "",
                                 const QString &userNube = "", const QString &passNube = "");

    /**
     * @brief Crea la base de datos completa e importa el esquema SQL si no existe.
     */
    bool crearBDYEsquema(const QString &host, const QString &puerto, const QString &bd,
                         const QString &usuario, const QString &clave, const QString &resourcePath);

    /**
     * @brief Ejecuta línea a línea un script de sentencias SQL.
     */
    bool ejecutarSqlScript(QSqlDatabase &db, const QString &resourcePath);

    /**
     * @brief Importa una tabla completa de la nube a la base de datos local.
     */
    bool importarTablaDesdeNube(const QString &nombreTabla, QSqlDatabase &dbLocal, QSqlDatabase &dbNube);
};

#endif // DIALOGASISTENTEINSTALACION_H
