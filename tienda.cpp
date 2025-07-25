﻿#include "tienda.h"
#include "conexion.h"
#include "login.h"
#include "ui_tienda.h"

#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QPalette>

Tienda::Tienda(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Tienda)
{
    ui->setupUi(this);
    QStringList datos = base.datosConexionLocal();
    if (datos.isEmpty()) {
        createConnection("localhost", "3306", "tiendaNueva", "root", "meganizado", "DB");
        conf->setConexionLocal("DB");
    } else {
        createConnection(datos.at(1), "3306", "tiendaNueva", datos.at(2), datos.at(3), datos.at(0));
        conf->setConexionLocal(datos.at(0));
    }
    QString conexionMaster = base.nombreConexionMaster();
    conf->setConexionMaster(conexionMaster);
    if (conf->getConexionMaster() != conf->getConexionLocal()) {
        ui->pushButtonGenerarVales->setEnabled(false);
        ui->pushButtonActualizarClientes->setEnabled(false);
    }

    QPixmap logo;
    logo.load("./documentos/logo.jpg");
    ui->logo->setPixmap(logo);

    conexiones = new conexionesRemotas(this);
    ui->statusBar->addPermanentWidget(ui->pushButtonConectar);

    sincroVales = new QPushButton("Sincro vales", this);
    connect(sincroVales, SIGNAL(clicked()), this, SLOT(sincronizarVales()));
    comprobarVales();
    usuario = new QPushButton(conf->getUsuario());
    QPalette paleta = usuario->palette();
    paleta.setColor(QPalette::Button, QColor(Qt::green));
    usuario->setPalette(paleta );
    ui->statusBar->addPermanentWidget(usuario);
    connect(usuario, SIGNAL(clicked()), this, SLOT(on_pushButtonSesion_clicked()));
    //on_pushButtonSesion_clicked();
}

Tienda::~Tienda()
{
    int respuesta
        = QMessageBox::warning(this,
                               tr("Salir de la aplicación"),
                               tr("Quieres hacer una copia de seguridad antes de cerrar?"),
                               QMessageBox::Yes | QMessageBox::No);
    if (respuesta == QMessageBox::Yes) {
        on_pushButtonCopia_clicked();
    }
    delete conf;
    delete ui;
}

void Tienda::cerrarAplicacion()
{
    qDebug() << "Pasa por aquí";
    this->close();
    //std::exit(0);
}

void Tienda::on_ventasButton_clicked()
{
    T = new Tpv();
    connect(T, SIGNAL(cerrar_tpv()), this, SLOT(activar_btn_tpv()));

    T->showMaximized();

    return;
}

void Tienda::permisos(int i)
{
    switch (i) {
    case 0:{
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();

        // Recorre la lista de botones y desactiva cada uno
        foreach(QPushButton *button, buttons) {
            button->setEnabled(true);
        }
        break;
    }
    case 1:
        ui->pushButtonListadoArqueos->setDisabled(true);
        ui->pushButtonListadoMovimientos->setDisabled(true);
        ui->pushButtonUsuarios->setDisabled(true);
        ui->listadoVentasButton->setDisabled(true);
        ui->pushButtonActualizarClientes->setDisabled(true);
        ui->pushButtonTiendas->setDisabled(true);
        ui->pushButtonEntradaSalida->setDisabled(true);
        ui->pushButtonFormatos->setDisabled(true);
        ui->pushButtonGenerarVales->setDisabled(true);
        ui->tabConfig->setDisabled(true);
        break;

    case 2:
        ui->pushButton_2->setDisabled(true);
        ui->pushButtonTiendas->setDisabled(true);
        ui->pushButtonEntradaSalida->setDisabled(true);
        ui->pushButtonFormatos->setDisabled(true);
        ui->pushButtonListadoArqueos->setDisabled(true);
        ui->pushButtonListadoMovimientos->setDisabled(true);
        ui->pushButtonUsuarios->setDisabled(true);
        ui->listadoVentasButton->setDisabled(true);
        ui->movimientosButton->setDisabled(true);
        ui->pushButtonActualizarClientes->setDisabled(true);
        ui->cajasButton->setDisabled(true);
        ui->pushButtonTickets->setDisabled(true);
        ui->pushButtonGenerarVales->setDisabled(true);
        ui->pushButtonInformes->setDisabled(true);
        ui->pushButtonTicket->setDisabled(true);
        ui->pushButtonConfiguracion->setDisabled(true);
        ui->tabConfig->setDisabled(true);

        break;
    case -1:{

        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();

        // Recorre la lista de botones y desactiva cada uno
        foreach(QPushButton *button, buttons) {
            button->setDisabled(true);

        }
        ui->pushButtonSesion->setEnabled(true);
        usuario->setEnabled(true);

        break;
    }
    default:
        QList<QPushButton *> buttons = this->findChildren<QPushButton *>();

        // Recorre la lista de botones y desactiva cada uno
        foreach(QPushButton *button, buttons) {
            button->setDisabled(true);
    }
        ui->pushButtonSesion->setEnabled(true);
        usuario->setEnabled(true);

        break;
}
}
void Tienda::activar_btn_tpv() {}

void Tienda::on_pushButtonUsuarios_clicked()
{
    U = new Ususarios();

    U->show();
}

void Tienda::on_pushButton_clicked()
{
    A = new Articulos(this);
    A->show();
}

void Tienda::on_pushButtonFamilias_clicked()
{
    F = new Familias(this);
    F->show();
}

void Tienda::on_pushButtonFabricantes_clicked()
{
    Fab = new Fabricantes(this);
    Fab->show();
}

void Tienda::on_pushButtonFormasPago_clicked()
{
    FPago = new FormasPago(this);
    FPago->show();
}

void Tienda::on_pushButton_3_clicked()
{
    if (!QSqlDatabase::database(conf->getConexionMaster()).isOpen()) {
        QMessageBox::warning(this,
                             "No hay definida una tienda MASTER",
                             "Los cambios que realice no serán guardados \n"
                             "Debe especificar una tienda master y estar conectado para operar");
    }
    Cli = new Clientes(this);
    Cli->show();
}

void Tienda::on_pushButtonProveedores_clicked()
{
    Prov = new Proveedores(this);
    Prov->exec();
}

void Tienda::on_pushButtonGestionar_clicked()
{
    GestPed = new GestionPedidos;
    GestPed->show();
}

void Tienda::on_cajasButton_clicked()
{
    caja = new Cajas;
    caja->exec();
}

void Tienda::on_pushButtonEntradaSalida_clicked()
{
    TiposEntSal = new TiposEntradasSalidas;
    TiposEntSal->exec();
}

void Tienda::on_movimientosButton_clicked()
{
    ES = new EntradaSalida;
    ES->exec();
}

void Tienda::on_pushButtonTickets_clicked()
{
    HT = new HistoricoTickets;
    HT->exec();
}

void Tienda::on_pushButtonFacturas_clicked()
{
    VFact = new VerFacturas("facturas", this);
    VFact->exec();
}

void Tienda::on_listadoVentasButton_clicked()
{
    ListVent = new ListadoVentas;
    ListVent->exec();
}

void Tienda::on_pushButton_2_clicked()
{
    VentaArticulos = new ListadoVentaArticulos(this);
    VentaArticulos->exec();
}

void Tienda::on_pushButtonEntradas_clicked()
{
    Entradas = new EntradaMercancia(this);
    Entradas->exec();
}

void Tienda::on_pushButton_5_clicked()
{
    Cad = new Caducidades(this);
    Cad->exec();
}

void Tienda::on_pushButtonSalidas_clicked()
{
    Salid = new Salidas(this);
    Salid->exec();
}

void Tienda::on_pushButtonEtiquetas_clicked()
{
    Etiq = new Etiquetas(this);
    Etiq->exec();
}

void Tienda::on_pushButtonCaducados_clicked()
{
    Caduca = new Caducados(this);
    Caduca->exec();
}

void Tienda::on_pushButtonTicket_clicked()
{
    CTicket = new ConfigTicket(this);
    CTicket->exec();
}

void Tienda::on_pushButtonConfigDB_clicked()
{
    CBase = new ConfigBase("configBase", this);
    CBase->exec();
}

void Tienda::on_pushButtonPrestamos_clicked()
{
    Prest = new Prestamos(this);
    Prest->exec();
}

void Tienda::on_pushButtonConfiguracion_clicked()
{
    ConfigOtros = new ConfiguracionOtros(this);
    ConfigOtros->exec();
}

void Tienda::on_pushButtonAlbaranes_clicked()
{
    VFact = new VerFacturas("albaranes", this);
    VFact->exec();
}

void Tienda::on_pushButtonPrestamistas_clicked()
{
    Prestamis = new Prestamistas(this);
    Prestamis->exec();
}

void Tienda::on_pushButtonListadoMovimientos_clicked()
{
    ListSalidas = new ListadoSalidas(this);
    ListSalidas->exec();
}

void Tienda::on_pushButtonListadoArqueos_clicked()
{
    ListaArqueos = new ListadoArqueos(this);
    ListaArqueos->exec();
}

void Tienda::on_pushButtonCaducados_2_clicked()
{
    ListaCaducados = new ListadoCaducados(this);
    ListaCaducados->exec();
}

void Tienda::on_pushButtonFormatos_clicked()
{
    Format = new Formatos(this);
    Format->exec();
}

void Tienda::on_pushButtonInformes_clicked()
{
    Director = new Directorios(this);
    Director->exec();
}

void Tienda::on_pushButtonTiendas_clicked()
{
    Sucursal = new tiendas(this);
    Sucursal->exec();
}

void Tienda::refrescarConexiones()
{
    foreach (QLabel *lab, ui->statusBar->findChildren<QLabel *>()) {
        lab->deleteLater();
    }
    QLabel *button[conexiones->lista().length()];
    for (int i = 0; i < conexiones->lista().length(); i++) {
        button[i] = new QLabel(conexiones->lista().at(i));
        ui->statusBar->insertWidget(i, button[i]);
    }
    QStringList conexionesActivas;
    QStringList conn;
    conn.clear();
    conn = conexiones->crear();
    conf->setNombreconexiones(conexiones->lista());
    for (int i = 0; i < conn.length(); i = i + 2) {
        if (conn.at(i + 1) == "0") {
            button[i / 2]->setStyleSheet("QLabel {background-color : red}");
        } else {
            button[i / 2]->setStyleSheet("QLabel {background-color : green}");
            conexionesActivas << conn.at(i);
        }
    }
    conf->setNombreConexionesActivas(conexionesActivas);
    conf->setConexionMaster(conexiones->conexionMaster());
}

void Tienda::on_pushButtonConectar_clicked()
{
    refrescarConexiones();
}

void Tienda::on_pushButtonActualizarClientes_clicked()
{
    ActualizarClientes *actClientes = new ActualizarClientes(this);
    actClientes->exec();
}

void Tienda::on_pushButtonGenerarVales_clicked()
{
    //    if(conf->getNombreConexionesActivas().isEmpty() || conf->getNombreConexionesActivas() != conf->getNombreConexiones()){
    //        QMessageBox::information(this,"No se puede generar ahora","Para generar los vales deben estar todos los ordenadores conectados.");
    //        return;
    //    }
    genVales = new GenerarVales(this);
    genVales->exec();
}

void Tienda::sincronizarVales()
{
    QSqlQuery vales = base.valesPendientes(conf->getConexionLocal());
    vales.first();
    for (int i = 0; i < vales.numRowsAffected(); i++) {
        if (base.usarVale(vales.record().value(2).toString(), vales.record().value(1).toInt())) {
            base.borrarValePendiente(conf->getConexionLocal(), vales.record().value(1).toInt());
        }
        vales.next();
    }
    qDebug() << "Sincronizando vales";
    comprobarVales();
}

void Tienda::comprobarVales()
{
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

void Tienda::on_pushButtonCopia_clicked()
{
    QString directorio = base.devolverDirectorio("copia");
    if (directorio.isEmpty()) {
        directorio = QFileDialog::getExistingDirectory(this,
                                                       "Elegir directorio",
                                                       QDir::homePath(),
                                                       QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);
    }
    qDebug() << directorio;

    QString nombreBackup = directorio + "/" + base.nombreConexionLocal() + "-"
                           + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + ".sql";
    qDebug() << nombreBackup;

    base.copiaSeguridad(conf->getConexionLocal(), nombreBackup);
}

void Tienda::on_pushButtonSesion_clicked() {
    conf->setUsuario(NULL);
    conf->setRol(-1);
    login();

}

void Tienda::login()
{
    Login login;
    int resultado;
    if (login.exec() == QDialog::Accepted) {
        resultado = 1;
    } else {
        resultado = 0;
    }
    if (resultado == 0) {
        this->close();
        //cerrarAplicacion();
    }
    usuario->setText(conf->getUsuario());
    qDebug() << conf->getUsuario();
    qDebug() << conf->getRol();
    permisos(conf->getRol());
}

void Tienda::on_pushButtonImpuestos_clicked()
{
    impuestos *editarImpuestos = new impuestos(this);
    editarImpuestos->exec();
}

