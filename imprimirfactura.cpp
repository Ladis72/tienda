#include "imprimirfactura.h"
#include <QFile>
#include <QTextDocument>
#include <QPrinter>
#include <QCoreApplication>
#include <QProcess>


ImprimirFactura::ImprimirFactura(QString nTicket, QObject *parent)
    : QObject(parent)
{
     consultaTicket = base.datosTicket(conf->getConexionLocal(), nTicket);
     ticket = consultaTicket.value(0).toString();
     fecha = consultaTicket.value(3).toString();
     hora = consultaTicket.value(4).toString();
     total = consultaTicket.value(8).toString();
     idCliente = consultaTicket.value(2).toString();
     idVendedor = consultaTicket.value(1).toString();

     double totalBases = consultaTicket.value(5).toDouble();
     double totalIVAS = consultaTicket.value(6).toDouble();
     totalFactura = consultaTicket.value(8).toString();
     cliente = base.etiquetaCliente(idCliente);
     fPago = base.nombreFormaPago(consultaTicket.value(9).toString(), conf->getConexionLocal());

     consulta = base.consultarLineasTicket(conf->getConexionLocal(), ticket);
     int i = 0;
     modeloTabla = new QStandardItemModel();
    while (consulta.next()) {
        QStandardItem *itemUds = new QStandardItem(consulta.value(4).toString());
        QStandardItem *itemProducto = new QStandardItem(consulta.value(3).toString());
        QStandardItem *itemPrecio = new QStandardItem(consulta.value(6).toString());
        QStandardItem *itemIVA = new QStandardItem(consulta.value(5).toString());
        QStandardItem *itemDto = new QStandardItem(consulta.value(7).toString());
        QStandardItem *itemTotal = new QStandardItem(consulta.value(8).toString());

        modeloTabla->setItem(i, 0, itemUds);
        modeloTabla->setItem(i, 1, itemProducto);
        modeloTabla->setItem(i, 2, itemPrecio);
        modeloTabla->setItem(i, 3, itemIVA);
        modeloTabla->setItem(i, 4, itemDto);
        modeloTabla->setItem(i, 5, itemTotal);

        ++i;
    }

    QMap<QString, double> totalBaseInponible;
    QMap<QString, double> totalIva;
    QMap<QString, double> totalGeneral;
    for (int row = 0; row < modeloTabla->rowCount(); ++row) {
        QString tipoIva = modeloTabla->data(modeloTabla->index(row,3)).toString();
        double porcentajeIva = tipoIva.toDouble()/100;
        double totalConIva = modeloTabla->data(modeloTabla->index(row,5)).toDouble();
        double base = totalConIva/(1+porcentajeIva);
        double iva = totalConIva-base;

        totalBaseInponible[tipoIva] += base;
        totalIva[tipoIva] += iva;
        totalGeneral[tipoIva] += totalConIva;

    }
    qDebug() << "Base " << totalBaseInponible;
    qDebug() << "IVA " << totalIva;
    qDebug() << "Total " << totalGeneral;
    QStandardItemModel *modeloImpuestos = new QStandardItemModel();
    modeloImpuestos->setColumnCount(4);
    modeloImpuestos->setHorizontalHeaderLabels({"Tipo", "Base", "IVA", "Total"});
    for (auto it = totalBaseInponible.begin(); it != totalBaseInponible.end(); ++it) {
        QString tipoIva = it.key();
        QList<QStandardItem*> fila;
        fila.append(new QStandardItem(tipoIva+"%"));
        fila.append(new QStandardItem(QString::number(totalBaseInponible[tipoIva], 'f', 2)));
        fila.append(new QStandardItem(QString::number(totalIva[tipoIva], 'f', 2)));
        fila.append(new QStandardItem(QString::number(totalGeneral[tipoIva], 'f', 2)));
        modeloImpuestos->appendRow(fila);
    }
     QFile fichero("./documentos/FacturaCliente.html");
     fichero.open(QIODevice::WriteOnly);
     QTextStream pagina(&fichero);
     QString html = "<!DOCTYPE html>"
                    "<html>"
                    "<head>"
                    "<meta http-equiv='content-type' content='text/html; charset=UTF-8'>"
                    "<title>Factura clientes</title>"
                    "<link type='text/css' rel='stylesheet' href='main.css'>"
                    "</head>"
                    "<body>"
                    "<table style='width: 100%; height: 339px;' border='0'>"
                    "<thead>"
                    "<tr style='background-color: #45dd94;'>"
                    "<th>Num. factura: "
                    + ticket
                    + "</th>"
                      "<th style=' text-align: center; vertical-align: middle; background-color: "
                      "#45dd94;'>FACTURA</th>"
                      "<th>Fecha: "
                    + fecha
                    + "<br>"
                      "</th>"
                      "</tr>"
                      "</thead>"
                      "<tbody>"
                      "<tr>"
                      "<td style='height: 19px; text-align: center; background-color: white;'>";
     pagina << html;

     datosTienda = base.datosTiendaLocal(conf->getConexionLocal());
     for (int i = 1; i < 7; i++) {
         pagina << datosTienda.at(i) + "<br>";
     }
     pagina << "20.196.639-V</td>";
     pagina << "<td style='text-align: center; background-color: white;'><img src='logo.jpg' "
               "alt='Logo' title='Logo' style='width: 175px; height: 175px;'>emeicjac.com</td>";
     pagina << "<td style='text-align: center; background-color: white;'>" + cliente
                   + "</td>"
                     "</tr>"
                     "</tbody>"
                     "</table>";
     pagina << "<div class='container'>\n"
               "<table class='table'>\n";
     //Cabecera
     pagina << "<thead>"
               "<tr><th>DESCRIPCION</th><th>UDS</th><th>PRECIO</th><th>IVA</th><th>DTO</"
               "th><th>TOTAL</th></tr>"
               "</thead>";
     //Datos tabla
     pagina << "<tbody>";
     pagina << "<tr>";
     for (int i = 0; i < modeloTabla->rowCount(); i++) {
         pagina << "<td>" + modeloTabla->item(i, 1)->text() + "</td>";
         pagina << "<td>" + modeloTabla->item(i, 0)->text() + "</td>";
         pagina << "<td>" + modeloTabla->item(i, 2)->text() + "</td>";
         pagina << "<td>" + modeloTabla->item(i, 3)->text() + "</td>";
         pagina << "<td>" + modeloTabla->item(i, 4)->text() + "</td>";
         pagina << "<td>" + modeloTabla->item(i, 5)->text() + "</td>";
         pagina << "</tr>";
     }
     pagina << "</tbody></table><br>";
     pagina << "<div class='container'>\n"
               "<table class='table' style='width: 70%'>\n";
     //Cabecera
     pagina << "<thead>"
               "<tr><th></th><th>BASE</th><th>IVA</th><th>TOTAL</th></tr>"
               "</thead>";
     pagina << "<tbody>";
     //          "<tr>";
     for (int row = 0; row < modeloImpuestos->rowCount(); ++row) {
         pagina << "<tr>";
         for (int col = 0; col < modeloImpuestos->columnCount(); ++col) {
             QString data = modeloImpuestos->data(modeloImpuestos->index(row,col)).toString();
             pagina << "<td>"+data+"</td>";
         }
         pagina << "</tr>";
     }


    pagina <<        "<tr>"
                     "<td><b>TOTAL"
                     "</td>"
                     "<td><span style = 'color:red;'>"
                   + QString::number(totalBases, 'f',2)
                   + "</span></td>"
                     "<td><span style = 'color:red;'>"
                   + QString::number(totalIVAS, 'f',2)
                   + "</span></td>"
                     "<td><span style = 'color:red;'>"
                   + totalFactura
                   + "</span></td></b>"
                     "</tr>"
                     "</tbody>"
                     "</table>"
                     "<br>"
                     "</div>"
                     "</body>"
                     "</html>";
     fichero.close();
     //system("firefox " + QCoreApplication::applicationDirPath().toLocal8Bit()
     //       + "/documentos/FacturaCliente.html");
     facturaPDF();


}

void ImprimirFactura::facturaPDF()
{

    QTextDocument documento;
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 10pt; }
    .factura { font-size: 14pt; font-weight: bold; text-align: center; margin-bottom: 20px; }
    table.cabecera { width: 100%; border: none; }
    td { vertical-align: top; }
    .cliente { font-size: 9pt; width: 33%; }
    .logo { text-align: center; width: 33%; padding-top: 5px;}
    .tienda { font-size: 9pt; text-align: right; width: 33%; }
    .lineas { width: 100%; border-collapse: collapse; margin-top: 15px; }
    .lineas th, .lineas td { border: 1px solid black; padding: 4px; text-align: left; }
    .totales { text-align: right; margin-top: 20px; }
  </style>
</head>
<body>

<div class='factura'>FACTURA Nº %NUM_FACTURA%   FECHA: %FECHA%</div>

<table class='cabecera'>
  <tr>
    <td class='cliente'>
      <b>Cliente:</b> %CLIENTE%<br/>
    </td>
    <td class='logo'>
      <table style='width: 100%;'>
        <tr>
          <td style='text-align: right;'>
            <img src=':/imagenes/Emeicjac logo.jpg' width='100'/>
          </td>
        </tr>
      </table>
    </td>

    <td class='tienda'>
      <b>Tienda:</b> %TIENDA%<br/>
    </td>
  </tr>

</table>

<table class='lineas'>
  <tr>
    <th>Cantidad</th>
    <th>Descripción</th>
    <th>Precio</th>
    <th>Descuento</th>
    <th>IVA</th>
    <th>Total</th>
  </tr>
  %LINEAS%
</table>

<div class='totales'>
  <p><b>Total base:</b> %BASE%</p>
  <p><b>Total IVA:</b> %IVA%</p>
  <p><b>Total factura:</b> %TOTAL%</p>
</div>

</body>
</html>
)";

    QString datosTiendaLocal ="";
    qDebug() << datosTienda.size();
    for (int i = 1; i < 7; ++i) {
        datosTiendaLocal += datosTienda.at(i)+"<br>";
    }
    html.replace("%TIENDA%", datosTiendaLocal );
    html.replace("%CLIENTE%", cliente);
    html.replace("%NUM_FACTURA%",ticket);
    html.replace("%FECHA%", fecha);
    double totalBase = 0;
    double totalIva = 0;
    QString lineasHTML;
    consulta.first();
    consulta.previous();
    while (consulta.next()) {
        QString cantidad = consulta.value(4).toString();
        QString descripcion = consulta.value(3).toString();
        QString precio = QString::number(consulta.value(6).toDouble(), 'f', 2);
        QString descuento = QString::number(consulta.value(7).toDouble(), 'f' , 2);
        QString tipoiva = QString::number(consulta.value(5).toDouble(), 'f' , 0)+"%";
        QString total = QString::number(consulta.value(8).toDouble(), 'f' ,2);

        double baseTMP = consulta.value(8).toDouble()/(1+ (consulta.value(5).toDouble()/100));
        double ivaTMP = consulta.value(8).toDouble()-baseTMP;

        totalBase += baseTMP;
        totalIva += ivaTMP;



        lineasHTML += QString(R"(
        <tr>
            <td>%1</td>
            <td>%2</td>
            <td>%3 €</td>
            <td>%4 %</td>
            <td>%5</td>
            <td>%6 €</td>
        </tr>
    )").arg(cantidad, descripcion, precio, descuento, tipoiva, total);
    }
    html.replace("%LINEAS%", lineasHTML);

    html.replace("%BASE%", QString::number(totalBase, 'f',2))+" €";
    html.replace("%IVA%", QString::number(totalIva, 'f',2))+ " €";
    html.replace("%TOTAL%", QString::number(totalBase + totalIva, 'f',2))+" €";
    documento.setHtml(html);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(base.devolverDirectorio("factura")+".pdf");
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMargins(15,15,15,15),QPageLayout::Millimeter);
    documento.print(&printer);
    QProcess::startDetached("xdg-open", QStringList() << base.devolverDirectorio("factura")+".pdf");
}


