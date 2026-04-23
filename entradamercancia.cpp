#include "entradamercancia.h"
#include "ui_entradamercancia.h"

#include <QMessageBox>

EntradaMercancia::EntradaMercancia(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EntradaMercancia)
{
    ui->setupUi(this);
    lineas = 0;
    productos = 0;
    mTablaEntradas = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    mTablaEntradas->setTable("entradaGenero_tmp");
    ui->tableView->setModel(mTablaEntradas);
    ui->tableView->setSortingEnabled(true);
    mTablaEntradas->setSort(3, Qt::AscendingOrder); // Orden por descripción por defecto

    connect(mTablaEntradas, &QSqlTableModel::dataChanged, this, &EntradaMercancia::actualizarTotales);

    llenarComboTiendas();
    actualizarTabla();

    // Desactivar autoDefault para evitar inserciones accidentales al pulsar ENTER
    ui->pushButtonAgregarLinea->setAutoDefault(false);
    ui->pushButtonAgregarLinea->setDefault(false);
    ui->pushButtonAceptar->setAutoDefault(false);
    ui->pushButtonAceptar->setDefault(false);

    // Cadena de enfoque mediante ENTER
    connect(ui->lineEditUds, &QLineEdit::returnPressed, [this](){
        ui->dateEditCaducidad->setFocus();
    });
    connect(ui->lineEditPVP, &QLineEdit::returnPressed, this, &EntradaMercancia::on_pushButtonAgregarLinea_clicked);
}

EntradaMercancia::~EntradaMercancia()
{
    delete ui;
}

void EntradaMercancia::on_pushButtonAceptar_clicked()
{
    int idTienda = base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                             ui->comboBoxProcedencia->currentText());

    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        procesarLineaEntrada(mTablaEntradas->record(i));
    }
    base->insertarLog(conf->getConexionLocal(), "Info", conf->getUsuario(), "Entrada genero ");

    guardarArticulo(idTienda);
    limpiarTabla(idTienda);
    actualizarTabla();
}

void EntradaMercancia::actualizarTabla()
{
    mTablaEntradas->setFilter(
        "idTienda = "
        + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                     ui->comboBoxProcedencia->currentText())));
    mTablaEntradas->select();

    // Forzar la carga de todos los registros para asegurar que rowCount() y la vista sean correctos
    while (mTablaEntradas->canFetchMore()) {
        mTablaEntradas->fetchMore();
    }

    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
}

void EntradaMercancia::llenarComboTiendas()
{
    QSqlQuery listaCombo = base->tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    listaCombo.first();
    do {
        ui->comboBoxProcedencia->addItem(listaCombo.value("nombre").toString());
    } while (listaCombo.next());
}

void EntradaMercancia::on_lineEditCod_returnPressed()
{
    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), ui->lineEditCod->text());
    
    if (registro.isEmpty()) {
        QString cod = base->codigoDesdeAux(conf->getConexionLocal(), ui->lineEditCod->text());
        registro = base->consulta_producto("DB", cod);
    }
    
    if (!registro.isEmpty()) {
        ui->lineEditCod->setText(registro.value("cod").toString());
        ui->lineEditDesc->setText(registro.value("descripcion").toString());
        ui->lineEditPVP->setText(registro.value("pvp").toString());
        ui->lineEditUds->setFocus();
    } else {
        QMessageBox msg(this);
        msg.setText("No se encuentra el producto");
        msg.setInformativeText("Desea crearlo?");
        msg.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg.setDefaultButton(QMessageBox::Ok);
        int resp = msg.exec();
        if (resp == QMessageBox::Ok) {
            Articulos articulo;
            articulo.exec();
            articulo.borrarFormulario();

            qDebug() << "Crear producto";
        } else {
            ui->lineEditCod->setFocus();
            ui->lineEditCod->selectAll();
        }
    }
}

void EntradaMercancia::on_pushButtonAgregarLinea_clicked()
{
    if (ui->lineEditUds->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, "Error", "La cantidad no puede estar vacía.");
        ui->lineEditUds->setFocus();
        return;
    }
    
    if (ui->lineEditPVP->text().trimmed().isEmpty()) {
        ui->lineEditPVP->setText("0");
    }
    
    QStringList datos;
    datos.clear();
    datos.append(ui->lineEditCod->text());
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(ui->lineEditDesc->text());
    datos.append(ui->lineEditUds->text());
    datos.append(ui->dateEditCaducidad->text());
    datos.append(ui->lineEditPVP->text());
    datos.append(QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                           ui->comboBoxProcedencia->currentText())));
    base->insertarEnTabla(QSqlDatabase::database("DB"), "entradaGenero_tmp", datos);

    //mTablaEntradas->select();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditPVP->clear();
    ui->lineEditUds->clear();
    ui->lineEditCod->setFocus();
    actualizarTabla();
}

void EntradaMercancia::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database("DB"),
                                              "articulos",
                                              ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this, std::move(consulta));
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void EntradaMercancia::on_pushButtonBorrar_clicked()
{
    QModelIndex index = ui->tableView->currentIndex();
    if (!index.isValid()) {
        return;
    }

    // Obtenemos el ID de la columna 0 de la fila seleccionada actualmente
    QString id = mTablaEntradas->data(mTablaEntradas->index(index.row(), 0)).toString();

    if (!id.isEmpty()) {
        QSqlQuery tmp = base->ejecutarSentencia("DELETE FROM entradaGenero_tmp WHERE id = '"
                                                    + id + "'",
                                                conf->getConexionLocal());
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}

void EntradaMercancia::on_dateEditCaducidad_editingFinished()
{
    if (ui->dateEditCaducidad->date() <= QDate::currentDate()) {
        QMessageBox::information(this,
                                 "Error en la fecha de caducidad",
                                 "La fecha no puede ser anterior a la fecha de hoy");
        ui->dateEditCaducidad->setFocus();
        ui->dateEditCaducidad->setDate(QDate::currentDate());
    }
}

// void EntradaMercancia::on_comboBoxProcedencia_activated(const QString &arg1)
// {
//     actualizarTabla();
// }

void EntradaMercancia::actualizarTotales()
{
    lineas = mTablaEntradas->rowCount();
    productos = 0;
    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        QModelIndex idx = mTablaEntradas->index(i, 4);
        double valor = mTablaEntradas->data(idx).toDouble();
        productos += valor;
    }
    ui->lbProductos->setText("Lineas= " + QString::number(lineas)
                             + "  Productos=" + QString::number(productos));
}

void EntradaMercancia::procesarLineaEntrada(const QSqlRecord &registro)
{
    const QString cod = registro.value("cod").toString();
    const QString fechaCaducidad = registro.value("fechaCaducidad").toString();
    const int uds = registro.value("cantidad").toInt();
    const QString descripcion = registro.value("descripcion").toString();
    const QString precio = registro.value("pvp").toString();

    QString idLote = base->idLote(conf->getConexionLocal(), cod, "", "2000-01-01");

    if (idLote != "0") {
        int pendientes = base->unidadesLote(conf->getConexionLocal(), idLote);

        if (abs(pendientes) > uds) {
            base->aumentarLote(conf->getConexionLocal(), idLote, uds);
        } else {
            base->ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'",
                                    conf->getConexionLocal());
            int nuevasUnidades = uds + pendientes;

            idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
            if (idLote == "0") {
                base->crearLote(conf->getConexionLocal(),
                                cod,
                                "",
                                fechaCaducidad,
                                QString::number(nuevasUnidades));
            } else {
                base->aumentarLote(conf->getConexionLocal(), idLote, nuevasUnidades);
            }
        }
    } else {
        idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
        if (idLote == "0") {
            base->crearLote(conf->getConexionLocal(), cod, "", fechaCaducidad, QString::number(uds));
        } else {
            base->aumentarLote(conf->getConexionLocal(), idLote, uds);
        }
    }

    actualizarArticulo(cod, descripcion, precio);
}

void EntradaMercancia::actualizarArticulo(const QString &cod,
                                          const QString &descripcion,
                                          const QString &precio)
{
    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), cod);
    if (!registro.isEmpty()) {
        QString descAnt = registro.value("descripcion").toString();
        QString pvpAnt = registro.value("pvp").toString();

        if (descripcion != descAnt || precio != pvpAnt) {
            QString precioValidado = precio.isEmpty() ? "0" : precio;
            base->ejecutarSentencia(
                QString("UPDATE articulos SET descripcion = '%1', pvp = %2 WHERE cod = '%3'")
                    .arg(descripcion, precioValidado, cod),
                conf->getConexionLocal());
        }
    }
}

void EntradaMercancia::guardarArticulo(int idTienda)
{
    base->ejecutarSentencia(
        QString("INSERT INTO entradaGenero (cod, fechaEntrada, descripcion, cantidad, "
                "fechaCaducidad, pvp, idTienda) "
                "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                "FROM entradaGenero_tmp WHERE idTienda = %1")
            .arg(idTienda),
        conf->getConexionLocal());
}

void EntradaMercancia::limpiarTabla(int idTienda)
{
    base->ejecutarSentencia(QString("DELETE FROM entradaGenero_tmp WHERE idTienda = %1")
                                .arg(idTienda),
                            conf->getConexionLocal());
}

void EntradaMercancia::on_comboBoxProcedencia_currentIndexChanged(int)
{
    actualizarTabla();
    qDebug() << "Current index changed";
}

void EntradaMercancia::on_pushButtonSalir_clicked()
{
    close();
}
