#include "proveedores.h"
#include <QMessageBox>
#include "ui_proveedores.h"

Proveedores::Proveedores(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Proveedores)
{
    ui->setupUi(this);
    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod, 0);
    mapper.addMapping(ui->lineEditNombre, 1);
    mapper.addMapping(ui->lineEditNIF, 2);
    mapper.addMapping(ui->lineEditDireccion, 3);
    mapper.addMapping(ui->lineEditCP, 4);
    mapper.addMapping(ui->lineEditLocalidad, 5);
    mapper.addMapping(ui->lineEditProvincia, 6);
    mapper.addMapping(ui->lineEditRepresentante, 7);
    mapper.addMapping(ui->lineEditTelefonoR, 8);
    mapper.addMapping(ui->lineEditMailR, 9);
    mapper.addMapping(ui->lineEditTelefono, 10);
    mapper.addMapping(ui->lineEditMail, 11);
    mapper.addMapping(ui->lineEditDescuento, 12);
    mapper.addMapping(ui->lineEditFechaUltimaCompra, 13);
    mapper.addMapping(ui->lineEditFormaPago, 14);
    mapper.addMapping(ui->plainTextEdit, 15);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());
    ui->lineEditCod->installEventFilter(this);
    borrarFormulario();
    cargarCompras();
}

Proveedores::~Proveedores()
{
    delete ui;
}

void Proveedores::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM proveedores", QSqlDatabase::database("DB"));
    qDebug() << modeloTabla->lastError().text();
    mapper.setModel(modeloTabla);
}

void Proveedores::borrarFormulario()
{
    foreach (QLineEdit *le, ui->tab->findChildren<QLineEdit *>()) {
        le->clear();
    }
    ui->plainTextEdit->clear();
    ui->labelFPago->clear();
    ui->labelNombre->clear();
}

bool Proveedores::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditCod) {
        if (event->type() == QEvent::MouseButtonPress) {
            borrarFormulario();
            return true;
        }
    }
    return false;
}

void Proveedores::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
    ui->labelNombre->setText(ui->lineEditNombre->text());
    cargarCompras();
}

QStringList Proveedores::recogerDatosFormulario()
{
    QStringList listaDatosFormulario;
    listaDatosFormulario.append(ui->lineEditCod->text());
    listaDatosFormulario.append(ui->lineEditNombre->text());
    listaDatosFormulario.append(ui->lineEditNIF->text());
    listaDatosFormulario.append(ui->lineEditDireccion->text());
    listaDatosFormulario.append(ui->lineEditCP->text());
    listaDatosFormulario.append(ui->lineEditLocalidad->text());
    listaDatosFormulario.append(ui->lineEditProvincia->text());
    listaDatosFormulario.append(ui->lineEditRepresentante->text());
    listaDatosFormulario.append(ui->lineEditTelefonoR->text());
    listaDatosFormulario.append(ui->lineEditMailR->text());
    listaDatosFormulario.append(ui->lineEditTelefono->text());
    listaDatosFormulario.append(ui->lineEditMail->text());
    listaDatosFormulario.append(ui->lineEditDescuento->text());
    if (ui->lineEditFechaUltimaCompra->text().isEmpty()) {
        listaDatosFormulario.append("2000-01-01");
    } else {
        listaDatosFormulario.append(ui->lineEditFechaUltimaCompra->text());
    }
    listaDatosFormulario.append(ui->lineEditFormaPago->text());
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());

    return listaDatosFormulario;
}

void Proveedores::cargarCompras()
{
    modeloCompras.clear();
    if (ui->radioButtonComprasFacturas->isChecked()) {
        modeloCompras.setQuery("SELECT * FROM facturas WHERE idProveedor = '"
                                   + ui->lineEditCod->text() + "' ORDER BY fechaFactura DESC",
                               QSqlDatabase::database("DB"));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonComprasMeses->isChecked()) {
        modeloCompras.setQuery("SELECT year(fechaFactura) , month(fechaFactura) , sum(total) FROM "
                               "facturas where idProveedor = '"
                                   + ui->lineEditCod->text()
                                   + "' group by year(fechaFactura) desc,"
                                     " month(fechaFactura) desc",
                               QSqlDatabase::database("DB"));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
    if (ui->radioButtonComprasAnos->isChecked()) {
        modeloCompras.setQuery(
            "SELECT year(fechaFactura) , sum(total) FROM facturas where idProveedor = '"
                + ui->lineEditCod->text() + "' group by year(fechaFactura) desc",
            QSqlDatabase::database("DB"));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();
    }
}

void Proveedores::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_F11) {
        ui->pushButtonBorrar->setEnabled(true);
    }
}

void Proveedores::on_pushButtonNuevo_clicked()
{
    if (base.existeDatoEnTabla(QSqlDatabase::database("DB"),
                               "proveedores",
                               "idProveedor",
                               ui->lineEditCod->text())) {
        QMessageBox::warning(this, "ATENCION", "El registro ya existe");
        return;
    }
    QStringList datos = recogerDatosFormulario();
    if (base.crearProveedor(QSqlDatabase::database("DB"), datos)) {
        QMessageBox::about(this, "Atención", "Proveedor creado con éxito");
    } else {
        QMessageBox::warning(this, "Error", "No se ha podido crear el proveedor.");
    }
    recargarTabla();
}

void Proveedores::on_pushButtonAnterior_clicked()
{
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Proveedores::on_pushButtonSiguiente_clicked()
{
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void Proveedores::on_lineEditNombre_returnPressed()
{
    BuscarProveedor *buscaProveedor = new BuscarProveedor(this, ui->lineEditNombre->text());
    buscaProveedor->exec();
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        if (modeloTabla->record(i).value("idProveedor").toString() == buscaProveedor->resultado) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
    delete buscaProveedor;
}

void Proveedores::on_pushButtonModificar_clicked()
{
    QStringList datos = recogerDatosFormulario();
    int i = mapper.currentIndex();
    QMessageBox msgBox;
    msgBox.setText("MODIFICACION.");
    msgBox.setInformativeText("Quiere guardar los cambios?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();
    if (resp == QMessageBox::Ok) {
        if (base.modificarProveedor(QSqlDatabase::database("DB"), datos, ui->lineEditCod->text())) {
            msgBox.setText("Guardado con exito");
            msgBox.setInformativeText("El registro se ha modificado correctamente");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        } else {
            msgBox.setText("Error al guardar");
            msgBox.setInformativeText(
                "Revise los datos del formulario o contacte con el administrador");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
}

void Proveedores::on_lineEditFormaPago_textChanged(const QString &arg1)
{
    ui->labelFPago->setText(base.nombreFormaPago(arg1, conf->getConexionLocal()));
}

void Proveedores::on_pushButtonFPago_clicked()
{
    FormasPago *fpago = new FormasPago(this);
    fpago->exec();
    if (fpago->result() > 0) {
        ui->lineEditFormaPago->setText(fpago->resultado);
    }
}

void Proveedores::on_lineEditCod_editingFinished()
{
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        if (modeloTabla->record(i).value("idProveedor").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
}

void Proveedores::on_pushButton_clicked()
{
    borrarFormulario();
    ui->lineEditCod->setText(base.codigoParaNuevoProveedor());
}

void Proveedores::on_radioButtonComprasFacturas_clicked()
{
    cargarCompras();
}

void Proveedores::on_radioButtonComprasMeses_clicked()
{
    cargarCompras();
}

void Proveedores::on_radioButtonComprasAnos_clicked()
{
    cargarCompras();
}

void Proveedores::on_pushButtonBorrar_clicked()
{
    int i = mapper.currentIndex();
    QMessageBox msgBox;
    msgBox.setText("Borrar.");
    msgBox.setInformativeText("Seguro que quiere borrar este proveedor?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();
    if (resp == QMessageBox::Ok) {
        if (base.borrarProveedor(QSqlDatabase::database("DB"), ui->lineEditCod->text())) {
            msgBox.setText("Borrado con exito");
            msgBox.setInformativeText("El registro se ha borrado correctamente");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        } else {
            msgBox.setText("Error al borrar");
            msgBox.setInformativeText(
                "Revise los datos del formulario o contacte con el administrador");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
        recargarTabla();
        mapper.setCurrentIndex(i);
        refrescarBotones(mapper.currentIndex());
    }
}
