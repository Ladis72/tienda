#ifndef CADUCADOS_H
#define CADUCADOS_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class Caducados;
}

class Caducados : public QDialog
{
    Q_OBJECT

public:
    explicit Caducados(QWidget *parent = 0);
    ~Caducados();

private slots:
    void on_lineEditCodigo_returnPressed();

    void on_pushButton_clicked();

private:
    Ui::Caducados *ui;
    baseDatos base;
    QSqlQuery consulta;

    void llenarComboFechas(QString id);
    void ajustarSpinbox(QString idLote);
};

#endif // CADUCADOS_H
