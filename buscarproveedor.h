#ifndef BUSCARPROVEEDOR_H
#define BUSCARPROVEEDOR_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class BuscarProveedor;
}

class BuscarProveedor : public QDialog
{
    Q_OBJECT

public:
    explicit BuscarProveedor(QWidget *parent, QString nombre);
    ~BuscarProveedor();
    QString resultado;
private slots:
    void on_tableView_clicked(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::BuscarProveedor *ui;
    baseDatos *base;
    QSqlQuery query;
    QSqlQueryModel modelo;
};

#endif // BUSCARPROVEEDOR_H
