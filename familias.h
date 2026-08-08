#ifndef FAMILIAS_H
#define FAMILIAS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class Familias;
}

class Familias : public QDialog
{
    Q_OBJECT

public:
    explicit Familias(QWidget *parent = 0);
    ~Familias();
    void aplicarPermisos();

    QString resultado;

private slots:
    void on_pushButtonSeleccionar_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButtonAnadir_clicked();

    void on_pushButtonGuardar_clicked();

    void on_pushButtonDeshacer_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    void on_pushButtonUnificar_clicked();
    void on_pushButtonSaneador_clicked();

private:
    Ui::Familias *ui;

    QSqlTableModel *modelo;
};

#endif // FAMILIAS_H
