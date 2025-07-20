#include "ventasusuariowidget.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "ui_ventasusuariowidget.h"
#include <QSqlQueryModel>
#include <QDebug>
#include <QtCharts/QChartView>
#include <QtCharts/QChart>
#include <QtCharts/QStackedBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>
#include <QToolTip>

using namespace QtCharts;

ventasUsuarioWidget::ventasUsuarioWidget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ventasUsuarioWidget)
{
    ui->setupUi(this);
    // Constructor de ResumenVentasWidget

    ui->dateDesde->setDate(QDate(2025,01,01));
    ui->dateHasta->setDate(QDate::currentDate());
    connect(ui->radioDia, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->radioMes, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->radioAnio, &QRadioButton::toggled, this, &ventasUsuarioWidget::actualizarResumen);

    connect(ui->dateDesde, &QDateEdit::dateChanged, this, &ventasUsuarioWidget::actualizarResumen);
    connect(ui->dateHasta, &QDateEdit::dateChanged, this, &ventasUsuarioWidget::actualizarResumen);
    usuario = "1";
    this->setFocusPolicy(Qt::StrongFocus);
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

        if (ui->radioDia->isChecked()) agrupacion = "dia";
        else if (ui->radioMes->isChecked()) agrupacion = "mes";
        else agrupacion = "anio";

        QDate desde = ui->dateDesde->date();
        QDate hasta = ui->dateHasta->date();

        // Aquí haces la consulta real con el criterio
        // por ejemplo:
        qDebug() << "Usuario=: " << usuario;

        actualizarTablaVentas(agrupacion, desde, hasta , usuario);


}

void ventasUsuarioWidget::actualizarTablaVentas(const QString &agrupacion, const QDate &desde, const QDate &hasta,QString &usuario)
{
    QString formatoFechaSQL;

    // Selecciona el formato SQL según el tipo de agrupación
    if (agrupacion == "dia")
        formatoFechaSQL = "DATE(fecha)";  // Ej: 2024-05-21
    else if (agrupacion == "mes")
        formatoFechaSQL = "DATE_FORMAT(fecha, '%Y-%m')";  // Ej: 2024-05
    else if (agrupacion == "anio")
        formatoFechaSQL = "YEAR(fecha)";  // Ej: 2024

    QSqlQueryModel *modelo = new QSqlQueryModel(this);

    QString consulta = QString(R"(

SELECT
  %1 AS mes,
SUM(ventas.total_tickets) AS ventas_tabla_tickets,
  SUM(ventas.total_ticketss) AS ventas_tabla_ticketss,
  SUM(ventas.total_tickets + ventas.total_ticketss) AS total_mensual
FROM (
  -- Seleccionar ventas de la primera tabla
  SELECT
    usuario,
    fecha,
    total AS total_tickets,
    0 AS total_ticketss
  FROM tickets
  UNION ALL
  -- Seleccionar ventas de la segunda tabla
  SELECT
    usuario,
    fecha,
    0 AS total_tickets,
    total AS total_ticketss
  FROM ticketss
) AS ventas
JOIN usuarios AS u
  ON ventas.usuario = u.id
WHERE
  u.id = :usuario  -- <-- Cambia este ID por el del usuario que quieras consultar
  and fecha between :desde and :hasta
GROUP BY
  mes,
  u.nombre
ORDER BY
  mes DESC;
    )").arg(formatoFechaSQL);

    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal())); // Asegúrate de que "DB" sea tu conexión
    query.prepare(consulta);
    query.bindValue(":usuario", usuario);
    query.bindValue(":desde", desde);
    query.bindValue(":hasta", hasta);
    if (!query.exec()) {
        qDebug() << "❌ Error al ejecutar consulta de ventas:" << query.lastError().text();
        return;
    }
    modelo->setQuery(query);
    modelo->setHeaderData(0,Qt::Horizontal, "Periodo");
    modelo->setHeaderData(1,Qt::Horizontal, "Ventas");
    modelo->setHeaderData(2,Qt::Horizontal, "B");
    modelo->setHeaderData(3,Qt::Horizontal, "Total");

    ui->tableResumen->setModel(modelo);
    ui->tableResumen->hideColumn(3);
    ui->tableResumen->hideColumn(2);
    ui->tableResumen->resizeColumnsToContents();
    generarGraficoDesdeTabla();
}

void ventasUsuarioWidget::generarGraficoDesdeTabla()
{
    QSqlQueryModel *modelo = qobject_cast<QSqlQueryModel*>(ui->tableResumen->model());
    if (!modelo) return;

    int filas = modelo->rowCount();

    bool columna2Visible = !ui->tableResumen->isColumnHidden(2);

    QBarSet *set1 = new QBarSet("Ventas");
    QBarSet *set2 = columna2Visible ? new QBarSet("B") : nullptr;
    connect(set1, &QBarSet::hovered, this, &ventasUsuarioWidget::mostrarTooltip);
    if (set2)
        connect(set2, &QBarSet::hovered, this, &ventasUsuarioWidget::mostrarTooltip);
    QString formatoFecha;
    QString granularidad;
    QString muestra = modelo->data(modelo->index(0,0)).toString();
    if (muestra.contains("-")) {
        if(muestra.size() == 7){formatoFecha = "yyyy-MM"; granularidad = "mes"; }
        else if(muestra.size() == 10){formatoFecha = "yyyy-MM-dd"; granularidad = "dia"; }
    }else {
        formatoFecha = "yyyy"; granularidad = "anio";
    }

    QDate fechaMin = QDate::fromString(modelo->data(modelo->index(0,0)).toString(), formatoFecha);
    QDate fechaMax = QDate::fromString(modelo->data(modelo->index(modelo->rowCount()-1,0)).toString(),formatoFecha);
    if (fechaMin > fechaMax) {
        std::swap(fechaMin, fechaMax);
    }
    QMap<QString, double> mapaSerie1;
    QMap<QString, double> mapaSerie2;
    QStringList fechas;


    for (int i = filas-1; i > -1; --i) {
        QString fecha = modelo->data(modelo->index(i,0)).toString();
        double cantidad = modelo->data(modelo->index(i,1)).toDouble();
        if(set2){
            double cantidadB = modelo->data(modelo->index(i,2)).toDouble();
            mapaSerie2[fecha] +=cantidadB;
        }
        mapaSerie1[fecha] += cantidad; // += suma los datos si lla hay un registro con esa fecha
    }
    QDate actual = fechaMin;

    while (actual <= fechaMax) {
        QString clave = actual.toString(formatoFecha);
        fechas << clave;
        double cantidad = mapaSerie1.value(clave,0);
        *set1 << cantidad ;
        if(set2){
            double cantidadB = mapaSerie2.value(clave,0);
            *set2 << cantidadB;

        }
        if (granularidad == "dia") actual = actual.addDays(1);
        else if (granularidad == "mes") actual = actual.addMonths(1);
        else if (granularidad == "anio") actual = actual.addYears(1);
    }


    QStackedBarSeries *series = new QStackedBarSeries();
    series->append(set1);
    if (set2) series->append(set2);

    set1->setColor(QColor("#6699cc"));
    if (set2) set2->setColor(QColor("#cc9966"));

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Resumen de ventas");
    chart->legend()->setAlignment(Qt::AlignBottom);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(fechas);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);




    QLayoutItem *child;
    while ((child = ui->fechasLayout->takeAt(0)) != nullptr) {
        delete child->widget();
        delete child;
    }
    ui->fechasLayout->addWidget(chartView);
}

void ventasUsuarioWidget::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_F2){
        bool visible = !ui->tableResumen->isColumnHidden(2);
        ui->tableResumen->setColumnHidden(2,visible);
        ui->tableResumen->setColumnHidden(3,visible);
        generarGraficoDesdeTabla();
        qDebug() << "Press event funciona";

    }
}

void ventasUsuarioWidget::mostrarTooltip(bool estado, int index)
{

    QBarSet *set = qobject_cast<QBarSet *>(sender());
    if (!set || !estado)
    {
        QToolTip::hideText();
        return;
    }


    // ➕ Calcular el acumulado si hay más sets en la serie

    // Buscar la serie que contiene este set
    QBarSet *senderSet = qobject_cast<QBarSet *>(sender());
    if (!senderSet)
        return;

    if (estado) {
        QString texto = QString("%1: %2")
        .arg(senderSet->label())
            .arg(senderSet->at(index));
        QToolTip::showText(QCursor::pos(), texto);
    } else {
        QToolTip::hideText();
    }

}




