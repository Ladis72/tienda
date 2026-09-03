#include "tiendas.h"
#include "syncmanager.h"
#include <QFileDialog>
#include <QMessageBox>
#include "ui_tiendas.h"
#include <QDebug>

tiendas::tiendas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tiendas)
{
    nombreConexionMaster = conf->getConexionMaster();
    ui->setupUi(this);
    editandoNuevo = false;
    
    modeloTabla = new QSqlQueryModel(this);
    ui->tableViewTiendas->setModel(modeloTabla);
    ui->tableViewTiendas->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableViewTiendas->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableViewTiendas->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    recargarTabla();
    ui->tableViewTiendas->hideColumn(0); // hide password column
    ui->tableViewTiendas->hideColumn(9); // hide password column

    mapper.addMapping(ui->lineEditId, 0);
    mapper.addMapping(ui->lineEditNombre, 1);
    mapper.addMapping(ui->lineEditDireccion, 2);
    mapper.addMapping(ui->lineEditCiudad, 3);
    mapper.addMapping(ui->lineEditTlfn, 4);
    mapper.addMapping(ui->lineEditWharsapp, 5);
    mapper.addMapping(ui->lineEditEmail, 6);
    mapper.addMapping(ui->lineEditIP, 7);
    mapper.addMapping(ui->lineEditUsusario, 8);
    mapper.addMapping(ui->lineEditPassword, 9);
    mapper.addMapping(ui->checkBoxMaster, 10);
    mapper.addMapping(ui->checkBoxLocal, 11);
    mapper.addMapping(ui->lineEditBase, 12);
    mapper.addMapping(ui->lineEditPuerto, 13);
    // Índice 14 corresponde al campo ssl_ca
    mapper.addMapping(ui->lineEditSslCa, 14);

    if(modeloTabla->rowCount() > 0) {
        ui->tableViewTiendas->selectRow(0);
        mapper.setCurrentIndex(0);
    }
    refrescarBotones();
}

tiendas::~tiendas()
{
    delete ui;
}

void tiendas::on_tableViewTiendas_clicked(const QModelIndex &index)
{
    mapper.setCurrentIndex(index.row());
    editandoNuevo = false;
    refrescarBotones();
}

void tiendas::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM tiendas", QSqlDatabase::database(conf->getConexionLocal()));
    mapper.setModel(modeloTabla);
    
    if(modeloTabla->rowCount() > 0) {
        ui->tableViewTiendas->selectRow(0);
        mapper.setCurrentIndex(0);
    } else {
        borrarFormulario();
    }
}

void tiendas::refrescarBotones()
{
    ui->pushButtonNuevo->setVisible(!editandoNuevo);
    ui->pushButtonBorrar->setVisible(!editandoNuevo);
    ui->pushButtonRefrescar->setVisible(!editandoNuevo);
    
    ui->pushButtonGuardar->setVisible(true); // Siempre visible par editar o crear
    ui->pushButtonCancelar->setVisible(editandoNuevo);
    
    ui->tableViewTiendas->setEnabled(!editandoNuevo);
}

void tiendas::borrarFormulario()
{
    foreach (QLineEdit *le, ui->general_2->findChildren<QLineEdit *>()) {
        le->clear();
    }
    ui->checkBoxLocal->setChecked(false);
    ui->checkBoxMaster->setChecked(false);
    ui->lineEditNombre->setFocus();
}

QStringList tiendas::recogerDatos()
{
    QStringList listaDatos;
    listaDatos.clear();
    listaDatos.append(ui->lineEditId->text());
    listaDatos.append(ui->lineEditNombre->text());
    listaDatos.append(ui->lineEditDireccion->text());
    listaDatos.append(ui->lineEditCiudad->text());
    listaDatos.append(ui->lineEditTlfn->text());
    listaDatos.append(ui->lineEditWharsapp->text());
    listaDatos.append(ui->lineEditEmail->text());
    listaDatos.append(ui->lineEditIP->text());
    listaDatos.append(ui->lineEditUsusario->text());
    listaDatos.append(ui->lineEditPassword->text());
    listaDatos.append(ui->checkBoxMaster->isChecked() ? "1" : "0");
    listaDatos.append(ui->checkBoxLocal->isChecked() ? "1" : "0");
    listaDatos.append(ui->lineEditBase->text());
    listaDatos.append(ui->lineEditPuerto->text());
    listaDatos.append(ui->lineEditSslCa->text());
    return listaDatos;
}

void tiendas::on_pushButtonNuevo_clicked()
{
    editandoNuevo = true;
    borrarFormulario();
    refrescarBotones();
}

void tiendas::on_pushButtonGuardar_clicked()
{
    QStringList datos = recogerDatos();
    
    QMessageBox msgBox;
    bool exito = false;
    
    if (editandoNuevo || ui->lineEditId->text().isEmpty()) {
        datos.replace(0, ""); // Asegurar ID vacío para INSERT
        if (base->crearTienda(datos)) {
            exito = true;
            msgBox.setText("Guardado con exito");
            msgBox.setInformativeText("El registro se ha creado correctamente.");
        }
    } else {
        if (base->modificarTienda(datos)) {
            exito = true;
            msgBox.setText("Modificado con exito");
            msgBox.setInformativeText("El registro se ha modificado correctamente.");
            if (ui->checkBoxMaster->isChecked()) {
                conf->setConexionMaster(base->nombreConexionMaster());
            }
        }
    }
    
    if (exito) {
        if (SyncManager::instance()) {
            SyncManager::instance()->cargarIdTiendaLocal();
        }
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        editandoNuevo = false;
        recargarTabla();
        refrescarBotones();
    } else {
        msgBox.setText("Error al guardar");
        msgBox.setInformativeText("Revise los datos del formulario.\nError SQL: " + base->getLastError());
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

void tiendas::on_pushButtonCancelar_clicked()
{
    editandoNuevo = false;
    refrescarBotones();
    if(modeloTabla->rowCount() > 0) {
        int r = ui->tableViewTiendas->currentIndex().row();
        if (r < 0) r = 0;
        mapper.setCurrentIndex(r);
        ui->tableViewTiendas->selectRow(r);
    } else {
        borrarFormulario();
    }
}

void tiendas::on_pushButtonBorrar_clicked()
{
    if (ui->lineEditId->text().isEmpty()) return;
    
    QMessageBox msgBox;
    msgBox.setText("Borrar");
    msgBox.setInformativeText("Seguro que quiere borrar esta tienda?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    
    if (msgBox.exec() == QMessageBox::Ok) {
        if (base->borrarTienda(ui->lineEditId->text())) {
            msgBox.setText("Borrado con exito");
            msgBox.setInformativeText("El registro se ha borrado correctamente.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
            recargarTabla();
        } else {
            msgBox.setText("Error al borrar");
            msgBox.setInformativeText("Revise los datos o contacte con el administrador.");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
}

void tiendas::on_pushButtonRefrescar_clicked()
{
    mapper.revert();
    recargarTabla();
}

void tiendas::on_checkBoxMaster_stateChanged(int arg1)
{
    qDebug() << base->nombreConexionMaster();
}

void tiendas::on_checkBoxLocal_stateChanged(int arg1) {}

void tiendas::on_pushButtonSslCa_clicked()
{
    // Abre un diálogo para seleccionar el fichero del certificado CA para SSL
    QString ruta = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar certificado CA"),
        QDir::homePath(),
        tr("Certificados (*.pem *.crt *.cer *.ca-bundle);;Todos los ficheros (*)"));
    if (!ruta.isEmpty()) {
        ui->lineEditSslCa->setText(ruta);
    }
}
