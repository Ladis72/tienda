#include "dialogcomparartraspaso.h"
#include "ui_dialogcomparartraspaso.h"

#include <QDate>
#include <QDebug>
#include <QMessageBox>
#include <QSqlError>
#include <QTableWidgetItem>

/**
 * @brief Constructor del diálogo de comparación de traspasos entre tiendas.
 */
DialogCompararTraspaso::DialogCompararTraspaso(const QString &nombreLocal,
                                               const QString &nombreRemota,
                                               const QString &tablaLocal,
                                               const QString &tablaRemota,
                                               int idTiendaRemotaEnLocal,
                                               int idTiendaLocalEnRemota,
                                               const QString &connRemota,
                                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCompararTraspaso)
    , m_nombreLocal(nombreLocal)
    , m_nombreRemota(nombreRemota)
    , m_tablaLocal(tablaLocal)
    , m_tablaRemota(tablaRemota)
    , m_idTiendaRemotaEnLocal(idTiendaRemotaEnLocal)
    , m_idTiendaLocalEnRemota(idTiendaLocalEnRemota)
    , m_connRemota(connRemota)
{
    ui->setupUi(this);

    // Configurar títulos y encabezados de la comparativa
    ui->labelTitulo->setText(QString("Comparación: %1 ⟷ %2").arg(m_nombreLocal, m_nombreRemota));
    ui->labelSubtitulo->setText(QString("Tabla Local [%1] vs Tabla Remota [%2 en %3]")
                                    .arg(m_tablaLocal, m_tablaRemota, m_nombreRemota));

    // Configurar columnas de la tabla comparativa (Código, Descripción, Cantidades, Precios, Caducidades y Diagnóstico)
    ui->tableWidgetComparativa->setColumnCount(10);
    QStringList headers;
    headers << tr("Código")
            << tr("Descripción")
            << QString("Cant. %1 (Local)").arg(m_nombreLocal)
            << QString("Cant. %1 (Remota)").arg(m_nombreRemota)
            << tr("Dif. Cant.")
            << QString("PVP %1").arg(m_nombreLocal)
            << QString("PVP %1").arg(m_nombreRemota)
            << QString("Cad. %1").arg(m_nombreLocal)
            << QString("Cad. %1").arg(m_nombreRemota)
            << tr("Estado / Diagnóstico");
    ui->tableWidgetComparativa->setHorizontalHeaderLabels(headers);

    cargarDatos();
}

/**
 * @brief Destructor del diálogo.
 */
DialogCompararTraspaso::~DialogCompararTraspaso()
{
    delete ui;
}

/**
 * @brief Carga los datos de las tablas local y remota y ejecuta la comparativa.
 */
void DialogCompararTraspaso::cargarDatos()
{
    m_locales.clear();
    m_remotas.clear();

    // 1. Cargar datos locales desde la conexión local activa
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    if (dbLocal.isOpen()) {
        QSqlQuery qLocal(dbLocal);
        QString sqlLocal = QString("SELECT id, cod, DATE_FORMAT(fechaEntrada, '%Y-%m-%d') AS f_ent, "
                                   "descripcion, cantidad, DATE_FORMAT(fechaCaducidad, '%Y-%m-%d') AS f_cad, "
                                   "pvp, idTienda FROM %1 WHERE idTienda = ?")
                               .arg(m_tablaLocal);
        qLocal.prepare(sqlLocal);
        qLocal.bindValue(0, m_idTiendaRemotaEnLocal);

        if (qLocal.exec()) {
            while (qLocal.next()) {
                LineaMercancia l;
                l.id = qLocal.value("id").toInt();
                l.cod = qLocal.value("cod").toString().trimmed();
                l.fechaEntrada = qLocal.value("f_ent").toString();
                l.descripcion = qLocal.value("descripcion").toString();
                l.cantidad = qLocal.value("cantidad").toDouble();
                l.fechaCaducidad = qLocal.value("f_cad").toString();
                l.pvp = qLocal.value("pvp").toDouble();
                l.idTienda = qLocal.value("idTienda").toInt();

                if (m_locales.contains(l.cod)) {
                    m_locales[l.cod].cantidad += l.cantidad;
                } else {
                    m_locales.insert(l.cod, l);
                }
            }
        } else {
            qWarning() << "DialogCompararTraspaso: Error al cargar datos locales:" << qLocal.lastError().text();
        }
    }

    // 2. Cargar datos remotos desde la conexión remota indicada (sin forzar apertura de conexión)
    QSqlDatabase dbRemota = QSqlDatabase::database(m_connRemota, false);
    if (dbRemota.isOpen()) {
        QSqlQuery qRemota(dbRemota);
        QString sqlRemota = QString("SELECT id, cod, DATE_FORMAT(fechaEntrada, '%Y-%m-%d') AS f_ent, "
                                    "descripcion, cantidad, DATE_FORMAT(fechaCaducidad, '%Y-%m-%d') AS f_cad, "
                                    "pvp, idTienda FROM %1 WHERE idTienda = ?")
                                .arg(m_tablaRemota);
        qRemota.prepare(sqlRemota);
        qRemota.bindValue(0, m_idTiendaLocalEnRemota);

        if (qRemota.exec()) {
            while (qRemota.next()) {
                LineaMercancia r;
                r.id = qRemota.value("id").toInt();
                r.cod = qRemota.value("cod").toString().trimmed();
                r.fechaEntrada = qRemota.value("f_ent").toString();
                r.descripcion = qRemota.value("descripcion").toString();
                r.cantidad = qRemota.value("cantidad").toDouble();
                r.fechaCaducidad = qRemota.value("f_cad").toString();
                r.pvp = qRemota.value("pvp").toDouble();
                r.idTienda = qRemota.value("idTienda").toInt();

                if (m_remotas.contains(r.cod)) {
                    m_remotas[r.cod].cantidad += r.cantidad;
                } else {
                    m_remotas.insert(r.cod, r);
                }
            }
        } else {
            qWarning() << "DialogCompararTraspaso: Error al consultar tienda remota:" << qRemota.lastError().text();
        }
    }

    compararYMostrar();
}

/**
 * @brief Compara las colecciones local y remota y rellena la tabla con colores de diagnóstico.
 */
/**
 * @brief Compara las colecciones local y remota (cantidades, precios y fechas de caducidad)
 * y rellena la tabla con colores de diagnóstico e información detallada.
 */
void DialogCompararTraspaso::compararYMostrar()
{
    ui->tableWidgetComparativa->setRowCount(0);

    // Obtener conjunto ordenado de todos los códigos presentes en ambas tablas
    QSet<QString> todosLosCodigos;
    for (auto it = m_locales.begin(); it != m_locales.end(); ++it) {
        todosLosCodigos.insert(it.key());
    }
    for (auto it = m_remotas.begin(); it != m_remotas.end(); ++it) {
        todosLosCodigos.insert(it.key());
    }
    QStringList listaCodigos = todosLosCodigos.values();
    listaCodigos.sort();

    int coincidentes = 0;
    int faltantesLocal = 0;
    int sobrantesLocal = 0;
    int conDiferencias = 0;

    int fila = 0;
    for (const QString &cod : listaCodigos) {
        bool existeLocal = m_locales.contains(cod);
        bool existeRemota = m_remotas.contains(cod);

        double cantLocal = existeLocal ? m_locales[cod].cantidad : 0.0;
        double cantRemota = existeRemota ? m_remotas[cod].cantidad : 0.0;
        double difCant = cantLocal - cantRemota;

        double pvpLocal = existeLocal ? m_locales[cod].pvp : 0.0;
        double pvpRemota = existeRemota ? m_remotas[cod].pvp : 0.0;

        QString descripcion = existeLocal ? m_locales[cod].descripcion : m_remotas[cod].descripcion;
        QString fechaCadLocal = existeLocal ? m_locales[cod].fechaCaducidad : "";
        QString fechaCadRemota = existeRemota ? m_remotas[cod].fechaCaducidad : "";

        QString estado;
        QColor colorFondo;

        if (existeLocal && existeRemota) {
            // Comparar cantidades, precios (PVP) y fechas de caducidad
            bool coincideCant = qAbs(cantLocal - cantRemota) < 0.001;
            bool coincidePvp = qAbs(pvpLocal - pvpRemota) < 0.001;
            bool coincideCad = (fechaCadLocal == fechaCadRemota);

            if (coincideCant && coincidePvp && coincideCad) {
                estado = tr("✓ Coincide");
                colorFondo = QColor(232, 245, 233); // Verde suave (#e8f5e9)
                coincidentes++;
            } else {
                // Especificar qué campos presentan diferencias
                QStringList difs;
                if (!coincideCant) difs << tr("Cantidad");
                if (!coincidePvp) difs << tr("PVP");
                if (!coincideCad) difs << tr("Caducidad");

                estado = tr("⚠ Dif. en %1").arg(difs.join(", "));
                colorFondo = QColor(255, 243, 224); // Naranja suave (#fff3e0)
                conDiferencias++;
            }
        } else if (!existeLocal && existeRemota) {
            estado = tr("❌ Falta en Local");
            colorFondo = QColor(255, 235, 238); // Rojo suave (#ffebee)
            faltantesLocal++;
        } else {
            estado = tr("➕ Solo en Local");
            colorFondo = QColor(237, 231, 246); // Púrpura suave (#ede7f6)
            sobrantesLocal++;
        }

        ui->tableWidgetComparativa->insertRow(fila);

        // Crear celdas con alineaciones y formatos específicos
        QTableWidgetItem *itemCod = new QTableWidgetItem(cod);
        QTableWidgetItem *itemDesc = new QTableWidgetItem(descripcion);
        QTableWidgetItem *itemCantLocal = new QTableWidgetItem(existeLocal ? QString::number(cantLocal, 'f', 2) : "-");
        QTableWidgetItem *itemCantRemota = new QTableWidgetItem(existeRemota ? QString::number(cantRemota, 'f', 2) : "-");
        QTableWidgetItem *itemDifCant = new QTableWidgetItem((difCant != 0.0) ? QString("%1%2").arg(difCant > 0 ? "+" : "", QString::number(difCant, 'f', 2)) : "0.00");
        QTableWidgetItem *itemPvpLocal = new QTableWidgetItem(existeLocal ? QString("%1 €").arg(QString::number(pvpLocal, 'f', 2)) : "-");
        QTableWidgetItem *itemPvpRemota = new QTableWidgetItem(existeRemota ? QString("%1 €").arg(QString::number(pvpRemota, 'f', 2)) : "-");
        QTableWidgetItem *itemCadLocal = new QTableWidgetItem(existeLocal ? (fechaCadLocal.isEmpty() ? "-" : fechaCadLocal) : "-");
        QTableWidgetItem *itemCadRemota = new QTableWidgetItem(existeRemota ? (fechaCadRemota.isEmpty() ? "-" : fechaCadRemota) : "-");
        QTableWidgetItem *itemEstado = new QTableWidgetItem(estado);

        // Alineación numérica y de fechas
        itemCantLocal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemCantRemota->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemDifCant->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemPvpLocal->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemPvpRemota->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        itemCadLocal->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        itemCadRemota->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);
        itemEstado->setTextAlignment(Qt::AlignCenter | Qt::AlignVCenter);

        // Aplicar color de fondo diagnóstico a la fila completa
        QList<QTableWidgetItem*> items = {itemCod, itemDesc, itemCantLocal, itemCantRemota, itemDifCant,
                                          itemPvpLocal, itemPvpRemota, itemCadLocal, itemCadRemota, itemEstado};
        for (QTableWidgetItem *it : items) {
            it->setBackground(colorFondo);
        }

        ui->tableWidgetComparativa->setItem(fila, 0, itemCod);
        ui->tableWidgetComparativa->setItem(fila, 1, itemDesc);
        ui->tableWidgetComparativa->setItem(fila, 2, itemCantLocal);
        ui->tableWidgetComparativa->setItem(fila, 3, itemCantRemota);
        ui->tableWidgetComparativa->setItem(fila, 4, itemDifCant);
        ui->tableWidgetComparativa->setItem(fila, 5, itemPvpLocal);
        ui->tableWidgetComparativa->setItem(fila, 6, itemPvpRemota);
        ui->tableWidgetComparativa->setItem(fila, 7, itemCadLocal);
        ui->tableWidgetComparativa->setItem(fila, 8, itemCadRemota);
        ui->tableWidgetComparativa->setItem(fila, 9, itemEstado);

        fila++;
    }

    ui->tableWidgetComparativa->resizeColumnsToContents();

    // Actualizar etiqueta resumen en cabecera
    ui->labelResumen->setText(QString("Coinciden: %1 | Faltan en local: %2 | Con diferencias: %3 | Solo local: %4")
                                  .arg(coincidentes)
                                  .arg(faltantesLocal)
                                  .arg(conDiferencias)
                                  .arg(sobrantesLocal));

    // Habilitar o deshabilitar botones de resolución según haya diferencias
    ui->btnImportarFaltantes->setEnabled(faltantesLocal > 0);
    ui->btnAjustarCantidades->setEnabled(conDiferencias > 0);
    ui->btnIgualarTodo->setEnabled(!m_remotas.isEmpty());
}

/**
 * @brief Importa en la tabla local las líneas que existen en la tienda remota pero no en la local.
 */
void DialogCompararTraspaso::on_btnImportarFaltantes_clicked()
{
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    if (!dbLocal.isOpen()) {
        QMessageBox::critical(this, tr("Error"), tr("No hay conexión con la base de datos local."));
        return;
    }

    int importadas = 0;
    QSqlQuery qInsert(dbLocal);
    QString sql = QString("INSERT INTO %1 (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                          "VALUES (?, ?, ?, ?, ?, ?, ?)")
                      .arg(m_tablaLocal);
    qInsert.prepare(sql);

    for (auto it = m_remotas.begin(); it != m_remotas.end(); ++it) {
        const QString &cod = it.key();
        if (!m_locales.contains(cod)) {
            const LineaMercancia &l = it.value();
            qInsert.bindValue(0, l.cod);
            qInsert.bindValue(1, l.fechaEntrada.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : l.fechaEntrada);
            qInsert.bindValue(2, l.descripcion);
            qInsert.bindValue(3, l.cantidad);
            qInsert.bindValue(4, l.fechaCaducidad.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : l.fechaCaducidad);
            qInsert.bindValue(5, l.pvp);
            qInsert.bindValue(6, m_idTiendaRemotaEnLocal);

            if (qInsert.exec()) {
                importadas++;
            } else {
                qWarning() << "Error al importar línea a local:" << qInsert.lastError().text();
            }
        }
    }

    emit datosModificados();
    QMessageBox::information(this, tr("Líneas Importadas"),
                             tr("Se han importado correctamente %1 productos faltantes a la tabla local.")
                                 .arg(importadas));
    cargarDatos();
}

/**
 * @brief Ajusta los datos (cantidades, PVP y fecha de caducidad) en la tabla local para igualarlos a los de la tienda remota.
 */
void DialogCompararTraspaso::on_btnAjustarCantidades_clicked()
{
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    if (!dbLocal.isOpen()) {
        QMessageBox::critical(this, tr("Error"), tr("No hay conexión con la base de datos local."));
        return;
    }

    int ajustadas = 0;
    QSqlQuery qUpdate(dbLocal);
    QString sql = QString("UPDATE %1 SET cantidad = ?, pvp = ?, fechaCaducidad = ? WHERE cod = ? AND idTienda = ?")
                      .arg(m_tablaLocal);
    qUpdate.prepare(sql);

    for (auto it = m_remotas.begin(); it != m_remotas.end(); ++it) {
        const QString &cod = it.key();
        if (m_locales.contains(cod)) {
            const LineaMercancia &rem = it.value();
            const LineaMercancia &loc = m_locales[cod];

            bool difCant = qAbs(loc.cantidad - rem.cantidad) >= 0.001;
            bool difPvp = qAbs(loc.pvp - rem.pvp) >= 0.001;
            bool difCad = (loc.fechaCaducidad != rem.fechaCaducidad);

            // Si hay alguna discrepancia en cantidad, precio o fecha de caducidad, actualizar en local
            if (difCant || difPvp || difCad) {
                qUpdate.bindValue(0, rem.cantidad);
                qUpdate.bindValue(1, rem.pvp);
                qUpdate.bindValue(2, rem.fechaCaducidad.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : rem.fechaCaducidad);
                qUpdate.bindValue(3, cod);
                qUpdate.bindValue(4, m_idTiendaRemotaEnLocal);
                if (qUpdate.exec()) {
                    ajustadas++;
                } else {
                    qWarning() << "Error al ajustar datos locales:" << qUpdate.lastError().text();
                }
            }
        }
    }

    emit datosModificados();
    QMessageBox::information(this, tr("Datos Ajustados"),
                             tr("Se han actualizado los datos (cantidades, PVP y caducidad) de %1 productos para igualar a la tienda remota.")
                                 .arg(ajustadas));
    cargarDatos();
}

/**
 * @brief Reemplaza completamente la tabla local por las líneas de la tienda remota.
 */
void DialogCompararTraspaso::on_btnIgualarTodo_clicked()
{
    QMessageBox msgBox(this);
    msgBox.setIcon(QMessageBox::Question);
    msgBox.setWindowTitle(tr("Confirmar Sincronización Total"));
    msgBox.setText(tr("¿Desea sustituir todas las líneas locales de este traspaso por las de la tienda remota?"));
    msgBox.setInformativeText(tr("Esta acción borrará las líneas locales actuales con destino/origen %1 y copiará exactamente las de %2.")
                                  .arg(m_nombreRemota, m_nombreRemota));
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    if (msgBox.exec() != QMessageBox::Yes) {
        return;
    }

    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    if (!dbLocal.isOpen()) {
        QMessageBox::critical(this, tr("Error"), tr("No hay conexión con la base de datos local."));
        return;
    }

    // 1. Borrar líneas locales actuales vinculadas a esta tienda
    QSqlQuery qDelete(dbLocal);
    qDelete.prepare(QString("DELETE FROM %1 WHERE idTienda = ?").arg(m_tablaLocal));
    qDelete.bindValue(0, m_idTiendaRemotaEnLocal);
    qDelete.exec();

    // 2. Insertar todas las líneas remotas
    int copiadas = 0;
    QSqlQuery qInsert(dbLocal);
    qInsert.prepare(QString("INSERT INTO %1 (cod, fechaEntrada, descripcion, cantidad, fechaCaducidad, pvp, idTienda) "
                            "VALUES (?, ?, ?, ?, ?, ?, ?)")
                        .arg(m_tablaLocal));

    for (auto it = m_remotas.begin(); it != m_remotas.end(); ++it) {
        const LineaMercancia &l = it.value();
        qInsert.bindValue(0, l.cod);
        qInsert.bindValue(1, l.fechaEntrada.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : l.fechaEntrada);
        qInsert.bindValue(2, l.descripcion);
        qInsert.bindValue(3, l.cantidad);
        qInsert.bindValue(4, l.fechaCaducidad.isEmpty() ? QDate::currentDate().toString("yyyy-MM-dd") : l.fechaCaducidad);
        qInsert.bindValue(5, l.pvp);
        qInsert.bindValue(6, m_idTiendaRemotaEnLocal);

        if (qInsert.exec()) {
            copiadas++;
        }
    }

    emit datosModificados();
    QMessageBox::information(this, tr("Sincronización Completada"),
                             tr("Se han volcado %1 productos de la tienda remota a la tabla local.").arg(copiadas));
    cargarDatos();
}
