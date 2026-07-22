#include "tpv.h"
#include "verifactuclass.h"
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
#include "visorimagenes.h"
#include "buscarcliente.h"
#include "buscarpornotas.h"
#include "encargosdialog.h"
#include "gestorencargosdialog.h"
#include <QTextDocument>
#include <QPrinter>
#include <QProcess>
#include <QMessageBox>
#include <QPushButton>
#include <QSqlRecord>

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
    permisos();
}

void Tpv::permisos() {
    if (!conf) return;
    int rol = conf->getRol();
    if (rol < 0) return;

    // Mapa de permisos granulares para el TPV
    QMap<QString, QWidget*> mapa = {
        {"tpv.anadir", ui->btn_anadir},
        {"tpv.borrar", ui->btn_borrar},
        {"tpv.borrar_todo", ui->pushButtonBorrarTodo},
        {"tpv.cobrar", ui->btn_cobrar},
        {"tpv.preticket", ui->btn_preTicket},
        {"tpv.hacer_encargo", ui->btn_encargo},
        {"tpv.gestor_encargos", ui->btnGestorEncargos}
    };

    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
        if (it.value()) {
            it.value()->setEnabled(conf->permisos()->tiene(it.key()));
        }
    }
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
            QString id = consulta.value(0).toString();
            QString nombre = consulta.value(1).toString();
            // Guardamos el ID como Data para poder buscarlo fácilmente por código
            ui->comboBox_vendedor->addItem(id + "-" + nombre, id);
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
    QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
    // Buscamos la fecha de caducidad más cercana con stock disponible (cantidad > 0)
    // y descartamos la fecha comodín "2000-01-01" de lotes genéricos.
    // Usamos DATE_FORMAT para asegurar el formato de fecha "yyyy-MM-dd".
    tmp.prepare("SELECT DATE_FORMAT(fecha, '%Y-%m-%d') FROM lotes "
                "WHERE ean = ? AND fecha != '2000-01-01' AND cantidad > 0 "
                "ORDER BY fecha ASC");
    tmp.bindValue(0, IdProducto);
    if (tmp.exec() && tmp.first()) {
        ui->labelFecha->setText(tmp.value(0).toString());
    } else {
        ui->labelFecha->setText("");
    }
    QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), IdProducto);
    
    currentFotoPath = registro.value("foto").toString();
    QString fichero = base.resolverRutaImagen(currentFotoPath);
    QImage foto(fichero);
    QPixmap imagen = QPixmap::fromImage(foto);
    ui->labelFoto->setPixmap(imagen.scaled(200, 200));

    ui->textInfo->setText(registro.value("notas").toString());
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
    QSqlRecord registro = base.consulta_producto(conf->getConexionCommon(), ui->lineEdit_cod->text());
    
    if (registro.isEmpty()) {
        QString cod = base.codigoDesdeAux(conf->getConexionCommon(), ui->lineEdit_cod->text());
        registro = base.consulta_producto(conf->getConexionCommon(), cod);
    }
    
    if (!registro.isEmpty()) {
        QList<QString> linea;
        linea << registro.value("cod").toString();
        datosProducto(registro.value("cod").toString());
        linea << registro.value("descripcion").toString();
        linea << ui->lineEdit_Uds->text();
        linea << registro.value("iva").toString();
        
        if (ui->lineEdit_precio->text().isEmpty()) {
            linea << QString::number(registro.value("pvp").toDouble());
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
    QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), codArticulo);
    if (registro.isEmpty()) {
        return false;
    }

    // 2. Si no hay ticket abierto, creamos uno
    if (modeloTicket->rowCount() == 0 && ui->tableView->rowAt(0) < 0) {
        ticketNuevo(base.maxTicketPendiente(QSqlDatabase::database(conf->getConexionLocal())) + 1);
    }

    // 3. Añadimos el artículo principal
    QList<QString> lineaArt;
    lineaArt << codArticulo;
    lineaArt << registro.value("descripcion").toString();
    lineaArt << QString::number(cantidad);
    lineaArt << registro.value("iva").toString();
    lineaArt << QString::number(registro.value("pvp").toDouble(), 'f', 2);
    lineaArt << "0"; // Descuento
    double totalLinea = registro.value("pvp").toDouble() * cantidad;
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
    {
        QMap<QString, QVariant> config = base.leerConfiguracion();
        QString id = config.value("vendedor_f1").toString();
        int index = ui->comboBox_vendedor->findData(id);
        if (index != -1)
            ui->comboBox_vendedor->setCurrentIndex(index);
        break;
    }
    
    case Qt::Key_F2:
    {
        QMap<QString, QVariant> config = base.leerConfiguracion();
        QString id = config.value("vendedor_f2").toString();
        int index = ui->comboBox_vendedor->findData(id);
        if (index != -1)
            ui->comboBox_vendedor->setCurrentIndex(index);
        break;
    }
    
    case Qt::Key_F3:
    {
        QMap<QString, QVariant> config = base.leerConfiguracion();
        QString id = config.value("vendedor_f3").toString();
        int index = ui->comboBox_vendedor->findData(id);
        if (index != -1)
            ui->comboBox_vendedor->setCurrentIndex(index);
        break;
    }
    
    case Qt::Key_F4:
    {
        QMap<QString, QVariant> config = base.leerConfiguracion();
        QString id = config.value("vendedor_f4").toString();
        int index = ui->comboBox_vendedor->findData(id);
        if (index != -1)
            ui->comboBox_vendedor->setCurrentIndex(index);
        break;
    }

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

// Slot para el botón Buscar por Notas en el TPV
void Tpv::on_btn_buscar_notas_clicked()
{
    // Realizar la búsqueda inteligente multitérmino por notas a partir del texto ingresado en lineEdit_desc
    consulta = base.buscarPorNotas(QSqlDatabase::database(conf->getConexionLocal()),
                                   ui->lineEdit_desc->text());
    consulta.first();
    BuscarPorNotas *buscarNotas = new BuscarPorNotas(this, consulta);
    if (buscarNotas->exec() == QDialog::Accepted) {
        // Establecer el código del artículo seleccionado y simular Enter para agregarlo al ticket
        ui->lineEdit_cod->setText(buscarNotas->resultado);
        emit on_lineEdit_cod_returnPressed();
    }
    delete buscarNotas;
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

    // Obtener el número de ticket de forma atómica dentro de la transacción.
    // FOR UPDATE bloquea las tablas tickets/ticketss hasta que se haga commit,
    // evitando que dos puestos obtengan el mismo número.
    ticket = base.obtenerSiguienteTicketAtomico(db);

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
        totalTicket.append(QString::number(totalizacion->formaPago));
        totalTicket.append(totalizacion->facturacion);
        totalTicket.append(QString::number(totalizacion->entrega));
        totalTicket.append(QString::number(totalizacion->cambio));

        if (totalizacion->facturacion == "0") {
            tabla = "tickets";
            ticketImpresion = QString::number(ticket);
            // Ya no se incrementa ticket manualmente: se obtiene atómicamente
            // de la BD al inicio de cada transacción de cobro.
        }

        if (!base.grabarTicket(conf->getConexionLocal(), tabla, totalTicket)) {
            throw std::runtime_error("Error al grabar el ticket");
        }
        // --- Integración VeriFactu ---
        // Con la tecla F2 (venta especial/B), el ticket se graba en la tabla 'ticketss'
        // en lugar de 'tickets'. Para cumplir con los requisitos y evitar errores de
        // restricción de clave foránea en la tabla verifactu_logs (que apunta a 'tickets'),
        // no procesamos VeriFactu para ventas que no vayan a la tabla oficial 'tickets'.
        VeriFactuConfig vfConfig = verifactuClass::cargarConfiguracion();
        if (vfConfig.modo != 0 && tabla == "tickets") { // 1 = VeriFactu (remisión), 2 = No VeriFactu (local)
            // Obtener el último hash de forma atómica (FOR UPDATE) dentro
            // de la transacción para que otro puesto espere si está cobrando
            // simultáneamente, evitando romper la cadena de hashes.
            QString ultimoHash = base.obtenerUltimoHashAtomico(db);
            QString fechaHoraGen = verifactuClass::obtenerFechaHoraHusoActual();
            
            // Serie y número de factura
            QString numSerie = serie;
            
            // Fecha de expedición formateada como DD-MM-YYYY
            QDate dateExp = QDate::fromString(totalTicket.at(3), "yyyy-MM-dd");
            QString fechaExpAEAT = dateExp.isValid() ? dateExp.toString("dd-MM-yyyy") : QDate::currentDate().toString("dd-MM-yyyy");
            
            // Tipo de factura
            QString tipoFacturaAEAT = (tabla == "ticketss") ? "F1" : "F2";
            
            // Formatear bases e importes para el hash de la AEAT
            QString baseStr = verifactuClass::formatearDecimalAEAT(totalTicket.at(5).toDouble());
            QString ivaStr = verifactuClass::formatearDecimalAEAT(totalTicket.at(6).toDouble());
            QString totalStr = verifactuClass::formatearDecimalAEAT(totalTicket.at(8).toDouble());
            
            // Calcular el hash oficial de alta
            QString hashFactura = verifactuClass::calcularHuellaAlta(
                vfConfig.emisorNif,
                numSerie,
                fechaExpAEAT,
                tipoFacturaAEAT,
                ivaStr,
                totalStr,
                ultimoHash,
                fechaHoraGen
            );
            
            // Generar el XML oficial de alta de facturación
            QString xmlContent = verifactuClass::generarXmlAlta(
                vfConfig,
                numSerie,
                fechaExpAEAT,
                totalTicket.at(4), // hora
                totalTicket.at(5).toDouble(), // base
                totalTicket.at(6).toDouble(), // iva
                totalTicket.at(8).toDouble(), // total
                tipoFacturaAEAT,
                ultimoHash,
                hashFactura,
                fechaHoraGen
            );
            
            // Si el modo es VERI*FACTU, remitimos telemáticamente a la AEAT
            int estadoEnvio = 1;
            if (vfConfig.modo == 1) {
                QString errStr;
                bool okEnvio = verifactuClass::enviarAEAT(xmlContent, vfConfig, errStr);
                if (!okEnvio) {
                    estadoEnvio = 0; // 0 = Pendiente / Error
                    qWarning() << "Error en la remisión VeriFactu a la AEAT:" << errStr;
                    // Registramos en local pero guardando el error en el log del sistema
                    base.insertarLog(conf->getConexionLocal(), "VeriFactuError", conf->getUsuario(),
                                     QString("Fallo envío ticket %1: %2").arg(numSerie).arg(errStr));
                } else {
                    qDebug() << "Ticket" << numSerie << "remitido con éxito a la AEAT.";
                }
            } else if (vfConfig.modo == 2) {
                // Modo No VeriFactu: guardar el XML firmado localmente en una carpeta del sistema
                QString dirCopia = base.devolverDirectorio("cseg");
                if (dirCopia.isEmpty() || !QDir(dirCopia).exists()) {
                    dirCopia = QCoreApplication::applicationDirPath() + "/verifactu_xml";
                } else {
                    dirCopia += "/verifactu_xml";
                }
                QDir().mkpath(dirCopia);
                QString fileXmlPath = QString("%1/%2.xml").arg(dirCopia).arg(numSerie);
                QFile file(fileXmlPath);
                if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
                    file.write(xmlContent.toUtf8());
                    file.close();
                } else {
                    qWarning() << "No se pudo guardar el XML de VeriFactu localmente en:" << fileXmlPath;
                }
            }
            
            // Registrar los datos en la tabla verifactu_logs de la base de datos
            if (base.registrarTickeckVerifactu(
                    conf->getConexionLocal(),
                    totalTicket.at(0).toInt(),
                    totalTicket.at(3),
                    totalTicket.at(4),
                    hashFactura,
                    ultimoHash,
                    xmlContent, // Guardamos el XML completo en cadena_firmada
                    totalTicket.at(1).toInt(),
                    estadoEnvio
                ) == "") {
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
    QMessageBox msgBox(this);
    msgBox.setWindowTitle("Pre-ticket / Presupuesto");
    msgBox.setText("¿Qué desea imprimir?");
    QPushButton *btnTicket = msgBox.addButton("Ticket", QMessageBox::ActionRole);
    QPushButton *btnPresupuesto = msgBox.addButton("Presupuesto", QMessageBox::ActionRole);
    msgBox.addButton("Cancelar", QMessageBox::RejectRole);
    
    msgBox.exec();
    
    if (msgBox.clickedButton() == btnTicket) {
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
    } else if (msgBox.clickedButton() == btnPresupuesto) {
        QTextDocument documento;
        QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; color: #333; margin: 0; padding: 0; }
    .cabecera-superior { padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 10px; text-align: right; }
    .numero-factura { font-size: 14pt; font-weight: bold; color: #2c3e50; }
    table.cabecera { width: 100%; border: none; margin-bottom: 20px; }
    .lineas { width: 100%; border-collapse: collapse; margin-top: 15px; }
    .lineas th { background-color: #2c3e50; color: white; padding: 8px; font-size: 9pt; }
    .lineas td { border: 1px solid #ccc; padding: 6px; text-align: right; font-size: 9pt; }
    .lineas td:nth-child(2) { text-align: left; }
    .totales { text-align: right; margin-top: 20px; font-size: 11pt; }
    .total-final { font-size: 14pt; color: #e74c3c; font-weight: bold; border-top: 2px solid #2c3e50; padding-top: 10px; }
  </style>
</head>
<body>
<!-- Número de factura y fecha en la parte SUPERIOR -->
<div class='cabecera-superior'>
    <div class='numero-factura' style='font-size: 18pt; text-transform: uppercase;'>PRESUPUESTO</div>
    <div>FECHA: %FECHA%</div>
</div>

<table class='cabecera' style='width: 100%; border-collapse: collapse;'>
  <!-- Logo centrado y ancho -->
  <tr>
    <td colspan='2' style='text-align: center; padding: 10px 0;'>
      <img src=':/imagenes/documentos/Cabecera factura.png' width='600' style='max-width: 100%; height: auto;' alt='Logo Emeicjac'/>
    </td>
  </tr>

  <!-- Información en dos columnas -->
  <tr>
    <td style='width: 50%; vertical-align: top; padding: 15px; background-color: #f9f9f9; border-radius: 5px;'>
      <div style='color: #2c3e50; font-weight: bold; margin-bottom: 5px;'>CLIENTE:</div>
      <b>%CLIENTE%</b>
    </td>

    <td style='width: 50%; vertical-align: top; padding: 15px; text-align: right;'>
      <div style='color: #2c3e50; font-weight: bold; margin-bottom: 5px;'>EMISOR:</div>
      %TIENDA%
    </td>
  </tr>
</table>

<table class='lineas'>
  <thead>
    <tr>
      <th>CANTIDAD</th>
      <th>DESCRIPCIÓN</th>
      <th>PRECIO</th>
      <th>DESC.</th>
      <th>IVA</th>
      <th>TOTAL</th>
    </tr>
  </thead>
  <tbody>
    %LINEAS%
  </tbody>
</table>

<div class='totales'>
  <p>Total Base: <strong>%BASE% €</strong></p>
  <p>Total IVA: <strong>%IVA% €</strong></p>
  <div class='total-final'>TOTAL PRESUPUESTO: %TOTAL% €</div>
</div>

</body>
</html>
)";

        QString datosTiendaLocal = "";
        QStringList datosTienda = base.datosTiendaLocal(conf->getConexionLocal());

        for (int i = 1; i < 7; ++i) {
            datosTiendaLocal += datosTienda.at(i) + "<br>";
        }
        html.replace("%TIENDA%", datosTiendaLocal);
        html.replace("%CLIENTE%", ui->lineEdit_nobre_cliente->text());
        html.replace("%FECHA%", QDate::currentDate().toString("yyyy-MM-dd"));
        
        double totalBase = 0;
        double totalIva = 0;
        QString lineasHTML;
        
        for (int i = 0; i < modeloTicket->rowCount(); i++) {
            QString cod = modeloTicket->record(i).value(2).toString();
            QString descripcion = modeloTicket->record(i).value(3).toString();
            QString cantidad = modeloTicket->record(i).value(4).toString();
            QString tipoiva = modeloTicket->record(i).value(5).toString() + "%";
            QString precio = QString::number(modeloTicket->record(i).value(6).toDouble(), 'f', 2);
            QString descuento = QString::number(modeloTicket->record(i).value(7).toDouble(), 'f', 2);
            double totalLinea = modeloTicket->record(i).value(8).toDouble();
            QString total = QString::number(totalLinea, 'f', 2);

            double baseTMP = totalLinea / (1 + (modeloTicket->record(i).value(5).toDouble() / 100));
            double ivaTMP = totalLinea - baseTMP;

            totalBase += baseTMP;
            totalIva += ivaTMP;

            lineasHTML += QString(R"(
            <tr>
                <td>%1</td>
                <td>%2</td>
                <td>%3 €</td>
                <td>%4 %</td>
                <td>%5</td>
                <td>%6 €</td>
            </tr>
        )").arg(cantidad, descripcion, precio, descuento, tipoiva, total);
        }
        
        html.replace("%LINEAS%", lineasHTML);

        html.replace("%BASE%", QString::number(totalBase, 'f', 2));
        html.replace("%IVA%", QString::number(totalIva, 'f', 2));
        html.replace("%TOTAL%", QString::number(totalBase + totalIva, 'f', 2));
        
        documento.setHtml(html);
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(base.devolverDirectorio("documentos") + "/Presupuesto.pdf");
        printer.setPageSize(QPageSize::A4);
        printer.setPageMargins(QMargins(15, 15, 15, 15), QPageLayout::Millimeter);
        documento.print(&printer);
        QProcess::startDetached("xdg-open", QStringList() << base.devolverDirectorio("documentos") + "/Presupuesto.pdf");
    }
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

/**
 * @brief Marca el vale como usado en la BD local y propaga el cambio a la nube.
 *
 * Graba la l\u00ednea de descuento en el ticket, marca el vale como USADO (estado=2)
 * en la BD local y, si hay conexi\u00f3n, tambi\u00e9n en la nube. El SyncManager se
 * encarga de bajar el cambio al resto de tiendas en su pr\u00f3ximo ciclo (5 min).
 * Si la nube no est\u00e1 disponible, el trigger local de sync_cola garantiza que
 * el cambio se subir\u00e1 autom\u00e1ticamente cuando se recupere la conexi\u00f3n.
 *
 * @param ticket   N\u00famero de ticket al que se a\u00f1ade la l\u00ednea de vale
 * @param idVale   ID del vale a marcar como usado
 * @param cantVale Importe del vale (se descuenta en el ticket)
 */
void Tpv::usarVale(int ticket, int idVale, double cantVale)
{
    // --- 1. Grabar l\u00ednea negativa de descuento en el ticket ---
    QStringList lineaTicket;
    lineaTicket.append(QString::number(ticket));
    lineaTicket.append("9999999999999");          // c\u00f3digo reservado para vales
    lineaTicket.append("Vale fidelidad");
    lineaTicket.append("1");                       // cantidad
    lineaTicket.append("0");                       // IVA
    lineaTicket.append(QString::number(cantVale)); // precio
    lineaTicket.append("0");                       // descuento
    lineaTicket.append("-" + QString::number(cantVale)); // total negativo
    lineaTicket.append(QDate::currentDate().toString("yyyy-MM-dd"));
    lineaTicket.append(QTime::currentTime().toString("hh:mm"));
    base.grabarLineaTicket(lineaTicket);

    QString conexionLocal = conf->getConexionLocal();

    // --- 2. Marcar vale como USADO en la BD local ---
    if (base.usarVale(conexionLocal, idVale)) {
        qDebug() << "Vale" << idVale << "marcado como usado en BD local";
    } else {
        qWarning() << "Error al marcar el vale" << idVale << "en BD local";
    }

    // --- 3. Propagar a la nube si est\u00e1 disponible ---
    // El SyncManager bajar\u00e1 el cambio al resto de tiendas en su pr\u00f3ximo ciclo.
    // Si no hay conexi\u00f3n, el trigger local en sync_cola garantiza la propagaci\u00f3n
    // autom\u00e1tica cuando se recupere la conexi\u00f3n con la nube.
    QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    if (dbNube.isOpen()) {
        // Obtener el vale_uuid del vale recién marcado en local
        QSqlQuery qUuid(QSqlDatabase::database(conexionLocal));
        qUuid.prepare("SELECT vale_uuid FROM vales WHERE idvales = ?");
        qUuid.addBindValue(idVale);
        if (qUuid.exec() && qUuid.first()) {
            QString uuid = qUuid.value(0).toString();
            
            // Marcar en la nube usando el UUID (idvales no es igual en la nube)
            QSqlQuery qNubeUpdate(dbNube);
            qNubeUpdate.prepare("UPDATE vales SET estado = 2, fechaUso = CURRENT_DATE() WHERE vale_uuid = ?");
            qNubeUpdate.addBindValue(uuid);
            if (qNubeUpdate.exec()) {
                qDebug() << "Vale" << idVale << "(UUID:" << uuid << ") marcado como usado en la nube inmediatamente.";
            } else {
                qWarning() << "Error al marcar vale UUID" << uuid << "en la nube:" << qNubeUpdate.lastError().text();
            }
        } else {
            qWarning() << "No se pudo obtener el vale_uuid para idvales" << idVale;
        }
    } else {
        qDebug() << "Vale" << idVale << ": nube no disponible, sync_cola propagar\u00e1 el cambio";
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

