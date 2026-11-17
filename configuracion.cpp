#include "configuracion.h"
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStandardPaths>

Configuracion::Configuracion() {
  usarPreciosLocales = false;
  rol = -1;
  usuario = "";
}

Configuracion::~Configuracion() {}

QStringList Configuracion::getNombreConexiones() { return nombreconexiones; }

void Configuracion::setNombreconexiones(QStringList conn) {
  nombreconexiones = conn;
}

QStringList Configuracion::getNombreConexionesActivas() {
  return nombreconexionesActivas;
}

void Configuracion::setNombreConexionesActivas(QStringList conn) {
  nombreconexionesActivas = conn;
}

QString Configuracion::getConexionMaster() { return conexionMaster; }

void Configuracion::setConexionMaster(QString conn) { conexionMaster = conn; }

QString Configuracion::getConexionLocal() { return conexionLocal; }

void Configuracion::setConexionLocal(QString conn) { conexionLocal = conn; }

QString Configuracion::getConexionCommon() {
  // Las tablas comunes (Artículos, Clientes, Familias) ya se sincronizan 
  // via SyncManager, por lo que siempre usamos la conexión local para evitar latencia.
  return conexionLocal;
}

bool Configuracion::getUsarPreciosLocales() { return usarPreciosLocales; }

void Configuracion::setUsarPreciosLocales(bool value) { usarPreciosLocales = value; }

/**
 * @brief Obtiene el identificador único de la tienda local actual.
 * Realiza una consulta rápida a la tabla de tiendas donde local = 1.
 * Si no se encuentra, retorna 1 por defecto.
 * @return ID de la tienda local.
 */
int Configuracion::getIdTienda() {
  QSqlQuery q(QSqlDatabase::database(conexionLocal));
  if (q.exec("SELECT id FROM tiendas WHERE local = 1 LIMIT 1") && q.first()) {
    return q.value(0).toInt();
  }
  return 1;
}

QString Configuracion::getUsuario() { return usuario; }

void Configuracion::setUsuario(QString user) { usuario = user; }

int Configuracion::getRol() { return rol; }

/**
 * @brief Establece el rol del usuario y recarga los permisos asociados.
 *
 * Al cambiar de rol se vacían los permisos anteriores y se cargan los
 * del nuevo rol desde la base de datos. Si el rol es negativo (sin sesión),
 * el gestor queda vacío → todo denegado.
 */
void Configuracion::setRol(int nuevoRol) {
  rol = nuevoRol;
  // Recargar permisos del nuevo rol desde la BD
  m_permisos.cargarPermisos(rol, conexionLocal);
}

/**
 * @brief Devuelve puntero al gestor de permisos para consultas rápidas.
 * @return Puntero al GestorPermisos embebido (no es null).
 */
GestorPermisos *Configuracion::permisos() { return &m_permisos; }

/**
 * @brief Determina el directorio raíz compartido de la instalación.
 *
 * Prioriza el directorio donde reside el ejecutable si contiene tienda.ini o la carpeta imagenes/.
 * Si no, comprueba si existe la instalación global en /opt/tienda.
 */
QString Configuracion::rutaDirectorioBase() {
  const QString appDir = QCoreApplication::applicationDirPath();
  if (QFile::exists(appDir + "/tienda.ini") ||
      QDir(appDir + "/imagenes").exists() ||
      appDir.startsWith("/opt/tienda")) {
    return appDir;
  }
  if (QDir("/opt/tienda").exists() &&
      (QFile::exists("/opt/tienda/tienda.ini") || QDir("/opt/tienda/imagenes").exists())) {
    return QStringLiteral("/opt/tienda");
  }
  return appDir;
}

/**
 * @brief Devuelve la ruta absoluta al archivo tienda.ini global compartido.
 * Busca en el directorio base, junto al ejecutable, en build/ o en /opt/tienda/tienda.ini.
 */
QString Configuracion::rutaIniGlobal() {
  const QString appDir = QCoreApplication::applicationDirPath();
  const QStringList candidatas = {
      appDir + "/tienda.ini",
      rutaDirectorioBase() + "/tienda.ini",
      "/opt/tienda/tienda.ini",
      appDir + "/build/tienda.ini",
      appDir + "/../tienda.ini"
  };

  for (const QString &ruta : candidatas) {
    QFileInfo fi(ruta);
    if (fi.exists() && fi.isReadable()) {
      return fi.absoluteFilePath();
    }
  }
  return rutaDirectorioBase() + "/tienda.ini";
}

/**
 * @brief Devuelve la ruta absoluta al archivo tienda.ini personal del usuario de Linux (~/.config/Ladis/tienda.ini).
 * Garantiza que el directorio padre (~/.config/Ladis) exista.
 */
QString Configuracion::rutaIniUsuario() {
  QString configDir = QStandardPaths::writableLocation(QStandardPaths::GenericConfigLocation);
  if (configDir.isEmpty()) {
    configDir = QDir::homePath() + "/.config";
  }
  QDir dir(configDir + "/Ladis");
  if (!dir.exists()) {
    dir.mkpath(".");
  }
  return dir.filePath("tienda.ini");
}

/**
 * @brief Indica si el usuario actual de Linux tiene definida una conexión [BaseDatos] propia.
 */
bool Configuracion::tieneConexionPorUsuario() {
  const QString userIni = rutaIniUsuario();
  if (!QFile::exists(userIni)) {
    return false;
  }
  QSettings s(userIni, QSettings::IniFormat);
  return !s.value("BaseDatos/usuario", "").toString().trimmed().isEmpty();
}

/**
 * @brief Devuelve el archivo tienda.ini del que se debe leer la conexión local ([BaseDatos] / [Local]).
 * Si el usuario tiene su propia configuración en ~/.config/Ladis/tienda.ini, la prioriza;
 * si no, utiliza el archivo global compartido (/opt/tienda/tienda.ini).
 */
QString Configuracion::rutaIniConexion() {
  if (tieneConexionPorUsuario()) {
    return rutaIniUsuario();
  }
  return rutaIniGlobal();
}

