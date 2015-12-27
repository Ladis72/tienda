#ifndef STOCK_H
#define STOCK_H

#include <QDialog>
#include "base_datos.h"
namespace Ui {
class Stock;
}

class Stock : public QDialog
{
    Q_OBJECT

public:
    explicit Stock(QString cod, QWidget *parent = 0);
    ~Stock();

private slots:
    void on_pushButtonActualizar_clicked();

    void on_pushButton_clicked();

private:
    Ui::Stock *ui;
    baseDatos *base;
    QString codProducto;
    QSqlTableModel *modeloLotes;
};

#endif // STOCK_H
