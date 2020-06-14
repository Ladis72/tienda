#include "imprimirfacturaproveedor.h"

#include <QFile>
#include <QTextStream>
#include <QCoreApplication>


imprimirFacturaProveedor::imprimirFacturaProveedor(QString tienda, QStringList cabecera, QString idFactura)
{
    QFile fichero("./documentos/facturaProveedor.html");
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
                "<h3 class='text-center' style='text-align: center'>FACTURA</h3>\n\n\n"
                "<h2 class='text-center' style='text-align: center'>"+tienda+"</h2>"
                    "<table class='table'>\n"
                "<thead>"
                "<tr><th>Nº. Factura</th><th>Fecha</th><th>Proveedor</th><th>Base</th><th>IVA</th><th>RE</th><th>Total</th><th>Vencimeinto</th></tr>"
                "</thead>";
    pagina << html;
    pagina << "<tr>";
    if (cabecera.isEmpty()) {
        qDebug() << "cabecera vacia";
        cabecera = base->datosFactura(QSqlDatabase::database(tienda),idFactura);
    }
    for (int i = 0; i < cabecera.length()-1 ;i++ ) {
        pagina << "<td>"+cabecera.at(i)+"</td>";
    }
    pagina << "</tr></table>";
    pagina << "</div>";
    pagina << "<div class='container'>\n"
              "<table class='table'>\n";
//Cabecera
    pagina << "<thead>"
              "<tr><th>COD</th><th>DESCRIPCION</th><th>CANTIDAD</th><th>BON</th><th>CADUCIDAD</th><th>COSTO</th><th>DESCUENTO</th><th>I.V.A.</th><th>Total base</th><th>P.V.P.</th></tr>"
              "</thead>";

//Datos tabla
        pagina << "<tbody>";
    QSqlQuery consulta = base->ejecutarSentencia("SELECT * FROM lineaspedido WHERE nDocumento = '"+idFactura+"'");
    consulta.first();
    for (int i = 0 ;i < consulta.numRowsAffected() ;i++ ) {
        pagina << "<tr class='table-primary'>";
        pagina << "<td>"+consulta.record().value(3).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(4).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(5).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(6).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(8).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(9).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(10).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(12).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(13).toString()+"</td>";
        pagina << "<td>"+consulta.record().value(16).toString()+"</td>";

        pagina << "</tr>";
        consulta.next();

    }
    pagina << "</tbody></table></div></body></html>";
    fichero.close();
    system("firefox "+QCoreApplication::applicationDirPath().toLocal8Bit()+"/documentos/facturaProveedor.html");

}
