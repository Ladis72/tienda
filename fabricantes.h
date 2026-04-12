#ifndef FABRICANTES_H
#define FABRICANTES_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class Fabricantes;
}

class Fabricantes : public QDialog
{
    Q_OBJECT

public:
    explicit Fabricantes(QWidget *parent = 0);
    ~Fabricantes();

    QString resultado;

private slots:
    void on_pushButtonSeleccionar_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButtonAnadir_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonDeshacer_clicked();

    void on_pushButtonBorrar_clicked();

    void on_tableView_clicked(const QModelIndex &index);

    void on_lineEdit_textChanged(const QString &arg1);

    /**
     * @brief Abre el diálogo de unificación genérico para fabricantes.
     */
    void on_pushButtonUnificar_clicked();
    void on_pushButtonSaneador_clicked();

private:
    Ui::Fabricantes *ui;

    QSqlTableModel *modelo;
};

#endif // FABRICANTES_H
