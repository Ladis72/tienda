#ifndef FORMASPAGO_H
#define FORMASPAGO_H

#include "base_datos.h"
#include <QDialog>

namespace Ui {
class FormasPago;
}

class FormasPago : public QDialog
{
    Q_OBJECT

public:
    explicit FormasPago(QWidget *parent = 0);
    ~FormasPago();
    QString resultado;

private slots:
    void on_pushButtonModificar_clicked();

    void on_pushButtonAnadir_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);


private:
    Ui::FormasPago *ui;
    QSqlTableModel *modelolista;
};

#endif // FORMASPAGO_H
