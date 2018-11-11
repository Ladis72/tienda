#include "imprimirfactura.h"
#include "qtrpt.h"

ImprimirFactura::ImprimirFactura(QString nTicket, QObject *parent) : QObject(parent)
{
    consulta = base.datosTicket(nTicket);
    ticket = consulta.value(0).toString();
    fecha = consulta.value(3).toString();
    hora = consulta.value(4).toString();
    total = consulta.value(12).toString();
    idCliente = consulta.value(2).toString();
    idVendedor = consulta.value(1).toString();
    base21 = consulta.value(9).toString();
    iva21 = consulta.value(10).toString();
    base10 = consulta.value(7).toString();
    iva10 = consulta.value(8).toString();
    base4 = consulta.value(5).toString();
    iva4 = consulta.value(6).toString();
    totalFactura = consulta.value(12).toString();
    cliente = base.etiquetaCliente(idCliente);
    fPago = base.nombreFormaPago(consulta.value(13).toString());
    consulta = base.consultarLineasTicket(ticket);
    int i = 0;
    modeloTabla = new QStandardItemModel();
    while (consulta.next()) {
        QStandardItem *itemUds = new QStandardItem(consulta.value(4).toString());
        QStandardItem *itemProducto = new QStandardItem(consulta.value(3).toString());
        QStandardItem *itemPrecio = new QStandardItem(consulta.value(6).toString());
        QStandardItem *itemIVA = new QStandardItem(consulta.value(5).toString());
        QStandardItem *itemDto = new QStandardItem(consulta.value(7).toString());
        QStandardItem *itemTotal = new QStandardItem(consulta.value(8).toString());


        modeloTabla->setItem(i,0,itemUds);
        modeloTabla->setItem(i,1,itemProducto);
        modeloTabla->setItem(i,2,itemPrecio);
        modeloTabla->setItem(i,3,itemIVA);
        modeloTabla->setItem(i,4,itemDto);
        modeloTabla->setItem(i,5,itemTotal);

        ++i;
    }
    QtRPT *informe = new QtRPT();
    informe->recordCount.append(modeloTabla->rowCount());

    QString informeDir = base.devolverDirectorio("factura");
    informe->loadReport(informeDir);
    connect(informe, &QtRPT::setValue, [&](const int recNo,
            const QString paramName,
            QVariant &paramValue,
            const int reportPage) {
        (void) reportPage;
        if(paramName == "fecha"){
            paramValue = fecha;
        }
        if(paramName == "cliente"){
            paramValue = cliente;
        }
        if(paramName == "cantidad"){
            paramValue = modeloTabla->item(recNo,0)->text();
        }
        if(paramName == "producto"){
            paramValue = modeloTabla->item(recNo,1)->text();
        }
        if(paramName == "iva"){
            paramValue = modeloTabla->item(recNo,3)->text();
        }
        if(paramName == "precio"){
            paramValue = modeloTabla->item(recNo,2)->text();
        }
        if(paramName == "dto"){
            paramValue = modeloTabla->item(recNo,4)->text();
        }
        if(paramName == "totalLinea"){
            paramValue = modeloTabla->item(recNo,5)->text();
        }
        if(paramName == "nFactura"){
            paramValue = ticket;
        }
        if(paramName == "base21"){
            paramValue = base21;
        }
        if(paramName == "IVA21"){
            paramValue = iva21;
        }
        if(paramName == "base10"){
            paramValue = base10;
        }
        if(paramName == "IVA10"){
            paramValue = iva10;
        }if(paramName == "base4"){
            paramValue = base4;
        }
        if(paramName == "IVA4"){
            paramValue = iva4;
        }
        if(paramName == "total21"){
            paramValue = base21.toDouble()+iva21.toDouble();
        }
        if(paramName == "total10"){
            paramValue = base10.toDouble()+iva10.toDouble();
        }
        if(paramName == "total4"){
            paramValue = base4.toDouble()+iva4.toDouble();
        }
        if(paramName == "totalBase"){
            paramValue = base21.toDouble()+base10.toDouble()+base4.toDouble();
        }
        if(paramName == "totalIVA"){
            paramValue = iva4.toDouble()+iva21.toDouble()+iva10.toDouble();
        }
        if(paramName == "total"){
            paramValue = totalFactura;
        }
    });
    informe->printExec();
}
