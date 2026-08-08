/**
 * @file dialogtrazabilidad.cpp
 * @brief Implementación de la clase DialogTrazabilidad.
 * 
 * Consulta y presenta la trazabilidad del producto dividida en:
 * 1. Compras a proveedores.
 * 2. Movimientos unificados entre tiendas (entradas y salidas ordenadas por fecha) con resaltado de color.
 */

#include "dialogtrazabilidad.h"
#include "ui_dialogtrazabilidad.h"

DialogTrazabilidad::DialogTrazabilidad(const QString &codArticulo, const QString &descripcionArticulo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogTrazabilidad)
    , m_codArticulo(codArticulo)
    , m_descripcionArticulo(descripcionArticulo)
    , totalUdsCompras(0)
    , totalUdsEntradas(0)
    , totalUdsSalidas(0)
    , totalRegEntradas(0)
    , totalRegSalidas(0)
{
    ui->setupUi(this);

    // Configurar el título superior de la ventana de trazabilidad
    ui->labelTitulo->setText(QString("Trazabilidad: [%1] %2")
                                 .arg(m_codArticulo, m_descripcionArticulo));

    // Cargar compras a proveedores y movimientos unificados entre tiendas
    cargarCompras();
    cargarMovimientosTiendas();

    // Actualizar el resumen general en la cabecera
    actualizarResumenGlobal();
}

DialogTrazabilidad::~DialogTrazabilidad()
{
    delete ui;
}

/**
 * @brief Carga la información de compras a proveedores desde lineaspedido y pedidos.
 */
void DialogTrazabilidad::cargarCompras()
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery query(db);

    // Consulta preparada formateando la fecha a "yyyy-MM-dd" y utilizando CONVERT para evitar problemas de collation en MySQL
    query.prepare("SELECT "
                  "DATE_FORMAT(p.fechaPedido, '%Y-%m-%d') AS 'Fecha', "
                  "lp.nDocumento AS 'Nº Pedido', "
                  "COALESCE(CONVERT(prov.nombre USING utf8mb4), CAST(lp.idProveedor AS CHAR)) AS 'Proveedor', "
                  "lp.cantidad AS 'Cantidad', "
                  "lp.bonificacion AS 'Bonif.', "
                  "lp.costo AS 'Precio Coste (€)', "
                  "ROUND(lp.cantidad * lp.costo, 2) AS 'Total Coste (€)' "
                  "FROM lineaspedido lp "
                  "LEFT JOIN pedidos p ON lp.nDocumento = p.npedido "
                  "LEFT JOIN proveedores prov ON lp.idProveedor = prov.idProveedor "
                  "WHERE lp.cod = ? "
                  "ORDER BY p.fechaPedido DESC");
    query.addBindValue(m_codArticulo);

    if (query.exec()) {
        modeloCompras.setQuery(std::move(query));
        ui->tableViewCompras->setModel(&modeloCompras);
        ui->tableViewCompras->resizeColumnsToContents();

        // Calcular total de registros y unidades de compras a proveedores
        totalUdsCompras = 0;
        int filas = modeloCompras.rowCount();
        for (int i = 0; i < filas; ++i) {
            totalUdsCompras += modeloCompras.data(modeloCompras.index(i, 3)).toInt();
        }

        ui->labelTotalCompras->setText(QString("Total compras a proveedores: %1 registros | %2 unidades")
                                           .arg(filas)
                                           .arg(totalUdsCompras));
    } else {
        qDebug() << "Error en consulta de compras de trazabilidad:" << query.lastError().text();
    }
}

/**
 * @brief Carga de forma unificada las Entradas y Salidas de tiendas ordenadas cronológicamente.
 */
void DialogTrazabilidad::cargarMovimientosTiendas()
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery query(db);

    // Consulta unificada UNION ALL ordenada cronológicamente por Fecha DESC, con formato "yyyy-MM-dd"
    query.prepare("SELECT "
                  "'ENTRADA' AS 'Tipo Movimiento', "
                  "DATE_FORMAT(e.fechaEntrada, '%Y-%m-%d') AS 'Fecha', "
                  "COALESCE(CONVERT(t.nombre USING utf8mb4), CAST(e.idTienda AS CHAR)) AS 'Tienda Origen / Destino', "
                  "e.cantidad AS 'Cantidad', "
                  "e.pvp AS 'PVP (€)', "
                  "DATE_FORMAT(e.fechaCaducidad, '%Y-%m-%d') AS 'Fecha Caducidad' "
                  "FROM entradaGenero e "
                  "LEFT JOIN tiendas t ON e.idTienda = t.id "
                  "WHERE e.cod = ? "
                  "UNION ALL "
                  "SELECT "
                  "'SALIDA' AS 'Tipo Movimiento', "
                  "DATE_FORMAT(s.fechaEntrada, '%Y-%m-%d') AS 'Fecha', "
                  "COALESCE(CONVERT(t.nombre USING utf8mb4), CAST(s.idTienda AS CHAR)) AS 'Tienda Origen / Destino', "
                  "s.cantidad AS 'Cantidad', "
                  "s.pvp AS 'PVP (€)', "
                  "DATE_FORMAT(s.fechaCaducidad, '%Y-%m-%d') AS 'Fecha Caducidad' "
                  "FROM salidaGenero s "
                  "LEFT JOIN tiendas t ON s.idTienda = t.id "
                  "WHERE s.cod = ? "
                  "ORDER BY Fecha DESC");

    query.addBindValue(m_codArticulo);
    query.addBindValue(m_codArticulo);

    if (query.exec()) {
        modeloMovimientos.setQuery(std::move(query));
        ui->tableViewMovimientos->setModel(&modeloMovimientos);
        ui->tableViewMovimientos->resizeColumnsToContents();

        // Inicializar contadores
        totalRegEntradas = 0;
        totalRegSalidas = 0;
        totalUdsEntradas = 0;
        totalUdsSalidas = 0;

        int filas = modeloMovimientos.rowCount();
        for (int i = 0; i < filas; ++i) {
            QString tipo = modeloMovimientos.data(modeloMovimientos.index(i, 0)).toString();
            int cantidad = modeloMovimientos.data(modeloMovimientos.index(i, 3)).toInt();

            if (tipo == "ENTRADA") {
                totalRegEntradas++;
                totalUdsEntradas += cantidad;
            } else if (tipo == "SALIDA") {
                totalRegSalidas++;
                totalUdsSalidas += cantidad;
            }
        }

        ui->labelTotalMovimientos->setText(QString("Total movimientos entre tiendas: %1 entradas (%2 uds) | %3 salidas (%4 uds)")
                                               .arg(totalRegEntradas)
                                               .arg(totalUdsEntradas)
                                               .arg(totalRegSalidas)
                                               .arg(totalUdsSalidas));
    } else {
        qDebug() << "Error en consulta unificada de movimientos de trazabilidad:" << query.lastError().text();
    }
}

/**
 * @brief Actualiza la etiqueta con el resumen general en la zona superior del diálogo.
 */
void DialogTrazabilidad::actualizarResumenGlobal()
{
    ui->labelTotalesResumen->setText(QString("Compras: %1 uds | Entradas: %2 uds | Salidas: %3 uds")
                                         .arg(totalUdsCompras)
                                         .arg(totalUdsEntradas)
                                         .arg(totalUdsSalidas));
}
