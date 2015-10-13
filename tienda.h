#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>

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

extern Configuracion *conf;
namespace Ui {
class Tienda;
}

class Tienda : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit Tienda(QWidget *parent = 0);
    ~Tienda();


    
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

private:
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
    //QSqlDatabase db;
};

#endif // TIENDA_H
