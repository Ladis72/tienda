#ifndef LISTADOARQUEOS_H
#define LISTADOARQUEOS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class ListadoArqueos;
}

class ListadoArqueos : public QDialog
{
    Q_OBJECT

public:
    explicit ListadoArqueos(QWidget *parent = 0);
    ~ListadoArqueos();

private slots:
    void on_pushButtonConsultar_clicked();

    void on_pushButtonImprimir_clicked();

private:
    Ui::ListadoArqueos *ui;
    QSqlTableModel *modeloTabla;
};

#endif // LISTADOARQUEOS_H
