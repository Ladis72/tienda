#include "tienda.h"
#include "ui_tienda.h"
#include "conexion.h"
#include <QDebug>
#include <QMessageBox>


Tienda::Tienda(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Tienda)
{
    ui->setupUi(this);
    qDebug() << conf->usuario;
    QPixmap logo;
    logo.load("./documentos/logo.jpg");
    ui->logo->setPixmap(logo);

}

Tienda::~Tienda()
{
    delete conf;
    delete ui;
}

void Tienda::on_ventasButton_clicked()
{



    T = new Tpv();
    connect(T, SIGNAL(cerrar_tpv()), this, SLOT(activar_btn_tpv()));

    T->showMaximized();

    return;

}
void Tienda::activar_btn_tpv()
{

}

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
    VFact = new VerFacturas("facturas",this);
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
    CBase = new ConfigBase(this);
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
    VFact = new VerFacturas("albaranes",this);
    VFact->exec();
}

void Tienda::on_pushButtonPrestamistas_clicked()
{
    Prestamis = new Prestamistas(this);
    Prestamis->exec();
}
