#ifndef PRESTAMISTAS_H
#define PRESTAMISTAS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class Prestamistas;
}

class Prestamistas : public QDialog
{
    Q_OBJECT

public:
    explicit Prestamistas(QWidget *parent = 0);
    ~Prestamistas();

private slots:
    void on_pushButtonAnadir_clicked();

    void on_pushButtonModificar_clicked();

private:
    Ui::Prestamistas *ui;
    QSqlTableModel *modelo;
};

#endif // PRESTAMISTAS_H
