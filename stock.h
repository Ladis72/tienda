#ifndef STOCK_H
#define STOCK_H

#include <QDialog>
#include <QKeyEvent>
#include "base_datos.h"
#include "dateeditdelegate.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class Stock;
}

class Stock : public QDialog
{
    Q_OBJECT

public:
    explicit Stock(QString cod, QWidget *parent = nullptr);
    ~Stock();

private slots:
    void on_pushButtonActualizar_clicked();

    void on_pushButton_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Stock *ui;
    baseDatos *base;
    QString codProducto;
    QSqlTableModel *modeloLotes;
    int filaSeleccionada;
    QSqlQuery producto;

    void keyPressEvent(QKeyEvent *e);
};

#endif // STOCK_H
