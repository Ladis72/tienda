#include "login.h"
#include "ui_login.h"

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_pushButton_clicked()
{
    QString usuario = ui->lineEditUser->text();
    QString password = ui->lineEditpwd->text();

    QString sentencia
        = QString("SELECT nombre , rol FROM usuarios WHERE usuario = '%1' AND clave = '%2'")
              .arg(usuario)
              .arg(password);
    qDebug() << sentencia;
    QSqlQuery consulta = base->ejecutarSentencia(sentencia, conf->getConexionLocal());
    if (consulta.exec() && consulta.next()) {
        qDebug() << "usuario= " << consulta.value(0).toString();
        int rol = consulta.value(1).toInt();
        conf->setUsuario(usuario);
        conf->setRol(rol);
        accept();
    }

}

void Login::on_pushButton_2_clicked()
{
    reject();
}
