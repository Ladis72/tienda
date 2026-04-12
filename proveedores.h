#ifndef PROVEEDORES_H
#define PROVEEDORES_H
#include "base_datos.h"
#include "buscarproveedor.h"
#include "configuracion.h"
#include "formaspago.h"

#include <QtCharts>
#include <QDialog>
#include <QKeyEvent>

extern Configuracion *conf;

namespace Ui {
class Proveedores;
}

class Proveedores : public QDialog
{
    Q_OBJECT

public:
    explicit Proveedores(QWidget *parent = 0);
    ~Proveedores();

private slots:
    void on_pushButtonNuevo_clicked();
    void on_pushButtonAnterior_clicked();
    void on_pushButtonSiguiente_clicked();
    void on_lineEditNombre_returnPressed();
    void on_pushButtonModificar_clicked();
    void on_lineEditFormaPago_textChanged(const QString &arg1);
    void on_pushButtonFPago_clicked();
    void on_lineEditCod_editingFinished();
    void on_pushButton_clicked();
    void on_radioButtonComprasFacturas_clicked();
    void on_radioButtonComprasMeses_clicked();
    void on_radioButtonComprasAnos_clicked();
    void on_pushButtonBorrar_clicked();
    void on_pushButtonRefrescar_clicked();
    void on_dateEditDesde_dateChanged(const QDate &date);
    void on_dateEditHasta_dateChanged(const QDate &date);
    void on_pushButtonCerrar_clicked();
    void on_pushButtonUnificar_clicked();
    void on_pushButtonSaneador_clicked();

private:
    Ui::Proveedores *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;
    BuscarProveedor *proveedor;
    FormasPago *fpago;
    QSqlQueryModel modeloCompras;
    QSqlQueryModel modeloVentas; // Modelo para análisis de ventas
    bool mostrarVentasB;         // Estado para tecla F2 (Ventas especiales)

    void recargarTabla();
    void borrarFormulario();
    bool eventFilter(QObject *obj, QEvent *event);
    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();
    void cargarCompras();
    void cargarVentas();
    void cargarGraficoCompras();
    void cargarGraficoVentas();
    void actualizarTotalesVentas(double totalA, double totalB);
    void keyPressEvent(QKeyEvent *e);
    void setupIcons();

    struct VentasProd {
        QString descripcion;
        double unidades = 0;
        double totalA = 0;
        double totalB = 0;
    };

    QChartView *chartViewCompras;
    QChartView *chartViewVentas;
    QComboBox *comboBoxVentasFiltro;
    QCheckBox *checkBoxRemoto;
    QStandardItemModel modeloComprasGlobal;
    QStandardItemModel modeloVentasGlobal;
};

#endif // PROVEEDORES_H
