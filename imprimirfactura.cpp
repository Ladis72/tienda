#include "imprimirfactura.h"
#include <QCoreApplication>
#include <QFile>
#include <QPrinter>
#include <QProcess>
#include <QTextDocument>

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

    totalFactura = consultaTicket.value(8).toString();
    cliente = base.etiquetaCliente(idCliente);
    fPago = base.nombreFormaPago(consultaTicket.value(9).toString(), conf->getConexionLocal());

    consulta = base.consultarLineasTicket(conf->getConexionLocal(), ticket);

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
    body {
        font-family: Arial, sans-serif;
        font-size: 10pt;
        margin: 0;
        padding: 0;
        border: 9;
    }
    .cabecera-superior {
        display: flex;
        justify-content: space-between;
        align-items: flex-start;
        margin: 0;
        padding: 0;
    }
    .numero-factura {
        font-size: 12pt;
        font-weight: bold;
        text-align: center
    }
    .fecha-factura {
        font-size: 12pt;
        font-weight: bold;
        text-align: right;
    }
    table.cabecera {
        width: 100%;
        border: none;
        margin-bottom: 8px;
    }
    td {
        vertical-align: top;
    }
    .lineas {
        width: 100%;
        border-collapse: collapse;
        margin-top: 8px;
        font-size: 10pt;
    }
    .lineas th {
        background-color: #f0f0f0;
        font-weight: bold;
        padding: 5px;
    }
    .lineas th, .lineas td {
        border: 1px solid black;
        padding: 4px;
    }
    .lineas td {
        text-align: right;
    }
    .lineas td:nth-child(2) {
        text-align: left;
    }
    .totales {
        text-align: right;
        margin-top: 12px;
        font-size: 10pt;
        font-weight: bold;
    }
    .seccion {
        margin-bottom: 2px;
    }
    .borde-superior {
        border-top: 2px solid #000;
        padding-top: 6px;
    }
  </style>
</head>
<body>
<!-- Número de factura y fecha en la parte SUPERIOR -->
<div class='cabecera-superior'>
    <div class='numero-factura'>FACTURA Nº %NUM_FACTURA% <br>FECHA: %FECHA%</div>
</div>

<table class='cabecera' style='width: 100%; border-collapse: collapse;'>
  <!-- Logo centrado y ancho -->
  <tr>
    <td colspan='2' style='text-align: center; padding: 3px 0;'>
      <img src=':/imagenes/documentos/Cabecera factura.png' width='500' style='max-width: 100%; height: auto;' alt='Logo Emeicjac'/>
    </td>
  </tr>

  <!-- Información en dos columnas -->
  <tr>
    <td style='width: 50%; vertical-align: top; padding: 7px 10px; border-top: 2px solid #000;'>

      <b>%CLIENTE%<br/>
    </td>

    <td style='width: 50%; vertical-align: top; padding: 15px 10px; border-top: 2px solid #000; text-align: right;'>

      <b>%TIENDA%<br/>

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

    QString datosTiendaLocal = "";
    datosTienda = base.datosTiendaLocal(conf->getConexionLocal());

    for (int i = 1; i < 7; ++i) {
        datosTiendaLocal += datosTienda.at(i) + "<br>";
    }
    html.replace("%TIENDA%", datosTiendaLocal);
    html.replace("%CLIENTE%", cliente);
    html.replace("%NUM_FACTURA%", ticket);
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
        QString descuento = QString::number(consulta.value(7).toDouble(), 'f', 2);
        QString tipoiva = QString::number(consulta.value(5).toDouble(), 'f', 0) + "%";
        QString total = QString::number(consulta.value(8).toDouble(), 'f', 2);

        double baseTMP = consulta.value(8).toDouble() / (1 + (consulta.value(5).toDouble() / 100));
        double ivaTMP = consulta.value(8).toDouble() - baseTMP;

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
    )")
                          .arg(cantidad, descripcion, precio, descuento, tipoiva, total);
    }
    html.replace("%LINEAS%", lineasHTML);

    html.replace("%BASE%", QString::number(totalBase, 'f', 2)) + " €";
    html.replace("%IVA%", QString::number(totalIva, 'f', 2)) + " €";
    html.replace("%TOTAL%", QString::number(totalBase + totalIva, 'f', 2)) + " €";
    documento.setHtml(html);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(base.devolverDirectorio("documentos") + "/Factura.pdf");
    printer.setPageSize(QPageSize::A4);
    printer.setPageMargins(QMargins(15, 15, 15, 15), QPageLayout::Millimeter);
    documento.print(&printer);
    QProcess::startDetached("xdg-open",
                            QStringList() << base.devolverDirectorio("documentos") + "/Factura.pdf");
}
