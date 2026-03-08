#ifndef CONFIGTICKET_H
#define CONFIGTICKET_H

#include <QDialog>
#include "base_datos.h"

namespace Ui {
class ConfigTicket;
}

class ConfigTicket : public QDialog
{
    Q_OBJECT

public:
    explicit ConfigTicket(QWidget *parent = 0);
    ~ConfigTicket();

private slots:
    void on_pushButtonCancelar_clicked();

    void on_pushButtonAceptar_clicked();

    void on_checkBoxPromo_toggled(bool checked);

private:
    Ui::ConfigTicket *ui;
    baseDatos *base;
};

#endif // CONFIGTICKET_H
