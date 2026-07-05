#include "configlocal.h"
#include "ui_configlocal.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

/**
 * @brief Constructor: inicializa el formulario y carga los parámetros actuales desde tienda.ini.
 * @param parent Widget padre (habitualmente Tienda).
 */
ConfigLocal::ConfigLocal(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::ConfigLocal)
{
    ui->setupUi(this);

    // Cargar los datos actuales desde tienda.ini
    cargarDatos();
}

/**
 * @brief Destructor: libera la memoria de la interfaz generada por Qt.
 */
ConfigLocal::~ConfigLocal()
{
    delete ui;
}

/**
 * @brief Carga los parámetros de conexión almacenados en tienda.ini en el formulario.
 */
void ConfigLocal::cargarDatos()
{
    // SEC-01: Leer la ruta del archivo tienda.ini junto al ejecutable
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("BaseDatos");

    ui->lineEditDireccion->setText(settings.value("host", "localhost").toString());
    ui->lineEditPuerto->setText(settings.value("puerto", "3306").toString());
    ui->lineEditBaseDatos->setText(settings.value("baseDatos", "tiendaNueva").toString());
    ui->lineEditUsuario->setText(settings.value("usuario", "").toString());
    ui->lineEditClave->setText(settings.value("clave", "").toString());

    settings.endGroup();
}

/**
 * @brief Slot del botón "Probar conexión". Realiza un intento de conexión temporal a MariaDB.
 */
void ConfigLocal::on_pushButtonTest_clicked()
{
    QString host    = ui->lineEditDireccion->text().trimmed();
    QString puerto  = ui->lineEditPuerto->text().trimmed();
    QString bd      = ui->lineEditBaseDatos->text().trimmed();
    QString usuario = ui->lineEditUsuario->text().trimmed();
    QString clave   = ui->lineEditClave->text();

    if (host.isEmpty() || bd.isEmpty() || usuario.isEmpty()) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("Introduce al menos el servidor, la base de datos y el usuario."));
        return;
    }

    // Nombre único para la conexión de prueba para evitar colisiones
    const QString nombreTest = "TEST_LOCAL_CON";
    {
        QSqlDatabase dbTest = QSqlDatabase::addDatabase("QMYSQL", nombreTest);
        dbTest.setHostName(host);
        dbTest.setPort(puerto.toInt());
        dbTest.setDatabaseName(bd);
        dbTest.setUserName(usuario);
        dbTest.setPassword(clave);
        dbTest.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

        if (dbTest.open()) {
            QMessageBox::information(this, tr("Conexión correcta"),
                                     tr("✅ Conexión al servidor local establecida correctamente."));
            dbTest.close();
        } else {
            QMessageBox::critical(this, tr("Error de conexión"),
                                  tr("❌ No se pudo conectar a la base de datos local:\n") + dbTest.lastError().text());
        }
    }
    // Eliminar la conexión temporal tras la prueba
    QSqlDatabase::removeDatabase(nombreTest);
}

/**
 * @brief Slot del botón "Guardar". Almacena los cambios de forma permanente usando QSettings.
 */
void ConfigLocal::on_pushButtonGuardar_clicked()
{
    QString host    = ui->lineEditDireccion->text().trimmed();
    QString puerto  = ui->lineEditPuerto->text().trimmed();
    QString bd      = ui->lineEditBaseDatos->text().trimmed();
    QString usuario = ui->lineEditUsuario->text().trimmed();
    QString clave   = ui->lineEditClave->text();

    if (host.isEmpty() || bd.isEmpty() || usuario.isEmpty()) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("Introduce al menos el servidor, la base de datos y el usuario."));
        return;
    }

    // Confirmación del usuario antes de proceder a la reescritura
    QMessageBox::StandardButton res = QMessageBox::question(
        this, tr("Guardar configuración"),
        tr("¿Deseas guardar los cambios en tienda.ini y reconfigurar la base de datos local?"),
        QMessageBox::Yes | QMessageBox::No
    );

    if (res == QMessageBox::Yes) {
        // SEC-01: Utilizar QSettings para reescribir de forma limpia tienda.ini
        QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
        QSettings settings(iniPath, QSettings::IniFormat);
        settings.beginGroup("BaseDatos");
        settings.setValue("host", host);
        settings.setValue("puerto", puerto.toInt());
        settings.setValue("baseDatos", bd);
        settings.setValue("usuario", usuario);
        settings.setValue("clave", clave);
        settings.endGroup();
        settings.sync(); // Forzar sincronización con el disco físico

        QMessageBox::information(this, tr("Guardado con éxito"),
                                 tr("Configuración guardada en tienda.ini correctamente.\n"
                                    "Los cambios surtirán efecto al reiniciar la aplicación."));
        accept();
    }
}

/**
 * @brief Slot del botón "Cancelar". Cierra el diálogo sin aplicar cambios.
 */
void ConfigLocal::on_pushButtonCancelar_clicked()
{
    reject();
}
