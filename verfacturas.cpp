#include "verfacturas.h"
#include "ui_verfacturas.h"
#include "imprimirfacturaproveedor.h"
#include <QMessageBox>
#include <QStandardItemModel>


VerFacturas::VerFacturas(QString docType, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VerFacturas)
{
    ui->setupUi(this);
    tipoDocumento = docType;
    llenarProveedores();
    llenarTabla();
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());
    idFactura="";
}

VerFacturas::~VerFacturas()
{
    delete ui;
}

void VerFacturas::llenarProveedores()
{
    ui->comboBoxProceedores->addItems(base->listadoProveedores());
}

void VerFacturas::llenarTabla()
{
    QString sentenciaSql;
    fechaInicial = ui->dateEditDesde->text();
    fechaFinal = ui->dateEditHasta->text();
    if (ui->dateEditDesde->date() > ui->dateEditHasta->date()) {
        QMessageBox msg;
        msg.setText("Error en la fecha");
        msg.setInformativeText("La fecha inicial no puede ser posterior a la final");
        msg.exec();
        return;
    }
    if (ui->checkBoxTodosProveedores->isChecked()) {
        if(tipoDocumento == "facturas"){
        sentenciaSql = "SELECT * FROM facturas WHERE fechaFactura >= '"+fechaInicial+"' AND fechaFactura <= '"+fechaFinal+"'";
        }else{
            sentenciaSql = "SELECT * FROM albaranes WHERE fechaFactura >= '"+fechaInicial+"' AND fechaFactura <= '"+fechaFinal+"'";
        }
    }else{
        if(tipoDocumento == "facturas"){
        idProveedor = base->idProveedor(ui->comboBoxProceedores->currentText());
        sentenciaSql = "SELECT * FROM facturas WHERE idProveedor = '"+idProveedor+"' AND fechaFactura >= '"+fechaInicial+"' AND fechaFactura <= '"+fechaFinal+"'";
        }else{
            idProveedor = base->idProveedor(ui->comboBoxProceedores->currentText());
            sentenciaSql = "SELECT * FROM albaranes WHERE idProveedor = '"+idProveedor+"' AND fechaFactura >= '"+fechaInicial+"' AND fechaFactura <= '"+fechaFinal+"'";
        }}
    modeloTabla = new QStandardItemModel(this);
    QSqlQuery resultado = base->ejecutarSentencia(sentenciaSql);
    qDebug() << resultado.lastError() << resultado.numRowsAffected();
    resultado.first();
    for (int i = 0; i < resultado.numRowsAffected(); ++i) {
        QStandardItem *itemPedido = new QStandardItem(resultado.value("nFactura").toString());
        modeloTabla->setItem(i,0,itemPedido);
        QStandardItem *itemFecha = new QStandardItem(resultado.value("fechaFactura").toString());
        modeloTabla->setItem(i,1,itemFecha);
        QStandardItem *itemProveedor = new QStandardItem(base->nombreProveedor(resultado.value("idProveedor").toString()));
        modeloTabla->setItem(i,2,itemProveedor);
        QStandardItem *itemBase = new QStandardItem(resultado.value("totalBase").toString());
        modeloTabla->setItem(i,3,itemBase);
        QStandardItem *itemIva = new QStandardItem(resultado.value("totalIva").toString());
        modeloTabla->setItem(i,4,itemIva);
        QStandardItem *itemRe = new QStandardItem(resultado.value("totalRe").toString());
        modeloTabla->setItem(i,5,itemRe);
        QStandardItem *itemTotal = new QStandardItem(resultado.value("total").toString());
        modeloTabla->setItem(i,6,itemTotal);
        if(tipoDocumento == "facturas"){
        QStandardItem *itemFechaV = new QStandardItem(resultado.value("vencimiento").toString());
        modeloTabla->setItem(i,7,itemFechaV);
        QStandardItem *itemPagado = new QStandardItem(resultado.value("pagada").toString());
        modeloTabla->setItem(i,8,itemPagado);
        }else{
            QStandardItem *itemFacturada = new QStandardItem(resultado.value("facturada").toString());
            modeloTabla->setItem(i,7,itemFacturada);
        }

        resultado.next();
    }
    QStringList etiquetas;

    if(tipoDocumento == "facturas"){
    etiquetas << "Nº Factura" << "Fecha" << "Proveedor" << "Base" << "I.V.A" << "R.E." << "Total" << "Fecha vencimiento" << "Pagada";
    }else{
        etiquetas << "Nº Factura" << "Fecha" << "Proveedor" << "Base" << "I.V.A" << "R.E." << "Total" << "Facturada";

    }
    modeloTabla->setHorizontalHeaderLabels(etiquetas);
    ui->tableView->setModel(modeloTabla);
    ui->tableView->resizeColumnsToContents();

}


void VerFacturas::on_checkBoxTodosProveedores_clicked(bool checked)
{
    if (checked == false) {
        ui->comboBoxProceedores->setEnabled(true);
    } else {ui->comboBoxProceedores->setDisabled(true);
    }
}

void VerFacturas::on_pushButtonVer_clicked()
{
    idFactura="";

    llenarTabla();
}



void VerFacturas::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTabla->index(index.row(),0);
    idFactura = modeloTabla->data(indice,Qt::EditRole).toString();
    if (idFactura == "") {
        return;
    }

    imprimirFacturaProveedor factura(conf->getConexionLocal(),datos,idFactura);

//    VisorFacturas *visor = new VisorFacturas(idFactura,this);
//    visor->show();
}

void VerFacturas::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTabla->index(index.row(),0);
    idFactura = modeloTabla->data(indice,Qt::EditRole).toString();
    datos.clear();
    for (int i = 0 ; i < modeloTabla->columnCount() ;i++ ) {
        indice = modeloTabla->index(index.row(),i);
        datos << modeloTabla->data(indice,Qt::EditRole).toString();
    }


}

void VerFacturas::on_pushButtonVerFactura_clicked()
{
    if (idFactura == "") {
        return;
    }

    imprimirFacturaProveedor factura(conf->getConexionLocal(),datos,idFactura);
//    VisorFacturas *visor = new VisorFacturas(idFactura,this);
//    visor->show();
}
