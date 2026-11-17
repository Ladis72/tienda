#include "dialogsyncimagenes.h"
#include "configuracion.h"
#include "ui_dialogsyncimagenes.h"

#include <QDateTime>
#include <QDir>
#include <QDirIterator>
#include <QFileDialog>
#include <QFileInfo>
#include <QHeaderView>
#include <QHostAddress>
#include <QHostInfo>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QScrollBar>
#include <QSettings>

/**
 * @brief Constructor del diálogo de sincronización de imágenes entre equipos TPV.
 */
DialogSyncImagenes::DialogSyncImagenes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogSyncImagenes)
    , m_proceso(new QProcess(this))
    , m_totalTareas(0)
    , m_tareasCompletadas(0)
    , m_cancelado(false)
    , m_archivosTransferidosActual(0)
{
    ui->setupUi(this);

    // Configurar el aspecto y anchura de columnas de la tabla de equipos
    ui->tableWidgetEquipos->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidgetEquipos->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidgetEquipos->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidgetEquipos->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);
    ui->tableWidgetEquipos->horizontalHeader()->setSectionResizeMode(4, QHeaderView::ResizeToContents);

    // Mostrar el hostname actual en la etiqueta del grupo local
    const QString hostActual = QHostInfo::localHostName();
    ui->labelHostLocal->setText(tr("Carpeta local (%1):").arg(hostActual));

    // Detectar y asignar el directorio local de imágenes (punto de unión)
    ui->lineEditDirLocal->setText(detectarDirectorioLocal());
    connect(ui->lineEditDirLocal, &QLineEdit::editingFinished, this, [this]() {
        ui->lineEditDirLocal->setText(normalizarRutaRsync(ui->lineEditDirLocal->text()));
        actualizarContadorArchivosLocales();
    });

    // Cargar lista de equipos desde tienda.ini global o usar los predeterminados del script
    QSettings settings(Configuracion::rutaIniGlobal(), QSettings::IniFormat);
    QStringList guardados = settings.value("SyncImagenes/equipos").toStringList();
    if (guardados.isEmpty()) {
        guardados = equiposPredeterminados();
    }
    cargarEquiposEnTabla(guardados);

    // Conectar señales del proceso asíncrono para SSH / rsync
    m_proceso->setProcessChannelMode(QProcess::MergedChannels);
    connect(m_proceso, &QProcess::readyReadStandardOutput, this, &DialogSyncImagenes::onProcesoOutputListo);
    connect(m_proceso, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, &DialogSyncImagenes::onProcesoTerminado);

    actualizarContadorArchivosLocales();
}

/**
 * @brief Destructor: detiene cualquier proceso en ejecución antes de liberar la UI.
 */
DialogSyncImagenes::~DialogSyncImagenes()
{
    if (m_proceso->state() != QProcess::NotRunning) {
        m_proceso->kill();
        m_proceso->waitForFinished(2000);
    }
    delete ui;
}

/**
 * @brief Asegura que la ruta del directorio termine siempre en '/' para rsync.
 */
QString DialogSyncImagenes::normalizarRutaRsync(const QString &ruta)
{
    QString limpia = ruta.trimmed();
    if (!limpia.isEmpty() && !limpia.endsWith('/')) {
        limpia.append('/');
    }
    return limpia;
}

/**
 * @brief Determina el directorio local de imágenes según el hostname o la configuración activa.
 * Replica la lógica del script sincronizar-imagenes-tpv priorizando /opt/tienda/imagenes/ si se ejecuta desde /opt/tienda.
 */
QString DialogSyncImagenes::detectarDirectorioLocal() const
{
    const QString host = QHostInfo::localHostName();

    // Si estamos ejecutando la instalación global en /opt/tienda, usar /opt/tienda/imagenes/
    if (Configuracion::rutaDirectorioBase() == "/opt/tienda" && QDir("/opt/tienda/imagenes").exists()) {
        return QStringLiteral("/opt/tienda/imagenes/");
    }

    // Mapeo directo por nombre de máquina según sincronizar-imagenes-tpv
    if (host.compare("LadisCasa", Qt::CaseInsensitive) == 0) {
        if (QDir("/home/ladis/Proyectos/qt/tienda/build/imagenes").exists()) {
            return QStringLiteral("/home/ladis/Proyectos/qt/tienda/build/imagenes/");
        }
    } else if (host.compare("herbolario-emeicjac", Qt::CaseInsensitive) == 0) {
        return QStringLiteral("/home/emeicjac/TPV/imagenes/");
    } else if (host.compare("herbolario-cervantes", Qt::CaseInsensitive) == 0) {
        return QStringLiteral("/home/cervantes/tpv/imagenes/");
    } else if (host.compare("reserva1", Qt::CaseInsensitive) == 0) {
        return QStringLiteral("/home/casablanca/TPV/imagenes/");
    } else if (host.compare("ladislao-ms7817", Qt::CaseInsensitive) == 0) {
        return QStringLiteral("/opt/tienda/imagenes/");
    }

    // Fallback al directorio de imágenes configurado en la aplicación
    baseDatos bd;
    QString dirConf = bd.devolverDirectorio("imagenes");
    if (!dirConf.isEmpty()) {
        return normalizarRutaRsync(QFileInfo(dirConf).absoluteFilePath());
    }

    return normalizarRutaRsync(QDir::homePath() + "/TPV/imagenes/");
}

/**
 * @brief Lista original de los 5 equipos definidos en sincronizar-imagenes-tpv.
 */
QStringList DialogSyncImagenes::equiposPredeterminados()
{
    return {
        QStringLiteral("LadisCasa|ladis@100.103.89.50|/home/ladis/Proyectos/qt/tienda/build/imagenes/"),
        QStringLiteral("Emeicjac|emeicjac@100.68.226.81|/home/emeicjac/TPV/imagenes/"),
        QStringLiteral("Cervantes|cervantes@100.89.11.2|/home/cervantes/tpv/imagenes/"),
        QStringLiteral("Casablanca (reserva1)|casablanca@100.107.250.3|/home/casablanca/TPV/imagenes/"),
        QStringLiteral("Casablanca (antiguo)|casablanca@100.107.42.78|/opt/tienda/imagenes/")
    };
}

/**
 * @brief Determina si un destino "usuario@IP" corresponde al propio equipo local
 * comprobando tanto el mapeo de hostname como las interfaces de red locales (Tailscale/LAN).
 */
bool DialogSyncImagenes::esEquipoLocal(const QString &hostUsuarioIp) const
{
    const QString hostName = QHostInfo::localHostName();
    QString selfIpPorHost;
    if (hostName.compare("LadisCasa", Qt::CaseInsensitive) == 0) {
        selfIpPorHost = QStringLiteral("100.103.89.50");
    } else if (hostName.compare("herbolario-emeicjac", Qt::CaseInsensitive) == 0) {
        selfIpPorHost = QStringLiteral("100.68.226.81");
    } else if (hostName.compare("herbolario-cervantes", Qt::CaseInsensitive) == 0) {
        selfIpPorHost = QStringLiteral("100.89.11.2");
    } else if (hostName.compare("reserva1", Qt::CaseInsensitive) == 0) {
        selfIpPorHost = QStringLiteral("100.107.250.3");
    } else if (hostName.compare("ladislao-ms7817", Qt::CaseInsensitive) == 0) {
        selfIpPorHost = QStringLiteral("100.107.42.78");
    }

    if (!selfIpPorHost.isEmpty() && hostUsuarioIp.contains(selfIpPorHost)) {
        return true;
    }

    // Extraer la parte de IP tras '@' y compararla con las IPs activas de este equipo
    const QString ipParte = hostUsuarioIp.contains('@')
                                ? hostUsuarioIp.section('@', 1, 1).trimmed()
                                : hostUsuarioIp.trimmed();
    if (!ipParte.isEmpty()) {
        const QList<QHostAddress> direcciones = QNetworkInterface::allAddresses();
        for (const QHostAddress &addr : direcciones) {
            if (addr.toString() == ipParte) {
                return true;
            }
        }
    }
    return false;
}

/**
 * @brief Rellena la tabla con la lista de entradas "Nombre|usuario@IP|ruta_remota".
 */
void DialogSyncImagenes::cargarEquiposEnTabla(const QStringList &entradas)
{
    ui->tableWidgetEquipos->setRowCount(0);
    for (const QString &entry : entradas) {
        const QStringList partes = entry.split('|');
        if (partes.size() >= 3) {
            agregarFilaEquipo(partes.at(0).trimmed(),
                              partes.at(1).trimmed(),
                              normalizarRutaRsync(partes.at(2).trimmed()));
        }
    }
}

/**
 * @brief Inserta una fila en la tabla de equipos y desmarca automáticamente el equipo local.
 */
void DialogSyncImagenes::agregarFilaEquipo(const QString &nombre, const QString &host, const QString &rutaRemota)
{
    const int fila = ui->tableWidgetEquipos->rowCount();
    ui->tableWidgetEquipos->insertRow(fila);

    const bool esPropio = esEquipoLocal(host);

    QTableWidgetItem *itemCheck = new QTableWidgetItem();
    itemCheck->setFlags(Qt::ItemIsUserCheckable | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    itemCheck->setCheckState(esPropio ? Qt::Unchecked : Qt::Checked);
    ui->tableWidgetEquipos->setItem(fila, 0, itemCheck);

    ui->tableWidgetEquipos->setItem(fila, 1, new QTableWidgetItem(nombre));
    ui->tableWidgetEquipos->setItem(fila, 2, new QTableWidgetItem(host));
    ui->tableWidgetEquipos->setItem(fila, 3, new QTableWidgetItem(rutaRemota));

    QTableWidgetItem *itemEstado = new QTableWidgetItem(
        esPropio ? tr("🏠 Equipo actual (omitido)") : tr("Pendiente"));
    itemEstado->setFlags(Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    ui->tableWidgetEquipos->setItem(fila, 4, itemEstado);
}

/**
 * @brief Cuenta los archivos existentes en la carpeta local de imágenes y actualiza la etiqueta.
 */
int DialogSyncImagenes::actualizarContadorArchivosLocales()
{
    const QString localDir = ui->lineEditDirLocal->text().trimmed();
    int total = 0;
    if (!localDir.isEmpty() && QDir(localDir).exists()) {
        QDirIterator it(localDir, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            it.next();
            ++total;
        }
    }
    ui->labelTotalLocal->setText(tr("Archivos locales: %1").arg(total));
    return total;
}

void DialogSyncImagenes::on_toolButtonExaminarLocal_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(
        this,
        tr("Seleccionar carpeta local de imágenes"),
        ui->lineEditDirLocal->text(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!dir.isEmpty()) {
        ui->lineEditDirLocal->setText(normalizarRutaRsync(dir));
        actualizarContadorArchivosLocales();
    }
}

void DialogSyncImagenes::on_pushButtonAnadirEquipo_clicked()
{
    agregarFilaEquipo(tr("Nueva Tienda"),
                      QStringLiteral("usuario@100.0.0.1"),
                      QStringLiteral("/opt/tienda/imagenes/"));
}

void DialogSyncImagenes::on_pushButtonQuitarEquipo_clicked()
{
    const int fila = ui->tableWidgetEquipos->currentRow();
    if (fila >= 0) {
        ui->tableWidgetEquipos->removeRow(fila);
    }
}

void DialogSyncImagenes::on_pushButtonRestaurarEquipos_clicked()
{
    cargarEquiposEnTabla(equiposPredeterminados());
}

void DialogSyncImagenes::on_pushButtonGuardarEquipos_clicked()
{
    QStringList lista;
    for (int i = 0; i < ui->tableWidgetEquipos->rowCount(); ++i) {
        const QString nombre = ui->tableWidgetEquipos->item(i, 1) ? ui->tableWidgetEquipos->item(i, 1)->text().trimmed() : QString();
        const QString host   = ui->tableWidgetEquipos->item(i, 2) ? ui->tableWidgetEquipos->item(i, 2)->text().trimmed() : QString();
        const QString ruta   = ui->tableWidgetEquipos->item(i, 3) ? normalizarRutaRsync(ui->tableWidgetEquipos->item(i, 3)->text()) : QString();
        if (!nombre.isEmpty() && !host.isEmpty() && !ruta.isEmpty()) {
            lista << QString("%1|%2|%3").arg(nombre, host, ruta);
        }
    }

    QString iniPath = Configuracion::rutaIniGlobal();
    QSettings settings(iniPath, QSettings::IniFormat);
    if (!settings.isWritable()) {
        iniPath = Configuracion::rutaIniUsuario();
    }
    QSettings saveSettings(iniPath, QSettings::IniFormat);
    saveSettings.setValue("SyncImagenes/equipos", lista);
    saveSettings.sync();

    QMessageBox::information(this, tr("Equipos guardados"),
                             tr("Se ha guardado la lista de equipos de sincronización en:\n%1").arg(iniPath));
}

void DialogSyncImagenes::agregarLog(const QString &texto)
{
    ui->plainTextEditLog->appendPlainText(texto);
    if (QScrollBar *sb = ui->plainTextEditLog->verticalScrollBar()) {
        sb->setValue(sb->maximum());
    }
}

/**
 * @brief Construye la cola de sincronización en 2 pasos (Pull y Push) y la inicia de forma asíncrona.
 */
void DialogSyncImagenes::on_pushButtonSincronizar_clicked()
{
    if (m_proceso->state() != QProcess::NotRunning) {
        return;
    }

    const QString localDir = normalizarRutaRsync(ui->lineEditDirLocal->text());
    ui->lineEditDirLocal->setText(localDir);

    if (localDir.isEmpty()) {
        QMessageBox::warning(this, tr("Carpeta local vacía"),
                             tr("Indica la carpeta local de imágenes que actuará como punto de unión."));
        return;
    }

    // Asegurar que la carpeta local existe (mkdir -p "$LOCAL_DIR")
    QDir dir(localDir);
    if (!dir.exists() && !dir.mkpath(".")) {
        QMessageBox::critical(this, tr("Error de permisos"),
                              tr("No se pudo crear ni acceder al directorio local:\n%1").arg(localDir));
        return;
    }

    m_colaTareas.clear();

    // Recopilar los equipos marcados en la tabla
    struct Destino {
        int fila;
        QString nombre;
        QString host;
        QString rutaRemota;
    };
    QList<Destino> destinos;

    for (int i = 0; i < ui->tableWidgetEquipos->rowCount(); ++i) {
        QTableWidgetItem *chk = ui->tableWidgetEquipos->item(i, 0);
        if (!chk || chk->checkState() != Qt::Checked) {
            continue;
        }
        const QString nombre = ui->tableWidgetEquipos->item(i, 1) ? ui->tableWidgetEquipos->item(i, 1)->text().trimmed() : QString();
        const QString host   = ui->tableWidgetEquipos->item(i, 2) ? ui->tableWidgetEquipos->item(i, 2)->text().trimmed() : QString();
        const QString ruta   = ui->tableWidgetEquipos->item(i, 3) ? normalizarRutaRsync(ui->tableWidgetEquipos->item(i, 3)->text()) : QString();
        if (ui->tableWidgetEquipos->item(i, 3)) {
            ui->tableWidgetEquipos->item(i, 3)->setText(ruta);
        }
        if (!host.isEmpty() && !ruta.isEmpty()) {
            destinos.append({i, nombre, host, ruta});
            if (ui->tableWidgetEquipos->item(i, 4)) {
                ui->tableWidgetEquipos->item(i, 4)->setText(tr("En cola"));
            }
        }
    }

    if (destinos.isEmpty()) {
        QMessageBox::information(this, tr("Sin equipos seleccionados"),
                                 tr("Marca al menos un equipo remoto en la tabla para sincronizar."));
        return;
    }

    // PASO 1: Recoger imágenes nuevas o actualizadas de cada equipo remoto
    for (const Destino &d : destinos) {
        m_colaTareas.append({TareaSync::PullCheckSSH, d.fila, d.nombre, d.host, d.rutaRemota});
        m_colaTareas.append({TareaSync::PullRsync,    d.fila, d.nombre, d.host, d.rutaRemota});
    }

    // PASO 2: Enviar el catálogo completo unificado a cada equipo remoto
    for (const Destino &d : destinos) {
        m_colaTareas.append({TareaSync::PushMkdirSSH, d.fila, d.nombre, d.host, d.rutaRemota});
        m_colaTareas.append({TareaSync::PushRsync,    d.fila, d.nombre, d.host, d.rutaRemota});
    }

    m_totalTareas = m_colaTareas.size();
    m_tareasCompletadas = 0;
    m_cancelado = false;

    ui->plainTextEditLog->clear();
    ui->progressBar->setRange(0, m_totalTareas);
    ui->progressBar->setValue(0);

    ui->pushButtonSincronizar->setEnabled(false);
    ui->pushButtonDetener->setEnabled(true);
    ui->tableWidgetEquipos->setEnabled(false);

    const QString fechaHoy = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    agregarLog(QStringLiteral("=========================================================="));
    agregarLog(tr(" [%1] Sincronizando imágenes TPV desde: %2")
                   .arg(fechaHoy, QHostInfo::localHostName()));
    agregarLog(tr(" Punto de unión local: %1").arg(localDir));
    agregarLog(QStringLiteral("=========================================================="));
    agregarLog(QString());
    agregarLog(tr(">>> PASO 1: Recogiendo imágenes nuevas o actualizadas de los demás equipos..."));

    ejecutarSiguienteTarea();
}

/**
 * @brief Desencola y lanza el proceso correspondiente (ssh o rsync) para la tarea actual.
 */
void DialogSyncImagenes::ejecutarSiguienteTarea()
{
    if (m_cancelado || m_colaTareas.isEmpty()) {
        const int totalArchivos = actualizarContadorArchivosLocales();
        ui->pushButtonSincronizar->setEnabled(true);
        ui->pushButtonDetener->setEnabled(false);
        ui->tableWidgetEquipos->setEnabled(true);

        if (m_cancelado) {
            ui->labelEstado->setText(tr("⏹️ Sincronización cancelada por el usuario."));
            agregarLog(QString());
            agregarLog(tr("⏹️ Proceso detenido por el usuario."));
        } else {
            ui->progressBar->setValue(m_totalTareas);
            ui->labelEstado->setText(tr("✅ Sincronización completada. Total de archivos locales: %1").arg(totalArchivos));
            agregarLog(QString());
            agregarLog(QStringLiteral("=========================================================="));
            agregarLog(tr(" Sincronización completada. Total de archivos locales: %1").arg(totalArchivos));
            agregarLog(QStringLiteral("=========================================================="));
        }
        return;
    }

    const TareaSync tarea = m_colaTareas.first();
    const QString localDir = normalizarRutaRsync(ui->lineEditDirLocal->text());
    m_archivosTransferidosActual = 0;

    // Mostrar cabecera al iniciar el PASO 2
    if (tarea.tipo == TareaSync::PushMkdirSSH && m_tareasCompletadas == (m_totalTareas / 2)) {
        actualizarContadorArchivosLocales();
        agregarLog(QString());
        agregarLog(tr(">>> PASO 2: Enviando catálogo completo unificado al resto de equipos..."));
    }

    QTableWidgetItem *itemEstado = ui->tableWidgetEquipos->item(tarea.filaTabla, 4);

    switch (tarea.tipo) {
    case TareaSync::PullCheckSSH: {
        ui->labelEstado->setText(tr("Paso 1/2: Conectando con %1 (%2)...").arg(tarea.nombre, tarea.host));
        if (itemEstado) itemEstado->setText(tr("⏳ Comprobando SSH (Paso 1)..."));
        agregarLog(tr("  - Conectando con %1 (%2)...").arg(tarea.nombre, tarea.host));

        QStringList args = {
            QStringLiteral("-o"), QStringLiteral("BatchMode=yes"),
            QStringLiteral("-o"), QStringLiteral("ConnectTimeout=5"),
            tarea.host,
            QString("test -d '%1'").arg(tarea.rutaRemota)
        };
        m_proceso->start(QStringLiteral("ssh"), args);
        break;
    }
    case TareaSync::PullRsync: {
        ui->labelEstado->setText(tr("Paso 1/2: Descargando novedades de %1...").arg(tarea.nombre));
        if (itemEstado) itemEstado->setText(tr("⬇️ Recibiendo novedades..."));

        // -az: archivo + compresión
        // --no-perms --no-owner --no-group: evita errores (código 23) de permisos/chown
        // entre máquinas con diferentes usuarios o ficheros con propietarios heterogéneos.
        QStringList args = {
            QStringLiteral("-az"),
            QStringLiteral("-u"),
            QStringLiteral("--no-perms"),
            QStringLiteral("--no-owner"),
            QStringLiteral("--no-group"),
            QStringLiteral("--itemize-changes"),
            QStringLiteral("-e"), QStringLiteral("ssh -o BatchMode=yes -o ConnectTimeout=5"),
            QString("%1:%2").arg(tarea.host, tarea.rutaRemota),
            localDir
        };
        m_proceso->start(QStringLiteral("rsync"), args);
        break;
    }
    case TareaSync::PushMkdirSSH: {
        ui->labelEstado->setText(tr("Paso 2/2: Preparando envío a %1 (%2)...").arg(tarea.nombre, tarea.host));
        if (itemEstado) itemEstado->setText(tr("⏳ Conectando SSH (Paso 2)..."));
        agregarLog(tr("  - Enviando a %1 (%2)...").arg(tarea.nombre, tarea.host));

        QStringList args = {
            QStringLiteral("-o"), QStringLiteral("BatchMode=yes"),
            QStringLiteral("-o"), QStringLiteral("ConnectTimeout=5"),
            tarea.host,
            QString("mkdir -p '%1'").arg(tarea.rutaRemota)
        };
        m_proceso->start(QStringLiteral("ssh"), args);
        break;
    }
    case TareaSync::PushRsync: {
        ui->labelEstado->setText(tr("Paso 2/2: Subiendo catálogo unificado a %1...").arg(tarea.nombre));
        if (itemEstado) itemEstado->setText(tr("⬆️ Enviando catálogo..."));

        // -az: archivo + compresión
        // --no-perms --no-owner --no-group: evita errores (código 23) de permisos/chown
        // entre máquinas con diferentes usuarios o ficheros con propietarios heterogéneos.
        QStringList args = {
            QStringLiteral("-az"),
            QStringLiteral("-u"),
            QStringLiteral("--no-perms"),
            QStringLiteral("--no-owner"),
            QStringLiteral("--no-group"),
            QStringLiteral("--itemize-changes"),
            QStringLiteral("-e"), QStringLiteral("ssh -o BatchMode=yes -o ConnectTimeout=5"),
            localDir,
            QString("%1:%2").arg(tarea.host, tarea.rutaRemota)
        };
        m_proceso->start(QStringLiteral("rsync"), args);
        break;
    }
    }
}

/**
 * @brief Captura en tiempo real la salida de rsync/ssh y la muestra en el área de registro.
 */
void DialogSyncImagenes::onProcesoOutputListo()
{
    const QString salida = QString::fromLocal8Bit(m_proceso->readAllStandardOutput());
    const QStringList lineas = salida.split('\n', Qt::SkipEmptyParts);
    for (const QString &linea : lineas) {
        const QString limpia = linea.trimmed();
        if (!limpia.isEmpty()) {
            ++m_archivosTransferidosActual;
            agregarLog(QStringLiteral("      ") + limpia);
        }
    }
}

/**
 * @brief Evalúa el resultado del proceso actual y avanza a la siguiente fase de la cola.
 */
void DialogSyncImagenes::onProcesoTerminado(int exitCode, QProcess::ExitStatus exitStatus)
{
    onProcesoOutputListo();

    if (m_colaTareas.isEmpty()) {
        return;
    }

    const TareaSync tarea = m_colaTareas.takeFirst();
    ++m_tareasCompletadas;
    ui->progressBar->setValue(m_tareasCompletadas);

    if (m_cancelado) {
        ejecutarSiguienteTarea();
        return;
    }

    const bool exito = (exitStatus == QProcess::NormalExit && exitCode == 0);
    QTableWidgetItem *itemEstado = ui->tableWidgetEquipos->item(tarea.filaTabla, 4);

    if (tarea.tipo == TareaSync::PullCheckSSH) {
        if (exito) {
            agregarLog(tr("    OK (descargando novedades)"));
        } else {
            agregarLog(tr("    NO DISPONIBLE (apagado, sin acceso SSH o sin carpeta remota; se omite)"));
            if (itemEstado) itemEstado->setText(tr("⚠️ No disponible (SSH)"));
            // Omitir el PullRsync inmediatamente posterior para este mismo servidor
            if (!m_colaTareas.isEmpty() && m_colaTareas.first().tipo == TareaSync::PullRsync &&
                m_colaTareas.first().filaTabla == tarea.filaTabla) {
                m_colaTareas.removeFirst();
                ++m_tareasCompletadas;
                ui->progressBar->setValue(m_tareasCompletadas);
            }
        }
    } else if (tarea.tipo == TareaSync::PullRsync) {
        if (exito) {
            if (itemEstado) {
                itemEstado->setText(tr("⬇️ Recibido (%1 cambios)").arg(m_archivosTransferidosActual));
            }
            actualizarContadorArchivosLocales();
        } else {
            agregarLog(tr("    ⚠️ Error durante rsync de recepción (código %1)").arg(exitCode));
            if (itemEstado) itemEstado->setText(tr("⚠️ Error al recibir"));
        }
    } else if (tarea.tipo == TareaSync::PushMkdirSSH) {
        if (exito) {
            agregarLog(tr("    OK (subiendo novedades)"));
        } else {
            agregarLog(tr("    NO DISPONIBLE (se omite)"));
            if (itemEstado) itemEstado->setText(tr("⚠️ No disponible (omitido)"));
            // Omitir el PushRsync inmediatamente posterior para este mismo servidor
            if (!m_colaTareas.isEmpty() && m_colaTareas.first().tipo == TareaSync::PushRsync &&
                m_colaTareas.first().filaTabla == tarea.filaTabla) {
                m_colaTareas.removeFirst();
                ++m_tareasCompletadas;
                ui->progressBar->setValue(m_tareasCompletadas);
            }
        }
    } else if (tarea.tipo == TareaSync::PushRsync) {
        if (exito) {
            if (itemEstado) {
                itemEstado->setText(tr("✅ Sincronizado (%1 enviados)").arg(m_archivosTransferidosActual));
            }
        } else {
            agregarLog(tr("    ⚠️ Error durante rsync de envío (código %1)").arg(exitCode));
            if (itemEstado) itemEstado->setText(tr("⚠️ Error al enviar"));
        }
    }

    ejecutarSiguienteTarea();
}

void DialogSyncImagenes::on_pushButtonDetener_clicked()
{
    m_cancelado = true;
    m_colaTareas.clear();
    if (m_proceso->state() != QProcess::NotRunning) {
        m_proceso->kill();
    }
}

void DialogSyncImagenes::on_pushButtonCerrar_clicked()
{
    if (m_proceso->state() != QProcess::NotRunning) {
        const QMessageBox::StandardButton res = QMessageBox::question(
            this,
            tr("Sincronización en curso"),
            tr("Hay una sincronización de imágenes en marcha. ¿Deseas detenerla y cerrar?"),
            QMessageBox::Yes | QMessageBox::No);
        if (res != QMessageBox::Yes) {
            return;
        }
        on_pushButtonDetener_clicked();
    }
    accept();
}
