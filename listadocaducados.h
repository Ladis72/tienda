#ifndef LISTADOCADUCADOS_H
#define LISTADOCADUCADOS_H

#include <QDialog>
#include <QSortFilterProxyModel>
#include "base_datos.h"

namespace Ui {
class ListadoCaducados;
}

class ListadoCaducados : public QDialog
{
    Q_OBJECT

public:
    explicit ListadoCaducados(QWidget *parent = 0);
    ~ListadoCaducados();

private slots:
    void on_dateEditDesde_userDateChanged(const QDate &date);
    void on_dateEditHasta_userDateChanged(const QDate &date);
    void on_pushButtonImprimir_clicked();
    void on_lineEditBuscar_textChanged(const QString &text);

private:
    Ui::ListadoCaducados *ui;

    void llenarTabla(QString desde, QString hasta);
    QString desde, hasta;
    QSqlQueryModel *mCaducados;
    QSortFilterProxyModel *proxyModel;
    baseDatos *base;
};

#endif // LISTADOCADUCADOS_H
