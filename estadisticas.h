#ifndef ESTADISTICAS_H
#define ESTADISTICAS_H

#include "base_datos.h"
#include "conexionesremotas.h"
#include "graficoventaswidget.h"
#include <QDate>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>

namespace Ui {
class Estadisticas;
}

/// @brief Diálogo de estadísticas avanzadas de la aplicación.
/// Muestra KPIs, gráficos de ventas, tablas de productos, clientes,
/// vendedores, formas de pago y familias.
/// Pulsar F2 conmuta entre modo normal (solo tickets) y modo consolidado
/// (tickets + ticketss), incluyendo todos los cálculos derivados.
class Estadisticas : public QDialog {
  Q_OBJECT

public:
  explicit Estadisticas(QWidget *parent = nullptr);
  ~Estadisticas();

protected:
  /// @brief Captura F2 para conmutar el modo consolidado tickets+ticketss
  void keyPressEvent(QKeyEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_comboTienda_currentIndexChanged(int index);

private:
  Ui::Estadisticas *ui;
  baseDatos base;                     ///< Acceso a la base de datos
  conexionesRemotas *conexiones;      ///< Gestión de conexiones remotas
  GraficoVentasWidget *graficoVentas; ///< Widget del gráfico de ventas

  /// @brief true = consultar tickets+ticketss; false = solo tickets.
  /// Se conmuta con F2.
  bool modoConsolidado;

  // Punteros a los modelos de cada tabla para gestión de memoria correcta.
  // Se eliminan y recrean en cada actualización evitando fugas de memoria.
  QSqlQueryModel *modeloTopVendidos;     ///< Modelo para tabla top vendidos
  QSqlQueryModel *modeloTopRentables;    ///< Modelo para tabla top rentables
  QSqlQueryModel *modeloMejoresClientes; ///< Modelo para tabla mejores clientes
  QSqlQueryModel *modeloVendedores;      ///< Modelo para tabla vendedores
  QSqlQueryModel *modeloFormaPago;       ///< Modelo para tabla forma de pago
  QSqlQueryModel *modeloFamilias;        ///< Modelo para tabla familias

  void cargarTiendas();
  void cargarEstadisticas();

  // Funciones auxiliares para cargar cada pestaña/sección.
  // El parámetro 'consolidado' indica si incluir la tabla ticketss.
  void cargarKPIs(const QString &db, const QDate &desde, const QDate &hasta);
  void cargarGraficoVentas(const QString &db, const QDate &desde,
                           const QDate &hasta);
  void cargarTablasProductos(const QString &db, const QDate &desde,
                             const QDate &hasta);
  void cargarTablaClientes(const QString &db, const QDate &desde,
                           const QDate &hasta);
  void cargarTablaVendedores(const QString &db, const QDate &desde,
                             const QDate &hasta);
  void cargarTablaFormaPago(const QString &db, const QDate &desde,
                            const QDate &hasta);
  void cargarTablaFamilias(const QString &db, const QDate &desde,
                           const QDate &hasta);

  QString getConexionSeleccionada();
};

#endif // ESTADISTICAS_H
