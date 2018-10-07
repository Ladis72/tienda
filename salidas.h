#ifndef SALIDAS_H
#define SALIDAS_H

#include <QDialog>
#include "base_datos.h"
#include "buscarproducto.h"
#include "articulos.h"

namespace Ui {
class Salidas;
}

class Salidas : public QDialog
{
    Q_OBJECT

public:
    explicit Salidas(QWidget *parent = 0);
    ~Salidas();

private slots:
    void on_lineEditCod_returnPressed();

    void on_pushButtonAgregar_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonEnviar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Salidas *ui;
    baseDatos *base;
    QSqlQuery consulta;
    QSqlTableModel *mTablaSalidas;
    QString codSeleccionado;
    void actualizarTabla();
    Articulos *articulo;

};

#endif // SALIDAS_H
