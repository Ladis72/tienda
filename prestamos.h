#ifndef PRESTAMOS_H
#define PRESTAMOS_H

#include <QDialog>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;
namespace Ui {
class Prestamos;
}

class Prestamos : public QDialog
{
    Q_OBJECT

public:
    explicit Prestamos(QWidget *parent = 0);
    ~Prestamos();

private slots:
    void on_lineEdit_textChanged(const QString &arg1);

    void on_lineEditCod_textChanged(const QString &arg1);

    void on_lineEditEan_returnPressed();

    void on_pushButtonProcesar_clicked();

    void on_lineEditDescripcion_returnPressed();

private:
    Ui::Prestamos *ui;
    baseDatos *base;
    QSqlQuery consulta;
    QSqlTableModel *modeloPrestamos;
};

#endif // PRESTAMOS_H
