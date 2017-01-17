#include "caducados.h"
#include "ui_caducados.h"
#include "QDate"

Caducados::Caducados(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caducados)
{
    ui->setupUi(this);
}

Caducados::~Caducados()
{
    delete ui;
}

void Caducados::on_lineEditCodigo_returnPressed()
{
    consulta = base.consulta_producto(QSqlDatabase::database("DB"),ui->lineEditCodigo->text());
    consulta.first();
    if (!consulta.isValid()) {
        QString cod = base.codigoDesdeAux(ui->lineEditCodigo->text());
        consulta = base.consulta_producto(QSqlDatabase::database("DB"),cod);
        consulta.first();
    }
    if (consulta.numRowsAffected() == 1 ) {
        ui->lineEditDescripcion->setText(consulta.value(1).toString());
        llenarComboFechas(consulta.value(0).toString());
    }
}

void Caducados::llenarComboFechas(QString id)
{
    ui->comboBox->clear();
    consulta = base.lotesProducto(id);
    while (consulta.next()) {
        ui->comboBox->addItem(consulta.value("fecha").toString());
    }
    ui->comboBox->addItem("Desconocido");
}

void Caducados::on_pushButton_clicked()
{
    base.disminuirLote(ui->lineEditCodigo->text(),ui->comboBox->currentText(),ui->spinBox->value());
    QString sentencia;
    QSqlQuery precioConsulta = base.consulta_producto(QSqlDatabase::database("DB"),ui->lineEditCodigo->text());
    precioConsulta.first();
    QString precio = precioConsulta.value("pvp").toString();
    qDebug() << precio;
    sentencia = "INSERT INTO caducados VALUES (NULL, '"+ui->lineEditCodigo->text()+"' , '"+QString::number(ui->spinBox->value())+"' , '"+ui->lineEditDescripcion->text()+"' , '"+QDate::currentDate().toString("yyyy-MM-dd")+"' , '"+precio+"' , '"+ui->comboBox->currentText()+"')";
    qDebug() << sentencia;
    QSqlQuery ejecutar = base.ejecutarSentencia(sentencia);
    qDebug() << ejecutar.lastError();
    ui->lineEditCodigo->clear();
    ui->lineEditDescripcion->clear();
    ui->comboBox->clear();
    ui->spinBox->setValue(1);
}

void Caducados::on_lineEditDescripcion_returnPressed()
{
    QSqlQuery consulta = base.buscarProducto(QSqlDatabase::database("DB"),"articulos",ui->lineEditDescripcion->text());
    consulta.first();
    BuscarProducto *buscar = new BuscarProducto(this,consulta);
    buscar->exec();
    ui->lineEditCodigo->setText(buscar->resultado);
    emit on_lineEditCodigo_returnPressed();
}
