#include "prestamos.h"
#include "ui_prestamos.h"
#include "buscarproducto.h"
#include <QMessageBox>

Prestamos::Prestamos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prestamos)
{
    ui->setupUi(this);
    modeloPrestamos = new QSqlTableModel(this,QSqlDatabase::database("DB"));
    modeloPrestamos->clear();
    modeloPrestamos->setTable("prestamos");
    modeloPrestamos->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modeloPrestamos->setHeaderData(1,Qt::Horizontal,"Cod.");
    modeloPrestamos->setHeaderData(2,Qt::Horizontal,"Descripción");
    modeloPrestamos->setHeaderData(3,Qt::Horizontal,"Tienda");
    modeloPrestamos->setHeaderData(4,Qt::Horizontal,"Fecha");
    modeloPrestamos->setHeaderData(5,Qt::Horizontal,"Cantidad");
    modeloPrestamos->setHeaderData(6,Qt::Horizontal,"Notas");
    modeloPrestamos->sort(2,Qt::AscendingOrder);
    modeloPrestamos->select();
    ui->tableView->setModel(modeloPrestamos);
    ui->tableView->hideColumn(0);
    ui->tableView->resizeColumnsToContents();
    ui->comboBox->addItems(base->listadoPrestamistas());
}

Prestamos::~Prestamos()
{
    delete ui;

}

void Prestamos::on_lineEdit_textChanged(const QString &arg1)
{
    modeloPrestamos->setFilter("producto LIKE '%"+arg1+"%'");
}

void Prestamos::on_lineEditCod_textChanged(const QString &arg1)
{
    modeloPrestamos->setFilter("ean LIKE '%"+arg1+"%'");
}

void Prestamos::on_lineEditEan_returnPressed()
{
    consulta = base->consulta_producto(QSqlDatabase::database("DB"),ui->lineEditEan->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base->codigoDesdeAux(ui->lineEditEan->text());
        consulta = base->consulta_producto(QSqlDatabase::database("DB"),cod);
        consulta.first();
    }
    if (consulta.numRowsAffected() == 1 ) {
        ui->lineEditEan->setText(consulta.value(0).toString());
        ui->lineEditDescripcion->setText(consulta.value(1).toString());
        ui->spinBox->setFocus();
    }
}

void Prestamos::on_pushButtonProcesar_clicked()
{
    QString idLote = base->idLote(ui->lineEditEan->text(),"",ui->dateEdit->text());
    if (idLote == "0" && ui->spinBox->value() < 0) {
        QMessageBox msgBox;
        msgBox.setText("Parece que no hay en el almacen un producto con esos datos");
        msgBox.setInformativeText("¿Deseas continual con la operación o Cancelar y volver a comprobarlo?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            qDebug() << "cancelado";
            return;
            break;
        default:
            break;
        }
    }
    QSqlQuery datosLote;
    datosLote = base->ejecutarSentencia("SELECT cantidad FROM lotes WHERE id ='"+idLote+"'");
    datosLote.first();
    int resto = datosLote.record().value("cantidad").toInt() - ui->spinBox->value();
    if (resto < 0 && ui->spinBox->value() < 0) {
        QMessageBox msgBox;
        msgBox.setText("Parece que no hay en el almacen suficiente producto con esos datos");
        msgBox.setInformativeText("¿Deseas continual con la operación o Cancelar y volver a comprobarlo?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        int ret = msgBox.exec();
        switch (ret) {
        case QMessageBox::Cancel:
            qDebug() << "cancelado";
            return;
            break;
        default:
            break;
        }
    }
    QStringList datos;
    datos.clear();
    datos.append(ui->lineEditEan->text());
    datos.append(ui->lineEditDescripcion->text());
    datos.append(ui->comboBox->currentText());
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(QString::number(ui->spinBox->value()));
    datos.append(ui->lineEditNotas->text());
    qDebug() << datos;
    if (datos.at(4) < 0) {
        base->descontarArticulo(datos.at(0),abs(datos.at(4).toInt()));
    } else {QString idLote = base->idLote(datos.at(0),"",ui->dateEdit->date().toString("yyyy-MM-dd"));
        if (idLote != "0") {
            base->aumentarLote(idLote,datos.at(4).toInt());
            qDebug() << "Error al aumentar lote especificado";
        }else{
            base->crearLote(datos.at(0),"",ui->dateEdit->date().toString("yyyy-MM-dd"),datos.at(4));
        }
    }
    base->insertarEnTabla(QSqlDatabase::database("DB"),"prestamos",datos);
    modeloPrestamos->select();
}

void Prestamos::on_lineEditDescripcion_returnPressed()
{
    consulta = base->buscarEnTabla(QSqlDatabase::database("DB"),"articulos","descripcion",ui->lineEditDescripcion->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    ui->lineEditEan->setText(buscar->resultado);
    emit on_lineEditEan_returnPressed();
}
