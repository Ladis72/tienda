#ifndef VERFACTURAS_H
#define VERFACTURAS_H

#include <QDialog>
#include <QDate>
#include "base_datos.h"


namespace Ui {
class VerFacturas;
}

class VerFacturas : public QDialog
{
    Q_OBJECT

public:
    explicit VerFacturas(QWidget *parent = 0);
    ~VerFacturas();

private slots:
    void on_checkBoxTodosProveedores_clicked(bool checked);

    void on_pushButtonVer_clicked();

private:
    void llenarProveedores();
    void llenarTabla();


    Ui::VerFacturas *ui;
    baseDatos *base;
    QString idProveedor , fechaInicial , fechaFinal;
    QSqlTableModel *tabla;
};

#endif // VERFACTURAS_H
