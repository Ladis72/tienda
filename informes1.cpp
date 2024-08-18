#include "informes1.h"
#include <QCoreApplication>
#include <QFile>

informes1::informes1(QString tienda, QString fechaI, QString fechaF, QStandardItemModel *tabla)
{
    QFile fichero("./documentos/ventas.html");
    fichero.open(QIODevice::WriteOnly);
    QTextStream pagina(&fichero);

    //Cabecera HTML
    QString html = "<!DOCTYPE html>\n"
                   "<HTML>\n"
                   "<HEAD>\n"
                   "<TITLE>Informe de ventas</TITLE>\n"
                   "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>\n"
                   "<link rel='stylesheet' href='main.css'>\n"
                   "</HEAD>\n"
                   "<BODY>\n"
                   "<div class='container'>\n"
                   "<h3 class='text-center' style='text-align: center'>Ventas entre fechas</h3>\n"
                   "<h2 class='text-center' style='text-align: center'>"
                   + tienda
                   + "</h2>"
                     "<h5 class='text-center' style='text-align: center'> DESDE: "
                   + fechaI + "  HASTA: " + fechaF
                   + "</h5>\n"
                     "<table class='table'>\n";
    pagina << html;
    //Cabecera
    pagina << "<thead>"
              "<tr><th> FECHA </th><th> VENTAS </th>";
    if (tabla->columnCount() > 2) {
        pagina << "<th>VENTAS B</th>";
    }
    pagina << "<tr>"
              "</thead>";
    //Tabla HTML
    pagina << "<tbody>";

    double totalA = 0, totalB = 0;
    for (int fila = 0; fila < tabla->rowCount(); fila++) {
        pagina << "<tr class='table-primary'>";
        totalA += tabla->item(fila, 1)->text().toDouble();
        if (tabla->columnCount() > 2) {
            totalB += tabla->item(fila, 2)->text().toDouble();
        }
        for (int col = 0; col < tabla->columnCount(); col++) {
            pagina << "<td>" + tabla->item(fila, col)->text() + "</td>\n";
        }
        pagina << "</tr>\n";
    }
    pagina << "<tr><td></td><td>" + QString::number(totalA) + "</td>";
    if (tabla->columnCount() > 2) {
        pagina << "<td>" + QString::number(totalB) + "</td>";
    }

    pagina << "</tr>";
    if (totalB > 0) {
        double total = totalA + totalB;
        pagina << "<tr><td></td><td></td><td>" + QString::number(total) + "</td></tr>";
    }

    pagina << "</tbody>"
              "</table>\n"
              "</body>"
              "</HTML>\n";

    fichero.close();
    system("firefox " + QCoreApplication::applicationDirPath().toLocal8Bit()
           + "/documentos/ventas.html");
}
