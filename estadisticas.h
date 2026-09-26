#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include "base_datos.h"
#include "conexionesremotas.h"
#include "graficoventaswidget.h"
#include <QDate>
#include <QDialog>
#include <QEvent>
#include <QKeyEvent>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class Estadisticas;
}

/// @brief Tipo de origen para la consulta estadística
enum class TipoOrigenEstadisticas {
  NubeGlobal,     ///< Consolidado de todas las tiendas en la Nube
  NubeTienda,     ///< Tienda remota específica consultada a través de la Nube
  Local,          ///< Base de datos de la tienda local
  RemotoDirecto   ///< Conexión directa TCP/MySQL a tienda remota (fallback si no hay nube)
};

/// @brief Metadatos descriptivos de la fuente de datos seleccionada
struct InfoOrigenEstadisticas {
  TipoOrigenEstadisticas tipo;
  int idTienda;           ///< ID numérico de tienda (o -1 para Global)
  QString nombreConexion; ///< Nombre de conexión Qt (para Local o RemotoDirecto)
  QString nombreTienda;   ///< Nombre descriptivo de la tienda
};

/// @brief Diálogo de estadísticas avanzadas de la aplicación.
/// Muestra KPIs, gráficos de ventas, tablas de productos, clientes,
/// vendedores, formas de pago y familias.
/// Optimizado con carga perezosa (lazy-loading) de pestañas y
/// consultas directas a la infraestructura central en la nube.
/// Pulsar F2 conmuta entre modo normal y consolidado (tickets + ticketss).
class Estadisticas : public QDialog {
  Q_OBJECT

public:
  explicit Estadisticas(QWidget *parent = nullptr);
  ~Estadisticas();

protected:
  /// @brief Captura F2 para conmutar el modo consolidado tickets+ticketss
  void keyPressEvent(QKeyEvent *event) override;

  /// @brief Recarga la lista de tiendas conectadas/disponibles al abrir la ventana.
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_comboTienda_currentIndexChanged(int index);
  void on_tabWidgetEstadisticas_currentChanged(int index);

private:
  Ui::Estadisticas *ui;
  baseDatos base;                     ///< Acceso a la base de datos
  conexionesRemotas *conexiones;      ///< Gestión de conexiones remotas
  GraficoVentasWidget *graficoVentas; ///< Widget del gráfico de ventas

  /// @brief true = consultar tickets+ticketss; false = solo tickets.
  /// Se conmuta con F2.
  bool modoConsolidado;

  /// @brief Control de pestañas pendientes de actualizar (lazy loading).
  /// Indice coincide con las pestañas de QTabWidget.
  QMap<int, bool> pestanasPendientes;

  // Punteros a los modelos de cada tabla en memoria (QStandardItemModel).
  // Desacoplan la GUI del cursor del driver MySQL para evitar fallos de concurrencia.
  QStandardItemModel *modeloTopVendidos;     ///< Modelo para tabla top vendidos
  QStandardItemModel *modeloTopRentables;    ///< Modelo para tabla top rentables
  QStandardItemModel *modeloMejoresClientes; ///< Modelo para tabla mejores clientes
  QStandardItemModel *modeloVendedores;      ///< Modelo para tabla vendedores
  QStandardItemModel *modeloFormaPago;       ///< Modelo para tabla forma de pago
  QStandardItemModel *modeloFamilias;        ///< Modelo para tabla familias

  void cargarTiendas();
  void cargarEstadisticas();
  void cargarPestana(int index);
  void actualizarEstadoUI();

  InfoOrigenEstadisticas obtenerOrigenActual();

  // Funciones auxiliares para cargar cada sección
  void cargarKPIs(const InfoOrigenEstadisticas &origen, const QDate &desde, const QDate &hasta);
  void cargarGraficoVentas(const InfoOrigenEstadisticas &origen, const QDate &desde,
                           const QDate &hasta);
  void cargarTablasProductos(const InfoOrigenEstadisticas &origen, const QDate &desde,
                             const QDate &hasta);
  void cargarTablaClientes(const InfoOrigenEstadisticas &origen, const QDate &desde,
                           const QDate &hasta);
  void cargarTablaVendedores(const InfoOrigenEstadisticas &origen, const QDate &desde,
                             const QDate &hasta);
  void cargarTablaFormaPago(const InfoOrigenEstadisticas &origen, const QDate &desde,
                            const QDate &hasta);
  void cargarTablaFamilias(const InfoOrigenEstadisticas &origen, const QDate &desde,
                           const QDate &hasta);
};

#endif // ESTADISTICAS_H
