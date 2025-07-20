#ifndef CLIENTES_H
#define CLIENTES_H
#include <QDialog>
#include <QList>
#include <QStandardItemModel>
#include "base_datos.h"
#include "configuracion.h"
#include "qdatetime.h"

extern Configuracion *conf;
namespace Ui {
class Clientes;
}

class Clientes : public QDialog
{
    Q_OBJECT

public:
    explicit Clientes(QWidget *parent = nullptr);
    explicit Clientes(QWidget *parent, QString codigo);
    ~Clientes();

private slots:
    void on_pushButtonAnterior_clicked();

    void on_pushButtonNuevo_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_lineEditNombre_returnPressed();

    void on_lineEditCod_editingFinished();

    void on_radioButtonMeses_clicked();

    void on_radioButtonAnos_clicked();

    void on_radioButtonFechas_clicked();

    void on_dateEditHasta_userDateChanged(const QDate &date);

    void on_dateEditDesde_userDateChanged(const QDate &date);

    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView2_doubleClicked(const QModelIndex &index);

    void on_radioButtonCantidad_clicked();

    void on_radioButtonFecha_clicked();

private:
    Ui::Clientes *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QSqlQueryModel modeloCompras;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;
    QString nombreConexionMaster;
    QString nombreConexionLocal;
    QString fechaI, fechaF;
    QSqlQueryModel *listaTickets;
    QSqlQueryModel *ticket;
    QString nTicket;
    QStandardItemModel *vistaTickets;
    QStringList listaConexionesRemotas;
    QList<QStandardItem *> listaItems;
    QStandardItemModel *vistaArticulosVendido;
    QSqlQueryModel *listaProductos;

    void recargarTabla();
    void borrarFormulario();
    bool eventFilter(QObject *obj, QEvent *event);
    void refrescarBotones(int i);
    void cargarCompras();
    QStringList recogerDatosFormulario();
};

#endif // CLIENTES_H
