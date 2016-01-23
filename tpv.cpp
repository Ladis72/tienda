#include "tpv.h"
#include "ui_tpv.h"
#include "dialogfecha.h"

#include <QItemDelegate>
#include <QDebug>
#include <QDate>
#include <QString>

Tpv::Tpv(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Tpv)
{
    ui->setupUi(this);
    ticketActual = ticketActualizado();
    ticket = base.obtenerNumeroUltimoTicket(QSqlDatabase::database("DB"))+1;
    llenar_usuarios(QSqlDatabase::database("DB"));

    modeloTicket = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    cambiarTicket(ticketActual);

    modeloTicketPendiente = new QStandardItemModel(this);
    recuperarTicketsPendientes();
    ui->tableViewTicketsPendientes->hideColumn(0);
    ui->lineEdit_cod_cliente->setText("1");
    emit on_lineEdit_cod_cliente_editingFinished();


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

    //Llenar el combo con los usuarios disponibles

    consulta = base.usuarios(db);
    if(consulta.isActive()){

    while (consulta.next()) {
        //uno la columna 1 y 2 separada por "-" para luego poder obtener
        ui->comboBox_vendedor->addItem(consulta.value(0).toString()+"-"+consulta.value(1).toString());
        ui->comboBox_vendedor->update();

    }
    consulta.first();
    usuario = consulta.value(0).toInt();
    return true;}

    return false;

}

double Tpv::calcularPrecioTotal()
{
    double total = 0;
    if(modeloTicket->rowCount() == 0){
        ui->lcdNumber_total->display(total);
    }
    for (int i = 0; i < modeloTicket->rowCount(); ++i){
        total += modeloTicket->record(i).value(8).toDouble();

    }
    ui->lcdNumber_total->display(total);
    return total;
}

double Tpv::redondear(double num, int dec)
{

        if(num == 0) return 0;
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

void Tpv::calcularNumProductos()
{
    int total = 0;
    for (int i = 0; i < modeloTicket->rowCount(); ++i){
        QModelIndex indice = modeloTicket->index(i,4);
        total += modeloTicket->data(indice).toInt();

    }
    ui->lcdProductos->display(total);
}

bool Tpv::actualizarLineaTicket(QList<QString> lista)
{
    QSqlQuery query(QSqlDatabase::database("DB"));
    query.prepare("INSERT INTO lineasticket_tmp VALUES (NULL,?,?,?,?,?,?,?,?)");
    //query.prepare("INSERT INTO lineasticket_tmp (orden, cod, descripcion, cantidad, precio, descuento, totallinea) VALUES (?,?,?,?,?,?,?)");
    query.bindValue(0,ticketActual);
    query.bindValue(1,lista.at(0));
    query.bindValue(2,lista.at(1));
    query.bindValue(3,lista.at(2).toInt());
    query.bindValue(4,lista.at(3).toInt());
    query.bindValue(5,lista.at(4).toDouble());
    query.bindValue(6,lista.at(5).toDouble());
    query.bindValue(7,lista.at(6).toDouble());
    if(!query.exec()){
        qDebug() << query.lastError().text();
        return false;
    }

    return true;

}

void Tpv::recuperarTicketsPendientes()
{
    modeloTicketPendiente->clear();
    QSqlQuery listaTicketsPendientes;
    listaTicketsPendientes = base.tcketsPendientes(QSqlDatabase::database("DB"));
    listaTicketsPendientes.first();
    for (int i = 0; i < listaTicketsPendientes.numRowsAffected(); ++i) {
        QStandardItem *itemOrden = new QStandardItem(listaTicketsPendientes.value(0).toString());
        modeloTicketPendiente->setItem(i,0,itemOrden);
        QStandardItem *itemCliente = new QStandardItem(base.nombreCliente(listaTicketsPendientes.value(1).toString()));
        modeloTicketPendiente->setItem(i,1,itemCliente);
        QStandardItem *itemVendedor = new QStandardItem(base.nombreUsusario(listaTicketsPendientes.value(2).toString()));
        modeloTicketPendiente->setItem(i,2,itemVendedor);
        QStandardItem *itemCodCliente = new QStandardItem(listaTicketsPendientes.value(1).toString());
        modeloTicketPendiente->setItem(i,3,itemCodCliente);
        QStandardItem *itemCodVendedor = new QStandardItem(listaTicketsPendientes.value(2).toString());
        modeloTicketPendiente->setItem(i,4,itemCodVendedor);
        listaTicketsPendientes.next();

    }
    ui->tableViewTicketsPendientes->setModel(modeloTicketPendiente);
    ui->tableViewTicketsPendientes->hideColumn(0);
    ui->tableViewTicketsPendientes->hideColumn(3);
    ui->tableViewTicketsPendientes->hideColumn(4);





}

void Tpv::actualizarParrillaVentas()
{
    cambiarTicket(ticketActual);
}

void Tpv::cambiarTicket(int i)
{
    ticketActual = i;
    modeloTicket->setTable("lineasticket_tmp");
    modeloTicket->setFilter("orden LIKE "+QString::number(ticketActual));
    modeloTicket->select();

    ui->tableView->setModel(modeloTicket);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->resizeColumnsToContents();

    calcularPrecioTotal();
    calcularNumProductos();
}

void Tpv::ticketNuevo(int ticketnuevo)
{
    ticketActual = ticketnuevo;
    cliente = ui->lineEdit_cod_cliente->text().toInt();
    usuario = ui->comboBox_vendedor->currentText().left(1).toInt();
    base.nuevoTicketTmp(ticketActual,cliente,usuario);
    recuperarTicketsPendientes();
    actualizarParrillaVentas();
}

int Tpv::ticketActualizado()
{
    ticketActual = base.maxTicketPendiente(QSqlDatabase::database("DB"));

}

QStringList Tpv::recopilarDatosTicket()
{
    QStringList datosTicket;
    datosTicket.clear();
    datosTicket.append(QString::number(ticket));
    datosTicket.append(ui->comboBox_vendedor->currentText().left(1));
    datosTicket.append(ui->lineEdit_cod_cliente->text());
    datosTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datosTicket.append(QTime::currentTime().toString());
    datosTicket.append(recopilarBasesIvas());

    return datosTicket;
}

QStringList Tpv::recopilarBasesIvas()
{
    QStringList basesIvas;
    basesIvas.clear();
    double base1=0,base2=0,base3=0;
    for (int i = 0; i < modeloTicket->rowCount(); ++i) {
        int tipoIva = modeloTicket->record(i).value(5).toInt();
        qDebug() << tipoIva;
        switch (tipoIva) {
        case 4:
            base1 += modeloTicket->record(i).value(6).toDouble();
            break;
        case 10:
            base2 += modeloTicket->record(i).value(6).toDouble();
            break;
        case 21:
            base3 += modeloTicket->record(i).value(6).toDouble();
            break;
        default:
            break;
        }
    }
    basesIvas.append(QString::number(base1/1.04));
    basesIvas.append(QString::number(base1 - (base1/1.04)));
    basesIvas.append(QString::number(base2/1.1));
    basesIvas.append(QString::number(base2 - (base2/1.1)));
    basesIvas.append(QString::number(base3/1.21));
    basesIvas.append(QString::number(base3 - (base3/1.21)));


    return basesIvas;

}

QString Tpv::formatearCadena(QString cadena, int tamano)
{
    if (cadena.length() >= tamano) {
        cadena.resize(tamano);
       return cadena;
    }
    int diferencia = tamano-cadena.length();
    for (int i = 0; i < diferencia; ++i) {
        cadena += " ";
    }
    return cadena;
}

void Tpv::on_lineEdit_cod_returnPressed(){

   consulta = base.consulta_producto(QSqlDatabase::database("DB"),ui->lineEdit_cod->text());
   consulta.first();
   if(!consulta.isValid()){
       QString cod = base.codigoDesdeAux(ui->lineEdit_cod->text());
       consulta = base.consulta_producto(QSqlDatabase::database("DB"),cod);
       consulta.first();
   }
   if (consulta.numRowsAffected() == 1) {
       QList<QString> linea;
       linea << consulta.value(0).toString();
       linea << consulta.value(1).toString();
       linea << ui->lineEdit_Uds->text();
       linea << consulta.value(3).toString();
       if(ui->lineEdit_precio->text().isEmpty()){
       linea << QString::number(consulta.value(2).toDouble());
            }else{
           linea << ui->lineEdit_precio->text();
       }
       if(ui->lineEdit_descuento->text() == "0"){
           qDebug() << "Descuento en line 0";
           linea << QString::number(descuentoCliente);

           }else{
            linea << ui->lineEdit_descuento->text();
       }
       double totalLinea = linea.at(4).toDouble()*linea.at(2).toDouble()*(1-linea.at(5).toDouble()/100);
       totalLinea = redondear(totalLinea,2);
       linea << QString::number(totalLinea);

       if(ui->tableView->rowAt(0) < 0){
           ticketNuevo(base.maxTicketPendiente(QSqlDatabase::database("DB"))+1);
       }
       actualizarLineaTicket(linea);
       actualizarParrillaVentas();

       ui->lineEdit_Uds->setText("1");
       ui->lineEdit_descuento->setText("0");
       ui->lineEdit_desc->clear();
       ui->lineEdit_precio->clear();
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
          ui->lineEdit_precio->setFocus();
          ui->lineEdit_precio->selectAll();
      break;

        case Qt::Key_F7:
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
        modeloTicket->removeRow(ui->tableView->currentIndex().row());
        actualizarParrillaVentas();
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
    QFile cajon("/dev/lp1");
    cajon.open(QIODevice::WriteOnly);
    QTextStream codigoApertura(&cajon);
    codigoApertura << char(0x1B) << char(0x70) << char(0x30);
    cajon.close();
    totalizacion = new totalizar(QString::number(calcularPrecioTotal()),this);

    QStringList lineaTicket,totalTicket;
    totalTicket.clear();

    if (totalizacion->exec() == totalizacion->Accepted){
        //QFile impresora("/dev/lp0");
        QFile impresora("ticket.txt");

        impresora.open(QIODevice::WriteOnly);
            QTextStream texto(&impresora);
            texto << "\n";
            texto << "HERBOLARIO EMEICJAC\n";
            texto << "C/Perines 14 bajo\n";
            texto << "Tlfn: 942-37-20-27\n";
            texto << "N.I.F.: 20196639-V\n";
            texto << "E-mail: emeicjac@emeicjac.com\n";
            texto << "Web: emeicjac.com\n\n";
            texto << QDateTime::currentDateTime().toString("dd-MMM-yyyy  HH:mm:ss");
            texto << "    Ticket:" << QString::number(ticket);
            texto <<"\n";
            texto << "UDS | Producto            |Prec.|Dto|Total\n";
            texto << "------------------------------------------\n";
        for(int i = 0; i < modeloTicket->rowCount(); i++){
            lineaTicket.clear();
            if (totalizacion->facturacion == "1" && base.existeDatoEnTabla(QSqlDatabase::database("DB"),"ticketss","ticket",QString::number(ticket)) == false) {
                qDebug() << "Serie" << totalizacion->facturacion;
                qDebug() << "Facturación por el B";
                lineaTicket.append("B"+QString::number(ticket));
            }else{
            lineaTicket.append(QString::number(ticket));
            totalizacion->facturacion = "0";
            qDebug() << "Facturación por el A";

            }
            for (int x = 2; x < modeloTicket->columnCount(); ++x) {
                lineaTicket.append(modeloTicket->record(i).value(x).toString());
                }
            lineaTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
            lineaTicket.append(QTime::currentTime().toString("hh:mm"));
            QString dato;
            dato = lineaTicket.at(3);
            dato = formatearCadena(dato,3);
            qDebug() << dato;
            texto << dato;
            //texto << " ";
            dato = lineaTicket.at(2);
            dato = formatearCadena(dato,23);
            qDebug() << dato;
            texto << dato;
            texto << " ";
            dato = lineaTicket.at(5);
            dato = formatearCadena(dato,6);
            texto << dato;
            //texto << " ";
            dato.clear();
            dato = lineaTicket.at(6);
            dato = formatearCadena(dato,2);
            texto << dato;
            texto << " ";
            dato.clear();
            dato = lineaTicket.at(7);
            dato = formatearCadena(dato,6);
            texto << dato;
            texto << "\n";
            dato.clear();
            qDebug() << lineaTicket;
            base.grabarLineaTicket(lineaTicket);
            if (lineaTicket.at(3).toInt() < 0) {
                DialogFecha *fechaCaducidad = new DialogFecha(lineaTicket.at(2));
                fechaCaducidad->exec();
                QString fecha = fechaCaducidad->fecha.toString("yyyy-MM-dd");
                QString lote = fechaCaducidad->lote;
                QString idLote = base.idLote(lineaTicket.at(1),lote,fecha);
                qDebug() << lote;
                if (idLote != "0") {
                    base.aumentarLote(idLote,abs(lineaTicket.at(3).toInt()));
                    qDebug() << "Error al devolver el producto lote";
                } else {
                    base.crearLote(lineaTicket.at(1),lote,fecha,QString::number(abs(lineaTicket.at(3).toInt())));
                    qDebug() << "Crear lote";
                }
            }else{
                base.descontarArticulo(lineaTicket.at(1),lineaTicket.at(3).toInt());
            }
            base.actualizarFechaVentaArticulo(lineaTicket.at(1),QDate::currentDate().toString("yyyy-MM-dd"));
        }
        totalTicket.append(recopilarDatosTicket());
        totalTicket.append(QString::number(totalizacion->descuento));
        totalTicket.append(QString::number(totalizacion->total));
        totalTicket.append(base.idFormaPago(totalizacion->efectivo));
        totalTicket.append(totalizacion->facturacion);

        texto << "\n\nTotal:";
        texto << QString::number(totalizacion->total)+"\n";
        texto << totalizacion->efectivo;
        texto << "\n\n\n     GRACIAS POR SU VISITA\n";
        texto << "\n\n\n\n";
        texto << char(0x1D) << char(0x56) << char(0x30);
        QString serie;
        serie = "ticketss";
        if (totalizacion->facturacion == "0") {
            serie = "tickets";
            ticket +=1;
        }

        base.grabarTicket(serie,totalTicket);
        qDebug() << totalTicket;
        qDebug() << serie;

        emit on_pushButtonBorrarTodo_clicked();
        impresora.close();
        if (totalizacion->ticket == true) {
            system("less ./ticket.txt >> /dev/lp0");
        }
    }

}

void Tpv::on_tableViewTicketsPendientes_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTicketPendiente->index(index.row(),0);
    QString dato = modeloTicketPendiente->data(indice,Qt::EditRole).toString();
    cambiarTicket(dato.toInt());

    indice = modeloTicketPendiente->index(index.row(),3);
    ui->lineEdit_cod_cliente->setText(modeloTicketPendiente->data(indice,Qt::EditRole).toString());
    emit on_lineEdit_cod_cliente_editingFinished();

    indice = modeloTicketPendiente->index(indice.row(),4);
    ui->comboBox_vendedor->setCurrentIndex(modeloTicketPendiente->data(indice,Qt::EditRole).toInt()-1);
    ui->lineEdit_cod->setFocus();
}

void Tpv::on_pushButtonBorrarTodo_clicked()
{
    for (int i = 0; i < modeloTicket->rowCount(); ++i) {
        modeloTicket->removeRow(i);
    }
    base.borrarTicketTmp(ticketActual);
    recuperarTicketsPendientes();
    ui->lineEdit_cod_cliente->setText("1");
    emit on_lineEdit_cod_cliente_editingFinished();
    cambiarTicket(ticketActualizado());

    actualizarParrillaVentas();
}

void Tpv::on_btn_modificar_clicked()
{

    ticketActual = base.maxTicketPendiente(QSqlDatabase::database("DB"))+1;
    actualizarParrillaVentas();
    ui->lineEdit_cod->clear();
    ui->lineEdit_cod->setFocus();
}

void Tpv::on_lineEdit_cod_cliente_editingFinished()
{
    QString nombreCliente;
    nombreCliente = base.nombreCliente(ui->lineEdit_cod_cliente->text());
    if(nombreCliente == "Sin asignar"){
        QMessageBox *msgbox = new QMessageBox(this);
        msgbox->setText("Crear cliente");
        msgbox->setInformativeText("Ese cliente no existe. /n¿Desea Crearlo?");
        msgbox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgbox->setDefaultButton(QMessageBox::Ok);
        int resp = msgbox->exec();
        if( resp == QMessageBox::Ok){
            clien = new Clientes(this,ui->lineEdit_cod_cliente->text());
            clien->exec();
            ui->lineEdit_nobre_cliente->setText(base.nombreCliente(ui->lineEdit_cod_cliente->text()));

        }else{
            ui->lineEdit_cod_cliente->setText("1");
            ui->lineEdit_nobre_cliente->setText(base.nombreCliente(ui->lineEdit_cod_cliente->text()));
        }
    }else{
    ui->lineEdit_nobre_cliente->setText(nombreCliente);
        }
    descuentoCliente = base.descuentoCliente(ui->lineEdit_cod_cliente->text());
    qDebug() << descuentoCliente;
}

void Tpv::on_lineEdit_nobre_cliente_returnPressed()
{
    consulta = base.buscarEnTabla(QSqlDatabase::database("DB"),"clientes","nombre", ui->lineEdit_nobre_cliente->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    qDebug() << buscar->resultado;
    ui->lineEdit_cod_cliente->setText(buscar->resultado);
    emit on_lineEdit_cod_cliente_editingFinished();
    delete buscar;
}

void Tpv::on_lineEdit_precio_returnPressed()
{

}

void Tpv::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTicket->index(index.row(),0);
    int idModeloTicket = indice.row();
    ui->lineEdit_cod->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,2)).toString());
    ui->lineEdit_desc->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,3)).toString());
    ui->lineEdit_Uds->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,4)).toString());
    ui->lineEdit_precio->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,6)).toString());
    ui->lineEdit_descuento->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,7)).toString());
    ui->lineEdit_6->setText(modeloTicket->data(modeloTicket->index(idModeloTicket,8)).toString());
    modeloTicket->removeRow(ui->tableView->currentIndex().row());

}
