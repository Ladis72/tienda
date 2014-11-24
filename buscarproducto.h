#ifndef BUSCARPRODUCTO_H
#define BUSCARPRODUCTO_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>

namespace Ui {
class BuscarProducto;
}

class BuscarProducto : public QDialog
{
    Q_OBJECT
    
public:
    explicit BuscarProducto(QWidget *parent, QSqlQuery query);
    ~BuscarProducto();
    QString resultado;

private slots:

    void on_tableView_clicked(const QModelIndex index);

    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::BuscarProducto *ui;
    QSqlQuery query;
    QSqlQueryModel modelo;

};

#endif // BUSCARPRODUCTO_H
