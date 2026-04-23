#ifndef BUSCARCLIENTE_H
#define BUSCARCLIENTE_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <utility>
#include <QSortFilterProxyModel>
#include <QSqlRecord>
#include "base_datos.h"

class ProxyClientes : public QSortFilterProxyModel {
    Q_OBJECT
public:
    ProxyClientes(QObject *parent = nullptr) : QSortFilterProxyModel(parent) {}
protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
    }
public:
    void actualizar() { invalidateFilter(); }
};

namespace Ui {
class BuscarCliente;
}

class BuscarCliente : public QDialog
{
    Q_OBJECT

public:
    explicit BuscarCliente(QWidget *parent, QSqlQuery query);
    ~BuscarCliente();
    void reject() override;
    QString resultado;

private slots:
    void on_tableView_activated(const QModelIndex &index);
    void on_lineEdit_buscar_textChanged(const QString &arg1);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);

    void on_pushButtonCancelar_clicked();
    void on_pushButtonAceptar_clicked();

private:
    Ui::BuscarCliente *ui;
    QSqlQuery query;
    QSqlQueryModel modelo;
    ProxyClientes proxyModel;
    baseDatos base;

    void filtrar();
    void mostrarDetalles(const QModelIndex &index);
};

#endif // BUSCARCLIENTE_H
