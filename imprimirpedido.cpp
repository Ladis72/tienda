#include "imprimirpedido.h"
#include <QCoreApplication>
#include <QProcess>
#include <QPrinter>
#include <QTextDocument>
#include <QSqlRecord>
#include <QDate>

imprimirPedido::imprimirPedido(QString tienda, QStringList cabecera, QSqlQueryModel *modelo)
{
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; color: #333; }
    .header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }
    .tienda { font-size: 16pt; font-weight: bold; color: #2c3e50; }
    .pedido-info { font-size: 12pt; margin: 10px 0; }
    table { width: 100%; border-collapse: collapse; margin-top: 15px; }
    th { background-color: #2c3e50; color: white; padding: 8px; font-size: 9pt; }
    td { border: 1px solid #ccc; padding: 6px; text-align: center; font-size: 9pt; }
    .text-left { text-align: left; }
    .footer-table { margin-top: 20px; width: 50%; margin-left: auto; }
    .footer-table td { background-color: #f9f9f9; font-weight: bold; }
    .total-row { font-size: 12pt; color: #e74c3c; }
  </style>
</head>
<body>
  <div class='header'>
    <div class='tienda'>%TIENDA%</div>
    <div class='pedido-info'>HOJA DE PEDIDO Nº: <strong>%NUM%</strong></div>
    <div>Proveedor: <strong>%PROVEEDOR%</strong> | Fecha: %FECHA%</div>
  </div>

  <table>
    <thead>
      <tr>
        <th>CÓDIGO</th>
        <th>DESCRIPCIÓN</th>
        <th>CANT.</th>
        <th>BON.</th>
        <th>CADUCIDAD</th>
        <th>COSTO</th>
        <th>DESC.</th>
        <th>I.V.A.</th>
        <th>P.V.P.</th>
      </tr>
    </thead>
    <tbody>
      %FILAS%
    </tbody>
  </table>

  <table class='footer-table'>
    <tr><td>Base Imponible:</td><td>%BASE% €</td></tr>
    <tr><td>I.V.A.:</td><td>%IVA% €</td></tr>
    <tr><td>Recargo:</td><td>%RECARGO% €</td></tr>
    <tr class='total-row'><td>TOTAL PEDIDO:</td><td>%TOTAL% €</td></tr>
  </table>
</body>
</html>
)";

    html.replace("%TIENDA%", tienda);
    html.replace("%NUM%", cabecera.value(0));
    html.replace("%PROVEEDOR%", cabecera.value(1));
    html.replace("%FECHA%", QDate::currentDate().toString("yyyy-MM-dd"));
    html.replace("%BASE%", cabecera.value(2));
    html.replace("%IVA%", cabecera.value(3));
    html.replace("%RECARGO%", cabecera.value(4));
    html.replace("%TOTAL%", cabecera.value(5));

    QString filas = "";
    for (int i = 0; i < modelo->rowCount(); i++) {
        filas += "<tr>";
        filas += "<td>" + modelo->record(i).value("cod").toString() + "</td>";
        filas += "<td class='text-left'>" + modelo->record(i).value("descripcion").toString() + "</td>";
        filas += "<td>" + modelo->record(i).value("cantidad").toString() + "</td>";
        filas += "<td>" + modelo->record(i).value("bonificacion").toString() + "</td>";
        filas += "<td>" + modelo->record(i).value("fc").toString() + "</td>";
        filas += "<td>" + modelo->record(i).value("costo").toString() + " €</td>";
        filas += "<td>" + modelo->record(i).value("descuento1").toString() + "%</td>";
        filas += "<td>" + modelo->record(i).value("tipoIva").toString() + "%</td>";
        filas += "<td>" + modelo->record(i).value("pvp").toString() + " €</td>";
        filas += "</tr>";
    }
    html.replace("%FILAS%", filas);

    QTextDocument doc;
    doc.setHtml(html);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    
    // Configurar orientación horizontal (Landscape)
    QPageLayout layout = printer.pageLayout();
    layout.setOrientation(QPageLayout::Landscape);
    layout.setMargins(QMarginsF(10, 10, 10, 10));
    printer.setPageLayout(layout);

    printer.setOutputFileName("./documentos/Pedido_" + cabecera.value(0) + ".pdf");
    
    doc.print(&printer);

    QProcess::startDetached("xdg-open", QStringList() << "./documentos/Pedido_" + cabecera.value(0) + ".pdf");
}
