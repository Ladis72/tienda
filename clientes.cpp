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
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());
}

Clientes::Clientes(QWidget *parent , QString codigo) :
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
    borrarFormulario();
    ui->lineEditCod->setText(codigo);
    ui->lineEditCod->setEnabled(false);
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
    ui->labelNombreCliente->clear();
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
    ui->labelNombreCliente->setText(ui->lineEditNombre->text()+" "+ui->lineEditApellidos->text());
    cargarCompras();
}

void Clientes::cargarCompras()
{
    modeloCompras.clear();
    if (ui->radioButtonMeses->isChecked()) {
        modeloCompras.setQuery("SELECT year(fecha), month(fecha) , sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" group by year(fecha) , month(fecha) order by year(fecha) desc , month(fecha) desc; ",QSqlDatabase::database("DB"));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Año");
        modeloCompras.setHeaderData(1,Qt::Horizontal,"Mes");
        modeloCompras.setHeaderData(2,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;
    }
    if (ui->radioButtonAnos->isChecked()) {
        modeloCompras.setQuery("SELECT year(fecha) , sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" group by year(fecha)  order by year(fecha) desc ; ",QSqlDatabase::database("DB"));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Año");
        modeloCompras.setHeaderData(1,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;
    }
    if (ui->radioButtonFechas->isChecked()) {
        modeloCompras.setQuery("select sum(total) FROM tienda.tickets where cliente = "+ui->lineEditCod->text()+" and fecha between '"+ui->dateEditDesde->date().toString("yyyy-MM-dd")+"' and '"+ui->dateEditHasta->date().toString("yyyy-MM-dd")+"';",QSqlDatabase::database("DB"));
        modeloCompras.setHeaderData(0,Qt::Horizontal,"Cantidad");
        ui->tableView->setModel(&modeloCompras);
        ui->tableView->resizeColumnsToContents();
        return;    }

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
    if(base.existeDatoEnTabla(QSqlDatabase::database("DB"),"clientes","idProveedor",ui->lineEditCod->text())){
        QMessageBox::warning(this, "ATENCION",
                              "El registro ya existe");
        return;
    }


    QStringList datos = recogerDatosFormulario();
    if (base.crearCliente(QSqlDatabase::database("DB"),datos)) {
        QMessageBox::about(this,"Atención", "Artículo creado con éxito");
    } else {
        QMessageBox::warning(this,"Error","No se ha podido crear el Cliente");
    }
    recargarTabla();

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



void Clientes::on_lineEditNombre_returnPressed()
{
    consulta = base.buscarEnTabla(QSqlDatabase::database("DB"),"clientes","nombre", ui->lineEditNombre->text());
    consulta.first();
    qDebug() << consulta.lastError().text();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    qDebug() << buscar->resultado;
    for (int i = 0; i < modeloTabla->rowCount(); i++){
        if (modeloTabla->record(i).value("idCliente").toString() == buscar->resultado) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
    delete buscar;
}

void Clientes::on_lineEditCod_editingFinished()
{
    for (int i = 0; i < modeloTabla->rowCount(); ++i) {
        if (modeloTabla->record(i).value("idCliente").toString() == ui->lineEditCod->text()) {
            mapper.setCurrentIndex(i);
            refrescarBotones(i);
            break;
        }
    }
}

void Clientes::on_radioButtonMeses_clicked()
{
    cargarCompras();
}

void Clientes::on_radioButtonAnos_clicked()
{
    cargarCompras();

}

void Clientes::on_radioButtonFechas_clicked()
{
    cargarCompras();

}

void Clientes::on_dateEditHasta_userDateChanged(const QDate &date)
{
    cargarCompras();

}

void Clientes::on_dateEditDesde_userDateChanged(const QDate &date)
{
    cargarCompras();

}
