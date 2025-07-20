#ifndef VENTASUSUARIOWIDGET_H
#define VENTASUSUARIOWIDGET_H

#include <QWidget>
#include <QRadioButton>
#include <QtCharts/QBarSet>

#include "configuracion.h"

extern Configuracion *conf;


namespace Ui {
class ventasUsuarioWidget;
}

class ventasUsuarioWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ventasUsuarioWidget(QWidget *parent = nullptr);
    ~ventasUsuarioWidget();
    void setUser(QString usuarioActual);

private:
    Ui::ventasUsuarioWidget *ui;
    void actualizarResumen();
    void actualizarTablaVentas(const QString &agrupacion, const QDate &desde, const QDate &hasta, QString &usuario);
    void generarGraficoDesdeTabla();
    void keyPressEvent(QKeyEvent *event);
    QString usuario;

private slots:
    void mostrarTooltip(bool estado, int index);
};

#endif // VENTASUSUARIOWIDGET_H
