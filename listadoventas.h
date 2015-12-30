#ifndef LISTADOVENTAS_H
#define LISTADOVENTAS_H

#include <QDialog>
#include <QVBoxLayout>
#include <QStandardItemModel>
#include "base_datos.h"

namespace Ui {
class ListadoVentas;
}

class ListadoVentas : public QDialog
{
    Q_OBJECT

public:
    explicit ListadoVentas(QWidget *parent = 0);
    ~ListadoVentas();

private slots:
    void on_pushButtonVer_clicked();

    void on_tableView_activated(const QModelIndex &index);

private:
    Ui::ListadoVentas *ui;

    baseDatos *base;
    QStandardItemModel *modeloTabla;
    void sumarVentas(QStandardItemModel *modelo);
};

#endif // LISTADOVENTAS_H
