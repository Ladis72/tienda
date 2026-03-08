#ifndef IMPUESTOS_H
#define IMPUESTOS_H

#include <QDialog>
#include "base_datos.h"

extern Configuracion *conf;

namespace Ui {
class impuestos;
}

class impuestos : public QDialog
{
    Q_OBJECT

public:
    explicit impuestos(QWidget *parent = nullptr);
    ~impuestos();

private slots:
    void on_pushButtonAgregar_clicked();

    void on_pushButtonElimiar_clicked();

    void on_pushButtonGuardar_clicked();

private:
    Ui::impuestos *ui;
    baseDatos *base;
    QSqlTableModel *modelo;
};

#endif // IMPUESTOS_H
