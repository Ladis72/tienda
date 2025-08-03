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
    ui->tableView->hideColumn(0);
    connect(mTablaEntradas, &QAbstractItemModel::dataChanged, this, &EntradaMercancia::actualizarTotales);

    llenarComboTiendas();
    actualizarTabla();
    codSeleccionado = "";
}

EntradaMercancia::~EntradaMercancia()
{
    delete ui;
}

void EntradaMercancia::on_pushButtonAceptar_clicked()
{
    const int idTienda = base->idTiendaDesdeNombre(
        QSqlDatabase::database(conf->getConexionLocal()),
        ui->comboBoxProcedencia->currentText());

    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        procesarLineaEntrada(mTablaEntradas->record(i));
    }
    base->insertarLog(conf->getConexionLocal(),"Info",conf->getUsuario(),"Entrada genero ");

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
    mTablaEntradas->setSort(3, Qt::AscendingOrder);
    mTablaEntradas->select();
    ui->tableView->setModel(mTablaEntradas);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    actualizarTotales();
    }

void EntradaMercancia::llenarComboTiendas()
{
    QSqlQuery listaCombo = base->tiendas(QSqlDatabase::database("DB"));
    listaCombo.first();
    do {
        ui->comboBoxProcedencia->addItem(listaCombo.value("nombre").toString());
    } while (listaCombo.next());
}

void EntradaMercancia::on_lineEditCod_returnPressed()
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
        ui->lineEditPVP->setText(consulta.value(2).toString());
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
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void EntradaMercancia::on_pushButtonBorrar_clicked()
{
    if (!codSeleccionado.isEmpty()) {
        QSqlQuery tmp = base->ejecutarSentencia("DELETE FROM entradaGenero_tmp WHERE id = '"
                                                    + codSeleccionado + "'",
                                                conf->getConexionLocal());
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}

void EntradaMercancia::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = mTablaEntradas->index(index.row(), 0);
    codSeleccionado = mTablaEntradas->data(indice, Qt::EditRole).toString();
    qDebug() << codSeleccionado;
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

void EntradaMercancia::on_comboBoxProcedencia_activated(const QString &arg1)
{
    actualizarTabla();
}

void EntradaMercancia::actualizarTotales()
{
    lineas = mTablaEntradas->rowCount();
    productos = 0;
    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        QModelIndex idx = mTablaEntradas->index(i,4);
        double valor = mTablaEntradas->data(idx).toDouble();
        productos += valor;
    }
    ui->lbProductos->setText("Lineas= "+QString::number(lineas)+"  Productos="+QString::number(productos));

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
                base->ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'", conf->getConexionLocal());
                int nuevasUnidades = uds + pendientes;

                idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
                if (idLote == "0") {
                    base->crearLote(conf->getConexionLocal(), cod, "", fechaCaducidad, QString::number(nuevasUnidades));
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

void EntradaMercancia::actualizarArticulo(const QString &cod, const QString &descripcion, const QString &precio)
{
    QSqlQuery producto = base->consulta_producto(conf->getConexionLocal(), cod);
    if (producto.first()) {
        QString descAnt = producto.value("descripcion").toString();
        QString pvpAnt = producto.value("pvp").toString();

        if (descripcion != descAnt || precio != pvpAnt) {
            base->ejecutarSentencia(
                QString("UPDATE articulos SET descripcion = '%1', pvp = %2 WHERE cod = '%3'")
                    .arg(descripcion, precio, cod),
                conf->getConexionLocal()
                );
        }
    }
}

void EntradaMercancia::guardarArticulo(int idTienda)
{
    base->ejecutarSentencia(QString("INSERT INTO entradaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                                    "SELECT cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda "
                                    "FROM entradaGenero_tmp WHERE idTienda = %1").arg(idTienda),
                            conf->getConexionLocal());
}

void EntradaMercancia::limpiarTabla(int idTienda)
{
    base->ejecutarSentencia(QString("DELETE FROM entradaGenero_tmp WHERE idTienda = %1").arg(idTienda),
    conf->getConexionLocal());
}
