#include "prepararpedidos.h"
#include "ui_prepararpedidos.h"
#include "syncmanager.h"
#include "configuracion.h"
#include "dialoganadirapedido.h"
#include "gestionpedidos.h"
#include <QMessageBox>
#include <QHeaderView>
#include <QTableWidgetItem>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QProcess>
#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
/**
 * @brief Elemento de tabla con soporte para ordenación numérica estricta.
 */
class NumericTableWidgetItem : public QTableWidgetItem {
public:
    explicit NumericTableWidgetItem(double valor, int decimales = 0, const QString &sufijo = QString())
        : QTableWidgetItem()
    {
        setData(Qt::UserRole, valor);
        if (decimales == 0) {
            setText(QString::number(valor, 'f', 0) + (sufijo.isEmpty() ? "" : " " + sufijo));
        } else {
            setText(QString::number(valor, 'f', decimales) + (sufijo.isEmpty() ? "" : " " + sufijo));
        }
        setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
    }

    bool operator<(const QTableWidgetItem &other) const override {
        QVariant v1 = data(Qt::UserRole);
        QVariant v2 = other.data(Qt::UserRole);
        if (v1.isValid() && v2.isValid()) {
            return v1.toDouble() < v2.toDouble();
        }
        return QTableWidgetItem::operator<(other);
    }
};

/**
 * @brief Constructor del diálogo PrepararPedidos.
 * Inicializa componentes, fechas por defecto ("yyyy-MM-dd"), tiendas y proveedores.
 */
PrepararPedidos::PrepararPedidos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::PrepararPedidos)
    , base(new baseDatos())
    , m_nubeDisponible(false)
    , m_codSeleccionado("")
    , m_descSeleccionada("")
{
    ui->setupUi(this);

    // Configurar fechas iniciales: Este Mes
    QDate hoy = QDate::currentDate();
    QDate primerDiaMes(hoy.year(), hoy.month(), 1);
    QDate ultimoDiaMes(hoy.year(), hoy.month(), hoy.daysInMonth());

    ui->dateEditDesde->setDisplayFormat("yyyy-MM-dd");
    ui->dateEditHasta->setDisplayFormat("yyyy-MM-dd");
    ui->dateEditDesde->setDate(primerDiaMes);
    ui->dateEditHasta->setDate(ultimoDiaMes);

    // Inicializar el diccionario de tiendas
    inicializarTiendas();

    // Cargar proveedores en el desplegable
    cargarProveedores();

    // Comprobar disponibilidad de la base de datos consolidada en la Nube
    verificarConexionNube();

    // Configuración visual de las tablas
    ui->tableWidgetPrincipal->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidgetPrincipal->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidgetTiendas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetHistCompras->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetHistVentas->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    // Ajustar proporción del splitter (70% principal, 30% panel lateral)
    ui->splitter->setSizes(QList<int>() << 850 << 450);

    // Si hay proveedores, ejecutar consulta inicial
    if (ui->comboBoxProveedor->count() > 0) {
        consultarDatos();
    }
}

/**
 * @brief Destructor del diálogo PrepararPedidos.
 */
PrepararPedidos::~PrepararPedidos()
{
    delete base;
    delete ui;
}

/**
 * @brief Inicializa el mapeo de nombres de tiendas y rellena el combo de tiendas.
 */
extern Configuracion *conf;

void PrepararPedidos::inicializarTiendas()
{
    m_nombresTiendas.clear();
    ui->comboBoxTienda->clear();
    ui->comboBoxTienda->addItem("Todas las tiendas", QVariant(0));

    QString connLocal = conf ? conf->getConexionLocal() : "";
    QSqlDatabase db = QSqlDatabase::contains(connLocal) ? QSqlDatabase::database(connLocal) : QSqlDatabase::database();

    if (db.isOpen()) {
        QSqlQuery q(db);
        if (q.exec("SELECT id, nombre FROM tiendas ORDER BY id")) {
            while (q.next()) {
                int id = q.value(0).toInt();
                QString nombre = q.value(1).toString().trimmed();
                m_nombresTiendas[id] = nombre;
                ui->comboBoxTienda->addItem(QString("%1 (ID %2)").arg(nombre).arg(id), QVariant(id));
            }
        }
    }

    if (m_nombresTiendas.isEmpty()) {
        m_nombresTiendas[1] = "Emeicjac";
        m_nombresTiendas[2] = "Casablanca";
        m_nombresTiendas[3] = "Cervantes";
        ui->comboBoxTienda->addItem("Emeicjac (ID 1)", QVariant(1));
        ui->comboBoxTienda->addItem("Casablanca (ID 2)", QVariant(2));
        ui->comboBoxTienda->addItem("Cervantes (ID 3)", QVariant(3));
    }
}

/**
 * @brief Obtiene el nombre amigable de una tienda dado su identificador.
 */
QString PrepararPedidos::obtenerNombreTienda(int idTienda)
{
    if (m_nombresTiendas.contains(idTienda)) return m_nombresTiendas.value(idTienda);
    if (idTienda == 0) return "Red Global";
    return QString("Tienda %1").arg(idTienda);
}

/**
 * @brief Comprueba y establece la conexión con la base de datos de la Nube.
 */
void PrepararPedidos::verificarConexionNube()
{
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        m_nubeDisponible = true;
        ui->lblEstadoNube->setText("☁️ Nube Conectada");
        ui->lblEstadoNube->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        m_nubeDisponible = false;
        ui->lblEstadoNube->setText("💾 Modo Local");
        ui->lblEstadoNube->setStyleSheet("color: #d35400; font-weight: bold;");
    }
}

/**
 * @brief Devuelve la base de datos adecuada para la consulta (Nube si está activa, o Local).
 */
QSqlDatabase PrepararPedidos::obtenerBaseDatosConsulta()
{
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) &&
        QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        return QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    }
    return QSqlDatabase::database(conf->getConexionLocal());
}

/**
 * @brief Carga todos los proveedores registrados en el QComboBox.
 */
void PrepararPedidos::cargarProveedores()
{
    ui->comboBoxProveedor->blockSignals(true);
    ui->comboBoxProveedor->clear();
    QSqlDatabase db = obtenerBaseDatosConsulta();
    QSqlQuery q(db);
    q.prepare("SELECT idProveedor, nombre FROM proveedores ORDER BY nombre ASC");
    if (q.exec()) {
        while (q.next()) {
            int id = q.value(0).toInt();
            QString nombre = q.value(1).toString().trimmed();
            if (!nombre.isEmpty()) {
                ui->comboBoxProveedor->addItem(QString("%1 (%2)").arg(nombre).arg(id), QVariant(id));
            }
        }
    }
    ui->comboBoxProveedor->blockSignals(false);
}

/**
 * @brief Slot para consultar datos con los filtros seleccionados.
 */
void PrepararPedidos::on_pushButtonConsultar_clicked()
{
    consultarDatos();
}

/**
 * @brief Slot cuando cambia el proveedor en el combo.
 */
void PrepararPedidos::on_comboBoxProveedor_currentIndexChanged(int)
{
    consultarDatos();
}

/**
 * @brief Slot cuando cambia el tipo de agrupación.
 */
void PrepararPedidos::on_comboBoxAgrupacion_currentIndexChanged(int)
{
    consultarDatos();
}

/**
 * @brief Slot cuando cambia la cantidad de días de cobertura deseada.
 * Recalcula al instante las unidades sugeridas sin repetir la consulta SQL.
 */
void PrepararPedidos::on_spinBoxDiasCobertura_valueChanged(int diasCobertura)
{
    QDate hoy = QDate::currentDate();
    QDate fDesde = ui->dateEditDesde->date();
    QDate fHasta = ui->dateEditHasta->date();
    QDate fFinEfectivo = (fHasta > hoy) ? hoy : fHasta;
    qint64 diasPeriodo = qMax((qint64)1, fDesde.daysTo(fFinEfectivo) + 1);

    for (int i = 0; i < m_listaArticulos.size(); ++i) {
        ItemAnalisis &item = m_listaArticulos[i];
        double ventaDiaria = (diasPeriodo > 0) ? (item.udsVendidas / (double)diasPeriodo) : 0.0;
        double necesidad = ventaDiaria * diasCobertura;
        double deficit = necesidad - item.stockActual;
        item.sugerido = qMax(0.0, std::ceil(deficit));
    }

    rellenarTablaPrincipal();
}

/**
 * @brief Slot para alternar entre mostrar u ocultar artículos sin ventas en el periodo.
 */
void PrepararPedidos::on_checkBoxMostrarSinVentas_toggled(bool)
{
    rellenarTablaPrincipal();
}

/**
 * @brief Consulta las compras y ventas consolidadas en la Nube y alimenta la tabla.
 */
void PrepararPedidos::consultarDatos()
{
    if (ui->comboBoxProveedor->currentIndex() == -1) return;

    QApplication::setOverrideCursor(Qt::WaitCursor);

    verificarConexionNube();
    QSqlDatabase db = obtenerBaseDatosConsulta();
    if (!db.isOpen()) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(this, "Aviso", "No hay conexión abierta con la base de datos.");
        return;
    }

    int idProveedor = ui->comboBoxProveedor->currentData().toInt();
    QString fDesde = ui->dateEditDesde->date().toString("yyyy-MM-dd");
    QString fHasta = ui->dateEditHasta->date().toString("yyyy-MM-dd");
    int modoAgrupacion = ui->comboBoxAgrupacion->currentIndex();
    int filtroTienda = ui->comboBoxTienda->currentData().toInt();

    m_listaArticulos.clear();

    struct Acumulado {
        QString cod;
        QString descripcion;
        QString periodo;
        double udsCompradas = 0.0;
        double comprasTotal = 0.0;
        double costoAcum = 0.0;
        double udsVendidas = 0.0;
        double ventasTotal = 0.0;
        double pvpAcum = 0.0;
        double stockActual = 0.0;
        double minStock = 0.0;
        double maxStock = 0.0;
    };
    QMap<QString, Acumulado> mapa;
    QMap<QString, QString> catalogoProveedor;

    // ── 1. Catálogo de artículos del proveedor ────────────────────────────────
    if (m_nubeDisponible) {
        QSqlQuery qCod(db);
        qCod.prepare("SELECT DISTINCT cod, MAX(descripcion) FROM lineaspedido_nube WHERE idProveedor = ? AND cod != '' GROUP BY cod");
        qCod.bindValue(0, idProveedor);
        if (qCod.exec()) {
            while (qCod.next()) {
                QString c = qCod.value(0).toString().trimmed();
                QString d = qCod.value(1).toString().trimmed();
                if (!c.isEmpty()) {
                    catalogoProveedor[c] = d;
                }
            }
        }
    } else {
        QSqlQuery qCod(db);
        qCod.prepare("SELECT DISTINCT lp.cod, MAX(lp.descripcion) FROM lineaspedido lp "
                     "JOIN pedidos p ON lp.nDocumento = p.npedido "
                     "WHERE p.idProveedor = ? AND lp.cod != '' GROUP BY lp.cod");
        qCod.bindValue(0, idProveedor);
        if (qCod.exec()) {
            while (qCod.next()) {
                QString c = qCod.value(0).toString().trimmed();
                QString d = qCod.value(1).toString().trimmed();
                if (!c.isEmpty()) {
                    catalogoProveedor[c] = d;
                }
            }
        }
    }

    // Precargar catálogo en modo agrupación por artículo
    if (modoAgrupacion == 0) {
        for (auto it = catalogoProveedor.begin(); it != catalogoProveedor.end(); ++it) {
            Acumulado &ac = mapa[it.key()];
            ac.cod = it.key();
            ac.descripcion = it.value();
        }
    }

    // ── 2. Consulta de Compras en el periodo ──────────────────────────────────
    QString sqlCompras = "";
    if (m_nubeDisponible) {
        if (modoAgrupacion == 0) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, '' as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido_nube lp "
                "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
                "WHERE lp.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' %4 "
                "GROUP BY lp.cod"
            ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND p.id_tienda = %1").arg(filtroTienda) : "");
        } else if (modoAgrupacion == 1) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, DATE_FORMAT(p.fechaPedido, '%Y-%m') as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido_nube lp "
                "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
                "WHERE lp.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' %4 "
                "GROUP BY lp.cod, DATE_FORMAT(p.fechaPedido, '%Y-%m')"
            ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND p.id_tienda = %1").arg(filtroTienda) : "");
        } else if (modoAgrupacion == 2) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, YEAR(p.fechaPedido) as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido_nube lp "
                "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
                "WHERE lp.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' %4 "
                "GROUP BY lp.cod, YEAR(p.fechaPedido)"
            ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND p.id_tienda = %1").arg(filtroTienda) : "");
        } else {
            sqlCompras = QString(
                "SELECT 'PROVEEDOR' as cod, 'TOTAL COMPRAS' as descr, DATE_FORMAT(p.fechaPedido, '%Y-%m') as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido_nube lp "
                "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
                "WHERE lp.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' %4 "
                "GROUP BY DATE_FORMAT(p.fechaPedido, '%Y-%m')"
            ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND p.id_tienda = %1").arg(filtroTienda) : "");
        }
    } else {
        if (modoAgrupacion == 0) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, '' as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido lp "
                "JOIN pedidos p ON lp.nDocumento = p.npedido AND lp.idProveedor = p.idProveedor "
                "WHERE p.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' "
                "GROUP BY lp.cod"
            ).arg(idProveedor).arg(fDesde, fHasta);
        } else if (modoAgrupacion == 1) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, DATE_FORMAT(p.fechaPedido, '%Y-%m') as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido lp "
                "JOIN pedidos p ON lp.nDocumento = p.npedido AND lp.idProveedor = p.idProveedor "
                "WHERE p.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' "
                "GROUP BY lp.cod, DATE_FORMAT(p.fechaPedido, '%Y-%m')"
            ).arg(idProveedor).arg(fDesde, fHasta);
        } else if (modoAgrupacion == 2) {
            sqlCompras = QString(
                "SELECT lp.cod, MAX(lp.descripcion) as descr, YEAR(p.fechaPedido) as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido lp "
                "JOIN pedidos p ON lp.nDocumento = p.npedido AND lp.idProveedor = p.idProveedor "
                "WHERE p.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' "
                "GROUP BY lp.cod, YEAR(p.fechaPedido)"
            ).arg(idProveedor).arg(fDesde, fHasta);
        } else {
            sqlCompras = QString(
                "SELECT 'PROVEEDOR' as cod, 'TOTAL COMPRAS' as descr, DATE_FORMAT(p.fechaPedido, '%Y-%m') as periodo, "
                "COALESCE(SUM(lp.cantidad + lp.bonificacion), 0) as uds, "
                "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) as total, "
                "AVG(lp.costo) as costo_med "
                "FROM lineaspedido lp "
                "JOIN pedidos p ON lp.nDocumento = p.npedido AND lp.idProveedor = p.idProveedor "
                "WHERE p.idProveedor = %1 AND p.fechaPedido >= '%2' AND p.fechaPedido <= '%3' "
                "GROUP BY DATE_FORMAT(p.fechaPedido, '%Y-%m')"
            ).arg(idProveedor).arg(fDesde, fHasta);
        }
    }

    QSqlQuery qCompras(db);
    if (qCompras.exec(sqlCompras)) {
        while (qCompras.next()) {
            QString cod = qCompras.value("cod").toString().trimmed();
            QString desc = qCompras.value("descr").toString().trimmed();
            QString periodo = qCompras.value("periodo").toString();
            double uds = qCompras.value("uds").toDouble();
            double tot = qCompras.value("total").toDouble();
            double costoMed = qCompras.value("costo_med").toDouble();

            QString clave = (modoAgrupacion == 0) ? cod : QString("%1|%2").arg(cod, periodo);
            Acumulado &ac = mapa[clave];
            ac.cod = cod;
            if (ac.descripcion.isEmpty()) ac.descripcion = desc;
            ac.periodo = periodo;
            ac.udsCompradas += uds;
            ac.comprasTotal += tot;
            ac.costoAcum = costoMed;
        }
    }

    // ── 3. Consulta de Ventas (en una única consulta SQL indexada) ─────────────
    if (!catalogoProveedor.isEmpty()) {
        QString sqlVentas = "";
        if (m_nubeDisponible) {
            if (modoAgrupacion == 0) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, '' as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket_nube lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido_nube WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' %4 "
                    "GROUP BY lt.cod"
                ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND lt.id_tienda = %1").arg(filtroTienda) : "");
            } else if (modoAgrupacion == 1) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, DATE_FORMAT(lt.fecha, '%Y-%m') as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket_nube lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido_nube WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' %4 "
                    "GROUP BY lt.cod, DATE_FORMAT(lt.fecha, '%Y-%m')"
                ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND lt.id_tienda = %1").arg(filtroTienda) : "");
            } else if (modoAgrupacion == 2) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, YEAR(lt.fecha) as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket_nube lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido_nube WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' %4 "
                    "GROUP BY lt.cod, YEAR(lt.fecha)"
                ).arg(idProveedor).arg(fDesde, fHasta, filtroTienda > 0 ? QString("AND lt.id_tienda = %1").arg(filtroTienda) : "");
            }
        } else {
            if (modoAgrupacion == 0) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, '' as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' "
                    "GROUP BY lt.cod"
                ).arg(idProveedor).arg(fDesde, fHasta);
            } else if (modoAgrupacion == 1) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, DATE_FORMAT(lt.fecha, '%Y-%m') as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' "
                    "GROUP BY lt.cod, DATE_FORMAT(lt.fecha, '%Y-%m')"
                ).arg(idProveedor).arg(fDesde, fHasta);
            } else if (modoAgrupacion == 2) {
                sqlVentas = QString(
                    "SELECT lt.cod, MAX(lt.descripcion) as descr, YEAR(lt.fecha) as periodo, "
                    "COALESCE(SUM(lt.cantidad), 0) as uds, "
                    "COALESCE(SUM(lt.totallinea), 0) as total, "
                    "AVG(lt.precio) as pvp_med "
                    "FROM lineasticket lt "
                    "WHERE lt.cod IN (SELECT DISTINCT cod FROM lineaspedido WHERE idProveedor = %1 AND cod != '') "
                    "AND lt.fecha >= '%2' AND lt.fecha <= '%3' "
                    "GROUP BY lt.cod, YEAR(lt.fecha)"
                ).arg(idProveedor).arg(fDesde, fHasta);
            }
        }

        QSqlQuery qVentas(db);
        if (qVentas.exec(sqlVentas)) {
            while (qVentas.next()) {
                QString cod = qVentas.value("cod").toString().trimmed();
                QString desc = qVentas.value("descr").toString().trimmed();
                QString periodo = qVentas.value("periodo").toString();
                double uds = qVentas.value("uds").toDouble();
                double tot = qVentas.value("total").toDouble();
                double pvpM = qVentas.value("pvp_med").toDouble();

                QString clave = (modoAgrupacion == 0) ? cod : QString("%1|%2").arg(cod, periodo);
                Acumulado &ac = mapa[clave];
                ac.cod = cod;
                if (ac.descripcion.isEmpty()) ac.descripcion = desc;
                ac.periodo = periodo;
                ac.udsVendidas += uds;
                ac.ventasTotal += tot;
                ac.pvpAcum = pvpM;
            }
        }

        // ── 4. Stock consolidado (en una única consulta SQL) ──────────────────────
        if (m_nubeDisponible) {
            QString sqlStock = QString(
                "SELECT s.cod, COALESCE(SUM(s.cantidad), 0) as stock_tot "
                "FROM stock_tiendas_nube s "
                "WHERE s.cod IN (SELECT DISTINCT cod FROM lineaspedido_nube WHERE idProveedor = %1 AND cod != '') %2 "
                "GROUP BY s.cod"
            ).arg(idProveedor).arg(filtroTienda > 0 ? QString("AND s.id_tienda = %1").arg(filtroTienda) : "");

            QSqlQuery qStk(db);
            if (qStk.exec(sqlStock)) {
                while (qStk.next()) {
                    QString cod = qStk.value("cod").toString().trimmed();
                    double stk = qStk.value("stock_tot").toDouble();
                    if (modoAgrupacion == 0) {
                        if (mapa.contains(cod)) mapa[cod].stockActual = stk;
                    } else {
                        for (auto it = mapa.begin(); it != mapa.end(); ++it) {
                            if (it.value().cod == cod) it.value().stockActual = stk;
                        }
                    }
                }
            }
        } else {
            QString sqlStockLocal = QString(
                "SELECT a.cod, COALESCE(SUM(l.cantidad), 0) as stk "
                "FROM articulos a "
                "LEFT JOIN lotes l ON a.cod = l.ean "
                "WHERE a.cod IN (SELECT DISTINCT cod FROM lineaspedido WHERE idProveedor = %1 AND cod != '') "
                "GROUP BY a.cod"
            ).arg(idProveedor);

            QSqlQuery qStk(db);
            if (qStk.exec(sqlStockLocal)) {
                while (qStk.next()) {
                    QString cod = qStk.value("cod").toString().trimmed();
                    double stk = qStk.value("stk").toDouble();
                    if (modoAgrupacion == 0) {
                        if (mapa.contains(cod)) mapa[cod].stockActual = stk;
                    } else {
                        for (auto it = mapa.begin(); it != mapa.end(); ++it) {
                            if (it.value().cod == cod) it.value().stockActual = stk;
                        }
                    }
                }
            }
        }
    }

    // ── 5. Convertir a lista de ItemAnalisis y calcular Sugerencias ───────────
    QDate hoy = QDate::currentDate();
    QDate fFinEfectivo = (ui->dateEditHasta->date() > hoy) ? hoy : ui->dateEditHasta->date();
    qint64 diasPeriodo = qMax((qint64)1, ui->dateEditDesde->date().daysTo(fFinEfectivo) + 1);
    int diasCobertura = ui->spinBoxDiasCobertura->value();

    for (auto it = mapa.begin(); it != mapa.end(); ++it) {
        const Acumulado &ac = it.value();
        ItemAnalisis item;
        item.cod = ac.cod;
        item.descripcion = ac.descripcion.isEmpty() ? catalogoProveedor.value(ac.cod, "Sin descripción") : ac.descripcion;
        item.periodo = ac.periodo;
        item.stockActual = ac.stockActual;
        item.minStock = ac.minStock;
        item.maxStock = ac.maxStock;
        item.udsCompradas = ac.udsCompradas;
        item.comprasTotal = ac.comprasTotal;
        item.costoMedio = (ac.udsCompradas > 0) ? (ac.comprasTotal / ac.udsCompradas) : ac.costoAcum;
        item.udsVendidas = ac.udsVendidas;
        item.ventasTotal = ac.ventasTotal;
        item.pvpMedio = (ac.udsVendidas > 0) ? (ac.ventasTotal / ac.udsVendidas) : ac.pvpAcum;

        double ventaDiaria = (diasPeriodo > 0) ? (item.udsVendidas / (double)diasPeriodo) : 0.0;
        double necesidadCobertura = ventaDiaria * diasCobertura;
        double deficit = necesidadCobertura - item.stockActual;
        item.sugerido = qMax(0.0, std::ceil(deficit));

        m_listaArticulos.append(item);
    }

    rellenarTablaPrincipal();
    QApplication::restoreOverrideCursor();
}

/**
 * @brief Rellena el QTableWidget principal con los datos analíticos.
 */
void PrepararPedidos::rellenarTablaPrincipal()
{
    ui->tableWidgetPrincipal->blockSignals(true);
    ui->tableWidgetPrincipal->setSortingEnabled(false);
    ui->tableWidgetPrincipal->clear();

    int modoAgrupacion = ui->comboBoxAgrupacion->currentIndex();
    int diasCob = ui->spinBoxDiasCobertura->value();
    QString colSugerido = QString("Sugerido (%1d)").arg(diasCob);
    QStringList headers;

    if (modoAgrupacion == 0) {
        headers << "Código" << "Descripción" << "Stock Red"
                << "Uds Vendidas" << "Total Ventas (€)" << "PVP Medio (€)"
                << "Uds Compradas" << "Total Compras (€)" << "Costo Medio (€)"
                << "Balance (V-C)" << "Margen (€)" << "Margen (%)" << colSugerido;
    } else {
        headers << "Periodo" << "Código" << "Descripción" << "Stock Red"
                << "Uds Vendidas" << "Total Ventas (€)" << "PVP Medio (€)"
                << "Uds Compradas" << "Total Compras (€)" << "Costo Medio (€)"
                << "Balance (V-C)" << "Margen (€)" << "Margen (%)" << colSugerido;
    }

    ui->tableWidgetPrincipal->setColumnCount(headers.size());
    ui->tableWidgetPrincipal->setHorizontalHeaderLabels(headers);
    ui->tableWidgetPrincipal->setRowCount(m_listaArticulos.size());

    QString filtro = ui->lineEditFiltro->text().trimmed().toLower();
    bool mostrarSinVentas = ui->checkBoxMostrarSinVentas->isChecked();
    QDate hoy = QDate::currentDate();
    QDate fFinEfectivo = (ui->dateEditHasta->date() > hoy) ? hoy : ui->dateEditHasta->date();
    qint64 diasPeriodo = qMax((qint64)1, ui->dateEditDesde->date().daysTo(fFinEfectivo) + 1);

    for (int i = 0; i < m_listaArticulos.size(); ++i) {
        const ItemAnalisis &item = m_listaArticulos.at(i);

        // Filtro por ventas: por defecto no mostrar artículos que no hayan tenido ventas en el periodo
        if (!mostrarSinVentas && modoAgrupacion != 3 && item.cod != "PROVEEDOR") {
            if (item.udsVendidas <= 0.0) {
                ui->tableWidgetPrincipal->setRowHidden(i, true);
                continue;
            }
        }

        // Filtro rápido de texto
        if (!filtro.isEmpty()) {
            if (!item.cod.toLower().contains(filtro) &&
                !item.descripcion.toLower().contains(filtro) &&
                !item.periodo.toLower().contains(filtro)) {
                ui->tableWidgetPrincipal->setRowHidden(i, true);
                continue;
            }
        }
        ui->tableWidgetPrincipal->setRowHidden(i, false);

        int col = 0;
        if (modoAgrupacion != 0) {
            QTableWidgetItem *itPeriodo = new QTableWidgetItem(item.periodo);
            itPeriodo->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetPrincipal->setItem(i, col++, itPeriodo);
        }

        // 1. Código
        QTableWidgetItem *itCod = new QTableWidgetItem(item.cod);
        itCod->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidgetPrincipal->setItem(i, col++, itCod);

        // 2. Descripción
        QTableWidgetItem *itDesc = new QTableWidgetItem(item.descripcion);
        itDesc->setTextAlignment(Qt::AlignLeft | Qt::AlignVCenter);
        ui->tableWidgetPrincipal->setItem(i, col++, itDesc);

        // 3. Stock Actual
        NumericTableWidgetItem *itStock = new NumericTableWidgetItem(item.stockActual, 0);
        if (item.stockActual <= 0) itStock->setForeground(QBrush(QColor("#c0392b")));
        ui->tableWidgetPrincipal->setItem(i, col++, itStock);

        // 4. Uds Vendidas
        NumericTableWidgetItem *itUdsV = new NumericTableWidgetItem(item.udsVendidas, 0);
        ui->tableWidgetPrincipal->setItem(i, col++, itUdsV);

        // 5. Total Ventas (€)
        NumericTableWidgetItem *itTotV = new NumericTableWidgetItem(item.ventasTotal, 2, "€");
        itTotV->setForeground(QBrush(QColor("#27ae60")));
        ui->tableWidgetPrincipal->setItem(i, col++, itTotV);

        // 6. PVP Medio (€)
        NumericTableWidgetItem *itPvpMed = new NumericTableWidgetItem(item.pvpMedio, 2, "€");
        ui->tableWidgetPrincipal->setItem(i, col++, itPvpMed);

        // 7. Uds Compradas
        NumericTableWidgetItem *itUdsC = new NumericTableWidgetItem(item.udsCompradas, 0);
        ui->tableWidgetPrincipal->setItem(i, col++, itUdsC);

        // 8. Total Compras (€)
        NumericTableWidgetItem *itTotC = new NumericTableWidgetItem(item.comprasTotal, 2, "€");
        ui->tableWidgetPrincipal->setItem(i, col++, itTotC);

        // 9. Costo Medio (€)
        NumericTableWidgetItem *itCostoMed = new NumericTableWidgetItem(item.costoMedio, 2, "€");
        ui->tableWidgetPrincipal->setItem(i, col++, itCostoMed);

        // 10. Balance Rotación (Vendidas - Compradas)
        double balance = item.udsVendidas - item.udsCompradas;
        NumericTableWidgetItem *itBal = new NumericTableWidgetItem(balance, 0);
        if (balance > 0) itBal->setForeground(QBrush(QColor("#2980b9")));
        ui->tableWidgetPrincipal->setItem(i, col++, itBal);

        // 11. Margen (€)
        double margenEur = item.ventasTotal - (item.udsVendidas * item.costoMedio);
        NumericTableWidgetItem *itMargEur = new NumericTableWidgetItem(margenEur, 2, "€");
        ui->tableWidgetPrincipal->setItem(i, col++, itMargEur);

        // 12. Margen (%)
        double margenPct = (item.ventasTotal > 0) ? (margenEur / item.ventasTotal) * 100.0 : 0.0;
        NumericTableWidgetItem *itMargPct = new NumericTableWidgetItem(margenPct, 1, "%");
        ui->tableWidgetPrincipal->setItem(i, col++, itMargPct);

        // 13. Sugerido (Uds) para la cobertura elegida
        NumericTableWidgetItem *itSug = new NumericTableWidgetItem(item.sugerido, 0);
        itSug->setFont(QFont(ui->tableWidgetPrincipal->font().family(), -1, QFont::Bold));
        if (item.sugerido > 0) itSug->setForeground(QBrush(QColor("#e67e22")));

        double ventaDiaria = (diasPeriodo > 0) ? (item.udsVendidas / (double)diasPeriodo) : 0.0;
        double necesidad = ventaDiaria * diasCob;
        itSug->setToolTip(QString("Ritmo ventas: %1 uds/día\nNecesidad para %2 días: %3 uds\nStock actual en red: %4 uds\nPropuesta pedido: %5 uds")
                          .arg(QString::number(ventaDiaria, 'f', 2))
                          .arg(diasCob)
                          .arg(QString::number(necesidad, 'f', 1))
                          .arg(QString::number(item.stockActual, 'f', 0))
                          .arg(QString::number(item.sugerido, 'f', 0)));
        ui->tableWidgetPrincipal->setItem(i, col++, itSug);
    }

    ui->tableWidgetPrincipal->setSortingEnabled(true);
    ui->tableWidgetPrincipal->blockSignals(false);
    actualizarTotales();

    // Si hay filas visibles, seleccionar la primera para actualizar el panel lateral
    bool filaSeleccionada = false;
    for (int r = 0; r < ui->tableWidgetPrincipal->rowCount(); ++r) {
        if (!ui->tableWidgetPrincipal->isRowHidden(r)) {
            ui->tableWidgetPrincipal->selectRow(r);
            filaSeleccionada = true;
            break;
        }
    }
    if (!filaSeleccionada) {
        ui->tableWidgetPrincipal->clearSelection();
        mostrarDetalleArticulo("", "");
    }
}

/**
 * @brief Actualiza la barra inferior de totales y KPIs.
 */
void PrepararPedidos::actualizarTotales()
{
    double totUdsC = 0.0;
    double totCompEur = 0.0;
    double totUdsV = 0.0;
    double totVentEur = 0.0;
    double totSugerido = 0.0;
    int visibles = 0;

    for (int i = 0; i < m_listaArticulos.size(); ++i) {
        if (ui->tableWidgetPrincipal->isRowHidden(i)) continue;
        visibles++;
        const ItemAnalisis &item = m_listaArticulos.at(i);
        totUdsC += item.udsCompradas;
        totCompEur += item.comprasTotal;
        totUdsV += item.udsVendidas;
        totVentEur += item.ventasTotal;
        totSugerido += item.sugerido;
    }

    double margenGlobalEur = totVentEur - totCompEur;
    double margenGlobalPct = (totVentEur > 0) ? (margenGlobalEur / totVentEur) * 100.0 : 0.0;

    ui->lblTotalArticulos->setText(QString("Artículos: %1").arg(visibles));
    ui->lblTotalUdsVentas->setText(QString("Ventas Uds: %1").arg(totUdsV, 0, 'f', 0));
    ui->lblTotalVentasEur->setText(QString("Ventas: %1 €").arg(QString::number(totVentEur, 'f', 2)));
    ui->lblTotalUdsCompras->setText(QString("Compras Uds: %1").arg(totUdsC, 0, 'f', 0));
    ui->lblTotalComprasEur->setText(QString("Compras: %1 €").arg(QString::number(totCompEur, 'f', 2)));
    ui->lblMargenTotal->setText(QString("Margen: %1 € (%2%)")
                               .arg(QString::number(margenGlobalEur, 'f', 2))
                               .arg(QString::number(margenGlobalPct, 'f', 1)));
    ui->lblSugeridoTotal->setText(QString("Sugerido Total: %1 uds").arg(totSugerido, 0, 'f', 0));
}

/**
 * @brief Slot cuando cambia la selección en la tabla principal.
 */
void PrepararPedidos::on_tableWidgetPrincipal_itemSelectionChanged()
{
    int fila = ui->tableWidgetPrincipal->currentRow();
    if (fila < 0 || fila >= ui->tableWidgetPrincipal->rowCount()) return;

    int modoAgrupacion = ui->comboBoxAgrupacion->currentIndex();
    int colCod = (modoAgrupacion == 0) ? 0 : 1;
    int colDesc = (modoAgrupacion == 0) ? 1 : 2;

    QTableWidgetItem *itCod = ui->tableWidgetPrincipal->item(fila, colCod);
    QTableWidgetItem *itDesc = ui->tableWidgetPrincipal->item(fila, colDesc);

    if (itCod && itDesc) {
        m_codSeleccionado = itCod->text().trimmed();
        m_descSeleccionada = itDesc->text().trimmed();
        mostrarDetalleArticulo(m_codSeleccionado, m_descSeleccionada);
    }
}

/**
 * @brief Slot para el doble clic en una fila de la tabla principal.
 */
void PrepararPedidos::on_tableWidgetPrincipal_cellDoubleClicked(int row, int)
{
    if (row < 0) return;
    on_tableWidgetPrincipal_itemSelectionChanged();
}

/**
 * @brief Muestra el desglose por tienda, historial de compras y ventas en el panel lateral.
 */
void PrepararPedidos::mostrarDetalleArticulo(const QString &cod, const QString &desc)
{
    ui->lblArticuloSeleccionado->setText(QString("📦 %1").arg(desc));
    ui->lblDetalleInfo->setText(QString("Código: %1  |  Proveedor: %2")
                                .arg(cod, ui->comboBoxProveedor->currentText()));

    if (cod.isEmpty() || cod == "PROVEEDOR") {
        ui->tableWidgetTiendas->clear();
        ui->tableWidgetHistCompras->clear();
        ui->tableWidgetHistVentas->clear();
        return;
    }

    QSqlDatabase db = obtenerBaseDatosConsulta();
    QString fDesde = ui->dateEditDesde->date().toString("yyyy-MM-dd");
    QString fHasta = ui->dateEditHasta->date().toString("yyyy-MM-dd");

    // ── 1. Desglose por Tienda ───────────────────────────────────────────────
    ui->tableWidgetTiendas->setRowCount(0);
    ui->tableWidgetTiendas->setColumnCount(6);
    ui->tableWidgetTiendas->setHorizontalHeaderLabels(
        QStringList() << "Tienda" << "Stock" << "Ventas Uds" << "Ventas (€)" << "Compras Uds" << "Compras (€)");

    QList<int> tiendasIds = m_nombresTiendas.keys();
    std::sort(tiendasIds.begin(), tiendasIds.end());
    ui->tableWidgetTiendas->setRowCount(tiendasIds.size());

    QMap<int, double> stkPorTienda;
    QMap<int, double> compUdsPorTienda;
    QMap<int, double> compEurPorTienda;
    QMap<int, double> ventUdsPorTienda;
    QMap<int, double> ventEurPorTienda;

    if (m_nubeDisponible) {
        // Stock agrupado por tienda
        QSqlQuery qStk(db);
        qStk.prepare("SELECT id_tienda, COALESCE(SUM(cantidad), 0) FROM stock_tiendas_nube WHERE cod = ? GROUP BY id_tienda");
        qStk.bindValue(0, cod);
        if (qStk.exec()) {
            while (qStk.next()) stkPorTienda[qStk.value(0).toInt()] = qStk.value(1).toDouble();
        }

        // Compras agrupadas por tienda
        QSqlQuery qC(db);
        qC.prepare("SELECT lp.id_tienda, COALESCE(SUM(lp.cantidad + lp.bonificacion), 0), "
                   "COALESCE(SUM(CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END), 0) "
                   "FROM lineaspedido_nube lp "
                   "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
                   "WHERE lp.cod = ? AND p.fechaPedido >= ? AND p.fechaPedido <= ? "
                   "GROUP BY lp.id_tienda");
        qC.bindValue(0, cod);
        qC.bindValue(1, fDesde);
        qC.bindValue(2, fHasta);
        if (qC.exec()) {
            while (qC.next()) {
                int idT = qC.value(0).toInt();
                compUdsPorTienda[idT] = qC.value(1).toDouble();
                compEurPorTienda[idT] = qC.value(2).toDouble();
            }
        }

        // Ventas agrupadas por tienda
        QSqlQuery qV(db);
        qV.prepare("SELECT id_tienda, COALESCE(SUM(cantidad), 0), COALESCE(SUM(totallinea), 0) "
                   "FROM lineasticket_nube "
                   "WHERE cod = ? AND fecha >= ? AND fecha <= ? "
                   "GROUP BY id_tienda");
        qV.bindValue(0, cod);
        qV.bindValue(1, fDesde);
        qV.bindValue(2, fHasta);
        if (qV.exec()) {
            while (qV.next()) {
                int idT = qV.value(0).toInt();
                ventUdsPorTienda[idT] = qV.value(1).toDouble();
                ventEurPorTienda[idT] = qV.value(2).toDouble();
            }
        }
    } else {
        // Fallback: Si no hay nube, consultar stock en conexiones directas locales/remotas
        for (int idT : tiendasIds) {
            QString nombreT = obtenerNombreTienda(idT);
            if (QSqlDatabase::contains(nombreT) && QSqlDatabase::database(nombreT).isOpen()) {
                QSqlQuery qStk(QSqlDatabase::database(nombreT));
                qStk.prepare("SELECT COALESCE(SUM(cantidad), 0) FROM lotes WHERE ean = ?");
                qStk.bindValue(0, cod);
                if (qStk.exec() && qStk.next()) stkPorTienda[idT] = qStk.value(0).toDouble();
            } else if (db.isOpen()) {
                QSqlQuery qStk(db);
                qStk.prepare("SELECT COALESCE(SUM(cantidad), 0) FROM lotes WHERE ean = ?");
                qStk.bindValue(0, cod);
                if (qStk.exec() && qStk.next()) stkPorTienda[idT] = qStk.value(0).toDouble();
            }
        }
    }

    for (int tIdx = 0; tIdx < tiendasIds.size(); ++tIdx) {
        int idT = tiendasIds.at(tIdx);
        QString nombreT = obtenerNombreTienda(idT);

        double stkTienda = stkPorTienda.value(idT, 0.0);
        double compUds = compUdsPorTienda.value(idT, 0.0);
        double compEur = compEurPorTienda.value(idT, 0.0);
        double ventUds = ventUdsPorTienda.value(idT, 0.0);
        double ventEur = ventEurPorTienda.value(idT, 0.0);

        ui->tableWidgetTiendas->setItem(tIdx, 0, new QTableWidgetItem(nombreT));
        ui->tableWidgetTiendas->setItem(tIdx, 1, new NumericTableWidgetItem(stkTienda, 0));
        ui->tableWidgetTiendas->setItem(tIdx, 2, new NumericTableWidgetItem(ventUds, 0));
        ui->tableWidgetTiendas->setItem(tIdx, 3, new NumericTableWidgetItem(ventEur, 2, "€"));
        ui->tableWidgetTiendas->setItem(tIdx, 4, new NumericTableWidgetItem(compUds, 0));
        ui->tableWidgetTiendas->setItem(tIdx, 5, new NumericTableWidgetItem(compEur, 2, "€"));
    }

    // ── 2. Historial de Compras / Albaranes ───────────────────────────────────
    ui->tableWidgetHistCompras->blockSignals(true);
    ui->tableWidgetHistCompras->setRowCount(0);
    ui->tableWidgetHistCompras->setColumnCount(6);
    ui->tableWidgetHistCompras->setHorizontalHeaderLabels(
        QStringList() << "Fecha" << "Tienda" << "Albarán/Doc" << "Uds" << "Costo (€)" << "Total (€)");

    QString sqlHistC = "";
    if (m_nubeDisponible) {
        sqlHistC = QString(
            "SELECT p.fechaPedido, lp.id_tienda, lp.nDocumento, (lp.cantidad + lp.bonificacion) as uds, "
            "lp.costo, CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END as total "
            "FROM lineaspedido_nube lp "
            "JOIN pedidos_nube p ON lp.id_tienda = p.id_tienda AND lp.nDocumento = p.npedido "
            "WHERE lp.cod = '%1' "
            "ORDER BY p.fechaPedido DESC LIMIT 50"
        ).arg(cod);
    } else {
        sqlHistC = QString(
            "SELECT p.fechaPedido, 1 as id_tienda, lp.nDocumento, (lp.cantidad + lp.bonificacion) as uds, "
            "lp.costo, CASE WHEN lp.totalbase > 0 THEN lp.totalbase ELSE (lp.costo * lp.cantidad) END as total "
            "FROM lineaspedido lp "
            "JOIN pedidos p ON lp.nDocumento = p.npedido "
            "WHERE lp.cod = '%1' "
            "ORDER BY p.fechaPedido DESC LIMIT 50"
        ).arg(cod);
    }

    QSqlQuery qHc(db);
    if (qHc.exec(sqlHistC)) {
        int r = 0;
        while (qHc.next()) {
            ui->tableWidgetHistCompras->insertRow(r);
            ui->tableWidgetHistCompras->setItem(r, 0, new QTableWidgetItem(qHc.value(0).toDate().toString("yyyy-MM-dd")));
            ui->tableWidgetHistCompras->setItem(r, 1, new QTableWidgetItem(obtenerNombreTienda(qHc.value(1).toInt())));
            ui->tableWidgetHistCompras->setItem(r, 2, new QTableWidgetItem(qHc.value(2).toString()));
            ui->tableWidgetHistCompras->setItem(r, 3, new NumericTableWidgetItem(qHc.value(3).toDouble(), 0));
            ui->tableWidgetHistCompras->setItem(r, 4, new NumericTableWidgetItem(qHc.value(4).toDouble(), 2, "€"));
            ui->tableWidgetHistCompras->setItem(r, 5, new NumericTableWidgetItem(qHc.value(5).toDouble(), 2, "€"));
            r++;
        }
    }
    ui->tableWidgetHistCompras->blockSignals(false);

    // ── 3. Historial de Ventas / Tickets ─────────────────────────────────────
    ui->tableWidgetHistVentas->blockSignals(true);
    ui->tableWidgetHistVentas->setRowCount(0);
    ui->tableWidgetHistVentas->setColumnCount(7);
    ui->tableWidgetHistVentas->setHorizontalHeaderLabels(
        QStringList() << "Fecha" << "Hora" << "Tienda" << "Ticket" << "Uds" << "PVP (€)" << "Total (€)");

    QString sqlHistV = "";
    if (m_nubeDisponible) {
        sqlHistV = QString(
            "SELECT fecha, hora, id_tienda, nticket, cantidad, precio, totallinea "
            "FROM lineasticket_nube "
            "WHERE cod = '%1' "
            "ORDER BY fecha DESC, hora DESC LIMIT 50"
        ).arg(cod);
    } else {
        sqlHistV = QString(
            "SELECT fecha, hora, 1 as id_tienda, nticket, cantidad, precio, totallinea "
            "FROM lineasticket "
            "WHERE cod = '%1' "
            "ORDER BY fecha DESC, hora DESC LIMIT 50"
        ).arg(cod);
    }

    QSqlQuery qHv(db);
    if (qHv.exec(sqlHistV)) {
        int r = 0;
        while (qHv.next()) {
            ui->tableWidgetHistVentas->insertRow(r);
            ui->tableWidgetHistVentas->setItem(r, 0, new QTableWidgetItem(qHv.value(0).toDate().toString("yyyy-MM-dd")));
            ui->tableWidgetHistVentas->setItem(r, 1, new QTableWidgetItem(qHv.value(1).toString()));
            ui->tableWidgetHistVentas->setItem(r, 2, new QTableWidgetItem(obtenerNombreTienda(qHv.value(2).toInt())));
            ui->tableWidgetHistVentas->setItem(r, 3, new QTableWidgetItem(qHv.value(3).toString()));
            ui->tableWidgetHistVentas->setItem(r, 4, new NumericTableWidgetItem(qHv.value(4).toDouble(), 0));
            ui->tableWidgetHistVentas->setItem(r, 5, new NumericTableWidgetItem(qHv.value(5).toDouble(), 2, "€"));
            ui->tableWidgetHistVentas->setItem(r, 6, new NumericTableWidgetItem(qHv.value(6).toDouble(), 2, "€"));
            r++;
        }
    }
    ui->tableWidgetHistVentas->blockSignals(false);
}

/**
 * @brief Slot para filtrar la tabla en tiempo real.
 */
void PrepararPedidos::on_lineEditFiltro_textChanged(const QString &)
{
    rellenarTablaPrincipal();
}

/**
 * @brief Atajo: Este Mes.
 */
void PrepararPedidos::on_btnMesActual_clicked()
{
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
    ui->dateEditHasta->setDate(QDate(hoy.year(), hoy.month(), hoy.daysInMonth()));
    consultarDatos();
}

/**
 * @brief Atajo: Mes Anterior.
 */
void PrepararPedidos::on_btnMesAnterior_clicked()
{
    QDate mesPasado = QDate::currentDate().addMonths(-1);
    ui->dateEditDesde->setDate(QDate(mesPasado.year(), mesPasado.month(), 1));
    ui->dateEditHasta->setDate(QDate(mesPasado.year(), mesPasado.month(), mesPasado.daysInMonth()));
    consultarDatos();
}

/**
 * @brief Atajo: Últimos 3 Meses.
 */
void PrepararPedidos::on_btnUltimos3Meses_clicked()
{
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(hoy.addMonths(-3));
    ui->dateEditHasta->setDate(hoy);
    consultarDatos();
}

/**
 * @brief Atajo: Últimos 6 Meses.
 */
void PrepararPedidos::on_btnUltimos6Meses_clicked()
{
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(hoy.addMonths(-6));
    ui->dateEditHasta->setDate(hoy);
    consultarDatos();
}

/**
 * @brief Atajo: Este Año.
 */
void PrepararPedidos::on_btnAnoActual_clicked()
{
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(QDate(hoy.year(), 1, 1));
    ui->dateEditHasta->setDate(hoy);
    consultarDatos();
}

/**
 * @brief Añade el artículo seleccionado a un pedido (borrador).
 */
void PrepararPedidos::anadirArticuloAPedido(const QString &cod)
{
    if (cod.isEmpty() || cod == "PROVEEDOR") {
        QMessageBox::warning(this, "Aviso", "Por favor, selecciona un artículo válido de la tabla.");
        return;
    }

    DialogAnadirAPedido *dlg = new DialogAnadirAPedido(cod, this);
    dlg->exec();
    delete dlg;
}

/**
 * @brief Slot para el botón inferior "Añadir al Pedido".
 */
void PrepararPedidos::on_pushButtonAnadirPedido_clicked()
{
    anadirArticuloAPedido(m_codSeleccionado);
}

/**
 * @brief Slot para el botón lateral "Añadir este artículo a Pedido".
 */
void PrepararPedidos::on_pushButtonAnadirLateral_clicked()
{
    anadirArticuloAPedido(m_codSeleccionado);
}

/**
 * @brief Abre la ventana de gestión de pedidos.
 */
void PrepararPedidos::on_pushButtonGestionarPedidos_clicked()
{
    GestionPedidos *gest = new GestionPedidos(this);
    gest->exec();
    delete gest;
}

/**
 * @brief Exporta o imprime el informe de preparación de pedidos a HTML/PDF.
 */
void PrepararPedidos::on_pushButtonExportar_clicked()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Guardar Informe de Pedido",
                                                    QString("Informe_PrepararPedido_%1.html")
                                                    .arg(QDate::currentDate().toString("yyyy-MM-dd")),
                                                    "HTML (*.html);;Todos los archivos (*)");
    if (fileName.isEmpty()) return;

    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, "Error", "No se pudo crear el archivo para exportar.");
        return;
    }

    QTextStream out(&file);
    out << "<!DOCTYPE html><html><head><meta charset='utf-8'>"
        << "<title>Informe de Compras y Ventas - Preparar Pedidos</title>"
        << "<style>"
        << "body { font-family: Arial, sans-serif; margin: 20px; color: #2c3e50; }"
        << "h1 { color: #2c3e50; border-bottom: 2px solid #3498db; padding-bottom: 5px; }"
        << "table { width: 100%; border-collapse: collapse; margin-top: 15px; font-size: 11pt; }"
        << "th { background-color: #34495e; color: white; padding: 8px; text-align: left; }"
        << "td { border: 1px solid #bdc3c7; padding: 6px; }"
        << "tr:nth-child(even) { background-color: #f8f9fa; }"
        << ".num { text-align: right; }"
        << ".totales { background-color: #ecf0f1; font-weight: bold; margin-top: 15px; padding: 10px; border-radius: 4px; }"
        << "</style></head><body>";

    out << QString("<h1>Informe de Preparación de Pedidos</h1>");
    out << QString("<p><strong>Proveedor:</strong> %1<br>").arg(ui->comboBoxProveedor->currentText());
    out << QString("<strong>Periodo:</strong> %1 a %2<br>").arg(ui->dateEditDesde->text(), ui->dateEditHasta->text());
    out << QString("<strong>Tienda:</strong> %1<br>").arg(ui->comboBoxTienda->currentText());
    out << QString("<strong>Fecha de Generación:</strong> %1</p>").arg(QDate::currentDate().toString("yyyy-MM-dd"));

    out << "<table><thead><tr>"
        << "<th>Código</th><th>Descripción</th><th class='num'>Stock</th><th class='num'>Ventas Uds</th>"
        << "<th class='num'>Ventas (€)</th><th class='num'>Compras Uds</th><th class='num'>Compras (€)</th>"
        << "<th class='num'>Balance</th><th class='num'>Sugerido</th></tr></thead><tbody>";

    for (const ItemAnalisis &item : m_listaArticulos) {
        out << "<tr>"
            << QString("<td>%1</td>").arg(item.cod)
            << QString("<td>%1</td>").arg(item.descripcion)
            << QString("<td class='num'>%1</td>").arg(item.stockActual)
            << QString("<td class='num'>%1</td>").arg(item.udsVendidas)
            << QString("<td class='num'>%1 €</td>").arg(QString::number(item.ventasTotal, 'f', 2))
            << QString("<td class='num'>%1</td>").arg(item.udsCompradas)
            << QString("<td class='num'>%1 €</td>").arg(QString::number(item.comprasTotal, 'f', 2))
            << QString("<td class='num'>%1</td>").arg(item.udsVendidas - item.udsCompradas)
            << QString("<td class='num'><strong>%1</strong></td>").arg(item.sugerido)
            << "</tr>";
    }
    out << "</tbody></table>";

    out << "<div class='totales'>"
        << QString("<p>%1 | %2 | %3 | %4 | %5</p>")
           .arg(ui->lblTotalArticulos->text(), ui->lblTotalVentasEur->text(),
                ui->lblTotalComprasEur->text(), ui->lblMargenTotal->text(), ui->lblSugeridoTotal->text())
        << "</div></body></html>";

    file.close();

    QMessageBox::information(this, "Exportación Completada",
                             QString("El informe ha sido exportado correctamente a:\n%1").arg(fileName));
    QDesktopServices::openUrl(QUrl::fromLocalFile(fileName));
}

/**
 * @brief Cierra el diálogo.
 */
void PrepararPedidos::on_pushButtonCerrar_clicked()
{
    this->close();
}
