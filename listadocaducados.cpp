#include "listadocaducados.h"
#include "qprinter.h"
#include "qtextdocument.h"
#include <QProcess>
#include "ui_listadocaducados.h"

ListadoCaducados::ListadoCaducados(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoCaducados)
{
    ui->setupUi(this);




    QDate fechaActual = QDate::currentDate();
    QDate targetDate(fechaActual.year(),1,1);

    hasta = fechaActual.toString("yyyy-MM-dd");
    desde = targetDate.toString("yyyy-MM-dd");

    mCaducados = new QSqlQueryModel;
    llenarTabla(desde, hasta);
    ui->dateEditHasta->setDate(fechaActual);
    ui->dateEditDesde->setDate(targetDate);
}

ListadoCaducados::~ListadoCaducados()
{
    delete ui;
}

void ListadoCaducados::on_dateEditDesde_userDateChanged(const QDate &date)
{
    desde = date.toString("yyyy-MM-dd");
    llenarTabla(desde, hasta);
}

void ListadoCaducados::on_dateEditHasta_userDateChanged(const QDate &date)
{
    hasta = date.toString("yyyy-MM-dd");
    llenarTabla(desde, hasta);
}

void ListadoCaducados::on_pushButtonImprimir_clicked()
{
    QTextDocument documento;
    QString lineasHtml = "";
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; }
    h2 { text-align: center; margin-bottom: 10px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th, td { border: 1px solid #444; padding: 6px; text-align: right; }
    th:first-child, td:first-child { text-align: left; }
    th { background-color: #e0e0e0; }
    tfoot td { font-weight: bold; border-top: 2px solid #000; }
  </style>
</head>
<body>

<h2>Resumen de Caducados entre %FECHAS%</h2>

<table>
  <thead>
    <tr>
    <th>Cod.</th>
    <th>Cantidad</th>
    <th>Producto</th>
    <th>Fecha op.</th>
    <th>Precio</th>
    <th>Fecha cad.</th>    </tr>
  </thead>
  <tbody>
    %LINEAS%
  </tbody>
  <tfoot>
    <tr>
      <td>Total:</td>
      %TOTALES%
    </tr>
  </tfoot>
</table>

</body>
</html>
)";

    html.replace("%FECHAS%", desde+" hasta "+hasta);
    for (int i = 0; i < mCaducados->rowCount(); ++i) {
        QString cod = mCaducados->data(mCaducados->index(i,1)).toString();
        QString cantidad = mCaducados->data(mCaducados->index(i,2)).toString();
        QString descripcion = mCaducados->data(mCaducados->index(i,3)).toString();
        QString fecha = mCaducados->data(mCaducados->index(i,4)).toString();
        QString precio = mCaducados->data(mCaducados->index(i,5)).toString();
        QString fechaCad = mCaducados->data(mCaducados->index(i,6)).toString();

        lineasHtml += QString("<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5 €</td><td>%6</td></tr>").arg(
            cod,cantidad,descripcion,fecha,precio,fechaCad);
    }
    html.replace("%LINEAS%",lineasHtml);

    documento.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("./documentos/ListadoCaducados.pdf");
    documento.print(&printer);

    // Mostrarlo
    QProcess::startDetached("xdg-open", QStringList() << "./documentos/ListadoCaducados.pdf");

}

void ListadoCaducados::llenarTabla(QString desde, QString hasta)
{
    mCaducados->setQuery(base->listadoCaducados(conf->getConexionLocal(),desde, hasta));
    ui->tableView->setModel(mCaducados);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
}
