#ifndef COMPRASVENTASREMOTO_H
#define COMPRASVENTASREMOTO_H

#include <QWidget>
#include "base_datos.h"

namespace Ui {
class comprasVentasRemoto;
}

class comprasVentasRemoto : public QWidget
{
    Q_OBJECT

public:
    explicit comprasVentasRemoto(QSqlDatabase base , QString ean ,QWidget *parent = nullptr);
    ~comprasVentasRemoto();

private:
    Ui::comprasVentasRemoto *ui;

    QSqlQueryModel *modeloVentas;
    QSqlQueryModel *modeloCompras;
};

#endif // COMPRASVENTASREMOTO_H
