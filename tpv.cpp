#include "tpv.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QItemDelegate>
#include <QString>
//#include <QtConcurrent/QtConcurrent>
#include <QCryptographicHash>
#include "dialogfecha.h"
#include "imprimirfactura.h"
#include "imprimirticket.h"
#include "ui_tpv.h"
#include <QCursor>
#include "buscarcliente.h"
#include "encargosdialog.h"
#include "gestorencargosdialog.h"

Tpv::Tpv(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Tpv)
{
    buscar = nullptr;
    totalizacion = nullptr;
    clien = nullptr;
    visor = nullptr;
    idVale = 0;
    idEncargoPendiente = 0;
    cliente = 0;
    descuentoCliente = 0;
    vale = 0;
    ui->setupUi(this);
    setAttribute(Qt::WA_DeleteOnClose);
    ui->labelFoto->setCursor(Qt::PointingHandCursor);
    connect(ui->labelFoto, SIGNAL(clicked()), this, SLOT(mostrarFoto()));
    ticketActual = ticketActualizado();
    ticket = base.obtenerNumeroUltimoTicket(QSqlDatabase::database(conf->getConexionLocal())) + 1;
    llenar_usuarios(QSqlDatabase::database(conf->getConexionLocal()));

    modeloTicket = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    cambiarTicket(ticketActual);

    modeloTicketPendiente = new QStandardItemModel(this);
    recuperarTicketsPendientes();
    ui->tableViewTicketsPendientes->hideColumn(0);
    ui->lineEdit_cod_cliente->setText("1");
    on_lineEdit_cod_cliente_editingFinished();
}

Tpv::~Tpv()
{
    delete ui;
    //db.removeDatabase("DB");
}

void Tpv::on_btn_cerrar_clicked()
{
    this->close();
}

void Tpv::closeEvent(QCloseEvent *event)
{
    emit cerrar_tpv();
    event->accept();
}

bool Tpv::llenar_usuarios(QSqlDatabase db)
{
    //Llenar el combo con los usuarios disponibles

    consulta = base.usuarios(db);
    if (consulta.isActive()) {
        while (consulta.next()) {
            //uno la columna 1 y 2 separada por "-" para luego poder obtener
            ui->comboBox_vendedor->addItem(consulta.value(0).toString() + "-"
                                           + consulta.value(1).toString());
            ui->comboBox_vendedor->update();
        }
        consulta.first();
        usuario = consulta.value(0).toInt();
        return true;
    }

    return false;
}

double Tpv::calcularPrecioTotal()
{
    double total = 0.0;
    if (modeloTicket->rowCount() == 0) {
        ui->lcdNumber_total->display(total);
    }
    for (int i = 0; i < modeloTicket->rowCount(); ++i) {
        total += modeloTicket->record(i).value(8).toDouble();
    }
    ui->lcdNumber_total->display(total);
    return total;
}

double Tpv::redondear(double num, int dec)
{
    if (num == 0)
        return 0;
    double z;
    int a, c, b, cont = 1;
    c = dec;

    for (int i = 0; i <= dec; i++)
        cont = cont * 10;

    if (c != num) {
        a = num * cont;
        z = a / 10;
        b = a % 10;
        cont = cont / 10;
        if (b >= 5) {
            z = z + 1;
        }
        num = z / cont;
    }

    return num;
}

void Tpv::calcularNumProductos()
{
    int total = 0;
    for (int i = 0; i < modeloTicket->rowCount(); ++i) {
        QModelIndex indice = modeloTicket->index(i, 4);
        total += modeloTicket->data(indice).toInt();
    }
    ui->lcdProductos->display(total);
}

bool Tpv::actualizarLineaTicket(QList<QString> lista)
{
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("INSERT INTO lineasticket_tmp VALUES (NULL,?,?,?,?,?,?,?,?)");
    //query.prepare("INSERT INTO lineasticket_tmp (orden, cod, descripcion, cantidad, precio, descuento, totallinea) VALUES (?,?,?,?,?,?,?)");
    query.bindValue(0, ticketActual);
    query.bindValue(1, lista.at(0));
    query.bindValue(2, lista.at(1));
    query.bindValue(3, lista.at(2).toInt());
    query.bindValue(4, lista.at(3).toInt());
    query.bindValue(5, lista.at(4).toDouble());
    query.bindValue(6, lista.at(5).toDouble());
    query.bindValue(7, lista.at(6).toDouble());
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return false;
    }

    return true;
}

void Tpv::recuperarTicketsPendientes()
{
    modeloTicketPendiente->clear();
    //2025-10-11
    QSqlQuery listaTicketsPendientes = base.tcketsPendientes(
        QSqlDatabase::database(conf->getConexionLocal()));
    int row = 0;
    while (listaTicketsPendientes.next()) {
        QStandardItem *itemOrden = new QStandardItem(listaTicketsPendientes.value(0).toString());
        modeloTicketPendiente->setItem(row, 0, itemOrden);
        QStandardItem *itemCliente = new QStandardItem(
            base.nombreCliente(listaTicketsPendientes.value(1).toString()));
        modeloTicketPendiente->setItem(row, 1, itemCliente);
        QStandardItem *itemVendedor = new QStandardItem(
            base.nombreUsusario(listaTicketsPendientes.value(2).toString(),
                                conf->getConexionLocal()));
        modeloTicketPendiente->setItem(row, 2, itemVendedor);
        QStandardItem *itemCodCliente = new QStandardItem(
            listaTicketsPendientes.value(1).toString());
        modeloTicketPendiente->setItem(row, 3, itemCodCliente);
        QStandardItem *itemCodVendedor = new QStandardItem(
            listaTicketsPendientes.value(2).toString());
        modeloTicketPendiente->setItem(row, 4, itemCodVendedor);
        row++;
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
    modeloTicket->setFilter("orden = " + QString::number(ticketActual));
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
    base.nuevoTicketTmp(ticketActual, cliente, usuario);
    recuperarTicketsPendientes();
    actualizarParrillaVentas();
}

int Tpv::ticketActualizado()
{
    ticketActual = base.maxTicketPendiente(QSqlDatabase::database(conf->getConexionLocal()));
    return ticketActual;
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
    double TotalBase = 0.0;
    double TotalIva = 0.0;

    for (int i = 0; i < modeloTicket->rowCount(); ++i) {
        double base = modeloTicket->record(i).value(8).toDouble()
                      / (1 + (modeloTicket->record(i).value(5).toDouble() / 100));
        double iva = modeloTicket->record(i).value(8).toDouble() - base;
        TotalBase += base;
        TotalIva += iva;
    }
    basesIvas.append(QString::number(TotalBase));
    basesIvas.append(QString::number(TotalIva));

    return basesIvas;
}

QString Tpv::formatearCadena(QString cadena, int tamano)
{
    if (cadena.length() >= tamano) {
        cadena.resize(tamano);
        return cadena;
    }
    int diferencia = tamano - cadena.length();
    for (int i = 0; i < diferencia; ++i) {
        cadena += " ";
    }
    return cadena;
}

void Tpv::datosProducto(QString IdProducto)
{
    ui->labelStock->setText(base.sumarStockArticulo(IdProducto, conf->getConexionLocal()));
    QSqlQuery tmp = base.ejecutarSentencia("SELECT fecha FROM lotes WHERE ean = " + IdProducto
                                               + " ORDER BY fecha asc",
                                           conf->getConexionLocal());
    tmp.first();
    ui->labelFecha->setText(tmp.value(0).toString());
    consulta = base.consulta_producto(conf->getConexionLocal(), IdProducto);
    consulta.first();
    currentFotoPath = consulta.value("foto").toString();
    QString fichero = QDir::currentPath() + "/" + currentFotoPath;
    QImage foto(fichero);
    QPixmap imagen = QPixmap::fromImage(foto);
    ui->labelFoto->setPixmap(imagen.scaled(200, 200));

    ui->textInfo->setText(consulta.value("notas").toString());
}

QString Tpv::generarDatosFactura(const QStringList datos, const QString ultimoHash)
{
    QString cadena = datos.join("|") + "|" + ultimoHash;

    return cadena;
}

QString Tpv::generarHashFactura(const QString &datosFactura)
{
    QByteArray hash = QCryptographicHash::hash(datosFactura.toUtf8(), QCryptographicHash::Sha256);
    return QString(hash.toHex());
}

bool Tpv::grabarLineasTicket(const QString serie)
{
    QStringList lineaTicket;
    for (int i = 0; i < modeloTicket->rowCount(); i++) {
        lineaTicket.clear();
        lineaTicket.append(serie);
        for (int x = 2; x < modeloTicket->columnCount(); ++x) {
            lineaTicket.append(modeloTicket->record(i).value(x).toString());
        }
        lineaTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
        lineaTicket.append(QTime::currentTime().toString("hh:mm"));
        QString dato;
        dato = lineaTicket.at(3);
        dato = formatearCadena(dato, 3);
        qDebug() << dato;
        dato = lineaTicket.at(2);
        dato = formatearCadena(dato, 20);
        qDebug() << dato;
        dato = lineaTicket.at(5);
        dato = formatearCadena(dato, 6);
        dato.clear();
        dato = lineaTicket.at(6);
        dato = formatearCadena(dato, 3);
        dato.clear();
        dato = lineaTicket.at(7);
        dato = formatearCadena(" " + dato, 6);
        dato.clear();
        qDebug() << lineaTicket;
        if (!base.grabarLineaTicket(lineaTicket)) {
            throw std::runtime_error("Error grabando llinea de ticktes:");
            return false;
        }
        if (lineaTicket.at(3).toInt() < 0) {
            DialogFecha *fechaCaducidad = new DialogFecha(lineaTicket.at(2));
            fechaCaducidad->exec();
            QString fecha = fechaCaducidad->fecha.toString("yyyy-MM-dd");
            QString lote = fechaCaducidad->lote;
            QString idLote = base.idLote(conf->getConexionLocal(), lineaTicket.at(1), lote, fecha);
            qDebug() << lote;
            if (idLote != "0") {
                base.aumentarLote(conf->getConexionLocal(), idLote, abs(lineaTicket.at(3).toInt()));
                qDebug() << "Error al devolver el producto lote";
            } else {
                base.crearLote(conf->getConexionLocal(),
                               lineaTicket.at(1),
                               lote,
                               fecha,
                               QString::number(abs(lineaTicket.at(3).toInt())));
                qDebug() << "Crear lote";
            }
        } else {
            base.descontarArticulo(conf->getConexionLocal(),
                                   lineaTicket.at(1),
                                   lineaTicket.at(3).toInt());
        }
        base.actualizarFechaVentaArticulo(conf->getConexionLocal(),
                                          lineaTicket.at(1),
                                          QDate::currentDate().toString("yyyy-MM-dd"));
    }
    return true;
}

void Tpv::mostrarFoto()
{
    if (!currentFotoPath.isEmpty()) {
        visor = new VisorImagenes(currentFotoPath, this);
        visor->setAttribute(Qt::WA_DeleteOnClose);
        visor->showMaximized();
    }
}

void Tpv::on_lineEdit_cod_returnPressed()
{
    consulta = base.consulta_producto(conf->getConexionLocal(), ui->lineEdit_cod->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base.codigoDesdeAux(conf->getConexionLocal(), ui->lineEdit_cod->text());
        consulta = base.consulta_producto(conf->getConexionLocal(), cod);
        consulta.first();
    }
    if (consulta.numRowsAffected() == 1) {
        QList<QString> linea;
        linea << consulta.value(0).toString();
        datosProducto(consulta.value(0).toString());
        linea << consulta.value(1).toString();
        linea << ui->lineEdit_Uds->text();
        linea << consulta.value(3).toString();
        if (ui->lineEdit_precio->text().isEmpty()) {
            linea << QString::number(consulta.value(2).toDouble());
        } else {
            linea << ui->lineEdit_precio->text();
        }

        linea << ui->lineEdit_descuento->text();
        double totalLinea = linea.at(4).toDouble() * linea.at(2).toDouble()
                            * (1 - linea.at(5).toDouble() / 100);
        totalLinea = classFormatear.redondear(totalLinea, 2);
        linea << QString::number(totalLinea);

        if (ui->tableView->rowAt(0) < 0) {
            ticketNuevo(base.maxTicketPendiente(QSqlDatabase::database(conf->getConexionLocal()))
                        + 1);
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

bool Tpv::cargarEncargo(QString codArticulo, double anticipo, int cantidad)
{
    // 1. Buscamos el producto para obtener sus datos (descripción, precio, etc)
    consulta = base.consulta_producto(conf->getConexionLocal(), codArticulo);
    if (!consulta.first()) {
        return false;
    }

    // 2. Si no hay ticket abierto, creamos uno
    if (modeloTicket->rowCount() == 0 && ui->tableView->rowAt(0) < 0) {
        ticketNuevo(base.maxTicketPendiente(QSqlDatabase::database(conf->getConexionLocal())) + 1);
    }

    // 3. Añadimos el artículo principal
    QList<QString> lineaArt;
    lineaArt << codArticulo;
    lineaArt << consulta.value("descripcion").toString();
    lineaArt << QString::number(cantidad);
    lineaArt << consulta.value("iva").toString();
    lineaArt << QString::number(consulta.value("pvp").toDouble(), 'f', 2);
    lineaArt << "0"; // Descuento
    double totalLinea = consulta.value("pvp").toDouble() * cantidad;
    lineaArt << QString::number(totalLinea, 'f', 2);

    if (!actualizarLineaTicket(lineaArt)) return false;

    // 4. Si hay anticipo, añadimos una línea negativa para descontarlo
    if (anticipo > 0) {
        QList<QString> lineaAnticipo;
        lineaAnticipo << "0"; // Código genérico o 0 para anticipos
        lineaAnticipo << "ANTICIPO ENCARGO: " + codArticulo;
        lineaAnticipo << "1"; // 1 unidad
        lineaAnticipo << "0"; // Sin IVA (ya se cobró en el anticipo original o se ajusta aquí)
        lineaAnticipo << QString::number(-anticipo, 'f', 2); // Precio negativo
        lineaAnticipo << "0"; // Descuento
        lineaAnticipo << QString::number(-anticipo, 'f', 2); // Total negativo

        if (!actualizarLineaTicket(lineaAnticipo)) return false;
    }

    actualizarParrillaVentas();
    ui->lineEdit_cod->setFocus();
    
    // Guardamos el ID del encargo para marcarlo como entregado cuando se cobre el ticket
    // Nota: Deberíamos extraer el ID si viene de cargarEncargo. 
    // He añadido una sobrecarga o modificado la firma para recibir el ID.
    return true;
}

bool Tpv::cargarEncargoConId(QString codArticulo, double anticipo, int cantidad, int idEncargo)
{
    if (cargarEncargo(codArticulo, anticipo, cantidad)) {
        this->idEncargoPendiente = idEncargo;
        return true;
    }
    return false;
}

void Tpv::keyPressEvent(QKeyEvent *e)
{
    switch (e->key()) {
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
        if (ui->comboBox_vendedor->count() >= 1)
            ui->comboBox_vendedor->setCurrentIndex(0);
        break;

    case Qt::Key_F2:
        if (ui->comboBox_vendedor->count() >= 2)
            ui->comboBox_vendedor->setCurrentIndex(1);
        break;

    case Qt::Key_F3:
        if (ui->comboBox_vendedor->count() >= 3)
            ui->comboBox_vendedor->setCurrentIndex(2);
        break;

    case Qt::Key_F4:
        if (ui->comboBox_vendedor->count() >= 4)
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
    consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                   "articulos",
                                   ui->lineEdit_desc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    buscar->exec();
    ui->lineEdit_cod->setText(buscar->resultado);
    emit on_lineEdit_cod_returnPressed();
}

void Tpv::on_btn_cobrar_clicked()
{
    tabla = "tickets";

    QStringList confTicket = base.recuperarConfigTicket(conf->getConexionLocal());
    QFile cajon(confTicket.at(3));
    qDebug() << confTicket.at(3);
    cajon.open(QIODevice::WriteOnly);
    QTextStream codigoApertura(&cajon);
    QString codApertura = confTicket.at(4);
    qDebug() << codApertura;
    QStringList cadaCodApertura = codApertura.split(",");
    for (int i = 0; i < cadaCodApertura.size(); ++i) {
        codigoApertura << char(cadaCodApertura.at(i).toInt());
    }
    cajon.close();
    qDebug() << "Finalizado apertura cajon";

    if (modeloTicket->rowCount() == 0) {
        QMessageBox::information(this, "Ticket vacio", "No hay productos en el ticket");
        return;
    }

    totalizacion = new totalizar(QString::number(calcularPrecioTotal()), vale, this);

    QStringList lineaTicket, totalTicket;
    totalTicket.clear();

    if (totalizacion->exec() != totalizacion->Accepted) {
        delete totalizacion;
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    if (!db.transaction()) {
        QMessageBox::critical(this,
                              "Error",
                              "No se pudo iniciar la transacción de venta. "
                                  + db.lastError().text());
        return;
    }

    QString serie = QString::number(ticket);

    if (totalizacion->facturacion == "1"
        && base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                                  "ticketss",
                                  "ticket",
                                  QString::number(ticket))
               == false) {
        serie = "B" + QString::number(ticket);
        tabla = "ticketss";
    }
    QString ticketImpresion = QString::number(ticket);

    try {
        if (!grabarLineasTicket(serie)) {
            throw std::runtime_error("Error al grabar las lineas del ticket");
        }
        if (totalizacion->valeUsado) {
            usarVale(ticket, idVale, vale);
        }
        totalTicket.append(recopilarDatosTicket());
        totalTicket.append(QString::number(totalizacion->descuento));
        totalTicket.append(QString::number(totalizacion->total));
        totalTicket.append(base.idFormaPago(totalizacion->efectivo, conf->getConexionLocal()));
        totalTicket.append(totalizacion->facturacion);
        totalTicket.append(QString::number(totalizacion->entrega));
        totalTicket.append(QString::number(totalizacion->cambio));

        if (totalizacion->facturacion == "0") {
            tabla = "tickets";
            ticketImpresion = QString::number(ticket);
            ticket += 1;
        }

        if (!base.grabarTicket(conf->getConexionLocal(), tabla, totalTicket)) {
            throw std::runtime_error("Error al grabar el ticket");
        }
        if (tabla == "tickets") {
            QString ultimoHash = base.obtenerUltimoHash(conf->getConexionLocal());
            QString datosFactura = generarDatosFactura(totalTicket, ultimoHash);
            QString hashFactura = generarHashFactura(datosFactura);
            if (base.registrarTickeckVerifactu(conf->getConexionLocal(),
                                               totalTicket.at(0).toInt(),
                                               hashFactura,
                                               ultimoHash,
                                               datosFactura,
                                               totalTicket.at(1).toInt())
                == "") {
                throw std::runtime_error("Error al grabar VeriFactuLog");
            }
        }

        if (!db.commit()) {
            throw std::runtime_error("Error al confirmar la transacción: "
                                     + db.lastError().text().toStdString());
        }

        // Si veníamos de cobrar un encargo, ahora que el ticket se ha grabado, lo marcamos como entregado
        if (idEncargoPendiente > 0) {
            QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
            q.prepare("UPDATE encargos SET estado = 'Entregado' WHERE id_encargo = ?");
            q.bindValue(0, idEncargoPendiente);
            if (q.exec()) {
                qDebug() << "Encargo ID" << idEncargoPendiente << "marcado como ENTREGADO tras cobro de ticket.";
            }
            idEncargoPendiente = 0; // Reset para el siguiente ticket
        }

    } catch (const std::exception &e) {
        db.rollback();
        QMessageBox::critical(this, "ERROR", QString::fromStdString(e.what()));
        delete totalizacion;
        return;
    }
    // for (int i = 0; i < modeloTicket->rowCount(); i++) {
    //     lineaTicket.clear();
    //     lineaTicket.append(serie);
    //     for (int x = 2; x < modeloTicket->columnCount(); ++x) {
    //         lineaTicket.append(modeloTicket->record(i).value(x).toString());
    //     }
    //     lineaTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
    //     lineaTicket.append(QTime::currentTime().toString("hh:mm"));
    //     QString dato;
    //     dato = lineaTicket.at(3);
    //     dato = formatearCadena(dato, 3);
    //     qDebug() << dato;
    //     dato = lineaTicket.at(2);
    //     dato = formatearCadena(dato, 20);
    //     qDebug() << dato;
    //     dato = lineaTicket.at(5);
    //     dato = formatearCadena(dato, 6);
    //     dato.clear();
    //     dato = lineaTicket.at(6);
    //     dato = formatearCadena(dato, 3);
    //     dato.clear();
    //     dato = lineaTicket.at(7);
    //     dato = formatearCadena(" " + dato, 6);
    //     dato.clear();
    //     qDebug() << lineaTicket;
    //     base.grabarLineaTicket(lineaTicket);
    //     if (lineaTicket.at(3).toInt() < 0) {
    //         DialogFecha *fechaCaducidad = new DialogFecha(lineaTicket.at(2));
    //         fechaCaducidad->exec();
    //         QString fecha = fechaCaducidad->fecha.toString("yyyy-MM-dd");
    //         QString lote = fechaCaducidad->lote;
    //         QString idLote = base.idLote(conf->getConexionLocal(),
    //                                      lineaTicket.at(1),
    //                                      lote,
    //                                      fecha);
    //         qDebug() << lote;
    //         if (idLote != "0") {
    //             base.aumentarLote(conf->getConexionLocal(),
    //                               idLote,
    //                               abs(lineaTicket.at(3).toInt()));
    //             qDebug() << "Error al devolver el producto lote";
    //         } else {
    //             base.crearLote(conf->getConexionLocal(),
    //                            lineaTicket.at(1),
    //                            lote,
    //                            fecha,
    //                            QString::number(abs(lineaTicket.at(3).toInt())));
    //             qDebug() << "Crear lote";
    //         }
    //     } else {
    //         base.descontarArticulo(lineaTicket.at(1), lineaTicket.at(3).toInt());
    //     }
    //     base.actualizarFechaVentaArticulo(lineaTicket.at(1),
    //                                       QDate::currentDate().toString("yyyy-MM-dd"));
    // }

    emit on_pushButtonBorrarTodo_clicked();
    //        impresora.close();
    if (totalizacion->ticket == true && totalizacion->factura == false) {
        //            system("less ./ticket.txt >> /dev/lp0");
        ImprimirTicket(ticketImpresion, "ticket", totalizacion->noTicketRegalo);
    } else if (totalizacion->factura == true) {
        ImprimirFactura(ticketImpresion, this);
    }
}

void Tpv::on_tableViewTicketsPendientes_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTicketPendiente->index(index.row(), 0);
    QString dato = modeloTicketPendiente->data(indice, Qt::EditRole).toString();
    cambiarTicket(dato.toInt());

    indice = modeloTicketPendiente->index(index.row(), 3);
    ui->lineEdit_cod_cliente->setText(modeloTicketPendiente->data(indice, Qt::EditRole).toString());
    emit on_lineEdit_cod_cliente_editingFinished();

    indice = modeloTicketPendiente->index(indice.row(), 4);
    ui->comboBox_vendedor->setCurrentIndex(modeloTicketPendiente->data(indice, Qt::EditRole).toInt()
                                           - 1);
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
    ticketActual = base.maxTicketPendiente(QSqlDatabase::database(conf->getConexionLocal())) + 1;
    actualizarParrillaVentas();
    ui->lineEdit_cod_cliente->setText("1");
    emit on_lineEdit_cod_cliente_editingFinished();
    ui->lineEdit_cod->clear();
    ui->lineEdit_cod->setFocus();
}

void Tpv::on_lineEdit_cod_cliente_editingFinished() {
    QString nombreCliente;
    nombreCliente = base.nombreCliente(ui->lineEdit_cod_cliente->text());
    if (nombreCliente == "Sin asignar") {
        QMessageBox *msgbox = new QMessageBox(this);
        msgbox->setText("Crear cliente");
        msgbox->setInformativeText("Ese cliente no existe. \n¿Desea Crearlo?");
        msgbox->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msgbox->setDefaultButton(QMessageBox::Cancel);
        if (msgbox->exec() == QMessageBox::Ok) {
            clien = new Clientes(this, ui->lineEdit_cod_cliente->text());
            clien->exec();
            ui->lineEdit_nobre_cliente->setText(
                base.nombreCliente(ui->lineEdit_cod_cliente->text()));

        } else {
            ui->lineEdit_cod_cliente->setText("1");
            ui->lineEdit_nobre_cliente->setText(
                base.nombreCliente(ui->lineEdit_cod_cliente->text()));
        }
    } else {
        ui->lineEdit_nobre_cliente->setText(nombreCliente);
    }
    descuentoCliente = base.descuentoCliente(ui->lineEdit_cod_cliente->text());
    vale = base.valeCliente(conf->getConexionLocal(), ui->lineEdit_cod_cliente->text());
    qDebug() << vale;
    if (vale > 0) {
        ui->labelVale->setNum(vale);
        idVale = base.idVale(conf->getConexionLocal(), ui->lineEdit_cod_cliente->text());
    } else {
        ui->labelVale->setNum(0);
        idVale = 0;
    }

    ui->lineEdit_cod->setFocus();
}

void Tpv::on_lineEdit_nobre_cliente_returnPressed()
{
    consulta = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                                  "clientes",
                                  "nombre",
                                  ui->lineEdit_nobre_cliente->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarCliente *buscar = new BuscarCliente(this, consulta);
    buscar->exec();
    qDebug() << buscar->resultado;
    ui->lineEdit_cod_cliente->setText(buscar->resultado);
    emit on_lineEdit_cod_cliente_editingFinished();
    delete buscar;
}

void Tpv::on_lineEdit_precio_returnPressed() {}

void Tpv::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTicket->index(index.row(), 0);
    int idModeloTicket = indice.row();
    ui->lineEdit_cod->setText(modeloTicket->data(modeloTicket->index(idModeloTicket, 2)).toString());
    ui->lineEdit_desc->setText(
        modeloTicket->data(modeloTicket->index(idModeloTicket, 3)).toString());
    ui->lineEdit_Uds->setText(modeloTicket->data(modeloTicket->index(idModeloTicket, 4)).toString());
    ui->lineEdit_precio->setText(
        modeloTicket->data(modeloTicket->index(idModeloTicket, 6)).toString());
    ui->lineEdit_descuento->setText(
        modeloTicket->data(modeloTicket->index(idModeloTicket, 7)).toString());
    ui->lineEdit_6->setText(modeloTicket->data(modeloTicket->index(idModeloTicket, 8)).toString());
    modeloTicket->removeRow(ui->tableView->currentIndex().row());
}

void Tpv::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTicket->index(index.row(), 0);
    int idModeloTicket = indice.row();
    datosProducto(modeloTicket->data(modeloTicket->index(idModeloTicket, 2)).toString());
}

void Tpv::on_btn_preTicket_clicked()
{
    QStringList lineaTicket;

    QStringList confTicket = base.recuperarConfigTicket(conf->getConexionLocal());

    QFile impresora("ticket.txt");
    impresora.open(QIODevice::WriteOnly);
    QTextStream texto(&impresora);
    QString abrirCajon = confTicket.at(4);
    QStringList comandoAbrirCajon = abrirCajon.split(",");
    for (int i = 0; i < comandoAbrirCajon.size(); ++i) {
        texto << char(comandoAbrirCajon.at(i).toInt());
    }
    texto << confTicket.at(0) + "\n\n";
    texto << QDate::currentDate().toString("yyyy-MM-dd") + "  "
                 + QTime::currentTime().toString("hh:mm") + "   " + "Ticket: TICKET";
    texto << "\n";
    texto << "UDS|  Producto            |Prec.|Dto|Total\n";
    texto << "------------------------------------------\n";

    for (int i = 0; i < modeloTicket->rowCount(); i++) {
        lineaTicket.clear();
        lineaTicket.append("TICKET");
        for (int x = 2; x < modeloTicket->columnCount(); ++x) {
            lineaTicket.append(modeloTicket->record(i).value(x).toString());
        }
        QString dato;
        dato = lineaTicket.at(3);
        dato = formatearCadena(dato, 3);
        qDebug() << dato;
        texto << dato;
        texto << " ";
        dato = lineaTicket.at(2);
        dato = formatearCadena(dato, 23);
        qDebug() << dato;
        texto << dato;
        texto << " ";
        dato = lineaTicket.at(5);
        dato = formatearCadena(dato, 6);
        texto << dato;
        texto << " ";
        dato.clear();
        dato = lineaTicket.at(6);
        dato = formatearCadena(dato, 2);
        texto << dato;
        texto << " ";
        dato.clear();
        dato = lineaTicket.at(7);
        dato = formatearCadena(dato, 6);
        texto << dato;
        texto << "\n";
        dato.clear();
        qDebug() << lineaTicket;
    }
    texto << "\n\n";
    texto << "Total : " + QString::number(calcularPrecioTotal()) + "\n";

    texto << "\n\n\n";
    texto << confTicket.at(1);
    texto << "\n\n\n\n";
    QString codCorte = confTicket.at(5);
    QStringList cadaCodCorte = codCorte.split(",");
    for (int i = 0; i < cadaCodCorte.size(); ++i) {
        texto << char(cadaCodCorte.at(i).toInt());
    }
    texto << "\n\n";
    impresora.close();
    QString imprimir = "cat ./ticket.txt >> " + confTicket.at(3);
    const char *ch = imprimir.toLocal8Bit().constData();
    system(ch);
}

#include "encargosdialog.h"

void Tpv::on_btn_encargo_clicked()
{
    QString codCliente = ui->lineEdit_cod_cliente->text();
    QString nombreCliente = ui->lineEdit_nobre_cliente->text();
    QString codArticulo = ui->lineEdit_cod->text();
    QString descArticulo = ui->lineEdit_desc->text();
    QString usuarioSistema = conf->getUsuario();
    if (usuarioSistema.isEmpty()) usuarioSistema = ui->comboBox_vendedor->currentText();

    EncargosDialog dialog(codCliente, nombreCliente, codArticulo, descArticulo, usuarioSistema, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString finalCodArticulo = dialog.getCodArticulo();
        QString finalCodCliente = dialog.getCodCliente();
        int cantidad = dialog.getCantidad();
        double anticipo = dialog.getAnticipo();
        QString notas = dialog.getNotas();

        if (finalCodArticulo.isEmpty() || finalCodCliente.isEmpty()) {
            QMessageBox::warning(this, "Aviso", "No se puede crear el encargo, faltan datos (artículo o cliente vacío).");
            return;
        }

        // Insertar en la tabla encargos
        QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
        query.prepare("INSERT INTO encargos (id_cliente, cod_articulo, cantidad, notas, empleado, anticipo, estado) "
                      "VALUES (?, ?, ?, ?, ?, ?, 'Pendiente')");
        query.bindValue(0, finalCodCliente.toInt());
        query.bindValue(1, finalCodArticulo);
        query.bindValue(2, cantidad);
        query.bindValue(3, notas);
        query.bindValue(4, usuarioSistema);
        query.bindValue(5, anticipo);
        
        if (!query.exec()) {
            QMessageBox::critical(this, "Error", "No se pudo guardar el encargo:\n" + query.lastError().text());
        } else {
            baseDatos base;
            base.crearNota(conf->getConexionLocal(),
                           "Nuevo Encargo: " + finalCodArticulo,
                           "Cliente ID: " + finalCodCliente + "\nCantidad: " + QString::number(cantidad) + "\nNotas: " + notas,
                           usuarioSistema,
                           "",
                           "Alta");
            
            QMessageBox::information(this, "Éxito", "Encargo creado y guardado.");
        }
        
        // Si hay un anticipo, registrarlo en movimientos (entradasSalidas)
        if (anticipo > 0) {
            QStringList datosES;
            datosES.append(QDate::currentDate().toString("yyyy-MM-dd"));
            datosES.append(QTime::currentTime().toString("hh:mm:ss"));
            datosES.append(QString::number(anticipo));
            // Suponemos ID 1 como ingreso por defecto, se puede crear un motivo específico
            datosES.append("1"); 
            datosES.append(QString("Anticipo Encargo (Cliente %1) - %2").arg(codCliente).arg(codArticulo));
            
            if (!base.insertarES(datosES, conf->getConexionLocal(), conf->getUsuario())) {
                QMessageBox::warning(this, "Aviso", "El encargo se guardó, pero no se pudo registrar el anticipo en la caja fuerte.");
            } else {
                QMessageBox::information(this, "Éxito", "Encargo creado y anticipo registrado en caja correctamente.");
            }
        } else {
            QMessageBox::information(this, "Éxito", "Encargo creado correctamente (sin anticipo).");
        }
        
        ui->lineEdit_cod->clear();
        ui->lineEdit_desc->clear();
        ui->lineEdit_cod->setFocus();
    }
}

void Tpv::usarVale(int ticket, int idVale, double cantVale)
{
    QStringList lineaTicket;
    lineaTicket.clear();
    lineaTicket.append(QString::number(ticket));
    lineaTicket.append("9999999999999");
    lineaTicket.append("Vale fidelidad");
    lineaTicket.append("1");
    lineaTicket.append("0");
    lineaTicket.append(QString::number(cantVale));
    lineaTicket.append("0");
    lineaTicket.append("-" + QString::number(cantVale));
    lineaTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
    lineaTicket.append(QTime::currentTime().toString("hh:mm"));
    base.grabarLineaTicket(lineaTicket);

    // 2025-10-12

    QString conexionLocal = conf->getConexionLocal();
    listaConexionesRemotas = conf->getNombreConexiones();
    qDebug() << "🧩 Sincronizando vale usado con tiendas remotas en hilo separado...";
    qDebug() << "Tienda local:" << conexionLocal;
    qDebug() << "Tiendas remotas:" << listaConexionesRemotas;

    //Marcar local
    if (base.usarVale(conexionLocal, idVale)) {
        qDebug() << "Vale marcado correctamente en tienda local";
    } else {
        qWarning() << "Error al marcar en tienda local";
    }

    //Intentar en tiendas remotas
    for (const QString &tienda : listaConexionesRemotas) {
        QSqlDatabase db = QSqlDatabase::database(tienda);
        if (!db.isOpen()) {
            qWarning() << "Conexion cerrada con tienda " << tienda;
            base.valesPendientesMarcar(conexionLocal, tienda, idVale);
            continue;
        }
        if (!base.usarVale(tienda, idVale)) {
            qWarning() << "Error al marcar el vale en la tienda remota " << tienda;
            base.valesPendientesMarcar(conexionLocal, tienda, idVale);
        }
    }
}

// listaConexionesRemotas = conf->getNombreConexiones();
// qDebug() << listaConexionesRemotas;
// conexionLocal = conf->getConexionLocal();
// qDebug() << conexionLocal;
// if (base.usarVale(conexionLocal, idVale)) {
//     qDebug() << "Vale usado";
// } else {
//     qDebug() << "Error al usar el vale";
// }
// for (int i = 0; i < listaConexionesRemotas.length(); i++) {
//     if (!base.usarVale(listaConexionesRemotas.at(i), idVale)) {
//         qDebug() << "Error al marcar vale en tienda " << listaConexionesRemotas.at(i);
//         base.valesPendientesMarcar(conexionLocal, listaConexionesRemotas.at(i), idVale);
//     } else {
//         qDebug() << "Vale marcado en tienda " << listaConexionesRemotas.at(i);
//     }
// }
//}
//ClickableLabel::ClickableLabel(QWidget *parent, Qt::WindowFlags f) : QLabel(parent)
//{

//}

//ClickableLabel::~ClickableLabel()
//{

//}

//void ClickableLabel::mousePressEvent(QMouseEvent *event)
//{
//    emit clicked();
//}
void Tpv::on_btnGestorEncargos_clicked()
{
    GestorEncargosDialog dial("", this);
    dial.exec();
}
