#ifndef VENTASUSUARIOWIDGET_H
#define VENTASUSUARIOWIDGET_H

#include <QRadioButton>
#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QLayout>
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
    void actualizarFechas(const QString &agrupacion,
                          const QDate &desde,
                          const QDate &hasta,
                          QString &usuario);
    void actualizarHoras(const QDate &desde, const QDate &hasta, QString &usuario);
    void actualizarSemana(const QDate &desde, const QDate &hasta, QString &usuario);
    void actualizarTickets(const QDate &desde, const QDate &hasta, QString &usuario);
    void actualizarProductos(const QDate &desde, const QDate &hasta, QString &usuario);
    QStringList getListaConexiones();
    void actualizarEstadisticas(const QDate &desde, const QDate &hasta, QString &usuario);
    void generarGraficoDesdeTabla(QTableView *table, QLayout *layout, const QString &titulo);
    void keyPressEvent(QKeyEvent *event);
    QString usuario;
    bool m_primeraCarga;

private slots:
    void mostrarTooltip(bool estado, int index);
};

#endif // VENTASUSUARIOWIDGET_H
