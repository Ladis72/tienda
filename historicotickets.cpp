#include "historicotickets.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include "formaspago.h"
#include "buscarcliente.h"
#include "ui_historicotickets.h"

HistoricoTickets::HistoricoTickets(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HistoricoTickets)
{
    ui->setupUi(this);
    ui->dateTimeEditHasta->setDate(QDate::currentDate());
    ui->dateTimeEditHasta->setTime(QTime(23, 59, 59));
    ui->dateTimeEditDesde->setDateTime(QDateTime(QDate::currentDate(), QTime(00, 00, 01)));
    listaTickets = new QSqlQueryModel(this);
    ticket = new QSqlQueryModel(this);
    nTicket = "";
    aplicarPermisos();
}

HistoricoTickets::~HistoricoTickets()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones sobre el histórico de tickets.
 */
void HistoricoTickets::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonImprimir->setEnabled(conf->permisos()->tiene("tickets.imprimir"));
    ui->pushButtonFormaPago->setEnabled(conf->permisos()->tiene("tickets.modificar"));
    ui->pushButtonCliente->setEnabled(conf->permisos()->tiene("tickets.modificar"));
    ui->pushButtonImprimirFactura->setEnabled(conf->permisos()->tiene("tickets.facturar"));
}

void HistoricoTickets::mostrarTickets()
{
    nTicket = "";
    QString fechaI, fechaF, horaI, horaF;
    fechaI = ui->dateTimeEditDesde->date().toString("yyyy-MM-dd");
    fechaF = ui->dateTimeEditHasta->date().toString("yyyy-MM-dd");
    horaI = ui->dateTimeEditDesde->time().toString("HH:mm:ss");
    horaF = ui->dateTimeEditHasta->time().toString("HH:mm:ss");
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    // Se usa DATE_FORMAT para garantizar el formato 'yyyy-MM-dd' e impedir fallos con Qt 6 y MariaDB.
    query.prepare("SELECT *, DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha_str FROM tickets WHERE concat_ws('/',fecha,hora) >= ? "
                  "AND concat_ws('/',fecha,hora) <= ?");
    query.bindValue(0, fechaI + "/" + horaI);
    query.bindValue(1, fechaF + "/" + horaF);
    query.exec();
    listaTickets->setQuery(std::move(query));
    QStandardItemModel *vistaTickets = new QStandardItemModel(listaTickets->rowCount(),
                                                              listaTickets->columnCount() - 3, this);
    for (int i = 0; i < listaTickets->rowCount(); ++i) {
        QStandardItem *itemTicket = new QStandardItem(listaTickets->record(i).value(0).toString());
        vistaTickets->setItem(i, 0, itemTicket);
        QString nombreUsusario = listaTickets->record(i).value(1).toString();
        QStandardItem *itemUsuario = new QStandardItem(
            base.nombreUsusario(nombreUsusario, conf->getConexionLocal()));
        vistaTickets->setItem(i, 1, itemUsuario);
        QStandardItem *itemCliente = new QStandardItem(
            base.nombreCliente(listaTickets->record(i).value(2).toString()));
        vistaTickets->setItem(i, 2, itemCliente);

        // Extraer la fecha ya formateada como yyyy-MM-dd desde el alias fecha_str o fallback a QDate
        QString strFecha = listaTickets->record(i).value("fecha_str").toString();
        if (strFecha.isEmpty()) {
            QDate d = listaTickets->record(i).value(3).toDate();
            strFecha = d.isValid() ? d.toString("yyyy-MM-dd") : listaTickets->record(i).value(3).toString();
        }
        QStandardItem *itemFecha = new QStandardItem(strFecha);
        QStandardItem *itemHora = new QStandardItem(listaTickets->record(i).value(4).toTime().toString("HH:mm:ss"));
        vistaTickets->setItem(i, 3, itemFecha);
        vistaTickets->setItem(i, 4, itemHora);
        QStandardItem *itemDescuento = new QStandardItem(
            listaTickets->record(i).value(7).toString());
        vistaTickets->setItem(i, 5, itemDescuento);
        QStandardItem *itemTotal = new QStandardItem(listaTickets->record(i).value(8).toString());
        vistaTickets->setItem(i, 7, itemTotal);
        QStandardItem *itemFormaPago = new QStandardItem(
            base.nombreFormaPago(listaTickets->record(i).value(9).toString(),
                                 conf->getConexionLocal()));
        vistaTickets->setItem(i, 6, itemFormaPago);
        QString pagado;
        if (listaTickets->record(i).value(10).toString() == "1") {
            pagado = "Si";
        } else {
            pagado = "No";
        }
        // QStandardItem *itemPagado = new QStandardItem(pagado);
        // vistaTickets->setItem(i, 8, itemPagado);
        QStandardItem *itemEntrega = new QStandardItem(listaTickets->record(i).value(11).toString());
        vistaTickets->setItem(i, 8, itemEntrega);
        QStandardItem *itemCambio = new QStandardItem(listaTickets->record(i).value(12).toString());
        vistaTickets->setItem(i, 9, itemCambio);
    }
    QStringList etiquetas;
    etiquetas << "Ticket" << "Vendedor" << "Cliente" << "Fecha" << "Hora" << "Dto" << "F. Pago"
              << "Total" << "Entrega" << "Cambio";
    qDebug() << etiquetas;
    vistaTickets->setHorizontalHeaderLabels(etiquetas);
    ui->tableViewTickets->setModel(vistaTickets);
    // for (int i = 5; i < 15; ++i) {
    //     ui->tableViewTickets->hideColumn(i);
    // }
    ui->tableViewTickets->resizeColumnsToContents();
}

void HistoricoTickets::on_pushButtonConsultar_clicked()
{
    mostrarTickets();
}

void HistoricoTickets::on_tableViewTickets_clicked(const QModelIndex &index)
{
    on_tableViewTickets_activated(index);
}

void HistoricoTickets::on_tableViewTickets_activated(const QModelIndex &index)
{
    // Obtenemos el número de ticket directamente de la primera columna de la fila pinchada
    nTicket = index.model()->data(index.siblingAtColumn(0)).toString();

    if (nTicket.isEmpty()) return;

    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("SELECT * FROM lineasticket WHERE nticket = ?");
    q.bindValue(0, nTicket);
    q.exec();
    ticket->setQuery(q);

    ui->tableViewLineasTicket->setModel(ticket);
    ui->tableViewLineasTicket->hideColumn(0); // id
    ui->tableViewLineasTicket->hideColumn(1); // nticket
    ui->tableViewLineasTicket->hideColumn(2); // id_articulo (si existe)
    ui->tableViewLineasTicket->resizeColumnsToContents();
}

void HistoricoTickets::on_pushButtonImprimir_clicked()
{
    if (nTicket == "") {
        QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    ImprimirTicket c1(nTicket, "ticket");
}

void HistoricoTickets::on_pushButtonFormaPago_clicked()
{
    if (nTicket == "") {
        QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    FormasPago *FP = new FormasPago;
    if (FP->exec() == QDialog::Accepted) {
        QString idFormaPago = FP->resultado;
        if (idFormaPago == "") {
            return;
        }
        QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
        consulta.prepare("UPDATE tickets SET fpago = ? WHERE ticket = ?");
        consulta.bindValue(0, FP->resultado);
        consulta.bindValue(1, nTicket);
        consulta.exec();
        qDebug() << consulta.lastError();
        mostrarTickets();
    }
}

void HistoricoTickets::on_pushButtonCliente_clicked()
{
    // Si no se ha seleccionado ningún ticket en la lista, mostramos un aviso
    if (nTicket == "") {
        QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }

    // Obtenemos todos los clientes de la base de datos para realizar la búsqueda
    QSqlQuery query = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "clientes", "nombre", "");

    // Mostramos el diálogo de búsqueda para que el usuario elija el nuevo cliente
    BuscarCliente *buscar = new BuscarCliente(this, std::move(query));
    if (buscar->exec() == QDialog::Accepted) {
        QString idCliente = buscar->resultado;
        if (idCliente != "") {
            // Actualizamos el campo cliente del ticket en la tabla tickets de la base de datos local
            QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
            consulta.prepare("UPDATE tickets SET cliente = ? WHERE ticket = ?");
            consulta.bindValue(0, idCliente);
            consulta.bindValue(1, nTicket);
            consulta.exec();
            qDebug() << consulta.lastError();

            // Volvemos a cargar y mostrar la lista de tickets para ver reflejado el cambio
            mostrarTickets();
        }
    }

    // Liberamos la memoria utilizada por el diálogo de búsqueda
    delete buscar;
}

void HistoricoTickets::on_pushButtonImprimirFactura_clicked()
{
    if (nTicket == "") {
        QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    ImprimirFactura fact(nTicket);
}

void HistoricoTickets::on_pushButtonCerrar_clicked()
{
    close();
}
