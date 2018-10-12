#ifndef TIPOSENTRADASSALIDAS_H
#define TIPOSENTRADASSALIDAS_H

#include <QDialog>
#include "base_datos.h"
#include "QMessageBox"

namespace Ui {
class TiposEntradasSalidas;
}

class TiposEntradasSalidas : public QDialog
{
    Q_OBJECT

public:
    explicit TiposEntradasSalidas(QWidget *parent = nullptr);
    ~TiposEntradasSalidas();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::TiposEntradasSalidas *ui;
    QSqlTableModel *modelo;

};

#endif // TIPOSENTRADASSALIDAS_H
