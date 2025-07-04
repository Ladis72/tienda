#ifndef GRAFICOVENTASWIDGET_H
#define GRAFICOVENTASWIDGET_H

#include <QWidget>
#include <QtCharts>

QT_CHARTS_USE_NAMESPACE

    class GraficoVentasWidget : public QWidget
{
    Q_OBJECT

public:
    explicit GraficoVentasWidget(QWidget *parent = nullptr);
    void configurar(const QString &titulo,
                    const QStringList &categorias,
                    const QList<QList<double>> &series,
                    const QStringList &nombresSeries,
                    const QList<QColor> &colores);

private:
    QChart *chart;
    QChartView *chartView;
};

#endif // GRAFICOVENTASWIDGET_H

