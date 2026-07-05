#include "login.h"
#include "ui_login.h"
#include "hashutil.h"
#include <QMessageBox>
#include "conexion.h"
#include <QSqlRecord>
#include <QFormLayout>
#include <QLabel>
#include <QDialogButtonBox>

Login::Login(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Login)
    , m_intentosFallidos(0)
{
    ui->setupUi(this);

    // SEC-05: Conectar el timer de bloqueo para rehabilitar el botón
    connect(&m_timerBloqueo, &QTimer::timeout, this, [this]() {
        ui->pushButton->setEnabled(true);
        ui->pushButton->setText(tr("Entrar"));
    });

    // Cargar las tiendas configuradas en la BD local ("DB")
    QSqlDatabase dbDefault = QSqlDatabase::database("DB");
    if (dbDefault.isOpen()) {
        QSqlQuery qTiendas(dbDefault);
        if (qTiendas.exec("SELECT nombre, ip, usuario, password, baseDatos, puerto, ssl_ca, local FROM tiendas ORDER BY local DESC, nombre ASC")) {
            while (qTiendas.next()) {
                QString nombre = qTiendas.value("nombre").toString();
                bool isLocal = qTiendas.value("local").toInt() == 1;

                QVariantMap conInfo;
                conInfo["nombre"] = nombre;
                conInfo["ip"] = qTiendas.value("ip").toString();
                conInfo["usuario"] = qTiendas.value("usuario").toString();
                conInfo["password"] = qTiendas.value("password").toString();
                conInfo["baseDatos"] = qTiendas.value("baseDatos").toString();
                conInfo["puerto"] = qTiendas.value("puerto").toString();
                conInfo["ssl_ca"] = qTiendas.value("ssl_ca").toString();
                conInfo["local"] = isLocal;

                ui->comboBoxTienda->addItem(isLocal ? QString("%1 (Local)").arg(nombre) : nombre, conInfo);
            }
        }
    }

    // Si la tabla de tiendas está vacía, añadir la opción por defecto
    if (ui->comboBoxTienda->count() == 0) {
        QVariantMap conInfo;
        conInfo["nombre"] = "DB";
        conInfo["local"] = true;
        ui->comboBoxTienda->addItem(tr("Base de datos local (DB)"), conInfo);
    }

    m_prevTiendaIndex = ui->comboBoxTienda->currentIndex();
    connect(ui->comboBoxTienda, &QComboBox::currentIndexChanged, this, &Login::on_comboBoxTienda_currentIndexChanged);
}

Login::~Login()
{
    delete ui;
}

/**
 * @brief Slot del botón "Entrar".
 *
 * Autentica al usuario contra la tabla 'usuarios' usando Prepared Statements.
 * SEC-02: Soporta contraseñas hasheadas (SHA-256+salt) y texto plano (migración progresiva).
 * SEC-05: Protección anti-fuerza bruta con bloqueo temporal.
 */
void Login::on_pushButton_clicked()
{
    QString usuario = ui->lineEditUser->text().trimmed();
    QString password = ui->lineEditpwd->text();

    // Validación básica de campos vacíos
    if (usuario.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, tr("Datos incompletos"),
                             tr("Introduce usuario y contraseña."));
        return;
    }

    // Obtener la información de conexión de la tienda seleccionada
    QVariantMap conInfo = ui->comboBoxTienda->currentData().toMap();
    QString connectionName = "DB"; // Conexión local por defecto

    if (!conInfo.isEmpty() && !conInfo.value("local").toBool()) {
        connectionName = conInfo.value("nombre").toString();
        // Si la conexión remota no está registrada o no está abierta, la abrimos
        if (!QSqlDatabase::contains(connectionName) || !QSqlDatabase::database(connectionName).isOpen()) {
            QString host = conInfo.value("ip").toString();
            QString puerto = conInfo.value("puerto").toString();
            if (puerto.isEmpty() || puerto == "0") puerto = "3306";
            QString baseDatos = conInfo.value("baseDatos").toString();
            if (baseDatos.isEmpty()) baseDatos = "tiendaNueva";
            QString dbUser = conInfo.value("usuario").toString();
            QString dbPass = conInfo.value("password").toString();
            QString sslCa = conInfo.value("ssl_ca").toString();

            if (!createConnection(host, puerto, baseDatos, dbUser, dbPass, connectionName, sslCa)) {
                QMessageBox::critical(this, tr("Error de conexión"),
                                     tr("No se pudo establecer conexión con la tienda remota '%1'.").arg(connectionName));
                return;
            }
        }
    }

    // Establecer la conexión elegida como la conexión local activa
    conf->setConexionLocal(connectionName);

    // SEC-02: Consultar usuario, clave y salt (si existe la columna)
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery consulta(db);

    // Comprobar si la columna salt existe en la tabla usuarios
    bool tieneSalt = false;
    QSqlQuery checkCol(db);
    if (checkCol.exec("SHOW COLUMNS FROM usuarios LIKE 'salt'")) {
        tieneSalt = checkCol.next();
    }

    if (tieneSalt) {
        consulta.prepare("SELECT nombre, rol, clave, salt FROM usuarios WHERE usuario = ?");
    } else {
        consulta.prepare("SELECT nombre, rol, clave FROM usuarios WHERE usuario = ?");
    }
    consulta.bindValue(0, usuario);

    bool autenticado = false;
    int rol = -1;

    if (consulta.exec() && consulta.next()) {
        QString claveAlmacenada = consulta.value(2).toString();
        QString saltAlmacenado = tieneSalt ? consulta.value(3).toString() : "";
        rol = consulta.value(1).toInt();

        if (!saltAlmacenado.isEmpty()) {
            // SEC-02: Contraseña ya migrada a hash — verificar con SHA-256
            autenticado = HashUtil::verificarPassword(password, claveAlmacenada, saltAlmacenado);
        } else {
            // Contraseña aún en texto plano — comparar directamente
            autenticado = (password == claveAlmacenada);

            // SEC-02: Auto-migrar al primer login exitoso
            if (autenticado && tieneSalt) {
                QString nuevoSalt = HashUtil::generarSalt();
                QString nuevoHash = HashUtil::hashPassword(password, nuevoSalt);
                QSqlQuery migrar(db);
                migrar.prepare("UPDATE usuarios SET clave = ?, salt = ? WHERE usuario = ?");
                migrar.bindValue(0, nuevoHash);
                migrar.bindValue(1, nuevoSalt);
                migrar.bindValue(2, usuario);
                if (migrar.exec()) {
                    qDebug() << "SEC-02: Contraseña migrada a hash para usuario:" << usuario;
                }
            }
        }
    }

    if (autenticado) {
        // Login correcto: resetear contador y aceptar
        m_intentosFallidos = 0;
        conf->setUsuario(usuario);
        conf->setRol(rol);

        // Registrar login exitoso en el log
        baseDatos base;
        base.insertarLog(conf->getConexionLocal(), "Info", usuario,
                         "Inicio de sesión correcto");
        accept();
    } else {
        // SEC-05: Login fallido — incrementar contador y registrar
        m_intentosFallidos++;

        // Registrar intento fallido en el log (SEC-05)
        baseDatos base;
        base.insertarLog(conf->getConexionLocal(), "Seguridad", usuario,
                         QString("Intento de login fallido (%1/%2)")
                             .arg(m_intentosFallidos)
                             .arg(MAX_INTENTOS));

        if (m_intentosFallidos >= MAX_INTENTOS) {
            // Bloquear el botón durante BLOQUEO_MS milisegundos
            ui->pushButton->setEnabled(false);
            ui->pushButton->setText(tr("Bloqueado (%1s)")
                                        .arg(BLOQUEO_MS / 1000));
            m_timerBloqueo.setSingleShot(true);
            m_timerBloqueo.start(BLOQUEO_MS);
            m_intentosFallidos = 0; // Resetear tras el bloqueo

            QMessageBox::critical(this, tr("Acceso bloqueado"),
                tr("Demasiados intentos fallidos.\n"
                   "El acceso queda bloqueado durante %1 segundos.")
                    .arg(BLOQUEO_MS / 1000));
        } else {
            QMessageBox::warning(this, tr("Error de autenticación"),
                tr("Usuario o contraseña incorrectos.\n"
                   "Intentos restantes: %1")
                    .arg(MAX_INTENTOS - m_intentosFallidos));
        }

        // Limpiar el campo de contraseña para el siguiente intento
        ui->lineEditpwd->clear();
        ui->lineEditpwd->setFocus();
    }
}

/**
 * @brief Muestra un diálogo solicitando autorización de un administrador local.
 *
 * Busca en la base de datos "DB" (local) un usuario con rol = 0 (Administrador)
 * y valida sus credenciales utilizando la misma lógica de login con soporte para hash y salt.
 */
static bool solicitarAutorizacionAdmin(QWidget *parent) {
    QDialog dialog(parent);
    dialog.setWindowTitle(QDialog::tr("Autorización de Administrador"));
    dialog.setMinimumWidth(320);

    QFormLayout form(&dialog);
    form.addRow(new QLabel(QDialog::tr("Se requiere autorización de un administrador local\npara cambiar la base de datos de trabajo.")));

    QLineEdit *userEdit = new QLineEdit(&dialog);
    userEdit->setPlaceholderText(QDialog::tr("Usuario Administrador"));
    form.addRow(QDialog::tr("Usuario:"), userEdit);

    QLineEdit *passEdit = new QLineEdit(&dialog);
    passEdit->setEchoMode(QLineEdit::Password);
    passEdit->setPlaceholderText(QDialog::tr("Contraseña"));
    form.addRow(QDialog::tr("Contraseña:"), passEdit);

    QDialogButtonBox buttonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal, &dialog);
    form.addRow(&buttonBox);

    QObject::connect(&buttonBox, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    QObject::connect(&buttonBox, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    while (dialog.exec() == QDialog::Accepted) {
        QString user = userEdit->text().trimmed();
        QString pass = passEdit->text();

        if (user.isEmpty() || pass.isEmpty()) {
            QMessageBox::warning(&dialog, QDialog::tr("Datos incompletos"),
                                 QDialog::tr("Introduce usuario y contraseña."));
            continue;
        }

        // Validar contra la base de datos local "DB"
        QSqlDatabase db = QSqlDatabase::database("DB");
        if (!db.isOpen()) {
            QMessageBox::critical(&dialog, QDialog::tr("Error"),
                                 QDialog::tr("La base de datos local no está disponible."));
            return false;
        }

        QSqlQuery query(db);
        bool tieneSalt = false;
        QSqlQuery checkCol(db);
        if (checkCol.exec("SHOW COLUMNS FROM usuarios LIKE 'salt'")) {
            tieneSalt = checkCol.next();
        }

        if (tieneSalt) {
            query.prepare("SELECT clave, salt, rol FROM usuarios WHERE usuario = ? AND rol = 0");
        } else {
            query.prepare("SELECT clave, rol FROM usuarios WHERE usuario = ? AND rol = 0");
        }
        query.bindValue(0, user);

        if (query.exec() && query.next()) {
            QString hashAlmacenado = query.value(0).toString();
            QString saltAlmacenado = tieneSalt ? query.value(1).toString() : "";

            bool ok = false;
            if (!saltAlmacenado.isEmpty()) {
                ok = HashUtil::verificarPassword(pass, hashAlmacenado, saltAlmacenado);
            } else {
                ok = (pass == hashAlmacenado);
            }

            if (ok) {
                return true;
            }
        }

        QMessageBox::warning(&dialog, QDialog::tr("Autorización fallida"),
                             QDialog::tr("Credenciales incorrectas o el usuario no es administrador."));
    }

    return false;
}

void Login::on_comboBoxTienda_currentIndexChanged(int index)
{
    if (index == m_prevTiendaIndex) return;

    // Obtener la información del elemento previamente seleccionado
    QVariantMap prevInfo = ui->comboBoxTienda->itemData(m_prevTiendaIndex).toMap();
    bool prevWasLocal = prevInfo.value("local").toBool();

    // Requerir autorización de administrador únicamente si se cambia desde la base de datos local (por defecto)
    if (prevWasLocal) {
        if (!solicitarAutorizacionAdmin(this)) {
            // Revertir la selección sin disparar de nuevo la señal de cambio
            ui->comboBoxTienda->blockSignals(true);
            ui->comboBoxTienda->setCurrentIndex(m_prevTiendaIndex);
            ui->comboBoxTienda->blockSignals(false);
            return;
        }
    }

    m_prevTiendaIndex = index;
}


