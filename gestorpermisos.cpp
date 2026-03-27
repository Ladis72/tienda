#include "gestorpermisos.h"

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

GestorPermisos::GestorPermisos() {}

/**
 * @brief Carga los permisos del rol dado desde la tabla `permisos`.
 *
 * Borra los permisos previos del QSet y los reemplaza con los del nuevo rol.
 * Si el rol es negativo (sin sesión), el conjunto queda vacío → todo denegado.
 */
void GestorPermisos::cargarPermisos(int rol, const QString &conexion) {
  m_permisos.clear();

  // Sin sesión = sin permisos
  if (rol < 0) {
    return;
  }

  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen()) {
    qDebug() << "GestorPermisos: conexión no abierta:" << conexion;
    return;
  }

  QSqlQuery query(db);
  query.prepare("SELECT clave FROM permisos WHERE rol = ?");
  query.bindValue(0, rol);
  if (query.exec()) {
    while (query.next()) {
      m_permisos.insert(query.value(0).toString());
    }
  } else {
    qDebug() << "GestorPermisos::cargarPermisos error:" << query.lastError().text();
  }

  qDebug() << "GestorPermisos: cargados" << m_permisos.size()
           << "permisos para rol" << rol;
}

/**
 * @brief Consulta si el rol activo tiene el permiso indicado.
 * @param clave Identificador como "ventas", "articulos.borrar", etc.
 */
bool GestorPermisos::tiene(const QString &clave) const {
  return m_permisos.contains(clave);
}

/**
 * @brief Devuelve todas las claves de permiso distintas que existen en la BD.
 */
QStringList GestorPermisos::todasLasClaves(const QString &conexion) {
  QStringList claves;
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen())
    return claves;

  QSqlQuery query(db);
  if (query.exec("SELECT DISTINCT clave FROM permisos ORDER BY clave")) {
    while (query.next()) {
      claves.append(query.value(0).toString());
    }
  }
  return claves;
}

/**
 * @brief Devuelve las claves concedidas a un rol específico.
 */
QStringList GestorPermisos::clavesDelRol(int rol, const QString &conexion) {
  QStringList claves;
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen())
    return claves;

  QSqlQuery query(db);
  query.prepare("SELECT clave FROM permisos WHERE rol = ? ORDER BY clave");
  query.bindValue(0, rol);
  if (query.exec()) {
    while (query.next()) {
      claves.append(query.value(0).toString());
    }
  }
  return claves;
}

/**
 * @brief Concede un permiso a un rol. No produce error si ya existe.
 */
bool GestorPermisos::conceder(int rol, const QString &clave,
                              const QString &conexion) {
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen())
    return false;

  QSqlQuery query(db);
  query.prepare(
      "INSERT IGNORE INTO permisos (rol, clave) VALUES (?, ?)");
  query.bindValue(0, rol);
  query.bindValue(1, clave);
  if (!query.exec()) {
    qDebug() << "GestorPermisos::conceder error:" << query.lastError().text();
    return false;
  }
  return true;
}

/**
 * @brief Revoca un permiso de un rol.
 */
bool GestorPermisos::revocar(int rol, const QString &clave,
                             const QString &conexion) {
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen())
    return false;

  QSqlQuery query(db);
  query.prepare("DELETE FROM permisos WHERE rol = ? AND clave = ?");
  query.bindValue(0, rol);
  query.bindValue(1, clave);
  if (!query.exec()) {
    qDebug() << "GestorPermisos::revocar error:" << query.lastError().text();
    return false;
  }
  return true;
}

/**
 * @brief Nombres descriptivos de los 5 roles del sistema.
 *
 * - 0: Administrador   → acceso total
 * - 1: Encargado       → gestión avanzada sin acceso a configuración del sistema
 * - 2: Vendedor        → ventas, consultas básicas y gestión de stock
 * - 3: Cajero          → solo TPV y funciones mínimas de caja
 * - 4: Almacén         → gestión de stock, entradas, salidas y pedidos
 */
QStringList GestorPermisos::nombresRoles() {
  return {"Administrador", "Encargado", "Vendedor", "Cajero", "Almacén"};
}

/**
 * @brief Crea la tabla `permisos` si no existe y la rellena
 * con los permisos por defecto para los 5 roles.
 *
 * Utiliza IF NOT EXISTS y INSERT IGNORE para ser idempotente:
 * se puede llamar en cada arranque sin riesgo de duplicar datos.
 */
void GestorPermisos::inicializar(const QString &conexion) {
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen()) {
    qDebug() << "GestorPermisos::inicializar: conexión no disponible";
    return;
  }

  QSqlQuery query(db);

  // Crear la tabla si no existe
  QString crearTabla =
      "CREATE TABLE IF NOT EXISTS permisos ("
      "  id    INT AUTO_INCREMENT PRIMARY KEY,"
      "  rol   INT NOT NULL,"
      "  clave VARCHAR(64) NOT NULL,"
      "  UNIQUE KEY uk_rol_clave (rol, clave)"
      ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4";

  if (!query.exec(crearTabla)) {
    qDebug() << "GestorPermisos: error creando tabla permisos:"
             << query.lastError().text();
    return;
  }

  // Comprobar si ya tiene datos (para no hacer INSERTs innecesarios)
  query.exec("SELECT COUNT(*) FROM permisos");
  if (query.next() && query.value(0).toInt() > 0) {
    qDebug() << "GestorPermisos: tabla permisos ya tiene datos, omitiendo inserts por defecto";
    return;
  }

  // Insertar permisos por defecto para los 5 roles
  insertarPermisosPorDefecto(conexion);
  qDebug() << "GestorPermisos: permisos inicializados correctamente";
}

/**
 * @brief Inserta los permisos por defecto de los 5 roles.
 *
 * Catálogo completo de permisos con notación granular:
 * - Claves simples: "ventas", "estadisticas", "tab_config" → acceso al módulo
 * - Claves granulares: "articulos.crear", "articulos.borrar" → acción específica
 *
 * ROL 0 (Admin):      TODO
 * ROL 1 (Encargado):  Gestión avanzada, sin config sistema ni estadísticas
 * ROL 2 (Vendedor):   Ventas, consultas, stock básico
 * ROL 3 (Cajero):     Solo TPV y caja
 * ROL 4 (Almacén):    Stock, entradas, salidas, pedidos, etiquetas
 */
void GestorPermisos::insertarPermisosPorDefecto(const QString &conexion) {
  QSqlDatabase db = QSqlDatabase::database(conexion);
  if (!db.isOpen())
    return;

  // ── Catálogo completo de claves de permiso ──
  // Cada clave es una cadena identificadora de una funcionalidad o acción.
  //
  // Convención de nombres:
  //   "modulo"           → acceso general (abrir ventana / ver datos)
  //   "modulo.crear"     → crear nuevos registros
  //   "modulo.modificar" → editar registros existentes
  //   "modulo.borrar"    → eliminar registros

  // Estructura: {clave, {roles que la tienen}}
  struct PermisoDefault {
    const char *clave;
    QList<int> roles; // Roles a los que se concede
  };

  QList<PermisoDefault> catalogo = {
      // ── Ventas / TPV ──
      {"ventas",                    {0, 1, 2, 3}},

      // ── Artículos ──
      {"articulos",                 {0, 1, 2, 4}},
      {"articulos.crear",           {0, 1}},
      {"articulos.modificar",       {0, 1}},
      {"articulos.borrar",          {0}},

      // ── Familias ──
      {"familias",                  {0, 1}},
      {"familias.crear",            {0, 1}},
      {"familias.modificar",        {0, 1}},
      {"familias.borrar",           {0}},

      // ── Fabricantes ──
      {"fabricantes",               {0, 1}},
      {"fabricantes.crear",         {0, 1}},
      {"fabricantes.modificar",     {0, 1}},
      {"fabricantes.borrar",        {0}},

      // ── Clientes ──
      {"clientes",                  {0, 1, 2}},
      {"clientes.crear",            {0, 1}},
      {"clientes.modificar",        {0, 1}},
      {"clientes.borrar",           {0}},

      // ── Proveedores ──
      {"proveedores",               {0, 1}},
      {"proveedores.crear",         {0, 1}},
      {"proveedores.modificar",     {0, 1}},
      {"proveedores.borrar",        {0}},

      // ── Formas de pago ──
      {"formas_pago",               {0, 1}},
      {"formas_pago.crear",         {0}},
      {"formas_pago.modificar",     {0}},
      {"formas_pago.borrar",        {0}},

      // ── Prestamistas ──
      {"prestamistas",              {0, 1}},
      {"prestamistas.crear",        {0, 1}},
      {"prestamistas.modificar",    {0, 1}},
      {"prestamistas.borrar",       {0}},

      // ── Usuarios ──
      {"usuarios",                  {0}},
      {"usuarios.crear",            {0}},
      {"usuarios.modificar",        {0}},
      {"usuarios.borrar",           {0}},

      // ── Tiendas ──
      {"tiendas",                   {0}},

      // ── Formatos ──
      {"formatos",                  {0}},

      // ── Tipos entrada/salida ──
      {"tipos_entrada_salida",      {0}},

      // ── Etiquetas ──
      {"etiquetas",                 {0, 1, 4}},

      // ── Entrada de mercancía ──
      {"entradas",                  {0, 1, 4}},
      {"entradas.crear",            {0, 1, 4}},

      // ── Salidas ──
      {"salidas",                   {0, 1, 4}},
      {"salidas.crear",             {0, 1, 4}},

      // ── Venta por artículo (listado) ──
      {"venta_articulos",           {0, 1}},

      // ── Caducidades ──
      {"caducidades",               {0, 1, 4}},

      // ── Caducados ──
      {"caducados",                 {0, 1, 4}},

      // ── Movimientos E/S ──
      {"movimientos",               {0, 1, 4}},

      // ── Tickets ──
      {"tickets",                   {0, 1}},
      {"tickets.borrar",            {0}},

      // ── Facturas ──
      {"facturas",                  {0, 1}},

      // ── Albaranes ──
      {"albaranes",                 {0, 1}},

      // ── Facturar albaranes ──
      {"facturar",                  {0, 1}},

      // ── Gestión de pedidos ──
      {"gestionar_pedidos",         {0, 1, 4}},

      // ── Cajas ──
      {"cajas",                     {0, 1, 3}},

      // ── Préstamos ──
      {"prestamos",                 {0, 1}},

      // ── Generar vales ──
      {"generar_vales",             {0}},

      // ── Actualizar clientes ──
      {"actualizar_clientes",       {0}},

      // ── Listados ──
      {"listado_ventas",            {0, 1}},
      {"listado_movimientos",       {0}},
      {"listado_arqueos",           {0}},
      {"listado_caducados",         {0, 1, 4}},

      // ── Estadísticas ──
      {"estadisticas",              {0}},

      // ── Configuración ──
      {"config_ticket",             {0}},
      {"configuracion",             {0}},
      {"informes",                  {0}},
      {"impuestos",                 {0, 1}},
      {"copia_seguridad",           {0, 1}},
      {"conectar",                  {0, 1}},
      {"config_base",               {0}},
      {"tab_config",                {0}},

      // ── Preparar (pedidos) ──
      {"preparar",                  {0, 1, 4}},
  };

  // Insertar todas las filas con INSERT IGNORE (idempotente)
  QSqlQuery query(db);
  query.prepare("INSERT IGNORE INTO permisos (rol, clave) VALUES (?, ?)");

  for (const auto &p : catalogo) {
    for (int rol : p.roles) {
      query.bindValue(0, rol);
      query.bindValue(1, QString(p.clave));
      if (!query.exec()) {
        qDebug() << "GestorPermisos: error insertando" << p.clave
                 << "para rol" << rol << ":" << query.lastError().text();
      }
    }
  }
}
