#include "actualizarclientes.h"
#include "ui_actualizarclientes.h"

ActualizarClientes::ActualizarClientes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActualizarClientes)
{
    ui->setupUi(this);
    conexiones = conf->getNombreConexiones();
    qDebug() << conexiones;
    for (int i = 0 ; i < conexiones.length() ; i++ ) {
    ui->listWidget->addItem(conexiones.at(i));
    }
}

ActualizarClientes::~ActualizarClientes()
{
    delete ui;
}

void ActualizarClientes::on_pushButtonActualizar_clicked()
{
    //Borrar tabla clientes remota y copiar la local
//    base.vaciarTabla("clientesTMP",QSqlDatabase::database("local"));
//    QString sentencia;
//    sentencia = "mysqldump -v --no-create-info -h "+QSqlDatabase::database("local").hostName()+
//            " -u "+QSqlDatabase::database("local").userName()+
//            " -p"+QSqlDatabase::database("local").password()+
//            " tienda clientes | mysql -h "+QSqlDatabase::database("local").hostName()+
//            " -u "+QSqlDatabase::database("local").userName()+
//            " -p"+QSqlDatabase::database("local").password()+" tienda2";
//    qDebug() << sentencia;
//    const char* ch = sentencia.toLocal8Bit().constData();
//    qDebug() << system(ch);
    bool exito = true;
    QString conexionLocal = conf->getConexionLocal();
    qDebug() << conexionLocal;
    for (int i = 0 ; i < conexiones.length() ; i++) {
        QString sentencia ="";
        QString conexionActual = conexiones.at(i);
        if(QSqlDatabase::database(conexionActual).isOpen()){
        base.vaciarTabla("clientes",QSqlDatabase::database(conexionActual));
        sentencia = "mysqldump -v --no-create-info -h "+QSqlDatabase::database(conexionLocal).hostName()+
                " -u "+QSqlDatabase::database(conexionLocal).userName()+
                " -p"+QSqlDatabase::database(conexionLocal).password()+
                " tienda clientes | mysql -h "+QSqlDatabase::database(conexionActual).hostName()+
                " -u "+QSqlDatabase::database(conexionActual).userName()+
                " -p"+QSqlDatabase::database(conexionActual).password()+" tienda";
        qDebug() << sentencia;
        const char* ch = sentencia.toLocal8Bit().constData();
        qDebug() << system(ch);

        ui->listWidget->item(i)->setForeground(Qt::green);
        }else {
        qDebug() << "Proceso no ejecutado";
        ui->listWidget->item(i)->setForeground(Qt::red);
        exito = false;
        }
        }
    if (exito) {
        // Grabar clientes actualizados
    }
}
