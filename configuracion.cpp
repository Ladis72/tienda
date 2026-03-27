#include "configuracion.h"

Configuracion::Configuracion() {}

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
