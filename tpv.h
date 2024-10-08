#ifndef TPV_H
#define TPV_H

#include <QKeyEvent>
#include <QLabel>
#include <QMessageBox>
#include <QStandardItemModel>
#include <QWidget>
#include "ClickableLabel.h"
#include "base_datos.h"
#include "buscarproducto.h"
#include "clientes.h"
#include "configuracion.h"
#include "formateo.h"
#include "totalizar.h"
#include "visorimagenes.h"

extern Configuracion *conf;

namespace Ui {
class Tpv;

}

class Tpv : public QWidget
{
    Q_OBJECT

public:
    explicit Tpv(QWidget *parent = nullptr);
    ~Tpv();

private slots:
    void on_btn_cerrar_clicked();
    void on_lineEdit_cod_returnPressed();

    void on_btn_anadir_clicked();

    void on_btn_borrar_clicked();

    void on_lineEdit_desc_returnPressed();

    void on_btn_cobrar_clicked();

    void on_tableViewTicketsPendientes_clicked(const QModelIndex &index);

    void on_pushButtonBorrarTodo_clicked();

    void on_btn_modificar_clicked();

    void on_lineEdit_cod_cliente_editingFinished();

    void on_lineEdit_nobre_cliente_returnPressed();

    void on_lineEdit_precio_returnPressed();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_btn_preTicket_clicked();

    void usarVale(int ticket, int idVale, double cantVale);

    void mostrarFoto();

private:
    int ticket;
    int ticketActual;
    int usuario;
    int cliente;
    int idVale;
    double descuentoCliente, vale;
    QStringList listaConexionesRemotas;
    QString conexionLocal;
    Formateo classFormatear;
    Ui::Tpv *ui;
    QSqlDatabase db;
    baseDatos base;
    QSqlQuery consulta;
    BuscarProducto *buscar;
    totalizar *totalizacion;
    QSqlTableModel *modeloTicket;
    QStandardItemModel *modeloTicketPendiente;
    Clientes *clien;
    VisorImagenes *visor;

    void keyPressEvent(QKeyEvent *e);
    bool llenar_usuarios(QSqlDatabase db);
    double calcularPrecioTotal();
    double redondear(double num, int dec);
    void calcularNumProductos();
    bool actualizarLineaTicket(QList<QString> lista);
    void recuperarTicketsPendientes();
    void actualizarParrillaVentas();
    void cambiarTicket(int i);
    void ticketNuevo(int ticketnuevo);
    int ticketActualizado();
    QStringList recopilarDatosTicket();
    QStringList recopilarBasesIvas();
    QString formatearCadena(QString cadena, int tamano);
    void datosProducto(QString IdProducto);
    //void mostrarFoto();

signals:
    void cerrar_tpv();
};

#endif // TPV_H
