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
    ui->lineEditDocumentos->setText(lista.at(0));
    ui->lineEditLogFactura->setText(lista.at(1));
    ui->lineEditCseg->setText(lista.at(2));
    ui->lineEditLogo->setText(lista.at(3));
    ui->lineEditImagenes->setText(lista.at(4));
}

void Directorios::cargarListaLineEdit()
{
    listaDatos.clear();
    listaDatos.append(ui->lineEditDocumentos->text());
    listaDatos.append(ui->lineEditLogFactura->text());
    listaDatos.append(ui->lineEditCseg->text());
    listaDatos.append(ui->lineEditLogo->text());
    listaDatos.append(ui->lineEditImagenes->text());
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

void Directorios::on_toolButtonCseg_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Seleccionar directorio para la copia",
                                                           ui->lineEditCseg->text(),
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);

    ui->lineEditCseg->setText(rutaRelativa(directorio));
}

void Directorios::on_toolButtonLogo_clicked()
{
    QString directorio = QFileDialog::getOpenFileName(this);
    ui->lineEditLogo->setText(rutaRelativa(directorio));
}

void Directorios::on_toolButtonLogoFactura_clicked()
{
    QString logoFactura = QFileDialog::getOpenFileName(this,
                                                       "Elige el logo de la factura");
    ui->lineEditLogFactura->setText(rutaRelativa(logoFactura));
}


void Directorios::on_toolButton_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Seleccionar directorio para la copia",
                                                           ui->lineEditCseg->text(),
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);

    ui->lineEditImagenes->setText(rutaRelativa(directorio));
}

