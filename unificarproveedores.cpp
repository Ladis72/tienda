#include "unificarproveedores.h"
#include "ui_unificarproveedores.h"
#include "base_datos.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QMessageBox>
#include <QDebug>
#include <QSqlDatabase>

extern Configuracion *conf; // Puntero global a la configuración

UnificarProveedores::UnificarProveedores(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::UnificarProveedores)
{
    ui->setupUi(this);
    
    // Llenar combo de tiendas
    ui->comboBoxTiendas->addItems(conf->getNombreConexionesActivas());
    
    // Ajustar tabla
    ui->tableWidgetIDs->setColumnCount(4);
    ui->tableWidgetIDs->setHorizontalHeaderLabels({"Proveedor", "NIF", "ID Local", "ID Remoto"});
    ui->tableWidgetIDs->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

UnificarProveedores::~UnificarProveedores()
{
    delete ui;
}

void UnificarProveedores::log(const QString &msg)
{
    ui->textEditLogSaneamiento->append(msg);
    qDebug() << "UNIFICAR:" << msg;
}

QString UnificarProveedores::normalizarNIF(QString nif)
{
    // Solo números y letras, en mayúsculas
    QString res = nif.trimmed().toUpper();
    QString limpia;
    for(int i=0; i<res.length(); ++i) {
        if(res[i].isLetterOrNumber()) limpia += res[i];
    }
    return limpia;
}

// =============================================================================
// FASE 0: SANEAMIENTO
// =============================================================================

void UnificarProveedores::on_pushButtonNormalizarNIF_clicked()
{
    QStringList tiendas = conf->getNombreConexionesActivas();
    tiendas.prepend(conf->getConexionLocal());
    
    int totalesNIF = 0;
    int totalesDate = 0;
    log("<b>Iniciando Saneamiento Global (NIFs y Fechas)...</b>");
    
    foreach(QString t, tiendas) {
        QSqlDatabase db = QSqlDatabase::database(t);
        if(!db.isValid() || !db.isOpen()) {
            log(QString("Tienda %1: No se pudo abrir conexión o conexión no válida.").arg(t));
            continue;
        }
        
        db.transaction();
        
        // 1. Sanear Fechas (Evitar errores en SyncManager y Nube)
        QSqlQuery qDate(db);
        if(qDate.exec("UPDATE proveedores SET fechaultimacompra = '2000-01-01' WHERE fechaultimacompra IS NULL OR fechaultimacompra = '' OR fechaultimacompra = '0000-00-00'")) {
            int nDates = qDate.numRowsAffected();
            if(nDates > 0) log(QString(" - %1: %2 fechas corregidas.").arg(t).arg(nDates));
            totalesDate += nDates;
        }

        // 2. Normalizar NIFs
        QSqlQuery q(db);
        q.exec("SELECT idProveedor, nif FROM proveedores");
        
        int contNIF = 0;
        while(q.next()) {
            QString id = q.value(0).toString();
            QString nifOrig = q.value(1).toString();
            QString nifLimpio = normalizarNIF(nifOrig);
            
            if(nifOrig != nifLimpio) {
                QSqlQuery up(db);
                up.prepare("UPDATE proveedores SET nif = ? WHERE idProveedor = ?");
                up.addBindValue(nifLimpio);
                up.addBindValue(id);
                if(up.exec()) contNIF++;
            }
        }
        db.commit();
        if(contNIF > 0) log(QString(" - %1: %2 NIFs normalizados.").arg(t).arg(contNIF));
        totalesNIF += contNIF;
    }
    log(QString("<b>Saneamiento completado. Fechas fijadas: %1, NIFs corregidos: %2</b>").arg(totalesDate).arg(totalesNIF));
}

void UnificarProveedores::on_pushButtonDetectarNIF_clicked()
{
    log("<b>Iniciando búsqueda cruzada de NIFs faltantes...</b>");
    
    // 1. Crear mapa maestro de Nombres -> NIF desde todas las tiendas
    QMap<QString, QString> mapaNombres; // Nombre.trimmed().toUpper() -> NIF
    QStringList tiendas = conf->getNombreConexionesActivas();
    tiendas.prepend(conf->getConexionLocal());
    
    foreach(QString t, tiendas) {
        QSqlDatabase db = QSqlDatabase::database(t);
        if (!db.isValid()) continue;
        QSqlQuery q(db);
        q.exec("SELECT TRIM(UPPER(nombre)), nif FROM proveedores WHERE nif IS NOT NULL AND nif <> ''");
        while(q.next()) {
            mapaNombres[q.value(0).toString()] = q.value(1).toString();
        }
    }
    
    // 2. Rellenar huecos en cada tienda
    int propagados = 0;
    foreach(QString t, tiendas) {
        QSqlDatabase db = QSqlDatabase::database(t);
        if (!db.isValid() || !db.isOpen()) continue;
        db.transaction();
        QSqlQuery q(db);
        q.exec("SELECT idProveedor, TRIM(UPPER(nombre)), nif FROM proveedores WHERE nif IS NULL OR nif = ''");
        
        int tiendaCont = 0;
        while(q.next()) {
            QString id = q.value(0).toString();
            QString nombre = q.value(1).toString();
            
            if(mapaNombres.contains(nombre)) {
                QString nifEncontrado = mapaNombres[nombre];
                QSqlQuery up(db);
                up.prepare("UPDATE proveedores SET nif = ? WHERE idProveedor = ?");
                up.addBindValue(nifEncontrado);
                up.addBindValue(id);
                if(up.exec()) {
                    tiendaCont++;
                    log(QString(" - %1: NIF %2 asignado a %3").arg(t).arg(nifEncontrado).arg(nombre));
                }
            }
        }
        db.commit();
        propagados += tiendaCont;
    }
    log(QString("<b>Propagación finalizada. %1 NIFs recuperados.</b>").arg(propagados));
}

// =============================================================================
// FASE 1: UNIFICACIÓN DE IDs
// =============================================================================

void UnificarProveedores::on_pushButtonAnalizarIDs_clicked()
{
    QString tiendaRemota = ui->comboBoxTiendas->currentText();
    if(tiendaRemota.isEmpty()) return;
    
    ui->tableWidgetIDs->setRowCount(0);
    discrepanciasActuales.clear();
    
    // 1. Cargar Maestros Locales (Local = Master)
    struct InfoProv { QString nombre; int id; };
    QMap<QString, InfoProv> maestrosLocales; // NIF -> Info
    QSqlQuery qLocal(QSqlDatabase::database(conf->getConexionLocal()));
    qLocal.exec("SELECT nif, nombre, idProveedor FROM proveedores WHERE nif IS NOT NULL AND nif <> ''");
    while(qLocal.next()) {
        InfoProv inf;
        inf.nombre = qLocal.value(1).toString();
        inf.id = qLocal.value(2).toInt();
        maestrosLocales[qLocal.value(0).toString()] = inf;
    }
    
    // 2. Cargar Remotos
    QMap<QString, InfoProv> remotos;
    QSqlQuery qRemoto(QSqlDatabase::database(tiendaRemota));
    qRemoto.exec("SELECT nif, nombre, idProveedor FROM proveedores WHERE nif IS NOT NULL AND nif <> ''");
    while(qRemoto.next()) {
        InfoProv inf;
        inf.nombre = qRemoto.value(1).toString();
        inf.id = qRemoto.value(2).toInt();
        remotos[qRemoto.value(0).toString()] = inf;
    }
    
    // 3. Cruzar Datos
    
    // CASO 1 y 3: Recorrer remotos
    QMapIterator<QString, InfoProv> itR(remotos);
    while (itR.hasNext()) {
        itR.next();
        QString nif = itR.key();
        InfoProv ri = itR.value();
        
        Discrepancia d;
        d.nif = nif;
        d.nombre = ri.nombre;
        d.idRemoto = ri.id;
        
        if (maestrosLocales.contains(nif)) {
            // Caso 1: Existe en ambos. Comprobar ID.
            d.idLocal = maestrosLocales[nif].id;
            if (d.idLocal != d.idRemoto) {
                d.accion = "Sincronizar";
            } else {
                continue; // Ya están unificados, saltar.
            }
        } else {
            // Caso 3: Solo en remoto.
            d.idLocal = 0; // Por asignar
            d.accion = "Importar";
        }
        discrepanciasActuales.append(d);
    }
    
    // CASO 2: Solo en Maestro (Clonar)
    QMapIterator<QString, InfoProv> itL(maestrosLocales);
    while (itL.hasNext()) {
        itL.next();
        QString nif = itL.key();
        if (!remotos.contains(nif)) {
            Discrepancia d;
            d.nif = nif;
            d.nombre = itL.value().nombre;
            d.idLocal = itL.value().id;
            d.idRemoto = 0;
            d.accion = "Clonar";
            discrepanciasActuales.append(d);
        }
    }
    
    // 4. Llenar tabla UI
    ui->tableWidgetIDs->setRowCount(discrepanciasActuales.size());
    for(int i=0; i < discrepanciasActuales.size(); ++i) {
        const Discrepancia &d = discrepanciasActuales[i];
        ui->tableWidgetIDs->setItem(i, 0, new QTableWidgetItem(d.nombre));
        ui->tableWidgetIDs->setItem(i, 1, new QTableWidgetItem(d.nif));
        ui->tableWidgetIDs->setItem(i, 2, new QTableWidgetItem(d.idLocal > 0 ? QString::number(d.idLocal) : "N/A"));
        ui->tableWidgetIDs->setItem(i, 3, new QTableWidgetItem(d.idRemoto > 0 ? QString::number(d.idRemoto) : "N/A"));
        
        QTableWidgetItem *itemAccion = new QTableWidgetItem(d.accion);
        if(d.accion == "Sincronizar") itemAccion->setForeground(QBrush(Qt::blue));
        if(d.accion == "Clonar") itemAccion->setForeground(QBrush(Qt::darkGreen));
        if(d.accion == "Importar") itemAccion->setForeground(QBrush(Qt::red));
        ui->tableWidgetIDs->setItem(i, 4, itemAccion);
    }
}

void UnificarProveedores::on_pushButtonGenerarSQL_clicked()
{
    if(discrepanciasActuales.isEmpty()) {
        QMessageBox::information(this, "Aviso", "No hay discrepancias que procesar.");
        return;
    }
    
    QString sql;
    sql += "-- SCRIPT DE UNIFICACIÓN GLOBAL\n";
    sql += "-- Tienda Remota: " + ui->comboBoxTiendas->currentText() + "\n";
    sql += "START TRANSACTION;\n\n";
    
    foreach(const Discrepancia &d, discrepanciasActuales) {
        sql += "-- Proveedor: " + d.nombre + " (NIF: " + d.nif + ")\n";
        
        if (d.accion == "Sincronizar") {
            QString oldId = QString::number(d.idRemoto);
            QString newId = QString::number(d.idLocal);
            sql += QString("UPDATE facturas SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
            sql += QString("UPDATE albaranes SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
            sql += QString("UPDATE albaranes_tmp SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
            sql += QString("UPDATE pedidos SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
            sql += QString("UPDATE lineaspedido SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
            sql += QString("UPDATE proveedores SET idProveedor = %1 WHERE idProveedor = %2;\n").arg(newId).arg(oldId);
        }
        else if (d.accion == "Clonar") {
            sql += QString("-- [Clonar] Insertar en Remoto con ID %1\n").arg(d.idLocal);
            sql += QString("INSERT INTO proveedores (idProveedor, nombre, nif) VALUES (%1, '%2', '%3');\n")
                    .arg(d.idLocal).arg(d.nombre).arg(d.nif);
        }
        else if (d.accion == "Importar") {
            sql += "-- [Importar] Requiere creación previa en Maestro para obtener ID nuevo.\n";
            sql += QString("-- INSERT INTO proveedores (nombre, nif) VALUES ('%1', '%2');\n").arg(d.nombre).arg(d.nif);
        }
        sql += "\n";
    }
    
    sql += "COMMIT;\n";
    
    QDialog *view = new QDialog(this);
    view->setWindowTitle("Script SQL Generado");
    QVBoxLayout *lay = new QVBoxLayout(view);
    QTextEdit *edit = new QTextEdit(view);
    edit->setFont(QFont("Courier New", 10));
    edit->setPlainText(sql);
    lay->addWidget(edit);
    view->resize(800, 600);
    view->exec();
}

void UnificarProveedores::on_pushButtonEjecutarUnificacion_clicked()
{
    if(discrepanciasActuales.isEmpty()) return;
    
    QString tienda = ui->comboBoxTiendas->currentText();
    QString msg = QString("Se van a procesar %1 cambios en %2.\n\n"
                         "Esto incluye Sincronizar IDs, Clonar maestros e Importar nuevos.\n"
                         "¿Deseas continuar?").arg(discrepanciasActuales.size()).arg(tienda);
                         
    if(QMessageBox::warning(this, "Confirmar Unificación", msg, QMessageBox::Yes | QMessageBox::No) != QMessageBox::Yes) return;
        
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    QSqlDatabase dbRemota = QSqlDatabase::database(tienda);
    
    foreach(const Discrepancia &d, discrepanciasActuales) {
        if (d.accion == "Sincronizar") {
            // Antes de sincronizar, liberar el ID de destino en la remota si está ocupado
            if (!resolverConflictoID(dbRemota, d.idLocal, d.nif)) continue;

            dbRemota.transaction();
            QString oldId = QString::number(d.idRemoto);
            QString newId = QString::number(d.idLocal);
            QStringList tablas = {"facturas", "albaranes", "albaranes_tmp", "pedidos", "lineaspedido", "proveedores"};
            foreach(QString tabla, tablas) {
                QSqlQuery q(dbRemota);
                q.prepare(QString("UPDATE %1 SET idProveedor = ? WHERE idProveedor = ?").arg(tabla));
                q.addBindValue(newId);
                q.addBindValue(oldId);
                if(!q.exec()) { 
                    dbRemota.rollback(); 
                    log("Error sincronizando tabla " + tabla + ": " + q.lastError().text());
                    break; 
                }
            }
            dbRemota.commit();
        }
        else if (d.accion == "Clonar") {
            // Antes de clonar, liberar el ID local en la remota si está ocupado
            if (!resolverConflictoID(dbRemota, d.idLocal, d.nif)) continue;

            // Del Maestro -> A la Remota (Clonado completo con ID original)
            QSqlQuery qL(dbLocal);
            qL.prepare("SELECT * FROM proveedores WHERE idProveedor = ?");
            qL.addBindValue(d.idLocal);
            if(qL.exec() && qL.next()) {
                QStringList datos;
                QSqlRecord rec = qL.record();
                for (int i = 0; i < rec.count(); ++i) {
                    QString val = qL.value(i).toString();
                    if (i == 13 && val.isEmpty()) val = "2000-01-01"; // Sanear fecha
                    datos << val;
                }
                
                if (base.crearProveedor(dbRemota, datos)) {
                    log(QString("Proveedor %1 clonado a remota con ID %2").arg(d.nombre).arg(d.idLocal));
                }
            }
        }
        else if (d.accion == "Importar") {
            // De la Remota -> Al Maestro
            QSqlQuery qR(dbRemota);
            qR.prepare("SELECT * FROM proveedores WHERE idProveedor = ?");
            qR.addBindValue(d.idRemoto);
            if(qR.exec() && qR.next()) {
                QStringList datos;
                QSqlRecord rec = qR.record();
                for (int i = 0; i < rec.count(); ++i) {
                    QString val = qR.value(i).toString();
                    if (i == 13 && val.isEmpty()) val = "2000-01-01"; // Sanear fecha
                    datos << val;
                }
                
                // Intentar insertar en local con el mismo ID de la remota
                if (base.crearProveedor(dbLocal, datos)) {
                    log(QString("Proveedor %1 importado con ID %2").arg(d.nombre).arg(d.idRemoto));
                } else {
                    // Si el ID está ocupado en local, generar uno nuevo
                    dbLocal.transaction();
                    QSqlQuery qId(dbLocal);
                    qId.exec("SELECT MAX(idProveedor) FROM proveedores");
                    int nuevoIdMaster = (qId.next()) ? qId.value(0).toInt() + 1 : 1;
                    
                    datos.replace(0, QString::number(nuevoIdMaster));
                    
                    if (base.crearProveedor(dbLocal, datos)) {
                        dbLocal.commit();
                        log(QString("Proveedor %1 importado con NUEVO ID %2 (re-sincronizando remota)").arg(d.nombre).arg(nuevoIdMaster));
                        // Sincronizar la remota
                        dbRemota.transaction();
                        QStringList tablas = {"facturas", "albaranes", "albaranes_tmp", "pedidos", "lineaspedido", "proveedores"};
                        foreach(QString tabla, tablas) {
                            QSqlQuery qUp(dbRemota);
                            qUp.prepare(QString("UPDATE %1 SET idProveedor = ? WHERE idProveedor = ?").arg(tabla));
                            qUp.addBindValue(nuevoIdMaster);
                            qUp.addBindValue(d.idRemoto);
                            qUp.exec();
                        }
                        dbRemota.commit();
                    } else {
                        dbLocal.rollback();
                    }
                }
            }
        }
    }
    
    QMessageBox::information(this, "Éxito", "Proceso de unificación finalizado.");
    on_pushButtonAnalizarIDs_clicked(); // Refrescar
}

bool UnificarProveedores::resolverConflictoID(QSqlDatabase db, int targetId, QString exceptNif)
{
    // 1. Comprobar si el ID está ocupado
    QSqlQuery q(db);
    q.prepare("SELECT nif FROM proveedores WHERE idProveedor = ?");
    q.addBindValue(targetId);
    if (!q.exec() || !q.next()) return true; // Libre

    QString nifExistente = q.value(0).toString();
    if (nifExistente == exceptNif) return true; // Ya unificado

    // 2. Está ocupado por otro. Desplazar.
    QSqlQuery qMax(db);
    qMax.exec("SELECT MAX(idProveedor) FROM proveedores");
    int nuevoIdTmp = (qMax.next()) ? qMax.value(0).toInt() + 1 : 1000;

    log(QString("<font color='orange'>Aviso: ID %1 ocupado en %2. Desplazando proveedor antiguo a ID %3...</font>")
        .arg(targetId).arg(db.connectionName()).arg(nuevoIdTmp));

    db.transaction();
    QStringList tablas = {"facturas", "albaranes", "albaranes_tmp", "pedidos", "lineaspedido", "proveedores"};
    foreach(QString tabla, tablas) {
        QSqlQuery qUp(db);
        qUp.prepare(QString("UPDATE %1 SET idProveedor = ? WHERE idProveedor = ?").arg(tabla));
        qUp.addBindValue(nuevoIdTmp);
        qUp.addBindValue(targetId);
        if (!qUp.exec()) {
            db.rollback();
            log("Error desplazando conflictivo: " + qUp.lastError().text());
            return false;
        }
    }
    db.commit();
    return true;
}
