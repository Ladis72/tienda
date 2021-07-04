#include "listadoventaarticulos.h"
#include "ui_listadoventaarticulos.h"
#include <QSortFilterProxyModel>

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
//    QString primerTicket,ultimoTicket,primerTicketB,ultimoTicketB;
//    primerTicket = base.ticketCercanoFecha("tickets",ui->dateTimeEditDesde->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"minimo");
//    ultimoTicket = base.ticketCercanoFecha("tickets",ui->dateTimeEditHasta->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"maximo");
//    primerTicketB = "B"+base.ticketCercanoFecha("ticketss",ui->dateTimeEditDesde->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"minimo");
//    ultimoTicketB = "B"+base.ticketCercanoFecha("ticketss",ui->dateTimeEditHasta->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss"),"maximo");
//    qDebug() << "1ªA " << primerTicket << "UltimoA " << ultimoTicket;
//    qDebug() << "1ªB " << primerTicketB << "UltimoB " << ultimoTicketB;
//    QSqlQuery consulta = base.estadisticasVentaProductos(primerTicket,ultimoTicket,primerTicketB,ultimoTicketB);
//    QSqlQueryModel *modeloListado = new QSqlQueryModel;
//    modeloListado->setQuery(consulta);
//    QSortFilterProxyModel *proxyModel =new QSortFilterProxyModel;
//    proxyModel->setSourceModel(modeloListado);
//    ui->tableView->setModel(proxyModel);
//    ui->tableView->resizeColumnsToContents();
    QString inicio = ui->dateTimeEditDesde->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss");
    QString final = ui->dateTimeEditHasta->dateTime().toString("yyyy-MM-dd'/'HH:mm:ss");
    QSqlQueryModel *modeloListado = new QSqlQueryModel;
    modeloListado->setQuery(base.listadoVentaArticulos(inicio,final,conf->getConexionLocal()));
    QSortFilterProxyModel *proxyModel = new QSortFilterProxyModel;
    proxyModel->setSourceModel(modeloListado);
    ui->tableView->setModel(proxyModel);
    ui->tableView->resizeColumnsToContents();
}

