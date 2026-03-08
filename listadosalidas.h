#ifndef LISTADOSALIDAS_H
#define LISTADOSALIDAS_H

#include <QDialog>
#include <QSqlQueryModel>
#include "base_datos.h"

namespace Ui {
class ListadoSalidas;
}

class ListadoSalidas : public QDialog
{
    Q_OBJECT

public:
    explicit ListadoSalidas(QWidget *parent = 0);
    ~ListadoSalidas();

private slots:
    void on_pushButtonVer_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::ListadoSalidas *ui;
    baseDatos *base;
    QSqlQueryModel *modeloTabla;

    double sumar(QSqlQueryModel *modelo);
};

#endif // LISTADOSALIDAS_H
