#ifndef ENTRADAMERCANCIA_H
#define ENTRADAMERCANCIA_H

#include <QDialog>
#include "base_datos.h"
#include "buscarproducto.h"
#include "articulos.h"

namespace Ui {
class EntradaMercancia;
}

class EntradaMercancia : public QDialog
{
    Q_OBJECT

public:
    explicit EntradaMercancia(QWidget *parent = 0);
    ~EntradaMercancia();

private slots:
    void on_pushButtonAceptar_clicked();

    void on_lineEditCod_returnPressed();


    void on_pushButtonAgregarLinea_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_dateEditCaducidad_editingFinished();

private:
    Ui::EntradaMercancia *ui;
    baseDatos *base;
    QSqlTableModel *mTablaEntradas;
    QSqlQuery consulta;
    QString codSeleccionado;
    Articulos *articulo;



    void actualizarTabla();
};

#endif // ENTRADAMERCANCIA_H
