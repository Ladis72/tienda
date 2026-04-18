#ifndef GENERARVALES_H
#define GENERARVALES_H

#include <QDialog>

#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

#include <QCheckBox>
#include <QGridLayout>

namespace Ui {
class GenerarVales;
}

/**
 * @brief Diálogo para generar los vales de fidelidad mensuales.
 *
 * Suma las ventas de todas las tiendas (local + remotas vía conexión
 * directa) para el mes seleccionado, calcula el importe del vale según
 * el porcentaje de descuento de cada cliente, e inserta los vales en la
 * BD local. El SyncManager se encarga de propagar los vales a la nube
 * y al resto de tiendas.
 */
class GenerarVales : public QDialog
{
    Q_OBJECT

public:
    explicit GenerarVales(QWidget *parent = nullptr);
    ~GenerarVales();

private slots:
    void on_pushButtonGenerar_clicked();

    /// Devuelve el % de descuento asignado a un cliente
    double descuentoCliente(int idCliente);

private:
    Ui::GenerarVales *ui;
    baseDatos base;
    /// Conexiones remotas activas para leer ventas (Opción B)
    QStringList tiendas;
    QCheckBox *simularCheck;
};

#endif // GENERARVALES_H
