#include "generarvales.h"
#include "ui_generarvales.h"


GenerarVales::GenerarVales(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GenerarVales)
{
    ui->setupUi(this);
    tiendas = conf->getNombreConexiones();
    qDebug() << tiendas;
    QString master = conf->getConexionMaster();
    qDebug() << master;
    ui->dateEdit->setDate(QDate::currentDate());

}

GenerarVales::~GenerarVales()
{
    delete ui;
}

void GenerarVales::on_pushButtonGenerar_clicked()
{

    //Comprobar fecha correcta
    ui->label->setText("Comprobando la fecha");
    ui->progressBar->setValue(5);
    QDate fecha = ui->dateEdit->date();
    QDate fechaActual = QDate::currentDate();

    if (fecha >= fechaActual) {
        qDebug() << "la fecha es posterior a hoy";
        return;
    }
    if(fecha.month() == QDate::currentDate().month()){
        qDebug() << "No se pueden generar los vales de un mes en curso";
        return;
    }
    if (base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),"vales","fechaEmision",fecha.toString("yyyy-MM-01"))) {
        qDebug() << "Ya exiten vales para esa fecha";
        return;
    }
    qDebug() << fechaI;

    //Marcar como caducados los vales no usados
    bool caducar = base.caducarVales(conf->getConexionLocal());
    for (int i = 0;i < tiendas.length() ;i++ ) {
        caducar = base.caducarVales(tiendas.at(1));
    }

    //Vaciar tabla temporal de compras de clientes
    ui->label->setText("Vaciando datos antiguos");
    ui->progressBar->setValue(10);
    base.vaciarTabla("comprasClienteTMP",QSqlDatabase::database(conf->getConexionLocal()));

    //Cargar datos del master
    ui->label->setText("Cargando datos locales");
    ui->progressBar->setValue(15);
    QSqlQuery compras = base.ventasClientes(conf->getConexionLocal(),fecha,fecha);
    compras.first();
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));

    for (int i = 0; i < compras.numRowsAffected() ;i++ ) {
        consulta.prepare("INSERT INTO comprasClienteTMP VALUES (NULL, ? , ? , ?)");
        consulta.bindValue(0,compras.record().value(0).toString());
        consulta.bindValue(1,compras.record().value(1).toString());
        consulta.bindValue(2,fecha.toString("yyyy-MM-01"));
        if (!consulta.exec()) {
            qDebug() << consulta.lastError();
        }
        qDebug() << compras.record().value(0) << " = " << compras.record().value(1);
        compras.next();
    }
    //Cargar datos remotos
    ui->label->setText("Cargando datos remotos");
    ui->progressBar->setValue(20);
    for (int i = 0;i < tiendas.length() ;i++ ) {
        QSqlQuery compras = base.ventasClientes(tiendas.at(i),fecha,fecha);
        compras.first();
        qDebug() << compras.lastError();
    //QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
        for (int i = 0; i < compras.numRowsAffected() ;i++ ) {
            consulta.prepare("INSERT INTO comprasClienteTMP VALUES (NULL, ? , ? , ?)");
            consulta.bindValue(0,compras.record().value(0).toString());
            consulta.bindValue(1,compras.record().value(1).toString());
            consulta.bindValue(2,fecha.toString("yyyy-MM-01"));
            if (!consulta.exec()) {
                qDebug() << consulta.lastError();
            }
            qDebug() << compras.record().value(0) << " = " << compras.record().value(1);
            compras.next();
        }
    ui->progressBar->setValue(ui->progressBar->value()+30/tiendas.length());
    }

    //Generar vales locales
    ui->label->setText("Generando los vales");
    QSqlQuery comprasTotal("SELECT * , sum(cantidad) FROM comprasClienteTMP GROUP BY idCliente",QSqlDatabase::database(conf->getConexionLocal()));
    comprasTotal.first();
    QSqlQuery vales(QSqlDatabase::database(conf->getConexionLocal()));
    for (int i = 0; i < comprasTotal.numRowsAffected() ;i++ ) {
        vales.prepare("INSERT INTO vales VALUES (NULL, ? , ? , 1 , ? , NULL)");
        vales.bindValue(0, comprasTotal.record().value(1));
        vales.bindValue(1, comprasTotal.record().value(4).toDouble()*descuentoCliente(comprasTotal.record().value(1).toInt())/100);
        vales.bindValue(2, comprasTotal.record().value(3));
        if (!vales.exec()) {
            qDebug() << vales.lastError();
        }
        comprasTotal.next();
    }

    //Generar vales remotos
    for (int i = 0;i < tiendas.length() ;i++ ) {
        comprasTotal.first();
        QSqlQuery valesRemotos(QSqlDatabase::database(tiendas.at(i)));
        for (int i = 0; i < comprasTotal.numRowsAffected() ;i++ ) {
            valesRemotos.prepare("INSERT INTO vales VALUES (NULL, ? , ? , 1 , ? , NULL)");
            valesRemotos.bindValue(0, comprasTotal.record().value(1));
            valesRemotos.bindValue(1, comprasTotal.record().value(4).toDouble()*descuentoCliente(comprasTotal.record().value(1).toInt())/100);
            valesRemotos.bindValue(2, comprasTotal.record().value(3));
            if (!valesRemotos.exec()) {
                qDebug() << vales.lastError();
            }
            comprasTotal.next();
    }
    }

}


double GenerarVales::descuentoCliente(int idCliente)
{
    double descuento = base.descuentoCliente(QString::number(idCliente));
    qDebug() << idCliente << "  " << descuento;
    return descuento;
}
