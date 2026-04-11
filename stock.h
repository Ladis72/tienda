#ifndef STOCK_H
#define STOCK_H

#include <QDialog>
#include <QKeyEvent>
#include "base_datos.h"
#include "configuracion.h"
#include "dateeditdelegate.h"

extern Configuracion *conf;

namespace Ui {
class Stock;
}

class Stock : public QDialog
{
    Q_OBJECT

public:
    explicit Stock(QString cod, baseDatos *db, QWidget *parent = nullptr);
    ~Stock();

private slots:
    void on_tableView_clicked(const QModelIndex &index);
    void on_pushButtonValidar_clicked();
    void on_pushButtonHistory_clicked();
    void on_pushButtonNuevoLote_clicked();
    void on_pushButtonCerrar_clicked();

private:
    Ui::Stock *ui;
    baseDatos *base;
    QString codProducto;
    QSqlTableModel *modeloLotes;
    int filaSeleccionada = -1;
    QString currentLoteId;
    QSqlQuery producto;

    void refrescarLotes();
    void limpiarFormulario();
};

#endif // STOCK_H
