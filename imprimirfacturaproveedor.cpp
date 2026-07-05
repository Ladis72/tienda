#include "imprimirfacturaproveedor.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QFile>
#include <QPrinter>
#include <QProcess>
#include <QTextDocument>
#include <QTextStream>

imprimirFacturaProveedor::imprimirFacturaProveedor(QString tienda,
                                                   QStringList cabecera,
                                                   QString idFactura)
{
    // QFile fichero("./documentos/facturaProveedor.html");
    // fichero.open(QIODevice::WriteOnly);
    // QTextStream pagina(&fichero);
    // QString html = "<!DOCTYPE html>\n"
    //                "<HTML>\n"
    //                "<HEAD>\n"
    //                "<TITLE>PEDIDO</TITLE>\n"
    //                "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>\n"
    //                "<link rel='stylesheet' href='main.css'>\n"
    //                "</HEAD>\n"
    //                "<BODY>\n"
    //                "<div class='container'>\n"
    //                "<h3 class='text-center' style='text-align: center'>FACTURA</h3>\n\n\n"
    //                "<h2 class='text-center' style='text-align: center'>"
    //                + tienda
    //                + "</h2>"
    //                  "<table class='table'>\n"
    //                  "<thead>"
    //                  "<tr><th>Nº. "
    //                  "Factura</th><th>Fecha</th><th>Proveedor</th><th>Base</th><th>IVA</th><th>RE</"
    //                  "th><th>Total</th><th>Vencimeinto</th></tr>"
    //                  "</thead>";
    // pagina << html;
    // pagina << "<tr>";
    // if (cabecera.isEmpty()) {
    //     qDebug() << "cabecera vacia";
    //     cabecera = base->datosFactura(QSqlDatabase::database(tienda), idFactura);
    // }
    // for (int i = 0; i < cabecera.length() - 1; i++) {
    //     pagina << "<td>" + cabecera.at(i) + "</td>";
    // }
    // pagina << "</tr></table>";
    // pagina << "</div>";
    // pagina << "<div class='container'>\n"
    //           "<table class='table'>\n";
    // //Cabecera
    // pagina << "<thead>"
    //           "<tr><th>COD</th><th>DESCRIPCION</th><th>CANTIDAD</th><th>BON</th><th>CADUCIDAD</"
    //           "th><th>COSTO</th><th>DESCUENTO</th><th>I.V.A.</th><th>Total "
    //           "base</th><th>P.V.P.</th></tr>"
    //           "</thead>";

    // //Datos tabla
    // pagina << "<tbody>";
    // QSqlQuery consulta = base->ejecutarSentencia("SELECT * FROM lineaspedido WHERE nDocumento = '"
    //                                                  + idFactura + "'",
    //                                              conf->getConexionLocal());
    // consulta.first();
    // for (int i = 0; i < consulta.numRowsAffected(); i++) {
    //     pagina << "<tr class='table-primary'>";
    //     pagina << "<td>" + consulta.record().value(3).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(4).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(5).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(6).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(8).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(9).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(10).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(12).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(13).toString() + "</td>";
    //     pagina << "<td>" + consulta.record().value(16).toString() + "</td>";

    //     pagina << "</tr>";
    //     consulta.next();
    // }
    // pagina << "</tbody></table></div></body></html>";
    // fichero.close();
    // system("firefox " + QCoreApplication::applicationDirPath().toLocal8Bit()
    //        + "/documentos/facturaProveedor.html");

    // 1. Generar el contenido HTML primero
    QString html;
    html += "<!DOCTYPE html>\n"
            "<HTML>\n"
            "<HEAD>\n"
            "<TITLE>FACTURA</TITLE>\n"
            "<meta charset='UTF-8'>\n"
            "<style>\n"
            "body { font-family: Arial, sans-serif; font-size: 9pt; color: #333; }\n"
            ".header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }\n"
            "h2 { color: #2c3e50; margin: 5px 0; }\n"
            "table { width: 100%; border-collapse: collapse; margin-top: 10px; }\n"
            "th { background-color: #2c3e50; color: white; padding: 6px; font-size: 8pt; }\n"
            "td { border: 1px solid #ccc; padding: 5px; text-align: center; font-size: 8pt; }\n"
            ".info-box { background-color: #f9f9f9; padding: 10px; border-radius: 5px; margin-bottom: 15px; }\n"
            "</style>\n"
            "</HEAD>\n"
            "<BODY>\n"
            "<div class='header'>\n"
            "  <h2>FACTURA DE PROVEEDOR</h2>\n"
            "  <div style='font-size: 14pt; font-weight: bold;'>" + tienda + "</div>\n"
            "</div>\n"
            "<div class='container'>\n"
              "<table>\n"
              "<thead>\n"
              "<tr>"
              "<th>Nº Factura</th>"
              "<th>Fecha</th>"
              "<th>Proveedor</th>"
              "<th>Base</th>"
              "<th>IVA</th>"
              "<th>RE</th>"
              "<th>Total</th>"
              "<th>Vencimiento</th>"
              "</tr>\n"
              "</thead>\n"
              "<tbody>\n"
              "<tr>";

    if (cabecera.isEmpty()) {
        qDebug() << "cabecera vacia";
        cabecera = base->datosFactura(QSqlDatabase::database(tienda), idFactura);
    }

    for (int i = 0; i < cabecera.length() - 1; i++) {
        html += "<td>" + cabecera.at(i) + "</td>";
    }

    html += "</tr>\n"
            "</tbody>\n"
            "</table>\n"
            "</div>\n"
            "<div class='container'>\n"
            "<table>\n"
            "<thead>\n"
            "<tr>"
            "<th>COD</th>"
            "<th>DESCRIPCION</th>"
            "<th>CANTIDAD</th>"
            "<th>BON</th>"
            "<th>CADUCIDAD</th>"
            "<th>COSTO</th>"
            "<th>DESCUENTO</th>"
            "<th>I.V.A.</th>"
            "<th>Total base</th>"
            "<th>P.V.P.</th>"
            "</tr>\n"
            "</thead>\n"
            "<tbody>\n";

    // Datos tabla
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
    consulta.prepare("SELECT * FROM lineaspedido WHERE nDocumento = ?");
    consulta.bindValue(0, idFactura);
    consulta.exec();

    if (consulta.first()) {
        do {
            html += "<tr class='table-primary'>";
            html += "<td>" + consulta.record().value(3).toString() + "</td>";
            html += "<td>" + consulta.record().value(4).toString() + "</td>";
            html += "<td>" + consulta.record().value(5).toString() + "</td>";
            html += "<td>" + consulta.record().value(6).toString() + "</td>";
            html += "<td>" + consulta.record().value(8).toString() + "</td>";
            html += "<td>" + consulta.record().value(9).toString() + "</td>";
            html += "<td>" + consulta.record().value(10).toString() + "</td>";
            html += "<td>" + consulta.record().value(12).toString() + "</td>";
            html += "<td>" + consulta.record().value(13).toString() + "</td>";
            html += "<td>" + consulta.record().value(16).toString() + "</td>";
            html += "</tr>\n";
        } while (consulta.next());
    }

    html += "</tbody>\n"
            "</table>\n"
            "</div>\n"
            "</BODY>\n"
            "</HTML>";

    // 2. Crear QTextDocument con el HTML
    QTextDocument documento;
    documento.setHtml(html);

    // 3. Configurar QPrinter para PDF
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setPageOrientation(QPageLayout::Landscape);

    // Guardar en directorio documentos
    QString rutaPDF = QCoreApplication::applicationDirPath() + "/documentos/FacturaProveedor.pdf";
    printer.setOutputFileName(rutaPDF);

    // Configurar tamaño de página (A4)
    printer.setPageSize(QPageSize(QPageSize::A4));
    printer.setPageMargins(QMarginsF(10, 10, 10, 10));

    // 4. Imprimir el documento en PDF
    documento.print(&printer);

    // 5. Opcional: Abrir el PDF con el visor predeterminado
    QProcess::startDetached("xdg-open", QStringList() << rutaPDF);
}
