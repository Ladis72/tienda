#include "listadoarqueos.h"
#include "ui_listadoarqueos.h"
#include "qtrpt.h"

ListadoArqueos::ListadoArqueos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListadoArqueos)
{
    ui->setupUi(this);
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());

}

ListadoArqueos::~ListadoArqueos()
{
    delete ui;
}

void ListadoArqueos::on_pushButtonConsultar_clicked()
{
    QString fechaI , fechaF;
    fechaI = ui->dateEditDesde->text();
    fechaF = ui->dateEditHasta->text();
    qDebug() << fechaF << "   " << fechaI;
    modeloTabla = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modeloTabla->setTable("arqueos");
    modeloTabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloTabla->setFilter("fecha >= '"+fechaI+"' AND fecha <= '"+fechaF+"'");
    modeloTabla->select();
    ui->tableView->setModel(modeloTabla);
}

void ListadoArqueos::on_pushButtonImprimir_clicked()
{
    QtRPT *informe = new QtRPT(this);
    informe->recordCount.append(modeloTabla->rowCount());
    informe->loadReport("./documentos/listadoArqueos.xml");
    connect(informe, &QtRPT::setValue, [&](const int recNo,
            const QString paramName,
            QVariant &paramValue,
            const int reportPage) {
        (void) reportPage;
        if(paramName == "desde"){
            paramValue = ui->dateEditDesde->text();
        }
        if(paramName == "hasta"){
            paramValue = ui->dateEditHasta->text();
        }
        if(paramName == "fecha"){
            paramValue = modeloTabla->record(recNo).value("fecha").toString();
        }
        if(paramName == "hora"){
            paramValue = modeloTabla->record(recNo).value("hora").toString();
        }
        if(paramName == "ventasE"){
            paramValue = modeloTabla->record(recNo).value("ventasEfectivo").toString();
        }
        if(paramName == "ventasT"){
            paramValue = modeloTabla->record(recNo).value("ventasTarjeta").toString();
        }
        if(paramName == "entradas"){
            paramValue = modeloTabla->record(recNo).value("entradas").toString();
        }
        if(paramName == "efectivo"){
            paramValue = modeloTabla->record(recNo).value("efectivoReal").toString();
        }
        if(paramName == "descuadre"){
            paramValue = modeloTabla->record(recNo).value("descuadre").toString();
        }
});
    informe->printExec();
}
