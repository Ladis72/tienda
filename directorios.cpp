#include "directorios.h"
#include "ui_directorios.h"
#include <QFileDialog>

Directorios::Directorios(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Directorios)
{
    ui->setupUi(this);
    cargalListaBase();
}

Directorios::~Directorios()
{
    delete ui;
}

void Directorios::on_pushButtonAceptar_clicked()
{
    cargarListaLineEdit();
    if (base->guardarDirectorios(listaDatos)) {
        qDebug() << listaDatos;
        this->close();
    }
}

void Directorios::llenarListaBase(QStringList lista)
{
    ui->lineEditFactura->setText(lista.at(0));
    ui->lineEditVentas->setText(lista.at(1));
    ui->lineEditEtiquetas->setText(lista.at(2));
    ui->lineEditCaducados->setText(lista.at(3));
    ui->lineEditArqueos->setText(lista.at(4));
    ui->lineEditMovimientos->setText(lista.at(5));
}

void Directorios::cargarListaLineEdit()
{
    listaDatos.clear();
    listaDatos.append(ui->lineEditFactura->text());
    listaDatos.append(ui->lineEditVentas->text());
    listaDatos.append(ui->lineEditEtiquetas->text());
    listaDatos.append(ui->lineEditCaducados->text());
    listaDatos.append(ui->lineEditArqueos->text());
    listaDatos.append(ui->lineEditMovimientos->text());
}

void Directorios::cargalListaBase()
{
    listaDatos = base->cargarDirectorios();
    llenarListaBase(listaDatos);
}

void Directorios::on_toolButtonFactura_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    qDebug() << directorio;
    ui->lineEditFactura->setText(directorio);
}

void Directorios::on_toolButtonVentas_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditVentas->setText(directorio);
}

void Directorios::on_toolButtonEtiquetas_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditEtiquetas->setText(directorio);

}

void Directorios::on_toolButtonCaducados_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditCaducados->setText(directorio);

}

void Directorios::on_toolButtonArqueos_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditArqueos->setText(directorio);

}

void Directorios::on_toolButtonMovimientos_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditMovimientos->setText(directorio);

}
