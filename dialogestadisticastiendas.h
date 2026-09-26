#ifndef DIALOGESTADISTICASTIENDAS_H
#define DIALOGESTADISTICASTIENDAS_H

#include "base_datos.h"
#include <QDate>
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasTiendas;
}

/**
 * @brief Diálogo de Comparativa Multitienda y Benchmarking de la Cadena.
 * Permite analizar en paralelo el rendimiento de todos los puntos de venta:
 * - Ventas totales, cuota porcentual sobre la facturación de la cadena y volumen de tickets.
 * - Ticket medio y promedio de venta diaria por tienda activa.
 * - Desglose de formas de pago (Efectivo vs Tarjeta) por cada tienda.
 * - Evolución temporal comparada (mes, semana o día).
 *
 * Utiliza consultas directas a la infraestructura central en la nube y modelos
 * desacoplados en memoria (QStandardItemModel) para evitar bloqueos de GUI.
 */
class DialogEstadisticasTiendas : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasTiendas(QWidget *parent = nullptr);
  ~DialogEstadisticasTiendas();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_tabWidgetTiendas_currentChanged(int index);
  void on_comboAgrupacion_currentIndexChanged(int index);

  // Filtros rápidos de fechas
  void on_btnHoy_clicked();
  void on_btnSemana_clicked();
  void on_btnMes_clicked();
  void on_btnAno_clicked();

  // Exportaciones
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();

private:
  Ui::DialogEstadisticasTiendas *ui;
  baseDatos base;

  QMap<int, bool> pestanasPendientes; ///< Control de carga bajo demanda (lazy-loading)

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloComparativa; ///< Resumen general comparativo de tiendas
  QStandardItemModel *modeloFormasPago;   ///< Desglose formas de pago por tienda
  QStandardItemModel *modeloEvolucion;    ///< Evolución temporal por tienda

  void cargarEstadisticas();
  void cargarPestana(int index);

  void cargarKPIs(const QDate &desde, const QDate &hasta);
  void cargarTablaComparativa(const QDate &desde, const QDate &hasta);
  void cargarTablaFormasPago(const QDate &desde, const QDate &hasta);
  void cargarTablaEvolucion(const QDate &desde, const QDate &hasta);
};

#endif // DIALOGESTADISTICASTIENDAS_H
