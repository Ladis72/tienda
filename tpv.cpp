#include "tpv.h"
#include "ui_tpv.h"
#include <QItemDelegate>
#include <QDebug>

Tpv::Tpv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tpv)
{
    ui->setupUi(this);
    ticket = base.obtenerNumeroUltimoTicket(QSqlDatabase::database("DB"))+1;
    llenar_usuarios(QSqlDatabase::database("DB"));
    recuperarTicketsPendientes();

}

Tpv::~Tpv()
{
    delete ui;
    //db.removeDatabase("DB");
}

void Tpv::on_btn_cerrar_clicked()
{

    emit cerrar_tpv();
    //this->deleteLater();


}

bool Tpv::llenar_usuarios(QSqlDatabase db)
{

    //Llenar el combo con los usuarios diaponibles

    consulta = base.usuarios(db);
    qDebug() << db.tables();
    if(consulta.isActive()){

    while (consulta.next()) {
        //uno la columna 1 y 2 separada por "-" para luego poder obtener
        ui->comboBox_vendedor->addItem(consulta.value(0).toString()+"-"+consulta.value(1).toString());
        ui->comboBox_vendedor->update();
        qDebug() << consulta.value(1).toString();

    };
    return true;}

    return false;

}

double Tpv::calcularPrecioTotal()
{
    double total = 0;

    for (int i = 0; i < ui->parrilla_ventas->rowCount(); ++i){
        total += ui->parrilla_ventas->item(i,5)->text().toDouble();
        //ui->lcdNumber_total->display(total);
    }
    return total;
}

double Tpv::redondear(double num, int dec)
{


        double z;
        int a,c,b,cont=1;
        c=dec;


        for (int i=0;i<=dec;i++)
            cont=cont*10;

        if(c!=num){
                 a=num*cont;
                 z=a/10;
                 b=a%10;
                 cont=cont/10;
                 if(b>=5){
                          z=z+1;
                          }
                 num=z/cont;
                 }


        return num;

}

int Tpv::calcularNumProductos()
{
    int total = 0;
    for (int i = 0; i < ui->parrilla_ventas->rowCount(); ++i){
        total += ui->parrilla_ventas->item(i,2)->text().toInt();
    }
    return total;
}

bool Tpv::actualizarLineaTicket(QList<QString> lista)
{
    QSqlQuery query(QSqlDatabase::database("DB"));
    query.prepare("INSERT INTO lineasticket_tmp (orden, cod, descripcion, cantidad, precio, descuento, totallinea) VALUES (?,?,?,?,?,?,?)");
    query.bindValue(0,9);
    query.bindValue(1,lista.at(0));
    query.bindValue(2,lista.at(1));
    query.bindValue(3,lista.at(2).toInt());
    query.bindValue(4,lista.at(3).toDouble());
    query.bindValue(5,lista.at(4).toDouble());
    query.bindValue(6,lista.at(5).toDouble());
    qDebug() << lista;
    if(!query.exec()){
        qDebug() << query.lastError().text();
    }
}

void Tpv::recuperarTicketsPendientes()
{
    QSqlQuery listaTicketsPendientes;
    listaTicketsPendientes = base.tcketsPendientes(QSqlDatabase::database("DB"));
    while (listaTicketsPendientes.next() == true) {
        qDebug() << listaTicketsPendientes.value(0).toInt();
        qDebug() << listaTicketsPendientes.value(1).toInt();
        qDebug() << base.nombreUsusario(listaTicketsPendientes.value(2).toString());

    }

}

void Tpv::on_lineEdit_cod_returnPressed(){

   consulta = base.consulta_producto(QSqlDatabase::database("DB"),ui->lineEdit_cod->text());
   consulta.first();
   if (consulta.numRowsAffected() == 1) {
       QList<QString> linea;
       linea << consulta.value(0).toString();
       linea << consulta.value(1).toString();
       linea << ui->lineEdit_Uds->text();
       linea << QString::number(consulta.value(2).toDouble());
       linea << ui->lineEdit_descuento->text();
       double totalLinea = consulta.value(2).toDouble()*ui->lineEdit_Uds->text().toDouble()*(1-(ui->lineEdit_descuento->text().toDouble()/100));
       totalLinea = redondear(totalLinea,2);
       linea << QString::number(totalLinea);

       actualizarLineaTicket(linea);

       ui->lineEdit_Uds->setText("1");
       ui->lineEdit_descuento->setText("0");
       ui->lineEdit_cod->setFocus();
       ui->lineEdit_cod->clear();
   }
}

void Tpv::keyPressEvent(QKeyEvent *e)
{

      switch ( e->key() )
      {
        case Qt::Key_F5:
            ui->lineEdit_Uds->setFocus();
            ui->lineEdit_Uds->selectAll();
        break;

        case Qt::Key_F6:
            ui->lineEdit_descuento->setFocus();
            ui->lineEdit_descuento->selectAll();
        break;

        case Qt::Key_F1:
            if(ui->comboBox_vendedor->count()>=1)
            ui->comboBox_vendedor->setCurrentIndex(0);
        break;

        case Qt::Key_F2:
            if(ui->comboBox_vendedor->count() >= 2)
            ui->comboBox_vendedor->setCurrentIndex(1);
        break;

        case Qt::Key_F3:
            if(ui->comboBox_vendedor->count() >= 3)
            ui->comboBox_vendedor->setCurrentIndex(2);
        break;

        case Qt::Key_F4:
            if(ui->comboBox_vendedor->count() >= 4)
            ui->comboBox_vendedor->setCurrentIndex(3);
        break;

        case Qt::Key_F8:
          emit on_btn_cobrar_clicked();
        break;

      }
      //Tpv::keyPressEvent(e);
    }


void Tpv::on_btn_anadir_clicked()
{
    emit on_lineEdit_cod_returnPressed();
}

void Tpv::on_btn_borrar_clicked()
{
    QModelIndex indice = ui->parrilla_ventas->currentIndex();
    if (indice.isValid()){
    ui->parrilla_ventas->removeRow(indice.row());

    qDebug() << indice;

    ui->lcdNumber_total->display(calcularPrecioTotal());
    ui->lcdProductos->display(calcularNumProductos());

    }
//qDebug() << ui->parrilla_ventas->currentItem()->text();
}

void Tpv::on_lineEdit_desc_returnPressed()
{
    consulta = base.buscarProducto(QSqlDatabase::database("DB"),"articulos",ui->lineEdit_desc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    ui->lineEdit_cod->setText(buscar->resultado);
    emit on_lineEdit_cod_returnPressed();
    }


void Tpv::on_btn_cobrar_clicked()
{
//    for (int i = 0; i < ui->parrilla_ventas->rowCount(); i++) {
//        for (int l = 0; l < ui->parrilla_ventas->columnCount(); ++l) {
//            qDebug() <<  ui->parrilla_ventas->item(i,l)->text();

//        }
//        qDebug() << ui->comboBox_vendedor->currentText().split("-");
//    }
    QStringList datos;

    datos.insert(0,QString::number(calcularPrecioTotal()));
    totalizacion = new totalizar(datos,this);

    if (totalizacion->exec() == totalizacion->Accepted){
        qDebug() << "Funciona";
        qDebug() << totalizacion->efectivo;
    }

}
