#ifndef TPV_H
#define TPV_H

#include <QWidget>
#include <QMessageBox>
#include <QKeyEvent>
#include "base_datos.h"
#include "buscarproducto.h"
#include "totalizar.h"

namespace Ui {
class Tpv;

}

class Tpv : public QWidget
{
    Q_OBJECT
    
public:
    explicit Tpv(QWidget *parent = 0);
    ~Tpv();



private slots:
    void on_btn_cerrar_clicked();
    void on_lineEdit_cod_returnPressed();


    void on_btn_anadir_clicked();

    void on_btn_borrar_clicked();

    void on_lineEdit_desc_returnPressed();

    void on_btn_cobrar_clicked();

private:
    double ticket;
    int ticketActual;

    Ui::Tpv *ui;
    QSqlDatabase db;
    baseDatos base;
    QSqlQuery consulta;
    BuscarProducto *buscar;
    totalizar *totalizacion;

    void keyPressEvent(QKeyEvent *e);
    bool llenar_usuarios(QSqlDatabase db);
    double calcularPrecioTotal();
    double redondear(double num, int dec);
    int calcularNumProductos();
    bool actualizarLineaTicket(QList<QString> lista);
    void recuperarTicketsPendientes();


signals:
    void cerrar_tpv();
};

#endif // TPV_H
