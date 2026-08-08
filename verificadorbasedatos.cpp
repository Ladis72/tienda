#include "verificadorbasedatos.h"
#include "ui_verificadorbasedatos.h"
#include "dialogdetallesverificador.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QColor>
#include <QFont>
#include <QStyle>
#include <QApplication>
#include <QDebug>
#include <QElapsedTimer>

/**
 * @brief Constructor del diálogo de verificación e integridad de la base de datos.
 *
 * Configura la interfaz de usuario, establece los iconos del sistema según el estilo global,
 * ajusta el comportamiento del QTableWidget e inicia el análisis automático inicial.
 *
 * @param parent Widget padre del diálogo.
 */
VerificadorBaseDatos::VerificadorBaseDatos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VerificadorBaseDatos)
{
    ui->setupUi(this);

    // Asignar iconos del sistema según el estilo global del programa
    ui->pushButtonAnalizar->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
    ui->pushButtonCorregir->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonMarcarTodos->setIcon(style()->standardIcon(QStyle::SP_DialogYesButton));
    ui->pushButtonDesmarcarTodos->setIcon(style()->standardIcon(QStyle::SP_DialogNoButton));
    ui->pushButtonDetalles->setIcon(style()->standardIcon(QStyle::SP_FileDialogContentsView));
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));

    // Configurar comportamiento y redimensionamiento de las 4 columnas de la tabla
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(1, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::Stretch);

    // Inicializar catálogo de comprobaciones SQL y ejecutar el análisis inicial
    inicializarComprobaciones();
    ejecutarAnalisis();
}

/**
 * @brief Destructor del diálogo. Libera la memoria consumida por la interfaz gráfica.
 */
VerificadorBaseDatos::~VerificadorBaseDatos()
{
    delete ui;
}

/**
 * @brief Inicializa el catálogo de comprobaciones de base de datos con sus respectivas consultas SQL altamente optimizadas.
 *
 * Se utiliza 'NOT EXISTS' correlacionado en lugar de 'NOT IN' para aprovechar los índices primarios (B-Tree),
 * evitando escaneos completos y bloqueos de interfaz en tablas grandes.
 */
void VerificadorBaseDatos::inicializarComprobaciones()
{
    m_comprobaciones.clear();

    // 1. Líneas de ticket huérfanas (sin cabecera de ticket asociada en tickets ni en la tabla histórica ticketss)
    {
        ComprobacionBD c;
        c.id = "lineas_ticket_huerfanas";
        c.titulo = tr("Líneas de ticket sin cabecera");
        c.consultaCheck = "SELECT COUNT(*) FROM lineasticket WHERE nticket IS NOT NULL AND nticket <> '' AND nticket <> '0' AND NOT EXISTS (SELECT 1 FROM tickets WHERE tickets.ticket = CAST(lineasticket.nticket AS UNSIGNED)) AND NOT EXISTS (SELECT 1 FROM ticketss WHERE ticketss.ticket = CAST(lineasticket.nticket AS UNSIGNED));";
        c.consultaFix = "DELETE FROM lineasticket WHERE nticket IS NOT NULL AND nticket <> '' AND nticket <> '0' AND NOT EXISTS (SELECT 1 FROM tickets WHERE tickets.ticket = CAST(lineasticket.nticket AS UNSIGNED)) AND NOT EXISTS (SELECT 1 FROM ticketss WHERE ticketss.ticket = CAST(lineasticket.nticket AS UNSIGNED));";
        c.consultaDetalle = "SELECT DISTINCT nticket FROM lineasticket WHERE nticket IS NOT NULL AND nticket <> '' AND nticket <> '0' AND NOT EXISTS (SELECT 1 FROM tickets WHERE tickets.ticket = CAST(lineasticket.nticket AS UNSIGNED)) AND NOT EXISTS (SELECT 1 FROM ticketss WHERE ticketss.ticket = CAST(lineasticket.nticket AS UNSIGNED)) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 2. Lotes huérfanos (asociados a artículos que no existen)
    {
        ComprobacionBD c;
        c.id = "lotes_huerfanos";
        c.titulo = tr("Lotes de artículos inexistentes");
        c.consultaCheck = "SELECT COUNT(*) FROM lotes WHERE ean IS NOT NULL AND ean <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = lotes.ean);";
        c.consultaFix = "DELETE FROM lotes WHERE ean IS NOT NULL AND ean <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = lotes.ean);";
        c.consultaDetalle = "SELECT DISTINCT ean FROM lotes WHERE ean IS NOT NULL AND ean <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = lotes.ean) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 3. Precios de tienda huérfanos
    {
        ComprobacionBD c;
        c.id = "precios_tienda_huerfanos";
        c.titulo = tr("Precios específicos de artículos inexistentes");
        c.consultaCheck = "SELECT COUNT(*) FROM precios_tienda WHERE cod_articulo IS NOT NULL AND cod_articulo <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = precios_tienda.cod_articulo);";
        c.consultaFix = "DELETE FROM precios_tienda WHERE cod_articulo IS NOT NULL AND cod_articulo <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = precios_tienda.cod_articulo);";
        c.consultaDetalle = "SELECT DISTINCT cod_articulo FROM precios_tienda WHERE cod_articulo IS NOT NULL AND cod_articulo <> '' AND NOT EXISTS (SELECT 1 FROM articulos WHERE articulos.cod = precios_tienda.cod_articulo) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 5. Stock negativo en artículos (calculado a partir de la suma de la tabla lotes)
    {
        ComprobacionBD c;
        c.id = "stock_negativo_lotes";
        c.titulo = tr("Artículos con stock total de lotes negativo");
        c.consultaCheck = "SELECT COUNT(*) FROM (SELECT ean FROM lotes GROUP BY ean HAVING SUM(cantidad) < 0) as stock_negativo;";
        c.consultaFix = "UPDATE lotes SET cantidad = 0 WHERE cantidad < 0;";
        c.consultaDetalle = "SELECT CONCAT(IFNULL(a.descripcion, 'SIN DESCRIPCIÓN'), ' [', l.ean, '] (Stock total lotes: ', SUM(l.cantidad), ')') FROM lotes l LEFT JOIN articulos a ON l.ean = a.cod GROUP BY l.ean, a.descripcion HAVING SUM(l.cantidad) < 0 LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 6. Artículos sin descripción o con descripción nula
    {
        ComprobacionBD c;
        c.id = "articulos_sin_descripcion";
        c.titulo = tr("Artículos sin descripción");
        c.consultaCheck = "SELECT COUNT(*) FROM articulos WHERE descripcion IS NULL OR TRIM(descripcion) = '';";
        c.consultaFix = "UPDATE articulos SET descripcion = 'Artículo sin descripción' WHERE descripcion IS NULL OR TRIM(descripcion) = '';";
        c.consultaDetalle = "SELECT cod FROM articulos WHERE descripcion IS NULL OR TRIM(descripcion) = '' LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 7. Vales de cliente huérfanos
    {
        ComprobacionBD c;
        c.id = "vales_huerfanos";
        c.titulo = tr("Vales asociados a clientes inexistentes");
        c.consultaCheck = "SELECT COUNT(*) FROM vales WHERE idCliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = vales.idCliente);";
        c.consultaFix = "UPDATE vales SET idCliente = 0 WHERE idCliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = vales.idCliente);";
        c.consultaDetalle = "SELECT CONCAT('Vale ID: ', idVales, ' (Cliente ID inexistente: ', idCliente, ')') FROM vales WHERE idCliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = vales.idCliente) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 8. Encargos huérfanos
    {
        ComprobacionBD c;
        c.id = "encargos_huerfanos";
        c.titulo = tr("Encargos asociados a clientes inexistentes");
        c.consultaCheck = "SELECT COUNT(*) FROM encargos WHERE id_cliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = encargos.id_cliente);";
        c.consultaFix = "UPDATE encargos SET id_cliente = 0 WHERE id_cliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = encargos.id_cliente);";
        c.consultaDetalle = "SELECT CONCAT('Encargo ID: ', id_encargo, ' (Cliente ID inexistente: ', id_cliente, ')') FROM encargos WHERE id_cliente <> 0 AND NOT EXISTS (SELECT 1 FROM clientes WHERE clientes.idCliente = encargos.id_cliente) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 9. Albaranes huérfanos sin factura válida (usando BINARY para ignorar colaciones mixtas entre latin1 y utf8mb4)
    {
        ComprobacionBD c;
        c.id = "albaranes_huerfanos";
        c.titulo = tr("Albaranes asociados a facturas inexistentes");
        c.consultaCheck = "SELECT COUNT(*) FROM albaranes WHERE idFactura IS NOT NULL AND idFactura <> '' AND idFactura <> '0' AND NOT EXISTS (SELECT 1 FROM facturas WHERE BINARY facturas.nFactura = BINARY albaranes.idFactura OR BINARY CAST(facturas.id AS CHAR) = BINARY albaranes.idFactura);";
        c.consultaFix = "UPDATE albaranes SET idFactura = '0' WHERE idFactura IS NOT NULL AND idFactura <> '' AND idFactura <> '0' AND NOT EXISTS (SELECT 1 FROM facturas WHERE BINARY facturas.nFactura = BINARY albaranes.idFactura OR BINARY CAST(facturas.id AS CHAR) = BINARY albaranes.idFactura);";
        c.consultaDetalle = "SELECT CONCAT('Albarán ID: ', id, ' (Factura inexistente: ', idFactura, ')') FROM albaranes WHERE idFactura IS NOT NULL AND idFactura <> '' AND idFactura <> '0' AND NOT EXISTS (SELECT 1 FROM facturas WHERE BINARY facturas.nFactura = BINARY albaranes.idFactura OR BINARY CAST(facturas.id AS CHAR) = BINARY albaranes.idFactura) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 10. Tickets vacíos sin líneas de venta asociadas
    {
        ComprobacionBD c;
        c.id = "tickets_sin_lineas";
        c.titulo = tr("Tickets registrados sin líneas de venta");
        c.consultaCheck = "SELECT COUNT(*) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR));";
        c.consultaFix = "DELETE FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR));";
        c.consultaDetalle = "SELECT CONCAT('Ticket Nº: ', ticket) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR)) LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 11. Verificación del formato obligatorio de fechas "yyyy-MM-dd" en tickets (consultando valores nulos o 0000-00-00)
    {
        ComprobacionBD c;
        c.id = "tickets_fecha_invalida";
        c.titulo = tr("Tickets con fechas nulas o fecha cero (0000-00-00)");
        c.consultaCheck = "SELECT COUNT(*) FROM tickets WHERE fecha IS NULL OR fecha = '0000-00-00';";
        c.consultaFix = ""; // Requiere inspección manual para evitar alteración indebida de fechas de facturación
        c.consultaDetalle = "SELECT CONCAT('Ticket Nº: ', ticket, ' (Fecha inválida/nula)') FROM tickets WHERE fecha IS NULL OR fecha = '0000-00-00' LIMIT 50;";
        c.esCorregible = false;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 12. Artículos con precios negativos (PVP o Precio de Compra menor a 0)
    {
        ComprobacionBD c;
        c.id = "articulos_precios_negativos";
        c.titulo = tr("Artículos con PVP o precio de compra negativo");
        c.consultaCheck = "SELECT COUNT(*) FROM articulos WHERE pvp < 0 OR precio_compra < 0;";
        c.consultaFix = "UPDATE articulos SET pvp = 0 WHERE pvp < 0; UPDATE articulos SET precio_compra = 0 WHERE precio_compra < 0;";
        c.consultaDetalle = "SELECT CONCAT('Código: ', cod, ' (PVP: ', pvp, ', Compra: ', IFNULL(precio_compra, 0), ')') FROM articulos WHERE pvp < 0 OR precio_compra < 0 LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }

    // 13. Tablas con juego de caracteres antiguo (latin1 en lugar del estándar utf8mb4)
    {
        ComprobacionBD c;
        c.id = "tablas_charset_obsoleto";
        c.titulo = tr("Tablas con codificación obsoleta (latin1 en lugar de utf8mb4)");
        c.consultaCheck = "SELECT COUNT(*) FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_TYPE = 'BASE TABLE' AND TABLE_COLLATION IS NOT NULL AND TABLE_COLLATION NOT LIKE 'utf8mb4%';";
        c.consultaFix = "DYNAMIC_CHARSET_CONVERSION"; // Se procesa dinámicamente fuera de transacción en on_pushButtonCorregir_clicked
        c.consultaDetalle = "SELECT CONCAT(TABLE_NAME, ' (Codificación: ', TABLE_COLLATION, ')') FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_TYPE = 'BASE TABLE' AND TABLE_COLLATION IS NOT NULL AND TABLE_COLLATION NOT LIKE 'utf8mb4%' LIMIT 50;";
        c.esCorregible = true;
        c.totalErrores = 0;
        m_comprobaciones.append(c);
    }
}

/**
 * @brief Slot para el botón 'Analizar'. Inicia el análisis completo.
 */
void VerificadorBaseDatos::on_pushButtonAnalizar_clicked()
{
    ejecutarAnalisis();
}

/**
 * @brief Realiza la ejecución de todas las comprobaciones registradas en la base de datos local.
 *
 * Muestra el cursor de espera durante la comprobación, mide los tiempos por consola y actualiza la UI.
 */
void VerificadorBaseDatos::ejecutarAnalisis()
{
    QApplication::setOverrideCursor(Qt::WaitCursor);

    ui->lblEstadoGlobal->setText(tr("Analizando consistencia de la base de datos... Por favor, espere."));
    ui->pushButtonAnalizar->setEnabled(false);
    ui->pushButtonCorregir->setEnabled(false);
    ui->pushButtonDetalles->setEnabled(false);

    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    if (!db.isOpen()) {
        QApplication::restoreOverrideCursor();
        ui->lblEstadoGlobal->setText(tr("Error: La conexión local con la base de datos no está abierta."));
        ui->lblEstadoGlobal->setStyleSheet("color: #c0392b; font-weight: bold;");
        ui->pushButtonAnalizar->setEnabled(true);
        return;
    }

    int erroresTotales = 0;
    QElapsedTimer timerGlobal;
    timerGlobal.start();

    // Recorrer cada comprobación registrada y ejecutar la consulta de verificación
    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        ComprobacionBD &c = m_comprobaciones[i];

        // Si es la comprobación de tickets sin líneas, adaptar la consulta según exista la tabla histórica lineasticketss
        if (c.id == "tickets_sin_lineas") {
            QSqlQuery qHist(db);
            bool tieneHistorical = qHist.exec("SELECT 1 FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'lineasticketss'") && qHist.next();
            if (tieneHistorical) {
                c.consultaCheck = "SELECT COUNT(*) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR)) AND NOT EXISTS (SELECT 1 FROM lineasticketss WHERE lineasticketss.nticket = CAST(tickets.ticket AS CHAR));";
                c.consultaFix = "DELETE FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR)) AND NOT EXISTS (SELECT 1 FROM lineasticketss WHERE lineasticketss.nticket = CAST(tickets.ticket AS CHAR));";
                c.consultaDetalle = "SELECT CONCAT('Ticket Nº: ', ticket) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR)) AND NOT EXISTS (SELECT 1 FROM lineasticketss WHERE lineasticketss.nticket = CAST(tickets.ticket AS CHAR)) LIMIT 50;";
            } else {
                c.consultaCheck = "SELECT COUNT(*) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR));";
                c.consultaFix = "DELETE FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR));";
                c.consultaDetalle = "SELECT CONCAT('Ticket Nº: ', ticket) FROM tickets WHERE NOT EXISTS (SELECT 1 FROM lineasticket WHERE lineasticket.nticket = CAST(tickets.ticket AS CHAR)) LIMIT 50;";
            }
        }

        QSqlQuery q(db);
        QElapsedTimer timerCheck;
        timerCheck.start();
        
        c.totalErrores = 0;
        c.detalles.clear();

        if (q.exec(c.consultaCheck) && q.next()) {
            c.totalErrores = q.value(0).toInt();
            erroresTotales += c.totalErrores;
            qDebug() << "Check" << c.id << "completado en" << timerCheck.elapsed() << "ms — Errores:" << c.totalErrores;
        } else {
            qDebug() << "Error ejecutando verificación SQL (" << c.id << "):" << q.lastError().text();
        }

        // Si existen anomalías y hay consulta de detalle definida, extraer las primeras filas afectadas
        if (c.totalErrores > 0 && !c.consultaDetalle.isEmpty()) {
            QSqlQuery qD(db);
            if (qD.exec(c.consultaDetalle)) {
                while (qD.next()) {
                    c.detalles.append(qD.value(0).toString());
                }
            } else {
                qDebug() << "Error obteniendo detalles SQL (" << c.id << "):" << qD.lastError().text();
            }
        }
    }

    qDebug() << "Análisis completo finalizado en" << timerGlobal.elapsed() << "ms. Errores totales:" << erroresTotales;

    actualizarTabla();

    QApplication::restoreOverrideCursor();

    ui->pushButtonAnalizar->setEnabled(true);
    ui->pushButtonCorregir->setEnabled(erroresTotales > 0);
    ui->pushButtonDetalles->setEnabled(true);

    if (erroresTotales == 0) {
        ui->lblEstadoGlobal->setText(tr("Análisis completado: ¡No se han detectado anomalías ni errores en los datos!"));
        ui->lblEstadoGlobal->setStyleSheet("color: #27ae60; font-weight: bold;");
    } else {
        ui->lblEstadoGlobal->setText(tr("Análisis completado: Se detectaron %1 anomalías o registros incoherentes.").arg(erroresTotales));
        ui->lblEstadoGlobal->setStyleSheet("color: #d35400; font-weight: bold;");
    }
}

/**
 * @brief Actualiza la tabla visual con el estado de cada diagnóstico y casillas de selección.
 */
void VerificadorBaseDatos::actualizarTabla()
{
    ui->tableWidget->setRowCount(0);
    ui->tableWidget->setRowCount(m_comprobaciones.size());

    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        const ComprobacionBD &c = m_comprobaciones[i];

        // Columna 0: Casilla de verificación para selección de reparación individual
        QTableWidgetItem *itemSel = new QTableWidgetItem();
        if (c.totalErrores > 0 && c.esCorregible) {
            itemSel->setFlags(itemSel->flags() | Qt::ItemIsUserCheckable | Qt::ItemIsEnabled);
            itemSel->setCheckState(Qt::Checked); // Marcada por defecto si hay errores corregibles
        } else {
            itemSel->setFlags(itemSel->flags() & ~Qt::ItemIsUserCheckable);
            itemSel->setCheckState(Qt::Unchecked);
        }
        ui->tableWidget->setItem(i, 0, itemSel);

        // Columna 1: Título descriptivo de la comprobación
        QTableWidgetItem *itemTitulo = new QTableWidgetItem(c.titulo);
        ui->tableWidget->setItem(i, 1, itemTitulo);

        // Columna 2: Estado del diagnóstico (Correcto / Cantidad de errores)
        QTableWidgetItem *itemEstado = new QTableWidgetItem();
        if (c.totalErrores == 0) {
            itemEstado->setText(tr("CORRECTO"));
            itemEstado->setForeground(QColor("#27ae60")); // Verde corporativo
            QFont font = itemEstado->font();
            font.setBold(true);
            itemEstado->setFont(font);
        } else {
            itemEstado->setText(tr("%1 errores").arg(c.totalErrores));
            itemEstado->setForeground(QColor("#c0392b")); // Rojo alerta
            QFont font = itemEstado->font();
            font.setBold(true);
            itemEstado->setFont(font);
        }
        ui->tableWidget->setItem(i, 2, itemEstado);

        // Columna 3: Resumen simplificado de los detalles
        QTableWidgetItem *itemDetalle = new QTableWidgetItem();
        if (c.totalErrores == 0) {
            itemDetalle->setText(tr("Sin problemas detectados."));
        } else {
            QString summary = c.detalles.join(", ");
            if (c.totalErrores > c.detalles.size()) {
                summary += tr("...");
            }
            itemDetalle->setText(summary);
        }
        ui->tableWidget->setItem(i, 3, itemDetalle);
    }
}

/**
 * @brief Slot para marcar todas las casillas de comprobaciones corregibles que contengan errores.
 */
void VerificadorBaseDatos::on_pushButtonMarcarTodos_clicked()
{
    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        const ComprobacionBD &c = m_comprobaciones[i];
        QTableWidgetItem *itemSel = ui->tableWidget->item(i, 0);
        if (itemSel && c.totalErrores > 0 && c.esCorregible) {
            itemSel->setCheckState(Qt::Checked);
        }
    }
}

/**
 * @brief Slot para desmarcar todas las casillas de verificación de la tabla.
 */
void VerificadorBaseDatos::on_pushButtonDesmarcarTodos_clicked()
{
    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        QTableWidgetItem *itemSel = ui->tableWidget->item(i, 0);
        if (itemSel) {
            itemSel->setCheckState(Qt::Unchecked);
        }
    }
}

/**
 * @brief Slot para el botón 'Corregir Seleccionados'. Aplica las consultas de reparación únicamente para las comprobaciones marcadas.
 */
void VerificadorBaseDatos::on_pushButtonCorregir_clicked()
{
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    if (!db.isOpen()) {
        QMessageBox::critical(this, tr("Error de Conexión"),
                              tr("La conexión a la base de datos local no está abierta. No se puede proceder con la corrección."));
        return;
    }

    // Contar cuántas comprobaciones están marcadas por el usuario
    int marcadosCount = 0;
    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        QTableWidgetItem *itemSel = ui->tableWidget->item(i, 0);
        const ComprobacionBD &c = m_comprobaciones[i];
        if (itemSel && itemSel->checkState() == Qt::Checked && c.totalErrores > 0 && c.esCorregible) {
            marcadosCount++;
        }
    }

    if (marcadosCount == 0) {
        QMessageBox::warning(this, tr("Selección Vacía"),
                             tr("No ha marcado ninguna comprobación corregible con errores.\nPor favor, marque la casilla de las comprobaciones que desee reparar."));
        return;
    }

    // Solicitar confirmación explícita al usuario indicando el número de comprobaciones seleccionadas
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, tr("Confirmar Corrección Seleccionada"),
                                  tr("¿Está seguro de que desea aplicar la corrección automática únicamente para las %1 comprobaciones seleccionadas?\n"
                                     "Se realizarán modificaciones directas sobre la base de datos local.").arg(marcadosCount),
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::No) {
        return;
    }

    db.transaction();
    bool todoOk = true;
    QString errorMsg;
    QStringList correccionesRealizadas;

    for (int i = 0; i < m_comprobaciones.size(); ++i) {
        QTableWidgetItem *itemSel = ui->tableWidget->item(i, 0);
        const ComprobacionBD &c = m_comprobaciones[i];

        // Ejecutar corrección solo si está marcada por el usuario
        if (itemSel && itemSel->checkState() == Qt::Checked && c.totalErrores > 0 && c.esCorregible && !c.consultaFix.isEmpty()) {

            // Caso especial: Conversión de codificación de tablas (DDL)
            if (c.id == "tablas_charset_obsoleto") {
                QSqlQuery q(db);
                q.exec("SET FOREIGN_KEY_CHECKS=0;");

                QSqlQuery qTablas(db);
                if (qTablas.exec("SELECT TABLE_NAME FROM information_schema.TABLES WHERE TABLE_SCHEMA = DATABASE() AND TABLE_TYPE = 'BASE TABLE' AND TABLE_COLLATION IS NOT NULL AND TABLE_COLLATION NOT LIKE 'utf8mb4%';")) {
                    while (qTablas.next()) {
                        QString tbl = qTablas.value(0).toString();
                        QSqlQuery qAlter(db);
                        QString alterSql = QString("ALTER TABLE `%1` CONVERT TO CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;").arg(tbl);
                        if (!qAlter.exec(alterSql)) {
                            qDebug() << "Warning al convertir tabla" << tbl << ":" << qAlter.lastError().text();
                        }
                    }
                }
                q.exec("SET FOREIGN_KEY_CHECKS=1;");
                correccionesRealizadas.append(QString("%1 (%2 tablas)").arg(c.titulo).arg(c.totalErrores));
                continue;
            }

            QSqlQuery q(db);
            // Soporte para múltiples consultas separadas por ';' en una sola corrección
            QStringList consultas = c.consultaFix.split(';', Qt::SkipEmptyParts);
            for (const QString &sqlQuery : consultas) {
                QString qTrim = sqlQuery.trimmed();
                if (qTrim.isEmpty()) continue;
                if (!q.exec(qTrim)) {
                    todoOk = false;
                    errorMsg = q.lastError().text();
                    qDebug() << "Error en corrección automática (" << c.id << "):" << errorMsg;
                    break;
                }
            }
            if (!todoOk) break;

            correccionesRealizadas.append(QString("%1 (%2 registros)").arg(c.titulo).arg(c.totalErrores));
        }
    }

    if (todoOk) {
        db.commit();

        // Registrar las correcciones en el log de auditoría local tras confirmar la transacción
        for (const QString &corr : correccionesRealizadas) {
            m_base.insertarLog(conf->getConexionLocal(), "Mantenimiento", conf->getUsuario(),
                               QString("Corrección automática aplicada: %1").arg(corr));
        }

        QMessageBox::information(this, tr("Corrección Completada"),
                                 tr("Se han corregido satisfactoriamente las comprobaciones seleccionadas."));
    } else {
        db.rollback();
        QMessageBox::critical(this, tr("Error de Corrección"),
                              tr("Ocurrió un error al aplicar las correcciones SQL. Se revirtieron todos los cambios.\nDetalles: %1").arg(errorMsg));
    }

    // Re-ejecutar el análisis completo para validar los resultados actualizados
    ejecutarAnalisis();
}

/**
 * @brief Slot para el botón 'Ver Detalles'. Muestra un cuadro modal con los detalles de la comprobación seleccionada.
 */
void VerificadorBaseDatos::on_pushButtonDetalles_clicked()
{
    int row = ui->tableWidget->currentRow();
    if (row < 0 || row >= m_comprobaciones.size()) {
        QMessageBox::warning(this, tr("Selección Requerida"),
                             tr("Por favor, seleccione una fila de la tabla para ver sus detalles."));
        return;
    }

    const ComprobacionBD &c = m_comprobaciones[row];
    if (c.totalErrores == 0) {
        QMessageBox::information(this, tr("Sin Anomalías"),
                                 tr("No se encontraron anomalías en esta comprobación."));
        return;
    }

    // Abrir ventana modal redimensionable con los detalles del diagnóstico
    DialogDetallesVerificador dlg(c.titulo, c.totalErrores, c.detalles, this);
    dlg.exec();
}

/**
 * @brief Slot para gestionar el doble clic sobre las filas de la tabla de diagnósticos.
 * @param item Celda seleccionada.
 */
void VerificadorBaseDatos::on_tableWidget_itemDoubleClicked(QTableWidgetItem *item)
{
    Q_UNUSED(item);
    on_pushButtonDetalles_clicked();
}

