#include "entradamercancia.h"
#include "ui_entradamercancia.h"

#include <QMessageBox>

EntradaMercancia::EntradaMercancia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EntradaMercancia)
{
    ui->setupUi(this);
    mTablaEntradas = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    mTablaEntradas->setTable("entradaGenero_tmp");
    ui->tableView->hideColumn(0);
    actualizarTabla();
    codSeleccionado="";
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



        // Buscar si hay lotes con unidades pendientes
        QString idLote = base->idLote(mTablaEntradas->record(i).value(1).toString(),"","2000-01-01");
        qDebug() << "IDLOTE "+ idLote;
        if (idLote != "0") {
            int pendientes = base->unidadesLote(idLote);
            qDebug() << "Hay lotes pendientes";
            qDebug() << "Lotes PENDIENTES "+pendientes;
            if(abs(pendientes) > uds.toInt() ){
                qDebug() << "Hay mas pendientes que entradas";
                base->aumentarLote(idLote,uds.toInt());
            }else if (abs(pendientes) == uds.toInt()) {
                qDebug() << "Hay los mismos pendientes que entradas";
                base->ejecutarSentencia("DELETE FROM lotes WHERE id = '"+idLote+"'");
            }else {
                qDebug() << "Hay mas entradas que pendientes";
                base->ejecutarSentencia("DELETE FROM lotes WHERE id = '"+idLote+"'");
                qDebug() << "ENTRADAS: "+uds;
                qDebug() << "PENDIENTES: "+pendientes;
                int unidades = uds.toInt()+pendientes;
                qDebug() << "DIFERENCIA: "+unidades;

                idLote = base->idLote(cod,"",fechaCaducidad);
                if (idLote == "0") {
                    base->crearLote(cod,"",fechaCaducidad,QString::number(unidades));
                }else {
                    base->aumentarLote(idLote,unidades);
                }
            }
        }else {
            idLote = base->idLote(cod,"",fechaCaducidad);
            if (idLote == "0") {
                base->crearLote(cod,"",fechaCaducidad,uds);
            }else {
                base->aumentarLote(idLote,uds.toInt());
            }
        }


    QString codigo = mTablaEntradas->record(i).value(1).toString();
    QString precio = mTablaEntradas->record(i).value(6).toString();
    QString descripcion = mTablaEntradas->record(i).value(3).toString();
    QSqlQuery cambios = base->consulta_producto(QSqlDatabase::database("DB"),codigo);
    cambios.first();
    QString precioAnterior = cambios.value("pvp").toString();
    QString descripcionAnterior = cambios.value("descripcion").toString();
    if(!(precio == precioAnterior && descripcion == descripcionAnterior)){
      QSqlQuery tmp = base->ejecutarSentencia("UPDATE articulos SET descripcion = '"+descripcion+"' , pvp = "+precio+" WHERE cod = '"+codigo+"'");
      qDebug() << tmp.lastError();
    }
    }
    base->vaciarTabla("entradaGenero_tmp");
    actualizarTabla();
}

void EntradaMercancia::actualizarTabla()
{
    mTablaEntradas->setSort(3,Qt::AscendingOrder);
    mTablaEntradas->select();
    ui->tableView->setModel(mTablaEntradas);
    ui->tableView->resizeColumnsToContents();
}

void EntradaMercancia::on_lineEditCod_returnPressed()
{
    consulta = base->consulta_producto(QSqlDatabase::database("DB"),ui->lineEditCod->text());
    consulta.first();
    if(!consulta.isValid()){
        QString cod = base->codigoDesdeAux(ui->lineEditCod->text());
        consulta = base->consulta_producto(QSqlDatabase::database("DB"),cod);
        consulta.first();
    }
    if(consulta.numRowsAffected() ==1){
        ui->lineEditCod->setText(consulta.value(0).toString());
        ui->lineEditDesc->setText(consulta.value(1).toString());
        ui->lineEditPVP->setText(consulta.value(2).toString());
        ui->lineEditUds->setFocus();

    }else{
        QMessageBox *msg = new QMessageBox(this);
        msg->setText("No se encuentra el producto");
        msg->setInformativeText("Desea crearlo?");
        msg->setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
        msg->setDefaultButton(QMessageBox::Ok);
        int resp = msg->exec();
        if(resp == QMessageBox::Ok){
            Articulos *articulo = new Articulos;
            articulo->exec();
            articulo->borrarFormulario();

            qDebug() << "Crear producto";
        }else{
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
    base->insertarEnTabla(QSqlDatabase::database("DB"),"entradaGenero_tmp",datos);

    mTablaEntradas->select();
    ui->lineEditCod->clear();
    ui->lineEditDesc->clear();
    ui->lineEditPVP->clear();
    ui->lineEditUds->clear();
    ui->lineEditCod->setFocus();
    actualizarTabla();

}

void EntradaMercancia::on_lineEditDesc_returnPressed()
{
    QSqlQuery consulta = base->buscarProducto(QSqlDatabase::database("DB"),"articulos",ui->lineEditDesc->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    ui->lineEditCod->setText(buscar->resultado);
    emit on_lineEditCod_returnPressed();
}

void EntradaMercancia::on_pushButtonBorrar_clicked()
{
    if (!codSeleccionado.isEmpty()) {
        QSqlQuery tmp =base->ejecutarSentencia("DELETE FROM entradaGenero_tmp WHERE id = '"+codSeleccionado+"'");
        qDebug() << tmp.lastError();
    }
    actualizarTabla();
}

void EntradaMercancia::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = mTablaEntradas->index(index.row(),0);
    codSeleccionado = mTablaEntradas->data(indice,Qt::EditRole).toString();
    qDebug() << codSeleccionado;
}

void EntradaMercancia::on_dateEditCaducidad_editingFinished()
{
    if (ui->dateEditCaducidad->date() <= QDate::currentDate()) {
        QMessageBox::information(this,"Error en la fecha de caducidad","La fecha no puede ser anterior a la fecha de hoy");
        ui->dateEditCaducidad->setFocus();
        ui->dateEditCaducidad->setDate(QDate::currentDate());
    }
}
