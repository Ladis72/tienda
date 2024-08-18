#include "tabwidget.h"
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include "base_datos.h"
#include "ui_tabwidget.h"

Ususarios::Ususarios(QWidget *parent)
    : QTabWidget(parent)
    , ui(new Ui::Ususarios)
{
    ui->setupUi(this);
    modeloTabla = new QSqlQueryModel;
    recargarTabla();

    ui->lineEditCod->installEventFilter(this);
}

Ususarios::~Ususarios()
{
    delete ui;
}

void Ususarios::on_pushButtonAnterior_clicked()
{
    mapper.toPrevious();
    refrescarBotones(mapper.currentIndex());
}

void Ususarios::on_pushButtonSiguiente_clicked()
{
    mapper.toNext();
    refrescarBotones(mapper.currentIndex());
}

void Ususarios::refrescarBotones(int i)
{
    ui->pushButtonAnterior->setEnabled(i > 0);
    ui->pushButtonSiguiente->setEnabled(i < modeloTabla->rowCount() - 1);
    ui->labelNombre->setText(ui->lineEditNombre->text() + " " + ui->lineEditApellido->text());
    QString fichero = QDir::currentPath() + "/" + ui->lineEditArchivoFoto->text();
    QImage foto(fichero);
    ui->labelFoto->setPixmap(QPixmap::fromImage(foto));
    ui->labelFoto->setScaledContents(true);
}

QStringList Ususarios::recogerDatosFormulario()
{
    QStringList listaDatosFormulario;
    listaDatosFormulario.append(ui->lineEditCod->text());
    listaDatosFormulario.append(ui->lineEditNombre->text());
    listaDatosFormulario.append(ui->lineEditApellido->text());
    listaDatosFormulario.append(ui->lineEditDireccion->text());
    listaDatosFormulario.append(ui->lineEditLocalidad->text());
    listaDatosFormulario.append(ui->lineEditProvincia->text());
    listaDatosFormulario.append(ui->lineEditCp->text());
    listaDatosFormulario.append(ui->lineEditTlfn->text());
    listaDatosFormulario.append(ui->lineEditNif->text());
    listaDatosFormulario.append(ui->lineEditMail->text());
    listaDatosFormulario.append(ui->lineEditUsusario->text());
    listaDatosFormulario.append(ui->lineEditClave->text());
    listaDatosFormulario.append(ui->plainTextEdit->toPlainText());
    listaDatosFormulario.append(ui->lineEditPermisos->text());
    if (ui->lineEditArchivoFoto->text() == "") {
        listaDatosFormulario.append("imagenes/anonimo.jpg");
    } else {
        listaDatosFormulario.append(ui->lineEditArchivoFoto->text());
    }
    //qDebug() << listaDatosFormulario;

    return listaDatosFormulario;
}

void Ususarios::recargarTabla()
{
    modeloTabla->setQuery("SELECT * FROM usuarios", QSqlDatabase::database("DB"));
    mapper.setModel(modeloTabla);

    ajustarMapper();
}

void Ususarios::ajustarMapper()
{
    mapper.setCurrentIndex(0);
    mapper.addMapping(ui->lineEditCod, 0);
    mapper.addMapping(ui->lineEditNombre, 1);
    mapper.addMapping(ui->lineEditApellido, 2);
    mapper.addMapping(ui->lineEditDireccion, 3);
    mapper.addMapping(ui->lineEditLocalidad, 4);
    mapper.addMapping(ui->lineEditProvincia, 5);
    mapper.addMapping(ui->lineEditCp, 6);
    mapper.addMapping(ui->lineEditTlfn, 7);
    mapper.addMapping(ui->lineEditNif, 8);
    mapper.addMapping(ui->lineEditMail, 9);
    mapper.addMapping(ui->lineEditUsusario, 10);
    mapper.addMapping(ui->lineEditClave, 11);
    mapper.addMapping(ui->plainTextEdit, 12);
    mapper.addMapping(ui->lineEditPermisos, 13);
    mapper.addMapping(ui->lineEditArchivoFoto, 14);
    mapper.toFirst();
    refrescarBotones(mapper.currentIndex());
}

bool Ususarios::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEditCod) {
        if (event->type() == QEvent::MouseButtonPress) {
            ui->lineEditCod->clear();
            borrarFormulario();
            return true;
        }
    }
    return false;
}

void Ususarios::on_pushButtonModificar_clicked()
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
        if (base.modificarUsuaruio(QSqlDatabase::database("DB"), datos, ui->lineEditCod->text())) {
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
        recargarTabla();
        mapper.setCurrentIndex(i);
        refrescarBotones(mapper.currentIndex());
    }
}

void Ususarios::on_pushButtonRefrescar_clicked()
{
    mapper.revert();
    refrescarBotones(mapper.currentIndex());
}

void Ususarios::borrarFormulario()
{
    foreach (QLineEdit *le, ui->General->findChildren<QLineEdit *>()) {
        le->clear();
    }
    ui->labelFoto->clear();
}

void Ususarios::on_pushButtonNuevo_clicked()
{
    consulta = base.usuarios(QSqlDatabase::database("DB"));
    if (consulta.isActive()) {
        while (consulta.next()) {
            if (consulta.value(0).toString() == ui->lineEditCod->text()) {
                QMessageBox::warning(this, "ATENCION", "El registro ya existe");
                return;
            }
        }

        QStringList lista = recogerDatosFormulario();
        if (base.insertarUsuario(QSqlDatabase::database("DB"), lista)) {
            QMessageBox::about(this, "ATENCION", "Nuevo usuario creado");
        } else {
            QMessageBox::warning(this, "ATENCION", "El error al crear usuario");
        }
    }
    recargarTabla();
}

void Ususarios::on_lineEditNombre_returnPressed()
{
    consulta = base.buscarEnTabla(QSqlDatabase::database("DB"),
                                  "usuarios",
                                  "nombre",
                                  ui->lineEditNombre->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    buscar->exec();
    //qDebug() << buscar->resultado;
    for (int i = 0; i < modeloTabla->rowCount(); i++) {
        if (modeloTabla->record(i).value("id").toInt() == buscar->resultado.toInt()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
}

void Ususarios::on_pushButtonCargarImagen_clicked()
{
    int curr = mapper.currentIndex();
    QString dir = QDir::currentPath();
    int i = dir.length();
    QString fichero = QFileDialog::getOpenFileName(this, "Elige el archivo", dir + "/imagenes");
    fichero.remove(
        0, i + 1); //Quita i caracteres desde la posicion 0 de la cadena fichero i=longitud del path
    //qDebug() << fichero;
    //qDebug() << ui->lineEditCod->text().toInt();
    base.modificarFotoUsusario(fichero, ui->lineEditCod->text().toInt());
    recargarTabla();

    mapper.setCurrentIndex(curr);
    refrescarBotones(mapper.currentIndex());
}

void Ususarios::on_pushButtonBorrarImagen_clicked()
{
    int curr = mapper.currentIndex();
    base.modificarFotoUsusario("", ui->lineEditCod->text().toInt());
    recargarTabla();
    mapper.setCurrentIndex(curr);
    refrescarBotones(mapper.currentIndex());
}

void Ususarios::on_pushButtonBorrar_clicked()
{
    int curr = mapper.currentIndex();
    if (base.borrarUsusario(QSqlDatabase::database("DB"), ui->lineEditCod->text().toInt())) {
        QMessageBox::information(this, "Borrado", "El usuario ha sido borrado");
        recargarTabla();
        mapper.setCurrentIndex(curr - 1);
        refrescarBotones(mapper.currentIndex());
    } else {
        QMessageBox::warning(this, "ERROR", "No ha sido bosible borrar el usuario");
        mapper.setCurrentIndex(curr);
    }
}

void Ususarios::on_lineEditCod_editingFinished()
{
    for (int i = 0; i < modeloTabla->rowCount(); i++) {
        if (modeloTabla->record(i).value("id").toInt() == ui->lineEditCod->text().toInt()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
}
