#include "imprimirfactura.h"
#include <QCoreApplication>
#include <QFile>
#include <QPrinter>
#include <QProcess>
#include <QTextDocument>
#include "verifactuclass.h"

ImprimirFactura::ImprimirFactura(QString nTicket, bool esSerieB, QObject *parent)
    : QObject(parent)
    , esSerieB(esSerieB)
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
    body { font-family: Arial, sans-serif; font-size: 10pt; color: #333; margin: 0; padding: 0; }
    .cabecera-superior { padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 10px; text-align: right; }
    .numero-factura { font-size: 14pt; font-weight: bold; color: #2c3e50; }
    table.cabecera { width: 100%; border: none; margin-bottom: 20px; }
    .lineas { width: 100%; border-collapse: collapse; margin-top: 15px; }
    .lineas th { background-color: #2c3e50; color: white; padding: 8px; font-size: 9pt; }
    .lineas td { border: 1px solid #ccc; padding: 6px; text-align: right; font-size: 9pt; }
    .lineas td:nth-child(2) { text-align: left; }
    .totales { text-align: right; margin-top: 20px; font-size: 11pt; }
    .total-final { font-size: 14pt; color: #e74c3c; font-weight: bold; border-top: 2px solid #2c3e50; padding-top: 10px; }
  </style>
</head>
<body>
<!-- Número de factura y fecha en la parte SUPERIOR -->
<div class='cabecera-superior'>
    <div class='numero-factura'>FACTURA Nº %NUM_FACTURA%</div>
    <div>FECHA: %FECHA%</div>
</div>

<table class='cabecera' style='width: 100%; border-collapse: collapse;'>
  <!-- Logo centrado y ancho -->
  <tr>
    <td colspan='2' style='text-align: center; padding: 10px 0;'>
      <img src=':/imagenes/documentos/Cabecera factura.png' width='600' style='max-width: 100%; height: auto;' alt='Logo Emeicjac'/>
    </td>
  </tr>

  <!-- Información en dos columnas -->
  <tr>
    <td style='width: 50%; vertical-align: top; padding: 15px; background-color: #f9f9f9; border-radius: 5px;'>
      <div style='color: #2c3e50; font-weight: bold; margin-bottom: 5px;'>CLIENTE:</div>
      <b>%CLIENTE%</b>
    </td>

    <td style='width: 50%; vertical-align: top; padding: 15px; text-align: right;'>
      <div style='color: #2c3e50; font-weight: bold; margin-bottom: 5px;'>EMISOR:</div>
      %TIENDA%
    </td>
  </tr>
</table>

<table class='lineas'>
  <thead>
    <tr>
      <th>CANTIDAD</th>
      <th>DESCRIPCIÓN</th>
      <th>PRECIO</th>
      <th>DESC.</th>
      <th>IVA</th>
      <th>TOTAL</th>
    </tr>
  </thead>
  <tbody>
    %LINEAS%
  </tbody>
</table>

<div class='totales'>
  <p>Total Base: <strong>%BASE% €</strong></p>
  <p>Total IVA: <strong>%IVA% €</strong></p>
  <div class='total-final'>TOTAL FACTURA: %TOTAL% €</div>
</div>
%QR_CODE%
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

    html.replace("%BASE%", QString::number(totalBase, 'f', 2));
    html.replace("%IVA%", QString::number(totalIva, 'f', 2));
    html.replace("%TOTAL%", QString::number(totalBase + totalIva, 'f', 2));

    // --- Integración QR de VeriFactu en el PDF ---
    // SEC/F1.4: las facturas serie B (ventas especiales, tabla 'ticketss') no se
    // registran en VeriFactu, así que no deben imprimir el QR tributario.
    VeriFactuConfig vfConfig = verifactuClass::cargarConfiguracion();
    QString qrHtml = "";
    if (vfConfig.modo != 0 && !esSerieB) {
        QDate dateExp = QDate::fromString(fecha, "yyyy-MM-dd");
        QString fechaExpAEAT = dateExp.isValid() ? dateExp.toString("dd-MM-yyyy") : QDate::currentDate().toString("dd-MM-yyyy");
        QString urlCotejo = verifactuClass::generarUrlQR(vfConfig.emisorNif, ticket, fechaExpAEAT, total.toDouble(), vfConfig.entorno);
        QImage qrImage = verifactuClass::generarCodigoQR(urlCotejo);
        
        QString qrPath = base.devolverDirectorio("documentos") + "/verifactu_qr_tmp.png";
        if (!qrPath.isEmpty() && qrImage.save(qrPath)) {
            qrHtml = QString(R"(
            <table style="width: 100%; margin-top: 30px; border-top: 1px solid #ccc; padding-top: 15px;">
              <tr>
                <td style="text-align: center; font-family: Arial, sans-serif; font-size: 8pt; line-height: 1.4;">
                  <div style="font-weight: bold; text-transform: uppercase; margin-bottom: 5px;">QR tributario</div>
                  <img src="%1" width="100" height="100" />
                  <div style="margin-top: 5px;">%2</div>
                </td>
              </tr>
            </table>
            )").arg(qrPath, (vfConfig.modo == 1 ? "Factura verificable en la sede electrónica de la AEAT - VERI*FACTU" : "Factura simplificada"));
        }
    }
    html.replace("%QR_CODE%", qrHtml);

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
