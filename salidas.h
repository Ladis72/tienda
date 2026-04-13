#ifndef SALIDAS_H
#define SALIDAS_H

#include <QDialog>
#include "articulos.h"
#include "base_datos.h"
#include "buscarproducto.h"

extern Configuracion *conf;

namespace Ui {
class Salidas;
}

class Salidas : public QDialog
{
    Q_OBJECT

public:
    explicit Salidas(QWidget *parent = nullptr);
    ~Salidas();

private slots:
    void on_lineEditCod_returnPressed();

    void on_pushButtonAgregar_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonEnviar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);


    void actualizarTotales();

    void on_comboBoxDestino_currentIndexChanged(int index);
    void on_pushButtonCerrar_clicked();

private:
    Ui::Salidas *ui;
    baseDatos *base;
    QSqlQuery consulta;
    QSqlTableModel *mTablaSalidas;
    QString codSeleccionado;
    void actualizarTabla();
    Articulos *articulo;
    void llenarComboTiendas();
    int lineas;
    double productos;
};

#endif // SALIDAS_H
