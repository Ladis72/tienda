#include "directorios.h"
#include <QFileDialog>
#include "ui_directorios.h"

Directorios::Directorios(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Directorios)
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
    if (base->guardarDirectorios(conf->getConexionLocal(), listaDatos)) {
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
    ui->lineEditCseg->setText(lista.at(6));
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
    listaDatos.append(ui->lineEditCseg->text());
}

void Directorios::cargalListaBase()
{
    listaDatos = base->cargarDirectorios(conf->getConexionLocal());
    llenarListaBase(listaDatos);
}

QString Directorios::rutaRelativa(QString directorio)
{
    if (!directorio.isEmpty()) {
        QString rutaEjecutable = QCoreApplication::applicationDirPath();
        QString rutaRelativa = QDir(rutaEjecutable).relativeFilePath(directorio);

        if (!rutaRelativa.startsWith(".") && !rutaRelativa.startsWith("/"))
            rutaRelativa.prepend("./");
        return rutaRelativa;
}
    return "";
}

void Directorios::on_toolButtonFactura_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Seleccionas directorio para la factura",
                                                           ui->lineEditFactura->text(),
                                                           QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);
    ui->lineEditFactura->setText(rutaRelativa(directorio));
}

void Directorios::on_toolButtonVentas_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditVentas->setText(directorio);
}

void Directorios::on_toolButtonEtiquetas_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Selecciona el directorio de etiquetas",
                                                           ui->lineEditEtiquetas->text(),
                                                           QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);
    ui->lineEditEtiquetas->setText(rutaRelativa(directorio));
}

void Directorios::on_toolButtonCaducados_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditCaducados->setText(directorio);
}

void Directorios::on_toolButtonArqueos_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Selecciona el directorio de Arqueos",
                                                           ui->lineEditArqueos->text(),
                                                           QFileDialog::ShowDirsOnly
                                                           | QFileDialog::DontResolveSymlinks);
    ui->lineEditArqueos->setText(rutaRelativa(directorio));
}

void Directorios::on_toolButtonMovimientos_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditMovimientos->setText(directorio);
}

void Directorios::on_toolButtonCseg_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Seleccionar directorio para la copia",
                                                           ui->lineEditCseg->text(),
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);

    ui->lineEditCseg->setText(rutaRelativa(directorio));

}
