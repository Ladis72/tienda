#include "listadosalidas.h"
#include "qtrpt.h"
#include "ui_listadosalidas.h"

ListadoSalidas::ListadoSalidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoSalidas)
{
    ui->setupUi(this);
}

ListadoSalidas::~ListadoSalidas()
{
    delete ui;
}

void ListadoSalidas::on_pushButtonVer_clicked()
{
    QString fechaI, fechaF;
    fechaI = ui->dateEditDesde->text();
    fechaF = ui->dateEditHasta->text();
    modeloTabla = new QSqlQueryModel(this);
    modeloTabla->setQuery(base->listadoMovimientosEfectivo(fechaI, fechaF));
    modeloTabla->setHeaderData(0, Qt::Horizontal, "FECHA");
    modeloTabla->setHeaderData(1, Qt::Horizontal, "HORA");
    modeloTabla->setHeaderData(2, Qt::Horizontal, "CANTIDAD");
    modeloTabla->setHeaderData(3, Qt::Horizontal, "TIPO");
    //modeloTabla->setHeaderData(4,Qt::Horizontal,"DETALLE");
    ui->tableView->setModel(modeloTabla);
    ui->labelTotal->setText(QString::number(sumar(modeloTabla)));
}

double ListadoSalidas::sumar(QSqlQueryModel *modelo)
{
    double A = 0;
    for (int i = 0; i < modelo->rowCount(); ++i) {
        A += modelo->record(i).value("cantidad").toDouble();
    }
    return A;
}

void ListadoSalidas::on_pushButton_2_clicked()
{
    QtRPT *informe = new QtRPT(this);
    informe->recordCount.append(modeloTabla->rowCount());
    QString informeDir = base->devolverDirectorio("movimientos");
    informe->loadReport(informeDir);
    connect(informe,
            &QtRPT::setValue,
            [&](const int recNo,
                const QString paramName,
                QVariant &paramValue,
                const int reportPage) {
                (void) reportPage;
                if (paramName == "desde") {
                    paramValue = ui->dateEditDesde->text();
                }
                if (paramName == "hasta") {
                    paramValue = ui->dateEditHasta->text();
                }
                if (paramName == "fecha") {
                    paramValue = modeloTabla->record(recNo).value("fecha").toString();
                }
                if (paramName == "hora") {
                    paramValue = modeloTabla->record(recNo).value("hora").toString();
                }
                if (paramName == "cantidad") {
                    paramValue = modeloTabla->record(recNo).value("cantidad").toString();
                }
                if (paramName == "tipo") {
                    paramValue = modeloTabla->record(recNo).value("descripcion").toString();
                }
                if (paramName == "descripcion") {
                    paramValue = modeloTabla->record(recNo).value(4).toString();
                }
                if (paramName == "total") {
                    paramValue = ui->labelTotal->text();
                }
            });
    informe->printExec();
}
