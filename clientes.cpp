#include "clientes.h"
#include "ui_clientes.h"

#include <QMessageBox>
#include <QStandardItemModel>

Clientes::Clientes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clientes)
{
    ui->setupUi(this);

    nombreConexionMaster = base.nombreConexionMaster();
    nombreConexionLocal = base.nombreConexionLocal();
    qDebug() << nombreConexionMaster;
    if (!QSqlDatabase::database(nombreConexionMaster).isOpen()) {
        qDebug() << "MASTER abierta" << nombreConexionMaster;
        ui->pushButtonNuevo->setEnabled(false);

        qDebug() << "MASTER abierta" << nombreConexionMaster;
        ui->pushButtonModificar->setEnabled(false);
        qDebug() << "MASTER abierta" << nombreConexionMaster;
    }
    QStringList co = QSqlDatabase::connectionNames();
    qDebug() << "ConnectoonNames: " << co;
    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod,0);
    mapper.addMapping(ui->lineEditNombre,1);
    mapper.addMapping(ui->lineEditApellidos,2);
    mapper.addMapping(ui->lineEditDireccion,3);
    mapper.addMapping(ui->lineEditCP,4);
    mapper.addMapping(ui->lineEditLocalidad,5);
    mapper.addMapping(ui->lineEditProvincia,6);
    mapper.addMapping(ui->lineEditNIF,7);
    mapper.addMapping(ui->lineEditTlfn1,8);
    mapper.addMapping(ui->lineEditTlfn2,9);
    mapper.addMapping(ui->lineEditMail,10);
    mapper.addMapping(ui->lineEditDescuento,11);
    mapper.addMapping(ui->dateEdit,12);
    mapper.addMapping(ui->plainTextEdit,13);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());

    listaTickets = new QSqlQueryModel;
    ticket = new QSqlQueryModel;
    nTicket = "";
}

Clientes::Clientes(QWidget *parent , QString codigo) :
    QDialog(parent),
    ui(new Ui::Clientes)
{

    ui->setupUi(this);
    nombreConexionMaster = base.nombreConexionMaster();
    nombreConexionLocal = base.nombreConexionLocal();
    qDebug() << nombreConexionMaster;
    if (!QSqlDatabase::database(nombreConexionMaster).isOpen()) {
        qDebug() << "MASTER abierta" << nombreConexionMaster;
        ui->pushButtonNuevo->setEnabled(false);

        qDebug() << "MASTER abierta" << nombreConexionMaster;
        ui->pushButtonModificar->setEnabled(false);
        qDebug() << "MASTER abierta" << nombreConexionMaster;
    }
    QStringList co = QSqlDatabase::connectionNames();
    qDebug() << "ConnectoonNames: " << co;

    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod,0);
    mapper.addMapping(ui->lineEditNombre,1);
    mapper.addMapping(ui->lineEditApellidos,2);
    mapper.addMapping(ui->lineEditDireccion,3);
    mapper.addMapping(ui->lineEditCP,4);
    mapper.addMapping(ui->lineEditLocalidad,5);
    mapper.addMapping(ui->lineEditProvincia,6);
    mapper.addMapping(ui->lineEditNIF,7);
    mapper.addMapping(ui->lineEditTlfn1,8);
    mapper.addMapping(ui->lineEditTlfn2,9);
    mapper.addMapping(ui->lineEditMail,10);
    mapper.addMapping(ui->lineEditDescuento,11);
    mapper.addMapping(ui->dateEdit,12);
    mapper.addMapping(ui->plainTextEdit,13);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
    borrarFormulario();
    ui->lineEditCod->setText(codigo);
    ui->lineEditCod->setEnabled(false);

}

Clientes::~Clientes()
{
    delete ui;
}

void Clientes::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM clientes",QSqlDatabase::database(nombreConexionLocal));
    //modeloTabla->setQuery("SELECT * FROM clientes",QSqlDatabase::database("DB"));
    mapper.setModel(modeloTabla);
}

void Clientes::borrarFormulario()
{
    foreach (QLineEdit* le, ui->Principal->findChildren<QLineEdit*>()) {
        le->clear();
    }
    ui->dateEdit->setDate(QDate::currentDate());
    ui->plainTextEdit->clear();
    ui->labelNombreCliente->clear();
}

QStringList Clientes::recogerDatosFormulario()
{
    QStringList listaDatosFormulario;
    listaDatosFormulario.append(ui->lineEditCod->text());
    listaDatosFormulario.append(ui->lineEditNombre->text());
    listaDatosFormulario.append(ui->lineEditApellidos->text());
    listaDatosFormulario.append(ui->lineEditDireccion->text());
    listaDatosFormulario.append(ui->lineEditCP->text());
    listaDatosFormulario.append(ui->lineEditLocalidad->text());
    listaDatosFormulario.append(ui->lineEditProvincia->text());
    listaDatosFormulario.append(ui->lineEditNIF->text());
    listaDatosFormulario.append(ui->lineEditTlfn1->text());
    listaDatosFormulario.append(ui->lineEditTlfn2->text());
    listaDatosFormulario.append(ui->lineEditMail->text());
    listaDatosFormulario.append(ui->lineEditDescuento->text());
    listaDatosFormulario.append(ui->dateEdit->text());
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());

    return listaDatosFormulario;
}
bool Clientes::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditCod) {
        if (event->type() == QEvent::MouseButtonPress) {
            borrarFormulario();
            qDebug() << "event";
            return true;
        }
    }
    return false;
}

void Clientes::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() -1);
    ui->labelNombreCliente->setText(ui->lineEditNombre->text()+" "+ui->lineEditApellidos->text());
    cargarCompras();
}

void Clientes::cargarCompras()
{
    modeloCompras.clear();
    if (ui->radioButtonMeses->isChecked()) {
        modeloCompras.setQuery("SELECT year(fecha), month(fecha) , sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" group by year(fecha) , month(fecha) order by year(fecha) desc , month(fecha) desc; ",QSqlDatabase::database(nombreConexionLocal));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Año");
        modeloCompras.setHeaderData(1,Qt::Horizontal,"Mes");
        modeloCompras.setHeaderData(2,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;
    }
    if (ui->radioButtonAnos->isChecked()) {
        modeloCompras.setQuery("SELECT year(fecha) , sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" group by year(fecha)  order by year(fecha) desc ; ",QSqlDatabase::database(nombreConexionLocal));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Año");
        modeloCompras.setHeaderData(1,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;
    }
    if (ui->radioButtonFechas->isChecked()) {
        modeloCompras.setQuery("select sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" and fecha between '"+ui->dateEditDesde->date().toString("yyyy-MM-dd")+"' and '"+ui->dateEditHasta->date().toString("yyyy-MM-dd")+"';",QSqlDatabase::database(nombreConexionLocal));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;    }

}


void Clientes::on_pushButtonAnterior_clicked()
{
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonSiguiente_clicked()
{
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonNuevo_clicked()
{
    if(base.existeDatoEnTabla(QSqlDatabase::database(nombreConexionMaster),"clientes","idCliente",ui->lineEditCod->text())){
        QMessageBox::warning(this, "ATENCION",
                              "El registro ya existe");
        return;
    }


    QStringList datos = recogerDatosFormulario();
    if (base.crearCliente(QSqlDatabase::database(nombreConexionMaster),datos)) {
        QMessageBox::about(this,"Atención", "Cliente creado con éxito en MASTER");
    } else {
        QMessageBox::warning(this,"Error","No se ha podido crear el Cliente en MASTER");
        return;
    }
    if (base.crearCliente(QSqlDatabase::database(nombreConexionLocal),datos)) {
        QMessageBox::about(this,"Atención", "Cliente creado con éxito en LOCAL");
    } else {
        QMessageBox::warning(this,"Error","No se ha podido crear el Cliente en LOCAL");
        return;
    }

    recargarTabla();

}

void Clientes::on_pushButtonModificar_clicked()
{
    QStringList datos=recogerDatosFormulario();
    int i = mapper.currentIndex();

    QMessageBox msgBox;
    msgBox.setText("MODIFICACION");
    msgBox.setInformativeText("Quiere guardar los cambios?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();
    if (resp == QMessageBox::Ok) {
       if(base.modificarCliente(QSqlDatabase::database(nombreConexionMaster),datos,ui->lineEditCod->text())){
           msgBox.setText("Guardado con exito");
           msgBox.setInformativeText("El registro se ha modificado correctamente en MASTER");
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();

       }else {
           msgBox.setText("Error al guardar");
           msgBox.setInformativeText("Revise los datos del formulario o contacte con el administrador");
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
           return;
       }
       if(base.modificarCliente(QSqlDatabase::database(nombreConexionLocal),datos,ui->lineEditCod->text())){
           msgBox.setText("Guardado con exito");
           msgBox.setInformativeText("El registro se ha modificado correctamente en LOCAL");
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
       }else {
        msgBox.setText("Error al guardar");
        msgBox.setInformativeText("Revise los datos del formulario o contacte con el administrador");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
       return;
       }
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(i);
}

void Clientes::on_pushButtonBorrar_clicked()
{
    int idCliente = mapper.currentIndex();
    QMessageBox msgBox;
    msgBox.setText("Borrar?.");
    msgBox.setInformativeText("Borrar un cliente puede influir en documentos anteriores.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (msgBox.exec() == QMessageBox::Ok) {
        if (base.borrarCliente(QSqlDatabase::database(nombreConexionLocal),ui->lineEditCod->text().toInt())) {
            msgBox.setText("Borrado");
            msgBox.setInformativeText("El cliente se ha borrado con éxito");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        } else {
            msgBox.setText("Error al borrar");
            msgBox.setInformativeText("No se ha podido borrar el cliente");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    recargarTabla();
    mapper.setCurrentIndex(idCliente);
    refrescarBotones(idCliente);
}



void Clientes::on_lineEditNombre_returnPressed()
{
    consulta = base.buscarEnTabla(QSqlDatabase::database(nombreConexionLocal),"clientes","nombre", ui->lineEditNombre->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    qDebug() << buscar->resultado;
    for (int i = 0; i < modeloTabla->rowCount(); i++){
        if (modeloTabla->record(i).value("idCliente").toString() == buscar->resultado) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
    delete buscar;
}

void Clientes::on_lineEditCod_editingFinished()
{
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        if (modeloTabla->record(i).value("idCliente").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
}

void Clientes::on_radioButtonMeses_clicked()
{
    cargarCompras();
}

void Clientes::on_radioButtonAnos_clicked()
{
    cargarCompras();

}

void Clientes::on_radioButtonFechas_clicked()
{
    cargarCompras();

}

void Clientes::on_dateEditHasta_userDateChanged(const QDate &date)
{
    cargarCompras();

}

void Clientes::on_dateEditDesde_userDateChanged(const QDate &date)
{
    cargarCompras();

}

void Clientes::on_pushButtonVerProductos_clicked()
{

}


void Clientes::on_tableView_clicked(const QModelIndex &index)
{

    if(ui->radioButtonAnos->isChecked()){
        QModelIndex indice = modeloCompras.index(index.row(),0);
        QString dato = modeloCompras.data(indice,Qt::DisplayRole).toString();
        fechaI = dato+"-01-01";
        fechaF = dato+"-12-31";
    }
    if (ui->radioButtonMeses->isChecked()) {
        QModelIndex indiceAno = modeloCompras.index(index.row(),0);
        QString ano = modeloCompras.data(indiceAno,Qt::DisplayRole).toString();
        QModelIndex indiceMes = modeloCompras.index(index.row(),1);
        QString mes = modeloCompras.data(indiceMes,Qt::DisplayRole).toString();
        fechaI = ano+"-"+mes+"-01";
        QDate fecha(ano.toInt(),mes.toInt(),1);
        fechaF = ano+"-"+mes+"-"+QString::number(fecha.daysInMonth());
    }
    if (ui->radioButtonFechas->isChecked()) {
        fechaI = ui->dateEditDesde->date().toString("yyyy-MM-dd");
        fechaF = ui->dateEditHasta->date().toString("yyyy-MM-dd");
    }
    qDebug() << fechaI << "    " << fechaF;
    //QSqlQueryModel *listaTickets = new QSqlQueryModel;
    //QSqlQuery listaTickes2 = base.tickesPorCLiente(nombreConexionLocal,fechaI,fechaF,ui->lineEditCod->text());
    listaTickets->setQuery(base.tickesPorCLiente(nombreConexionLocal,fechaI,fechaF,ui->lineEditCod->text()));
    qDebug() << listaTickets->rowCount();
    QStandardItemModel *vistaTickets = new QStandardItemModel(listaTickets->rowCount(),listaTickets->columnCount());

    for (int i = 0;i < listaTickets->rowCount() ; i++) {
        QStandardItem *itemTicket = new QStandardItem(listaTickets->record(i).value(0).toString());
        vistaTickets->setItem(i,0,itemTicket);
        QString nombreUsusario = listaTickets->record(i).value(1).toString();
        QStandardItem *itemUsuario = new QStandardItem(base.nombreUsusario(nombreUsusario));
        vistaTickets->setItem(i,1,itemUsuario);
//        QStandardItem *itemCliente = new QStandardItem(base.nombreCliente(listaTickets->record(i).value(2).toString()));
//        vistaTickets->setItem(i,2,itemCliente);
        QStandardItem *itemFecha = new QStandardItem(listaTickets->record(i).value(3).toString());
        QStandardItem *itemHora = new QStandardItem(listaTickets->record(i).value(4).toString());
        vistaTickets->setItem(i,3,itemFecha);
        vistaTickets->setItem(i,4,itemHora);
        QStandardItem *itemDescuento = new QStandardItem(listaTickets->record(i).value(11).toString());
        vistaTickets->setItem(i,11,itemDescuento);
        QStandardItem *itemTotal = new QStandardItem(listaTickets->record(i).value(12).toString());
        vistaTickets->setItem(i,12,itemTotal);
        QStandardItem *itemFormaPago = new QStandardItem(base.nombreFormaPago(listaTickets->record(i).value(13).toString()));
        vistaTickets->setItem(i,13,itemFormaPago);
        QString pagado;
        if (listaTickets->record(i).value(14).toString() == "1") {
            pagado = "Si";
        } else {pagado = "No";
        }
        QStandardItem *itemPagado = new QStandardItem(pagado);
        vistaTickets->setItem(i,14,itemPagado);
        QStandardItem *itemEntrega = new QStandardItem(listaTickets->record(i).value(15).toString());
        vistaTickets->setItem(i,15,itemEntrega);
        QStandardItem *itemCambio = new QStandardItem(listaTickets->record(i).value(16).toString());
        vistaTickets->setItem(i,16,itemCambio);
    }
    QStringList etiquetas;
    etiquetas << "Ticket" << "Vendedor" << "" << "Fecha" << "Hora" << "" << "" << "" << "" << "" << "" << "Dto" << "Total" << "F. Pago" << "Pagado" << "Entrega" << "Cambio";
    qDebug() << etiquetas;
    vistaTickets->setHorizontalHeaderLabels(etiquetas);
    ui->tableView2->setModel(vistaTickets);
    ui->tableView2->hideColumn(2);
    for (int i = 5; i < 11; ++i) {
        ui->tableView2->hideColumn(i);
    }
    ui->tableView2->resizeColumnsToContents();
}


void Clientes::on_tableView2_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = listaTickets->index(index.row(),0);
    nTicket = listaTickets->data(indice,Qt::EditRole).toString();
    ticket->setQuery("SELECT * FROM lineasticket WHERE nticket = "+nTicket,QSqlDatabase::database("DB"));


    ui->tableViewDetalleTicket->setModel(ticket);
    ui->tableViewDetalleTicket->hideColumn(0);
    ui->tableViewDetalleTicket->hideColumn(1);
    ui->tableViewDetalleTicket->hideColumn(2);
    ui->tableViewDetalleTicket->resizeColumnsToContents();
}

