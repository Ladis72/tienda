#ifndef DIALOGANADIRAPEDIDO_H
#define DIALOGANADIRAPEDIDO_H

#include <QDialog>
#include <QStandardItemModel>
#include "base_datos.h"

namespace Ui {
class DialogAnadirAPedido;
}

class DialogAnadirAPedido : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAnadirAPedido(QString codArticulo, QWidget *parent = nullptr);
    ~DialogAnadirAPedido();

private slots:
    void on_pushButtonNuevo_clicked();
    void on_pushButtonAceptar_clicked();
    void on_pushButtonCancelar_clicked();

private:
    Ui::DialogAnadirAPedido *ui;
    QString m_codArticulo;
    baseDatos *base;
    void llenarComboPedidos();
};

#endif // DIALOGANADIRAPEDIDO_H
