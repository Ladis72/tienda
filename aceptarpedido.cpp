#include "aceptarpedido.h"
#include "ui_aceptarpedido.h"
#include <QMessageBox>
#include <QInputDialog>

AceptarPedido::AceptarPedido(QString pedido, QString proveedor, QString nDoc, QString fechaPedido, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AceptarPedido)
{
    ui->setupUi(this);
    correcto = false;
    idPedido = pedido;
    ui->labelProveedor->setText(proveedor);
    descuento=1;
    idProveedor = base.idProveedor(proveedor);
    descuentoReal=0;
    fecha = fechaPedido;
    ui->lineEditNDoc->setText(nDoc);
    ui->dateEditDocumento->setDate(QDate::fromString(fecha,"yyyy-MM-dd"));
    llenarTabla(idPedido,descuento);
    modeloPedido = new QSqlQueryModel(this);
    modeloPedido->setQuery(QString("SELECT * FROM lineaspedido_tmp WHERE idPedido = %1").arg(idPedido),QSqlDatabase::database("DB"));

}

AceptarPedido::~AceptarPedido()
{
    delete ui;
}

void AceptarPedido::llenarTabla(QString idPedido, double desc)
{
    ui->leIva10->setText(QString::number(base.sumarIvasPedido(idPedido,"10")*desc));
    ui->leIva21->setText(QString::number(base.sumarIvasPedido(idPedido,"21")*desc));
    ui->leIva4->setText(QString::number(base.sumarIvasPedido(idPedido,"4")*desc));
    ui->leRe4->setText(QString::number(base.sumarRePedido(idPedido,"4")*desc));
    ui->leRe10->setText(QString::number(base.sumarRePedido(idPedido,"10")*desc));
    ui->leRe21->setText(QString::number(base.sumarRePedido(idPedido,"21")*desc));
    ui->leBase4->setText(QString::number(base.sumarBasesPedido(idPedido,"4")*desc));
    ui->leBase10->setText(QString::number(base.sumarBasesPedido(idPedido,"10")*desc));
    ui->leBase21->setText(QString::number(base.sumarBasesPedido(idPedido,"21")*desc));
    ui->leTotal4->setText(QString::number(ui->leBase4->text().toDouble()+ui->leIva4->text().toDouble()+ui->leRe4->text().toDouble()));
    ui->leTotal10->setText(QString::number(ui->leBase10->text().toDouble()+ui->leIva10->text().toDouble()+ui->leRe10->text().toDouble()));
    ui->leTotal21->setText(QString::number(ui->leBase21->text().toDouble()+ui->leIva21->text().toDouble()+ui->leRe21->text().toDouble()));
    ui->leTotalBase->setText(QString::number(ui->leBase4->text().toDouble()+ui->leBase10->text().toDouble()+ui->leBase21->text().toDouble()));
    ui->leTotalIva->setText(QString::number(ui->leIva4->text().toDouble()+ui->leIva10->text().toDouble()+ui->leIva21->text().toDouble()));
    ui->leTotalRe->setText(QString::number(ui->leRe4->text().toDouble()+ui->leRe10->text().toDouble()+ui->leRe21->text().toDouble()));
    ui->leTotal->setText(QString::number(ui->leTotalBase->text().toDouble()+ui->leTotalIva->text().toDouble()+ui->leTotalRe->text().toDouble()));

}

bool AceptarPedido::procesarPedido(QSqlQueryModel *modelo)
{
    QStringList datos;
    QString idLinea , ean,descripcion, lote, fechaCaducidad, descuentoLinea , tipoIva , baseProducto , baseLinea , ivaLinea , reLinea;
    int uds , unidades, bonificacion, tipoActualizacion = 0; //0-preguntar 1-pedido 2 almacen
    double precioCosto, pvp ;
    if (ui->comboBox->currentText() == "Seleccionar") {
        QMessageBox msg;
        msg.setText("Seleccione primero");
        msg.setInformativeText("Antes de poder aceptar el pedido debe indicar si es un albarán o una factura");
        msg.exec();
        return false;
    }
    if (ui->lineEditNDoc->text().isEmpty()) {
        QMessageBox msg;
        msg.setText("Falta el número de documento");
        msg.setInformativeText("Antes de poder aceptar el pedido debe indicar el número del documento al que pertenece");
        msg.exec();
        return false;
    }

    for (int i = 0; i < modelo->rowCount(); ++i) {
        datos.clear();
        idLinea = modelo->record(i).value("id").toString();
        ean = modelo->record(i).value("cod").toString();
        unidades = modelo->record(i).value("cantidad").toInt();
        bonificacion = modelo->record(i).value("bonificacion").toInt();
        uds = unidades+bonificacion;
        descuentoLinea = modelo->record(i).value("descuento1").toString();
        descripcion = modelo->record(i).value("descripcion").toString();
        lote = modelo->record(i).value("lote").toString();
        fechaCaducidad = modelo->record(i).value("fc").toString();
        precioCosto = modelo->record(i).value("costo").toDouble();
        pvp = modelo->record(i).value("pvp").toDouble();
        baseProducto = modelo->record(i).value("base").toString();
        tipoIva = modelo->record(i).value("tipoIva").toString();
        baseLinea = modelo->record(i).value("totalbase").toString();
        ivaLinea = modelo->record(i).value("iva").toString();
        reLinea = modelo->record(i).value("re").toString();
        //qDebug() << ean << " " << descripcion << " " << uds << " " << lote << " " << fechaCaducidad << " " << precioCosto << " " <<pvp;

        // Buscar si hay lotes con unidades pendientes
        QString idLote = base.idLote(ean,"","2000-01-01");
        qDebug() << "IDLOTE "+ idLote;
        if (idLote != "0") {
            qDebug() << "IDLOTE "+ idLote;
            int pendientes = base.unidadesLote(idLote);
            qDebug() << "PENDIENTES: "+QString::number(pendientes);
            if(abs(pendientes) > uds ){
                base.aumentarLote(idLote,uds);
            }else if (abs(pendientes) == uds) {
                base.ejecutarSentencia("DELETE FROM lotes WHERE id = '"+idLote+"'");
            }else {
                base.ejecutarSentencia("DELETE FROM lotes WHERE id = '"+idLote+"'");
                uds = uds+pendientes;
                //Modificar o crear lotes
                idLote = base.idLote(ean,lote,fechaCaducidad);
                if (idLote == "0") {
                    base.crearLote(ean,lote,fechaCaducidad,QString::number(uds));
                    qDebug() << "Creando lote";
                } else {
                    base.aumentarLote(idLote,uds);

                }
            }
        }else {
            //Modificar o crear lotes
            idLote = base.idLote(ean,lote,fechaCaducidad);
            if (idLote == "0") {
                base.crearLote(ean,lote,fechaCaducidad,QString::number(uds));
                qDebug() << "Creando lote";
            } else {
                base.aumentarLote(idLote,uds);

            }
        }


        //Comprobar cambio de nombre o PVP y actualizar artículos
        consulta = base.consulta_producto("DB",ean);
        consulta.first();
        QString descripcionAnterior = consulta.value("descripcion").toString();
        QString precioAnterior = consulta.value("pvp").toString();
        switch (tipoActualizacion) {
        case 2:
            descripcion=descripcionAnterior;
            pvp = precioAnterior.toDouble();
            break;
        case 0:
            if (!(descripcionAnterior == descripcion && precioAnterior == QString::number(pvp))) {
                cdProducto = new CambioDatosProducto(descripcion,descripcionAnterior,QString::number(pvp),precioAnterior,this);
                cdProducto->exec();
                descripcion = cdProducto->nombre;
                pvp = cdProducto->pvp.toDouble();
                tipoActualizacion = cdProducto->tipoActualizacion;
                if (precioAnterior != QString::number(pvp)) {
                    base.insertarEtiqueta(ean);
                }
            }

        case 1:
            if (precioAnterior != QString::number(pvp)) {
                base.insertarEtiqueta(ean);
            }
        default:
            break;
        }


        datos.append(ean);
        datos.append(descripcion);
        datos.append(QString::number(pvp));
        datos.append(tipoIva);
        datos.append(QString::number(precioCosto));
        datos.append(fecha);
        if(base.actualizarArticulosDesdeCompras(datos)){
            qDebug() << "actualizando " << datos.at(1);
        }else{
          QMessageBox msg;
          msg.setText("ERROR");
          msg.setInformativeText("No se ha podido actualizar la información de la tabla artículos");
          msg.show();
          return false;
        }

//        qDebug() << ean;
//        qDebug() << uds;
//        qDebug() << descripcion;
//        qDebug() << lote;
//        qDebug() << fechaCaducidad;
//        qDebug() << precioCosto;
//        qDebug() << pvp;
        //Grabar linea pedido
        QStringList datosLineaPedido;
        datosLineaPedido.clear();
        datosLineaPedido.append(nDoc);
        datosLineaPedido.append(idProveedor);
        datosLineaPedido.append(ean);
        datosLineaPedido.append(descripcion);
        datosLineaPedido.append(QString::number(unidades));
        datosLineaPedido.append(QString::number(bonificacion));
        datosLineaPedido.append(lote);
        datosLineaPedido.append(fechaCaducidad);
        datosLineaPedido.append(QString::number(precioCosto));
        datosLineaPedido.append(descuentoLinea);
        datosLineaPedido.append(baseProducto);
        datosLineaPedido.append(tipoIva);
        datosLineaPedido.append(baseLinea);
        datosLineaPedido.append(ivaLinea);
        datosLineaPedido.append(reLinea);
        datosLineaPedido.append(QString::number(pvp));
        if (!base.pasarLineaPedidoAHistorico(datosLineaPedido)) {
            return false;
        }else{
            qDebug() << base.borrarPedido(idPedido);

        }


    }
    //Grabar pedido
    QStringList datosPedido;
    datosPedido.clear();
    datosPedido.append(idProveedor);
    datosPedido.append(nDoc);
    datosPedido.append(fecha);
    datosPedido.append(QString::number(modeloPedido->rowCount()));
//    double unidades = base.sumarColumna("lineaspedido_tmp","cantidad","idPedido",idPedido);
//    unidades += base.sumarColumna("lineaspedido_tmp","bonificacion","idPedido",idPedido);
    datosPedido.append(QString::number(unidades));
    datosPedido.append(QString::number(descuentoReal));
    datosPedido.append(ui->leBase21->text());
    datosPedido.append(ui->leIva21->text());
    datosPedido.append(ui->leRe21->text());
    datosPedido.append(ui->leBase10->text());
    datosPedido.append(ui->leIva10->text());
    datosPedido.append(ui->leRe10->text());
    datosPedido.append(ui->leBase4->text());
    datosPedido.append(ui->leIva4->text());
    datosPedido.append(ui->leRe4->text());
    datosPedido.append(ui->leTotalBase->text());
    datosPedido.append(ui->leTotalIva->text());
    datosPedido.append(ui->leTotalRe->text());
    datosPedido.append(ui->leTotal->text());
    if(ui->comboBox->currentText() == "Factura"){
        datosPedido.append(ui->lineEditNDoc->text());
    }else{
        datosPedido.append("0");
    }

    if (!base.contabilizarPedido(datosPedido)) {
        return false;
    }
    qDebug() << "Pedido grabado";
    QStringList datosFactura;
    datosFactura.clear();
    datosFactura.append(ui->lineEditNDoc->text());
    datosFactura.append(fecha);
    datosFactura.append(base.idProveedor(ui->labelProveedor->text()));
    datosFactura.append(ui->leTotalBase->text());
    datosFactura.append(ui->leTotalIva->text());
    datosFactura.append(ui->leTotalRe->text());
    datosFactura.append(ui->leTotal->text());

    if (ui->comboBox->currentText() == "Factura") {

        datosFactura.append(ui->dateEditVencimiento->text());
        datosFactura.append("0");
        if (!base.grabarFactura(datosFactura)) {
            return false;
        }
    }else {
        datosFactura.append("0");
        if (base.grabarAlbaran(datosFactura)) {
            return false;
        }
    }
    qDebug() << "Factura grabada";

    return true;
}



void AceptarPedido::on_doubleSpinBox_valueChanged(double arg1)
{
    descuento = ((100-arg1)/100);
    descuentoReal = arg1;
    llenarTabla(idPedido,descuento);
}

void AceptarPedido::on_pushButton_clicked()
{
    correcto = procesarPedido(modeloPedido);
        qDebug() << "Pedido aceptado";
    close();
}

void AceptarPedido::on_lineEditNDoc_textChanged(const QString &arg1)
{
    nDoc=arg1;
}

void AceptarPedido::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Factura") {
        ui->dateEditVencimiento->setEnabled(true);
        ui->dateEditVencimiento->setDate(QDate::currentDate());
    }else{
        ui->dateEditVencimiento->setEnabled(false);
    }
}
