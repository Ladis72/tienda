#include "salidas.h"
#include <QMessageBox>
#include <QInputDialog>
#include "ui_salidas.h"

Salidas::Salidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Salidas)
{
    ui->setupUi(this);
    lineas = 0;
    productos = 0;
    mTablaSalidas = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    ui->comboBoxDestino->blockSignals(true);
    llenarComboTiendas();
    ui->comboBoxDestino->blockSignals(false);

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
    ui->pushButtonBorrar->setAutoDefault(false);
    ui->pushButtonBorrar->setDefault(false);
    ui->pushButtonCerrar->setAutoDefault(false);
    ui->pushButtonCerrar->setDefault(false);

    // Cadena de enfoque mediante ENTER
    connect(ui->lineEditCantidad, &QLineEdit::returnPressed, [this](){
        ui->dateEditFC->setFocus();
    });
    connect(ui->lineEditPrecio, &QLineEdit::returnPressed, this, &Salidas::on_pushButtonAgregar_clicked);

    aplicarPermisos();
}

Salidas::~Salidas()
{
    delete ui;
}

/**
 * @brief Aplica permisos a las acciones del módulo de salidas de mercancía.
 */
void Salidas::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonAgregar->setEnabled(conf->permisos()->tiene("salidas.crear"));
    ui->pushButtonEnviar->setEnabled(conf->permisos()->tiene("salidas.crear"));
    ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("salidas.borrar"));
}

void Salidas::on_lineEditCod_returnPressed()
{

    QString codigo = ui->lineEditCod->text().trimmed();
    if (codigo.isEmpty())
        return;

    QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), codigo);

    // 1. Buscar por código directo
    if (registro.isEmpty()) {
        // 2. Buscar por código auxiliar
        QString codAux = base.codigoDesdeAux(conf->getConexionLocal(), codigo);
        if (!codAux.isEmpty()) {
            registro = base.consulta_producto(conf->getConexionLocal(), codAux);
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
    qDebug() << "Salidas::actualizarTabla - Inicio";
    QString tienda = ui->comboBoxDestino->currentText();
    
    if (tienda.isEmpty()) {
        mTablaSalidas->setFilter("idTienda = -1");
        qDebug() << "Salidas::actualizarTabla - Combo vacío, aplicando filtro idTienda = -1";
    } else {
        int idTienda = base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()), tienda);
        mTablaSalidas->setFilter("idTienda = " + QString::number(idTienda));
        qDebug() << "Salidas::actualizarTabla - Filtrando por idTienda:" << idTienda;
    }
    
    if (!mTablaSalidas->select()) {
        qDebug() << "Salidas::actualizarTabla - Error en select:" << mTablaSalidas->lastError().text();
    }

    // Forzar la carga de todos los registros para asegurar que rowCount() y la vista sean correctos
    int count = 0;
    while (mTablaSalidas->canFetchMore()) {
        mTablaSalidas->fetchMore();
        count++;
        if (count > 100) break; // Seguridad para evitar bucles infinitos en errores de driver
    }
    
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
    qDebug() << "Salidas::actualizarTabla - Fin. Filas cargadas:" << mTablaSalidas->rowCount();
}

void Salidas::llenarComboTiendas()
{
    ui->comboBoxDestino->clear();
    QSqlQuery listaCombo = base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (listaCombo.next()) {
        ui->comboBoxDestino->addItem(listaCombo.value("nombre").toString());
    }
}

void Salidas::on_pushButtonAgregar_clicked()
{
    if (ui->lineEditCantidad->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "La cantidad no puede estar vacía.");
        ui->lineEditCantidad->setFocus();
        return;
    }

    QString codProducto = ui->lineEditCod->text().trimmed();
    QString fechaFC = ui->dateEditFC->text();

    QString idLote = base.idLote(conf->getConexionLocal(),
                                  codProducto,
                                  "",
                                  fechaFC);
    if (idLote == "0") {
        // Consultar fechas/lotes existentes con stock para este producto
        QSqlQuery qFechas(QSqlDatabase::database(conf->getConexionLocal()));
        qFechas.prepare("SELECT DATE_FORMAT(fecha, '%Y-%m-%d') AS f, sum(cantidad) AS cant "
                        "FROM lotes WHERE ean = ? GROUP BY fecha HAVING cant > 0 ORDER BY f ASC");
        qFechas.bindValue(0, codProducto);
        QStringList opcionesFechas;
        if (qFechas.exec()) {
            while (qFechas.next()) {
                opcionesFechas << QString("%1 (Stock: %2)").arg(qFechas.value("f").toString()).arg(qFechas.value("cant").toString());
            }
        }

        if (!opcionesFechas.isEmpty()) {
            bool ok = false;
            QString seleccion = QInputDialog::getItem(
                this,
                "Seleccionar fecha de caducidad",
                "La fecha indicada no coincide con ninguna de las fechas registradas en almacén.\n"
                "Seleccione de qué fecha desea descontar los artículos:",
                opcionesFechas, 0, false, &ok);
            if (ok && !seleccion.isEmpty()) {
                fechaFC = seleccion.split(" ").first();
                ui->dateEditFC->setDate(QDate::fromString(fechaFC, "yyyy-MM-dd"));
                idLote = base.idLote(conf->getConexionLocal(), codProducto, "", fechaFC);
            } else {
                return; // Operación cancelada por el usuario
            }
        } else {
            QMessageBox msgBox;
            msgBox.setText("Parece que no hay en el almacén un producto con esos datos");
            msgBox.setInformativeText(
                "¿Desea continuar con la operación o cancelar y volver a comprobarlo?");
            msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
            int ret = msgBox.exec();
            if (ret == QMessageBox::Cancel) {
                return;
            }
        }
    }
    // Se utiliza una consulta preparada para consultar las existencias del lote de forma segura
    QSqlQuery datosLote(QSqlDatabase::database(conf->getConexionLocal()));
    datosLote.prepare("SELECT cantidad FROM lotes WHERE id = ?");
    datosLote.bindValue(0, idLote);
    datosLote.exec();
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
    datos.append(QString::number(base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                           ui->comboBoxDestino->currentText())));
    base.insertarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "salidaGenero_tmp", datos);

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
    QSqlQuery consulta = base.buscarProducto(QSqlDatabase::database(conf->getConexionLocal()),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, std::move(consulta));
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

        base.disminuirLote(cod, fechaCaducidad, uds);
        QString precioValidado = pvp.isEmpty() ? "0" : pvp;
        // Se parametriza la actualización del artículo para prevenir inyecciones SQL
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("UPDATE articulos SET descripcion = ?, pvp = ? WHERE cod = ?");
        tmp.bindValue(0, descripcion);
        tmp.bindValue(1, precioValidado.toDouble());
        tmp.bindValue(2, cod);
        tmp.exec();
    }
    int idTienda = base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()), ui->comboBoxDestino->currentText());
    // Se parametriza el trasvase masivo de salida de género filtrando por idTienda
    QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
    tmp.prepare("INSERT INTO salidaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                "FROM salidaGenero_tmp WHERE idTienda = ?");
    tmp.bindValue(0, idTienda);
    tmp.exec();

    // Se parametriza el vaciado de la tabla temporal filtrando por idTienda
    QSqlQuery deleteTmpQuery(QSqlDatabase::database(conf->getConexionLocal()));
    deleteTmpQuery.prepare("DELETE FROM salidaGenero_tmp WHERE idTienda = ?");
    deleteTmpQuery.bindValue(0, idTienda);
    deleteTmpQuery.exec();
    base.insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(), "Salida genero ");

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
        // Se parametriza la consulta de borrado de la línea temporal
        QSqlQuery tmp(QSqlDatabase::database(conf->getConexionLocal()));
        tmp.prepare("DELETE FROM salidaGenero_tmp WHERE id = ?");
        tmp.bindValue(0, id);
        tmp.exec();
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
