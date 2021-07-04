#ifndef PEDIDOS_H
#define PEDIDOS_H

#include <QDialog>
#include "base_datos.h"
#include "buscarproducto.h"
#include "articulos.h"
#include "configuracion.h"

extern Configuracion *conf;


namespace Ui {
class pedidos;
}

class pedidos : public QDialog
{
    Q_OBJECT

public:
    explicit pedidos(QString idPedido, QString proveedor, QString ndoc, QWidget *parent = 0);
    ~pedidos();

private slots:
    void on_leCod_editingFinished();

    void on_leUds_textChanged(const QString &arg1);

    void on_lePvt_textChanged(const QString &arg1);

    void on_leIva_textChanged(const QString &arg1);

    void on_leDescuento_textChanged(const QString &arg1);

    void on_pushButtonAnadir_clicked();

    void on_leCod_returnPressed();

    void on_pushButtonBorrar_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

    void on_leDescripcion_returnPressed();



    void on_pushButtonModificar_clicked();

    void on_leIva_editingFinished();

    void on_dateEdit_editingFinished();

    void on_leBon_textChanged(const QString &arg1);

    void on_lePvp_textChanged(const QString &arg1);

    void on_pushButtonImprimir_clicked();

private:
    Ui::pedidos *ui;

    QString calcularTotalLinea();
    QString lineaSeleccionada;
    void llenarTablaPedido(QString idPedido);
    void borrarLineEdits();
    void contarLineas();
    void contarArticulos();
    QString idPedido, nDoc , idProveedor , proveedorNombre;
    baseDatos base;
    QSqlQueryModel *modeloPedido;
    QSqlQuery consulta;
    Articulos *articulo;
    bool editando;
};

#endif // PEDIDOS_H
