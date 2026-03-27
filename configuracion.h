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
  QString getUsuario();
  void setUsuario(QString user);
  int getRol();
  void setRol(int nuevoRol);

  /// @brief Devuelve un puntero al gestor de permisos del rol activo.
  /// Permite consultar permisos desde cualquier parte de la aplicación:
  /// @code conf->permisos()->tiene("articulos.borrar") @endcode
  GestorPermisos *permisos();

private:
  QStringList nombreconexiones;
  QStringList nombreconexionesActivas;
  QString conexionMaster;
  QString conexionLocal;
  int rol;

  /// Gestor de permisos granulares embebido (no es un puntero para
  /// evitar gestión de memoria adicional).
  GestorPermisos m_permisos;
};

#endif // CONFIGURACION_H
