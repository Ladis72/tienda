#ifndef TIENDA_H
#define TIENDA_H

#include <QMainWindow>

#include "tpv.h"
#include "tabwidget.h"
#include "articulos.h"
#include "familias.h"
#include "fabricantes.h"
#include "formaspago.h"
#include "clientes.h"

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

private:
    Ui::Tienda *ui;
    Tpv *T;
    Ususarios *U;
    Articulos *A;
    Familias *F;
    Fabricantes *Fab;
    FormasPago *FPago;
    Clientes * Cli;
    //QSqlDatabase db;
};

#endif // TIENDA_H
