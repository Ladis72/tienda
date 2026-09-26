#ifndef DIALOGESTADISTICASCLIENTES_H
#define DIALOGESTADISTICASCLIENTES_H

#include "base_datos.h"
#include <QDate>
#include <QDialog>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasClientes;
}

/**
 * @brief Diálogo de Estadísticas de Clientes, Recurrencia y Fidelización.
 *
 * Proporciona métricas avanzadas sobre el comportamiento de compra de los clientes:
 * - Ranking VIP por facturación acumulada y número de visitas.
 * - Frecuencia, periodicidad media entre visitas y recurrencia de compra.
 * - Detección preventiva de clientes en riesgo de fuga (> 60 días inactivos).
 * - Desglose geográfico de compras y clientes por localidad o municipio.
 * - Acceso directo a la ficha del cliente con historial completo.
 *
 * Utiliza consultas directas a la base central de la nube y modelos desacoplados en memoria.
 */
class DialogEstadisticasClientes : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasClientes(QWidget *parent = nullptr);
  ~DialogEstadisticasClientes();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_tabWidgetClientes_currentChanged(int index);

  // Filtros rápidos de fechas
  void on_btnHoy_clicked();
  void on_btnSemana_clicked();
  void on_btnMes_clicked();
  void on_btnAno_clicked();

  // Acciones y exportaciones
  void on_btnVerFicha_clicked();
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();
  void onTablaDobleClick(const QModelIndex &index);

private:
  Ui::DialogEstadisticasClientes *ui;
  baseDatos base;

  QMap<int, bool> pestanasPendientes; ///< Control de lazy-loading para las pestañas

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloRankingVIP;
  QStandardItemModel *modeloRecurrencia;
  QStandardItemModel *modeloRiesgoFuga;
  QStandardItemModel *modeloLocalidades;

  void inicializarFiltros();
  void cargarEstadisticas();
  void cargarPestana(int index);

  void cargarKPIs(const QDate &desde, const QDate &hasta, int idTienda);
  void cargarTablaRankingVIP(const QDate &desde, const QDate &hasta, bool soloRegistrados, int limite, int idTienda);
  void cargarTablaRecurrencia(const QDate &desde, const QDate &hasta, int limite, int idTienda);
  void cargarTablaRiesgoFuga(int dias, int limite, int idTienda);
  void cargarTablaLocalidades(const QDate &desde, const QDate &hasta, int idTienda);

  int obtenerIdTiendaSeleccionada() const;
  bool obtenerSoloRegistrados() const;
  int obtenerLimiteSeleccionado() const;
  int obtenerIdClienteSeleccionado() const;
};

#endif // DIALOGESTADISTICASCLIENTES_H
