#include "pedidos.h"
#include "imprimirpedido.h"
#include "ui_pedidos.h"

#include <QDebug>
#include <QInputDialog>
#include <QMessageBox>
#include <QtMath>
#include <QVariant>
#include <QStandardItemModel>

extern Configuracion *conf;

pedidos::pedidos(QString idPed, QString proveedor, QString ndoc, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::pedidos)
{
    ui->setupUi(this);

    proveedorNombre = proveedor;
    ui->labelProveedor->setText(proveedor);
    ui->labelDocumento->setText(ndoc);
    modeloPedido = new QSqlQueryModel(this);

    lineaSeleccionada = "";
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

void pedidos::on_leCod_editingFinished() {}

QString pedidos::calcularTotalLinea()
{
    double total = ui->lePvt->text().toDouble() * ui->leUds->text().toDouble()
                   * ((100 - ui->leDescuento->text().toDouble()) / 100);
    double venta = (ui->leUds->text().toDouble() + ui->leBon->text().toDouble())
                   * ui->lePvp->text().toDouble();
    double margen = (venta - total) / venta * 100;
    ui->leMargen->setText(QString::number(margen, 'f', 2));
    return QString::number(total);
}

// void pedidos::llenarTablaPedido(QString idPedido)
// {
//     modeloPedido
//         ->setQuery(QString("SELECT * FROM lineaspedido_tmp WHERE idPedido = '%1'").arg(idPedido),
//                    QSqlDatabase::database(conf->getConexionLocal()));
//     ui->tableView->setModel(modeloPedido);
//     ui->tableView->hideColumn(0);
//     ui->tableView->hideColumn(1);
//     ui->tableView->hideColumn(10);
//     ui->tableView->hideColumn(13);
//     ui->tableView->hideColumn(14);
//     ui->tableView->resizeColumnsToContents();

//     contarArticulos();
//     contarLineas();
//     ui->leIva10->setText(
//         QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido, "10")));
//     ui->leIva21->setText(
//         QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido, "21")));
//     ui->leIva4->setText(
//         QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido, "4")));
//     ui->leIva5->setText(
//         QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido, "5")));
//     ui->leIva0->setText(
//         QString::number(base.sumarIvasPedido(conf->getConexionLocal(), idPedido, "0")));
//     ui->leRe0->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido, "0")));
//     ui->leRe5->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido, "5")));
//     ui->leRe4->setText(QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido, "4")));
//     ui->leRe10->setText(
//         QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido, "10")));
//     ui->leRe21->setText(
//         QString::number(base.sumarRePedido(conf->getConexionLocal(), idPedido, "21")));
//     ui->leBase0->setText(
//         QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido, "0")));
//     ui->leBase4->setText(
//         QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido, "4")));
//     ui->leBase5->setText(
//         QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido, "5")));
//     ui->leBase10->setText(
//         QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido, "10")));
//     ui->leBase21->setText(
//         QString::number(base.sumarBasesPedido(conf->getConexionLocal(), idPedido, "21")));
//     ui->leTotal0->setText(QString::number(ui->leBase0->text().toDouble()
//                                           + ui->leIva0->text().toDouble()
//                                           + ui->leRe0->text().toDouble()));
//     ui->leTotal4->setText(QString::number(ui->leBase4->text().toDouble()
//                                           + ui->leIva4->text().toDouble()
//                                           + ui->leRe4->text().toDouble()));
//     ui->leTotal5->setText(QString::number(ui->leBase5->text().toDouble()
//                                           + ui->leIva5->text().toDouble()
//                                           + ui->leRe5->text().toDouble()));
//     ui->leTotal10->setText(QString::number(ui->leBase10->text().toDouble()
//                                            + ui->leIva10->text().toDouble()
//                                            + ui->leRe10->text().toDouble()));
//     ui->leTotal21->setText(QString::number(ui->leBase21->text().toDouble()
//                                            + ui->leIva21->text().toDouble()
//                                            + ui->leRe21->text().toDouble()));
//     ui->leTotalBase->setText(
//         QString::number(ui->leBase0->text().toDouble() + ui->leBase4->text().toDouble()
//                         + ui->leBase5->text().toDouble() + ui->leBase10->text().toDouble()
//                         + ui->leBase21->text().toDouble()));
//     ui->leTotalIva->setText(QString::number(
//         ui->leIva0->text().toDouble() + ui->leIva4->text().toDouble() + ui->leIva5->text().toDouble()
//         + ui->leIva10->text().toDouble() + ui->leIva21->text().toDouble()));
//     ui->leTotalRe->setText(QString::number(
//         ui->leRe0->text().toDouble() + ui->leRe4->text().toDouble() + ui->leRe5->text().toDouble()
//         + ui->leRe10->text().toDouble() + ui->leRe21->text().toDouble()));
//     ui->leTotal->setText(QString::number(ui->leTotalBase->text().toDouble()
//                                          + ui->leTotalIva->text().toDouble()
//                                          + ui->leTotalRe->text().toDouble()));
// }

void pedidos::llenarTablaPedido(QString idPedido)
{
    modeloPedido->setQuery(QString("SELECT * FROM lineaspedido_tmp WHERE idPedido = '%1'").arg(idPedido),
                           QSqlDatabase::database(conf->getConexionLocal()));
            ui->tableView->setModel(modeloPedido);
            ui->tableView->hideColumn(0);
            ui->tableView->hideColumn(1);
            ui->tableView->hideColumn(10);
            ui->tableView->hideColumn(13);
            ui->tableView->hideColumn(14);
            ui->tableView->resizeColumnsToContents();

    // Consulta para agrupar por tipo de IVA
    QString queryStr = QString(
                           "SELECT tipoIva, "
                           "SUM(totalbase) AS totalBase, "
                           "SUM(iva) AS totalIva, "
                           "SUM(re) AS totalRe, "
                           "(SUM(totalbase) + SUM(iva) + SUM(re)) AS totalGeneral "
                           "FROM lineaspedido_tmp "
                           "WHERE idPedido = '%1' "
                           "GROUP BY tipoIva").arg(idPedido);

    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    if (!query.exec(queryStr)) {
        QMessageBox::critical(this, "Error", "Error al consultar los datos agrupados por IVA");
        return;
    }

    // Crear un modelo de tabla para mostrar los datos agrupados
    QStandardItemModel *modelo = new QStandardItemModel(this);

    // Definir encabezados
    modelo->setHorizontalHeaderLabels(QStringList() << "Tipo IVA" << "Base Imponible"
                                                    << "Total IVA" << "Total RE"
                                                    << "Total General");

    double totalBase = 0, totalIva = 0, totalRe = 0, totalGeneral = 0;

    // Procesar resultados de la consulta
    while (query.next()) {
        QList<QStandardItem *> fila;

        // Obtener valores
        double tipoIva = query.value("tipoIva").toDouble();
        double base = query.value("totalBase").toDouble();
        double iva = query.value("totalIva").toDouble();
        double re = query.value("totalRe").toDouble();
        double total = query.value("totalGeneral").toDouble();

        // Crear celdas de la fila
        fila << new QStandardItem(QString::number(tipoIva)) // Tipo IVA
             << new QStandardItem(QString::number(base, 'f', 2)) // Base Imponible
             << new QStandardItem(QString::number(iva, 'f', 2)) // IVA
             << new QStandardItem(QString::number(re, 'f', 2)) // Recargo de Equivalencia
             << new QStandardItem(QString::number(total, 'f', 2)); // Total General

        // Agregar fila al modelo
        modelo->appendRow(fila);

        // Sumar totales generales
        totalBase += base;
        totalIva += iva;
        totalRe += re;
        totalGeneral += total;

    }
    //Lenar los totales
    ui->lineEditBase->setText(QString::number(totalBase,'f',2));
    ui->lineEditIVA->setText(QString::number(totalIva,'f',2));
    ui->lineEditRecargo->setText(QString::number(totalRe,'f',2));
    ui->lineEditTotal->setText(QString::number(totalGeneral,'f',2));
    contarArticulos();
    contarLineas();
    // Establecer el modelo en la vista de tabla
    ui->tableViewTotales->setModel(modelo);
    ui->tableViewTotales->resizeColumnsToContents();

    // Mostrar los totales generales en los campos correspondientes
    // ui->leTotalBase->setText(QString::number(totalBase, 'f', 2));
    // ui->leTotalIva->setText(QString::number(totalIva, 'f', 2));
    // ui->leTotalRe->setText(QString::number(totalRe, 'f', 2));
    // ui->leTotal->setText(QString::number(totalGeneral, 'f', 2));
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
    ui->lineEditLineas->setText(QString::number(lineas));
}

void pedidos::contarArticulos()
{
    float articulos = base.sumarColumna(conf->getConexionLocal(),
                                        "lineaspedido_tmp",
                                        "cantidad",
                                        "idPedido",
                                        idPedido);
    float bonificacion = base.sumarColumna(conf->getConexionLocal(),
                                           "lineaspedido_tmp",
                                           "bonificacion",
                                           "idPedido",
                                           idPedido);
    ui->lineEditUnidades->setText(QString::number(articulos + bonificacion));
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
    // QStringList datos;
    // datos.clear();
    // //Comprobar de nuevo que hay un artículo con ese código
    // bool existe = base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
    //                                      "articulos",
    //                                      "cod",
    //                                      ui->leCod->text());
    // if (existe == true) {
    //     float baseTotal = ui->leTotalLinea->text().toFloat();
    //     int tipoIva = ui->leIva->text().toInt();
    //     float iva = baseTotal * tipoIva / 100;
    //     float re;

    //     if (base.leerConfiguracion() == "1") {
    //         switch (tipoIva) {
    //         case 21:
    //             re = baseTotal * 5.2 / 100;
    //             break;
    //         case 10:
    //             re = baseTotal * 1.4 / 100;
    //             break;
    //         case 4:
    //             re = baseTotal * 0.5 / 100;
    //             break;
    //         case 5:
    //             re = baseTotal * 0.625 / 100;
    //             break;
    //         case 0:
    //             re = 0;
    //             break;
    //         default:
    //             re = 0;
    //             break;
    //         };
    //     } else {
    //         re = 0;
    //     }
    //     float baseProducto = ui->leTotalLinea->text().toFloat() / ui->leUds->text().toFloat();

    //     datos.append(idPedido);
    //     datos.append(ui->leCod->text());
    //     datos.append(ui->leDescripcion->text());
    //     datos.append(ui->leUds->text());
    //     if (ui->leBon->text().isEmpty()) {
    //         datos.append("0");
    //     } else {
    //         datos.append(ui->leBon->text());
    //     }
    //     datos.append(ui->leLote->text());
    //     datos.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
    //     datos.append(ui->lePvt->text());
    //     datos.append(ui->leDescuento->text());
    //     datos.append(QString::number(baseProducto));
    //     datos.append(ui->leIva->text());
    //     datos.append(ui->leTotalLinea->text());
    //     datos.append(QString::number(iva));
    //     datos.append(QString::number(re));
    //     datos.append(ui->lePvp->text());
    //     if (editando == true) {
    //         datos.append(lineaSeleccionada);
    //         base.modificarLineaPedido(conf->getConexionLocal(), datos);
    //     } else {
    //         base.grabarLineaPedido(conf->getConexionLocal(), datos);
    //     }
    //     llenarTablaPedido(idPedido);
    // } else {
    //     QMessageBox *msg = new QMessageBox(this);
    //     msg->setText("ERROR");
    //     msg->setInformativeText("No hay un producto con ese código");
    //     msg->exec();
    // }
    // editando = false;

    // borrarLineEdits();
    // ui->leCod->setFocus();

        QStringList datos;
        datos.clear();

        // Verificar que el producto existe
        bool existe = base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                                             "articulos", "cod", ui->leCod->text());
        if (existe == true) {
            // Obtener tipo de IVA desde la entrada del usuario
            QVariant tipoIva = ui->leIva->text();
            double baseTotal = ui->leTotalLinea->text().toDouble();
            double iva = 0.0;
            double re = 0.0;

            // Calcular IVA y RE desde la tabla impuestos
            qDebug() << "Tipo IVA: " << tipoIva << tipoIva.typeName();
            QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
            query.prepare("SELECT tipoIva, porcentaje, recargo FROM impuestos WHERE porcentaje = :porcentaje");
            query.bindValue(":porcentaje", tipoIva);
            query.exec();
            query.first();
            qDebug() << query.lastQuery();
            qDebug() << query.lastError();

            qDebug() << "Base: " << baseTotal << " IVA: " << query.value(1).toDouble();
            iva = baseTotal * query.value(1).toDouble() / 100;
            qDebug() << "Base: " << baseTotal << " IVA: " << query.value(2).toDouble();
            re = baseTotal * query.value(2).toDouble() / 100;


            // Continuar con la creación de la línea del pedido
            double baseProducto = baseTotal / ui->leUds->text().toDouble();
            datos.append(idPedido);
            datos.append(ui->leCod->text());
            datos.append(ui->leDescripcion->text());
            datos.append(ui->leUds->text());
            datos.append(ui->leBon->text().isEmpty() ? "0" : ui->leBon->text());
            datos.append(ui->leLote->text());
            datos.append(ui->dateEdit->date().toString("yyyy-MM-dd"));
            datos.append(ui->lePvt->text());
            datos.append(ui->leDescuento->text());
            datos.append(QString::number(baseProducto));
            datos.append(tipoIva.toString());
            datos.append(ui->leTotalLinea->text());
            datos.append(QString::number(iva));
            datos.append(QString::number(re));
            datos.append(ui->lePvp->text());

            qDebug() << datos;

            if (editando) {
                datos.append(lineaSeleccionada);
                base.modificarLineaPedido(conf->getConexionLocal(), datos);
            } else {
                base.grabarLineaPedido(conf->getConexionLocal(), datos);
            }

            llenarTablaPedido(idPedido);
        } else {
            QMessageBox::information(this, "ERROR", "No hay un producto con ese código.");
        }

        editando = false;
        borrarLineEdits();
        ui->leCod->setFocus();


}

void pedidos::on_leCod_returnPressed()
{
    consulta = base.consulta_producto(conf->getConexionLocal(), ui->leCod->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base.codigoDesdeAux(conf->getConexionLocal(), ui->leCod->text());
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
    } else {
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("No se encuentra el producto");
        msg->setInformativeText("Desea crearlo?");
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Ok);
        int resp = msg->exec();
        if (resp == QMessageBox::Ok) {
            Articulos *articulo = new Articulos;
            articulo->exec();
            articulo->borrarFormulario();

            qDebug() << "Crear producto";
        } else {
            borrarLineEdits();
            ui->leCod->setFocus();
        }
    }
}

void pedidos::on_pushButtonBorrar_clicked()
{
    if (lineaSeleccionada.isEmpty()) {
        QMessageBox::information(this,
                                 "ATENCIÓN",
                                 "Primero debe seleccionar una linea para poder borrarla");
        return;
    }
    base.borrarLineaPedido(conf->getConexionLocal(), lineaSeleccionada);
    llenarTablaPedido(idPedido);
}

void pedidos::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloPedido->index(index.row(), 0);
    int fila = indice.row(); //Devuelve el número de fila del modelo
    ui->leCod->setText(modeloPedido->data(modeloPedido->index(fila, 2)).toString());
    ui->leDescripcion->setText(modeloPedido->data(modeloPedido->index(fila, 3)).toString());
    ui->leUds->setText(modeloPedido->data(modeloPedido->index(fila, 4)).toString());
    ui->leBon->setText(modeloPedido->data(modeloPedido->index(fila, 5)).toString());
    ui->leLote->setText(modeloPedido->data(modeloPedido->index(fila, 6)).toString());
    ui->dateEdit->setDate(
        QDate::fromString(modeloPedido->data(modeloPedido->index(fila, 7)).toString(),
                          "yyyy-MM-dd"));
    ui->lePvt->setText(modeloPedido->data(modeloPedido->index(fila, 8)).toString());
    ui->leIva->setText(modeloPedido->data(modeloPedido->index(fila, 11)).toString());
    ui->lePvp->setText(modeloPedido->data(modeloPedido->index(fila, 15)).toString());
    ui->leDescuento->setText(modeloPedido->data(modeloPedido->index(fila, 9)).toString());
    ui->leTotalLinea->setText(modeloPedido->data(modeloPedido->index(fila, 12)).toString());
    lineaSeleccionada = modeloPedido->data(indice, Qt::EditRole).toString();
    editando = true;

    qDebug() << lineaSeleccionada;
}

void pedidos::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloPedido->index(index.row(), 0);
    lineaSeleccionada = modeloPedido->data(indice, Qt::EditRole).toString();
    qDebug() << lineaSeleccionada;
}

void pedidos::on_leDescripcion_returnPressed()
{
    consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                   "articulos",
                                   ui->leDescripcion->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
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

    // Obtener los tipos de IVA permitidos desde la tabla impuestos
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("SELECT porcentaje FROM impuestos");
    query.exec();

    bool valido = false;
    while (query.next()) {
        qDebug() << query.value(0).toString() << "  " << ui->leIva->text();
        if (ui->leIva->text() == query.value(0).toString()) {
            valido = true;
            break;
        }
    }

    if (!valido) {
        QMessageBox::information(this, "Error en el IVA", "El IVA ingresado no es válido.");
        ui->leIva->clear();
        ui->leIva->setFocus();
    } else {
        ui->leTotalLinea->setText(calcularTotalLinea());  // Actualiza el total con el nuevo IVA
    }
}

void pedidos::on_dateEdit_editingFinished()
{
    if (ui->dateEdit->date() <= QDate::currentDate()) {
        QMessageBox::information(this,
                                 "Error en fecha de caducidad",
                                 "La fecha no puede ser anterior al día de hoy");
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
    cabecera << ui->lineEditBase->text();
    cabecera << ui->lineEditIVA->text();
    cabecera << ui->lineEditRecargo->text();
    cabecera << ui->lineEditTotal->text();
    imprimirPedido pedido(tienda, cabecera, modeloPedido);
}
