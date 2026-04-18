#include "generarvales.h"
#include "ui_generarvales.h"

#include <QMessageBox>
#include <QCheckBox>
#include <QGridLayout>

/**
 * @brief Constructor del diálogo de generación de vales.
 */
GenerarVales::GenerarVales(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::GenerarVales)
{
    ui->setupUi(this);
    // Conexiones remotas activas: se usarán para leer ventas (Opción B)
    tiendas = conf->getNombreConexionesActivas();
    ui->dateEdit->setDate(QDate::currentDate());

    // Añadir checkbox de simulación dinámicamente
    simularCheck = new QCheckBox("Modo Simulación (no guardar en BD)", this);
    simularCheck->setChecked(true); // Por seguridad, activado por defecto
    if (ui->gridLayout) {
        ui->gridLayout->addWidget(simularCheck, 3, 0, 1, 2);
    }
}

GenerarVales::~GenerarVales()
{
    delete ui;
}

void GenerarVales::on_pushButtonGenerar_clicked()
{
    // ── 0. Verificar que TODAS las tiendas estén conectadas ──────────────
    ui->label->setText("Verificando conexiones...");
    QStringList todasLasTiendas = conf->getNombreConexiones();
    QStringList tiendasDesconectadas;

    for (const QString &nombre : todasLasTiendas) {
        if (!QSqlDatabase::database(nombre).isOpen()) {
            tiendasDesconectadas << nombre;
        }
    }

    if (!tiendasDesconectadas.isEmpty()) {
        QMessageBox::critical(this, "Error de Conexión",
                              "No se pueden generar los vales porque hay tiendas desconectadas:\n\n" +
                              tiendasDesconectadas.join(", ") + "\n\n"
                              "Asegúrese de que todas las tiendas estén encendidas y conectadas "
                              "para que el cálculo de ventas sea completo.");
        return;
    }

    // ── 1. Validar la fecha seleccionada ──────────────────────────────────
    ui->label->setText("Comprobando la fecha");
    ui->progressBar->setValue(5);
    QDate fecha = ui->dateEdit->date();
    QDate fechaActual = QDate::currentDate();

    if (fecha >= fechaActual) {
        QMessageBox::warning(this, "Error de fecha",
                             "La fecha seleccionada debe ser anterior a hoy.");
        return;
    }
    if (fecha.month() == fechaActual.month()
        && fecha.year() == fechaActual.year()) {
        QMessageBox::warning(this, "Error de fecha",
                             "No se pueden generar los vales de un mes en curso.");
        return;
    }
    if (base.existeDatoEnTabla(QSqlDatabase::database(conf->getConexionLocal()),
                               "vales",
                               "fechaEmision",
                               fecha.toString("yyyy-MM-01"))) {
        QMessageBox::information(this, "Vales ya generados",
                                 "Ya existen vales para el mes seleccionado.");
        return;
    }

    // ── 2. Caducar los vales activos anteriores ───────────────────────────
    ui->label->setText("Caducando vales anteriores");
    ui->progressBar->setValue(10);
    if (simularCheck->isChecked()) {
        qDebug() << "SIMULACIÓN: Saltando caducación de vales.";
    } else {
        if (!base.caducarVales(conf->getConexionLocal())) {
            qWarning() << "Advertencia: no se pudieron caducar los vales anteriores";
        }
    }

    // ── 3. Asegurar y vaciar la tabla temporal ───────────────────────────
    ui->label->setText("Vaciando datos antiguos");
    ui->progressBar->setValue(15);
    QSqlDatabase dbLocal = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery queryPrep(dbLocal);
    // Aseguramos que la tabla existe por si es una instalación limpia
    queryPrep.exec("CREATE TABLE IF NOT EXISTS comprasClienteTMP ("
                   "id INT AUTO_INCREMENT PRIMARY KEY, "
                   "idCliente INT, "
                   "cantidad DOUBLE, "
                   "fecha DATE)");
    base.vaciarTabla("comprasClienteTMP", dbLocal);

    // ── 4. Cargar ventas del mes desde la BD local ────────────────────────
    ui->label->setText("Cargando datos locales");
    ui->progressBar->setValue(20);
    QSqlQuery consulta(dbLocal);
    QSqlQuery compras = base.ventasClientes(conf->getConexionLocal(),
                                            fecha, fecha);
    while (compras.next()) {
        consulta.prepare("INSERT INTO comprasClienteTMP (idCliente, cantidad, fecha) "
                         "VALUES (?, ?, ?)");
        consulta.bindValue(0, compras.value(0).toInt());
        consulta.bindValue(1, compras.value(1).toDouble());
        consulta.bindValue(2, fecha.toString("yyyy-MM-01"));
        if (!consulta.exec()) {
            qWarning() << "Error insertando en TMP local:" << consulta.lastError().text();
        }
    }

    // ── 5. Cargar ventas remotas (conexiones directas) ────────────────────
    ui->label->setText("Cargando datos remotos");
    ui->progressBar->setValue(30);
    
    // Filtramos para no repetir la conexión local si estuviera en la lista
    QStringList tiendasRemotas;
    for (const QString &t : tiendas) {
        if (t != conf->getConexionLocal()) tiendasRemotas << t;
    }

    int totalTiendasRemotas = tiendasRemotas.length();
    for (int t = 0; t < totalTiendasRemotas; t++) {
        QString nombreTienda = tiendasRemotas.at(t);
        QSqlDatabase dbRemota = QSqlDatabase::database(nombreTienda);
        if (!dbRemota.isOpen()) {
            qWarning() << "Tienda" << nombreTienda << "no está abierta, saltando...";
            continue;
        }

        QSqlQuery comprasRemotas = base.ventasClientes(nombreTienda, fecha, fecha);
        while (comprasRemotas.next()) {
            consulta.prepare("INSERT INTO comprasClienteTMP (idCliente, cantidad, fecha) "
                             "VALUES (?, ?, ?)");
            consulta.bindValue(0, comprasRemotas.value(0).toInt());
            consulta.bindValue(1, comprasRemotas.value(1).toDouble());
            consulta.bindValue(2, fecha.toString("yyyy-MM-01"));
            consulta.exec();
        }
        int progreso = 30 + ((40 * (t + 1)) / qMax(1, totalTiendasRemotas));
        ui->progressBar->setValue(progreso);
    }

    // ── 6. Generar vales ──────────────────────────────────────────────────
    ui->label->setText("Generando los vales");
    ui->progressBar->setValue(75);

    QSqlQuery comprasTotal(dbLocal);
    if (!comprasTotal.exec("SELECT idCliente, SUM(cantidad) AS total "
                          "FROM comprasClienteTMP GROUP BY idCliente")) {
        QMessageBox::critical(this, "Error", "No se pudo calcular el total de compras: " + comprasTotal.lastError().text());
        return;
    }

    int valesGenerados = 0;
    if (!simularCheck->isChecked()) {
        dbLocal.transaction();
    }

    QSqlQuery insertVale(dbLocal);
    while (comprasTotal.next()) {
        int idCliente = comprasTotal.value(0).toInt();
        double totalCompras = comprasTotal.value(1).toDouble();
        double pctDescuento = descuentoCliente(idCliente);

        if (pctDescuento <= 0 || totalCompras <= 0) continue;

        double importeVale = totalCompras * (pctDescuento / 100.0);
        // Redondear a 2 decimales
        importeVale = qRound(importeVale * 100.0) / 100.0;

        if (importeVale < 0.01) continue;

        if (simularCheck->isChecked()) {
            valesGenerados++;
            qDebug() << "SIMULACIÓN - Vale calculado: cliente" << idCliente
                     << "importe" << importeVale << "€ (Ventas:" << totalCompras << ")";
        } else {
            // Estructura: idvales (NULL), idCliente, cantidad, estado (1=activo), fechaEmision, fechaUso (NULL)
            insertVale.prepare("INSERT INTO vales (idCliente, cantidad, estado, fechaEmision, fechaUso) "
                               "VALUES (?, ?, 1, ?, NULL)");
            insertVale.bindValue(0, idCliente);
            insertVale.bindValue(1, importeVale);
            insertVale.bindValue(2, fecha.toString("yyyy-MM-01"));
            
            if (insertVale.exec()) {
                valesGenerados++;
            } else {
                qWarning() << "Error al generar vale para cliente" << idCliente << ":" << insertVale.lastError().text();
            }
        }
    }

    if (!simularCheck->isChecked()) {
        if (dbLocal.commit()) {
            qDebug() << "Transacción de vales completada.";
        } else {
            dbLocal.rollback();
            QMessageBox::critical(this, "Error", "No se pudieron guardar los vales en la base de datos.");
            return;
        }
    }

    // ── 7. Resultado ──────────────────────────────────────────────────────
    QString msgFin = simularCheck->isChecked() ? "Simulación finalizada" : "Finalizado";
    ui->label->setText(
        QString("%1: %2 vales calculados").arg(msgFin).arg(valesGenerados));
    ui->progressBar->setValue(100);

    if (valesGenerados > 0) {
        if (simularCheck->isChecked()) {
             QMessageBox::information(
                this, "Simulación completada",
                QString("Se han calculado %1 vales.\n\n"
                        "Revisa la consola para ver el detalle de importes por cliente.\n"
                        "No se ha modificado nada en la base de datos.")
                    .arg(valesGenerados));
        } else {
            QMessageBox::information(
                this, "Vales generados",
                QString("Se han generado %1 vales correctamente.")
                    .arg(valesGenerados));
        }
    } else {
        QMessageBox::warning(this, "Sin vales", "No se ha generado ningún vale.");
    }
}

double GenerarVales::descuentoCliente(int idCliente)
{
    return base.descuentoCliente(QString::number(idCliente));
}
