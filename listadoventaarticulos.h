#ifndef LISTADOVENTAARTICULOS_H
#define LISTADOVENTAARTICULOS_H

#include <QDialog>
#include <QStandardItemModel>
#include "base_datos.h"

namespace Ui {
class ListadoVentaArticulos;
}

class ListadoVentaArticulos : public QDialog
{
    Q_OBJECT

public:
    explicit ListadoVentaArticulos(QWidget *parent = 0);
    ~ListadoVentaArticulos();

private slots:
    void on_pushButtonVer_clicked();

private:
    Ui::ListadoVentaArticulos *ui;
    baseDatos base;
    QString primerTicket,ultimoTicket;
    QSqlQueryModel *modeloListado;

};

#endif // LISTADOVENTAARTICULOS_H
