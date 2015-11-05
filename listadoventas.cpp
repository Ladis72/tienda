#include "listadoventas.h"
#include "ui_listadoventas.h"

ListadoVentas::ListadoVentas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListadoVentas)
{
    ui->setupUi(this);
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());
}

ListadoVentas::~ListadoVentas()
{
    delete ui;
}

void ListadoVentas::on_pushButtonVer_clicked()
{
    QString fechaI,fechaF;
    fechaF = ui->dateEditHasta->text();
    fechaI = ui->dateEditDesde->text();
    modeloTabla = new QSqlQueryModel(this);
    modeloTabla->setQuery(base->ventasEntreFechas(fechaI,fechaF));
    ui->tableView->setModel(modeloTabla);
}
