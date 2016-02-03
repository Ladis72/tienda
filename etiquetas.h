#ifndef ETIQUETAS_H
#define ETIQUETAS_H

#include <QDialog>
#include <QStandardItemModel>
#include "base_datos.h"


namespace Ui {
class Etiquetas;
}

class Etiquetas : public QDialog
{
    Q_OBJECT

public:
    explicit Etiquetas(QWidget *parent = 0);
    ~Etiquetas();

private slots:
    void on_pushButtonImprimir_clicked();

    void on_lineEditCod_returnPressed();

private:
    Ui::Etiquetas *ui;
    QStandardItemModel *modelo;
    baseDatos *base;

    QString formatearCadena(QString cadena);
    void llenarModelo();
};

#endif // ETIQUETAS_H
