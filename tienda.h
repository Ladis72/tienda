#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>

#include "base_datos.h"
#include "configuracion.h"
#include "tpv.h"
#include "tabwidget.h"
#include "articulos.h"
#include "familias.h"
#include "fabricantes.h"
#include "formaspago.h"
#include "clientes.h"
#include "proveedores.h"
#include "gestionpedidos.h"
#include "cajas.h"
#include "tiposentradassalidas.h"
#include "entradasalida.h"
#include "historicotickets.h"
#include "verfacturas.h"
#include "listadoventas.h"
#include "listadoventaarticulos.h"
#include "entradamercancia.h"
#include "caducidades.h"
#include "salidas.h"
#include "etiquetas.h"
#include "caducados.h"
#include "configticket.h"
#include "prestamos.h"
#include "configbase.h"
#include "configuracionotros.h"
#include "prestamistas.h"
#include "listadosalidas.h"
#include "listadoarqueos.h"
#include "listadocaducados.h"
#include "formatos.h"
#include "directorios.h"
#include "tiendas.h"
#include "conexionesremotas.h"
#include "actualizarclientes.h"
#include "generarvales.h"

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

    QStringList listaConexiones , conexionesActivas;
    QString conexionMaster;
    
private slots:
    void on_ventasButton_clicked();

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

private:
    baseDatos base;
    Ui::Tienda *ui;
    Tpv *T;
    Ususarios *U;
    Articulos *A;
    Familias *F;
    Fabricantes *Fab;
    FormasPago *FPago;
    Clientes * Cli;
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
};

#endif // TIENDA_H
