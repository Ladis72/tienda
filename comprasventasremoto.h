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

private slots:
    void on_radioButtonVentasDia_clicked();

    void on_radioButtonVentaMes_clicked();

    void on_radioButtonVentasAno_clicked();

    void on_radioButtonComprasDia_clicked();

    void on_radioButtonComprasMes_clicked();

    void on_radioButtonComprasano_clicked();

private:
    Ui::comprasVentasRemoto *ui;

    QSqlQueryModel modeloVentas;
    QSqlQueryModel modeloCompras;
    QString codigo;
    QSqlDatabase db;
};

#endif // COMPRASVENTASREMOTO_H
