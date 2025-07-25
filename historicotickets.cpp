#include "historicotickets.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include "formaspago.h"
#include "ui_historicotickets.h"

HistoricoTickets::HistoricoTickets(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::HistoricoTickets)
{
    ui->setupUi(this);
    ui->dateTimeEditHasta->setDate(QDate::currentDate());
    ui->dateTimeEditHasta->setTime(QTime(23, 59, 59));
    ui->dateTimeEditDesde->setDateTime(QDateTime(QDate::currentDate(), QTime(00, 00, 01)));
    listaTickets = new QSqlQueryModel;
    ticket = new QSqlQueryModel;
    nTicket = "";
}

HistoricoTickets::~HistoricoTickets()
{
    delete ui;
}

void HistoricoTickets::mostrarTickets()
{
    nTicket = "";
    QString fechaI, fechaF, horaI, horaF;
    fechaI = ui->dateTimeEditDesde->date().toString("yyyy-MM-dd");
    fechaF = ui->dateTimeEditHasta->date().toString("yyyy-MM-dd");
    horaI = ui->dateTimeEditDesde->time().toString("HH:mm:ss");
    horaF = ui->dateTimeEditHasta->time().toString("HH:mm:ss");
    listaTickets->setQuery("SELECT * FROM tickets WHERE concat_ws('/',fecha,hora) >='" + fechaI
                               + "/" + horaI
                               + "' AND "
                                 "concat_ws('/',fecha,hora) <= '"
                               + fechaF + "/" + horaF + "'",
                           QSqlDatabase::database(conf->getConexionLocal()));
    QStandardItemModel *vistaTickets = new QStandardItemModel(listaTickets->rowCount(),
                                                              listaTickets->columnCount()-3);
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
        QStandardItem *itemFecha = new QStandardItem(listaTickets->record(i).value(3).toString());
        QStandardItem *itemHora = new QStandardItem(listaTickets->record(i).value(4).toString());
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
    etiquetas << "Ticket" << "Vendedor" << "Cliente" << "Fecha" << "Hora"  << "Dto" << "F. Pago" << "Total" << "Entrega" << "Cambio";
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

void HistoricoTickets::on_tableViewTickets_activated(const QModelIndex &index)
{
    QModelIndex indice = listaTickets->index(index.row(), 0);
    nTicket = listaTickets->data(indice, Qt::EditRole).toString();
    ticket->setQuery("SELECT * FROM lineasticket WHERE nticket = " + nTicket,
                     QSqlDatabase::database(conf->getConexionLocal()));

    ui->tableViewLineasTicket->setModel(ticket);
    ui->tableViewLineasTicket->hideColumn(0);
    ui->tableViewLineasTicket->hideColumn(1);
    ui->tableViewLineasTicket->hideColumn(2);
    ui->tableViewLineasTicket->resizeColumnsToContents();
    //ui->label->setText("Vendedor"+listaTickets->data());
}

void HistoricoTickets::on_pushButtonImprimir_clicked()
{
    if (nTicket == "") {
        int msg = QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    ImprimirTicket c1(nTicket, "ticket");
}

void HistoricoTickets::on_pushButtonFormaPago_clicked()
{
    if (nTicket == "") {
        int msg = QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    FormasPago *FP = new FormasPago;
    if (FP->exec() == QDialog::Accepted) {
        QString idFormaPago = FP->resultado;
        if (idFormaPago == "") {
            return;
        }
        QSqlQuery consulta = base.ejecutarSentencia("UPDATE tickets SET fpago ='" + FP->resultado
                                                        + "' WHERE ticket = '" + nTicket + "'",
                                                    conf->getConexionLocal());
        qDebug() << consulta.lastError();
        mostrarTickets();
    }
}

void HistoricoTickets::on_pushButtonImprimirFactura_clicked()
{
    if (nTicket == "") {
        int msg = QMessageBox::information(this, "Error", "Primero debe seleccionar un ticket");
        return;
    }
    ImprimirFactura fact(nTicket);
}
