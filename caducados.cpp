#include "caducados.h"
#include "ui_caducados.h"
#include "QDate"
#include <QMessageBox>

Caducados::Caducados(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caducados)
{
    ui->setupUi(this);
}

Caducados::Caducados(QString ean, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Caducados)
{
    ui->setupUi(this);
    ui->lineEditCodigo->setText(ean);
    emit on_lineEditCodigo_returnPressed();
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
    ui->comboBox->addItem("Selecciona uno");

    consulta = base.lotesProducto(id,"DB");
    while (consulta.next()) {
        ui->comboBox->addItem(consulta.value("fecha").toString());
    }
    ui->comboBox->addItem("Desconocido");
}

void Caducados::on_pushButton_clicked()
{
    if (ui->comboBox->currentText() == "Selecciona uno") {
        QMessageBox::information(this,"Faltan datos","Selecciona la fecha adecuada del desplegable");
        return;
    }
    if (ui->lineEditCodigo->text().isEmpty() || ui->lineEditDescripcion->text().isEmpty()) {
        QMessageBox::warning(this, "Faltan datos del producto","No hay datos en el codigo o descripción del producto");
        return;
    }
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
    //ui->lineEditCodigo->clear();
    //ui->lineEditDescripcion->clear();
    ui->comboBox->clear();
    ui->spinBox->setValue(1);
    //ui->lineEditCodigo->setFocus();
    emit on_lineEditCodigo_returnPressed();
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

void Caducados::on_comboBox_currentIndexChanged(const QString &arg1)
{
    QString idLote = base.idLote(ui->lineEditCodigo->text(),"",arg1);
    int udsLote = base.unidadesLote(idLote);
    if (arg1 == "Desconocido") {
        ui->spinBox->setMaximum(1000);
        ui->spinBox->setMinimum(0);
    }else {
        ui->spinBox->setMaximum(udsLote);
        ui->spinBox->setMinimum(0);
    }
}
