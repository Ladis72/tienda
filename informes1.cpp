#include "informes1.h"
#include "qprinter.h"
#include "qprocess.h"
#include "qtextdocument.h"
#include <QCoreApplication>
#include <QFile>

informes1::informes1(QString tienda, QString fechaI, QString fechaF, QStandardItemModel *tabla)
{
    // QFile fichero("./documentos/ventas.html");
    // fichero.open(QIODevice::WriteOnly);
    // QTextStream pagina(&fichero);

    // //Cabecera HTML
    // QString html = "<!DOCTYPE html>\n"
    //                "<HTML>\n"
    //                "<HEAD>\n"
    //                "<TITLE>Informe de ventas</TITLE>\n"
    //                "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>\n"
    //                "<link rel='stylesheet' href='main.css'>\n"
    //                "</HEAD>\n"
    //                "<BODY>\n"
    //                "<div class='container'>\n"
    //                "<h3 class='text-center' style='text-align: center'>Ventas entre fechas</h3>\n"
    //                "<h2 class='text-center' style='text-align: center'>"
    //                + tienda
    //                + "</h2>"
    //                  "<h5 class='text-center' style='text-align: center'> DESDE: "
    //                + fechaI + "  HASTA: " + fechaF
    //                + "</h5>\n"
    //                  "<table class='table'>\n";
    // pagina << html;
    // //Cabecera
    // pagina << "<thead>"
    //           "<tr><th> FECHA </th><th> VENTAS </th>";
    // if (tabla->columnCount() > 2) {
    //     pagina << "<th>VENTAS B</th>";
    // }
    // pagina << "<tr>"
    //           "</thead>";
    // //Tabla HTML
    // pagina << "<tbody>";

    // double totalA = 0, totalB = 0;
    // for (int fila = 0; fila < tabla->rowCount(); fila++) {
    //     pagina << "<tr class='table-primary'>";
    //     totalA += tabla->item(fila, 1)->text().toDouble();
    //     if (tabla->columnCount() > 2) {
    //         totalB += tabla->item(fila, 2)->text().toDouble();
    //     }
    //     for (int col = 0; col < tabla->columnCount(); col++) {
    //         pagina << "<td>" + tabla->item(fila, col)->text() + "</td>\n";
    //     }
    //     pagina << "</tr>\n";
    // }
    // pagina << "<tr><td></td><td>" + QString::number(totalA) + "</td>";
    // if (tabla->columnCount() > 2) {
    //     pagina << "<td>" + QString::number(totalB) + "</td>";
    // }

    // pagina << "</tr>";
    // if (totalB > 0) {
    //     double total = totalA + totalB;
    //     pagina << "<tr><td></td><td></td><td>" + QString::number(total) + "</td></tr>";
    // }

    // pagina << "</tbody>"
    //           "</table>\n"
    //           "</body>"
    //           "</HTML>\n";

    // fichero.close();
    // system("firefox " + QCoreApplication::applicationDirPath().toLocal8Bit()
    //        + "/documentos/ventas.html");
    imprimir(tienda,fechaI,fechaF,tabla);
}

bool informes1::imprimir(QString tienda, QString fechaI, QString fechaF, QStandardItemModel *tabla)
{
    QTextDocument pagina;
    QString lineasHtml, totalesHtml;
    double total = 0;
    double totalB = 0;
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

        <h2>Ventas de %FECHAS%</h2>

        <table>
          <thead>
            <tr>
              %CABECERAS%
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
    html.replace("%FECHAS%", tienda+" desde "+fechaI+" hasta"+fechaF);
    if (tabla->columnCount() == 2) {
        for (int i = 0; i < tabla->rowCount(); ++i) {
            QString fecha = tabla->item(i,0)->text();
            QString venta = QString::number(tabla->item(i,1)->text().toDouble(), 'f',2);
            total += venta.toDouble();
            lineasHtml += QString(R"(
    <tr>
        <td>%1</td>
        <td>%2 €</td>
    </tr>
)").arg(fecha,venta);
        }
        totalesHtml = QString(R"(<td>%1</td>)").arg(QString::number(total));}
    else {
        for (int i = 0; i < tabla->rowCount(); ++i) {
            QString fecha = tabla->item(i,0)->text();
            QString venta = QString::number(tabla->item(i,1)->text().toDouble(), 'f',2);
            QString ventaB = QString::number(tabla->item(i,2)->text().toDouble(), 'f',2);
            total += venta.toDouble();
            totalB += ventaB.toDouble();
            lineasHtml += QString(R"(
    <tr>
        <td>%1</td>
        <td>%2 €</td>
        <td>%3 €</td>
    </tr>
)").arg(fecha,venta,ventaB);
        }
    totalesHtml = QString(R"(<td>%1</td><td>%2</td><td>%3</td>)").arg(QString::number(total),QString::number(totalB),QString::number(totalB+total));
    }
    html.replace("%LINEAS%",lineasHtml);

    html.replace("%TOTALES%",totalesHtml);
    pagina.setHtml(html);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("./documentos/ventas.pdf");
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMargins(15,15,15,15),QPageLayout::Millimeter);
    pagina.print(&printer);
    QProcess::startDetached("xdg-open", QStringList() << "./documentos/ventas.pdf");
    return true;

}
