#include "listadocaducados.h"
#include "ui_listadocaducados.h"
#include "qtrpt.h"

ListadoCaducados::ListadoCaducados(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ListadoCaducados)
{
    ui->setupUi(this);
    desde = ui->dateEditDesde->date().toString("yyyy-MM-dd");
    hasta = ui->dateEditHasta->date().toString("yyyy-MM-dd");
    mCaducados = new QSqlQueryModel;
    llenarTabla(desde , hasta);
}

ListadoCaducados::~ListadoCaducados()
{
    delete ui;
}

void ListadoCaducados::on_dateEditDesde_userDateChanged(const QDate &date)
{
    desde = date.toString("yyyy-MM-dd");
    emit llenarTabla(desde , hasta);
}

void ListadoCaducados::on_dateEditHasta_userDateChanged(const QDate &date)
{
    hasta = date.toString("yyyy-MM-dd");
    emit llenarTabla(desde , hasta);

}

void ListadoCaducados::on_pushButtonImprimir_clicked()
{
    int udsCaducadas = 0;
    float precioUdsCaducadas = 0;
    for (int i = 0; i < mCaducados->rowCount(); ++i) {
        int udsAct = mCaducados->record(i).value("cantidad").toInt();
        udsCaducadas += udsAct;
        precioUdsCaducadas += udsAct*mCaducados->record(i).value("precio").toFloat();
    }
    QtRPT *informe = new QtRPT(this);
    informe->recordCount.append(mCaducados->rowCount());
    informe->loadReport("./documentos/caducados.xml");
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
        if(paramName == "ean"){
            paramValue = mCaducados->record(recNo).value("cod").toString();
        }
        if(paramName == "uds"){
            paramValue = mCaducados->record(recNo).value("cantidad").toString();
        }
        if(paramName == "producto"){
            paramValue = mCaducados->record(recNo).value("descripcion").toString();
        }
        if(paramName == "fecha"){
            paramValue = mCaducados->record(recNo).value("fecha").toString();
        }
        if(paramName == "precio"){
            paramValue = mCaducados->record(recNo).value("precio").toString();
        }
        if(paramName == "sumUds"){
            paramValue = udsCaducadas;
        }
        if(paramName == "sumPrecio"){
            paramValue = precioUdsCaducadas;
        }
});
    informe->printExec();
}

void ListadoCaducados::llenarTabla(QString desde, QString hasta)
{
    mCaducados->setQuery(base->listadoCaducados(desde , hasta));
    ui->tableView->setModel(mCaducados);
    ui->tableView->hideColumn(0);
}
