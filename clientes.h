#ifndef CLIENTES_H
#define CLIENTES_H
#include "base_datos.h"
#include "configuracion.h"
#include <QDataWidgetMapper>
#include <QDate>
#include <QDateTime>
#include <QDialog>
#include <QList>
#include <QSortFilterProxyModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QStandardItem>
#include <QStandardItemModel>
#include <QString>
#include <QtCharts>

extern Configuracion *conf;
namespace Ui {
class Clientes;
}

class Clientes : public QDialog {
  Q_OBJECT

public:
  explicit Clientes(QWidget *parent = nullptr);
  explicit Clientes(QWidget *parent, QString codigo);
  ~Clientes();

private slots:
  void on_pushButtonAnterior_clicked();

  void on_pushButtonNuevo_clicked();

  void on_pushButtonModificar_clicked();

  void on_pushButtonBorrar_clicked();

  void on_pushButtonSiguiente_clicked();

  void on_lineEditNombre_returnPressed();

  void on_lineEditCod_editingFinished();

  // Slot unificado: gestiona Ventas y Productos a la vez
  void on_checkBoxTiendasConectadas_clicked();

  void on_tableView2_doubleClicked(const QModelIndex &index);

  void on_tableView2_clicked(const QModelIndex &index);

  void on_radioButtonCantidad_clicked();

  void on_radioButtonFecha_clicked();

  void on_comboBoxAgrupacion_currentIndexChanged(int index);

  void on_comboBoxTipoGrafico_currentIndexChanged(int index);

  void on_dateEditDesde_dateChanged(const QDate &date);
  void on_lineEditBuscarProducto_textChanged(const QString &arg1);
  void on_btn_encargos_cliente_clicked();

  void on_dateEditHasta_dateChanged(const QDate &date);
  void on_dateEditDesde_2_dateChanged(const QDate &date);
  void on_dateEditHasta_2_dateChanged(const QDate &date);
  void on_pushButtonUnificar_clicked();
  void on_pushButtonSaneador_clicked();
  void on_pushButtonRefrescar_clicked();

  void onBarClicked(int index, QBarSet *barset);

  void onBarHovered(bool status, int index, QBarSet *barset);

  void cargarTicketsPorRango(const QString &rangoMapeado);

private:
  Ui::Clientes *ui;

  baseDatos base;
  QSqlQueryModel *modeloTabla;
  QSqlQueryModel modeloCompras;
  QDataWidgetMapper mapper;
  QSqlQuery consulta;
  QString nombreConexionLocal;
  QString fechaI, fechaF;
  QSqlQueryModel *listaTickets;
  QSqlQueryModel *ticket;
  QString nTicket;
  QStandardItemModel *vistaTickets;
  QStringList listaConexionesRemotas;
  QList<QStandardItem *> listaItems;
  QStandardItemModel *vistaArticulosVendido;
  QStandardItemModel *modeloProductos;
  QSortFilterProxyModel *proxyProductos;
  QSqlQueryModel *listaProductos;

  QChart *ventasChart;
  QChartView *ventasChartView;
  QAbstractBarSeries *seriesVentas;
  QStringList mapeoCategRango;

  void inicializarComponentes();
  void recargarTabla();
  void borrarFormulario();
  bool eventFilter(QObject *obj, QEvent *event);
  void keyPressEvent(QKeyEvent *e) override;
  void refrescarBotones(int i);
  void cargarCompras();
  QStringList recogerDatosFormulario();

  bool mostrarVentasB = false;
};

#endif // CLIENTES_H
