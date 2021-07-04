#include "imprimirpedido.h"

#include <QDebug>
#include <QFile>
#include <QSqlRecord>
#include <QCoreApplication>


imprimirPedido::imprimirPedido(QString tienda, QStringList cabecera, QSqlQueryModel *modelo)
{
    QFile fichero("./documentos/pedido.html");
    fichero.open(QIODevice::WriteOnly);
    QTextStream pagina(&fichero);
    QString html = "<!DOCTYPE html>\n"
                "<HTML>\n"
                       "<HEAD>\n"
                "<TITLE>PEDIDO</TITLE>\n"
                "<meta name = 'viewport' content = 'width=device-width, initial-scale=1'>\n"
                "<link rel='stylesheet' href='main.css'>\n"
                "</HEAD>\n"
                "<BODY>\n"
                   "<div class='container'>\n"
                "<h3 class='text-center' style='text-align: center'>PEDIDO Nº:"+cabecera.at(0)+" de "+cabecera.at(1)+"</h3>\n\n\n"
                "<h2 class='text-center' style='text-align: center'>"+tienda+"</h2>"
                    "<table class='table'>\n"
                "<thead>"
                "<tr><th>Base</th><th>IVA</th><th>RE</th><th>Total</th></tr>"
                "</thead>";;
    pagina << html;
    pagina << "<tr>";
    for (int i = 2;i < cabecera.length() ;i++ ) {
        pagina << "<td>"+cabecera.at(i)+"</td>";
    }
    pagina << "</tr></table>";
    pagina << "</div>";
    pagina << "<div class='container'>\n"
              "<table class='table'>\n";
//Cabecera
    pagina << "<thead>"
              "<tr><th>COD</th><th>DESCRIPCION</th><th>CANTIDAD</th><th>BON</th><th>CADUCIDAD</th><th>COSTO</th><th>DESCUENTO</th><th>I.V.A.</th><th>P.V.P.</th></tr>"
              "</thead";
//Datos tabla
    pagina << "<tbody>";
    for (int i = 0; i < modelo->rowCount() ;i++ ) {
        pagina << "<tr class='table-primary'>";
        pagina << "<td>"+modelo->record(i).value("cod").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("descripcion").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("cantidad").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("bonificacion").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("fc").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("costo").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("descuento1").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("tipoIva").toString()+"</td>";
        pagina << "<td>"+modelo->record(i).value("pvp").toString()+"</td>";
        pagina << "</tr>";

    }
    pagina << "</tbody></table></div></body></html>";
    fichero.close();
    system("firefox "+QCoreApplication::applicationDirPath().toLocal8Bit()+"/documentos/pedido.html");


}
