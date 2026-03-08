#ifndef CADUCIDADES_H
#define CADUCIDADES_H

#include <QDialog>
#include <QSqlRelationalDelegate>
#include <QSqlRelationalTableModel>
#include "base_datos.h"
namespace Ui {
class Caducidades;
}

class Caducidades : public QDialog
{
    Q_OBJECT

public:
    explicit Caducidades(QWidget *parent = 0);
    ~Caducidades();

private:
    Ui::Caducidades *ui;

    baseDatos *base;
    QSqlRelationalTableModel *modelo;
};

#endif // CADUCIDADES_H
