#include "salidas.h"
#include <QMessageBox>
#include "ui_salidas.h"

Salidas::Salidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Salidas)
{
    ui->setupUi(this);
    lineas = 0;
    productos = 0;
    llenarComboTiendas();
    mTablaSalidas = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    mTablaSalidas->setTable("salidaGenero_tmp");
    ui->tableView->hideColumn(0);
    actualizarTabla();
    codSeleccionado = "";
}

Salidas::~Salidas()
{
    delete ui;
}

void Salidas::on_lineEditCod_returnPressed()
{
    consulta = base->consulta_producto("DB", ui->lineEditCod->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base->codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
        consulta = base->consulta_producto("DB", cod);
        consulta.first();
    }
    if (consulta.numRowsAffected() == 1) {
        ui->lineEditCod->setText(consulta.value(0).toString());
        ui->lineEditDesc->setText(consulta.value(1).toString());
        ui->lineEditPrecio->setText(consulta.value(2).toString());
        ui->lineEditCantidad->setFocus();
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
            ui->lineEditCod->setFocus();
            ui->lineEditCod->selectAll();
        }
    }
}

void Salidas::actualizarTabla()
{
    mTablaSalidas->setFilter(
        "idTienda = "
        + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                    ui->comboBoxDestino->currentText())));
    mTablaSalidas->setSort(0, Qt::AscendingOrder);
    mTablaSalidas->select();
    ui->tableView->setModel(mTablaSalidas);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    lineas = mTablaSalidas->rowCount();
    productos = base->sumarColumna(conf->getConexionLocal(),
                                   "salidaGenero_tmp",
                                   "cantidad",
                                   "idTienda",
                                   QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                                             ui->comboBoxDestino->currentText())));
    ui->lbSalidas->setText("Lineas= "+QString::number(lineas)+"  Productos="+QString::number(productos));
}

void Salidas::llenarComboTiendas()
{
    QSqlQuery listaCombo = base->tiendas(QSqlDatabase::database("DB"));
    listaCombo.first();
    do {
        ui->comboBoxDestino->addItem(listaCombo.value("nombre").toString());
    } while (listaCombo.next());
}

void Salidas::on_pushButtonAgregar_clicked()
{
    QString idLote = base->idLote(conf->getConexionLocal(),
                                  ui->lineEditCod->text(),
                                  "",
                                  ui->dateEditFC->text());
    if (idLote == "0") {
        QMessageBox msgBox;
        msgBox.setText("Parece que no hay en el almacen un producto con esos datos");
        msgBox.setInformativeText(
            "¿Deseas continual con la operación o Cancelar y volver a comprobarlo?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            qDebug() << "cancelado";
            return;
            break;
        default:
            break;
        }
    }
    QSqlQuery datosLote;
    datosLote = base->ejecutarSentencia("SELECT cantidad FROM lotes WHERE id ='" + idLote + "'",
                                        conf->getConexionLocal());
    datosLote.first();
    int resto = datosLote.record().value("cantidad").toInt() - ui->lineEditCantidad->text().toInt();
    if (resto < 0) {
        QMessageBox msgBox;
        msgBox.setText("Parece que no hay en el almacen suficiente producto con esos datos");
        msgBox.setInformativeText(
            "¿Deseas continual con la operación o Cancelar y volver a comprobarlo?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            qDebug() << "cancelado";
            return;
            break;
        default:
            break;
        }
    }
    QStringList datos;
    datos.clear();
    datos.append(ui->lineEditCod->text());
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(ui->lineEditDesc->text());
    datos.append(ui->lineEditCantidad->text());
    datos.append(ui->dateEditFC->text());
    datos.append(ui->lineEditPrecio->text());
    datos.append(QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                           ui->comboBoxDestino->currentText())));
    base->insertarEnTabla(QSqlDatabase::database("DB"), "salidaGenero_tmp", datos);

    //    mTablaSalidas->select();
    ui->lineEditCantidad->clear();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditPrecio->clear();
    ui->lineEditCod->setFocus();
    //    ui->tableView->resizeColumnsToContents();
    actualizarTabla();
}

void Salidas::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database("DB"),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void Salidas::on_pushButtonEnviar_clicked()
{
    QMessageBox msgBox;
    msgBox.setText("Los datos serán prodesados");
    msgBox.setInformativeText("¿Deseas continual? \nNo se puede deshacer");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
    int ret = msgBox.exec();
    switch (ret) {
    case QMessageBox::Cancel:
        qDebug() << "cancelado";
        return;
    default:
        break;
    }
    for (int i = 0; i < mTablaSalidas->rowCount(); ++i) {
        QString cod, descripcion, pvp, fechaCaducidad, idTienda;
        int uds;
        cod = mTablaSalidas->record(i).value(1).toString();
        descripcion = mTablaSalidas->record(i).value(3).toString();
        pvp = mTablaSalidas->record(i).value(6).toString();
        fechaCaducidad = mTablaSalidas->record(i).value(5).toString();
        uds = mTablaSalidas->record(i).value(4).toInt();
        idTienda = mTablaSalidas->record(i).value(7).toString();

        base->disminuirLote(cod, fechaCaducidad, uds);
        QSqlQuery tmp = base->ejecutarSentencia("UPDATE articulos SET descripcion = '" + descripcion
                                                    + "' , pvp = " + pvp + " WHERE cod = '" + cod
                                                    + "'",
                                                conf->getConexionLocal());
    }
    QSqlQuery tmp = base->ejecutarSentencia(
        "INSERT INTO salidaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp , "
        "idTienda) "
        "SELECT salidaGenero_tmp.cod, salidaGenero_tmp.fechaEntrada, salidaGenero_tmp.descripcion, "
        "salidaGenero_tmp.cantidad, salidaGenero_tmp.fechaCaducidad, salidaGenero_tmp.pvp , "
        "salidaGenero_tmp.idTienda"
        " FROM salidaGenero_tmp WHERE salidaGenero_tmp.idTienda = "
            + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                        ui->comboBoxDestino->currentText())),
        conf->getConexionLocal());
    base->ejecutarSentencia("DELETE FROM salidaGenero_tmp WHERE idTienda = "
                                + QString::number(
                                    base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                              ui->comboBoxDestino->currentText())),
                            conf->getConexionLocal());
    actualizarTabla();
}

void Salidas::on_pushButtonBorrar_clicked()
{
    if (!codSeleccionado.isEmpty()) {
        QSqlQuery tmp = base->ejecutarSentencia("DELETE FROM salidaGenero_tmp WHERE id = '"
                                                    + codSeleccionado + "'",
                                                conf->getConexionLocal());
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}

void Salidas::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = mTablaSalidas->index(index.row(), 0);
    codSeleccionado = mTablaSalidas->data(indice, Qt::EditRole).toString();
    qDebug() << codSeleccionado;
}

void Salidas::on_comboBoxDestino_activated(const QString &arg1)
{
    actualizarTabla();
    qDebug() << "Current index chaned";
}

void Salidas::on_pushButtonActualizar_clicked()
{
    actualizarTabla();
}
