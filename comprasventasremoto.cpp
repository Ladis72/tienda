#include "comprasventasremoto.h"
#include "ui_comprasventasremoto.h"
#include "syncmanager.h"

// Constructor de la ventana de histórico de compras y ventas de un artículo en tienda remota
comprasVentasRemoto::comprasVentasRemoto(QSqlDatabase base, QString ean, int idTienda, QString nombreTienda, QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::comprasVentasRemoto)
    , codigo(ean)
    , db(base)
    , m_idTienda(idTienda)
    , m_nombreTienda(nombreTienda)
{
    ui->setupUi(this);
    // Si la conexión directa no es válida o no está abierta, intentar obtenerla por nombre
    if ((!db.isValid() || !db.isOpen()) && !m_nombreTienda.isEmpty()) {
        if (QSqlDatabase::contains(m_nombreTienda)) {
            db = QSqlDatabase::database(m_nombreTienda);
        }
    }
    if (db.isValid() && !db.isOpen()) {
        db.open();
    }

    // Si no se proporcionó idTienda pero se tiene el nombre, resolverlo mediante la Nube
    if (m_idTienda <= 0 && !m_nombreTienda.isEmpty()) {
        if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
            QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
            QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
            QSqlQuery qId(dbNube);
            qId.prepare("SELECT id FROM tiendas WHERE nombre = ? LIMIT 1");
            qId.bindValue(0, m_nombreTienda);
            if (qId.exec() && qId.next()) {
                m_idTienda = qId.value(0).toInt();
            }
        }
    }

    setAttribute(Qt::WA_DeleteOnClose);
    modeloVentas = new QSqlQueryModel(this);
    modeloCompras = new QSqlQueryModel(this);

    // Cargar vistas iniciales
    on_radioButtonVentaMes_clicked();
    on_radioButtonComprasMes_clicked();
}

comprasVentasRemoto::~comprasVentasRemoto()
{
    if (ui && ui->tableViewVentas) {
        ui->tableViewVentas->setModel(nullptr);
    }
    if (ui && ui->tableViewCompras) {
        ui->tableViewCompras->setModel(nullptr);
    }
    if (modeloVentas) {
        delete modeloVentas;
        modeloVentas = nullptr;
    }
    if (modeloCompras) {
        delete modeloCompras;
        modeloCompras = nullptr;
    }
    delete ui;
    ui = nullptr;
}

void comprasVentasRemoto::on_radioButtonVentasDia_clicked()
{
    if (!modeloVentas) return;
    modeloVentas->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT descripcion, fecha, SUM(cantidad) FROM lineasticket_nube "
                  "WHERE cod = :cod AND id_tienda = :idTienda GROUP BY fecha DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT descripcion , fecha , sum(cantidad) FROM lineasticket "
                  "WHERE cod = ? GROUP BY fecha DESC");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::VentasDia:" << q.lastError().text();
    modeloVentas->setQuery(q);
    modeloVentas->setHeaderData(0, Qt::Horizontal, "Producto");
    modeloVentas->setHeaderData(1, Qt::Horizontal, "Fecha");
    modeloVentas->setHeaderData(2, Qt::Horizontal, "Cantidad");
    ui->tableViewVentas->setModel(modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonVentaMes_clicked()
{
    if (!modeloVentas) return;
    modeloVentas->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT descripcion, YEAR(fecha), MONTH(fecha), SUM(cantidad) "
                  "FROM lineasticket_nube WHERE cod = :cod AND id_tienda = :idTienda "
                  "GROUP BY YEAR(fecha) DESC, MONTH(fecha) DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT descripcion , YEAR(fecha) , MONTH(fecha) , sum(cantidad) "
                  "from lineasticket WHERE cod = ? GROUP BY YEAR(fecha) desc , "
                  "MONTH(fecha) desc");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::VentaMes:" << q.lastError().text();
    modeloVentas->setQuery(q);
    modeloVentas->setHeaderData(0, Qt::Horizontal, "Artículo");
    modeloVentas->setHeaderData(1, Qt::Horizontal, "Año");
    modeloVentas->setHeaderData(2, Qt::Horizontal, "Mes");
    modeloVentas->setHeaderData(3, Qt::Horizontal, "Cantidad");
    ui->tableViewVentas->setModel(modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonVentasAno_clicked()
{
    if (!modeloVentas) return;
    modeloVentas->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT descripcion, YEAR(fecha), SUM(cantidad) "
                  "FROM lineasticket_nube WHERE cod = :cod AND id_tienda = :idTienda "
                  "GROUP BY YEAR(fecha) DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT descripcion , YEAR(fecha) , sum(cantidad) from "
                  "lineasticket WHERE cod = ? GROUP BY YEAR(fecha) desc");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::VentasAno:" << q.lastError().text();
    modeloVentas->setQuery(q);
    modeloVentas->setHeaderData(0, Qt::Horizontal, "Artículo");
    modeloVentas->setHeaderData(1, Qt::Horizontal, "Año");
    modeloVentas->setHeaderData(2, Qt::Horizontal, "Cantidad");

    ui->tableViewVentas->setModel(modeloVentas);
    ui->tableViewVentas->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasDia_clicked()
{
    if (!modeloCompras) return;
    modeloCompras->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT l.nDocumento, p.idProveedor, l.cantidad, l.bonificacion, l.costo, l.descuento1, p.fechaPedido "
                  "FROM lineaspedido_nube l "
                  "JOIN pedidos_nube p ON l.id_tienda = p.id_tienda AND l.nDocumento = p.npedido "
                  "WHERE l.cod = :cod AND l.id_tienda = :idTienda "
                  "ORDER BY p.fechaPedido DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT `nDocumento` , `pedidos`.`idProveedor` , `cantidad` , "
                  "`bonificacion` , `costo` , `descuento1`, `pedidos`.`fechaPedido` "
                  "FROM `lineaspedido` JOIN `pedidos` on `nDocumento` = "
                  "`pedidos`.`npedido` WHERE `cod` = ? ORDER BY "
                  "`pedidos`.`fechaPedido` DESC");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::ComprasDia:" << q.lastError().text();
    modeloCompras->setQuery(q);
    ui->tableViewCompras->setModel(modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasMes_clicked()
{
    if (!modeloCompras) return;
    modeloCompras->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT YEAR(p.fechaPedido), MONTH(p.fechaPedido), "
                  "SUM(l.cantidad), SUM(l.bonificacion) "
                  "FROM lineaspedido_nube l "
                  "JOIN pedidos_nube p ON l.id_tienda = p.id_tienda AND l.nDocumento = p.npedido "
                  "WHERE l.cod = :cod AND l.id_tienda = :idTienda "
                  "GROUP BY YEAR(p.fechaPedido) DESC, MONTH(p.fechaPedido) DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT YEAR(pedidos.fechaPedido) , MONTH(pedidos.fechaPedido) , "
                  "sum(cantidad) , sum(bonificacion) FROM lineaspedido JOIN "
                  "pedidos ON nDocumento = pedidos.npedido WHERE cod = ? GROUP BY "
                  "YEAR(pedidos.fechaPedido) DESC , MONTH(pedidos.fechaPedido) DESC");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::ComprasMes:" << q.lastError().text();
    modeloCompras->setQuery(q);
    ui->tableViewCompras->setModel(modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}

void comprasVentasRemoto::on_radioButtonComprasano_clicked()
{
    if (!modeloCompras) return;
    modeloCompras->clear();

    QSqlQuery q;
    // Priorizar consulta a la Nube centralizada si está disponible
    if (m_idTienda > 0 && QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        q = QSqlQuery(dbNube);
        q.prepare("SELECT YEAR(p.fechaPedido), SUM(l.cantidad), SUM(l.bonificacion) "
                  "FROM lineaspedido_nube l "
                  "JOIN pedidos_nube p ON l.id_tienda = p.id_tienda AND l.nDocumento = p.npedido "
                  "WHERE l.cod = :cod AND l.id_tienda = :idTienda "
                  "GROUP BY YEAR(p.fechaPedido) DESC");
        q.bindValue(":cod", codigo);
        q.bindValue(":idTienda", m_idTienda);
    } else {
        q = QSqlQuery(db);
        q.prepare("SELECT YEAR(pedidos.fechaPedido) , sum(cantidad) , "
                  "sum(bonificacion) FROM lineaspedido JOIN pedidos ON nDocumento = "
                  "pedidos.npedido WHERE cod = ? GROUP BY "
                  "YEAR(pedidos.fechaPedido) DESC");
        q.bindValue(0, codigo);
    }

    if (!q.exec()) qDebug() << "comprasVentasRemoto::ComprasAno:" << q.lastError().text();
    modeloCompras->setQuery(q);
    ui->tableViewCompras->setModel(modeloCompras);
    ui->tableViewCompras->resizeColumnsToContents();
}
