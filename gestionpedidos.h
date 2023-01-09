#ifndef GESTIONPEDIDOS_H
#define GESTIONPEDIDOS_H

#include <QDialog>
#include "base_datos.h"
#include "pedidonuevo.h"
#include "pedidos.h"
#include "aceptarpedido.h"
#include <QStandardItemModel>


namespace Ui {
class GestionPedidos;
}

class GestionPedidos : public QDialog
{
    Q_OBJECT

public:
    explicit GestionPedidos(QWidget *parent = 0);
    ~GestionPedidos();

private slots:
    void on_pushButtonNuevo_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);

    void on_pushButtonBorrar_clicked();

    void on_pushButtonGestionar_clicked();

    void borrarVariables();

    void on_pushButtonAceptar_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

private:
    Ui::GestionPedidos *ui;
    baseDatos *base;
    QStandardItemModel listaPedidos;
    PedidoNuevo *pedido;
    pedidos *gestPedido;
    AceptarPedido *aPedido;
    QString pedidoSeleccionado;
    QString docSeleccionado;
    QString proveedor;
    QString fecha;

    void llenarTablaPedidos();
};

#endif // GESTIONPEDIDOS_H
