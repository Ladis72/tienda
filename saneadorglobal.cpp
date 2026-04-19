#include "saneadorglobal.h"
#include "ui_saneadorglobal.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDebug>
#include <QStyle>
#include <QTime>
#include <QSqlRecord>
#include <QSet>

SaneadorGlobal::SaneadorGlobal(const UnificarMaestrosConfig &config, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SaneadorGlobal),
    m_config(config)
{
    ui->setupUi(this);
    setWindowTitle(QString("Saneador Global: %1").arg(m_config.titulo));
    ui->labelTitle->setText(m_config.titulo);

    // Llenar combo de tiendas online
    ui->comboBoxTienda->addItem(tr("-- Seleccione Tienda --"), "");
    QStringList activas = conf->getNombreConexionesActivas();
    
    // Añadir NUBE explícitamente a la lista si la conexión está abierta
    if (QSqlDatabase::database("NUBE").isOpen() && !activas.contains("NUBE")) {
        activas.prepend("NUBE");
    }

    for (const QString &c : activas) {
        if (c != "DB" && c != conf->getConexionCommon()) {
            ui->comboBoxTienda->addItem(c, c);
        }
    }

    // Estética tabla
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // Iconos
    ui->pushButtonAnalizar->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pushButtonIgualar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonIgualarLocal->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonClonar->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonClonarLocal->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
}

SaneadorGlobal::~SaneadorGlobal()
{
    delete ui;
}

void SaneadorGlobal::log(const QString &msg)
{
    ui->textEditLog->append(QString("[%1] %2").arg(QTime::currentTime().toString(), msg));
}

QSqlDatabase SaneadorGlobal::dbLocal() { return QSqlDatabase::database("DB"); }
QSqlDatabase SaneadorGlobal::dbRemota() { return QSqlDatabase::database(m_nombreConexionRemota); }

void SaneadorGlobal::on_comboBoxTienda_currentIndexChanged(int index)
{
    if (index < 0) return;
    m_nombreConexionRemota = ui->comboBoxTienda->itemData(index).toString();
    ui->tableWidget->setRowCount(0);
}

void SaneadorGlobal::on_pushButtonAnalizar_clicked()
{
    if (m_nombreConexionRemota.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione una tienda remota."));
        return;
    }
    refrescarComparativa();
}

void SaneadorGlobal::refrescarComparativa()
{
    ui->tableWidget->setRowCount(0);
    log("Iniciando análisis comparativo...");

    QSqlDatabase local = dbLocal();
    QSqlDatabase remota = dbRemota();

    if (!remota.isOpen()) {
        log("ERROR: La conexión remota no está abierta.");
        return;
    }

    struct InfoRow {
        int id;
        QString nombre;
    };

    // 1. Cargar Maestros Locales
    QMap<QString, InfoRow> locales;
    QSqlQuery qL(local);
    QString selectL = QString("SELECT %1, %2").arg(m_config.campoId, m_config.campoNombre);
    if (!m_config.campoNif.isEmpty()) selectL += ", " + m_config.campoNif;
    selectL += QString(" FROM %1").arg(m_config.tablaMaestra);

    qL.exec(selectL);
    while (qL.next()) {
        int id = qL.value(0).toInt();
        QString nombre = qL.value(1).toString().trimmed();
        QString key = nombre.toUpper(); // Default key: Name

        if (!m_config.campoNif.isEmpty()) {
            QString nif = qL.value(2).toString().trimmed().toUpper();
            if (!nif.isEmpty()) key = nif; // Override key with NIF if available
        }
        locales.insert(key, {id, nombre});
    }

    // 2. Cargar Maestros Remotos
    QMap<QString, InfoRow> remotos;
    QSqlQuery qR(remota);
    QString selectR = QString("SELECT %1, %2").arg(m_config.campoId, m_config.campoNombre);
    if (!m_config.campoNif.isEmpty()) selectR += ", " + m_config.campoNif;
    selectR += QString(" FROM %1").arg(m_config.tablaMaestra);

    qR.exec(selectR);
    while (qR.next()) {
        int id = qR.value(0).toInt();
        QString nombre = qR.value(1).toString().trimmed();
        QString key = nombre.toUpper();

        if (!m_config.campoNif.isEmpty()) {
            QString nif = qR.value(2).toString().trimmed().toUpper();
            if (!nif.isEmpty()) key = nif;
        }
        remotos.insert(key, {id, nombre});
    }

    // 3. Cruzar datos
    QSet<QString> todasLasKeys = QSet<QString>::fromList(locales.keys());
    todasLasKeys.unite(QSet<QString>::fromList(remotos.keys()));
    QStringList keysOrdenadas = todasLasKeys.values();
    keysOrdenadas.sort();

    for (const QString &key : keysOrdenadas) {
        bool existeL = locales.contains(key);
        bool existeR = remotos.contains(key);
        
        int idL = existeL ? locales[key].id : -1;
        int idR = existeR ? remotos[key].id : -1;
        QString nombreL = existeL ? locales[key].nombre : "";
        QString nombreR = existeR ? remotos[key].nombre : "";

        QString nombreAMostrar = nombreL.isEmpty() ? nombreR : nombreL;
        if (!nombreL.isEmpty() && !nombreR.isEmpty() && nombreL != nombreR) {
            nombreAMostrar = QString("%1 / %2").arg(nombreL, nombreR);
        }

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        
        // Col 0: Identificador (Nombre o NIF)
        QString desc = key;
        if (!m_config.campoNif.isEmpty() && key.length() < 5) { // Probablemente no sea un NIF si es muy corto y hay campoNif
             desc = QString("%1 (%2)").arg(nombreAMostrar, key);
        } else if (!m_config.campoNif.isEmpty()) {
             desc = QString("%1 [%2]").arg(nombreAMostrar, key);
        } else {
             desc = nombreAMostrar;
        }

        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(desc));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(idL == -1 ? "" : QString::number(idL)));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(idR == -1 ? "" : QString::number(idR)));

        QTableWidgetItem *status = new QTableWidgetItem();
        if (idL != -1 && idR != -1) {
            if (idL == idR) {
                status->setText("IDENTICO");
                status->setBackground(Qt::green);
            } else {
                status->setText("DIFERENTE ID");
                status->setBackground(Qt::yellow);
            }
        } else if (idL != -1) {
            status->setText("FALTA EN REMOTA");
            status->setBackground(Qt::red);
            status->setForeground(Qt::white);
        } else {
            status->setText("FALTA EN LOCAL");
            status->setBackground(Qt::cyan);
        }
        ui->tableWidget->setItem(row, 3, status);
    }
    
    log(QString("Análisis finalizado. %1 registros únicos encontrados.").arg(keysOrdenadas.count()));
}

void SaneadorGlobal::on_pushButtonIgualar_clicked()
{
    log("Iniciando igualación en remoto...");
    QList<QTableWidgetItem *> selected = ui->tableWidget->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("Atención"), tr("Por favor, seleccione primero los registros que desea igualar en la tabla."));
        return;
    }

    // Obtener filas únicas seleccionadas
    QSet<int> rows;
    for (auto *item : selected) rows.insert(item->row());

    int exitos = 0;
    for (int row : rows) {
        QString nombre = ui->tableWidget->item(row, 0)->text();
        QString idLStr = ui->tableWidget->item(row, 1)->text();
        QString idRStr = ui->tableWidget->item(row, 2)->text();
        QString estado = ui->tableWidget->item(row, 3)->text();

        if (idLStr.isEmpty() || idRStr.isEmpty()) {
            log(QString("Saltando '%1': no existe en uno de los lados. Use Clonar.").arg(nombre));
            continue;
        }

        int idLocal = idLStr.toInt();
        int idRemoto = idRStr.toInt();

        if (estado == "DIFERENTE") {
            if (ejecutarAlineacion(dbRemota(), idLocal, idRemoto, nombre)) exitos++;
        } else {
            log(QString("Saltando '%1': ya están alineados (ID %2).").arg(nombre, idLStr));
        }
    }

    if (exitos > 0) {
        QMessageBox::information(this, tr("Saneamiento"), tr("Se han alineado %1 registros en remoto.").arg(exitos));
        refrescarComparativa();
    }
}

void SaneadorGlobal::on_pushButtonIgualarLocal_clicked()
{
    log("Iniciando igualación en local...");
    QList<QTableWidgetItem *> selected = ui->tableWidget->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("Atención"), tr("Por favor, seleccione primero los registros que desea igualar en la tabla."));
        return;
    }

    QSet<int> rows;
    for (auto *item : selected) rows.insert(item->row());

    int exitos = 0;
    for (int row : rows) {
        QString nombre = ui->tableWidget->item(row, 0)->text();
        QString idLStr = ui->tableWidget->item(row, 1)->text();
        QString idRStr = ui->tableWidget->item(row, 2)->text();
        QString estado = ui->tableWidget->item(row, 3)->text();

        if (idLStr.isEmpty() || idRStr.isEmpty()) {
            log(QString("Saltando '%1': no existe en uno de los lados. Use Clonar.").arg(nombre));
            continue;
        }

        int idLocalActual = idLStr.toInt();
        int idNuevoLocal = idRStr.toInt(); 

        if (estado == "DIFERENTE") {
            if (ejecutarAlineacion(dbLocal(), idNuevoLocal, idLocalActual, nombre)) exitos++;
        } else {
            log(QString("Saltando '%1': ya están alineados (ID %2).").arg(nombre, idRStr));
        }
    }

    if (exitos > 0) {
        QMessageBox::information(this, tr("Saneamiento"), tr("Se han alineado %1 registros en local.").arg(exitos));
        refrescarComparativa();
    }
}

bool SaneadorGlobal::ejecutarAlineacion(QSqlDatabase targetDb, int idNuevo, int idActual, const QString &nombre)
{
    log(QString("Alineando '%1' en %2: %3 -> %4")
        .arg(nombre, targetDb.connectionName(), QString::number(idActual), QString::number(idNuevo)));

    if (!targetDb.transaction()) return false;

    // 1. Desplazar si el idNuevo está ocupado por otro en la base destino
    if (!desplazarID(targetDb, idNuevo)) {
        targetDb.rollback();
        return false;
    }

    QSqlQuery q(targetDb);
    
    // 2. Cambiar ID del Maestro
    q.prepare(QString("UPDATE %1 SET %2 = :new WHERE %2 = :old")
              .arg(m_config.tablaMaestra, m_config.campoId));
    q.bindValue(":new", idNuevo);
    q.bindValue(":old", idActual);
    if (!q.exec()) {
        log("Error actualizando maestro: " + q.lastError().text());
        targetDb.rollback();
        return false;
    }

    // 3. Actualizar Dependencias
    for (const auto &dep : m_config.dependencias) {
        if (!dep.campoMaestroOrigen.isEmpty()) continue; 

        QString sql = QString("UPDATE %1 SET %2 = %3 WHERE %2 = %4")
                      .arg(dep.tabla, dep.campo, QString::number(idNuevo), QString::number(idActual));
        if (!q.exec(sql)) {
            log(QString("Error en dependencia %1: %2").arg(dep.tabla, q.lastError().text()));
            targetDb.rollback();
            return false;
        }
    }

    if (targetDb.commit()) {
        log("OK: Registro alineado correctamente.");
        return true;
    }
    return false;
}

bool SaneadorGlobal::desplazarID(QSqlDatabase db, int idOcupado)
{
    QSqlQuery q(db);
    // ¿Está ocupado?
    q.prepare(QString("SELECT count(*) FROM %1 WHERE %2 = %3").arg(m_config.tablaMaestra, m_config.campoId, QString::number(idOcupado)));
    q.exec();
    if (q.next() && q.value(0).toInt() == 0) return true; // Libre

    log(QString("El ID %1 en %2 está ocupado. Desplazando ocupante...").arg(QString::number(idOcupado), db.connectionName()));

    // Buscar nuevo ID libre (max + 1)
    q.exec(QString("SELECT max(%1) FROM %2").arg(m_config.campoId, m_config.tablaMaestra));
    q.next();
    int nuevoId = q.value(0).toInt() + 1;

    // Desplazar maestro
    q.prepare(QString("UPDATE %1 SET %2 = %3 WHERE %2 = %4").arg(m_config.tablaMaestra, m_config.campoId, QString::number(nuevoId), QString::number(idOcupado)));
    if (!q.exec()) return false;

    // Desplazar dependencias del desplazado
    for (const auto &dep : m_config.dependencias) {
        if (!dep.campoMaestroOrigen.isEmpty()) continue;
        q.exec(QString("UPDATE %1 SET %2 = %3 WHERE %2 = %4").arg(dep.tabla, dep.campo, QString::number(nuevoId), QString::number(idOcupado)));
    }

    return true;
}

void SaneadorGlobal::on_pushButtonClonar_clicked()
{
    log("Iniciando clonación a remoto...");
    QList<QTableWidgetItem*> selected = ui->tableWidget->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("Atención"), tr("Por favor, seleccione primero los registros que desea clonar en la tabla."));
        return;
    }

    QSet<int> rows;
    for (auto item : selected) rows.insert(item->row());

    int exitos = 0;
    for (int row : rows) {
        QString nombre = ui->tableWidget->item(row, 0)->text();
        QString idLStr = ui->tableWidget->item(row, 1)->text();
        QString idRStr = ui->tableWidget->item(row, 2)->text();

        if (idLStr.isEmpty()) continue;   // Nada que clonar
        if (!idRStr.isEmpty()) continue; // Ya existe en destino

        if (ejecutarClonacion(dbLocal(), dbRemota(), idLStr.toInt(), nombre)) exitos++;
    }

    if (exitos > 0) {
        QMessageBox::information(this, tr("Saneamiento"), tr("Se han clonado %1 registros en remoto.").arg(exitos));
        refrescarComparativa();
    }
}

void SaneadorGlobal::on_pushButtonClonarLocal_clicked()
{
    log("Iniciando clonación a local...");
    QList<QTableWidgetItem*> selected = ui->tableWidget->selectedItems();
    if (selected.isEmpty()) {
        QMessageBox::information(this, tr("Atención"), tr("Por favor, seleccione primero los registros que desea clonar en la tabla."));
        return;
    }

    QSet<int> rows;
    for (auto item : selected) rows.insert(item->row());

    int exitos = 0;
    for (int row : rows) {
        QString nombre = ui->tableWidget->item(row, 0)->text();
        QString idLStr = ui->tableWidget->item(row, 1)->text();
        QString idRStr = ui->tableWidget->item(row, 2)->text();

        if (idRStr.isEmpty()) continue;   // Nada que clonar
        if (!idLStr.isEmpty()) continue; // Ya existe en destino

        if (ejecutarClonacion(dbRemota(), dbLocal(), idRStr.toInt(), nombre)) exitos++;
    }

    if (exitos > 0) {
        QMessageBox::information(this, tr("Saneamiento"), tr("Se han clonado %1 registros en local.").arg(exitos));
        refrescarComparativa();
    }
}

bool SaneadorGlobal::ejecutarClonacion(QSqlDatabase srcDb, QSqlDatabase destDb, int idSource, const QString &nombre)
{
    log(QString("Clonando '%1' (%2) de %3 a %4...")
        .arg(nombre, QString::number(idSource), srcDb.connectionName(), destDb.connectionName()));

    if (!destDb.transaction()) return false;

    // 1. Asegurar que el ID esté libre en destino
    if (!desplazarID(destDb, idSource)) {
        destDb.rollback();
        return false;
    }

    // 2. Obtener registro completo origen
    QSqlQuery qS(srcDb);
    qS.prepare(QString("SELECT * FROM %1 WHERE %2 = %3")
               .arg(m_config.tablaMaestra, m_config.campoId, QString::number(idSource)));
    if (!qS.exec() || !qS.next()) {
        destDb.rollback();
        log("Error leyendo registro origen.");
        return false;
    }

    QSqlRecord rec = qS.record();
    
    // 3. Construir INSERT dinámico
    QStringList fields;
    QStringList placeholders;
    for (int i=0; i < rec.count(); ++i) {
        fields << rec.fieldName(i);
        placeholders << ":" + rec.fieldName(i);
    }

    QSqlQuery qD(destDb);
    qD.prepare(QString("INSERT INTO %1 (%2) VALUES (%3)")
               .arg(m_config.tablaMaestra, fields.join(", "), placeholders.join(", ")));
    
    for (int i=0; i < rec.count(); ++i) {
        qD.bindValue(":" + rec.fieldName(i), qS.value(i));
    }

    if (qD.exec()) {
        if (destDb.commit()) {
            log("OK: Registro clonado satisfactoriamente.");
            return true;
        } else {
            log("Error en commit destino.");
        }
    } else {
        log("Error insertando en destino: " + qD.lastError().text());
        destDb.rollback();
    }
    return false;
}
void SaneadorGlobal::on_pushButtonCerrar_clicked() { accept(); }
void SaneadorGlobal::on_tableWidget_itemSelectionChanged() { /* Visual update */ }
