#include "graficoventaswidget.h"

GraficoVentasWidget::GraficoVentasWidget(QWidget *parent)
    : QWidget(parent)
{

    chart = new QChart();
    chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
    setLayout(layout);
}

void GraficoVentasWidget::configurar(const QString &titulo,
                                     const QStringList &categorias,
                                     const QList<QList<double>> &series,
                                     const QStringList &nombresSeries,
                                     const QList<QColor> &colores, const QString tipo)
{
    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());
    chart->setTitle(titulo);
    if (!chart || categorias.isEmpty() || series.isEmpty())
        return;
    int puntos = categorias.size();
    int numSeries = series.size();



    if(tipo != "barras"){
    QCategoryAxis *axisX = new QCategoryAxis;
    QValueAxis *axisY = new QValueAxis;

    chart->addAxis(axisX, Qt::AlignBottom);
    chart->addAxis(axisY, Qt::AlignLeft);

    for (int i = 0; i < numSeries; ++i) {
        if (series[i].size() != puntos)
            continue;

        QLineSeries *line = new QLineSeries();
        line->setName(nombresSeries.value(i, QString("Serie %1").arg(i)));

        for (int j = 0; j < puntos; ++j) {
            line->append(j, series[i][j]);
        }

        if (i < colores.size())
            line->setColor(colores[i]);

        chart->addSeries(line);
        line->attachAxis(axisX);
        line->attachAxis(axisY);
    }

    for (int j = 0; j < puntos; ++j) {
        axisX->append(categorias[j], j);
    }

    axisX->setLabelsAngle(-60);
    axisX->setTickCount(qMin(puntos, 10));
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
    return;
    }
    QBarSeries *barSeries = new QBarSeries();

    // Cada lista de valores será un QBarSet (una serie agrupada)
    for (int i = 0; i < series.size(); ++i) {
        QBarSet *barSet = new QBarSet(nombresSeries.value(i, QString("Serie %1").arg(i)));
        for (double valor : series[i]) {
            *barSet << valor;  // Añade los valores secuencialmente
        }
        if (i < colores.size())
            barSet->setColor(colores[i]);

        barSeries->append(barSet);
    }

    chart->addSeries(barSeries);

    // Eje X por categorías
    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categorias);
    chart->addAxis(axisX, Qt::AlignBottom);
    barSeries->attachAxis(axisX);

    // Eje Y automático
    QValueAxis *axisY = new QValueAxis();
    chart->addAxis(axisY, Qt::AlignLeft);
    barSeries->attachAxis(axisY);

    // Opciones visuales
    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);
}
