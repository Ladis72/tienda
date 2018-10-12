#ifndef FORMATOS_H
#define FORMATOS_H

#include <QDialog>
#include "base_datos.h"
#include <QMessageBox>
#include <QKeyEvent>

namespace Ui {
class Formatos;
}

class Formatos : public QDialog
{
    Q_OBJECT

public:
    explicit Formatos(QWidget *parent = nullptr);
    ~Formatos();

private slots:

    void on_pushButtonModificar_clicked();

    void on_pushButtonnadir_clicked();

    void on_lineEditFiltro_textChanged(const QString &arg1);

private:
    Ui::Formatos *ui;
    QSqlTableModel *modelo;
    void keyPressEvent(QKeyEvent *e);
};

#endif // FORMATOS_H
