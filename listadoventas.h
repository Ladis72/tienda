#ifndef LISTADOVENTAS_H
#define LISTADOVENTAS_H

#include <QDialog>
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

private:
    Ui::ListadoVentas *ui;

    baseDatos *base;
    QSqlQueryModel *modeloTabla;
};

#endif // LISTADOVENTAS_H
