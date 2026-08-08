#include "dialogasistenteinstalacion.h"
#include "ui_dialogasistenteinstalacion.h"
#include <QCoreApplication>
#include <QMessageBox>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QStyle>
#include <QDebug>

/**
 * @brief Constructor del asistente interactivo de instalación.
 * Configura la interfaz, asigna los iconos del sistema e inicializa la vista de páginas.
 *
 * @param parent Widget padre.
 */
DialogAsistenteInstalacion::DialogAsistenteInstalacion(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAsistenteInstalacion)
{
    ui->setupUi(this);

    // Asignar los iconos según el estilo global del programa
    ui->pushButtonTest->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonGuardar->setIcon(style()->standardIcon(QStyle::SP_DialogOkButton));
    ui->pushButtonCancelar->setIcon(style()->standardIcon(QStyle::SP_DialogCancelButton));

    // Conectar los eventos de cambio de selección en los modos de instalación
    connect(ui->radioNuevaEmpresa, &QRadioButton::toggled, this, &DialogAsistenteInstalacion::on_radioNuevaEmpresa_toggled);
    connect(ui->radioUnirGrupo, &QRadioButton::toggled, this, &DialogAsistenteInstalacion::on_radioUnirGrupo_toggled);
    connect(ui->radioNuevoTPV, &QRadioButton::toggled, this, &DialogAsistenteInstalacion::on_radioNuevoTPV_toggled);

    // Seleccionar por defecto la primera página (Nueva Empresa)
    ui->stackedWidgetParametros->setCurrentIndex(0);
}

/**
 * @brief Destructor del diálogo. Libera la memoria de los componentes gráficos.
 */
DialogAsistenteInstalacion::~DialogAsistenteInstalacion()
{
    delete ui;
}

/**
 * @brief Slot activado al seleccionar el Modo 1: Nueva Empresa / Tienda Principal.
 */
void DialogAsistenteInstalacion::on_radioNuevaEmpresa_toggled(bool checked)
{
    if (checked) {
        ui->stackedWidgetParametros->setCurrentIndex(0);
    }
}

/**
 * @brief Slot activado al seleccionar el Modo 2: Unirse a un Grupo de Tiendas Existente.
 */
void DialogAsistenteInstalacion::on_radioUnirGrupo_toggled(bool checked)
{
    if (checked) {
        ui->stackedWidgetParametros->setCurrentIndex(1);
    }
}

/**
 * @brief Slot activado al seleccionar el Modo 3: Nuevo Punto de Venta (TPV Secundario en misma tienda).
 */
void DialogAsistenteInstalacion::on_radioNuevoTPV_toggled(bool checked)
{
    if (checked) {
        ui->stackedWidgetParametros->setCurrentIndex(2);
    }
}

/**
 * @brief Método auxiliar para probar temporalmente la conexión con MariaDB/MySQL.
 */
bool DialogAsistenteInstalacion::probarConexion(const QString &host, const QString &puerto,
                                               const QString &bd, const QString &usuario,
                                               const QString &clave)
{
    const QString connName = "TEST_INSTALL_WIZARD";
    bool ok = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", connName);
        db.setHostName(host);
        db.setPort(puerto.toInt() > 0 ? puerto.toInt() : 3306);
        db.setDatabaseName(bd);
        db.setUserName(usuario);
        db.setPassword(clave);
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

        if (db.open()) {
            ok = true;
            db.close();
        } else {
            qDebug() << "Error de conexión en asistente:" << db.lastError().text();
        }
    }
    QSqlDatabase::removeDatabase(connName);
    return ok;
}

/**
 * @brief Slot del botón "Probar Conexión". Realiza la prueba de red según la página activa.
 */
void DialogAsistenteInstalacion::on_pushButtonTest_clicked()
{
    int index = ui->stackedWidgetParametros->currentIndex();
    QString host, puerto, bd, user, pass;

    if (index == 0) { // Modo 1: Nueva Empresa
        host   = ui->lineEditHostP0->text().trimmed();
        puerto = ui->lineEditPuertoP0->text().trimmed();
        bd     = ui->lineEditBDP0->text().trimmed();
        user   = ui->lineEditUserP0->text().trimmed();
        pass   = ui->lineEditPassP0->text();
    } else if (index == 1) { // Modo 2: Unirse a grupo
        host   = "localhost";
        puerto = "3306";
        bd     = ui->lineEditBDLocalP1->text().trimmed();
        user   = ui->lineEditUserLocalP1->text().trimmed();
        pass   = ui->lineEditPassLocalP1->text();
    } else { // Modo 3: TPV Secundario
        host   = ui->lineEditIPServidorP2->text().trimmed();
        puerto = ui->lineEditPuertoP2->text().trimmed();
        bd     = ui->lineEditBDP2->text().trimmed();
        user   = ui->lineEditUserP2->text().trimmed();
        pass   = ui->lineEditPassP2->text();
    }

    if (host.isEmpty() || bd.isEmpty() || user.isEmpty()) {
        QMessageBox::warning(this, tr("Datos Incompletos"),
                             tr("Por favor, rellene al menos el Servidor, la Base de Datos y el Usuario."));
        return;
    }

    if (probarConexion(host, puerto, bd, user, pass)) {
        QMessageBox::information(this, tr("Conexión Correcta"),
                                 tr("✅ Conexión establecida con éxito con el servidor MariaDB. La base de datos ya existe."));
    } else {
        // Si no se puede conectar a la BD, probamos a conectar sin base de datos (vacía)
        if (probarConexion(host, puerto, "", user, pass)) {
            QMessageBox::information(this, tr("Conexión Correcta"),
                                     tr("✅ Conexión establecida con éxito con el servidor MariaDB.\n"
                                        "La base de datos '%1' no existe y se creará automáticamente cuando pulses 'Guardar e Iniciar'.").arg(bd));
        } else {
            QMessageBox::critical(this, tr("Error de Conexión"),
                                  tr("❌ No se pudo conectar al servidor MariaDB con los parámetros especificados.\n"
                                     "Verifique que MariaDB esté iniciado y que el usuario y contraseña sean correctos."));
        }
    }
}

/**
 * @brief Guarda la configuración estructurada en el archivo tienda.ini mediante QSettings.
 */
void DialogAsistenteInstalacion::guardarConfiguracionIni(const QString &host, const QString &puerto,
                                                         const QString &bd, const QString &usuario,
                                                         const QString &clave, const QString &nombreTienda,
                                                         const QString &hostNube, const QString &bdNube,
                                                         const QString &userNube, const QString &passNube)
{
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);

    settings.beginGroup("BaseDatos");
    settings.setValue("host", host);
    settings.setValue("puerto", puerto.isEmpty() ? "3306" : puerto);
    settings.setValue("baseDatos", bd);
    settings.setValue("usuario", usuario);
    settings.setValue("clave", clave);
    settings.endGroup();

    settings.beginGroup("Local");
    settings.setValue("nombreTienda", nombreTienda);
    settings.setValue("puertoImpresora", "/dev/usb/lp0");
    settings.endGroup();

    if (!hostNube.isEmpty()) {
        settings.beginGroup("Nube");
        settings.setValue("host", hostNube);
        settings.setValue("puerto", "3306");
        settings.setValue("baseDatos", bdNube);
        settings.setValue("usuario", userNube);
        settings.setValue("clave", passNube);
        settings.endGroup();
    }

    settings.sync();
}

/**
 * @brief Slot del botón "Guardar e Iniciar". Almacena los parámetros y acepta el diálogo.
 */
void DialogAsistenteInstalacion::on_pushButtonGuardar_clicked()
{
    int index = ui->stackedWidgetParametros->currentIndex();
    QString host, puerto, bd, user, pass, nombreTienda;
    QString hostNube, bdNube, userNube, passNube;

    if (index == 0) { // Modo 1: Nueva Empresa
        host         = ui->lineEditHostP0->text().trimmed();
        puerto       = ui->lineEditPuertoP0->text().trimmed();
        bd           = ui->lineEditBDP0->text().trimmed();
        user         = ui->lineEditUserP0->text().trimmed();
        pass         = ui->lineEditPassP0->text();
        nombreTienda = ui->lineEditNombreTiendaP0->text().trimmed();
    } else if (index == 1) { // Modo 2: Unirse a grupo
        host         = "localhost";
        puerto       = "3306";
        bd           = ui->lineEditBDLocalP1->text().trimmed();
        user         = ui->lineEditUserLocalP1->text().trimmed();
        pass         = ui->lineEditPassLocalP1->text();
        nombreTienda = ui->lineEditNombreTiendaP1->text().trimmed();

        hostNube     = ui->lineEditHostNubeP1->text().trimmed();
        bdNube       = ui->lineEditBDNubeP1->text().trimmed();
        userNube     = ui->lineEditUserNubeP1->text().trimmed();
        passNube     = ui->lineEditPassNubeP1->text();
    } else { // Modo 3: TPV Secundario en la misma tienda
        host         = ui->lineEditIPServidorP2->text().trimmed();
        puerto       = ui->lineEditPuertoP2->text().trimmed();
        bd           = ui->lineEditBDP2->text().trimmed();
        user         = ui->lineEditUserP2->text().trimmed();
        pass         = ui->lineEditPassP2->text();
        nombreTienda = ui->lineEditNombreTPVP2->text().trimmed();
    }

    if (host.isEmpty() || bd.isEmpty() || user.isEmpty()) {
        QMessageBox::warning(this, tr("Datos Incompletos"),
                             tr("Complete los campos obligatorios antes de continuar."));
        return;
    }

    // 1. Probar la conexión a la base de datos local. Si falla, intentar crearla con tiendaNueva_schema
    if (!probarConexion(host, puerto, bd, user, pass)) {
        QMessageBox::StandardButton resp = QMessageBox::question(
            this, tr("Base de datos local no encontrada"),
            tr("La conexión a la base de datos local falló o no existe.\n"
               "¿Desea que el asistente intente crear la base de datos '%1' e importar la estructura completa automáticamente?").arg(bd),
            QMessageBox::Yes | QMessageBox::No
        );
        if (resp == QMessageBox::Yes) {
            // Crear base de datos local e importar el esquema tiendaNueva_schema
            if (crearBDYEsquema(host, puerto, bd, user, pass, ":/tiendaNueva_schema-MIRAR NOTAS AL FINAL.sql")) {
                QMessageBox::information(this, tr("Base de datos local creada"),
                                         tr("✅ Base de datos local '%1' creada e inicializada correctamente.").arg(bd));
            } else {
                QMessageBox::critical(this, tr("Error de instalación"),
                                      tr("❌ No se pudo crear o inicializar la base de datos local automáticamente.\n"
                                         "Verifique los permisos del usuario del servidor MariaDB/MySQL."));
                return;
            }
        } else {
            // Preguntar si quiere guardar de todas formas
            QMessageBox::StandardButton resp2 = QMessageBox::question(
                this, tr("Advertencia de Conexión"),
                tr("¿Desea guardar los datos de todas formas e intentar iniciar la aplicación?"),
                QMessageBox::Yes | QMessageBox::No
            );
            if (resp2 != QMessageBox::Yes) {
                return;
            }
        }
    }

    // 2. Si se han configurado parámetros de la nube, actuar según el modo de instalación
    if (!hostNube.isEmpty() && !bdNube.isEmpty()) {
        if (index == 1) { // Modo 2: Unirse a un grupo de tiendas existente
            // En este modo la nube ya existe con datos, NO debemos crear ni importar nada en ella.
            // Solo probamos conexión para asegurar que es correcta.
            if (!probarConexion(hostNube, "3306", bdNube, userNube, passNube)) {
                QMessageBox::critical(this, tr("Conexión a Nube Fallida"),
                                      tr("❌ No se pudo establecer conexión con la base de datos de la nube.\n"
                                         "En el modo 'Unirse a grupo de tiendas', la base de datos en la nube ya debe existir y estar poblada.\n"
                                         "Verifique la dirección, credenciales y que no esté bloqueado el puerto 3306 en el servidor."));
                return;
            }
        } else if (index == 0) { // Modo 1: Nueva Empresa (si tuviera nube configurada)
            // Para nueva empresa, si no existe la BD en la nube, la creamos y poblamos con tiendaNube_schema.sql
            if (!probarConexion(hostNube, "3306", bdNube, userNube, passNube)) {
                if (crearBDYEsquema(hostNube, "3306", bdNube, userNube, passNube, ":/tiendaNube_schema.sql")) {
                    QMessageBox::information(this, tr("Base de datos de Nube Creada"),
                                             tr("✅ Base de datos de la nube '%1' creada e inicializada correctamente.").arg(bdNube));
                } else {
                    QMessageBox::critical(this, tr("Error de Nube"),
                                          tr("❌ No se pudo crear o inicializar la base de datos de la nube automáticamente."));
                    return;
                }
            }
        }

        // Para ambos casos (Modos 1 y 2 con Nube), guardamos las credenciales de la nube en la tabla 'config_nube' local
        const QString connLocalName = "SYNC_INIT_LOCAL";
        const QString connNubeName = "SYNC_INIT_NUBE";
        {
            QSqlDatabase dbLocal = QSqlDatabase::addDatabase("QMYSQL", connLocalName);
            dbLocal.setHostName(host);
            dbLocal.setPort(puerto.toInt() > 0 ? puerto.toInt() : 3306);
            dbLocal.setDatabaseName(bd);
            dbLocal.setUserName(user);
            dbLocal.setPassword(pass);

            QSqlDatabase dbNube = QSqlDatabase::addDatabase("QMYSQL", connNubeName);
            dbNube.setHostName(hostNube);
            dbNube.setPort(3306);
            dbNube.setDatabaseName(bdNube);
            dbNube.setUserName(userNube);
            dbNube.setPassword(passNube);

            if (dbLocal.open() && dbNube.open()) {
                // Escribir config_nube local
                QSqlQuery qConfig(dbLocal);
                qConfig.exec("CREATE TABLE IF NOT EXISTS config_nube ("
                             "  id       INT          NOT NULL DEFAULT 1 PRIMARY KEY,"
                             "  servidor VARCHAR(255) NOT NULL DEFAULT '',"
                             "  puerto   INT          NOT NULL DEFAULT 3306,"
                             "  baseDatos VARCHAR(100) NOT NULL DEFAULT '',"
                             "  usuario  VARCHAR(100) NOT NULL DEFAULT '',"
                             "  clave    VARCHAR(255) NOT NULL DEFAULT '',"
                             "  ssl_ca   VARCHAR(512)          DEFAULT ''"
                             ") ENGINE=InnoDB DEFAULT CHARSET=utf8mb4");

                qConfig.prepare("INSERT INTO config_nube (id, servidor, puerto, baseDatos, usuario, clave) "
                                "VALUES (?, ?, ?, ?, ?, ?) "
                                "ON DUPLICATE KEY UPDATE "
                                "servidor = VALUES(servidor), puerto = VALUES(puerto), baseDatos = VALUES(baseDatos), "
                                "usuario = VALUES(usuario), clave = VALUES(clave)");
                qConfig.addBindValue(1);
                qConfig.addBindValue(hostNube);
                qConfig.addBindValue(3306);
                qConfig.addBindValue(bdNube);
                qConfig.addBindValue(userNube);
                qConfig.addBindValue(passNube);
                if (!qConfig.exec()) {
                    qDebug() << "❌ Error al insertar en config_nube:" << qConfig.lastError().text();
                } else {
                    qDebug() << "✅ Configuración de la nube guardada con éxito en la tabla config_nube.";
                }

                // Si es Modo 2 (Unirse a grupo), importamos inicialmente usuarios y permisos para el login
                if (index == 1) {
                    bool importUsers = importarTablaDesdeNube("usuarios", dbLocal, dbNube);
                    bool importPerms = importarTablaDesdeNube("permisos", dbLocal, dbNube);
                    if (importUsers && importPerms) {
                        qDebug() << "Tablas 'usuarios' y 'permisos' importadas desde la nube con éxito.";
                    } else {
                        QMessageBox::warning(this, tr("Advertencia de Sincronización"),
                                             tr("⚠️ No se pudieron descargar completamente los usuarios y permisos desde la nube.\n"
                                                "Es posible que deba crearlos manualmente en la base de datos local para poder iniciar sesión."));
                    }
                }
                dbLocal.close();
                dbNube.close();
            }
        }
        QSqlDatabase::removeDatabase(connLocalName);
        QSqlDatabase::removeDatabase(connNubeName);
    }

    // Guardar los parámetros en tienda.ini
    guardarConfiguracionIni(host, puerto, bd, user, pass, nombreTienda,
                            hostNube, bdNube, userNube, passNube);

    QMessageBox::information(this, tr("Configuración Guardada"),
                             tr("Se han guardado los parámetros de instalación en 'tienda.ini'. Iniciando Tienda 2.0..."));

    accept();
}

/**
 * @brief Slot del botón "Cancelar". Cierra el asistente rechazando el diálogo.
 */
void DialogAsistenteInstalacion::on_pushButtonCancelar_clicked()
{
    reject();
}

/**
 * @brief Crea la base de datos completa e importa el esquema SQL si no existe.
 */
bool DialogAsistenteInstalacion::crearBDYEsquema(const QString &host, const QString &puerto, const QString &bd,
                                                 const QString &usuario, const QString &clave, const QString &resourcePath)
{
    const QString tempConnName = "TEMP_CREAR_BD";
    bool ok = false;

    // 1. Conectar al servidor MySQL sin especificar base de datos (usando la del sistema "mysql")
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", tempConnName);
        db.setHostName(host);
        db.setPort(puerto.toInt() > 0 ? puerto.toInt() : 3306);
        db.setDatabaseName("");
        db.setUserName(usuario);
        db.setPassword(clave);
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=5");

        if (db.open()) {
            QSqlQuery query(db);
            // 2. Crear la base de datos si no existe
            QString sqlCreate = QString("CREATE DATABASE IF NOT EXISTS `%1` CHARACTER SET utf8 COLLATE utf8_general_ci;").arg(bd);
            if (query.exec(sqlCreate)) {
                qDebug() << "Base de datos creada o ya existente:" << bd;
                ok = true;
            } else {
                qDebug() << "Error al crear la base de datos:" << query.lastError().text();
            }
            db.close();
        } else {
            qDebug() << "Error al conectar al servidor MariaDB para crear la base de datos:" << db.lastError().text();
        }
    }
    QSqlDatabase::removeDatabase(tempConnName);

    if (!ok) {
        return false;
    }

    // 3. Conectar a la base de datos recién creada e importar el esquema
    const QString schemaConnName = "SCHEMA_CREAR_BD";
    bool schemaOk = false;
    {
        QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", schemaConnName);
        db.setHostName(host);
        db.setPort(puerto.toInt() > 0 ? puerto.toInt() : 3306);
        db.setDatabaseName(bd);
        db.setUserName(usuario);
        db.setPassword(clave);
        db.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=10");

        if (db.open()) {
            // 4. Ejecutar el script SQL de la ruta indicada
            schemaOk = ejecutarSqlScript(db, resourcePath);
            db.close();
        } else {
            qDebug() << "Error al conectar a la nueva base de datos para importar esquema:" << db.lastError().text();
        }
    }
    QSqlDatabase::removeDatabase(schemaConnName);

    return schemaOk;
}

/**
 * @brief Ejecuta línea a línea un script de sentencias SQL.
 */
bool DialogAsistenteInstalacion::ejecutarSqlScript(QSqlDatabase &db, const QString &resourcePath)
{
    QFile file(resourcePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "No se pudo abrir el recurso de base de datos:" << resourcePath;
        return false;
    }

    QTextStream in(&file);
    QSqlQuery query(db);
    QString currentQuery;
    bool inComment = false;

    // Usar transacción para optimizar la inserción de todas las tablas e índices
    db.transaction();

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();

        // Ignorar líneas vacías y comentarios de una sola línea
        if (line.isEmpty() || line.startsWith("--") || line.startsWith("#")) {
            continue;
        }

        // Ignorar comentarios de bloque normales (que no sean ejecutables de MySQL/MariaDB)
        if (line.startsWith("/*") && !line.startsWith("/*!") && !line.startsWith("/*M!")) {
            if (line.endsWith("*/") || line.endsWith("*/;")) {
                continue;
            }
            inComment = true;
            continue;
        }

        if (inComment) {
            if (line.contains("*/")) {
                inComment = false;
            }
            continue;
        }

        currentQuery += line + "\n";

        // Si la línea actual termina con ';', ejecutamos el lote
        if (line.endsWith(";")) {
            if (!query.exec(currentQuery)) {
                qDebug() << "Error ejecutando consulta:" << query.lastError().text();
                qDebug() << "Consulta fallida:" << currentQuery;
                db.rollback();
                return false;
            }
            currentQuery.clear();
        }
    }

    db.commit();
    return true;
}

#include <QSqlRecord>

bool DialogAsistenteInstalacion::importarTablaDesdeNube(const QString &nombreTabla, QSqlDatabase &dbLocal, QSqlDatabase &dbNube)
{
    QSqlQuery queryNube(dbNube);
    if (!queryNube.exec(QString("SELECT * FROM `%1`").arg(nombreTabla))) {
        qDebug() << "Error al consultar tabla remota:" << nombreTabla << queryNube.lastError().text();
        return false;
    }

    QSqlQuery queryLocal(dbLocal);
    
    // Desactivar temporalmente restricciones de clave foránea
    queryLocal.exec("SET FOREIGN_KEY_CHECKS=0;");
    queryLocal.exec(QString("DELETE FROM `%1`").arg(nombreTabla));

    QSqlRecord record = queryNube.record();
    dbLocal.transaction();

    while (queryNube.next()) {
        QStringList campos;
        QStringList placeholders;
        QVariantList valores;

        for (int i = 0; i < record.count(); ++i) {
            campos << QString("`%1`").arg(record.fieldName(i));
            placeholders << "?";
            valores << queryNube.value(i);
        }

        QString sql = QString("INSERT INTO `%1` (%2) VALUES (%3)")
                          .arg(nombreTabla)
                          .arg(campos.join(", "))
                          .arg(placeholders.join(", "));

        queryLocal.prepare(sql);
        for (int i = 0; i < valores.size(); ++i) {
            queryLocal.addBindValue(valores[i]);
        }

        if (!queryLocal.exec()) {
            qDebug() << "Error al insertar en tabla local:" << nombreTabla << queryLocal.lastError().text();
            dbLocal.rollback();
            queryLocal.exec("SET FOREIGN_KEY_CHECKS=1;");
            return false;
        }
    }

    dbLocal.commit();
    queryLocal.exec("SET FOREIGN_KEY_CHECKS=1;");
    return true;
}
