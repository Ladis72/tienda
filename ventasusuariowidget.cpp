#include "ventasusuariowidget.h"
#include <QDebug>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include <QKeyEvent>
#include <QToolTip>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QBarSet>
#include <QtCharts/QChart>
#include <QtCharts/QChartView>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts>
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "syncmanager.h"
#include "ui_ventasusuariowidget.h"

//using namespace QtCharts;

ventasUsuarioWidget::ventasUsuarioWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ventasUsuarioWidget)
{
    ui->setupUi(this);
    // Constructor de ResumenVentasWidget

    ui->dateDesde->setDate(QDate(2025, 01, 01));
    ui->dateHasta->setDate(QDate::currentDate());
    ui->dateDesde->setDisplayFormat("yyyy-MM-dd");
    ui->dateHasta->setDisplayFormat("yyyy-MM-dd");
    // Activar por defecto la vista global para mostrar las ventas del trabajador en todas las tiendas
    ui->chkGlobal->setChecked(true);
    connect(ui->radioDia, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->radioMes, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->radioAnio, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);

    connect(ui->dateDesde, &QDateEdit::dateChanged, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->dateHasta, &QDateEdit::dateChanged, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->chkGlobal, &QCheckBox::toggled, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->tabWidge, &QTabWidget::currentChanged, this, &ventasUsuarioWidget::actualizarResumen);
    usuario = "1";
    m_primeraCarga = true;
    this->setFocusPolicy(Qt::StrongFocus);

    // Habilitar ordenación en las tablas
    ui->tableResumen->setSortingEnabled(true);
    ui->tableHoras->setSortingEnabled(true);
    ui->tableSemana->setSortingEnabled(true);
    ui->tableTickets->setSortingEnabled(true);
    ui->tableProductos->setSortingEnabled(true);

    // Ajustes visuales de cabeceras
    ui->tableResumen->horizontalHeader()->setStretchLastSection(true);
    ui->tableHoras->horizontalHeader()->setStretchLastSection(true);
    ui->tableSemana->horizontalHeader()->setStretchLastSection(true);
    ui->tableTickets->horizontalHeader()->setStretchLastSection(true);
    ui->tableProductos->horizontalHeader()->setStretchLastSection(true);
}

ventasUsuarioWidget::~ventasUsuarioWidget()
{
    delete ui;
}

void ventasUsuarioWidget::setUser(QString usuarioActual)
{
    usuario = usuarioActual;
    actualizarResumen();
}

void ventasUsuarioWidget::actualizarResumen()
{
    QString agrupacion;

    if (ui->radioDia->isChecked())
        agrupacion = "dia";
    else if (ui->radioMes->isChecked())
        agrupacion = "mes";
    else
        agrupacion = "anio";

    QDate desde = ui->dateDesde->date();
    QDate hasta = ui->dateHasta->date();

    int index = ui->tabWidge->currentIndex();
    switch (index) {
    case 0:
        actualizarFechas(agrupacion, desde, hasta, usuario);
        break;
    case 1:
        actualizarHoras(desde, hasta, usuario);
        break;
    case 2:
        actualizarSemana(desde, hasta, usuario);
        break;
    case 3:
        actualizarTickets(desde, hasta, usuario);
        break;
    case 4:
        actualizarProductos(desde, hasta, usuario);
        break;
    }

    // Actualizar siempre la barra superior de estadísticas globales con independencia de la pestaña activa
    actualizarEstadisticas(desde, hasta, usuario);
}

void ventasUsuarioWidget::actualizarFechas(const QString &agrupacion,
                                         const QDate &desde,
                                         const QDate &hasta,
                                         QString &usuario)
{
    QString formatoFechaSQL;

    // Selecciona el formato SQL según el tipo de agrupación (formato "yyyy-MM-dd" para día)
    if (agrupacion == "dia")
        formatoFechaSQL = "DATE_FORMAT(fecha, '%Y-%m-%d')"; // Ej: 2024-05-21
    else if (agrupacion == "mes")
        formatoFechaSQL = "DATE_FORMAT(fecha, '%Y-%m')"; // Ej: 2024-05
    else if (agrupacion == "anio")
        formatoFechaSQL = "YEAR(fecha)"; // Ej: 2024

    // Consulta que unifica ambas tablas de tickets
    QString consulta = QString(R"(
        SELECT
            %1 AS mes,
            SUM(ventas.total_tickets) AS v1,
            SUM(ventas.total_ticketss) AS v2
        FROM (
            SELECT usuario, fecha, total AS total_tickets, 0 AS total_ticketss FROM tickets
            UNION ALL
            SELECT usuario, fecha, 0 AS total_tickets, total AS total_ticketss FROM ticketss
        ) AS ventas
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        GROUP BY mes
        ORDER BY mes DESC;
    )").arg(formatoFechaSQL);

    QStringList conexiones = getListaConexiones();

    // Mapa para acumular resultados de diferentes tiendas
    QMap<QString, QVector<double>> acumulado;

    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) {
            qDebug() << "⚠️ Conexión no abierta para ventas:" << conn;
            continue;
        }

        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));

        if (query.exec()) {
            while (query.next()) {
                QString periodo = query.value(0).toString();
                double t1 = query.value(1).toDouble();
                double t2 = query.value(2).toDouble();

                if (!acumulado.contains(periodo)) {
                    acumulado[periodo] = {0, 0};
                }
                acumulado[periodo][0] += t1;
                acumulado[periodo][1] += t2;
            }
        } else {
            qDebug() << "❌ Error en consulta para" << conn << ":" << query.lastError().text();
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar desde tickets_nube y ticketss_nube
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("id_tienda IN (1, 2, 3) AND id_tienda != %1").arg(idTiendaLocal)
                                         : QString("id_tienda IN (1, 2, 3)");
            QSqlQuery qNube(dbNube);
            QString sqlNube = QString(R"(
                SELECT
                    %1 AS mes,
                    SUM(ventas.total_tickets) AS v1,
                    SUM(ventas.total_ticketss) AS v2
                FROM (
                    SELECT id_tienda, usuario, fecha, total AS total_tickets, 0 AS total_ticketss FROM tickets_nube
                    UNION ALL
                    SELECT id_tienda, usuario, fecha, 0 AS total_tickets, total AS total_ticketss FROM ticketss_nube
                ) AS ventas
                WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %2
                GROUP BY mes
                ORDER BY mes DESC
            )").arg(formatoFechaSQL, condTiendaNube);
            qNube.prepare(sqlNube);
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    QString periodo = qNube.value(0).toString();
                    double t1 = qNube.value(1).toDouble();
                    double t2 = qNube.value(2).toDouble();
                    if (!acumulado.contains(periodo)) {
                        acumulado[periodo] = {0, 0};
                    }
                    acumulado[periodo][0] += t1;
                    acumulado[periodo][1] += t2;
                }
            } else {
                qDebug() << "ventasUsuarioWidget::actualizarResumen Nube error:" << qNube.lastError().text();
            }
        }
    }

    // Crear modelo y llenar con datos acumulados
    QStandardItemModel *modelo = new QStandardItemModel(this);
    QStringList periodos = acumulado.keys();
    // Ordenar periodos descendente (más recientes primero)
    std::sort(periodos.begin(), periodos.end(), std::greater<QString>());

    int row = 0;
    for (const QString &p : periodos) {
        double v1 = acumulado[p][0];
        double v2 = acumulado[p][1];
        double total = v1 + v2;

        modelo->setItem(row, 0, new QStandardItem(p));
        QStandardItem *itemV1 = new QStandardItem();
        itemV1->setData(v1, Qt::EditRole);
        modelo->setItem(row, 1, itemV1);

        QStandardItem *itemV2 = new QStandardItem();
        itemV2->setData(v2, Qt::EditRole);
        modelo->setItem(row, 2, itemV2);

        QStandardItem *itemTotal = new QStandardItem();
        itemTotal->setData(total, Qt::EditRole);
        modelo->setItem(row, 3, itemTotal);
        row++;
    }

    modelo->setHeaderData(0, Qt::Horizontal, "Periodo");
    modelo->setHeaderData(1, Qt::Horizontal, "Ventas");
    modelo->setHeaderData(2, Qt::Horizontal, "B");
    modelo->setHeaderData(3, Qt::Horizontal, "Total");

    // Guardar el estado de visibilidad actual de las columnas antes de cambiar el modelo
    bool col2Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(2);
    bool col3Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(3);
    m_primeraCarga = false;

    ui->tableResumen->setSortingEnabled(false);
    ui->tableResumen->setModel(modelo);
    ui->tableResumen->setSortingEnabled(true);
    ui->tableResumen->sortByColumn(0, Qt::DescendingOrder);

    // Restaurar visibilidad (o esconder por defecto si es la primera vez)
    ui->tableResumen->setColumnHidden(2, col2Hidden);
    ui->tableResumen->setColumnHidden(3, col3Hidden);

    ui->tableResumen->resizeColumnsToContents();
    generarGraficoDesdeTabla(ui->tableResumen, ui->fechasLayout, "Resumen de ventas");
}

void ventasUsuarioWidget::actualizarHoras(const QDate &desde, const QDate &hasta, QString &usuario)
{
    QString consulta = R"(
        SELECT HOUR(hora) as h, SUM(total) as t1, 0 as t2 FROM tickets 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta GROUP BY h
        UNION ALL
        SELECT HOUR(hora) as h, 0 as t1, SUM(total) as t2 FROM ticketss 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta GROUP BY h
    )";
    QStringList conexiones = getListaConexiones();

    QMap<int, QVector<double>> acumulado;
    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                int h = query.value(0).toInt();
                if (!acumulado.contains(h)) acumulado[h] = {0, 0};
                acumulado[h][0] += query.value(1).toDouble();
                acumulado[h][1] += query.value(2).toDouble();
            }
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar ventas por hora desde tickets_nube y ticketss_nube
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("id_tienda IN (1, 2, 3) AND id_tienda != %1").arg(idTiendaLocal)
                                         : QString("id_tienda IN (1, 2, 3)");
            QSqlQuery qNube(dbNube);
            qNube.prepare(QString("SELECT HOUR(hora) as h, SUM(total) as t1, 0 as t2 FROM tickets_nube "
                                  "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1 GROUP BY h "
                                  "UNION ALL "
                                  "SELECT HOUR(hora) as h, 0 as t1, SUM(total) as t2 FROM ticketss_nube "
                                  "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1 GROUP BY h")
                              .arg(condTiendaNube));
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    int h = qNube.value(0).toInt();
                    if (!acumulado.contains(h)) acumulado[h] = {0, 0};
                    acumulado[h][0] += qNube.value(1).toDouble();
                    acumulado[h][1] += qNube.value(2).toDouble();
                }
            }
        }
    }

    QStandardItemModel *modelo = new QStandardItemModel(this);
    QList<int> horas = acumulado.keys();
    std::sort(horas.begin(), horas.end());

    int row = 0;
    for (int h : horas) {
        double v1 = acumulado[h][0];
        double v2 = acumulado[h][1];
        double total = v1 + v2;

        modelo->setItem(row, 0, new QStandardItem(QString("%1:00").arg(h, 2, 10, QChar('0'))));

        QStandardItem *itemV1 = new QStandardItem();
        itemV1->setData(v1, Qt::EditRole);
        modelo->setItem(row, 1, itemV1);

        QStandardItem *itemV2 = new QStandardItem();
        itemV2->setData(v2, Qt::EditRole);
        modelo->setItem(row, 2, itemV2);

        QStandardItem *itemTotal = new QStandardItem();
        itemTotal->setData(total, Qt::EditRole);
        modelo->setItem(row, 3, itemTotal);
        row++;
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Hora");
    modelo->setHeaderData(1, Qt::Horizontal, "Ventas");
    modelo->setHeaderData(2, Qt::Horizontal, "B");
    modelo->setHeaderData(3, Qt::Horizontal, "Total");

    bool col2Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(2);
    bool col3Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(3);
    m_primeraCarga = false;

    ui->tableHoras->setSortingEnabled(false);
    ui->tableHoras->setModel(modelo);
    ui->tableHoras->setSortingEnabled(true);
    ui->tableHoras->sortByColumn(0, Qt::AscendingOrder);

    ui->tableHoras->setColumnHidden(2, col2Hidden);
    ui->tableHoras->setColumnHidden(3, col3Hidden);
    ui->tableHoras->resizeColumnsToContents();
    generarGraficoDesdeTabla(ui->tableHoras, ui->chartLayoutHoras, "Ventas por Hora");
}

void ventasUsuarioWidget::actualizarSemana(const QDate &desde, const QDate &hasta, QString &usuario)
{
    QString consulta = R"(
        SELECT DAYOFWEEK(fecha) as d, SUM(total) as t1, 0 as t2 FROM tickets 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta GROUP BY d
        UNION ALL
        SELECT DAYOFWEEK(fecha) as d, 0 as t1, SUM(total) as t2 FROM ticketss 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta GROUP BY d
    )";
    QStringList conexiones = getListaConexiones();

    QMap<int, QVector<double>> acumulado;
    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                int d = query.value(0).toInt();
                if (!acumulado.contains(d)) acumulado[d] = {0, 0};
                acumulado[d][0] += query.value(1).toDouble();
                acumulado[d][1] += query.value(2).toDouble();
            }
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar desde tickets_nube y ticketss_nube
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("id_tienda IN (1, 2, 3) AND id_tienda != %1").arg(idTiendaLocal)
                                         : QString("id_tienda IN (1, 2, 3)");
            QSqlQuery qNube(dbNube);
            qNube.prepare(QString("SELECT DAYOFWEEK(fecha) as d, SUM(total) as t1, 0 as t2 FROM tickets_nube "
                                  "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1 GROUP BY d "
                                  "UNION ALL "
                                  "SELECT DAYOFWEEK(fecha) as d, 0 as t1, SUM(total) as t2 FROM ticketss_nube "
                                  "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1 GROUP BY d")
                              .arg(condTiendaNube));
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    int d = qNube.value(0).toInt();
                    if (!acumulado.contains(d)) acumulado[d] = {0, 0};
                    acumulado[d][0] += qNube.value(1).toDouble();
                    acumulado[d][1] += qNube.value(2).toDouble();
                }
            }
        }
    }

    QStringList diasNombres = {"", "Dom", "Lun", "Mar", "Mie", "Jue", "Vie", "Sab"};
    QStandardItemModel *modelo = new QStandardItemModel(this);
    for (int d = 1; d <= 7; ++d) {
        double v1 = acumulado.value(d, {0, 0})[0];
        double v2 = acumulado.value(d, {0, 0})[1];
        double total = v1 + v2;
        modelo->setItem(d - 1, 0, new QStandardItem(diasNombres.value(d)));
        QStandardItem *itemV1 = new QStandardItem();
        itemV1->setData(v1, Qt::EditRole);
        modelo->setItem(d - 1, 1, itemV1);

        QStandardItem *itemV2 = new QStandardItem();
        itemV2->setData(v2, Qt::EditRole);
        modelo->setItem(d - 1, 2, itemV2);

        QStandardItem *itemTotal = new QStandardItem();
        itemTotal->setData(total, Qt::EditRole);
        modelo->setItem(d - 1, 3, itemTotal);
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Día");
    modelo->setHeaderData(1, Qt::Horizontal, "Ventas");
    modelo->setHeaderData(2, Qt::Horizontal, "B");
    modelo->setHeaderData(3, Qt::Horizontal, "Total");

    bool col2Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(2);
    bool col3Hidden = m_primeraCarga ? true : ui->tableResumen->isColumnHidden(3);
    m_primeraCarga = false;

    // Mantener el orden natural de los días de la semana (Dom..Sab) sin reordenar alfabéticamente
    ui->tableSemana->setSortingEnabled(false);
    ui->tableSemana->setModel(modelo);
    ui->tableSemana->setColumnHidden(2, col2Hidden);
    ui->tableSemana->setColumnHidden(3, col3Hidden);
    ui->tableSemana->resizeColumnsToContents();
    generarGraficoDesdeTabla(ui->tableSemana, ui->chartLayoutSemana, "Ventas por Día de Semana");
}

void ventasUsuarioWidget::actualizarTickets(const QDate &desde, const QDate &hasta, QString &usuario)
{
    bool incluirB = !m_primeraCarga && !ui->tableResumen->isColumnHidden(2);

    // Se usa DATE_FORMAT y TIME_FORMAT para evitar el bug del protocolo binario de Qt6/QMYSQL con DATE/TIME
    QString consulta = R"(
        SELECT ticket, DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha_str, TIME_FORMAT(hora, '%H:%i') AS hora_str, total
        FROM tickets
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
    )";
    if (incluirB) {
        consulta += R"(
            UNION ALL
            SELECT ticket, DATE_FORMAT(fecha, '%Y-%m-%d') AS fecha_str, TIME_FORMAT(hora, '%H:%i') AS hora_str, total
            FROM ticketss
            WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        )";
    }
    consulta += " ORDER BY fecha_str DESC, hora_str DESC LIMIT 2000";

    QStringList conexiones = getListaConexiones();

    QStandardItemModel *modelo = new QStandardItemModel(this);
    int row = 0;
    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;

        // Obtener el nombre real de la tienda cuando la conexión es "DB"
        QString nombreTienda = conn;
        if (conn == "DB") {
            QSqlQuery qNombre(db);
            if (qNombre.exec("SELECT nombre FROM tiendas WHERE local = 1 LIMIT 1") && qNombre.next()) {
                nombreTienda = qNombre.value(0).toString();
            }
        }

        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                modelo->setItem(row, 0, new QStandardItem(nombreTienda));

                QStandardItem *itemTicket = new QStandardItem();
                itemTicket->setData(query.value(0).toInt(), Qt::EditRole);
                modelo->setItem(row, 1, itemTicket);

                modelo->setItem(row, 2, new QStandardItem(query.value(1).toString()));
                modelo->setItem(row, 3, new QStandardItem(query.value(2).toString()));

                QStandardItem *itemTotal = new QStandardItem();
                itemTotal->setData(query.value(3).toDouble(), Qt::EditRole);
                modelo->setItem(row, 4, itemTotal);

                row++;
            }
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar tickets desde tickets_nube (y ticketss_nube si B está activo)
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                         : QString("t.id_tienda IN (1, 2, 3)");

            QString sqlNube = QString(
                "SELECT COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda, "
                "t.ticket, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str, TIME_FORMAT(t.hora, '%H:%i') AS hora_str, t.total "
                "FROM tickets_nube t "
                "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                "WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta AND %1"
            ).arg(condTiendaNube);

            if (incluirB) {
                sqlNube += QString(
                    " UNION ALL "
                    "SELECT COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS tienda, "
                    "t.ticket, DATE_FORMAT(t.fecha, '%Y-%m-%d') AS fecha_str, TIME_FORMAT(t.hora, '%H:%i') AS hora_str, t.total "
                    "FROM ticketss_nube t "
                    "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                    "WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta AND %1"
                ).arg(condTiendaNube);
            }
            sqlNube += " ORDER BY fecha_str DESC, hora_str DESC LIMIT 2000";

            QSqlQuery qNube(dbNube);
            qNube.prepare(sqlNube);
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    modelo->setItem(row, 0, new QStandardItem(qNube.value(0).toString()));

                    QStandardItem *itemTicket = new QStandardItem();
                    itemTicket->setData(qNube.value(1).toInt(), Qt::EditRole);
                    modelo->setItem(row, 1, itemTicket);

                    modelo->setItem(row, 2, new QStandardItem(qNube.value(2).toString()));
                    modelo->setItem(row, 3, new QStandardItem(qNube.value(3).toString()));

                    QStandardItem *itemTotal = new QStandardItem();
                    itemTotal->setData(qNube.value(4).toDouble(), Qt::EditRole);
                    modelo->setItem(row, 4, itemTotal);

                    row++;
                }
            }
        }
    }

    modelo->setHeaderData(0, Qt::Horizontal, "Tienda");
    modelo->setHeaderData(1, Qt::Horizontal, "Nº");
    modelo->setHeaderData(2, Qt::Horizontal, "Fecha");
    modelo->setHeaderData(3, Qt::Horizontal, "Hora");
    modelo->setHeaderData(4, Qt::Horizontal, "Total");

    // Desactivar ordenación antes de asignar el modelo y ordenar por Fecha (columna 2) descendente
    ui->tableTickets->setSortingEnabled(false);
    ui->tableTickets->setModel(modelo);
    ui->tableTickets->setSortingEnabled(true);
    ui->tableTickets->sortByColumn(2, Qt::DescendingOrder);
    ui->tableTickets->resizeColumnsToContents();
}

void ventasUsuarioWidget::actualizarProductos(const QDate &desde, const QDate &hasta, QString &usuario)
{
    bool incluirB = !m_primeraCarga && !ui->tableResumen->isColumnHidden(2);

    // Unificar por código de artículo (o descripción si no tiene código) usando JOIN indexado sobre CAST(t.ticket AS CHAR)
    QString consulta = R"(
        SELECT COALESCE(NULLIF(TRIM(l.cod), ''), TRIM(l.descripcion)) AS clave_prod,
               COALESCE(NULLIF(TRIM(a.descripcion), ''), TRIM(l.descripcion)) AS desc_prod,
               SUM(l.cantidad) AS c,
               SUM(l.totallinea) AS t
        FROM tickets t
        INNER JOIN lineasticket l ON l.nticket = CAST(t.ticket AS CHAR)
        LEFT JOIN articulos a ON l.cod = a.cod
        WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta
        GROUP BY clave_prod, desc_prod
    )";
    if (incluirB) {
        consulta += R"(
            UNION ALL
            SELECT COALESCE(NULLIF(TRIM(l.cod), ''), TRIM(l.descripcion)) AS clave_prod,
                   COALESCE(NULLIF(TRIM(a.descripcion), ''), TRIM(l.descripcion)) AS desc_prod,
                   SUM(l.cantidad) AS c,
                   SUM(l.totallinea) AS t
            FROM ticketss t
            INNER JOIN lineasticket l ON l.nticket = CONCAT('B', t.ticket)
            LEFT JOIN articulos a ON l.cod = a.cod
            WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta
            GROUP BY clave_prod, desc_prod
        )";
    }

    QStringList conexiones = getListaConexiones();

    struct AcumProd {
        QString descripcion;
        double cantidad = 0.0;
        double total = 0.0;
    };
    QMap<QString, AcumProd> acumulado;

    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                QString clave = query.value(0).toString();
                QString desc = query.value(1).toString();
                if (clave.isEmpty()) clave = desc;
                if (acumulado[clave].descripcion.isEmpty()) {
                    acumulado[clave].descripcion = desc;
                }
                acumulado[clave].cantidad += query.value(2).toDouble();
                acumulado[clave].total += query.value(3).toDouble();
            }
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar ventas de productos desde lineasticket_nube
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("t.id_tienda IN (1, 2, 3) AND t.id_tienda != %1").arg(idTiendaLocal)
                                         : QString("t.id_tienda IN (1, 2, 3)");

            QString sqlNube = QString(R"(
                SELECT COALESCE(NULLIF(TRIM(l.cod), ''), TRIM(l.descripcion)) AS clave_prod,
                       COALESCE(NULLIF(TRIM(a.descripcion), ''), TRIM(l.descripcion)) AS desc_prod,
                       SUM(l.cantidad) AS c,
                       SUM(l.totallinea) AS t
                FROM tickets_nube t
                INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CAST(t.ticket AS CHAR)
                LEFT JOIN articulos a ON l.cod = a.cod
                WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta AND %1
                GROUP BY clave_prod, desc_prod
            )").arg(condTiendaNube);

            if (incluirB) {
                sqlNube += QString(R"(
                    UNION ALL
                    SELECT COALESCE(NULLIF(TRIM(l.cod), ''), TRIM(l.descripcion)) AS clave_prod,
                           COALESCE(NULLIF(TRIM(a.descripcion), ''), TRIM(l.descripcion)) AS desc_prod,
                           SUM(l.cantidad) AS c,
                           SUM(l.totallinea) AS t
                    FROM ticketss_nube t
                    INNER JOIN lineasticket_nube l ON l.id_tienda = t.id_tienda AND l.nticket = CONCAT('B', t.ticket)
                    LEFT JOIN articulos a ON l.cod = a.cod
                    WHERE t.usuario = :usuario AND t.fecha BETWEEN :desde AND :hasta AND %1
                    GROUP BY clave_prod, desc_prod
                )").arg(condTiendaNube);
            }

            QSqlQuery qNube(dbNube);
            qNube.prepare(sqlNube);
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    QString clave = qNube.value(0).toString();
                    QString desc = qNube.value(1).toString();
                    if (clave.isEmpty()) clave = desc;
                    if (acumulado[clave].descripcion.isEmpty()) {
                        acumulado[clave].descripcion = desc;
                    }
                    acumulado[clave].cantidad += qNube.value(2).toDouble();
                    acumulado[clave].total += qNube.value(3).toDouble();
                }
            }
        }
    }

    QStandardItemModel *modelo = new QStandardItemModel(this);
    QList<QString> claves = acumulado.keys();
    std::sort(claves.begin(), claves.end(), [&](const QString &a, const QString &b) {
        return acumulado[a].cantidad > acumulado[b].cantidad;
    });

    int row = 0;
    for (const QString &k : claves.mid(0, 500)) {
        QStandardItem *itemProd = new QStandardItem(acumulado[k].descripcion);
        itemProd->setToolTip(k);
        modelo->setItem(row, 0, itemProd);

        QStandardItem *itemCant = new QStandardItem();
        itemCant->setData(acumulado[k].cantidad, Qt::EditRole);
        modelo->setItem(row, 1, itemCant);

        QStandardItem *itemTotal = new QStandardItem();
        itemTotal->setData(acumulado[k].total, Qt::EditRole);
        modelo->setItem(row, 2, itemTotal);

        row++;
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Producto");
    modelo->setHeaderData(1, Qt::Horizontal, "Cantidad");
    modelo->setHeaderData(2, Qt::Horizontal, "Total");

    // Desactivar ordenación antes de asignar el modelo y ordenar por Cantidad (columna 1) descendente
    ui->tableProductos->setSortingEnabled(false);
    ui->tableProductos->setModel(modelo);
    ui->tableProductos->setSortingEnabled(true);
    ui->tableProductos->sortByColumn(1, Qt::DescendingOrder);
    ui->tableProductos->resizeColumnsToContents();
}

QStringList ventasUsuarioWidget::getListaConexiones()
{
    QStringList conexiones;
    int idTiendaLocal = conf ? conf->getIdTienda() : 1;

    // Si estamos en modo global con Nube activa y la BD local no es una de las 3 tiendas reales (p.ej. id=4 de pruebas),
    // consultar únicamente la Nube para no mezclar tickets de prueba locales.
    if (ui->chkGlobal->isChecked()) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            if (idTiendaLocal >= 1 && idTiendaLocal <= 3) {
                conexiones << conf->getConexionLocal();
            }
            return conexiones;
        }
    }

    conexiones << conf->getConexionLocal(); // "DB"

    // Si se activó ver todas las tiendas pero la Nube NO está disponible, usar conexiones remotas directas como fallback
    if (ui->chkGlobal->isChecked()) {
        QString nombreTiendaLocal;
        QSqlQuery qT(QSqlDatabase::database(conf->getConexionLocal()));
        if (qT.exec("SELECT nombre FROM tiendas WHERE local = 1") && qT.next()) {
            nombreTiendaLocal = qT.value(0).toString();
        }

        QStringList activas = conf->getNombreConexionesActivas();
        for (const QString &c : activas) {
            if (c != nombreTiendaLocal && c != "DB") {
                conexiones << c;
            }
        }
    }
    conexiones.removeDuplicates();
    return conexiones;
}

void ventasUsuarioWidget::actualizarEstadisticas(const QDate &desde, const QDate &hasta, QString &usuario)
{
    bool incluirB = !m_primeraCarga && !ui->tableResumen->isColumnHidden(2);

    QString consulta = R"(
        SELECT COUNT(*), COALESCE(SUM(total), 0) FROM tickets WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
    )";
    if (incluirB) {
        consulta += R"(
            UNION ALL
            SELECT COUNT(*), COALESCE(SUM(total), 0) FROM ticketss WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        )";
    }

    QStringList conexiones = getListaConexiones();
    double totalVentas = 0;
    int totalTickets = 0;

    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde.toString("yyyy-MM-dd"));
        query.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
        if (query.exec()) {
            while (query.next()) {
                totalTickets += query.value(0).toInt();
                totalVentas += query.value(1).toDouble();
            }
        }
    }

    // Si se activó modo global y la Nube está disponible, agregar estadísticas desde tickets_nube (y ticketss_nube si B está visible)
    if (ui->chkGlobal->isChecked() && conexiones.size() <= 1) {
        QSqlDatabase dbNube = base.obtenerConexionNube();
        if (dbNube.isValid() && dbNube.isOpen()) {
            int idTiendaLocal = conf ? conf->getIdTienda() : 1;
            QString condTiendaNube = (idTiendaLocal >= 1 && idTiendaLocal <= 3)
                                         ? QString("id_tienda IN (1, 2, 3) AND id_tienda != %1").arg(idTiendaLocal)
                                         : QString("id_tienda IN (1, 2, 3)");
            QString sqlNube = QString("SELECT COUNT(*), COALESCE(SUM(total), 0) FROM tickets_nube "
                                      "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1")
                                  .arg(condTiendaNube);
            if (incluirB) {
                sqlNube += QString(" UNION ALL "
                                   "SELECT COUNT(*), COALESCE(SUM(total), 0) FROM ticketss_nube "
                                   "WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta AND %1")
                               .arg(condTiendaNube);
            }
            QSqlQuery qNube(dbNube);
            qNube.prepare(sqlNube);
            qNube.bindValue(":usuario", usuario);
            qNube.bindValue(":desde", desde.toString("yyyy-MM-dd"));
            qNube.bindValue(":hasta", hasta.toString("yyyy-MM-dd"));
            if (qNube.exec()) {
                while (qNube.next()) {
                    totalTickets += qNube.value(0).toInt();
                    totalVentas += qNube.value(1).toDouble();
                }
            }
        }
    }

    double ticketMedio = (totalTickets > 0) ? (totalVentas / totalTickets) : 0;
    
    // Calcular media de tickets por día en el rango
    int dias = desde.daysTo(hasta) + 1;
    double ticketsPorDia = (dias > 0) ? (static_cast<double>(totalTickets) / dias) : 0;

    ui->lblStats->setText(QString("<font color='#1976D2'>Total Ventas:</font> <b>%1 €</b> | "
                                  "<font color='#1976D2'>Tickets:</font> <b>%2</b> | "
                                  "<font color='#1976D2'>Ticket Medio:</font> <b>%3 €</b> | "
                                  "<font color='#1976D2'>Media/Día:</font> <b>%4</b>")
                          .arg(QString::number(totalVentas, 'f', 2))
                          .arg(totalTickets)
                          .arg(QString::number(ticketMedio, 'f', 2))
                          .arg(QString::number(ticketsPorDia, 'f', 2)));
}

void ventasUsuarioWidget::generarGraficoDesdeTabla(QTableView *table, QLayout *layout, const QString &titulo)
{
    QAbstractItemModel *modelo = table->model();
    if (!modelo)
        return;
    
    int filas = modelo->rowCount();
    bool columna2Visible = !table->isColumnHidden(2);

    QBarSet *set1 = new QBarSet("Ventas A");
    QBarSet *set2 = columna2Visible ? new QBarSet("Ventas B") : nullptr;
    
    set1->setColor(QColor("#2196F3")); // Azul Moderno
    if (set2)
        set2->setColor(QColor("#FF9800")); // Naranja Moderno
    
    connect(set1, &QBarSet::hovered, this, &ventasUsuarioWidget::mostrarTooltip);
    if (set2)
        connect(set2, &QBarSet::hovered, this, &ventasUsuarioWidget::mostrarTooltip);

    QStringList labels;
    for (int i = 0; i < filas; ++i) {
        labels << modelo->data(modelo->index(i, 0)).toString();
        *set1 << modelo->data(modelo->index(i, 1)).toDouble();
        if (set2) {
            *set2 << modelo->data(modelo->index(i, 2)).toDouble();
        }
    }

    QStackedBarSeries *series = new QStackedBarSeries();
    series->append(set1);
    if (set2)
        series->append(set2);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle(titulo);
    chart->setBackgroundVisible(false); // Transparente para usar el fondo del widget
    
    // Mejorar tipografía del título
    QFont titleFont = chart->titleFont();
    titleFont.setBold(true);
    titleFont.setPointSize(11);
    titleFont.setFamily("Segoe UI");
    chart->setTitleFont(titleFont);

    chart->legend()->setAlignment(Qt::AlignBottom);
    chart->legend()->setVisible(true);
    chart->legend()->setMarkerShape(QLegend::MarkerShapeCircle);
    chart->setAnimationOptions(QChart::AllAnimations);
    chart->setMargins(QMargins(0, 0, 0, 0));

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(labels);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setLabelFormat("%.2f €");
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);
    chartView->setMinimumSize(400, 300); // Asegurar tamaño mínimo práctico

    QLayoutItem *child;
    while ((child = layout->takeAt(0)) != nullptr) {
        if (child->widget()) delete child->widget();
        delete child;
    }
    layout->addWidget(chartView);
}

void ventasUsuarioWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2) {
        bool nuevoOculto = !ui->tableResumen->isColumnHidden(2);
        ui->tableResumen->setColumnHidden(2, nuevoOculto);
        ui->tableResumen->setColumnHidden(3, nuevoOculto);
        ui->tableHoras->setColumnHidden(2, nuevoOculto);
        ui->tableHoras->setColumnHidden(3, nuevoOculto);
        ui->tableSemana->setColumnHidden(2, nuevoOculto);
        ui->tableSemana->setColumnHidden(3, nuevoOculto);
        actualizarResumen();
    }
}

void ventasUsuarioWidget::mostrarTooltip(bool estado, int index)
{
    QBarSet *set = qobject_cast<QBarSet *>(sender());
    if (!set || !estado) {
        QToolTip::hideText();
        return;
    }

    // ➕ Calcular el acumulado si hay más sets en la serie

    // Buscar la serie que contiene este set
    QBarSet *senderSet = qobject_cast<QBarSet *>(sender());
    if (!senderSet)
        return;

    if (estado) {
        QString texto = QString("%1: %2").arg(senderSet->label()).arg(senderSet->at(index));
        QToolTip::showText(QCursor::pos(), texto);
    } else {
        QToolTip::hideText();
    }
}
