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
    explicit comprasVentasRemoto(QSqlDatabase base, QString ean, int idTienda = 0, QString nombreTienda = QString(), QWidget *parent = nullptr);
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

    QSqlQueryModel *modeloVentas = nullptr;
    QSqlQueryModel *modeloCompras = nullptr;
    QString codigo;
    QSqlDatabase db;
    int m_idTienda;
    QString m_nombreTienda;
};

#endif // COMPRASVENTASREMOTO_H
