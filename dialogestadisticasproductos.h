#ifndef DIALOGESTADISTICASPRODUCTOS_H
#define DIALOGESTADISTICASPRODUCTOS_H

#include "base_datos.h"
#include <QDate>
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasProductos;
}

/**
 * @brief Diálogo de Ranking de Productos, Rentabilidad y Familias.
 *
 * Proporciona un análisis exhaustivo del catálogo en base a datos reales:
 * - Ranking por facturación y volumen de unidades vendidas.
 * - Ranking de rentabilidad neta y margen comercial sobre coste.
 * - Desglose y cuota de ventas y margen por familia/categoría.
 * - Detección de artículos con stock positivo y nula rotación (cero ventas).
 *
 * Todas las operaciones leen directamente de la base de datos central en la nube
 * y emplean modelos en memoria desacoplados (QStandardItemModel) para un rendimiento óptimo.
 */
class DialogEstadisticasProductos : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasProductos(QWidget *parent = nullptr);
  ~DialogEstadisticasProductos();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_tabWidgetProductos_currentChanged(int index);

  // Filtros rápidos de fechas
  void on_btnHoy_clicked();
  void on_btnSemana_clicked();
  void on_btnMes_clicked();
  void on_btnAno_clicked();

  // Exportaciones
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();

private:
  Ui::DialogEstadisticasProductos *ui;
  baseDatos base;

  QMap<int, bool> pestanasPendientes; ///< Control de lazy-loading para las pestañas

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloTopVendidos;
  QStandardItemModel *modeloTopRentables;
  QStandardItemModel *modeloFamilias;
  QStandardItemModel *modeloBajaRotacion;

  void inicializarFiltros();
  void cargarEstadisticas();
  void cargarPestana(int index);

  void cargarKPIs(const QDate &desde, const QDate &hasta, int idFamilia, int idTienda);
  void cargarTablaTopVendidos(const QDate &desde, const QDate &hasta, int idFamilia, int limite, int idTienda);
  void cargarTablaTopRentables(const QDate &desde, const QDate &hasta, int idFamilia, int limite, int idTienda);
  void cargarTablaFamilias(const QDate &desde, const QDate &hasta, int idTienda);
  void cargarTablaBajaRotacion(const QDate &desde, const QDate &hasta, int idFamilia, int limite, int idTienda);

  int obtenerIdTiendaSeleccionada() const;
  int obtenerIdFamiliaSeleccionada() const;
  int obtenerLimiteSeleccionado() const;
};

#endif // DIALOGESTADISTICASPRODUCTOS_H
