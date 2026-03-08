#include "tiendas.h"
#include <QMessageBox>
#include "ui_tiendas.h"

tiendas::tiendas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::tiendas)
{
    nombreConexionMaster = conf->getConexionMaster();
    ui->setupUi(this);
    modeloTabla = new QSqlQueryModel;
    recargarTabla();
    mapper.setCurrentIndex(0);
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

    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());
}

tiendas::~tiendas()
{
    delete ui;
}

void tiendas::on_pushButtonNuevo_clicked()
{
    QStringList datos = recogerDatos();
    datos.replace(0, "");
    QMessageBox msgBox;
    qDebug() << datos;
    if (base->crearTienda(datos)) {
        msgBox.setText("Guardado con exito");
        msgBox.setInformativeText("El registro se ha creado correctamente");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    } else {
        msgBox.setText("Error al guardar");
        msgBox.setInformativeText(
            "Revise los datos del formulario o contacte con el administrador");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    recargarTabla();
}

void tiendas::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM tiendas", QSqlDatabase::database("DB"));
    mapper.setModel(modeloTabla);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());
}

void tiendas::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
}

void tiendas::borrarFormulario()
{
    foreach (QLineEdit *le, ui->general_2->findChildren<QLineEdit *>()) {
        le->clear();
        qDebug() << "Borrando line edit";
    }
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
    if (ui->checkBoxMaster->isChecked()) {
        listaDatos.append("1");
    } else {
        listaDatos.append("0");
    }
    if (ui->checkBoxLocal->isChecked()) {
        listaDatos.append("1");
    } else {
        listaDatos.append("0");
    }
    return listaDatos;
}

void tiendas::on_pushButtonSiguiente_clicked()
{
    borrarFormulario();
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void tiendas::on_pushButtonAnterior_clicked()
{
    borrarFormulario();
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void tiendas::on_pushButtonModificar_clicked()
{
    QStringList datos = recogerDatos();
    int i = mapper.currentIndex();
    qDebug() << datos;
    QMessageBox msgBox;
    if (base->modificarTienda(datos)) {
        msgBox.setText("Guardado con exito");
        msgBox.setInformativeText("El registro se ha modificado correctamente");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
        if (ui->checkBoxMaster->isChecked()) {
            conf->setConexionMaster(base->nombreConexionMaster());
        }
    } else {
        msgBox.setText("Error al guardar");
        msgBox.setInformativeText(
            "Revise los datos del formulario o contacte con el administrador");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(mapper.currentIndex());
}

void tiendas::on_pushButtonBorrar_clicked()
{
    int i = mapper.currentIndex();
    QMessageBox msgBox;
    msgBox.setText("Borrar");
    msgBox.setInformativeText("Seguro que quiere borrar esta tienda.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    int resp = msgBox.exec();
    if (resp == QMessageBox::Ok) {
        if (base->borrarTienda(ui->lineEditId->text())) {
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
    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(mapper.currentIndex());
}

void tiendas::on_pushButtonRefrescar_clicked()
{
    mapper.revert();
}

void tiendas::on_checkBoxMaster_stateChanged(int arg1)
{
    qDebug() << base->nombreConexionMaster();
}

void tiendas::on_checkBoxLocal_stateChanged(int arg1) {}
