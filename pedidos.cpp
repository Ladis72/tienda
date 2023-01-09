#include "pedidos.h"
#include "ui_pedidos.h"
#include "imprimirpedido.h"

#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>
#include <QtMath>

extern Configuracion *conf;


pedidos::pedidos(QString idPed, QString proveedor, QString ndoc, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pedidos)
{
    ui->setupUi(this);

    proveedorNombre = proveedor;
    ui->labelProveedor->setText(proveedor);
    ui->labelDocumento->setText(ndoc);
    modeloPedido = new QSqlQueryModel(this);

    lineaSeleccionada="";
    nDoc = ndoc;
    idPedido = idPed;
    idProveedor = base.idProveedor(proveedor, conf->getConexionLocal());
    llenarTablaPedido(idPed);
    editando = false;
}

pedidos::~pedidos()
{
    delete ui;
}

void pedidos::on_leCod_editingFinished()
{

}

QString pedidos::calcularTotalLinea()
{
    double total = ui->lePvt->text().toDouble()*ui->leUds->text().toDouble()*((100-ui->leDescuento->text().toDouble())/100);
    double venta = (ui->leUds->text().toDouble()+ui->leBon->text().toDouble()) * ui->lePvp->text().toDouble();
    double margen = (venta-total)/venta*100;
    ui->leMargen->setText(QString::number(margen,'f',2));
    return QString::number(total);

}

void pedidos::llenarTablaPedido(QString idPedido)
{
    modeloPedido->setQuery(QString("SELECT * FROM lineaspedido_tmp WHERE idPedido = '%1'").arg(idPedido),QSqlDatabase::database(conf->getConexionLocal()));
    ui->tableView->setModel(modeloPedido);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(10);
    ui->tableView->hideColumn(13);
    ui->tableView->hideColumn(14);
    ui->tableView->resizeColumnsToContents();

    contarArticulos();
    contarLineas();
    ui->leIva10->setText(QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido,"10")));
    ui->leIva21->setText(QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido,"21")));
    ui->leIva4->setText(QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido,"4")));
    ui->leIva5->setText(QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido,"5")));
    ui->leIva0->setText(QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido,"0")));
    ui->leRe0->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido,"0")));
    ui->leRe5->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido,"5")));
    ui->leRe4->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido,"4")));
    ui->leRe10->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido,"10")));
    ui->leRe21->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido,"21")));
    ui->leBase0->setText(QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido,"0")));
    ui->leBase4->setText(QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido,"4")));
    ui->leBase5->setText(QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido,"5")));
    ui->leBase10->setText(QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido,"10")));
    ui->leBase21->setText(QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido,"21")));
    ui->leTotal0->setText(QString::number(ui->leBase0->text().toDouble()+ui->leIva0->text().toDouble()+ui->leRe0->text().toDouble()));
    ui->leTotal4->setText(QString::number(ui->leBase4->text().toDouble()+ui->leIva4->text().toDouble()+ui->leRe4->text().toDouble()));
    ui->leTotal5->setText(QString::number(ui->leBase5->text().toDouble()+ui->leIva5->text().toDouble()+ui->leRe5->text().toDouble()));
    ui->leTotal10->setText(QString::number(ui->leBase10->text().toDouble()+ui->leIva10->text().toDouble()+ui->leRe10->text().toDouble()));
    ui->leTotal21->setText(QString::number(ui->leBase21->text().toDouble()+ui->leIva21->text().toDouble()+ui->leRe21->text().toDouble()));
    ui->leTotalBase->setText(QString::number(ui->leBase0->text().toDouble()+ui->leBase4->text().toDouble()+ui->leBase5->text().toDouble()+ui->leBase10->text().toDouble()+ui->leBase21->text().toDouble()));
    ui->leTotalIva->setText(QString::number(ui->leIva0->text().toDouble()+ui->leIva4->text().toDouble()+ui->leIva5->text().toDouble()+ui->leIva10->text().toDouble()+ui->leIva21->text().toDouble()));
    ui->leTotalRe->setText(QString::number(ui->leRe0->text().toDouble()+ui->leRe4->text().toDouble()+ui->leRe5->text().toDouble()+ui->leRe10->text().toDouble()+ui->leRe21->text().toDouble()));
    ui->leTotal->setText(QString::number(ui->leTotalBase->text().toDouble()+ui->leTotalIva->text().toDouble()+ui->leTotalRe->text().toDouble()));

}

void pedidos::borrarLineEdits()
{
    ui->leCod->clear();
    ui->leDescripcion->clear();
    ui->leUds->clear();
    ui->leBon->clear();
    ui->lePvt->clear();
    ui->leDescuento->clear();
    ui->leIva->clear();
    ui->lePvp->clear();
    ui->leTotalLinea->clear();
    ui->dateEdit->setDate(QDate::currentDate());
    ui->leLote->clear();

}

void pedidos::contarLineas()
{

    int lineas = modeloPedido->rowCount();
    ui->labelNLineas->setText(QString::number(lineas));
}

void pedidos::contarArticulos()
{
    float articulos = base.sumarColumna(conf->getConexionLocal(),"lineaspedido_tmp","cantidad","idPedido",idPedido);
    float bonificacion = base.sumarColumna(conf->getConexionLocal(),"lineaspedido_tmp","bonificacion","idPedido",idPedido);
    ui->labelNArticulos->setText(QString::number(articulos+bonificacion));

}

void pedidos::on_leUds_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());

}

void pedidos::on_lePvt_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());
}

void pedidos::on_leIva_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());
}

void pedidos::on_leDescuento_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());
}

void pedidos::on_pushButtonAnadir_clicked()
{
    QStringList datos;
    datos.clear();
    //Comprobar de nuevo que hay un artículo con ese código
    bool existe = base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),"articulos","cod",ui->leCod->text());
    if(existe == true){
        float baseTotal = ui->leTotalLinea->text().toFloat();
        int tipoIva = ui->leIva->text().toInt();
        float iva = baseTotal*tipoIva/100;
        float re;

        if(base.leerConfiguracion() == "1"){

        switch (tipoIva) {
        case 21:
            re = baseTotal*5.2/100;
            break;
        case 10:
            re = baseTotal*1.4/100;
            break;
        case 4:
            re = baseTotal*0.5/100;
            break;
        case 5:
            re = baseTotal*0.625/100;
            break;
        case 0:
            re = 0;
            break;
        default:
            re = 0;
            break;
        };
        }else {re = 0;}
        float baseProducto = ui->leTotalLinea->text().toFloat()/ui->leUds->text().toFloat();

    datos.append(idPedido);
    datos.append(ui->leCod->text());
    datos.append(ui->leDescripcion->text());
    datos.append(ui->leUds->text());
    if (ui->leBon->text().isEmpty()) {
        datos.append("0");
    }else{
        datos.append(ui->leBon->text());
    }
    datos.append(ui->leLote->text());
    datos.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
    datos.append(ui->lePvt->text());
    datos.append(ui->leDescuento->text());
    datos.append(QString::number(baseProducto));
    datos.append(ui->leIva->text());
    datos.append(ui->leTotalLinea->text());
    datos.append(QString::number(iva));
    datos.append(QString::number(re));
    datos.append(ui->lePvp->text());
    if (editando == true) {
        datos.append(lineaSeleccionada);
        base.modificarLineaPedido(conf->getConexionLocal(),datos);
    }else{
        base.grabarLineaPedido(conf->getConexionLocal(), datos);
    }
    llenarTablaPedido(idPedido);
    }else{
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("ERROR");
        msg->setInformativeText("No hay un producto con ese código");
        msg->exec();
    }
    editando = false;

    borrarLineEdits();
    ui->leCod->setFocus();

}

void pedidos::on_leCod_returnPressed()
{
    consulta = base.consulta_producto(conf->getConexionLocal(),ui->leCod->text());
    consulta.first();
    if(!consulta.isValid()){
           QString cod = base.codigoDesdeAux(conf->getConexionLocal(),ui->leCod->text());
           consulta = base.consulta_producto(conf->getConexionLocal(), cod);
           consulta.first();
       }
    if (consulta.numRowsAffected() == 1) {
        ui->leCod->setText(consulta.value(0).toString());
        ui->leDescripcion->setText(consulta.value(1).toString());
        ui->lePvt->setText(consulta.value(12).toString());
        ui->leIva->setText(consulta.value(3).toString());
        ui->lePvp->setText(consulta.value(2).toString());
        ui->leDescuento->setText(base.descuentoProveedor(ui->labelProveedor->text()));
        ui->leUds->setText("1");
        ui->leTotalLinea->setText(calcularTotalLinea());
        ui->leUds->setFocus();
        ui->leUds->selectAll();
    }else{
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("No se encuentra el producto");
        msg->setInformativeText("Desea crearlo?");
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Ok);
        int resp = msg->exec();
        if(resp == QMessageBox::Ok){
            Articulos *articulo = new Articulos;
            articulo->exec();
            articulo->borrarFormulario();

            qDebug() << "Crear producto";
        }else{
            borrarLineEdits();
            ui->leCod->setFocus();
        }
    }
}

void pedidos::on_pushButtonBorrar_clicked()
{
    if(lineaSeleccionada.isEmpty()){
        QMessageBox::information(this,"ATENCIÓN","Primero debe seleccionar una linea para poder borrarla");
        return;
    }
    base.borrarLineaPedido(conf->getConexionLocal(), lineaSeleccionada);
    llenarTablaPedido(idPedido);
}

void pedidos::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloPedido->index(index.row(),0);
    int fila = indice.row(); //Devuelve el número de fila del modelo
    ui->leCod->setText(modeloPedido->data(modeloPedido->index(fila,2)).toString());
    ui->leDescripcion->setText(modeloPedido->data(modeloPedido->index(fila,3)).toString());
    ui->leUds->setText(modeloPedido->data(modeloPedido->index(fila,4)).toString());
    ui->leBon->setText(modeloPedido->data(modeloPedido->index(fila,5)).toString());
    ui->leLote->setText(modeloPedido->data(modeloPedido->index(fila,6)).toString());
    ui->dateEdit->setDate(QDate::fromString(modeloPedido->data(modeloPedido->index(fila,7)).toString(),"yyyy-MM-dd"));
    ui->lePvt->setText(modeloPedido->data(modeloPedido->index(fila,8)).toString());
    ui->leIva->setText(modeloPedido->data(modeloPedido->index(fila,11)).toString());
    ui->lePvp->setText(modeloPedido->data(modeloPedido->index(fila,15)).toString());
    ui->leDescuento->setText(modeloPedido->data(modeloPedido->index(fila,9)).toString());
    ui->leTotalLinea->setText(modeloPedido->data(modeloPedido->index(fila,12)).toString());
    lineaSeleccionada = modeloPedido->data(indice,Qt::EditRole).toString();
    editando = true;

    qDebug() << lineaSeleccionada;
}

void pedidos::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloPedido->index(index.row(),0);
    lineaSeleccionada = modeloPedido->data(indice,Qt::EditRole).toString();
    qDebug() << lineaSeleccionada;
}

void pedidos::on_leDescripcion_returnPressed()
{
    consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),"articulos",ui->leDescripcion->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    ui->leCod->setText(buscar->resultado);
    emit on_leCod_returnPressed();
}


void pedidos::on_pushButtonModificar_clicked()
{
    editando = true;
    emit on_pushButtonAnadir_clicked();
}

void pedidos::on_leIva_editingFinished()
{
    if (ui->leIva->text() != "4" && ui->leIva->text() != "10" && ui->leIva->text() != "21" && ui->leIva->text() != "5" && ui->leIva->text() != "0") {
        QMessageBox::information(this,"Error en el IVA","Ha escrito un IVA diferente de los permitidos");
        ui->leIva->clear();
        ui->leIva->setFocus();
    }
}

void pedidos::on_dateEdit_editingFinished()
{
    if (ui->dateEdit->date() <= QDate::currentDate()) {
        QMessageBox::information(this,"Error en fecha de caducidad","La fecha no puede ser anterior al día de hoy");
        ui->dateEdit->setFocus();
        ui->dateEdit->setDate(QDate::currentDate());
    }
}

void pedidos::on_leBon_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());
}

void pedidos::on_lePvp_textChanged(const QString &arg1)
{
    ui->leTotalLinea->setText(calcularTotalLinea());
}

void pedidos::on_pushButtonImprimir_clicked()
{
    QString tienda = conf->getConexionLocal();
    QStringList cabecera;
    cabecera.clear();
    cabecera << nDoc;
    cabecera << proveedorNombre;
    cabecera << ui->leTotalBase->text();
    cabecera << ui->leTotalIva->text();
    cabecera << ui->leTotalRe->text();
    cabecera << ui->leTotal->text();
    imprimirPedido pedido(tienda , cabecera , modeloPedido);
}
