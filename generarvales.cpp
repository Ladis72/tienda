#include "generarvales.h"
#include "ui_generarvales.h"

#include <QMessageBox>
#include <QHeaderView>
#include <QSqlError>
#include <QSqlRecord>

/**
 * @brief Constructor del diálogo de generación de vales.
 */
GenerarVales::GenerarVales(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GenerarVales)
{
    ui->setupUi(this);

    // Fecha por defecto: primer día del mes anterior
    QDate hoy = QDate::currentDate();
    QDate mesAnterior = hoy.addMonths(-1);
    ui->dateEdit->setDate(QDate(mesAnterior.year(), mesAnterior.month(), 1));

    // Ajustar columnas de la tabla de previsualización
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    // NOTA: La lista de conexiones activas se carga en el momento de generar
    // (on_pushButtonGenerar_clicked), no aquí, para reflejar el estado actual
    // de las conexiones en el instante de calcular los vales.
}

GenerarVales::~GenerarVales()
{
    delete ui;
}

/**
 * @brief Calcula las ventas de un cliente en un mes concreto sumando
 *        todas las tiendas que estén conectadas en ese momento.
 *
 * @param idCliente  ID del cliente
 * @param desde      Primer día del mes (yyyy-MM-01)
 * @param hasta      Último día del mes  (yyyy-MM-DD)
 * @return Total de ventas en euros
 */
double GenerarVales::ventasTotalesCliente(int idCliente,
                                          const QString &desde,
                                          const QString &hasta)
{
    double total = 0.0;

    for (const QString &conexion : tiendas) {
        QSqlDatabase db = QSqlDatabase::database(conexion);
        if (!db.isOpen()) continue;

        // Construimos la consulta unificando tickets y ticketss si existe
        QSqlQuery q(db);
        if (db.tables().contains("ticketss")) {
            q.prepare("SELECT COALESCE(SUM(total),0) FROM ("
                      "  SELECT total FROM tickets  WHERE cliente=? AND fecha BETWEEN ? AND ? "
                      "  UNION ALL "
                      "  SELECT total FROM ticketss WHERE cliente=? AND fecha BETWEEN ? AND ?"
                      ") AS todas");
            q.bindValue(0, idCliente);
            q.bindValue(1, desde);
            q.bindValue(2, hasta);
            q.bindValue(3, idCliente);
            q.bindValue(4, desde);
            q.bindValue(5, hasta);
        } else {
            q.prepare("SELECT COALESCE(SUM(total),0) FROM tickets "
                      "WHERE cliente=? AND fecha BETWEEN ? AND ?");
            q.bindValue(0, idCliente);
            q.bindValue(1, desde);
            q.bindValue(2, hasta);
        }
        if (q.exec() && q.first()) {
            total += q.value(0).toDouble();
        } else {
            qWarning() << "Error consultando ventas en" << conexion << ":" << q.lastError().text();
        }
    }
    return total;
}

void GenerarVales::on_pushButtonGenerar_clicked()
{
    // ── 0. Validar la fecha seleccionada ──────────────────────────────────
    ui->label->setText("Comprobando la fecha...");
    ui->progressBar->setValue(5);
    QDate fecha     = ui->dateEdit->date();
    QDate fechaHoy  = QDate::currentDate();

    if (fecha.month() == fechaHoy.month() && fecha.year() == fechaHoy.year()) {
        QMessageBox::warning(this, "Error de fecha",
                             "No se pueden generar los vales de un mes en curso.");
        return;
    }
    if (fecha > fechaHoy) {
        QMessageBox::warning(this, "Error de fecha",
                             "La fecha seleccionada no puede ser futura.");
        return;
    }

    // ── 0b. Verificar que TODAS las tiendas están conectadas ───────────────
    // Los vales son UNIFICADOS: si alguna tienda remota no está conectada,
    // los totales serían incorrectos (incompletos). Se bloquea la generación
    // hasta que todas estén disponibles.
    {
        // Tiendas remotas configuradas en la BD (local = 0)
        const QStringList todasRemotas  = conf->getNombreConexiones();
        // Tiendas remotas con conexión activa en este instante
        const QStringList activas       = conf->getNombreConexionesActivas();

        // Detectar cuáles remotas están offline
        QStringList offline;
        for (const QString &t : todasRemotas) {
            if (!activas.contains(t)) {
                offline << t;
            }
        }

        if (!offline.isEmpty()) {
            // Hay tiendas sin conexión → no se puede garantizar un cálculo completo
            QMessageBox::critical(
                this,
                tr("Tiendas no conectadas"),
                tr("No se pueden generar los vales porque las siguientes tiendas "
                   "no están conectadas:\n\n  • %1\n\n"
                   "Conéctelas pulsando el botón «Conectar» e inténtelo de nuevo.")
                    .arg(offline.join("\n  • ")));
            ui->progressBar->setValue(0);
            ui->label->setText(tr("Generación cancelada: hay tiendas offline."));
            return;
        }

        // Todas las remotas están activas.
        // La lista de cálculo = conexión local + todas las remotas activas.
        // IMPORTANTE: getNombreConexiones() solo devuelve remotas (local=0),
        // por eso añadimos explícitamente la BD local.
        tiendas.clear();
        tiendas << conf->getConexionLocal(); // BD local (tickets propios)
        tiendas << activas;                  // BDs remotas (tickets de las demás)
        qDebug() << "GenerarVales: Tiendas para el cálculo:" << tiendas;
    }

    // Rango del mes seleccionado
    QString desde = fecha.toString("yyyy-MM-01");
    QString hasta = fecha.toString(QString("yyyy-MM-") + QString::number(fecha.daysInMonth()));

    // ── 1. Comprobar vales ya existentes para ese mes ─────────────────────
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    if (base.existeDatoEnTabla(dbLocal, "vales", "fechaEmision", desde)) {
        QMessageBox::StandardButton res = QMessageBox::question(
            this, "Vales ya generados",
            QString("Ya existen vales para %1.\n"
                    "¿Deseas eliminar los no usados y recalcular?")
                .arg(fecha.toString("MMMM yyyy")),
            QMessageBox::Yes | QMessageBox::No);
        if (res == QMessageBox::No) return;

        // Borrar únicamente los vales activos (estado=1) de ese mes
        QSqlQuery qBorrar(dbLocal);
        qBorrar.prepare("DELETE FROM vales WHERE fechaEmision = ? AND estado = 1");
        qBorrar.bindValue(0, desde);
        qBorrar.exec();
    }

    // ── 2. Caducar vales activos anteriores ──────────────────────────────
    ui->label->setText("Caducando vales anteriores...");
    ui->progressBar->setValue(10);
    if (ui->checkBoxSimular->isChecked()) {
        qDebug() << "SIMULACIÓN: Saltando caducación de vales.";
    } else {
        if (!base.caducarVales(conf->getConexionLocal())) {
            qWarning() << "No se pudieron caducar los vales anteriores.";
        }
    }

    // ── 3. Obtener todos los clientes con descuento > 0 ───────────────────
    //    Usamos la BD local (tablas maestras sincronizadas vía SyncManager)
    ui->label->setText("Cargando clientes con descuento...");
    ui->progressBar->setValue(20);

    QSqlQuery qClientes(dbLocal);
    if (!qClientes.exec("SELECT idCliente, nombre, apellidos, descuento "
                        "FROM clientes WHERE descuento > 0 ORDER BY idCliente")) {
        QMessageBox::critical(this, "Error",
                              "No se pudo obtener la lista de clientes:\n" +
                              qClientes.lastError().text());
        return;
    }

    // Almacenamos en lista para poder mostrar progreso
    struct DatosCliente { int id; QString nombre; double descuento; };
    QList<DatosCliente> clientes;
    while (qClientes.next()) {
        clientes.append({qClientes.value(0).toInt(),
                         qClientes.value(1).toString() + " " + qClientes.value(2).toString(),
                         qClientes.value(3).toDouble()});
    }

    if (clientes.isEmpty()) {
        QMessageBox::information(this, "Sin clientes",
                                 "No hay clientes con porcentaje de descuento configurado.");
        ui->progressBar->setValue(0);
        return;
    }

    // ── 4. Calcular vales iterando sobre clientes ─────────────────────────
    ui->tableWidget->setRowCount(0);
    ui->pushButtonGenerar->setEnabled(false);

    int valesCalculados = 0;
    double totalImporteVales = 0.0;

    if (!ui->checkBoxSimular->isChecked()) {
        dbLocal.transaction();
    }

    // ── 3b. Obtener el ID de la tienda local (una sola vez, fuera del bucle) ──
    // Se hace aquí para no repetir la query por cada cliente.
    int idTiendaOrigen = 1; // Fallback por defecto
    {
        QSqlQuery qTienda(dbLocal);
        if (qTienda.exec("SELECT id FROM tiendas WHERE local = 1") && qTienda.first()) {
            idTiendaOrigen = qTienda.value(0).toInt();
        } else {
            qWarning() << "GenerarVales: No se pudo obtener id_tienda_origen; usando 1 por defecto."
                       << qTienda.lastError().text();
        }
    }

    int n = clientes.size();
    for (int i = 0; i < n; ++i) {
        const DatosCliente &cli = clientes.at(i);

        // Progreso: de 20 a 90
        ui->progressBar->setValue(20 + (70 * (i + 1)) / n);
        ui->label->setText(QString("Calculando: %1...").arg(cli.nombre));
        QCoreApplication::processEvents(); // Mantener UI responsiva

        // Suma de ventas en TODAS las tiendas conectadas
        double totalVentas = ventasTotalesCliente(cli.id, desde, hasta);

        if (totalVentas <= 0) continue;

        double importeVale = qRound(totalVentas * (cli.descuento / 100.0) * 100.0) / 100.0;
        if (importeVale < 0.01) continue;

        // Mostrar en tabla de previsualización
        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(QString::number(cli.id)));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem(cli.nombre));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(
            QString::number(totalVentas, 'f', 2) + " €"));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(
            QString::number(cli.descuento, 'f', 1) + " %"));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(
            QString::number(importeVale, 'f', 2) + " €"));

        valesCalculados++;
        totalImporteVales += importeVale;

        if (ui->checkBoxSimular->isChecked()) {
            qDebug() << "SIMULACIÓN — Cliente:" << cli.id << cli.nombre
                     << "| Ventas:" << totalVentas
                     << "| Descuento:" << cli.descuento << "%"
                     << "| Vale:" << importeVale << "€";
            continue;
        }

        // Inserción real en la BD local.
        // vale_uuid se genera con UUID() de MariaDB para garantizar
        // unicidad global: evita colisiones de idvales entre tiendas.
        QSqlQuery qInsert(dbLocal);
        qInsert.prepare("INSERT INTO vales "
                        "(idCliente, cantidad, estado, fechaEmision, id_tienda_origen, vale_uuid) "
                        "VALUES (?, ?, 1, ?, ?, UUID())");
        qInsert.bindValue(0, cli.id);
        qInsert.bindValue(1, importeVale);
        qInsert.bindValue(2, desde);
        // idTiendaOrigen ya fue calculado antes del bucle (ver paso 3b)
        qInsert.bindValue(3, idTiendaOrigen);

        if (!qInsert.exec()) {
            qWarning() << "Error insertando vale para cliente"
                       << cli.id << ":" << qInsert.lastError().text();
        }
    }

    // ── 5. Confirmar transacción ──────────────────────────────────────────
    if (!ui->checkBoxSimular->isChecked()) {
        if (!dbLocal.commit()) {
            dbLocal.rollback();
            QMessageBox::critical(this, "Error",
                                  "No se pudieron guardar los vales.\n" +
                                  dbLocal.lastError().text());
            ui->pushButtonGenerar->setEnabled(true);
            return;
        }
    }

    // ── 6. Resultado ──────────────────────────────────────────────────────
    ui->progressBar->setValue(100);
    ui->pushButtonGenerar->setEnabled(true);

    QString msgFinal = ui->checkBoxSimular->isChecked()
                           ? "Simulación finalizada"
                           : "Generación completada";

    ui->label->setText(QString("%1: %2 vales · %3 €")
                           .arg(msgFinal)
                           .arg(valesCalculados)
                           .arg(QString::number(totalImporteVales, 'f', 2)));

    if (ui->checkBoxSimular->isChecked()) {
        QMessageBox::information(
            this, "Simulación finalizada",
            QString("Clientes con ventas en el periodo: %1\n"
                    "Vales que se generarían: %2\n"
                    "Importe total: %3 €\n\n"
                    "(No se ha guardado nada en la base de datos.)")
                .arg(n).arg(valesCalculados)
                .arg(totalImporteVales, 0, 'f', 2));
    } else {
        QMessageBox::information(
            this, "Vales generados",
            QString("Se han generado %1 vales de fidelidad.\n"
                    "Importe total: %2 €")
                .arg(valesCalculados)
                .arg(totalImporteVales, 0, 'f', 2));
    }
}

double GenerarVales::descuentoCliente(int idCliente)
{
    return base.descuentoCliente(QString::number(idCliente));
}
