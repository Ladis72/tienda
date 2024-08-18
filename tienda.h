#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>

#include "actualizarclientes.h"
#include "articulos.h"
#include "base_datos.h"
#include "caducados.h"
#include "caducidades.h"
#include "cajas.h"
#include "clientes.h"
#include "conexionesremotas.h"
#include "configbase.h"
#include "configticket.h"
#include "configuracion.h"
#include "configuracionotros.h"
#include "directorios.h"
#include "entradamercancia.h"
#include "entradasalida.h"
#include "etiquetas.h"
#include "fabricantes.h"
#include "familias.h"
#include "formaspago.h"
#include "formatos.h"
#include "generarvales.h"
#include "gestionpedidos.h"
#include "historicotickets.h"
#include "listadoarqueos.h"
#include "listadocaducados.h"
#include "listadosalidas.h"
#include "listadoventaarticulos.h"
#include "listadoventas.h"
#include "prestamistas.h"
#include "prestamos.h"
#include "proveedores.h"
#include "salidas.h"
#include "tabwidget.h"
#include "tiendas.h"
#include "tiposentradassalidas.h"
#include "tpv.h"
#include "verfacturas.h"

extern Configuracion *conf;
namespace Ui {
class Tienda;
}

class Tienda : public QMainWindow
{
    Q_OBJECT

public:
    explicit Tienda(QWidget *parent = nullptr);
    ~Tienda();

    QStringList listaConexiones, conexionesActivas;
    QString conexionMaster;

private slots:
    void on_ventasButton_clicked();
    void permisos(int i);
    //public slots:
    void activar_btn_tpv();

    void on_pushButtonUsuarios_clicked();
    void on_pushButton_clicked();
    void on_pushButtonFamilias_clicked();
    void on_pushButtonFabricantes_clicked();
    void on_pushButtonFormasPago_clicked();
    void on_pushButton_3_clicked();
    void on_pushButtonProveedores_clicked();
    void on_pushButtonGestionar_clicked();
    void on_cajasButton_clicked();
    void on_pushButtonEntradaSalida_clicked();
    void on_movimientosButton_clicked();
    void on_pushButtonTickets_clicked();
    void on_pushButtonFacturas_clicked();
    void on_listadoVentasButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButtonEntradas_clicked();
    void on_pushButton_5_clicked();
    void on_pushButtonSalidas_clicked();
    void on_pushButtonEtiquetas_clicked();
    void on_pushButtonCaducados_clicked();
    void on_pushButtonTicket_clicked();
    void on_pushButtonConfigDB_clicked();
    void on_pushButtonPrestamos_clicked();
    void on_pushButtonConfiguracion_clicked();
    void on_pushButtonAlbaranes_clicked();
    void on_pushButtonPrestamistas_clicked();
    void on_pushButtonListadoMovimientos_clicked();
    void on_pushButtonListadoArqueos_clicked();
    void on_pushButtonCaducados_2_clicked();

    void on_pushButtonFormatos_clicked();

    void on_pushButtonInformes_clicked();

    void on_pushButtonTiendas_clicked();

    void refrescarConexiones();

    void on_pushButtonConectar_clicked();

    void on_pushButtonActualizarClientes_clicked();

    void on_pushButtonGenerarVales_clicked();
    void sincronizarVales();
    void comprobarVales();
    void keyPressEvent(QKeyEvent *e);

    void on_pushButtonCopia_clicked();

    void on_pushButtonSesion_clicked();
    void login();

private:
    QPushButton *sincroVales;
    QPushButton *usuario;
    QPalette paleta;

    baseDatos base;
    Ui::Tienda *ui;
    Tpv *T;
    Ususarios *U;
    Articulos *A;
    Familias *F;
    Fabricantes *Fab;
    FormasPago *FPago;
    Clientes *Cli;
    Proveedores *Prov;
    GestionPedidos *GestPed;
    Cajas *caja;
    TiposEntradasSalidas *TiposEntSal;
    EntradaSalida *ES;
    HistoricoTickets *HT;
    VerFacturas *VFact;
    ListadoVentas *ListVent;
    ListadoVentaArticulos *VentaArticulos;
    EntradaMercancia *Entradas;
    Caducidades *Cad;
    Salidas *Salid;
    Etiquetas *Etiq;
    Caducados *Caduca;
    ConfigTicket *CTicket;
    ConfigBase *CBase;
    Prestamos *Prest;
    ConfiguracionOtros *ConfigOtros;
    Prestamistas *Prestamis;
    ListadoSalidas *ListSalidas;
    ListadoArqueos *ListaArqueos;
    ListadoCaducados *ListaCaducados;
    Formatos *Format;
    Directorios *Director;
    tiendas *Sucursal;
    conexionesRemotas *conexiones;
    ActualizarClientes *actClientes;
    GenerarVales *genVales;

    QLabel *button[];
    void cerrarAplicacion();
};

#endif // TIENDA_H
