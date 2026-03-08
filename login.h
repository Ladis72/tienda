#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;
namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::Login *ui;
    baseDatos *base;
};

#endif // LOGIN_H
