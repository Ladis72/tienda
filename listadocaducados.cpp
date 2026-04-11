#include "listadocaducados.h"
#include <QProcess>
#include "qprinter.h"
#include "qtextdocument.h"
#include "ui_listadocaducados.h"

ListadoCaducados::ListadoCaducados(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ListadoCaducados)
{
    ui->setupUi(this);

    QDate fechaActual = QDate::currentDate();
    QDate targetDate(fechaActual.year(), 1, 1);

    hasta = fechaActual.toString("yyyy-MM-dd");
    desde = targetDate.toString("yyyy-MM-dd");

    mCaducados = new QSqlQueryModel(this);
    proxyModel = new QSortFilterProxyModel(this);
    proxyModel->setSourceModel(mCaducados);
    proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel->setFilterKeyColumn(-1); // Filtrar en todas las columnas visibles

    ui->tableView->setModel(proxyModel);
    ui->tableView->setSortingEnabled(true);
    
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

void ListadoCaducados::on_lineEditBuscar_textChanged(const QString &text)
{
    proxyModel->setFilterFixedString(text);
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
    body { font-family: Arial, sans-serif; font-size: 10pt; color: #333; }
    .header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }
    h2 { color: #2c3e50; margin-bottom: 5px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th { background-color: #2c3e50; color: white; padding: 8px; font-size: 9pt; }
    td { border: 1px solid #ccc; padding: 6px; text-align: center; font-size: 9pt; }
    th:first-child, td:first-child { text-align: left; }
    tfoot td { font-weight: bold; background-color: #f9f9f9; border-top: 2px solid #2c3e50; }
  </style>
</head>
<body>

<div class='header'>
  <h2>Resumen de Caducados</h2>
  <div>Rango: %FECHAS%</div>
</div>

<table>
  <thead>
    <tr>
      <th>CÓDIGO</th>
      <th>CANTIDAD</th>
      <th>PRODUCTO</th>
      <th>FECHA OP.</th>
      <th>PRECIO</th>
      <th>FECHA CAD.</th>
    </tr>
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

    html.replace("%FECHAS%", desde + " hasta " + hasta);
    
    double totalCantidad = 0;
    double totalImporte = 0;

    // Usamos el proxyModel para que el PDF refleje el orden y filtro de la pantalla
    for (int i = 0; i < proxyModel->rowCount(); ++i) {
        QString cod = proxyModel->data(proxyModel->index(i, 1)).toString();
        double cantidad = proxyModel->data(proxyModel->index(i, 2)).toDouble();
        QString descripcion = proxyModel->data(proxyModel->index(i, 3)).toString();
        QString fecha = proxyModel->data(proxyModel->index(i, 4)).toString();
        double precio = proxyModel->data(proxyModel->index(i, 5)).toDouble();
        QString fechaCad = proxyModel->data(proxyModel->index(i, 6)).toString();

        totalCantidad += cantidad;
        totalImporte += (cantidad * precio);

        lineasHtml
            += QString(
                   "<tr><td>%1</td><td>%2</td><td>%3</td><td>%4</td><td>%5 €</td><td>%6</td></tr>")
                   .arg(cod)
                   .arg(cantidad, 0, 'f', 2)
                   .arg(descripcion, fecha)
                   .arg(precio, 0, 'f', 2)
                   .arg(fechaCad);
    }
    html.replace("%LINEAS%", lineasHtml);

    // Preparar la fila de totales (6 columnas en total)
    // 1: "Total:" (ya está en el HTML)
    // 2: Suma de cantidades
    // 3: Vacío (Producto)
    // 4: Vacío (Fecha op)
    // 5: Suma de importes (Cantidad * Precio)
    // 6: Vacío (Fecha cad)
    QString totalesHtml = QString("<td>%1</td><td></td><td></td><td>%2 €</td><td></td>")
                              .arg(totalCantidad, 0, 'f', 2)
                              .arg(totalImporte, 0, 'f', 2);
    
    html.replace("%TOTALES%", totalesHtml);

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
    mCaducados->setQuery(base->listadoCaducados(conf->getConexionLocal(), desde, hasta));
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
}
