#include "entradasalida.h"
#include <QDate>
#include <QMessageBox>
#include "ui_entradasalida.h"

EntradaSalida::EntradaSalida(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EntradaSalida)
{
    ui->setupUi(this);
    llenarCombo();
}

EntradaSalida::~EntradaSalida()
{
    delete ui;
}

void EntradaSalida::on_pushButtonAceptar_clicked()
{
    QStringList datos;
    QMessageBox msgBox;
    datos.append(QDate::currentDate().toString("yyyy-MM-dd"));
    datos.append(QTime::currentTime().toString("hh:mm:ss"));
    datos.append(ui->lineEditCantidad->text());
    datos.append(
        ui->comboBoxMotivo->currentText().left(ui->comboBoxMotivo->currentText().indexOf("-")));
    qDebug() << ui->comboBoxMotivo->currentText().left(
        ui->comboBoxMotivo->currentText().indexOf("-"));
    datos.append(ui->lineEditDescripcion->text());
    if (base->insertarES(datos, conf->getConexionLocal())) {
        msgBox.setText("Añadido con éxito");
        msgBox.setInformativeText("La información se ha almacenado");
        msgBox.exec();
    } else {
        msgBox.setText("ERROR");
        msgBox.setInformativeText("La información no se ha almacenado");
        msgBox.exec();
    }
    close();
}

void EntradaSalida::on_toolButton_clicked()
{
    anadirTipo = new TiposEntradasSalidas;
    anadirTipo->exec();
    llenarCombo();
}

void EntradaSalida::llenarCombo()
{
    consulta = base->devolverTablaCompleta(conf->getConexionLocal(),"motivosEntrada");
    consulta.first();
    do {
        ui->comboBoxMotivo->addItem(consulta.value("idtiposEntrada").toString() + "-"
                                    + consulta.value("descripcion").toString());
    } while (consulta.next());
}
