#include "tienda.h"
#include "conexion.h"
#include "login.h"
#include "ui_tienda.h"
#include "gestorencargosdialog.h"

#include "facturaralbaranes.h"
#include "gestorpermisos.h"
#include <QDebug>
#include <QEvent>
#include <QFileDialog>
#include <QMessageBox>
#include <QPalette>
#include <QSizePolicy>
#include <QSplitter>

Tienda::Tienda(QWidget *parent) : QMainWindow(parent), ui(new Ui::Tienda) {

  ui->setupUi(this);
  estadisticasDialog = nullptr;

  QStringList datos = base.datosConexionLocal();
  if (datos.isEmpty()) {
    createConnection("localhost", "3306", "tiendaNueva", "root", "meganizado",
                     "DB");
  }
  conf->setConexionLocal("DB");
  

  GestorPermisos::inicializar(conf->getConexionLocal());
  

  base.ejecutarSentencia(
    "CREATE TABLE IF NOT EXISTS `encargos` ("
    "  `id_encargo` INT AUTO_INCREMENT PRIMARY KEY,"
    "  `id_cliente` INT DEFAULT '0',"
    "  `cod_articulo` VARCHAR(15) NOT NULL,"
    "  `cantidad` INT NOT NULL,"
    "  `fecha_encargo` DATETIME DEFAULT CURRENT_TIMESTAMP,"
    "  `notas` VARCHAR(255) DEFAULT NULL,"
    "  `empleado` VARCHAR(100) DEFAULT NULL,"
    "  `anticipo` DOUBLE(10,2) DEFAULT '0.00',"
    "  `estado` ENUM('Pendiente', 'Recibido', 'Entregado', 'Cancelado') DEFAULT 'Pendiente'"
    ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4;", conf->getConexionLocal());


  QString conexionMaster = base.nombreConexionMaster();
  conf->setConexionMaster(conexionMaster);
  if (conf->getConexionMaster() != conf->getConexionLocal()) {
    ui->pushButtonGenerarVales->setEnabled(false);
    ui->pushButtonActualizarClientes->setEnabled(false);
  }


  cargarLogo();
  if (ui->logo) {
    ui->logo->setScaledContents(false);
    ui->logo->setAlignment(Qt::AlignCenter);
    ui->logo->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    ui->logo->installEventFilter(this);
  }


  conexiones = new conexionesRemotas(this);
  conexiones->base = &base;
  ui->statusBar->addPermanentWidget(ui->pushButtonConectar);

  sincroVales = new QPushButton("Sincro vales", this);
  connect(sincroVales, SIGNAL(clicked()), this, SLOT(sincronizarVales()));
  comprobarVales();
  usuario = new QPushButton(conf->getUsuario());
  usuario->setObjectName("usuarioButton");
  ui->statusBar->addPermanentWidget(usuario);
  connect(usuario, SIGNAL(clicked()), this,
          SLOT(on_pushButtonSesion_clicked()));


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

  mainSplitter->setSizes({550, 450});
  mainSplitter->setHandleWidth(8);
  mainSplitter->setStyleSheet(
      "QSplitter::handle { background-color: #F5F5F5; border-left: 1px solid "
      "#E0E0E0; border-right: 1px solid #E0E0E0; }"
      "QSplitter::handle:hover { background-color: #1565C0; }");
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
  connect(notasWidget, &NotasWidget::hideRequested, this,
          &Tienda::onToggleNotas);
  connect(btnNotifNotas, &QPushButton::clicked, this, &Tienda::onToggleNotas);


  notasWidget->refrescar();

  base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                   "Inicio programa ");
  conf->setNombreconexiones(conexiones->lista());
  //login();
}

Tienda::~Tienda() {
  int respuesta = QMessageBox::warning(
      this, tr("Salir de la aplicación"),
      tr("Quieres hacer una copia de seguridad antes de cerrar?"),
      QMessageBox::Yes | QMessageBox::No);
  if (respuesta == QMessageBox::Yes) {
    base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                     "Copia de seguridad creada");

    on_pushButtonCopia_clicked();
  }
  base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(),
                   "Fin del programa ");

  delete conf;
  delete ui;
}

void Tienda::cerrarAplicacion() {
  qDebug() << "Pasa por aquí";
  this->close();
  // std::exit(0);
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

/**
 * @brief Aplica los permisos del rol activo a los widgets de la ventana
 * principal.
 *
 * Utiliza el GestorPermisos centralizado para consultar si el rol actual
 * tiene acceso a cada funcionalidad. Los permisos se definen en la tabla
 * MySQL `permisos` y se cargan automáticamente al hacer setRol().
 *
 * Para añadir una nueva funcionalidad, solo hay que:
 *  1. Añadir una entrada al mapa clave→widget de esta función
 *  2. Insertar la fila en la tabla `permisos` para los roles que deban tener
 * acceso
 */
void Tienda::permisos(int rol) {
  // Mapa: cada clave de permiso → widget que controla
  // Al añadir un nuevo botón/funcionalidad, solo hay que añadir aquí una línea
  QMap<QString, QWidget *> mapa = {
      {"ventas", ui->ventasButton},
      {"articulos", ui->pushButton},
      {"familias", ui->pushButtonFamilias},
      {"fabricantes", ui->pushButtonFabricantes},
      {"clientes", ui->pushButton_3},
      {"proveedores", ui->pushButtonProveedores},
      {"formas_pago", ui->pushButtonFormasPago},
      {"prestamistas", ui->pushButtonPrestamistas},
      {"usuarios", ui->pushButtonUsuarios},
      {"tiendas", ui->pushButtonTiendas},
      {"formatos", ui->pushButtonFormatos},
      {"tipos_entrada_salida", ui->pushButtonEntradaSalida},
      {"etiquetas", ui->pushButtonEtiquetas},
      {"entradas", ui->pushButtonEntradas},
      {"salidas", ui->pushButtonSalidas},
      {"venta_articulos", ui->pushButton_2},
      {"caducidades", ui->pushButton_5},
      {"caducados", ui->pushButtonCaducados},
      {"movimientos", ui->movimientosButton},
      {"tickets", ui->pushButtonTickets},
      {"facturas", ui->pushButtonFacturas},
      {"albaranes", ui->pushButtonAlbaranes},
      {"facturar", ui->pushButtonFacturar},
      {"gestionar_pedidos", ui->pushButtonGestionar},
      {"cajas", ui->cajasButton},
      {"prestamos", ui->pushButtonPrestamos},
      {"generar_vales", ui->pushButtonGenerarVales},
      {"actualizar_clientes", ui->pushButtonActualizarClientes},
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
      {"tab_config", ui->tabConfig},
  };

  // Caso especial: sin sesión → todo bloqueado
  if (rol < 0) {
    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
      if (it.value())
        it.value()->setEnabled(false); // guard: widget puede ser nullptr
    }
    ui->pushButtonSesion->setEnabled(true);
    usuario->setEnabled(true);
    return;
  }

  // Caso normal: consultar al GestorPermisos para cada widget
  for (auto it = mapa.begin(); it != mapa.end(); ++it) {
    if (it.value())
      it.value()->setEnabled(
          conf->permisos()->tiene(it.key())); // guard: widget puede ser nullptr
  }

  // Botón de sesión y usuario siempre accesibles
  ui->pushButtonSesion->setEnabled(true);
  usuario->setEnabled(true);
}
void Tienda::activar_btn_tpv() {}


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
        "#ef5350; "
        "border-radius: 10px; padding: 2px 10px; margin: 2px; }"
        "QPushButton:hover { background-color: #d32f2f; }");
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

void Tienda::on_pushButton_clicked() {
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

void Tienda::on_pushButton_3_clicked() {
  if (!QSqlDatabase::database(conf->getConexionMaster()).isOpen()) {
    QMessageBox::warning(
        this, "No hay definida una tienda MASTER",
        "Los cambios que realice no serán guardados \n"
        "Debe especificar una tienda master y estar conectado para operar");
  }
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

void Tienda::on_pushButton_5_clicked() {
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

void Tienda::on_pushButtonCaducados_clicked() {
  Caduca = new Caducados(this);
  Caduca->exec();
}

void Tienda::on_pushButtonTicket_clicked() {
  CTicket = new ConfigTicket(this);
  CTicket->exec();
}

void Tienda::on_pushButtonConfigDB_clicked() {
  CBase = new ConfigBase("configBase", this);
  CBase->exec();
}

void Tienda::on_pushButtonPrestamos_clicked() {
  Prest = new Prestamos(this);
  Prest->exec();
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

void Tienda::on_pushButtonPrestamistas_clicked() {
  Prestamis = new Prestamistas(this);
  Prestamis->exec();
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
  foreach (QLabel *lab, ui->statusBar->findChildren<QLabel *>()) {
    lab->deleteLater();
  }
  QList<QLabel *> button;
  for (int i = 0; i < conexiones->lista().length(); i++) {
    button.append(new QLabel(conexiones->lista().at(i)));
    ui->statusBar->insertWidget(i, button.at(i));
  }
  QStringList conexionesActivas;
  QStringList conn;
  conn.clear();
  conn = conexiones->crear();
  for (int i = 0; i < conn.length(); i = i + 2) {
    if (conn.at(i + 1) == "0") {
      button[i / 2]->setStyleSheet("QLabel {background-color : #ef5350; color: "
                                   "white; border-radius: 4px; padding: 2px;}");
    } else {
      button[i / 2]->setStyleSheet("QLabel {background-color : #7cb342; color: "
                                   "white; border-radius: 4px; padding: 2px;}");
      conexionesActivas << conn.at(i);
    }
  }
  conf->setNombreConexionesActivas(conexionesActivas);
  conf->setConexionMaster(conexiones->conexionMaster());
}

void Tienda::on_pushButtonConectar_clicked() { refrescarConexiones(); }

void Tienda::on_pushButtonActualizarClientes_clicked() {
  ActualizarClientes *actClientes = new ActualizarClientes(this);
  actClientes->exec();
}

void Tienda::on_pushButtonGenerarVales_clicked() {
  //    if(conf->getNombreConexionesActivas().isEmpty() ||
  //    conf->getNombreConexionesActivas() != conf->getNombreConexiones()){
  //        QMessageBox::information(this,"No se puede generar ahora","Para
  //        generar los vales deben estar todos los ordenadores conectados.");
  //        return;
  //    }
  genVales = new GenerarVales(this);
  genVales->exec();
}

void Tienda::sincronizarVales() {
  QSqlQuery vales = base.valesPendientes(conf->getConexionLocal());
  vales.first();
  for (int i = 0; i < vales.numRowsAffected(); i++) {
    if (base.usarVale(vales.record().value(2).toString(),
                      vales.record().value(1).toInt())) {
      base.borrarValePendiente(conf->getConexionLocal(),
                               vales.record().value(1).toInt());
    }
    vales.next();
  }
  qDebug() << "Sincronizando vales";
  comprobarVales();
}

void Tienda::comprobarVales() {
  if (base.hayValesPendientesMarcar(conf->getConexionLocal())) {
    QPalette pal = sincroVales->palette();
    pal.setColor(QPalette::Button, QColor(Qt::red));
    sincroVales->setAutoFillBackground(true);
    sincroVales->setPalette(pal);
    sincroVales->update();
    ui->statusBar->addPermanentWidget(sincroVales);
    return;
  }
  sincroVales->hide();
}

void Tienda::on_pushButtonCopia_clicked() {
  QString directorio = base.devolverDirectorio("copia");
  if (directorio.isEmpty()) {
    directorio = QFileDialog::getExistingDirectory(
        this, "Elegir directorio", QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
  }
  qDebug() << directorio;

  QString nombreBackup =
      directorio + "/" + base.nombreConexionLocal() + "-" +
      QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + ".sql";
  qDebug() << nombreBackup;

  base.copiaSeguridad(conf->getConexionLocal(), nombreBackup);
}

void Tienda::on_pushButtonSesion_clicked() {
  conf->setUsuario(NULL);
  conf->setRol(-1);
  login();
}

void Tienda::login() {
  Login login;
  int resultado;
  if (login.exec() == QDialog::Accepted) {
    resultado = 1;
  } else {
    resultado = 0;
  }
  if (resultado == 0) {
    this->close();
    // cerrarAplicacion();
  }
  usuario->setText(conf->getUsuario());
  qDebug() << conf->getUsuario();
  qDebug() << conf->getRol();
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
      rutaLogo = "/home/ladis/AndroidStudioProjects/tienda/documentos/logo.jpg";
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



void Tienda::on_pushButton_2_clicked()
{
    ListadoVentaArticulos *listVentArticulos = new ListadoVentaArticulos(this);
    listVentArticulos->exec();
}

