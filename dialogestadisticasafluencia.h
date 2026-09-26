#ifndef DIALOGESTADISTICASALFLUENCIA_H
#define DIALOGESTADISTICASALFLUENCIA_H

#include "base_datos.h"
#include "conexionesremotas.h"
#include <QDate>
#include <QDialog>
#include <QKeyEvent>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogEstadisticasAfluencia;
}

/// @brief Ámbito de consulta para el diálogo de afluencia
enum class TipoOrigenAfluencia {
  NubeGlobal,     ///< Consolidado de toda la cadena en la Nube
  NubeTienda,     ///< Tienda individual a través de la Nube
  Local,          ///< Base de datos local
  RemotoDirecto   ///< Fallback TCP directo a tienda remota
};

/// @brief Datos del origen seleccionado
struct InfoOrigenAfluencia {
  TipoOrigenAfluencia tipo;
  int idTienda;           ///< ID numérico de la tienda (-1 para Global)
  QString nombreConexion; ///< Identificador de conexión en Qt
  QString nombreTienda;   ///< Nombre legible de la tienda
};

/**
 * @brief Diálogo especializado en análisis de concurrencia y patrones de afluencia.
 * Desglosa ventas y afluencia por:
 * 1. Franjas horarias (00h - 23h), identificando las horas pico de máxima facturación y tickets.
 * 2. Días de la semana (Lunes a Domingo), con comparativa entre laborables y fines de semana.
 * 3. Matriz cruzada de actividad (Día x Franja horaria) con mapa de intensidad de tránsito.
 *
 * Utiliza modelos de memoria desacoplados (QStandardItemModel) y consultas aceleradas en la nube.
 * Pulsar F2 conmuta entre modo normal (tickets) y consolidado (tickets + ticketss).
 */
class DialogEstadisticasAfluencia : public QDialog {
  Q_OBJECT

public:
  explicit DialogEstadisticasAfluencia(QWidget *parent = nullptr);
  ~DialogEstadisticasAfluencia();

protected:
  /// @brief Captura la pulsación de F2 para conmutar modo consolidado
  void keyPressEvent(QKeyEvent *event) override;

  /// @brief Recarga la lista de tiendas y datos al mostrar la ventana
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_comboTienda_currentIndexChanged(int index);
  void on_tabWidgetAfluencia_currentChanged(int index);

  // Filtros de fecha rápidos
  void on_btnHoy_clicked();
  void on_btnSemana_clicked();
  void on_btnMes_clicked();
  void on_btnAno_clicked();

  // Exportaciones
  void on_btnExportarCSV_clicked();
  void on_btnCopiarTabla_clicked();

private:
  Ui::DialogEstadisticasAfluencia *ui;
  baseDatos base;
  conexionesRemotas *conexiones;

  bool modoConsolidado;              ///< Conmuta con F2 (true = tickets+ticketss)
  QMap<int, bool> pestanasPendientes; ///< Control de lazy-loading por pestaña

  // Modelos desacoplados en memoria
  QStandardItemModel *modeloHoras;   ///< Franjas horarias
  QStandardItemModel *modeloDias;    ///< Días de la semana

  void cargarTiendas();
  void cargarEstadisticas();
  void cargarPestana(int index);
  void actualizarEstadoUI();

  InfoOrigenAfluencia obtenerOrigenActual();

  void cargarKPIs(const InfoOrigenAfluencia &origen, const QDate &desde, const QDate &hasta);
  void cargarTablaHoras(const InfoOrigenAfluencia &origen, const QDate &desde, const QDate &hasta);
  void cargarTablaDias(const InfoOrigenAfluencia &origen, const QDate &desde, const QDate &hasta);
  void cargarMatrizDiaHora(const InfoOrigenAfluencia &origen, const QDate &desde, const QDate &hasta);
};

#endif // DIALOGESTADISTICASALFLUENCIA_H
