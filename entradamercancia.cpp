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
    for (int i = 0; i < mTablaEntradas->rowCount(); ++i) {
        QString cod = mTablaEntradas->record(i).value(1).toString();
        QString fechaCaducidad = mTablaEntradas->record(i).value(5).toString();
        QString uds = mTablaEntradas->record(i).value(4).toString();
        QString idTienda = mTablaEntradas->record(i).value(7).toString();

        // Buscar si hay lotes con unidades pendientes
        QString idLote = base->idLote(conf->getConexionLocal(),
                                      mTablaEntradas->record(i).value(1).toString(),
                                      "",
                                      "2000-01-01");
        qDebug() << "IDLOTE " + idLote;
        if (idLote != "0") {
            int pendientes = base->unidadesLote(conf->getConexionLocal(), idLote);
            qDebug() << "Hay lotes pendientes";
            qDebug() << "Lotes PENDIENTES " + pendientes;
            if (abs(pendientes) > uds.toInt()) {
                qDebug() << "Hay mas pendientes que entradas";
                base->aumentarLote(conf->getConexionLocal(), idLote, uds.toInt());
            } else if (abs(pendientes) == uds.toInt()) {
                qDebug() << "Hay los mismos pendientes que entradas";
                base->ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'",
                                        conf->getConexionLocal());
            } else {
                qDebug() << "Hay mas entradas que pendientes";
                base->ejecutarSentencia("DELETE FROM lotes WHERE id = '" + idLote + "'",
                                        conf->getConexionLocal());
                qDebug() << "ENTRADAS: " + uds;
                qDebug() << "PENDIENTES: " + pendientes;
                int unidades = uds.toInt() + pendientes;
                qDebug() << "DIFERENCIA: " + unidades;

                idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
                if (idLote == "0") {
                    base->crearLote(conf->getConexionLocal(),
                                    cod,
                                    "",
                                    fechaCaducidad,
                                    QString::number(unidades));
                } else {
                    base->aumentarLote(conf->getConexionLocal(), idLote, unidades);
                }
            }
        } else {
            idLote = base->idLote(conf->getConexionLocal(), cod, "", fechaCaducidad);
            if (idLote == "0") {
                base->crearLote(conf->getConexionLocal(), cod, "", fechaCaducidad, uds);
            } else {
                base->aumentarLote(conf->getConexionLocal(), idLote, uds.toInt());
            }
        }

        QString codigo = mTablaEntradas->record(i).value(1).toString();
        QString precio = mTablaEntradas->record(i).value(6).toString();
        QString descripcion = mTablaEntradas->record(i).value(3).toString();
        QSqlQuery cambios = base->consulta_producto("DB", codigo);
        cambios.first();
        QString precioAnterior = cambios.value("pvp").toString();
        QString descripcionAnterior = cambios.value("descripcion").toString();
        if (!(precio == precioAnterior && descripcion == descripcionAnterior)) {
            QSqlQuery tmp = base->ejecutarSentencia("UPDATE articulos SET descripcion = '"
                                                        + descripcion + "' , pvp = " + precio
                                                        + " WHERE cod = '" + codigo + "'",
                                                    conf->getConexionLocal());
            qDebug() << tmp.lastError();
        }
    }
    QSqlQuery tmp = base->ejecutarSentencia(
        "INSERT INTO entradaGenero (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp "
        ", idTienda) "
        "SELECT entradaGenero_tmp.cod, entradaGenero_tmp.fechaEntrada, "
        "entradaGenero_tmp.descripcion, entradaGenero_tmp.cantidad, "
        "entradaGenero_tmp.fechaCaducidad, entradaGenero_tmp.pvp , entradaGenero_tmp.idTienda"
        " FROM entradaGenero_tmp WHERE entradaGenero_tmp.idTienda = "
            + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                        ui->comboBoxProcedencia->currentText())),
        conf->getConexionLocal());
    base->ejecutarSentencia("DELETE FROM entradaGenero_tmp WHERE idTienda = "
                                + QString::number(
                                    base->idTiendaDesdeNombre(QSqlDatabase::database("DB"),
                                                              ui->comboBoxProcedencia
                                                                  ->currentText())),
                            conf->getConexionLocal());
    actualizarTabla();
}

void EntradaMercancia::actualizarTabla()
{
    mTablaEntradas->setFilter(
        "idTienda = "
        + QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                    ui->comboBoxProcedencia->currentText())));
    mTablaEntradas->setSort(0, Qt::AscendingOrder);
    mTablaEntradas->select();
    ui->tableView->setModel(mTablaEntradas);
    ui->tableView->resizeColumnsToContents();
    lineas = mTablaEntradas->rowCount();
    productos = base->sumarColumna(conf->getConexionLocal(),
                                   "entradaGenero_tmp",
                                   "cantidad",
                                   "idTienda",
                                   QString::number(base->idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                                                                                                                                ui->comboBoxProcedencia->currentText())));
    ui->lbProductos->setText("Lineas= "+QString::number(lineas)+"  Productos="+QString::number(productos));
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
