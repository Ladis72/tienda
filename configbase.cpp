#include "configbase.h"
#include "ui_configbase.h"
#include <QCoreApplication>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/**
 * @brief Constructor: crea la tabla config_nube si no existe y carga los datos guardados.
 */
ConfigBase::ConfigBase(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigBase)
{
    ui->setupUi(this);

    // Asegurar que existe la tabla antes de leerla
    crearTablaConfigNube();

    // Cargar los datos guardados en el formulario
    cargarDatos();
}

ConfigBase::~ConfigBase()
{
    delete ui;
}

/**
 * @brief Crea la tabla config_nube en la BD local si no existe.
 *
 * La tabla almacena un único registro (id=1) con los datos de conexión
 * al servidor MariaDB en la nube.
 */
void ConfigBase::crearTablaConfigNube()
{
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.exec("CREATE TABLE IF NOT EXISTS config_nube ("
           "  id       INT          NOT NULL DEFAULT 1 PRIMARY KEY,"
           "  servidor VARCHAR(255) NOT NULL DEFAULT '',"
           "  puerto   INT          NOT NULL DEFAULT 3306,"
           "  baseDatos VARCHAR(100) NOT NULL DEFAULT '',"
           "  usuario  VARCHAR(100) NOT NULL DEFAULT '',"
           "  clave    VARCHAR(255) NOT NULL DEFAULT '',"
           "  ssl_ca   VARCHAR(512)          DEFAULT ''"
           ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COMMENT='Configuración servidor MariaDB en la nube'");

    // Insertar fila por defecto si está vacía
    q.exec("INSERT IGNORE INTO config_nube (id) VALUES (1)");
}

/**
 * @brief Carga en el formulario los datos guardados en config_nube.
 */
void ConfigBase::cargarDatos()
{
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.exec("SELECT servidor, puerto, baseDatos, usuario, clave, ssl_ca FROM config_nube WHERE id = 1");

    if (q.first()) {
        ui->lineEditDireccion->setText(q.value(0).toString());
        ui->lineEditPuerto->setText(q.value(1).toString());
        ui->lineEditBaseDatos->setText(q.value(2).toString());
        ui->lineEditUsuario->setText(q.value(3).toString());
        ui->lineEditClave->setText(q.value(4).toString());
        ui->lineEditSSLCA->setText(q.value(5).toString());
    }
}

/**
 * @brief Slot del botón "Probar conexión".
 *
 * Intenta conectar con los datos introducidos sin guardar nada.
 * Muestra el resultado directamente en el diálogo.
 */
void ConfigBase::on_pushButtonTest_clicked()
{
    QString host    = ui->lineEditDireccion->text().trimmed();
    QString puerto  = ui->lineEditPuerto->text().trimmed();
    QString bd      = ui->lineEditBaseDatos->text().trimmed();
    QString usuario = ui->lineEditUsuario->text().trimmed();
    QString clave   = ui->lineEditClave->text();
    QString sslCa   = ui->lineEditSSLCA->text().trimmed();

    if (host.isEmpty() || bd.isEmpty() || usuario.isEmpty()) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("Introduce al menos el servidor, la base de datos y el usuario."));
        return;
    }

    // Construir nombre de conexión temporal para la prueba
    const QString nombreTest = "TEST_NUBE";
    {
        QSqlDatabase dbTest = QSqlDatabase::addDatabase("QMYSQL", nombreTest);
        dbTest.setHostName(host);
        dbTest.setPort(puerto.toInt());
        dbTest.setDatabaseName(bd);
        dbTest.setUserName(usuario);
        dbTest.setPassword(clave);

        // Añadir SSL si se especifica certificado CA
        QString opciones = "MYSQL_OPT_CONNECT_TIMEOUT=5";
        if (!sslCa.isEmpty())
            opciones += ";SSL_CA=" + sslCa;
        dbTest.setConnectOptions(opciones);

        if (dbTest.open()) {
            QMessageBox::information(this, tr("Conexión correcta"),
                                     tr("✅ Conexión al servidor en la nube establecida correctamente."));
            dbTest.close();
        } else {
            QMessageBox::critical(this, tr("Error de conexión"),
                                  tr("❌ No se pudo conectar:\n") + dbTest.lastError().text());
        }
    }
    // Eliminar la conexión temporal
    QSqlDatabase::removeDatabase(nombreTest);
}

/**
 * @brief Slot del botón "Guardar".
 *
 * Guarda los datos introducidos en la tabla config_nube y cierra el diálogo.
 */
void ConfigBase::on_pushButton_clicked()
{
    QString host    = ui->lineEditDireccion->text().trimmed();
    QString puerto  = ui->lineEditPuerto->text().trimmed();
    QString bd      = ui->lineEditBaseDatos->text().trimmed();
    QString usuario = ui->lineEditUsuario->text().trimmed();
    QString clave   = ui->lineEditClave->text();
    QString sslCa   = ui->lineEditSSLCA->text().trimmed();

    if (host.isEmpty() || bd.isEmpty() || usuario.isEmpty()) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("Introduce al menos el servidor, la base de datos y el usuario."));
        return;
    }

    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("UPDATE config_nube SET "
              "servidor = :servidor, puerto = :puerto, baseDatos = :bd, "
              "usuario = :usuario, clave = :clave, ssl_ca = :ssl_ca "
              "WHERE id = 1");
    q.bindValue(":servidor", host);
    q.bindValue(":puerto",   puerto.toInt());
    q.bindValue(":bd",       bd);
    q.bindValue(":usuario",  usuario);
    q.bindValue(":clave",    clave);
    q.bindValue(":ssl_ca",   sslCa);

    if (q.exec()) {
        QMessageBox::information(this, tr("Guardado"),
                                 tr("Configuración del servidor en la nube guardada correctamente."));
        accept();
    } else {
        QMessageBox::critical(this, tr("Error al guardar"),
                              tr("No se pudieron guardar los datos:\n") + q.lastError().text());
    }
}

/**
 * @brief Slot del botón "..." junto al campo de certificado SSL CA.
 *
 * Abre un selector de fichero para elegir el certificado .pem/.crt.
 * La ruta seleccionada se convierte a ruta relativa respecto al
 * directorio del ejecutable y se muestra en lineEditSSLCA.
 *
 * Usar ruta relativa facilita mover la instalación entre equipos.
 */
void ConfigBase::on_pushButtonBuscarCa_clicked()
{
    QString rutaAbsoluta = QFileDialog::getOpenFileName(
        this,
        tr("Seleccionar certificado CA"),
        QCoreApplication::applicationDirPath(),
        tr("Certificados (*.pem *.crt *.cer *.ca-bundle);;Todos los ficheros (*)")
    );

    if (rutaAbsoluta.isEmpty())
        return;

    // Convertir a ruta relativa respecto al directorio del ejecutable
    QDir dirApp(QCoreApplication::applicationDirPath());
    QString rutaRelativa = dirApp.relativeFilePath(rutaAbsoluta);

    ui->lineEditSSLCA->setText(rutaRelativa);
}
