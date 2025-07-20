#include "aceptarpedido.h"
#include <QInputDialog>
#include <QMessageBox>
#include "ui_aceptarpedido.h"

AceptarPedido::AceptarPedido(
    QString pedido, QString proveedor, QString nDoc, QString fechaPedido, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AceptarPedido)
{
    ui->setupUi(this);
    correcto = false;
    idPedido = pedido;
    ui->labelProveedor->setText(proveedor);
    descuento = 1;
    idProveedor = base.idProveedor(proveedor, conf->getConexionLocal());
    descuentoReal = 0;
    fecha = fechaPedido;
    ui->lineEditNDoc->setText(nDoc);
    ui->dateEditDocumento->setDate(QDate::fromString(fecha, "yyyy-MM-dd"));
    llenarTabla(idPedido, descuento);
    modeloPedido = new QSqlQueryModel(this);
    modeloPedido
        ->setQuery(QString("SELECT * FROM lineaspedido_tmp WHERE idPedido = %1").arg(idPedido),
                   QSqlDatabase::database("DB"));
}

AceptarPedido::~AceptarPedido()
{
    delete ui;
}

void AceptarPedido::llenarTabla(QString idPedido, double desc)
{


    double totalBase = 0, totalIva = 0, totalRe = 0, totalGeneral = 0;


        // Consulta para agrupar por tipo de IVA
        QString queryStr = QString(
                               "SELECT tipoIva, "
                               "SUM(totalbase) AS totalBase, "
                               "SUM(iva) AS totalIva, "
                               "SUM(re) AS totalRe, "
                               "(SUM(totalbase) + SUM(iva) + SUM(re)) AS totalGeneral "
                               "FROM lineaspedido_tmp "
                               "WHERE idPedido = '%1' "
                               "GROUP BY tipoIva").arg(idPedido);

        QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
        if (!query.exec(queryStr)) {
            QMessageBox::critical(this, "Error", "Error al consultar los datos agrupados por IVA");
            return;
        }

        // Crear un modelo de tabla para mostrar los datos agrupados
        QStandardItemModel *modelo = new QStandardItemModel(this);

        // Definir encabezados
        modelo->setHorizontalHeaderLabels(QStringList() << "Tipo IVA" << "Base Imponible"
                                                        << "Total IVA" << "Total RE"
                                                        << "Total General");



        // Procesar resultados de la consulta
        while (query.next()) {
            QList<QStandardItem *> fila;

            // Obtener valores
            double tipoIva = query.value("tipoIva").toDouble();
            double base = query.value("totalBase").toDouble();
            double iva = query.value("totalIva").toDouble();
            double re = query.value("totalRe").toDouble();
            double total = query.value("totalGeneral").toDouble();

            // Crear celdas de la fila
            fila << new QStandardItem(QString::number(tipoIva)) // Tipo IVA
                 << new QStandardItem(QString::number(base*desc, 'f', 2)) // Base Imponible
                 << new QStandardItem(QString::number(iva*desc, 'f', 2)) // IVA
                 << new QStandardItem(QString::number(re*desc, 'f', 2)) // Recargo de Equivalencia
                 << new QStandardItem(QString::number(total*desc, 'f', 2)); // Total General

            // Agregar fila al modelo
            modelo->appendRow(fila);

            // Sumar totales generales
            totalBase += base;
            totalIva += iva;
            totalRe += re;
            totalGeneral += total;
        }

        // Establecer el modelo en la vista de tabla
        ui->tableView->setModel(modelo);
        ui->tableView->resizeColumnsToContents();

        //Mostrar los totales generales en los campos correspondientes
        ui->leTotalBase->setText(QString::number(totalBase*desc, 'f', 2));
        ui->leTotalIva->setText(QString::number(totalIva*desc, 'f', 2));
        ui->leTotalRe->setText(QString::number(totalRe*desc, 'f', 2));
        ui->leTotal->setText(QString::number(totalGeneral*desc, 'f', 2));
        //ui->leLineas->setText(QString::number(modeloPedido->rowCount()));

}

bool AceptarPedido::procesarPedido(QSqlQueryModel *modelo)
{
    QStringList datos;
    QString idLinea, ean, descripcion, lote, fechaCaducidad, descuentoLinea, tipoIva, baseProducto,
        baseLinea, ivaLinea, reLinea;
    int uds, unidades, bonificacion, tipoActualizacion = 0; //0-preguntar 1-pedido 2 almacen
    double precioCosto, pvp;
    if (ui->comboBox->currentText() == "Seleccionar") {
        QMessageBox msg;
        msg.setText("Seleccione primero");
        msg.setInformativeText(
            "Antes de poder aceptar el pedido debe indicar si es un albarán o una factura");
        msg.exec();
        return false;
    }
    if (ui->lineEditNDoc->text().isEmpty()) {
        QMessageBox msg;
        msg.setText("Falta el número de documento");
        msg.setInformativeText("Antes de poder aceptar el pedido debe indicar el número del "
                               "documento al que pertenece");
        msg.exec();
        return false;
    }

    for (int i = 0; i < modelo->rowCount(); ++i) {
        datos.clear();
        idLinea = modelo->record(i).value("id").toString();
        ean = modelo->record(i).value("cod").toString();
        unidades = modelo->record(i).value("cantidad").toInt();
        bonificacion = modelo->record(i).value("bonificacion").toInt();
        uds = unidades + bonificacion;
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
        QString idLote = base.idLote(conf->getConexionLocal(), ean, "", "2000-01-01");
        qDebug() << "IDLOTE " + idLote;
        if (idLote != "0") {
            qDebug() << "IDLOTE " + idLote;
            int pendientes = base.unidadesLote(conf->getConexionLocal(), idLote);
            qDebug() << "PENDIENTES: " + QString::number(pendientes);
            if (abs(pendientes) > uds) {
                base.aumentarLote(conf->getConexionLocal(), idLote, uds);
            } else if (abs(pendientes) == uds) {
                base.ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'",
                                       conf->getConexionLocal());
            } else {
                base.ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'",
                                       conf->getConexionLocal());
                uds = uds + pendientes;
                //Modificar o crear lotes
                idLote = base.idLote(conf->getConexionLocal(), ean, lote, fechaCaducidad);
                if (idLote == "0") {
                    base.crearLote(conf->getConexionLocal(),
                                   ean,
                                   lote,
                                   fechaCaducidad,
                                   QString::number(uds));
                    qDebug() << "Creando lote";
                } else {
                    base.aumentarLote(conf->getConexionLocal(), idLote, uds);
                }
            }
        } else {
            //Modificar o crear lotes
            idLote = base.idLote(conf->getConexionLocal(), ean, lote, fechaCaducidad);
            if (idLote == "0") {
                base.crearLote(conf->getConexionLocal(),
                               ean,
                               lote,
                               fechaCaducidad,
                               QString::number(uds));
                qDebug() << "Creando lote";
            } else {
                base.aumentarLote(conf->getConexionLocal(), idLote, uds);
            }
        }

        //Comprobar cambio de nombre o PVP y actualizar artículos
        consulta = base.consulta_producto(conf->getConexionLocal(), ean);
        consulta.first();
        QString descripcionAnterior = consulta.value("descripcion").toString();
        QString precioAnterior = consulta.value("pvp").toString();
        switch (tipoActualizacion) {
        case 2:
            descripcion = descripcionAnterior;
            pvp = precioAnterior.toDouble();
            break;
        case 0:
            if (!(descripcionAnterior == descripcion && precioAnterior == QString::number(pvp))) {
                cdProducto = new CambioDatosProducto(descripcion,
                                                     descripcionAnterior,
                                                     QString::number(pvp),
                                                     precioAnterior,
                                                     this);
                cdProducto->exec();
                descripcion = cdProducto->nombre;
                pvp = cdProducto->pvp.toDouble();
                tipoActualizacion = cdProducto->tipoActualizacion;
                if (precioAnterior != QString::number(pvp)) {
                    base.insertarEtiqueta(conf->getConexionLocal(), ean);
                }
            }

        case 1:
            if (precioAnterior != QString::number(pvp)) {
                base.insertarEtiqueta(conf->getConexionLocal(), ean);
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
        if (base.actualizarArticulosDesdeCompras(datos)) {
            qDebug() << "actualizando " << datos.at(1);
        } else {
            QMessageBox msg;
            msg.setText("ERROR");
            msg.setInformativeText(
                "No se ha podido actualizar la información de la tabla artículos");
            msg.show();
            return false;
        }

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
        if (!base.pasarLineaPedidoAHistorico(conf->getConexionLocal(), datosLineaPedido)) {
            return false;
        } else {
            qDebug() << base.borrarPedido(conf->getConexionLocal(), idPedido);
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
    datosPedido.append(ui->leBase0->text());
    datosPedido.append(ui->leIva0->text());
    datosPedido.append(ui->leRe0->text());
    datosPedido.append(ui->leBase5->text());
    datosPedido.append(ui->leIva5->text());
    datosPedido.append(ui->leRe5->text());
    datosPedido.append(ui->leTotalBase->text());
    datosPedido.append(ui->leTotalIva->text());
    datosPedido.append(ui->leTotalRe->text());
    datosPedido.append(ui->leTotal->text());
    if (ui->comboBox->currentText() == "Factura") {
        datosPedido.append(ui->lineEditNDoc->text());
    } else {
        datosPedido.append("0");
    }

    if (!base.contabilizarPedido(conf->getConexionLocal(), datosPedido)) {
        return false;
    }
    qDebug() << "Pedido grabado";
    QStringList datosFactura;
    datosFactura.clear();
    datosFactura.append(ui->lineEditNDoc->text());
    datosFactura.append(fecha);
    datosFactura.append(base.idProveedor(ui->labelProveedor->text(), conf->getConexionLocal()));
    datosFactura.append(ui->leTotalBase->text());
    datosFactura.append(ui->leTotalIva->text());
    datosFactura.append(ui->leTotalRe->text());
    datosFactura.append(ui->leTotal->text());

    if (ui->comboBox->currentText() == "Factura") {
        datosFactura.append(ui->dateEditVencimiento->text());
        datosFactura.append("0");
        if (!base.grabarFactura(conf->getConexionLocal(),datosFactura)) {
            return false;
        }
    } else {
        datosFactura.append("0");
        if (base.grabarAlbaran(conf->getConexionLocal(),datosFactura)) {
            return false;
        }
    }
    qDebug() << "Factura grabada";

    return true;
}

void AceptarPedido::on_doubleSpinBox_valueChanged(double arg1)
{
    descuento = ((100 - arg1) / 100);
    descuentoReal = arg1;
    llenarTabla(idPedido, descuento);
}

void AceptarPedido::on_pushButton_clicked()
{
    correcto = procesarPedido(modeloPedido);
    qDebug() << "Pedido aceptado";
    close();
}

void AceptarPedido::on_lineEditNDoc_textChanged(const QString &arg1)
{
    nDoc = arg1;
}

void AceptarPedido::on_comboBox_currentTextChanged(const QString &arg1)
{
    if (arg1 == "Factura") {
        ui->dateEditVencimiento->setEnabled(true);
        ui->dateEditVencimiento->setDate(QDate::currentDate());
    } else {
        ui->dateEditVencimiento->setEnabled(false);
    }
}
