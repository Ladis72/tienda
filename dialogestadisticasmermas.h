#ifndef DIALOGESTADISTICASMERMAS_H
#define DIALOGESTADISTICASMERMAS_H

#include "base_datos.h"
#include <QDate>
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasMermas;
}

/**
 * @brief Diálogo de Control de Mermas, Roturas y Desperdicios.
 *
 * Permite auditar y controlar las pérdidas de stock de la empresa:
 * - Ranking de productos con mayor merma, coste económico y unidades perdidas.
 * - Desglose analítico por motivo o concepto de salida (caducidades, roturas, etc.).
 * - Comparativa de pérdidas entre tiendas físicas para benchmarking de control de inventario.
 * - Pérdidas agregadas por familias y categorías de producto.
 *
 * Emplea consultas optimizadas a la base de datos central en la nube y modelos desacoplados.
 */
class DialogEstadisticasMermas : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasMermas(QWidget *parent = nullptr);
  ~DialogEstadisticasMermas();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_tabWidgetMermas_currentChanged(int index);

  // Filtros rápidos de fechas
  void on_btnHoy_clicked();
  void on_btnSemana_clicked();
  void on_btnMes_clicked();
  void on_btnAno_clicked();

  // Exportaciones
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();

private:
  Ui::DialogEstadisticasMermas *ui;
  baseDatos base;

  QMap<int, bool> pestanasPendientes; ///< Control de lazy-loading para las pestañas

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloRankingMermas;
  QStandardItemModel *modeloMotivos;
  QStandardItemModel *modeloTiendas;
  QStandardItemModel *modeloFamilias;

  void inicializarFiltros();
  void cargarEstadisticas();
  void cargarPestana(int index);

  void cargarKPIs(const QDate &desde, const QDate &hasta, int idFamilia, int tipoMerma, int idTienda);
  void cargarTablaRankingMermas(const QDate &desde, const QDate &hasta, int idFamilia, int tipoMerma, int limite, int idTienda);
  void cargarTablaMotivos(const QDate &desde, const QDate &hasta, int idTienda);
  void cargarTablaTiendas(const QDate &desde, const QDate &hasta);
  void cargarTablaFamilias(const QDate &desde, const QDate &hasta, int idTienda);

  int obtenerIdTiendaSeleccionada() const;
  int obtenerIdFamiliaSeleccionada() const;
  int obtenerTipoMermaSeleccionado() const;
};

#endif // DIALOGESTADISTICASMERMAS_H
