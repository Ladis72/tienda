#include "listadoventaarticulos.h"
#include "ui_listadoventaarticulos.h"

ListadoVentaArticulos::ListadoVentaArticulos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListadoVentaArticulos)
{
    ui->setupUi(this);
    ui->dateTimeEditDesde->setDate(QDate::currentDate());
    ui->dateTimeEditDesde->setTime(QTime(0,0));
    ui->dateTimeEditHasta->setDate(QDate::currentDate());
    ui->dateTimeEditHasta->setTime(QTime(23,59));
}

ListadoVentaArticulos::~ListadoVentaArticulos()
{
    delete ui;
}

void ListadoVentaArticulos::on_pushButtonVer_clicked()
{
    QString primerTicket,ultimoTicket,primerTicketB,ultimoTicketB;
    primerTicket = base.ticketCercanoFecha("tickets",ui->dateTimeEditDesde->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"minimo");
    ultimoTicket = base.ticketCercanoFecha("tickets",ui->dateTimeEditHasta->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"maximo");
    primerTicketB = base.ticketCercanoFecha("ticketss",ui->dateTimeEditDesde->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"minimo");
    ultimoTicketB = base.ticketCercanoFecha("ticketss",ui->dateTimeEditHasta->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"maximo");
    qDebug() << "1ªA " << primerTicket << "UltimoA " << ultimoTicket;
    qDebug() << "1ªB " << primerTicketB << "UltimoB " << ultimoTicketB;
    QSqlQuery consulta = base.estadisticasVentaProductos("lineasticket",primerTicket,ultimoTicket);
    QSqlQueryModel *modeloListado = new QSqlQueryModel;
    modeloListado->setQuery(consulta);
    ui->tableView->setModel(modeloListado);

}

