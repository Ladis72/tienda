#ifndef PREPARARPEDIDOS_H
#define PREPARARPEDIDOS_H

#include <QDialog>
#include <QDate>
#include <QMap>
#include <QStringList>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class PrepararPedidos;
}

/**
 * @brief Estructura para almacenar los datos agregados de un artículo o periodo.
 */
struct ItemAnalisis {
    QString cod;
    QString descripcion;
    QString periodo;        // Año-Mes, Año, o vacío si es total general
    double stockActual = 0.0;
    double minStock = 0.0;
    double maxStock = 0.0;
    double udsCompradas = 0.0;
    double comprasTotal = 0.0;
    double costoMedio = 0.0;
    double udsVendidas = 0.0;
    double ventasTotal = 0.0;
    double pvpMedio = 0.0;
    double sugerido = 0.0;
};

/**
 * @brief Estructura para almacenar el desglose de un artículo en una tienda específica.
 */
struct DetalleTienda {
    int idTienda = 0;
    QString nombreTienda;
    double stock = 0.0;
    double udsCompradas = 0.0;
    double totalCompras = 0.0;
    double udsVendidas = 0.0;
    double totalVentas = 0.0;
};

/**
 * @brief Diálogo para la preparación de pedidos con análisis consolidado en la Nube
 * de compras y ventas por proveedor, agrupación temporal y desglose por tienda.
 */
class PrepararPedidos : public QDialog
{
    Q_OBJECT

public:
    explicit PrepararPedidos(QWidget *parent = nullptr);
    ~PrepararPedidos();

private slots:
    void on_pushButtonConsultar_clicked();
    void on_tableWidgetPrincipal_itemSelectionChanged();
    void on_tableWidgetPrincipal_cellDoubleClicked(int row, int column);
    void on_lineEditFiltro_textChanged(const QString &text);
    void on_comboBoxAgrupacion_currentIndexChanged(int index);
    void on_comboBoxProveedor_currentIndexChanged(int index);
    void on_spinBoxDiasCobertura_valueChanged(int value);
    void on_checkBoxMostrarSinVentas_toggled(bool checked);

    // Atajos de rango de fechas
    void on_btnMesActual_clicked();
    void on_btnMesAnterior_clicked();
    void on_btnUltimos3Meses_clicked();
    void on_btnUltimos6Meses_clicked();
    void on_btnAnoActual_clicked();

    // Acciones de pedido y utilidades
    void on_pushButtonAnadirPedido_clicked();
    void on_pushButtonAnadirLateral_clicked();
    void on_pushButtonGestionarPedidos_clicked();
    void on_pushButtonExportar_clicked();
    void on_pushButtonCerrar_clicked();

private:
    Ui::PrepararPedidos *ui;
    baseDatos *base;
    bool m_nubeDisponible;
    QMap<int, QString> m_nombresTiendas;
    QList<ItemAnalisis> m_listaArticulos;
    QString m_codSeleccionado;
    QString m_descSeleccionada;

    // Métodos auxiliares de inicialización y consulta
    void inicializarTiendas();
    void cargarProveedores();
    void verificarConexionNube();
    QSqlDatabase obtenerBaseDatosConsulta();
    void consultarDatos();
    void rellenarTablaPrincipal();
    void mostrarDetalleArticulo(const QString &cod, const QString &desc);
    void actualizarTotales();
    void anadirArticuloAPedido(const QString &cod);
    QString obtenerNombreTienda(int idTienda);
};

#endif // PREPARARPEDIDOS_H
