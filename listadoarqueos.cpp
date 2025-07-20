#include "listadoarqueos.h"
#include "qprinter.h"
#include "qprocess.h"
#include "qtextdocument.h"
#include "ui_listadoarqueos.h"

ListadoArqueos::ListadoArqueos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoArqueos)
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
    QString fechaI, fechaF;
    fechaI = ui->dateEditDesde->text();
    fechaF = ui->dateEditHasta->text();
    qDebug() << fechaF << "   " << fechaI;
    modeloTabla = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modeloTabla->setTable("arqueos");
    modeloTabla->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloTabla->setFilter("fecha >= '" + fechaI + "' AND fecha <= '" + fechaF + "'");
    modeloTabla->select();
    ui->tableView->setModel(modeloTabla);
}

void ListadoArqueos::on_pushButtonImprimir_clicked()
{
    QTextDocument documento;
    QString lineasHtml = "";
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; }
    h2 { text-align: center; margin-bottom: 20px; }
    .rango { text-align: center; margin-bottom: 15px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th, td { border: 1px solid #333; padding: 6px; text-align: left; }
    th { background-color: #f0f0f0; }
    .total { text-align: right; margin-top: 20px; font-weight: bold; }
  </style>
</head>
<body>

<h2>Listado de Arqueos</h2>
<div class='rango'>
  Desde: <b>%DESDE%</b> &nbsp;&nbsp;&nbsp; Hasta: <b>%HASTA%</b>
</div>

<table>
  <tr>
    <th>Fecha</th>
    <th>Hora</th>
    <th>Ventas efecivo</th>
    <th>Ventas tarjeta</th>
    <th>Entradas</th>
    <th>Efectivo</th>
    <th>Descuadre</th>
  </tr>
  %LINEAS%
</table>


</body>
</html>
)";
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        QString fecha = modeloTabla->data(modeloTabla->index(i,1)).toString();
        QString hora = modeloTabla->data(modeloTabla->index(i,2)).toString();
        QString vefect = modeloTabla->data(modeloTabla->index(i,3)).toString();
        QString vtarj = modeloTabla->data(modeloTabla->index(i,4)).toString();
        QString entradas = modeloTabla->data(modeloTabla->index(i,5)).toString();
        QString efectivo = modeloTabla->data(modeloTabla->index(i,6)).toString();
        QString descuadre = modeloTabla->data(modeloTabla->index(i,7)).toString();

    lineasHtml += QString("<tr><td>%1</td><td>%2</td><td>%3 €</td><td>%4 €</td><td>%5 €</td><td>%6 €</td><td>%7 €</td></tr>").arg(fecha,hora,vefect,vtarj,entradas,efectivo,descuadre);
    }
    html.replace("%DESDE%", ui->dateEditDesde->text());
    html.replace("%HASTA%", ui->dateEditHasta->text());
    html.replace("%LINEAS%",lineasHtml);
    documento.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(base->devolverDirectorio("arqueos")+"/ListadoArqueos.pdf");
    documento.print(&printer);

    // Mostrarlo
    QProcess::startDetached("xdg-open", QStringList() << base->devolverDirectorio("arqueos")+"/ListadoArqueos.pdf");
}
