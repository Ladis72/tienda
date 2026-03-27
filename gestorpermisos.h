#ifndef GESTORPERMISOS_H
#define GESTORPERMISOS_H

#include <QSet>
#include <QString>
#include <QStringList>

/**
 * @brief Sistema modular de permisos granulares por rol.
 *
 * Cada funcionalidad del programa tiene una clave de permiso (ej. "ventas",
 * "articulos.crear", "estadisticas"). Los permisos se almacenan en la tabla
 * MySQL `permisos` como pares (rol, clave).
 *
 * Soporta 5 roles (0-4) y permisos granulares con notación de punto:
 *   - "articulos"         → acceso general al módulo
 *   - "articulos.crear"   → puede crear artículos
 *   - "articulos.borrar"  → puede borrar artículos
 *
 * Uso típico:
 * @code
 *   // Cargar permisos del rol tras el login
 *   gestorPermisos.cargarPermisos(rol, conexion);
 *
 *   // Consultar si el usuario tiene permiso
 *   if (gestorPermisos.tiene("estadisticas")) { ... }
 *   if (gestorPermisos.tiene("articulos.borrar")) { ... }
 * @endcode
 */
class GestorPermisos {
public:
  GestorPermisos();

  /// @brief Carga todos los permisos del rol indicado desde la BD.
  /// Se llama automáticamente al cambiar de rol en Configuracion::setRol().
  void cargarPermisos(int rol, const QString &conexion);

  /// @brief Consulta si el rol activo tiene el permiso indicado.
  /// @param clave Identificador del permiso (ej. "ventas", "articulos.borrar")
  /// @return true si el permiso está concedido, false en caso contrario.
  bool tiene(const QString &clave) const;

  /// @brief Devuelve todas las claves de permiso registradas en el sistema.
  /// Útil para la UI de edición de permisos por rol.
  static QStringList todasLasClaves(const QString &conexion);

  /// @brief Devuelve las claves concedidas a un rol específico.
  static QStringList clavesDelRol(int rol, const QString &conexion);

  /// @brief Concede un permiso a un rol (INSERT IGNORE).
  static bool conceder(int rol, const QString &clave,
                       const QString &conexion);

  /// @brief Revoca un permiso de un rol (DELETE).
  static bool revocar(int rol, const QString &clave,
                      const QString &conexion);

  /// @brief Crea la tabla `permisos` si no existe y la rellena
  /// con los valores por defecto para los 5 roles.
  /// Se llama una vez al inicio de la aplicación.
  static void inicializar(const QString &conexion);

  /// @brief Nombres descriptivos de los 5 roles del sistema.
  /// Índice = nº de rol.
  static QStringList nombresRoles();

private:
  /// Conjunto de claves concedidas al rol activo.
  /// Permite consultas O(1) con tiene().
  QSet<QString> m_permisos;

  /// @brief Inserta los permisos por defecto de todos los roles.
  /// Se llama desde inicializar() la primera vez.
  static void insertarPermisosPorDefecto(const QString &conexion);
};

#endif // GESTORPERMISOS_H
