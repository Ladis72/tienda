#ifndef LISTADOVENTAS_H
#define LISTADOVENTAS_H

#include <QDialog>
#include <QKeyEvent>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

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

    void on_pushButtonImprimir_clicked();

private:
    Ui::ListadoVentas *ui;

    baseDatos *base;
    QStandardItemModel *modeloTabla;
    void sumarVentas(QStandardItemModel *modelo);
    void keyPressEvent(QKeyEvent *e);
    QString fechaI, fechaF;
};

#endif // LISTADOVENTAS_H
