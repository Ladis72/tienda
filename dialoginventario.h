#ifndef DIALOGINVENTARIO_H
#define DIALOGINVENTARIO_H

#include "base_datos.h"
#include "estadisticas.h"
#include "estadisticas_utils.h"
#include <QDate>
#include <QDialog>
#include <QList>
#include <QMap>
#include <QStandardItemModel>

namespace Ui {
class DialogInventario;
}

/**
 * @brief Estructura que representa un lote físico dentro de un artículo.
 */
struct DetalleLoteInventario {
  QString lote;             ///< Código o número de lote
  QDate fechaCaducidad;     ///< Fecha de caducidad del lote
  double cantidad;          ///< Cantidad disponible en dicho lote
  int idTienda;             ///< ID de la tienda (en entornos multitienda)
  QString nombreTienda;     ///< Nombre descriptivo de la tienda
};

/**
 * @brief Estructura con la información consolidada de un producto en el inventario.
 */
struct ArticuloInventario {
  QString codigo;           ///< Código EAN o referencia del producto
  QString descripcion;      ///< Nombre / descripción del producto
  int idFamilia;            ///< ID numérico de la familia
  QString familia;          ///< Nombre de la familia / categoría
  double pvp;               ///< Precio de venta al público (PVP)
  double coste;             ///< Precio de coste / compra (PVD)
  double stockTotal;        ///< Suma del stock de todos los lotes
  QDate proximaCaducidad;   ///< Fecha de caducidad más cercana
  QList<DetalleLoteInventario> lotes; ///< Desglose detallado de lotes
};

/**
 * @brief Estructura para la agregación de inventario por familias.
 */
struct ResumenFamiliaInventario {
  int idFamilia = 0;
  QString nombreFamilia;
  int articulosCount = 0;
  double unidadesTotal = 0.0;
  double valorCosteTotal = 0.0;
  double valorPvpTotal = 0.0;
};

/**
 * @brief Diálogo para listar el inventario actual y su valoración económica.
 * Permite desglosar la información por:
 *  1. Artículos consolidados (resumen por producto)
 *  2. Detalle de lotes y fechas de caducidad
 *  3. Árbol jerárquico interactivo (Familia ➔ Artículo ➔ Lotes)
 *  4. Desglose agrupado por familias / categorías
 */
class DialogInventario : public QDialog {
  Q_OBJECT

public:
  explicit DialogInventario(const InfoOrigenEstadisticas &origenInicial, QWidget *parent = nullptr);
  ~DialogInventario();

protected:
  void showEvent(QShowEvent *event) override;

private slots:
  void on_btnActualizar_clicked();
  void on_comboTienda_currentIndexChanged(int index);
  void on_comboFamilia_currentIndexChanged(int index);
  void on_comboFiltroStock_currentIndexChanged(int index);
  void on_txtBuscar_textChanged(const QString &texto);

  // Botones de control del árbol jerárquico
  void on_btnExpandirTodo_clicked();
  void on_btnContraerTodo_clicked();
  void on_btnExpandirFamilias_clicked();

  // Exportación y utilidades
  void on_btnExportarCSV_clicked();
  void on_btnCopiar_clicked();
  void on_btnImprimir_clicked();

  // Apertura de ficha de stock al hacer doble clic
  void on_tablaArticulos_doubleClicked(const QModelIndex &index);
  void on_tablaLotes_doubleClicked(const QModelIndex &index);
  void on_treeInventario_itemDoubleClicked(class QTreeWidgetItem *item, int column);

private:
  Ui::DialogInventario *ui;
  baseDatos base;
  InfoOrigenEstadisticas origenActual;

  // Lista global en memoria de todos los artículos cargados de la base de datos
  QList<ArticuloInventario> articulosCargados;

  // Modelos para las tablas
  QStandardItemModel *modeloArticulos;
  QStandardItemModel *modeloLotes;
  QStandardItemModel *modeloFamilias;

  void inicializarFiltros();
  void cargarDatosDesdeBD();
  void aplicarFiltrosYActualizarVistas();

  // Métodos de población de vistas
  void actualizarKPIs(const QList<ArticuloInventario> &articulosFiltrados);
  void poblarTablaArticulos(const QList<ArticuloInventario> &articulosFiltrados);
  void poblarTablaLotes(const QList<ArticuloInventario> &articulosFiltrados);
  void poblarArbolJerarquico(const QList<ArticuloInventario> &articulosFiltrados);
  void poblarTablaFamilias(const QList<ArticuloInventario> &articulosFiltrados);

  // Métodos auxiliares de filtrado
  QList<ArticuloInventario> obtenerArticulosFiltrados() const;
  void abrirFichaStock(const QString &codigoArticulo);
};

#endif // DIALOGINVENTARIO_H
