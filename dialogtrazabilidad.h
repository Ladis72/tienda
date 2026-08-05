/**
 * @file dialogtrazabilidad.h
 * @brief Declaración de la clase DialogTrazabilidad y del modelo ModeloMovimientosTienda.
 * 
 * Permite consultar el historial unificado de compras a proveedores y movimientos entre tiendas 
 * (entradas y salidas ordenadas cronológicamente) con diferenciación cromática.
 */

#ifndef DIALOGTRAZABILIDAD_H
#define DIALOGTRAZABILIDAD_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QColor>
#include <QFont>
#include <QDebug>
#include "configuracion.h"

extern Configuracion *conf;

/**
 * @class ModeloMovimientosTienda
 * @brief Modelo de datos para movimientos entre tiendas (entradas y salidas) con diferenciación cromática.
 * 
 * Aplica código de colores con excelente legibilidad y acorde a la paleta de la aplicación:
 * - ENTRADA: Fondo verde claro (#e8f5e9), texto verde oscuro (#1b5e20).
 * - SALIDA: Fondo naranja claro (#fff3e0), texto naranja/rojo oscuro (#d84315).
 */
class ModeloMovimientosTienda : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModeloMovimientosTienda(QObject *parent = nullptr) : QSqlQueryModel(parent) {}

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
    {
        if (!index.isValid())
            return QVariant();

        // Aplicar color de fondo diferenciado según el tipo de movimiento (columna 0)
        if (role == Qt::BackgroundRole) {
            QString tipo = QSqlQueryModel::data(this->index(index.row(), 0), Qt::DisplayRole).toString();
            if (tipo == "ENTRADA") {
                return QColor("#e8f5e9"); // Verde claro suave
            } else if (tipo == "SALIDA") {
                return QColor("#fff3e0"); // Naranja claro suave
            }
        }
        // Aplicar color de texto de alto contraste para máxima legibilidad
        else if (role == Qt::ForegroundRole) {
            QString tipo = QSqlQueryModel::data(this->index(index.row(), 0), Qt::DisplayRole).toString();
            if (tipo == "ENTRADA") {
                return QColor("#1b5e20"); // Verde oscuro
            } else if (tipo == "SALIDA") {
                return QColor("#d84315"); // Naranja/rojo oscuro
            }
        }
        // Resaltar en negrita la columna del Tipo de Movimiento
        else if (role == Qt::FontRole) {
            if (index.column() == 0) {
                QFont font;
                font.setBold(true);
                return font;
            }
        }

        return QSqlQueryModel::data(index, role);
    }
};

namespace Ui {
class DialogTrazabilidad;
}

class DialogTrazabilidad : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del diálogo de trazabilidad.
     * @param codArticulo Código del artículo a consultar.
     * @param descripcionArticulo Descripción del artículo.
     * @param parent Widget padre.
     */
    explicit DialogTrazabilidad(const QString &codArticulo, const QString &descripcionArticulo, QWidget *parent = nullptr);

    /**
     * @brief Destructor del diálogo.
     */
    ~DialogTrazabilidad();

private:
    Ui::DialogTrazabilidad *ui; //!< Puntero a la interfaz gráfica generada desde Qt Creator

    QSqlQueryModel modeloCompras;          //!< Modelo de datos para compras a proveedores
    ModeloMovimientosTienda modeloMovimientos; //!< Modelo de datos unificado para entradas/salidas entre tiendas

    QString m_codArticulo;         //!< Código del artículo consultado
    QString m_descripcionArticulo; //!< Descripción del artículo consultado

    int totalUdsCompras;   //!< Total unidades compradas a proveedores
    int totalUdsEntradas;  //!< Total unidades recibidas de otras tiendas
    int totalUdsSalidas;   //!< Total unidades enviadas a otras tiendas
    int totalRegEntradas;  //!< Total registros de entradas
    int totalRegSalidas;   //!< Total registros de salidas

    /**
     * @brief Carga las compras realizadas a proveedores.
     */
    void cargarCompras();

    /**
     * @brief Carga y unifica cronológicamente las entradas y salidas de producto con otras tiendas.
     */
    void cargarMovimientosTiendas();

    /**
     * @brief Actualiza la etiqueta del resumen global en la cabecera.
     */
    void actualizarResumenGlobal();
};

#endif // DIALOGTRAZABILIDAD_H
