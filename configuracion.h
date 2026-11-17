#ifndef CONFIGURACION_H
#define CONFIGURACION_H

#include "gestorpermisos.h"
#include <QString>
#include <QStringList>

/**
 * @brief Clase global de configuración de la aplicación.
 *
 * Almacena el estado de sesión (usuario, rol), las conexiones activas
 * y el gestor de permisos granulares por rol.
 */
class Configuracion {
public:
  Configuracion();
  ~Configuracion();

  QString usuario;

  QStringList getNombreConexiones();
  void setNombreconexiones(QStringList conn);
  QStringList getNombreConexionesActivas();
  void setNombreConexionesActivas(QStringList conn);
  QString getConexionMaster();
  void setConexionMaster(QString conn);
  QString getConexionLocal();
  void setConexionLocal(QString conn);
  
  /// @brief Devuelve la conexión a usar para tablas comunes (Artículos, Clientes, etc.)
  /// Según la configuración, puede ser la local o la nube.
  QString getConexionCommon();
  bool getUsarPreciosLocales();
  void setUsarPreciosLocales(bool value);

  /// @brief Obtiene el ID numérico de la tienda local actual desde la base de datos
  int getIdTienda();

  QString getUsuario();
  void setUsuario(QString user);
  int getRol();
  void setRol(int nuevoRol);

  /// @brief Devuelve un puntero al gestor de permisos del rol activo.
  /// Permite consultar permisos desde cualquier parte de la aplicación:
  /// @code conf->permisos()->tiene("articulos.borrar") @endcode
  GestorPermisos *permisos();

  /// @brief Devuelve el directorio raíz común de la aplicación (p. ej. "/opt/tienda" o applicationDirPath()).
  /// Allí residen de forma compartida las carpetas 'imagenes', 'documentos', 'ia_embeddings.cache' y 'tienda.ini' global.
  static QString rutaDirectorioBase();

  /// @brief Devuelve la ruta absoluta al archivo tienda.ini global compartido (p. ej. "/opt/tienda/tienda.ini").
  static QString rutaIniGlobal();

  /// @brief Devuelve la ruta absoluta al archivo tienda.ini específico del usuario actual de Linux (~/.config/Ladis/tienda.ini).
  static QString rutaIniUsuario();

  /// @brief Comprueba si el usuario actual del sistema operativo tiene una conexión [BaseDatos] propia en su tienda.ini.
  static bool tieneConexionPorUsuario();

  /// @brief Devuelve la ruta del archivo tienda.ini activo para leer la conexión [BaseDatos] y [Local]:
  /// Si el usuario tiene su propia conexión en ~/.config/Ladis/tienda.ini, devuelve rutaIniUsuario();
  /// en caso contrario, devuelve rutaIniGlobal() (/opt/tienda/tienda.ini).
  static QString rutaIniConexion();

private:
  QStringList nombreconexiones;
  QStringList nombreconexionesActivas;
  QString conexionMaster;
  QString conexionLocal;
  bool usarPreciosLocales;
  int rol;

  /// Gestor de permisos granulares embebido (no es un puntero para
  /// evitar gestión de memoria adicional).
  GestorPermisos m_permisos;
};

#endif // CONFIGURACION_H
