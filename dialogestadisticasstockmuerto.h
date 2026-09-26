#ifndef DIALOGESTADISTICASSTOCKMUERTO_H
#define DIALOGESTADISTICASSTOCKMUERTO_H

#include "base_datos.h"
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasStockMuerto;
}

/**
 * @brief Diálogo de Detección y Análisis de Stock Muerto, Inactividad y Optimización de Inventario.
 *
 * Permite auditar el capital atrapado en almacén:
 * - Detección de artículos con stock positivo y sin ventas en los últimos N días (30, 60, 90, 180, 365 días).
 * - Identificación de artículos comprados/dados de alta que NUNCA han registrado una venta.
 * - Desglose de capital inmovilizado y riesgo financiero por familia/categoría.
 * - Simulador interactivo de liquidación (cálculo de liquidez recuperable con descuentos de saldo/liquidación).
 *
 * Utiliza consultas directas a la base central de la nube y modelos desacoplados en memoria.
 */
class DialogEstadisticasStockMuerto : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasStockMuerto(QWidget *parent = nullptr);
  ~DialogEstadisticasStockMuerto();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_tabWidgetStockMuerto_currentChanged(int index);
  void on_comboDescuento_currentIndexChanged(int index);

  // Exportaciones
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();

private:
  Ui::DialogEstadisticasStockMuerto *ui;
  baseDatos base;

  QMap<int, bool> pestanasPendientes; ///< Control de lazy-loading para las pestañas

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloStockMuerto;
  QStandardItemModel *modeloNuncaVendidos;
  QStandardItemModel *modeloInmovilizadoFamilias;
  QStandardItemModel *modeloLiquidacion;

  void inicializarFiltros();
  void cargarEstadisticas();
  void cargarPestana(int index);

  void cargarKPIs(int dias, int idFamilia, int idTienda);
  void cargarTablaStockMuerto(int dias, int idFamilia, int limite, int idTienda);
  void cargarTablaNuncaVendidos(int idFamilia, int limite, int idTienda);
  void cargarTablaInmovilizadoFamilias(int dias, int idTienda);
  void cargarTablaSimulador(int dias, int idFamilia, int limite, int idTienda);

  int obtenerDiasSeleccionados() const;
  int obtenerIdTiendaSeleccionada() const;
  int obtenerIdFamiliaSeleccionada() const;
  int obtenerLimiteSeleccionado() const;
};

#endif // DIALOGESTADISTICASSTOCKMUERTO_H
