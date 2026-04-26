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
    // Solo el administrador (Rol 0) puede modificar los directorios
    if (conf->getRol() != 0) {
        QMessageBox::warning(this, tr("Acceso denegado"),
                             tr("Solo el administrador puede modificar la configuración de directorios."));
        return;
    }

    // Aseguramos que todas las rutas sean relativas antes de guardar
    ui->lineEditDocumentos->setText(rutaRelativa(ui->lineEditDocumentos->text()));
    ui->lineEditLogFactura->setText(rutaRelativa(ui->lineEditLogFactura->text()));
    ui->lineEditCseg->setText(rutaRelativa(ui->lineEditCseg->text()));
    ui->lineEditLogo->setText(rutaRelativa(ui->lineEditLogo->text()));
    ui->lineEditImagenes->setText(rutaRelativa(ui->lineEditImagenes->text()));

    // Cargamos los datos de la UI al mapa para guardar
    cargarListaLineEdit();

    // Guardamos en la base de datos local (el SyncManager se encargará de subirlo a la nube)
    if (base->guardarDirectorios(conf->getConexionLocal(), listaDatos)) {
        qDebug() << "Directorios guardados y listos para sincronización:" << listaDatos;
        this->accept(); // Usamos accept en lugar de close para indicar éxito
    }
}

void Directorios::llenarListaBase(const QMap<QString, QString> &mapa)
{
    ui->lineEditDocumentos->setText(mapa.value("documentos"));
    ui->lineEditLogFactura->setText(mapa.value("logofactura"));
    ui->lineEditCseg->setText(mapa.value("cseg"));
    ui->lineEditLogo->setText(mapa.value("logo"));
    ui->lineEditImagenes->setText(mapa.value("imagenes"));
}

void Directorios::cargarListaLineEdit()
{
    listaDatos.clear();
    listaDatos.insert("documentos", ui->lineEditDocumentos->text());
    listaDatos.insert("logofactura", ui->lineEditLogFactura->text());
    listaDatos.insert("cseg", ui->lineEditCseg->text());
    listaDatos.insert("logo", ui->lineEditLogo->text());
    listaDatos.insert("imagenes", ui->lineEditImagenes->text());
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


void Directorios::on_toolButtonDocumentos_clicked()
{
    QString directorio = QFileDialog::getExistingDirectory(this,
                                                           "Selecciona el directorio de documentos",
                                                           ui->lineEditDocumentos->text(),
                                                           QFileDialog::ShowDirsOnly
                                                               | QFileDialog::DontResolveSymlinks);
    ui->lineEditDocumentos->setText(rutaRelativa(directorio));
}
