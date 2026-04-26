#ifndef BUSCARPRODUCTO_H
#define BUSCARPRODUCTO_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <utility>
#include <QSortFilterProxyModel>
#include <QSqlRecord>
#include "base_datos.h"

class ProxyProductos : public QSortFilterProxyModel {
    Q_OBJECT
public:
    bool soloConStock = false;
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        if (soloConStock) {
            QSqlQueryModel *sModel = qobject_cast<QSqlQueryModel*>(sourceModel());
            if (sModel) {
                QSqlRecord record = sModel->record(source_row);
                double stock = record.value("stock_total").toDouble();
                if (stock <= 0) return false;
            }
        }
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
public:
    void actualizar() {
        invalidateFilter();
    }
};

namespace Ui {
class BuscarProducto;
}

class BuscarProducto : public QDialog
{
    Q_OBJECT

public:
    explicit BuscarProducto(QWidget *parent, QSqlQuery query);
    ~BuscarProducto();
    void reject() override;
    QString resultado;

private slots:
    void on_tableView_activated(const QModelIndex &index);
    void on_lineEdit_buscar_textChanged(const QString &arg1);
    void on_checkBox_conStock_stateChanged(int arg1);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);


    void on_pushButtonCancelar_clicked();

    void on_pushButtonAceptar_clicked();

private:
    Ui::BuscarProducto *ui;
    QSqlQuery query;
    QSqlQueryModel modelo;
    ProxyProductos proxyModel;
    baseDatos base;

    void filtrar();
    void mostrarDetalles(const QModelIndex &index);
};

#endif // BUSCARPRODUCTO_H
