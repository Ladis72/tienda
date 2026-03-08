#include "gestionpedidos.h"
#include <QMessageBox>
#include "ui_gestionpedidos.h"

GestionPedidos::GestionPedidos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GestionPedidos)
{
    ui->setupUi(this);
    borrarVariables();
    llenarTablaPedidos();
}

GestionPedidos::~GestionPedidos()
{
    delete ui;
}

void GestionPedidos::on_pushButtonNuevo_clicked()
{
    PedidoNuevo *pedido = new PedidoNuevo;
    pedido->exec();
    llenarTablaPedidos();
}

void GestionPedidos::llenarTablaPedidos()
{
    listaPedidos.clear();
    QSqlQuery consultaPedidos = base->recuperarPedidos(conf->getConexionLocal());
    consultaPedidos.first();
    QList<QStandardItem *> listaItems;
    for (int i = 0; i < consultaPedidos.numRowsAffected(); i++) {
        listaItems.clear();
        QString idPedido = consultaPedidos.value(0).toString();
        //qDebug() << idPedido;
        QStandardItem *itemId = new QStandardItem(idPedido);
        listaItems.append(itemId);
        QStandardItem *itemProveedor = new QStandardItem(
            base->nombreProveedor(consultaPedidos.value(1).toString(), conf->getConexionLocal()));
        listaItems.append(itemProveedor);
        QStandardItem *itemAlbaran = new QStandardItem(consultaPedidos.value(2).toString());
        listaItems.append(itemAlbaran);
        QStandardItem *itemFecha = new QStandardItem(consultaPedidos.value(3).toString());
        listaItems.append(itemFecha);
        QStandardItem *itemLineas = new QStandardItem(QString::number(
            base->contarLineas("lineaspedido_tmp", conf->getConexionLocal(), "idPedido", idPedido)));
        listaItems.append(itemLineas);
        float articulos = base->sumarColumna(conf->getConexionLocal(),
                                             "lineaspedido_tmp",
                                             "cantidad",
                                             "idPedido",
                                             idPedido);
        float bonificacion = base->sumarColumna(conf->getConexionLocal(),
                                                "lineaspedido_tmp",
                                                "bonificacion",
                                                "idPedido",
                                                idPedido);
        QStandardItem *itemArticulos = new QStandardItem(QString::number(articulos + bonificacion));
        listaItems.append(itemArticulos);
        float baseArticulos = base->sumarColumna(conf->getConexionLocal(),
                                                 "lineaspedido_tmp",
                                                 "totalbase",
                                                 "idPedido",
                                                 idPedido);
        QStandardItem *itemBase = new QStandardItem(QString::number(baseArticulos));
        listaItems.append(itemBase);
        float iva = base->sumarColumna(conf->getConexionLocal(),
                                       "lineaspedido_tmp",
                                       "iva",
                                       "idPedido",
                                       idPedido);
        QStandardItem *itemIva = new QStandardItem(QString::number(iva));
        listaItems.append(itemIva);
        float re = base->sumarColumna(conf->getConexionLocal(),
                                      "lineaspedido_tmp",
                                      "re",
                                      "idPedido",
                                      idPedido);
        QStandardItem *itemRe = new QStandardItem(QString::number(re));
        listaItems.append(itemRe);
        QStandardItem *itemTotal = new QStandardItem(QString::number(baseArticulos + iva + re));
        listaItems.append(itemTotal);
        listaPedidos.appendRow(listaItems);
        consultaPedidos.next();
    }
    QStringList cabeceras;
    cabeceras << "ID" << "PROVEEDOR" << "PEDIDO" << "FECHA" << "LINEAS" << "ARICULOS" << "BASE"
              << "IVA" << "RE" << "TOTAL";
    listaPedidos.setHorizontalHeaderLabels(cabeceras);

    ui->tableView->setModel(&listaPedidos);
    ui->tableView->hideColumn(0);
}

void GestionPedidos::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = listaPedidos.index(index.row(), 0);
    pedidoSeleccionado = listaPedidos.data(indice, Qt::EditRole).toString();

    indice = listaPedidos.index(index.row(), 2);
    docSeleccionado = listaPedidos.data(indice, Qt::EditRole).toString();
    indice = listaPedidos.index(index.row(), 1);
    proveedor = listaPedidos.data(indice, Qt::EditRole).toString();
    indice = listaPedidos.index(index.row(), 3);
    fecha = listaPedidos.data(indice, Qt::EditRole).toString();
}

void GestionPedidos::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex indice = listaPedidos.index(index.row(), 0);
    pedidoSeleccionado = listaPedidos.data(indice, Qt::EditRole).toString();
    indice = listaPedidos.index(index.row(), 2);
    docSeleccionado = listaPedidos.data(indice, Qt::EditRole).toString();
    indice = listaPedidos.index(index.row(), 1);
    proveedor = listaPedidos.data(indice, Qt::EditRole).toString();
    indice = listaPedidos.index(index.row(), 3);
    fecha = listaPedidos.data(indice, Qt::EditRole).toString();
}

void GestionPedidos::on_pushButtonBorrar_clicked()
{
    if (pedidoSeleccionado.isEmpty()) {
        QMessageBox::information(this,
                                 "ATENCIÓN",
                                 "Primero debe seleccionar un pedido para poder borrarlo");
        return;
    }
    QMessageBox msgBox;
    msgBox.setText("Borrar documento");
    msgBox.setInformativeText(
        "¿Quiere borrar el documento?.\nEsta operación no se puede deshacer ");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    if (msgBox.exec() == QMessageBox::Ok) {
        base->borrarPedido(conf->getConexionLocal(), pedidoSeleccionado);
        llenarTablaPedidos();
        pedidoSeleccionado.clear();
    }
}

void GestionPedidos::on_pushButtonGestionar_clicked()
{
    if (pedidoSeleccionado.isEmpty()) {
        QMessageBox::information(this,
                                 "ATENCIÓN",
                                 "Primero debe seleccionar un pedido para poder borrarlo");
        return;
    }
    pedidos *gestPedido = new pedidos(pedidoSeleccionado, proveedor, docSeleccionado, this);
    gestPedido->exec();
    llenarTablaPedidos();
    borrarVariables();
}

void GestionPedidos::borrarVariables()
{
    pedidoSeleccionado.clear();
    docSeleccionado.clear();
    fecha.clear();
}

void GestionPedidos::on_pushButtonAceptar_clicked()
{
    if (pedidoSeleccionado.isEmpty()) {
        QMessageBox::information(this,
                                 "ATENCIÓN",
                                 "Primero debe seleccionar un pedido para poder borrarlo");
        return;
    }
    AceptarPedido *aPedido = new AceptarPedido(pedidoSeleccionado,
                                               proveedor,
                                               docSeleccionado,
                                               fecha,
                                               this);
    aPedido->exec();
    if (aPedido->correcto) {
        qDebug() << "Pedido procesado";
    }
    if (!aPedido->correcto) {
        qDebug() << "Pedido no procesado";
    }
    llenarTablaPedidos();
}

void GestionPedidos::on_tableView_doubleClicked(const QModelIndex &index)
{
    emit on_pushButtonGestionar_clicked();
}
