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
}

void ventasUsuarioWidget::actualizarFechas(const QString &agrupacion,
                                         const QDate &desde,
                                         const QDate &hasta,
                                         QString &usuario)
{
    QString formatoFechaSQL;

    // Selecciona el formato SQL según el tipo de agrupación
    if (agrupacion == "dia")
        formatoFechaSQL = "DATE(fecha)"; // Ej: 2024-05-21
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
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);

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
 
    ui->tableResumen->setModel(modelo);
    
    // Restaurar visibilidad (o esconder por defecto si es la primera vez)
    ui->tableResumen->setColumnHidden(2, col2Hidden);
    ui->tableResumen->setColumnHidden(3, col3Hidden);
    
    ui->tableResumen->resizeColumnsToContents();
    generarGraficoDesdeTabla(ui->tableResumen, ui->fechasLayout, "Resumen de ventas");
    actualizarEstadisticas(desde, hasta, usuario);
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
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);
        if (query.exec()) {
            while (query.next()) {
                int h = query.value(0).toInt();
                if (!acumulado.contains(h)) acumulado[h] = {0, 0};
                acumulado[h][0] += query.value(1).toDouble();
                acumulado[h][1] += query.value(2).toDouble();
            }
        }
    }

    QStandardItemModel *modelo = new QStandardItemModel(this);
    QList<int> horas = acumulado.keys();
    std::sort(horas.begin(), horas.end());

    int row = 0;
    for (int h : horas) {
        modelo->setItem(row, 0, new QStandardItem(QString("%1:00").arg(h, 2, 10, QChar('0'))));
        modelo->setItem(row, 1, new QStandardItem(QString::number(acumulado[h][0], 'f', 2)));
        modelo->setItem(row, 2, new QStandardItem(QString::number(acumulado[h][1], 'f', 2)));
        modelo->setItem(row, 3, new QStandardItem(QString::number(acumulado[h][0] + acumulado[h][1], 'f', 2)));
        row++;
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Hora");
    modelo->setHeaderData(1, Qt::Horizontal, "Ventas");
    modelo->setHeaderData(2, Qt::Horizontal, "B");
    modelo->setHeaderData(3, Qt::Horizontal, "Total");
    
    bool col2Hidden = m_primeraCarga ? true : ui->tableHoras->isColumnHidden(2);
    bool col3Hidden = m_primeraCarga ? true : ui->tableHoras->isColumnHidden(3);
    m_primeraCarga = false;

    ui->tableHoras->setModel(modelo);
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
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);
        if (query.exec()) {
            while (query.next()) {
                int d = query.value(0).toInt();
                if (!acumulado.contains(d)) acumulado[d] = {0, 0};
                acumulado[d][0] += query.value(1).toDouble();
                acumulado[d][1] += query.value(2).toDouble();
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

    bool col2Hidden = m_primeraCarga ? true : ui->tableSemana->isColumnHidden(2);
    bool col3Hidden = m_primeraCarga ? true : ui->tableSemana->isColumnHidden(3);
    m_primeraCarga = false;

    ui->tableSemana->setModel(modelo);
    ui->tableSemana->setColumnHidden(2, col2Hidden);
    ui->tableSemana->setColumnHidden(3, col3Hidden);
    ui->tableSemana->resizeColumnsToContents();
    generarGraficoDesdeTabla(ui->tableSemana, ui->chartLayoutSemana, "Ventas por Día de Semana");
}

void ventasUsuarioWidget::actualizarTickets(const QDate &desde, const QDate &hasta, QString &usuario)
{
    QString consulta = R"(
        SELECT ticket, fecha, hora, total FROM tickets 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        UNION ALL
        SELECT ticket, fecha, hora, total FROM ticketss 
        WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        ORDER BY fecha DESC, hora DESC LIMIT 200
    )";
    QStringList conexiones = getListaConexiones();

    QStandardItemModel *modelo = new QStandardItemModel(this);
    int row = 0;
    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);
        if (query.exec()) {
            while (query.next()) {
                modelo->setItem(row, 0, new QStandardItem(conn));
                
                QStandardItem *itemTicket = new QStandardItem();
                itemTicket->setData(query.value(0).toInt(), Qt::EditRole);
                modelo->setItem(row, 1, itemTicket);
                
                modelo->setItem(row, 2, new QStandardItem(query.value(1).toDate().toString("yyyy-MM-dd")));
                modelo->setItem(row, 3, new QStandardItem(query.value(2).toTime().toString("HH:mm")));
                
                QStandardItem *itemTotal = new QStandardItem();
                itemTotal->setData(query.value(3).toDouble(), Qt::EditRole);
                modelo->setItem(row, 4, itemTotal);
                
                row++;
            }
        }
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Tienda");
    modelo->setHeaderData(1, Qt::Horizontal, "Nº");
    modelo->setHeaderData(2, Qt::Horizontal, "Fecha");
    modelo->setHeaderData(3, Qt::Horizontal, "Hora");
    modelo->setHeaderData(4, Qt::Horizontal, "Total");
    ui->tableTickets->setModel(modelo);
    ui->tableTickets->resizeColumnsToContents();
}

void ventasUsuarioWidget::actualizarProductos(const QDate &desde, const QDate &hasta, QString &usuario)
{
    QString consulta = R"(
        SELECT descripcion, SUM(cantidad) as c, SUM(totallinea) as t FROM lineasticket 
        WHERE fecha BETWEEN :desde AND :hasta 
        AND nticket IN (SELECT ticket FROM tickets WHERE usuario = :usuario)
        GROUP BY descripcion ORDER BY c DESC LIMIT 100
    )";
    QStringList conexiones = getListaConexiones();

    QMap<QString, QVector<double>> acumulado;
    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);
        if (query.exec()) {
            while (query.next()) {
                QString d = query.value(0).toString();
                if (!acumulado.contains(d)) acumulado[d] = {0, 0};
                acumulado[d][0] += query.value(1).toDouble();
                acumulado[d][1] += query.value(2).toDouble();
            }
        }
    }

    QStandardItemModel *modelo = new QStandardItemModel(this);
    QList<QString> prods = acumulado.keys();
    std::sort(prods.begin(), prods.end(), [&](const QString &a, const QString &b){
        return acumulado[a][0] > acumulado[b][0];
    });

    int row = 0;
    for (const QString &p : prods.mid(0, 100)) {
        modelo->setItem(row, 0, new QStandardItem(p));
        
        QStandardItem *itemCant = new QStandardItem();
        itemCant->setData(acumulado[p][0], Qt::EditRole);
        modelo->setItem(row, 1, itemCant);
        
        QStandardItem *itemTotal = new QStandardItem();
        itemTotal->setData(acumulado[p][1], Qt::EditRole);
        modelo->setItem(row, 2, itemTotal);
        
        row++;
    }
    modelo->setHeaderData(0, Qt::Horizontal, "Producto");
    modelo->setHeaderData(1, Qt::Horizontal, "Cantidad");
    modelo->setHeaderData(2, Qt::Horizontal, "Total");
    ui->tableProductos->setModel(modelo);
    ui->tableProductos->resizeColumnsToContents();
}

QStringList ventasUsuarioWidget::getListaConexiones()
{
    QStringList conexiones;
    conexiones << conf->getConexionLocal(); // "DB"

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
    QString consulta = R"(
        SELECT COUNT(*), SUM(total) FROM tickets WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
        UNION ALL
        SELECT COUNT(*), SUM(total) FROM ticketss WHERE usuario = :usuario AND fecha BETWEEN :desde AND :hasta
    )";

    QStringList conexiones = getListaConexiones();
    double totalVentas = 0;
    int totalTickets = 0;

    for (const QString &conn : conexiones) {
        QSqlDatabase db = QSqlDatabase::database(conn);
        if (!db.isOpen()) continue;
        QSqlQuery query(db);
        query.prepare(consulta);
        query.bindValue(":usuario", usuario);
        query.bindValue(":desde", desde);
        query.bindValue(":hasta", hasta);
        if (query.exec()) {
            while (query.next()) {
                totalTickets += query.value(0).toInt();
                totalVentas += query.value(1).toDouble();
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
        QTableView *table = nullptr;
        QLayout *layout = nullptr;
        QString titulo;

        switch(ui->tabWidge->currentIndex()) {
            case 0: table = ui->tableResumen; layout = ui->fechasLayout; titulo = "Resumen de ventas"; break;
            case 1: table = ui->tableHoras; layout = ui->chartLayoutHoras; titulo = "Ventas por Hora"; break;
            case 2: table = ui->tableSemana; layout = ui->chartLayoutSemana; titulo = "Ventas por Día de Semana"; break;
        }

        if (table && layout) {
            bool currentlyHidden = table->isColumnHidden(2);
            table->setColumnHidden(2, !currentlyHidden);
            table->setColumnHidden(3, !currentlyHidden);
            generarGraficoDesdeTabla(table, layout, titulo);
        }
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
