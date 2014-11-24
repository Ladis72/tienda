#include "clientes.h"
#include "ui_clientes.h"

#include <QMessageBox>

Clientes::Clientes(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Clientes)
{
    ui->setupUi(this);
    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod,0);
    mapper.addMapping(ui->lineEditNombre,1);
    mapper.addMapping(ui->lineEditApellidos,2);
    mapper.addMapping(ui->lineEditDireccion,3);
    mapper.addMapping(ui->lineEditCP,4);
    mapper.addMapping(ui->lineEditLocalidad,5);
    mapper.addMapping(ui->lineEditProvincia,6);
    mapper.addMapping(ui->lineEditNIF,7);
    mapper.addMapping(ui->lineEditTlfn1,8);
    mapper.addMapping(ui->lineEditTlfn2,9);
    mapper.addMapping(ui->lineEditMail,10);
    mapper.addMapping(ui->lineEditDescuento,11);
    mapper.addMapping(ui->dateEdit,12);
    mapper.addMapping(ui->plainTextEdit,13);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());

    ui->lineEditCod->installEventFilter(this);
}

Clientes::~Clientes()
{
    delete ui;
}

void Clientes::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM clientes",QSqlDatabase::database("DB"));
    mapper.setModel(modeloTabla);
}

void Clientes::borrarFormulario()
{
    foreach (QLineEdit* le, ui->Principal->findChildren<QLineEdit*>()) {
        le->clear();
    }
    ui->dateEdit->setDate(QDate::currentDate());
    ui->plainTextEdit->clear();
    ui->labelNombre->clear();
}

QStringList Clientes::recogerDatosFormulario()
{
    QStringList listaDatosFormulario;
    listaDatosFormulario.append(ui->lineEditCod->text());
    listaDatosFormulario.append(ui->lineEditNombre->text());
    listaDatosFormulario.append(ui->lineEditApellidos->text());
    listaDatosFormulario.append(ui->lineEditDireccion->text());
    listaDatosFormulario.append(ui->lineEditCP->text());
    listaDatosFormulario.append(ui->lineEditLocalidad->text());
    listaDatosFormulario.append(ui->lineEditProvincia->text());
    listaDatosFormulario.append(ui->lineEditNIF->text());
    listaDatosFormulario.append(ui->lineEditTlfn1->text());
    listaDatosFormulario.append(ui->lineEditTlfn2->text());
    listaDatosFormulario.append(ui->lineEditMail->text());
    listaDatosFormulario.append(ui->lineEditDescuento->text());
    listaDatosFormulario.append(ui->dateEdit->text());
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());

    return listaDatosFormulario;
}
bool Clientes::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditCod) {
        if (event->type() == QEvent::MouseButtonPress) {
            borrarFormulario();
            qDebug() << "event";
            return true;
        }
    }
    return false;
}

void Clientes::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() -1);
    ui->labelNombre->setText(ui->lineEditNombre->text()+" "+ui->lineEditApellidos->text());
}


void Clientes::on_pushButtonAnterior_clicked()
{
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonSiguiente_clicked()
{
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void Clientes::on_pushButtonNuevo_clicked()
{
    borrarFormulario();
}

void Clientes::on_pushButtonModificar_clicked()
{
    QStringList datos=recogerDatosFormulario();
    int i = mapper.currentIndex();

    QMessageBox msgBox;
    msgBox.setText("MODIFICACION");
    msgBox.setInformativeText("Quiere guardar los cambios?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int resp = msgBox.exec();
    if (resp == QMessageBox::Ok) {
       if(base.modificarCliente(QSqlDatabase::database("DB"),datos,ui->lineEditCod->text())){
           msgBox.setText("Guardado con exito");
           msgBox.setInformativeText("El registro se ha modificado correctamente");
           msgBox.setStandardButtons(QMessageBox::Ok);
           msgBox.exec();
       }
    }else {
        msgBox.setText("Error al guardar");
        msgBox.setInformativeText("Revise los datos del formulario o contacte con el administrador");
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();    }
    recargarTabla();
    mapper.setCurrentIndex(i);
    refrescarBotones(i);
}

void Clientes::on_pushButtonBorrar_clicked()
{
    int idCliente = mapper.currentIndex();
    QMessageBox msgBox;
    msgBox.setText("Borrar?.");
    msgBox.setInformativeText("Borrar un cliente puede influir en documentos anteriores.");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Cancel);
    if (msgBox.exec() == QMessageBox::Ok) {
        if (base.borrarCliente(QSqlDatabase::database("DB"),ui->lineEditCod->text().toInt())) {
            msgBox.setText("Borrado");
            msgBox.setInformativeText("El cliente se ha borrado con éxito");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        } else {
            msgBox.setText("Error al borrar");
            msgBox.setInformativeText("No se ha podido borrar el cliente");
            msgBox.setStandardButtons(QMessageBox::Ok);
            msgBox.exec();
        }
    }
    recargarTabla();
    mapper.setCurrentIndex(idCliente);
    refrescarBotones(idCliente);
}


