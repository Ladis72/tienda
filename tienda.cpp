#include "tienda.h"
#include "conexion.h"
#include "editorpermisos.h"
#include "gestorencargosdialog.h"
#include "login.h"
#include "ui_tienda.h"
#include "unificarproveedores.h"
#include "verfacturas.h"
#include "verifactudialog.h"
#include "verifactuclass.h"

#include "facturaralbaranes.h"
#include "gestorpermisos.h"
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QtConcurrent>
#include <QPalette>
#include <QSettings>
#include <QSizePolicy>
#include <QSplitter>
#include <QDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QLabel>
#include <QApplication>
#include <QCheckBox>

/******************************************************************************
 * CONSTRUCTOR principal de la aplicación Tienda
 * Inicializa:
 *  - Conexión a base de datos
 *  - Creación de tablas si no existen
 *  - Widgets principales (splitter, notas, sync)
 *  - Sistema de permisos
 *  - Carga de logo y configuración
 ******************************************************************************/
Tienda::Tienda(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tienda) {

  ui->setupUi(this);

  // Inicializar todos los punteros a nullptr para evitar punteros colgantes
  T = nullptr;
  U = nullptr;
  A = nullptr;
  F = nullptr;
  Fab = nullptr;
  FPago = nullptr;
  Cli = nullptr;
  Prov = nullptr;
  GestPed = nullptr;
  caja = nullptr;
  TiposEntSal = nullptr;
  ES = nullptr;
  HT = nullptr;
  VFact = nullptr;
  ListVent = nullptr;
  VentaArticulos = nullptr;
  Entradas = nullptr;
  Cad = nullptr;
  Salid = nullptr;
  Etiq = nullptr;
  CTicket = nullptr;
  CBase = nullptr;
  ConfigOtros = nullptr;
  ListSalidas = nullptr;
  ListaArqueos = nullptr;
  ListaCaducados = nullptr;
  Format = nullptr;
  estadisticasDialog = nullptr;
  Director = nullptr;
  Sucursal = nullptr;

  genVales = nullptr;
  editatImpuestos = nullptr;

  /******************************************************************************
   * CONEXIÓN A BASE DE DATOS
   * Intenta primero obtener la configuración local de la tabla 'tiendas'
   * Si no existe, usa valores por defecto: localhost / tiendaNueva / root
   ******************************************************************************/
  // SEC-01: Mantener siempre la conexión local abierta en main.cpp desde tienda.ini
  conf->setConexionLocal("DB");

  /******************************************************************************
   * CARGAR CONFIGURACIÓN GLOBAL
   * Lee opciones como precios_locales para mostrar precios locales
   * por例外 en cada tienda
   ******************************************************************************/
  QMap<QString, QVariant> config = base.leerConfiguracion();
  conf->setUsarPreciosLocales(config.value("precios_locales").toBool());

  /******************************************************************************
   * INICIALIZAR SISTEMA DE PERMISOS
   * Carga los permisos de la tabla 'permisos' según el rol del usuario
   ******************************************************************************/
  GestorPermisos::inicializar(conf->getConexionLocal());

  /******************************************************************************
   * CREAR TABLAS SI NO EXISTEN
   * Tablas necesarias para el funcionamiento de la app:
   *  - encargos: pedidos de clientes pendientes
   *  - historico_stock: control de cambios de stock
   *  - proveedores: catálogo de proveedores
   *  - precios_tienda: precios por excepción por tienda
   ******************************************************************************/
  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `encargos` ("
                         "  `id_encargo` INT AUTO_INCREMENT PRIMARY KEY,"
                         "  `id_cliente` INT DEFAULT '0',"
                         "  `cod_articulo` VARCHAR(15) NOT NULL,"
                         "  `cantidad` INT NOT NULL,"
                         "  `fecha_encargo` DATETIME DEFAULT CURRENT_TIMESTAMP,"
                         "  `notas` VARCHAR(255) DEFAULT NULL,"
                         "  `empleado` VARCHAR(100) DEFAULT NULL,"
                         "  `anticipo` DOUBLE(10,2) DEFAULT '0.00',"
                         "  `forma_pago` VARCHAR(50) DEFAULT 'Efectivo',"
                         "  `estado` ENUM('Pendiente', 'Recibido', "
                         "'Entregado', 'Cancelado') DEFAULT 'Pendiente'"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  // Añadimos el campo forma_pago si la tabla ya existía anteriormente
  base.ejecutarSentencia("ALTER TABLE `encargos` ADD COLUMN `forma_pago` VARCHAR(50) DEFAULT 'Efectivo';",
                         conf->getConexionLocal());

  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `historico_stock` ("
                         "  `id` INT AUTO_INCREMENT PRIMARY KEY,"
                         "  `ean` VARCHAR(15) NOT NULL,"
                         "  `lote` VARCHAR(50),"
                         "  `fecha_caducidad_ant` DATE,"
                         "  `fecha_caducidad_new` DATE,"
                         "  `stock_ant` DOUBLE(10,2),"
                         "  `stock_new` DOUBLE(10,2),"
                         "  `motivo` VARCHAR(255),"
                         "  `notas` VARCHAR(255),"
                         "  `usuario` VARCHAR(100),"
                         "  `fecha_hora` DATETIME DEFAULT CURRENT_TIMESTAMP"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  // Asegurar que la tabla historico_stock tiene el campo notas
  base.ejecutarSentencia(
      "ALTER TABLE `historico_stock` ADD COLUMN IF NOT EXISTS "
      "`notas` VARCHAR(255);",
      conf->getConexionLocal());

  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `proveedores` ("
                         "  `idProveedor` INT PRIMARY KEY,"
                         "  `nombre` VARCHAR(255),"
                         "  `nif` VARCHAR(20),"
                         "  `direccion` VARCHAR(255),"
                         "  `cp` VARCHAR(10),"
                         "  `localidad` VARCHAR(100),"
                         "  `provincia` VARCHAR(100),"
                         "  `representante` VARCHAR(100),"
                         "  `telefonor` VARCHAR(20),"
                         "  `mailr` VARCHAR(100),"
                         "  `telefono` VARCHAR(20),"
                         "  `mail` VARCHAR(100),"
                         "  `descuento` DOUBLE(10,2) DEFAULT '0.00',"
                         "  `fechaUltimaCompra` DATE DEFAULT '2000-01-01',"
                         "  `formapago` INT DEFAULT '0',"
                         "  `notas` TEXT"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  // Nueva tabla para precios locales por excepción
  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `precios_tienda` ("
                         "  `cod_articulo` VARCHAR(15) PRIMARY KEY,"
                         "  `pvp` DOUBLE(10,3) DEFAULT '0.000',"
                         "  `precio_venta` DOUBLE(10,3) DEFAULT '0.000'"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  // Tabla y trigger para registrar el historial de precios (Opción B)
  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `historico_precios` ("
                         "  `id` INT AUTO_INCREMENT PRIMARY KEY,"
                         "  `cod_articulo` VARCHAR(64) NOT NULL,"
                         "  `tipo` VARCHAR(20) NOT NULL,"
                         "  `precio_viejo` DOUBLE(10,3),"
                         "  `precio_nuevo` DOUBLE(10,3),"
                         "  `fecha_cambio` DATETIME DEFAULT CURRENT_TIMESTAMP"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  // Tabla para la gestión de notas y avisos de la aplicación
  base.ejecutarSentencia("CREATE TABLE IF NOT EXISTS `notas` ("
                         "  `id` INT AUTO_INCREMENT PRIMARY KEY,"
                         "  `titulo` VARCHAR(200) NOT NULL,"
                         "  `descripcion` TEXT,"
                         "  `usuario` VARCHAR(100) NOT NULL,"
                         "  `fecha_creacion` DATETIME DEFAULT CURRENT_TIMESTAMP,"
                         "  `fecha_limite` DATE DEFAULT NULL,"
                         "  `estado` ENUM('Pendiente','Completada') DEFAULT 'Pendiente',"
                         "  `prioridad` ENUM('Alta','Normal','Baja') DEFAULT 'Normal'"
                         ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;",
                         conf->getConexionLocal());

  base.ejecutarSentencia("DROP TRIGGER IF EXISTS `set_tienda_origen_precio`", conf->getConexionLocal());
  base.ejecutarSentencia(
      "CREATE TRIGGER `set_tienda_origen_precio` BEFORE UPDATE ON `articulos` "
      "FOR EACH ROW "
      "BEGIN "
      "  IF @skip_sync IS NULL OR @skip_sync = 0 THEN "
      "    IF NEW.pvp <> OLD.pvp OR NEW.precio_compra <> OLD.precio_compra THEN "
      "      SET NEW.id_tienda_origen = (SELECT id FROM tiendas WHERE local = 1 LIMIT 1); "
      "    END IF; "
      "  END IF; "
      "END;",
      conf->getConexionLocal());

  base.ejecutarSentencia("DROP TRIGGER IF EXISTS `log_cambio_precio`", conf->getConexionLocal());
  base.ejecutarSentencia(
      "CREATE TRIGGER `log_cambio_precio` AFTER UPDATE ON `articulos` "
      "FOR EACH ROW "
      "BEGIN "
      "  IF NEW.pvp <> OLD.pvp THEN "
      "    INSERT INTO historico_precios (cod_articulo, tipo, precio_viejo, precio_nuevo, fecha_cambio) "
      "    VALUES (NEW.cod, 'PVP', OLD.pvp, NEW.pvp, NOW()); "
      "  END IF; "
      "  IF NEW.precio_compra <> OLD.precio_compra THEN "
      "    INSERT INTO historico_precios (cod_articulo, tipo, precio_viejo, precio_nuevo, fecha_cambio) "
      "    VALUES (NEW.cod, 'COSTO', OLD.precio_compra, NEW.precio_compra, NOW()); "
      "  END IF; "
      "END;",
      conf->getConexionLocal());

  // Asegurar que la tabla configuracion tiene el campo precios_locales
  base.ejecutarSentencia("ALTER TABLE `configuracion` ADD COLUMN IF NOT EXISTS "
                         "`precios_locales` TINYINT(1) DEFAULT '0';",
                         conf->getConexionLocal());

  // Asegurar que la tabla configuracion tiene los campos para las teclas
  // rápidas de vendedores
  base.ejecutarSentencia("ALTER TABLE `configuracion` ADD COLUMN IF NOT EXISTS "
                         "`vendedor_f1` INT DEFAULT NULL;",
                         conf->getConexionLocal());
  base.ejecutarSentencia("ALTER TABLE `configuracion` ADD COLUMN IF NOT EXISTS "
                         "`vendedor_f2` INT DEFAULT NULL;",
                         conf->getConexionLocal());
  base.ejecutarSentencia("ALTER TABLE `configuracion` ADD COLUMN IF NOT EXISTS "
                         "`vendedor_f3` INT DEFAULT NULL;",
                         conf->getConexionLocal());
  base.ejecutarSentencia("ALTER TABLE `configuracion` ADD COLUMN IF NOT EXISTS "
                         "`vendedor_f4` INT DEFAULT NULL;",
                         conf->getConexionLocal());

  // SEC-02: Añadir columna salt a la tabla usuarios para hashing de contraseñas
  base.ejecutarSentencia("ALTER TABLE `usuarios` ADD COLUMN IF NOT EXISTS "
                         "`salt` VARCHAR(64) DEFAULT NULL;",
                         conf->getConexionLocal());

  // SEC-02: Ampliar el tamaño de la columna clave a VARCHAR(64) para guardar hashes SHA-256 sin truncar
  base.ejecutarSentencia("ALTER TABLE `usuarios` MODIFY COLUMN `clave` VARCHAR(64) NOT NULL;",
                         conf->getConexionLocal());

  // Asegurar que la tabla verifactu_logs tiene el campo estado_envio (por defecto 1 = Enviado para registros anteriores)
  base.ejecutarSentencia("ALTER TABLE `verifactu_logs` ADD COLUMN IF NOT EXISTS "
                         "`estado_envio` INT DEFAULT '1';",
                         conf->getConexionLocal());

  /******************************************************************************
   * CONEXIÓN MAESTRA (NUBE)
   * Si la conexión local es diferente de la master,某些botones
   * que solo funcionan en la master se deshabilitan
   ******************************************************************************/
  QString conexionMaster = base.nombreConexionMaster();
  conf->setConexionMaster(conexionMaster);
  ui->pushButtonGenerarVales->setEnabled(true);

  /******************************************************************************
   * CARGAR LOGO DE LA TIENDA
   * Busca en el directorio configurado o usa fallbacks
   ******************************************************************************/
  cargarLogo();
  if (ui->logo) {
    ui->logo->setScaledContents(false);
    ui->logo->setAlignment(Qt::AlignCenter);
    ui->logo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->logo->installEventFilter(this);
  }

  /******************************************************************************
   * GESTIÓN DE CONEXIONES REMOTAS
   * Administra la lista de tiendas y su estado de conexión
   ******************************************************************************/
  conexiones = new conexionesRemotas(this);
  conexiones->base = &base;
  ui->pushButtonConectar->setToolTip(tr("Conectar a las tiendas remotas seleccionadas.\n"
                                         "Mantén pulsado Shift al hacer clic para cambiar la selección de tiendas."));
  ui->statusBar->addPermanentWidget(ui->pushButtonConectar);

  // Botón de usuario en la barra de estado
  // Muestra el usuario actual y permite cerrar sesión
  usuario = new QPushButton(conf->getUsuario());
  usuario->setObjectName("usuarioButton");
  ui->statusBar->addPermanentWidget(usuario);
  connect(usuario, SIGNAL(clicked()), this,
          SLOT(on_pushButtonSesion_clicked()));

  /******************************************************************************
   * SPLITTER PRINCIPAL
   * Divide el espacio entre el logo y el panel de notas
   ******************************************************************************/
  mainSplitter = new QSplitter(Qt::Horizontal, ui->centralWidget);

  QWidget *logoContainer = new QWidget(mainSplitter);
  QVBoxLayout *logoLayout = new QVBoxLayout(logoContainer);
  logoLayout->setContentsMargins(0, 0, 0, 0);
  if (ui->logo) {
    logoLayout->addWidget(ui->logo);
  }
  logoContainer->setLayout(logoLayout);
  mainSplitter->addWidget(logoContainer);

  notasWidget = new NotasWidget(mainSplitter);
  mainSplitter->addWidget(notasWidget);

  /******************************************************************************
   * CONFIGURAR SPLITTER
   * Tamaño inicial: 550px logo, 450px notas
   * Handle ancho para facilitanarrastrar
   ******************************************************************************/
  mainSplitter->setSizes({550, 450});
  mainSplitter->setHandleWidth(8);
  mainSplitter->setStyleSheet(
      "QSplitter::handle { background-color: #F5F5F5; border-left: 1px solid "
      "#E0E0E0; border-right: 1px solid #E0E0E0; }"
      "QSplitter::handle:hover { background-color: #1976D2; }");
  mainSplitter->setChildrenCollapsible(true);

  QGridLayout *grid = qobject_cast<QGridLayout *>(ui->centralWidget->layout());
  if (grid) {
    grid->addWidget(mainSplitter, 1, 0);
  }

  btnNotifNotas = new QPushButton(tr("📋 Notas: 0"), this);
  btnNotifNotas->setFlat(true);
  btnNotifNotas->setCursor(Qt::PointingHandCursor);
  btnNotifNotas->setStyleSheet(
      "font-weight: bold; color: #558b2f; padding: 0 10px;");
  ui->statusBar->addPermanentWidget(btnNotifNotas);

  connect(notasWidget, &NotasWidget::pendingCountChanged, this,
          &Tienda::actualizarNotificacionNotas);

  // --- Inicialización del Sistema de Sincronización Nube ---
  managerSync = new SyncManager(this);
  labelEstadoNube = new QLabel("Nube: 🔴", this);
  labelEstadoNube->setStyleSheet(
      "font-weight: bold; color: #c62828; padding: 0 10px;");
  ui->statusBar->addPermanentWidget(labelEstadoNube);

  // Conectar cambios de estado de conexión
  connect(managerSync, &SyncManager::conexionRecuperada, this, [this]() {
    labelEstadoNube->setText("Nube: 🟢");
    labelEstadoNube->setStyleSheet(
        "font-weight: bold; color: #388e3c; padding: 0 10px;");
    ui->statusBar->showMessage(tr("Conexión con la nube establecida"), 3000);
  });

  connect(managerSync, &SyncManager::conexionPerdida, this, [this]() {
    labelEstadoNube->setText("Nube: 🔴");
    labelEstadoNube->setStyleSheet(
        "font-weight: bold; color: #d32f2f; padding: 0 10px;");
    ui->statusBar->showMessage(tr("Conexión con la nube perdida"), 3000);
  });

  // Mostrar aviso al terminar una sincronización exitosa
  connect(managerSync, &SyncManager::syncCompletado, this,
          [this](int subidos, int bajados) {
            if (subidos > 0 || bajados > 0) {
              ui->statusBar->showMessage(
                  tr("Sincronización: %1 subidos, %2 bajados")
                      .arg(subidos)
                      .arg(bajados),
                  5000);
            }
          });

  // Arrancar el mánager (triggers, cola y timers)
  managerSync->iniciar();
  connect(notasWidget, &NotasWidget::hideRequested, this,
          &Tienda::onToggleNotas);
  connect(btnNotifNotas, &QPushButton::clicked, this, &Tienda::onToggleNotas);

  notasWidget->refrescar();

  // Configurar temporizador para reenvío automático de VeriFactu cada 1 hora
  verifactuTimer = new QTimer(this);
  connect(verifactuTimer, &QTimer::timeout, this, [this]() {
    // Lanzamos el reenvío en un hilo secundario para no congelar la UI
    QtConcurrent::run([conn = conf->getConexionLocal()]() {
      verifactuClass::procesarEnviosPendientes(conn);
    });
  });
  verifactuTimer->start(3600000); // 1 hora (3600000 ms)

  // Realizar un primer reenvío automático al arrancar la aplicación (con un pequeño delay de 5 segundos)
  QTimer::singleShot(5000, this, [this]() {
    QtConcurrent::run([conn = conf->getConexionLocal()]() {
      verifactuClass::procesarEnviosPendientes(conn);
    });
  });

  // Botones dinámicos para nuevas opciones de configuración
  btnEditorPermisos = new QPushButton(tr("Editor de Permisos"), this);
  btnVerifactu = new QPushButton(tr("VeriFactu"), this);
  btnVisorLog = new QPushButton(tr("Visor de Logs"), this);
  btnVerificarBD = new QPushButton(tr("Verificar BD"), this);

  QGridLayout *configLayout =
      qobject_cast<QGridLayout *>(ui->tabConfig->layout());
  if (configLayout) {
    // Los añado a la segunda fila (row 1) del layout de configuración
    configLayout->addWidget(btnEditorPermisos, 1, 1);
    configLayout->addWidget(btnVerifactu, 1, 2);
    configLayout->addWidget(btnVisorLog, 1, 3);
    configLayout->addWidget(btnVerificarBD, 1, 4);
  }

  connect(btnEditorPermisos, &QPushButton::clicked, this, [this]() {
    EditorPermisos dial(conf->getRol(), conf->getConexionLocal(), this);
    dial.exec();
  });

  connect(btnVerifactu, &QPushButton::clicked, this, [this]() {
    VerifactuDialog dial(this);
    dial.exec();
  });

  connect(btnVisorLog, &QPushButton::clicked, this, [this]() {
    VisorLog dial(this);
    dial.exec();
  });

  connect(btnVerificarBD, &QPushButton::clicked, this, [this]() {
    VerificadorBaseDatos dial(this);
    dial.exec();
  });

  // Botón para iniciar el Gestor de Encargos desde la pantalla principal
  // btnEncargosMain = new QPushButton(tr("Gestor de Encargos"), this);
  /*QGridLayout *pedidosLayout = qobject_cast<QGridLayout
  *>(ui->TabPedidos->layout()); if (pedidosLayout) {
      // Se añade en una nueva posición del grid de pedidos
      pedidosLayout->addWidget(btnEncargosMain, 1, 1);
  }*/
  /*
  connect(btnEncargosMain, &QPushButton::clicked, this, [this]() {
    GestorEncargosDialog dial("", this);
    dial.exec();
  });
  */

  base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                   "Inicio programa ");
  conf->setNombreconexiones(conexiones->lista());
  
  // -- Monitor Inteligente de Caducidades --
  m_recomendacionesActivas.clear();
  btnMonitorCaducidades = new QPushButton("🔍 Analizar Caducidades Inteligente", this);
  btnMonitorCaducidades->setStyleSheet("background-color: #f1c40f; color: black; font-weight: bold; border-radius: 5px; padding: 5px; margin-right: 10px;");
  ui->statusBar->addPermanentWidget(btnMonitorCaducidades);
  connect(btnMonitorCaducidades, &QPushButton::clicked, this, &Tienda::onBtnMonitorCaducidadesClicked);
  
  login();
}

/******************************************************************************
 * DESTRUCTOR
 * Pregunta si quieres hacer copia de seguridad antes de cerrar
 * Registra el cierre en el log y libera memoria
 ******************************************************************************/
Tienda::~Tienda() {
  // Preguntamos siempre al cerrar si se desea realizar la copia
  int respuesta = QMessageBox::warning(
      this, tr("Salir de la aplicación"),
      tr("¿Quieres hacer una copia de seguridad antes de cerrar?"),
      QMessageBox::Yes | QMessageBox::No);

  if (respuesta == QMessageBox::Yes) {
    base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                     "Copia de seguridad iniciada al cerrar");
    // on_pushButtonCopia_clicked se encargará de usar el directorio guardado
    // ("cseg") o de preguntar por uno si no está configurado.
    on_pushButtonCopia_clicked();
  }

  base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                   "Fin del programa ");

  delete conf; // Liberar configuración global
  delete ui;   // Liberar interfaz generada por Qt
}

void Tienda::cerrarAplicacion() {
  this->close();
}

void Tienda::on_ventasButton_clicked() {
  if (T && T->isVisible()) {
    T->raise();
    T->activateWindow();
    return;
  }
  T = new Tpv();
  connect(T, SIGNAL(cerrar_tpv()), this, SLOT(activar_btn_tpv()));

  T->showMaximized();

  return;
}

/******************************************************************************
 * @brief Aplica permisos del rol a los widgets de la ventana principal
 *
 * Mapa de permisos: cada clave → widget controlado
 * Para añadir nueva funcionalidad:
 *   1. Añadir entrada al mapa (clave → widget)
 *   2. Insertar fila en tabla MySQL 'permisos' para los roles con acceso
 *
 * @param rol Rol del usuario (-1 = sin sesión, 0 =admin, etc.)
 ******************************************************************************/
void Tienda::permisos(int rol) {
  // Mapa: cada clave de permiso → widget que controla
  // Al añadir un nuevo botón/funcionalidad, solo hay que añadir aquí una línea
  QMap<QString, QWidget *> mapa = {
      {"ventas", ui->ventasButton},
      {"articulos", ui->pushButtonArticulos},
      {"familias", ui->pushButtonFamilias},
      {"fabricantes", ui->pushButtonFabricantes},
      {"clientes", ui->pushButtonClientes},
      {"proveedores", ui->pushButtonProveedores},
      {"formas_pago", ui->pushButtonFormasPago},
      {"usuarios", ui->pushButtonUsuarios},
      {"tiendas", ui->pushButtonTiendas},
      {"formatos", ui->pushButtonFormatos},
      {"tipos_entrada_salida", ui->pushButtonEntradaSalida},
      {"etiquetas", ui->pushButtonEtiquetas},
      {"entradas", ui->pushButtonEntradas},
      {"salidas", ui->pushButtonSalidas},
      {"venta_articulos", ui->pushButtonVentaArticulos},
      {"caducidades", ui->pushButtonCaducidades},

      {"movimientos", ui->movimientosButton},
      {"tickets", ui->pushButtonTickets},
      {"facturas", ui->pushButtonFacturas},
      {"albaranes", ui->pushButtonAlbaranes},
      {"facturar", ui->pushButtonFacturar},
      {"gestionar_pedidos", ui->pushButtonGestionar},
      {"cajas", ui->cajasButton},
      {"generar_vales", ui->pushButtonGenerarVales},

      {"listado_ventas", ui->listadoVentasButton},
      {"listado_movimientos", ui->pushButtonListadoMovimientos},
      {"listado_arqueos", ui->pushButtonListadoArqueos},
      {"listado_caducados", ui->pushButtonCaducados_2},
      {"estadisticas", ui->pushButtonEstadisticas},
      {"config_ticket", ui->pushButtonTicket},
      {"configuracion", ui->pushButtonConfiguracion},
      {"informes", ui->pushButtonInformes},
      {"impuestos", ui->pushButtonImpuestos},
      {"copia_seguridad", ui->pushButtonCopia},
      {"conectar", ui->pushButtonConectar},
      {"config_base", this->findChild<QWidget *>("pushButtonConfigDB")},
      {"config_local", ui->pushButtonConfigLocal},
      {"tab_config", ui->tabConfig},
      {"preparar", ui->pushButtonPreparar},
      {"notas", btnNotifNotas},
      {"editor_permisos", btnEditorPermisos},
      {"verifactu", btnVerifactu},
      {"visor_log", btnVisorLog},
      {"verificar_bd", btnVerificarBD},
      {"encargos", btnEncargosMain},
  };

  // Caso especial: sin sesión → todo bloqueado
  if (rol < 0) {
    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
      if (it.value())
        it.value()->setEnabled(false); // guard: widget puede ser nullptr
    }
    ui->pushButtonSesion->setEnabled(true);
    usuario->setEnabled(true);
    if (notasWidget) {
      notasWidget->aplicarPermisos();
    }
    return;
  }

  // Caso normal: consultar al GestorPermisos para cada widget
  for (auto it = mapa.begin(); it != mapa.end(); ++it) {
    if (it.value())
      it.value()->setEnabled(
          conf->permisos()->tiene(it.key())); // guard: widget puede ser nullptr
  }

  // Refrescar también los permisos internos del widget de notas
  if (notasWidget) {
    notasWidget->aplicarPermisos();
  }

  // Botón de sesión y usuario siempre accesibles
  ui->pushButtonSesion->setEnabled(true);
  usuario->setEnabled(true);
}
void Tienda::activar_btn_tpv() { T = nullptr; }

void Tienda::onToggleNotas() {
  if (notasWidget->isVisible()) {
    notasWidget->hide();
    btnNotifNotas->setToolTip(tr("Mostrar panel de notas"));
  } else {
    notasWidget->show();
    btnNotifNotas->setToolTip(tr("Ocultar panel de notas"));
    // Restaurar tamaño si estaba colapsado
    mainSplitter->setSizes({550, 450});
  }
}

void Tienda::actualizarNotificacionNotas(int count) {
  btnNotifNotas->setText(QString(tr("📋 Notas: %1")).arg(count));
  if (count > 0) {
    btnNotifNotas->setStyleSheet(
        "QPushButton { font-weight: bold; color: white; background-color: "
        "#d32f2f; "
        "border-radius: 10px; padding: 2px 10px; margin: 2px; }"
        "QPushButton:hover { background-color: #b71c1c; }");
  } else {
    btnNotifNotas->setStyleSheet("QPushButton { font-weight: bold; color: "
                                 "#558b2f; background-color: transparent; "
                                 "padding: 0 10px; }"
                                 "QPushButton:hover { color: #33691e; }");
  }
}

void Tienda::on_pushButtonUsuarios_clicked() {
  U = new Ususarios();

  U->show();
}

void Tienda::on_pushButtonArticulos_clicked() {
  A = new Articulos(this);
  A->show();
}

void Tienda::on_pushButtonFamilias_clicked() {
  F = new Familias(this);
  F->show();
}

void Tienda::on_pushButtonFabricantes_clicked() {
  Fab = new Fabricantes(this);
  Fab->show();
}

void Tienda::on_pushButtonFormasPago_clicked() {
  FPago = new FormasPago(this);
  FPago->show();
}

void Tienda::on_pushButtonClientes_clicked() {
  Cli = new Clientes(this);
  Cli->show();
}

void Tienda::on_pushButtonProveedores_clicked() {
  Prov = new Proveedores(this);
  Prov->exec();
}

void Tienda::on_pushButtonGestionar_clicked() {
  GestPed = new GestionPedidos;
  GestPed->show();
}

void Tienda::on_cajasButton_clicked() {
  caja = new Cajas;
  caja->exec();
}

void Tienda::on_pushButtonEntradaSalida_clicked() {
  TiposEntSal = new TiposEntradasSalidas;
  TiposEntSal->exec();
}

void Tienda::on_movimientosButton_clicked() {
  ES = new EntradaSalida;
  ES->exec();
}

void Tienda::on_pushButtonTickets_clicked() {
  HT = new HistoricoTickets;
  HT->exec();
}

void Tienda::on_pushButtonFacturas_clicked() {
  VFact = new VerFacturas("facturas", this);
  VFact->exec();
}

void Tienda::on_listadoVentasButton_clicked() {
  ListVent = new ListadoVentas;
  ListVent->exec();
}

void Tienda::on_pushButtonEntradas_clicked() {
  Entradas = new EntradaMercancia(this);
  Entradas->exec();
}

void Tienda::on_pushButtonCaducidades_clicked() {
  Cad = new Caducidades(this);
  Cad->exec();
}

void Tienda::on_pushButtonSalidas_clicked() {
  Salid = new Salidas(this);
  Salid->exec();
}

void Tienda::on_pushButtonEtiquetas_clicked() {
  Etiq = new Etiquetas(this);
  Etiq->exec();
}

void Tienda::on_pushButtonTicket_clicked() {
  CTicket = new ConfigTicket(this);
  CTicket->exec();
}

void Tienda::on_pushButtonConfigDB_clicked() {
  // Abre el diálogo de configuración del servidor MariaDB en la nube
  CBase = new ConfigBase(this);
  CBase->exec();
}

void Tienda::on_pushButtonConfigLocal_clicked() {
  // Abre el diálogo de configuración de la base de datos local (tienda.ini)
  CLocal = new ConfigLocal(this);
  CLocal->exec();
}

void Tienda::on_pushButtonConfiguracion_clicked() {
  ConfigOtros = new ConfiguracionOtros(this);
  ConfigOtros->exec();
}

void Tienda::on_pushButtonAlbaranes_clicked() {
  VFact = new VerFacturas("albaranes", this);
  VFact->exec();
}

void Tienda::on_pushButtonFacturar_clicked() {
  FacturarAlbaranes *fa = new FacturarAlbaranes(this);
  fa->exec();
  delete fa;
}

void Tienda::on_pushButtonListadoMovimientos_clicked() {
  ListSalidas = new ListadoSalidas(this);
  ListSalidas->exec();
}

void Tienda::on_pushButtonListadoArqueos_clicked() {
  ListaArqueos = new ListadoArqueos(this);
  ListaArqueos->exec();
}

void Tienda::on_pushButtonCaducados_2_clicked() {
  ListaCaducados = new ListadoCaducados(this);
  ListaCaducados->exec();
}

void Tienda::on_pushButtonFormatos_clicked() {
  Format = new Formatos(this);
  Format->exec();
}

void Tienda::on_pushButtonInformes_clicked() {
  Director = new Directorios(this);
  if (Director->exec() == QDialog::Accepted ||
      true) { // Refresh anyway if it was modal and could have changed things
    cargarLogo();
  }
}

void Tienda::on_pushButtonTiendas_clicked() {
  Sucursal = new tiendas(this);
  Sucursal->exec();
}

void Tienda::refrescarConexiones() {
  // 1. Limpiar etiquetas de conexión previas de forma precisa usando su
  // objectName
  QList<QLabel *> existingLabels =
      ui->statusBar->findChildren<QLabel *>("connLabel");
  for (QLabel *oldLab : existingLabels) {
    ui->statusBar->removeWidget(oldLab);
    oldLab->deleteLater();
  }

  // 2. Obtener la lista completa de tiendas configuradas
  QStringList nombresTiendas = conexiones->lista();
  if (nombresTiendas.isEmpty()) {
      return;
  }

  // Cargar configuración guardada desde tienda.ini
  QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
  QSettings settings(iniPath, QSettings::IniFormat);
  bool autoConectar = settings.value("TiendasRemotas/conectar_automatico", false).toBool();
  QStringList tiendasGuardadas = settings.value("TiendasRemotas/tiendas_seleccionadas").toStringList();

  // Comprobar si se mantiene pulsada la tecla Shift al hacer clic
  bool forzarConfig = (QApplication::keyboardModifiers() & Qt::ShiftModifier);

  QStringList tiendasSeleccionadas;

  // Si está activado conectar automático y hay una selección guardada, usarla (salvo que pulsen Shift)
  if (autoConectar && !tiendasGuardadas.isEmpty() && !forzarConfig) {
      for (const QString &t : tiendasGuardadas) {
          if (nombresTiendas.contains(t)) {
              tiendasSeleccionadas << t;
          }
      }
  }

  // Si no se han determinado las tiendas a conectar (primera vez o Shift pulsado), mostrar diálogo
  if (tiendasSeleccionadas.isEmpty()) {
      QDialog dlg(this);
      dlg.setWindowTitle(tr("Seleccionar Tiendas Remotas"));
      dlg.resize(350, 420);

      QVBoxLayout *layout = new QVBoxLayout(&dlg);
      QLabel *lblInfo = new QLabel(tr("Selecciona las tiendas a las que deseas conectar:"), &dlg);
      layout->addWidget(lblInfo);

      QListWidget *listWidget = new QListWidget(&dlg);
      layout->addWidget(listWidget);

      // Agregar tiendas a la lista con casillas de verificación
      for (const QString &nombre : nombresTiendas) {
          QListWidgetItem *item = new QListWidgetItem(nombre, listWidget);
          item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
          
          // Si había una selección guardada, restaurar su estado; si no, marcar por defecto
          if (!tiendasGuardadas.isEmpty()) {
              item->setCheckState(tiendasGuardadas.contains(nombre) ? Qt::Checked : Qt::Unchecked);
          } else {
              item->setCheckState(Qt::Checked);
          }
      }

      // Layout para botones de marcación rápida
      QHBoxLayout *btnLayout = new QHBoxLayout();
      QPushButton *btnMarcarTodo = new QPushButton(tr("Marcar todo"), &dlg);
      QPushButton *btnDesmarcarTodo = new QPushButton(tr("Desmarcar todo"), &dlg);
      btnLayout->addWidget(btnMarcarTodo);
      btnLayout->addWidget(btnDesmarcarTodo);
      layout->addLayout(btnLayout);

      connect(btnMarcarTodo, &QPushButton::clicked, [listWidget]() {
          for (int i = 0; i < listWidget->count(); ++i) {
              listWidget->item(i)->setCheckState(Qt::Checked);
          }
      });

      connect(btnDesmarcarTodo, &QPushButton::clicked, [listWidget]() {
          for (int i = 0; i < listWidget->count(); ++i) {
              listWidget->item(i)->setCheckState(Qt::Unchecked);
          }
      });

      // Opción para guardar la selección de forma persistente
      QCheckBox *chkAuto = new QCheckBox(tr("Recordar mi selección y conectar directamente"), &dlg);
      chkAuto->setToolTip(tr("Si se activa, el botón Conectar usará esta selección de forma directa.\n"
                             "Para modificarla en el futuro, mantén pulsada la tecla Shift mientras haces clic en Conectar."));
      chkAuto->setChecked(autoConectar);
      layout->addWidget(chkAuto);

      QDialogButtonBox *buttonBox = new QDialogButtonBox(
          QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &dlg);
      layout->addWidget(buttonBox);

      connect(buttonBox, &QDialogButtonBox::accepted, &dlg, &QDialog::accept);
      connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

      if (dlg.exec() != QDialog::Accepted) {
          return; // Cancelado por el usuario
      }

      // Filtrar las tiendas seleccionadas
      for (int i = 0; i < listWidget->count(); ++i) {
          QListWidgetItem *item = listWidget->item(i);
          if (item->checkState() == Qt::Checked) {
              tiendasSeleccionadas << item->text();
          }
      }

      if (tiendasSeleccionadas.isEmpty()) {
          QMessageBox::information(this, tr("Conexión"), tr("No has seleccionado ninguna tienda."));
          return;
      }

      // Guardar la nueva configuración en tienda.ini
      settings.setValue("TiendasRemotas/conectar_automatico", chkAuto->isChecked());
      settings.setValue("TiendasRemotas/tiendas_seleccionadas", tiendasSeleccionadas);
  }

  // Conectar solo a las tiendas seleccionadas
  QStringList estadosConexiones =
      conexiones->crear(tiendasSeleccionadas); // Devuelve pares [nombre, "1" o "0"]

  QStringList conexionesActivas;

  // 3. Crear y configurar las etiquetas correspondientes a las tiendas seleccionadas en la barra de estado
  for (int i = 0; i < tiendasSeleccionadas.length(); i++) {
    QString nombre = tiendasSeleccionadas.at(i);
    QLabel *lab = new QLabel(nombre, this);
    lab->setObjectName("connLabel");

    // Buscar el estado correspondiente en la lista devuelta por crear()
    bool isOnline = false;
    int idx = estadosConexiones.indexOf(nombre);
    if (idx != -1 && idx + 1 < estadosConexiones.length()) {
      isOnline = (estadosConexiones.at(idx + 1) == "1");
    }

    if (isOnline) {
      lab->setStyleSheet(
          "QLabel { background-color: #388e3c; color: white; border-radius: "
          "4px; "
          "padding: 2px 6px; font-weight: bold; margin-right: 4px; }");
      conexionesActivas << nombre;
    } else {
      lab->setStyleSheet(
          "QLabel { background-color: #d32f2f; color: white; border-radius: "
          "4px; "
          "padding: 2px 6px; font-weight: bold; margin-right: 4px; }");
    }

    // Insertar al principio de la barra de estado (lado izquierdo)
    ui->statusBar->insertWidget(i, lab);
    lab->show();
  }

  // 4. Actualizar configuración global
  conf->setNombreConexionesActivas(conexionesActivas);
  conf->setConexionMaster(conexiones->conexionMaster());
}

void Tienda::on_pushButtonConectar_clicked() { refrescarConexiones(); }

/**
 * @brief Comprueba si los vales del mes anterior ya están generados.
 *
 * Si no existen vales para el mes anterior, pide confirmación al usuario
 * y lanza el proceso de generación. Los vales generados en local son
 * propagados a la nube automáticamente por el SyncManager.
 */
void Tienda::on_pushButtonGenerarVales_clicked() {
  // Lanzar el proceso de generación de vales de fidelidad
  GenerarVales *genVales = new GenerarVales(this);
  genVales->exec();
  delete genVales;
}

/******************************************************************************
 * @brief Realiza copia de seguridad de la base de datos
 *
 * 1. Busca directorio guardado previamente
 * 2. Si no existe, pregunta al usuario
 * 3. Genera nombre: {tienda}-{fecha}.sql
 * 4. Ejecuta mysqldump para exportar
 ******************************************************************************/
void Tienda::on_pushButtonCopia_clicked() {
  // Se usa la clave "cseg" que es la que se guarda en la tabla directorios
  QString directorio = base.devolverDirectorio("cseg");

  // Si no hay directorio guardado o el directorio guardado ya no existe en el
  // disco, se pregunta
  if (directorio.isEmpty() || !QDir(directorio).exists()) {
    directorio = QFileDialog::getExistingDirectory(
        this, "Elegir directorio para Copias de Seguridad", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    // Si el usuario elige un directorio, lo guardamos para la próxima vez
    if (!directorio.isEmpty()) {
      QMap<QString, QString> m;
      m.insert("cseg", directorio);
      base.guardarDirectorios(conf->getConexionLocal(), m);
    }
  }

  if (directorio.isEmpty()) {
    qDebug()
        << "Copia de seguridad cancelada: no se ha seleccionado directorio.";
    return; // Cancelar si el usuario cierra el diálogo
  }

  qDebug() << directorio;

  QString nombreBackup =
      directorio + "/" + base.nombreConexionLocal() + "-" +
      QDateTime::currentDateTime().toString("yyyy-MM-dd_HH-mm-ss") + ".sql";
  qDebug() << nombreBackup;

  if (base.copiaSeguridad(conf->getConexionLocal(), nombreBackup)) {
    // Si la copia ha sido exitosa, ejecutamos la rotación/limpieza
    limpiarCopiasAntiguas(directorio);
  }
}

void Tienda::on_pushButtonSesion_clicked() {
  conf->setUsuario(NULL);
  conf->setRol(-1);
  login();
}

void Tienda::login() {
  Login login;
  if (login.exec() != QDialog::Accepted) {
    this->close();
    return;
  }
  usuario->setText(conf->getUsuario());
  // SEC-07: No imprimir usuario/rol en la salida de debug
  permisos(conf->getRol());
}

void Tienda::on_pushButtonImpuestos_clicked() {
  impuestos *editarImpuestos = new impuestos(this);
  editarImpuestos->exec();
}

void Tienda::resizeEvent(QResizeEvent *event) {
  if (!logoOriginal.isNull()) {
    ui->logo->setPixmap(logoOriginal.scaled(
        ui->logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
  }
  QMainWindow::resizeEvent(event);
}
bool Tienda::eventFilter(QObject *obj, QEvent *event) {
  if (obj == ui->logo && event->type() == QEvent::Resize) {
    if (!logoOriginal.isNull() && ui->logo->width() > 0) {
      ui->logo->setPixmap(logoOriginal.scaled(
          ui->logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  }
  return QMainWindow::eventFilter(obj, event);
}

void Tienda::cargarLogo() {
  QString rutaLogo = base.devolverDirectorio("logo");

  if (!QFile::exists(rutaLogo)) {
    // Try absolute path if it was relative
    rutaLogo = QCoreApplication::applicationDirPath() + "/" + rutaLogo;
  }

  if (rutaLogo.isEmpty() || !QFile::exists(rutaLogo)) {
    // Fallback to default logos if not configured or not found
    rutaLogo = QCoreApplication::applicationDirPath() + "/documentos/logo.png";
    if (!QFile::exists(rutaLogo)) {
      // SEC-12: Usar ruta relativa al ejecutable, no ruta absoluta hardcodeada
      rutaLogo = QCoreApplication::applicationDirPath() + "/documentos/logo.jpg";
    }
  }

  if (QFile::exists(rutaLogo)) {
    logoOriginal.load(rutaLogo);
    // Only set the pixmap here if the label already has a valid size.
    // Otherwise, the deferred QTimer or resizeEvent will handle it.
    if (!logoOriginal.isNull() && ui->logo->width() > 0) {
      ui->logo->setPixmap(logoOriginal.scaled(
          ui->logo->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
  }
}

void Tienda::on_pushButtonEstadisticas_clicked() {
  if (!estadisticasDialog) {
    estadisticasDialog = new Estadisticas(this);
  }
  estadisticasDialog->exec();
}

void Tienda::on_pushButtonVentaArticulos_clicked() {
  ListadoVentaArticulos *listVentArticulos = new ListadoVentaArticulos(this);
  listVentArticulos->exec();
}

void Tienda::on_pushButtonUnificarGlobal_clicked() {
  UnificarProveedores dlg(this);
  dlg.exec();
}

/**
 * @brief Rotación de copias de seguridad.
 * Conserva:
 *  - Todas las copias de los últimos 7 días.
 *  - Una copia por cada sábado para el histórico anterior a 7 días.
 * Borra el resto para ahorrar espacio.
 */
void Tienda::limpiarCopiasAntiguas(const QString &directorio) {
  QDir dir(directorio);
  if (!dir.exists())
    return;

  // Filtramos por prefijo de la tienda y extensión .sql
  QString prefix = base.nombreConexionLocal() + "-";
  QStringList filtros;
  filtros << prefix + "*.sql";

  QFileInfoList lista = dir.entryInfoList(filtros, QDir::Files, QDir::Name);
  QDate hoy = QDate::currentDate();

  qDebug() << "Iniciando limpieza de copias en:" << directorio;

  for (const QFileInfo &info : lista) {
    QString nombre = info.fileName();
    // Extraer fecha del nombre (formato: Tienda-yyyy-MM-dd_...)
    // La fecha empieza justo después del prefijo y ocupa 10 caracteres
    QString strFecha = nombre.mid(prefix.length(), 10);
    QDate fechaCopia = QDate::fromString(strFecha, "yyyy-MM-dd");

    if (!fechaCopia.isValid())
      continue;

    qint64 dias = fechaCopia.daysTo(hoy);

    // 1. Conservar si es de los últimos 7 días
    if (dias <= 7) {
      continue;
    }

    // 2. Conservar si es un sábado (histórico semanal)
    if (fechaCopia.dayOfWeek() == Qt::Saturday) {
      continue;
    }

    // 3. De lo contrario, eliminar
    if (QFile::remove(info.absoluteFilePath())) {
      qDebug() << "Rotación: eliminada copia obsoleta ->" << nombre;
    } else {
      qWarning() << "Rotación: no se pudo eliminar ->" << nombre;
    }
  }
}

void Tienda::onRecomendacionesListas(QList<RecomendacionCaducidad> recomendaciones)
{
    m_recomendacionesActivas = recomendaciones;
    btnMonitorCaducidades->setEnabled(true);
    
    if (!m_recomendacionesActivas.isEmpty()) {
        btnMonitorCaducidades->setText(QString("⚠ %1 Avisos (Re-Analizar)").arg(m_recomendacionesActivas.size()));
        DialogRecomendaciones *dialog = new DialogRecomendaciones(nullptr);
        dialog->setAttribute(Qt::WA_DeleteOnClose);
        dialog->setWindowModality(Qt::NonModal);
        dialog->cargarRecomendaciones(m_recomendacionesActivas);
        dialog->show();
    } else {
        btnMonitorCaducidades->setText("✅ Todo OK (Re-Analizar)");
        QMessageBox::information(this, "Monitor de Caducidades", "No se han encontrado lotes con riesgo de caducidad inminente o merma.");
    }
}

void Tienda::onBtnMonitorCaducidadesClicked() {
    if (!m_recomendacionesActivas.isEmpty()) {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle("Monitor de Caducidades");
        msgBox.setText("Ya hay resultados de un análisis anterior.");
        msgBox.setInformativeText("¿Qué desea hacer?");
        QPushButton *btnVer = msgBox.addButton("Ver Avisos", QMessageBox::ActionRole);
        QPushButton *btnReanalizar = msgBox.addButton("Re-Analizar", QMessageBox::ActionRole);
        msgBox.addButton("Cancelar", QMessageBox::RejectRole);
        
        msgBox.exec();
        
        if (msgBox.clickedButton() == btnVer) {
            DialogRecomendaciones *dialog = new DialogRecomendaciones(nullptr);
            dialog->setAttribute(Qt::WA_DeleteOnClose);
            dialog->setWindowModality(Qt::NonModal);
            dialog->cargarRecomendaciones(m_recomendacionesActivas);
            dialog->show();
            return;
        } else if (msgBox.clickedButton() != btnReanalizar) {
            return; // Cancelar
        }
    }

    btnMonitorCaducidades->setEnabled(false);
    btnMonitorCaducidades->setText("⏳ Analizando en 2º plano...");
    
    m_monitorCaducidades = new MonitorCaducidades(conf->getConexionLocal());
    QThread *monitorThread = new QThread(this);
    m_monitorCaducidades->moveToThread(monitorThread);
    
    // Al arrancar el hilo, lanza la función de análisis
    connect(monitorThread, &QThread::started, m_monitorCaducidades, &MonitorCaducidades::iniciar);
    connect(m_monitorCaducidades, &MonitorCaducidades::analisisCompletado, this, &Tienda::onRecomendacionesListas);
    connect(m_monitorCaducidades, &MonitorCaducidades::errorOcurrido, this, &Tienda::onMonitorCaducidadesError);
    
    // Limpieza
    connect(monitorThread, &QThread::finished, m_monitorCaducidades, &QObject::deleteLater);
    connect(m_monitorCaducidades, &MonitorCaducidades::finished, monitorThread, &QThread::quit);
    connect(monitorThread, &QThread::finished, monitorThread, &QObject::deleteLater);
    
    monitorThread->start();
}

void Tienda::onMonitorCaducidadesError(QString msg) {
    btnMonitorCaducidades->setEnabled(true);
    btnMonitorCaducidades->setText("🔍 Analizar Caducidades Inteligente");
    QMessageBox::warning(this, "Monitor de Caducidades", msg);
}
