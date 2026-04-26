#include "editorpermisos.h"
#include "ui_editorpermisos.h"

#include <QGroupBox>
#include <QLabel>
#include <QMessageBox>
#include <QVBoxLayout>

/**
 * @brief Construye el diálogo de edición de permisos.
 *
 * Rellena el combo de roles con los nombres definidos en GestorPermisos,
 * crea los checkboxes agrupados por módulo y preselecciona el rol indicado.
 *
 * @param rolInicial Rol a preseleccionar (normalmente el del usuario editado).
 * @param conexion Nombre de la conexión a BD.
 */
EditorPermisos::EditorPermisos(int rolInicial, const QString &conexion,
                               QWidget *parent)
    : QDialog(parent), ui(new Ui::EditorPermisos), m_conexion(conexion) {
  ui->setupUi(this);

  // Rellenar combo con los 5 roles
  QStringList roles = GestorPermisos::nombresRoles();
  for (int i = 0; i < roles.size(); ++i) {
    ui->comboRol->addItem(
        QString("%1 - %2").arg(i).arg(roles.at(i)), i);
  }

  // Crear los checkboxes con todas las claves de permiso
  construirCheckboxes();



  // Seleccionar el rol inicial (dispara on_comboRol_currentIndexChanged)
  int indice = (rolInicial >= 0 && rolInicial < roles.size()) ? rolInicial : 0;
  ui->comboRol->setCurrentIndex(indice);
}

EditorPermisos::~EditorPermisos() { delete ui; }

/**
 * @brief Crea dinámicamente los checkboxes agrupados por módulo.
 *
 * Agrupa las claves por el prefijo antes del punto (ej. "articulos.crear"
 * se agrupa bajo "Artículos"). Las claves sin punto van al grupo "General".
 * Cada grupo es un QGroupBox dentro del scroll area.
 */
void EditorPermisos::construirCheckboxes() {
  // Obtener todas las claves de permiso del sistema
  QStringList todasClaves = GestorPermisos::todasLasClaves(m_conexion);

  // Agrupar claves por módulo (prefijo antes del punto)
  // Ej: "articulos" → grupo "articulos", "articulos.crear" → grupo "articulos"
  QMap<QString, QStringList> grupos;
  for (const QString &clave : todasClaves) {
    int pos = clave.indexOf('.');
    QString grupo = (pos > 0) ? clave.left(pos) : clave;
    grupos[grupo].append(clave);
  }

  // Descripciones legibles para las claves de permiso
  QMap<QString, QString> descripciones = {
      // ── Módulos principales ──
      {"ventas", "Acceso al TPV"},
      {"articulos", "Ver artículos"},
      {"articulos.crear", "Crear artículos"},
      {"articulos.modificar", "Modificar artículos"},
      {"articulos.borrar", "Borrar artículos"},
      {"familias", "Gestión de familias"},
      {"familias.crear", "Crear familias"},
      {"familias.modificar", "Modificar familias"},
      {"familias.borrar", "Borrar familias"},
      {"fabricantes", "Gestión de fabricantes"},
      {"fabricantes.crear", "Crear fabricantes"},
      {"fabricantes.modificar", "Modificar fabricantes"},
      {"fabricantes.borrar", "Borrar fabricantes"},
      {"clientes", "Ver clientes"},
      {"clientes.crear", "Crear clientes"},
      {"clientes.modificar", "Modificar clientes"},
      {"clientes.borrar", "Borrar clientes"},
      {"proveedores", "Gestión de proveedores"},
      {"proveedores.crear", "Crear proveedores"},
      {"proveedores.modificar", "Modificar proveedores"},
      {"proveedores.borrar", "Borrar proveedores"},
      {"formas_pago", "Formas de pago"},
      {"formas_pago.crear", "Crear formas de pago"},
      {"formas_pago.modificar", "Modificar formas de pago"},
      {"formas_pago.borrar", "Borrar formas de pago"},
      {"usuarios", "Gestión de usuarios"},
      {"usuarios.crear", "Crear usuarios"},
      {"usuarios.modificar", "Modificar usuarios"},
      {"usuarios.borrar", "Borrar usuarios"},
      // ── Operaciones ──
      {"tiendas", "Configuración de tiendas"},
      {"formatos", "Gestión de formatos"},
      {"tipos_entrada_salida", "Tipos de entrada/salida"},
      {"etiquetas", "Impresión de etiquetas"},
      {"entradas", "Entrada de mercancía"},
      {"entradas.crear", "Crear entradas"},
      {"salidas", "Salidas de mercancía"},
      {"salidas.crear", "Crear salidas"},
      {"venta_articulos", "Listado venta artículos"},
      {"caducidades", "Gestión de caducidades"},
      {"caducados", "Productos caducados"},
      {"movimientos", "Movimientos E/S"},
      {"tickets", "Histórico de tickets"},
      {"tickets.borrar", "Borrar tickets"},
      {"facturas", "Facturas"},
      {"albaranes", "Albaranes"},
      {"facturar", "Facturar albaranes"},
      {"gestionar_pedidos", "Gestión de pedidos"},
      {"cajas", "Cajas"},

      {"preparar", "Preparar pedidos"},
      // ── Administración ──
      {"generar_vales", "Generar vales"},
      {"actualizar_clientes", "Actualizar clientes"},
      {"listado_ventas", "Listado de ventas"},
      {"listado_movimientos", "Listado de movimientos"},
      {"listado_arqueos", "Listado de arqueos"},
      {"listado_caducados", "Listado de caducados"},
      {"estadisticas", "Panel de estadísticas"},
      {"config_ticket", "Configuración de ticket"},
      {"configuracion", "Configuración general"},
      {"informes", "Directorios / informes"},
      {"impuestos", "Gestión de impuestos"},
      {"copia_seguridad", "Copia de seguridad"},
      {"conectar", "Conectar tiendas remotas"},
      {"config_base", "Configuración de BD"},
      {"tab_config", "Pestaña de administración"},

      // ── Nuevas funcionalidades y granularidad TPV ──
      {"notas", "Notas y Avisos"},
      {"encargos", "Sistema de Encargos"},
      {"editor_permisos", "Editor de Permisos"},
      {"verifactu", "Logs Verifactu"},
      {"tpv.anadir", "TPV: Añadir artículo"},
      {"tpv.borrar", "TPV: Borrar línea"},
      {"tpv.borrar_todo", "TPV: Borrar todo"},
      {"tpv.cobrar", "TPV: Cobrar"},
      {"tpv.preticket", "TPV: Pre-ticket"},
      {"tpv.hacer_encargo", "TPV: Crear encargo"},
      {"tpv.gestor_encargos", "TPV: Gestor encargos"},
  };

  // Nombres legibles para los grupos
  QMap<QString, QString> nombresGrupos = {
      {"ventas", "Ventas"},
      {"articulos", "Artículos"},
      {"familias", "Familias"},
      {"fabricantes", "Fabricantes"},
      {"clientes", "Clientes"},
      {"proveedores", "Proveedores"},
      {"formas_pago", "Formas de Pago"},

      {"usuarios", "Usuarios"},
      {"tiendas", "Tiendas"},
      {"formatos", "Formatos"},
      {"tipos_entrada_salida", "Tipos E/S"},
      {"etiquetas", "Etiquetas"},
      {"entradas", "Entradas"},
      {"salidas", "Salidas"},
      {"venta_articulos", "Venta Artículos"},
      {"caducidades", "Caducidades"},
      {"caducados", "Caducados"},
      {"movimientos", "Movimientos"},
      {"tickets", "Tickets"},
      {"facturas", "Facturas"},
      {"albaranes", "Albaranes"},
      {"facturar", "Facturar"},
      {"gestionar_pedidos", "Pedidos"},
      {"cajas", "Cajas"},

      {"preparar", "Preparar"},
      {"generar_vales", "Vales"},
      {"actualizar_clientes", "Act. Clientes"},
      {"listado_ventas", "Listado Ventas"},
      {"listado_movimientos", "List. Movimientos"},
      {"listado_arqueos", "List. Arqueos"},
      {"listado_caducados", "List. Caducados"},
      {"estadisticas", "Estadísticas"},
      {"config_ticket", "Config. Ticket"},
      {"configuracion", "Configuración"},
      {"informes", "Informes"},
      {"impuestos", "Impuestos"},
      {"copia_seguridad", "Copia Seguridad"},
      {"conectar", "Conectar"},
      {"config_base", "Config. BD"},
      {"tab_config", "Tab Config"},

      // Grupos para las nuevas funcionalidades
      {"tpv", "TPV (Granular)"},
      {"notas", "Notas"},
      {"encargos", "Encargos"},
      {"editor_permisos", "Permisos"},
      {"verifactu", "VeriFactu"},
  };

  // Crear un QGroupBox por cada grupo de permisos
  for (auto it = grupos.constBegin(); it != grupos.constEnd(); ++it) {
    QString nombreGrupo =
        nombresGrupos.value(it.key(), it.key());
    QGroupBox *groupBox = new QGroupBox(nombreGrupo, this);
    QVBoxLayout *groupLayout = new QVBoxLayout(groupBox);
    groupLayout->setSpacing(2);
    groupLayout->setContentsMargins(8, 4, 8, 4);

    // Crear un checkbox por cada clave del grupo
    for (const QString &clave : it.value()) {
      QString texto = descripciones.value(clave, clave);
      QCheckBox *check = new QCheckBox(texto, groupBox);
      check->setProperty("permisoClave", clave);
      groupLayout->addWidget(check);
      m_checks.insert(clave, check);
    }

    ui->layoutPermisos->addWidget(groupBox);
  }

  // Spacer al final para que los grupos no se estiren verticalmente
  ui->layoutPermisos->addStretch();
}

/**
 * @brief Slot que se dispara al cambiar el rol seleccionado en el combo.
 *
 * Lee las claves concedidas al nuevo rol desde la BD y marca/desmarca
 * los checkboxes correspondientes.
 */
void EditorPermisos::on_comboRol_currentIndexChanged(int index) {
  int rol = ui->comboRol->itemData(index).toInt();
  actualizarChecks(rol);
}

/**
 * @brief Actualiza los checkboxes según los permisos del rol indicado.
 */
void EditorPermisos::actualizarChecks(int rol) {
  // Obtener las claves concedidas a este rol
  QStringList concedidas = GestorPermisos::clavesDelRol(rol, m_conexion);
  QSet<QString> setConcedidas(concedidas.begin(), concedidas.end());

  // Marcar/desmarcar cada checkbox
  for (auto it = m_checks.constBegin(); it != m_checks.constEnd(); ++it) {
    it.value()->setChecked(setConcedidas.contains(it.key()));
  }
}

/**
 * @brief Guarda los permisos del rol seleccionado en la BD.
 *
 * Compara el estado actual de los checkboxes con lo que hay en la BD.
 * Concede los ya marcados que no tenía y revoca los desmarcados que sí tenía.
 */
void EditorPermisos::guardar() {
  int rol = ui->comboRol->currentData().toInt();

  // Obtener claves que actualmente tiene el rol en BD
  QStringList actuales = GestorPermisos::clavesDelRol(rol, m_conexion);
  QSet<QString> setActuales(actuales.begin(), actuales.end());

  int concedidos = 0;
  int revocados = 0;

  for (auto it = m_checks.constBegin(); it != m_checks.constEnd(); ++it) {
    bool marcado = it.value()->isChecked();
    bool existeEnBD = setActuales.contains(it.key());

    if (marcado && !existeEnBD) {
      // Nuevo permiso: conceder
      GestorPermisos::conceder(rol, it.key(), m_conexion);
      concedidos++;
    } else if (!marcado && existeEnBD) {
      // Permiso quitado: revocar
      GestorPermisos::revocar(rol, it.key(), m_conexion);
      revocados++;
    }
  }

  QMessageBox::information(
      this, "Permisos guardados",
      QString("Rol %1: %2 permisos concedidos, %3 revocados.")
          .arg(GestorPermisos::nombresRoles().at(rol))
          .arg(concedidos)
          .arg(revocados));
}

/**
 * @brief Marca todos los checkboxes.
 */
void EditorPermisos::on_btnMarcarTodo_clicked() {
  for (auto it = m_checks.constBegin(); it != m_checks.constEnd(); ++it) {
    it.value()->setChecked(true);
  }
}

/**
 * @brief Desmarca todos los checkboxes.
 */
void EditorPermisos::on_btnDesmarcarTodo_clicked() {
  for (auto it = m_checks.constBegin(); it != m_checks.constEnd(); ++it) {
    it.value()->setChecked(false);
  }
}
void EditorPermisos::on_pushButtonGuardar_clicked() {
  guardar();
}

void EditorPermisos::on_pushButtonCerrar_clicked() {
  reject();
}
