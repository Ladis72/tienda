#ifndef BUSCARPORNOTAS_H
#define BUSCARPORNOTAS_H

#include <QDialog>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
#include <QSqlRecord>
#include <QRegularExpression>
#include <utility>
#include "base_datos.h"

// Clase ProxyNotas para filtrado interactivo e inteligente multitérmino en la tabla de productos
class ProxyNotas : public QSortFilterProxyModel {
    Q_OBJECT
public:
    bool soloConStock = false;
    QString textoFiltro;

    // Actualiza el patrón de búsqueda multitérmino
    void setTextoFiltro(const QString &texto) {
        textoFiltro = texto.trimmed();
        invalidate();
    }

protected:
    // Filtra cada fila verificando que cada palabra clave coincida en código, descripción o notas
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override {
        Q_UNUSED(source_parent);
        QSqlQueryModel *sModel = qobject_cast<QSqlQueryModel*>(sourceModel());
        if (!sModel) return true;

        QSqlRecord record = sModel->record(source_row);

        // Filtrado por stock si está activo el checkbox
        if (soloConStock) {
            double stock = record.value("stock_total").toDouble();
            if (stock <= 0) return false;
        }

        // Filtrado multitérmino por palabras clave
        if (!textoFiltro.isEmpty()) {
            QString cod = record.value("cod").toString();
            QString descripcion = record.value("descripcion").toString();
            QString notas = record.value("notas").toString();

            QStringList palabras = textoFiltro.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
            for (const QString &palabra : palabras) {
                bool coincide = cod.contains(palabra, Qt::CaseInsensitive) ||
                                descripcion.contains(palabra, Qt::CaseInsensitive) ||
                                notas.contains(palabra, Qt::CaseInsensitive);
                if (!coincide) {
                    return false;
                }
            }
        }

        return true;
    }

public:
    void actualizar() {
        invalidate();
    }
};

namespace Ui {
class BuscarPorNotas;
}

// Diálogo independiente para la búsqueda inteligente de productos por notas y descripción
class BuscarPorNotas : public QDialog
{
    Q_OBJECT

public:
    explicit BuscarPorNotas(QWidget *parent, QSqlQuery query);
    ~BuscarPorNotas();
    void reject() override;
    QString resultado; // Código del producto seleccionado

private slots:
    void on_tableView_activated(const QModelIndex &index);
    void on_lineEdit_buscar_textChanged(const QString &arg1);
    void on_checkBox_conStock_stateChanged(int arg1);
    void selectionChanged(const QModelIndex &current, const QModelIndex &previous);
    void on_pushButtonCancelar_clicked();
    void on_pushButtonAceptar_clicked();

private:
    Ui::BuscarPorNotas *ui;
    QSqlQuery query;
    QSqlQueryModel modelo;
    ProxyNotas proxyModel;
    baseDatos base;

    void filtrar();
    void mostrarDetalles(const QModelIndex &index);
};

#endif // BUSCARPORNOTAS_H
