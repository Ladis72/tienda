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
    mTablaSalidas = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    llenarComboTiendas();
    mTablaSalidas->setTable("salidaGenero_tmp");
    ui->tableView->setModel(mTablaSalidas);
    ui->tableView->setSortingEnabled(true);
    mTablaSalidas->setSort(3, Qt::AscendingOrder); // Orden por descripción por defecto
    connect(mTablaSalidas, &QAbstractItemModel::dataChanged, this, &Salidas::actualizarTotales);

    actualizarTabla();

    // Desactivar autoDefault para evitar inserciones accidentales al pulsar ENTER
    ui->pushButtonAgregar->setAutoDefault(false);
    ui->pushButtonAgregar->setDefault(false);
    ui->pushButtonEnviar->setAutoDefault(false);
    ui->pushButtonEnviar->setDefault(false);

    // Cadena de enfoque mediante ENTER
    connect(ui->lineEditCantidad, &QLineEdit::returnPressed, [this](){
        ui->dateEditFC->setFocus();
    });
    connect(ui->lineEditPrecio, &QLineEdit::returnPressed, this, &Salidas::on_pushButtonAgregar_clicked);
}

Salidas::~Salidas()
{
    delete ui;
}

void Salidas::on_lineEditCod_returnPressed()
{
    // consulta = base->consulta_producto(conf->getConexionLocal(), ui->lineEditCod->text());
    // consulta.first();
    // if (!consulta.isValid()) {
    //     QString cod = base->codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
    //     consulta = base->consulta_producto(conf->getConexionLocal(), cod);
    //     consulta.first();
    // }
    // if (consulta.numRowsAffected() == 1) {
    //     ui->lineEditCod->setText(consulta.value(0).toString());
    //     ui->lineEditDesc->setText(consulta.value(1).toString());
    //     ui->lineEditPrecio->setText(consulta.value(2).toString());
    //     ui->lineEditCantidad->setFocus();
    // } else {
    //     QMessageBox msg(this);
    //     msg.setText("No se encuentra el producto");
    //     msg.setInformativeText("Desea crearlo?");
    //     msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    //     msg.setDefaultButton(QMessageBox::Ok);
    //     int resp = msg.exec();
    //     if (resp == QMessageBox::Ok) {
    //         Articulos *articulo = new Articulos;
    //         articulo->exec();
    //         articulo->borrarFormulario();

    //         qDebug() << "Crear producto";
    //     } else {
    //         ui->lineEditCod->setFocus();
    //         ui->lineEditCod->selectAll();
    //     }
    // }

    QString codigo = ui->lineEditCod->text().trimmed();
    if (codigo.isEmpty())
        return;

    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), codigo);

    // 1. Buscar por código directo
    if (registro.isEmpty()) {
        // 2. Buscar por código auxiliar
        QString codAux = base->codigoDesdeAux(conf->getConexionLocal(), codigo);
        if (!codAux.isEmpty()) {
            registro = base->consulta_producto(conf->getConexionLocal(), codAux);
        }
    }

    // 3. Si hay producto válido
    if (!registro.isEmpty()) {
        ui->lineEditCod->setText(registro.value("cod").toString());
        ui->lineEditDesc->setText(registro.value("descripcion").toString());
        ui->lineEditPrecio->setText(registro.value("pvp").toString());
        ui->lineEditCantidad->setFocus();
        return;
    }

    // 4. No existe → preguntar creación
    QMessageBox msg(this);
    msg.setText("No se encuentra el producto");
    msg.setInformativeText("¿Desea crearlo?");
    msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msg.setDefaultButton(QMessageBox::Ok);

    if (msg.exec() == QMessageBox::Ok) {
        Articulos articulo(this);
        articulo.exec();
        articulo.borrarFormulario();
    } else {
        ui->lineEditCod->setFocus();
        ui->lineEditCod->selectAll();
    }
}

void Salidas::actualizarTabla()
{
    mTablaSalidas->setFilter(
        "idTienda = "
        + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                     ui->comboBoxDestino->currentText())));
    mTablaSalidas->select();

    // Forzar la carga de todos los registros para asegurar que rowCount() y la vista sean correctos
    while (mTablaSalidas->canFetchMore()) {
        mTablaSalidas->fetchMore();
    }

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
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
    if (ui->lineEditCantidad->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "La cantidad no puede estar vacía.");
        ui->lineEditCantidad->setFocus();
        return;
    }

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
    if (ui->lineEditPrecio->text().trimmed().isEmpty()) {
        ui->lineEditPrecio->setText("0");
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
        QString precioValidado = pvp.isEmpty() ? "0" : pvp;
        QSqlQuery tmp = base->ejecutarSentencia("UPDATE articulos SET descripcion = '" + descripcion
                                                    + "' , pvp = " + precioValidado + " WHERE cod = '" + cod
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
    base->insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(), "Salida genero ");

    actualizarTabla();
}

void Salidas::on_pushButtonBorrar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    // Obtenemos el ID de la columna 0 de la fila seleccionada actualmente
    QString id = mTablaSalidas->data(mTablaSalidas->index(index.row(), 0)).toString();

    if (!id.isEmpty()) {
        QSqlQuery tmp = base->ejecutarSentencia("DELETE FROM salidaGenero_tmp WHERE id = '"
                                                    + id + "'",
                                                conf->getConexionLocal());
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}


void Salidas::actualizarTotales()
{
    lineas = mTablaSalidas->rowCount();
    productos = 0;
    for (int i = 0; i < mTablaSalidas->rowCount(); ++i) {
        QModelIndex idx = mTablaSalidas->index(i, 4);
        double valor = mTablaSalidas->data(idx).toDouble();
        productos += valor;
    }
    ui->lbSalidas->setText("Lineas= " + QString::number(lineas)
                           + "  Productos=" + QString::number(productos));
}

void Salidas::on_comboBoxDestino_currentIndexChanged(int)
{
    actualizarTabla();
    qDebug() << "Current index changed";
}

void Salidas::on_pushButtonCerrar_clicked()
{
    close();
}
