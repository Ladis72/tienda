#ifndef ETIQUETAS_H
#define ETIQUETAS_H

#include <QDialog>
#include <QStandardItemModel>
#include "base_datos.h"
#include "buscarproducto.h"
#include "configuracion.h"
#include "formateo.h"

extern Configuracion *conf;

namespace Ui {
class Etiquetas;
}

class Etiquetas : public QDialog
{
    Q_OBJECT

public:
    explicit Etiquetas(QWidget *parent = 0);
    ~Etiquetas();

private slots:
    void on_pushButtonImprimir_clicked();

    void on_lineEditCod_returnPressed();

    void on_lineEditDesc_returnPressed();

    void on_tableView_clicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void generarPDF(const QList<QList<QString> > datos);
    void abriPDF(const QString &ruta);

private:
    Ui::Etiquetas *ui;
    QStandardItemModel *modelo;
    baseDatos *base;
    BuscarProducto *buscarProducto;
    QString articuloSeleccionado;
    Formateo classFormatear;

    void llenarModelo();
};

#endif // ETIQUETAS_H
