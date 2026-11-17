#include "configlocal.h"
#include "configuracion.h"
#include "ui_configlocal.h"
#include <QCoreApplication>
#include <QFileInfo>
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

    // Cargar los datos actuales desde tienda.ini (por usuario o global)
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
    // Determinar si el usuario actual de Linux ya tiene una conexión personalizada
    const bool usaPorUsuario = Configuracion::tieneConexionPorUsuario();
    const QString iniPath = Configuracion::rutaIniConexion();

    ui->checkBoxPorUsuario->setChecked(usaPorUsuario);
    ui->labelRutaActiva->setText(
        usaPorUsuario
            ? tr("Archivo activo (Usuario): %1").arg(iniPath)
            : tr("Archivo activo (Global): %1").arg(iniPath));

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
        const bool porUsuario = ui->checkBoxPorUsuario->isChecked();
        const QString userIni = Configuracion::rutaIniUsuario();
        const QString globalIni = Configuracion::rutaIniGlobal();

        if (porUsuario) {
            // Guardar exclusivamente en el archivo del usuario (~/.config/Ladis/tienda.ini)
            QSettings settings(userIni, QSettings::IniFormat);
            settings.beginGroup("BaseDatos");
            settings.setValue("host", host);
            settings.setValue("puerto", puerto.toInt());
            settings.setValue("baseDatos", bd);
            settings.setValue("usuario", usuario);
            settings.setValue("clave", clave);
            settings.endGroup();
            settings.sync();

            QMessageBox::information(this, tr("Guardado con éxito"),
                                     tr("Configuración guardada para este usuario en:\n%1\n\n"
                                        "Los cambios surtirán efecto al reiniciar la aplicación.")
                                         .arg(userIni));
        } else {
            // Eliminar sobreescritura por usuario si existía para que herede la global
            if (QFile::exists(userIni)) {
                QSettings settingsUser(userIni, QSettings::IniFormat);
                settingsUser.remove("BaseDatos");
                settingsUser.sync();
            }

            // Guardar en el archivo global (/opt/tienda/tienda.ini)
            QSettings settingsGlobal(globalIni, QSettings::IniFormat);
            settingsGlobal.beginGroup("BaseDatos");
            settingsGlobal.setValue("host", host);
            settingsGlobal.setValue("puerto", puerto.toInt());
            settingsGlobal.setValue("baseDatos", bd);
            settingsGlobal.setValue("usuario", usuario);
            settingsGlobal.setValue("clave", clave);
            settingsGlobal.endGroup();
            settingsGlobal.sync();

            if (settingsGlobal.status() != QSettings::NoError) {
                QMessageBox::warning(this, tr("Permisos en archivo global"),
                                     tr("Se ha restablecido este usuario para usar la conexión global (%1),\n"
                                        "pero el usuario actual de Linux no tiene permisos de escritura sobre ese archivo.\n"
                                        "Si deseas una conexión propia para este usuario, marca la casilla de configuración por usuario.")
                                         .arg(globalIni));
            } else {
                QMessageBox::information(this, tr("Guardado con éxito"),
                                         tr("Configuración global guardada en:\n%1\n\n"
                                            "Los cambios surtirán efecto al reiniciar la aplicación.")
                                             .arg(globalIni));
            }
        }
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
