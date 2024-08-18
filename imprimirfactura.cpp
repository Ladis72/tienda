#include "imprimirfactura.h"
#include <QFile>
//#include "qtrpt.h"

#include <QCoreApplication>

ImprimirFactura::ImprimirFactura(QString nTicket, QObject *parent)
    : QObject(parent)
{
    consulta = base.datosTicket(conf->getConexionLocal(), nTicket);
    ticket = consulta.value(0).toString();
    fecha = consulta.value(3).toString();
    hora = consulta.value(4).toString();
    total = consulta.value(16).toString();
    idCliente = consulta.value(2).toString();
    idVendedor = consulta.value(1).toString();
    base21 = consulta.value(13).toString();
    iva21 = consulta.value(14).toString();
    double t21 = consulta.value(13).toDouble() + consulta.value(14).toDouble();
    base10 = consulta.value(11).toString();
    iva10 = consulta.value(12).toString();
    double t10 = consulta.value(11).toDouble() + consulta.value(12).toDouble();
    base5 = consulta.value(9).toString();
    iva5 = consulta.value(10).toString();
    double t5 = consulta.value(9).toDouble() + consulta.value(10).toDouble();
    base4 = consulta.value(5).toString();
    iva4 = consulta.value(6).toString();
    double t4 = consulta.value(5).toDouble() + consulta.value(6).toDouble();
    base0 = consulta.value(5).toString();
    iva0 = "0";
    double t0 = base0.toDouble();
    double totalBases = base21.toDouble() + base10.toDouble() + base5.toDouble() + base4.toDouble()
                        + base0.toDouble();
    double totalIVAS = iva21.toDouble() + iva10.toDouble() + iva5.toDouble() + iva4.toDouble();
    totalFactura = consulta.value(16).toString();
    cliente = base.etiquetaCliente(idCliente);
    fPago = base.nombreFormaPago(consulta.value(13).toString(), conf->getConexionLocal());
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
    //    QtRPT *informe = new QtRPT();
    //    informe->recordCount.append(modeloTabla->rowCount());

    //    QString informeDir = base.devolverDirectorio("factura");
    //    informe->loadReport(informeDir);
    //    connect(informe, &QtRPT::setValue, [&](const int recNo,
    //            const QString paramName,
    //            QVariant &paramValue,
    //            const int reportPage) {
    //        (void) reportPage;
    //        if(paramName == "fecha"){
    //            paramValue = fecha;
    //        }
    //        if(paramName == "cliente"){
    //            paramValue = cliente;
    //        }
    //        if(paramName == "cantidad"){
    //            paramValue = modeloTabla->item(recNo,0)->text();
    //        }
    //        if(paramName == "producto"){
    //            paramValue = modeloTabla->item(recNo,1)->text();
    //        }
    //        if(paramName == "iva"){
    //            paramValue = modeloTabla->item(recNo,3)->text();
    //        }
    //        if(paramName == "precio"){
    //            paramValue = modeloTabla->item(recNo,2)->text();
    //        }
    //        if(paramName == "dto"){
    //            paramValue = modeloTabla->item(recNo,4)->text();
    //        }
    //        if(paramName == "totalLinea"){
    //            paramValue = modeloTabla->item(recNo,5)->text();
    //        }
    //        if(paramName == "nFactura"){
    //            paramValue = ticket;
    //        }
    //        if(paramName == "base21"){
    //            paramValue = base21;
    //        }
    //        if(paramName == "IVA21"){
    //            paramValue = iva21;
    //        }
    //        if(paramName == "base10"){
    //            paramValue = base10;
    //        }
    //        if(paramName == "IVA10"){
    //            paramValue = iva10;
    //        }if(paramName == "base4"){
    //            paramValue = base4;
    //        }
    //        if(paramName == "IVA4"){
    //            paramValue = iva4;
    //        }
    //        if(paramName == "total21"){
    //            paramValue = base21.toDouble()+iva21.toDouble();
    //        }
    //        if(paramName == "total10"){
    //            paramValue = base10.toDouble()+iva10.toDouble();
    //        }
    //        if(paramName == "total4"){
    //            paramValue = base4.toDouble()+iva4.toDouble();
    //        }
    //        if(paramName == "totalBase"){
    //            paramValue = base21.toDouble()+base10.toDouble()+base4.toDouble();
    //        }
    //        if(paramName == "totalIVA"){
    //            paramValue = iva4.toDouble()+iva21.toDouble()+iva10.toDouble();
    //        }
    //        if(paramName == "total"){
    //            paramValue = totalFactura;
    //        }
    //    });
    //    informe->printExec();
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

    QStringList datosTienda = base.datosTiendaLocal(conf->getConexionLocal());
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
    pagina << "<tbody>"
              "<tr>"
              "<td>IVA 21"
              "</td>"
              "<td>"
                  + base21
                  + "</td>"
                    "<td>"
                  + iva21
                  + "</td>"
                    "<td>"
                  + QString::number(t21)
                  + "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>IVA 10"
                    "</td>"
                    "<td>"
                  + base10
                  + "</td>"
                    "<td>"
                  + iva10
                  + "</td>"
                    "<td>"
                  + QString::number(t10)
                  + "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>IVA 5"
                    "</td>"
                    "<td>"
                  + base5
                  + "</td>"
                    "<td>"
                  + iva5
                  + "</td>"
                    "<td>"
                  + QString::number(t5)
                  + "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>IVA 4"
                    "</td>"
                    "<td>"
                  + base4
                  + "</td>"
                    "<td>"
                  + iva4
                  + "</td>"
                    "<td>"
                  + QString::number(t4)
                  + "</td>"
                    "</tr>"
                    "<tr>"
                    "<td>IVA 0"
                    "</td>"
                    "<td>"
                  + base0
                  + "</td>"
                    "<td>"
                  + iva0
                  + "</td>"
                    "<td>"
                  + QString::number(t0)
                  + "</td>"
                    "</tr>"
                    "<tr>"
                    "<td><b>TOTAL"
                    "</td>"
                    "<td><span style = 'color:red;'>"
                  + QString::number(totalBases)
                  + "</span></td>"
                    "<td><span style = 'color:red;'>"
                  + QString::number(totalIVAS)
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
    system("firefox " + QCoreApplication::applicationDirPath().toLocal8Bit()
           + "/documentos/FacturaCliente.html");
}
