#ifndef SALIDAS_H
#define SALIDAS_H

#include <QDialog>
#include "base_datos.h"
#include "buscarproducto.h"

namespace Ui {
class Salidas;
}

class Salidas : public QDialog
{
    Q_OBJECT

public:
    explicit Salidas(QWidget *parent = 0);
    ~Salidas();

private slots:
    void on_lineEditCod_returnPressed();

    void on_pushButtonAgregar_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonEnviar_clicked();

private:
    Ui::Salidas *ui;
    baseDatos *base;
    QSqlQuery consulta;
    QSqlTableModel *mTablaSalidas;
    void actualizarTabla();
};

#endif // SALIDAS_H
