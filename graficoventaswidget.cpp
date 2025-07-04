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
                                     const QList<QColor> &colores)
{
    if (!chart || categorias.isEmpty() || series.isEmpty())
        return;
    int puntos = categorias.size();
    int numSeries = series.size();

    chart->removeAllSeries();
    chart->removeAxis(chart->axisX());
    chart->removeAxis(chart->axisY());
    chart->setTitle(titulo);

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
}
