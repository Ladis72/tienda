#include "dialoganadirasalidas.h"
#include "ui_dialoganadirasalidas.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include <QDebug>

/**
 * @brief Constructor del diálogo para añadir producto a salidas.
 * @param codArticulo Código del artículo seleccionado en el catálogo.
 * @param parent Widget padre del diálogo.
 */
DialogAnadirASalidas::DialogAnadirASalidas(const QString &codArticulo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAnadirASalidas)
    , m_codArticulo(codArticulo.trimmed())
{
    ui->setupUi(this);

    // Inicializar fecha de caducidad con la fecha actual en formato yyyy-MM-dd
    ui->dateEditFechaCaducidad->setDate(QDate::currentDate());

    // Cargar datos y combos
    cargarDatosArticulo();
    llenarComboTiendas();
    llenarComboLotes();
}

/**
 * @brief Destructor del diálogo.
 */
DialogAnadirASalidas::~DialogAnadirASalidas()
{
    delete ui;
}

/**
 * @brief Carga la información del producto (descripción y precio) desde la base de datos.
 */
void DialogAnadirASalidas::cargarDatosArticulo()
{
    ui->lineEditCod->setText(m_codArticulo);

    if (m_codArticulo.isEmpty()) {
        return;
    }

    QSqlRecord registro = base.consulta_producto(conf->getConexionLocal(), m_codArticulo);
    if (!registro.isEmpty()) {
        ui->lineEditDesc->setText(registro.value("descripcion").toString());
        
        // Asignar PVP como precio por defecto
        double pvp = registro.value("pvp").toDouble();
        ui->lineEditPrecio->setText(QString::number(pvp, 'f', 2));
    }
}

/**
 * @brief Llena el combo de destinos con las tiendas configuradas.
 */
void DialogAnadirASalidas::llenarComboTiendas()
{
    ui->comboBoxDestino->clear();
    QSqlQuery listaTiendas = base.tiendas(QSqlDatabase::database(conf->getConexionLocal()));
    while (listaTiendas.next()) {
        ui->comboBoxDestino->addItem(listaTiendas.value("nombre").toString());
    }
}

/**
 * @brief Llena el combo de lotes disponibles con existencias registradas en almacén.
 */
void DialogAnadirASalidas::llenarComboLotes()
{
    ui->comboBoxLotes->blockSignals(true);
    ui->comboBoxLotes->clear();
    ui->comboBoxLotes->addItem(tr("-- Seleccionar lote existente (opcional) --"), QString());

    if (!m_codArticulo.isEmpty()) {
        QSqlQuery qFechas(QSqlDatabase::database(conf->getConexionLocal()));
        // Consultar fechas/lotes agrupadas con existencias positivas ordenadas por fecha
        qFechas.prepare("SELECT DATE_FORMAT(fecha, '%Y-%m-%d') AS f, sum(cantidad) AS cant "
                        "FROM lotes WHERE ean = ? GROUP BY fecha HAVING cant > 0 ORDER BY f ASC");
        qFechas.bindValue(0, m_codArticulo);

        if (qFechas.exec()) {
            while (qFechas.next()) {
                QString fechaStr = qFechas.value("f").toString();
                QString cantStr = qFechas.value("cant").toString();
                QString etiqueta = QString("%1 (Stock: %2)").arg(fechaStr, cantStr);
                ui->comboBoxLotes->addItem(etiqueta, fechaStr);
            }
        }
    }

    ui->comboBoxLotes->blockSignals(false);
}

/**
 * @brief Slot llamado al seleccionar un lote disponible del desplegable.
 * @param index Índice del elemento seleccionado.
 */
void DialogAnadirASalidas::on_comboBoxLotes_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    QString fechaLote = ui->comboBoxLotes->currentData().toString();
    if (!fechaLote.isEmpty()) {
        QDate fecha = QDate::fromString(fechaLote, "yyyy-MM-dd");
        if (fecha.isValid()) {
            ui->dateEditFechaCaducidad->setDate(fecha);
        }
    }
}

/**
 * @brief Valida los datos y registra la línea en salidaGenero_tmp.
 */
void DialogAnadirASalidas::on_pushButtonAceptar_clicked()
{
    if (ui->comboBoxDestino->currentIndex() == -1 || ui->comboBoxDestino->currentText().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Aviso"), tr("Debe seleccionar una tienda de destino."));
        ui->comboBoxDestino->setFocus();
        return;
    }

    double cantidad = ui->doubleSpinBoxCantidad->value();
    if (cantidad <= 0.0) {
        QMessageBox::warning(this, tr("Aviso"), tr("La cantidad debe ser mayor a cero."));
        ui->doubleSpinBoxCantidad->setFocus();
        return;
    }

    QString codProducto = ui->lineEditCod->text().trimmed();
    QString fechaFC = ui->dateEditFechaCaducidad->date().toString("yyyy-MM-dd");

    // Verificar si el lote existe en la base de datos
    QString idLote = base.idLote(conf->getConexionLocal(), codProducto, "", fechaFC);
    if (idLote == "0" || idLote.isEmpty()) {
        // Advertir si no hay lote registrado con esa fecha
        QMessageBox msgBox(this);
        msgBox.setText(tr("Parece que no hay en el almacén un lote registrado con esa fecha de caducidad."));
        msgBox.setInformativeText(tr("¿Desea continuar con la operación o cancelar y comprobar los datos?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        if (msgBox.exec() == QMessageBox::Cancel) {
            return;
        }
    } else {
        // Comprobar si hay existencias suficientes en el lote seleccionado
        QSqlQuery datosLote(QSqlDatabase::database(conf->getConexionLocal()));
        datosLote.prepare("SELECT cantidad FROM lotes WHERE id = ?");
        datosLote.bindValue(0, idLote);
        if (datosLote.exec() && datosLote.first()) {
            double stockLote = datosLote.record().value("cantidad").toDouble();
            if (stockLote < cantidad) {
                QMessageBox msgBox(this);
                msgBox.setText(tr("Las existencias del lote (%1) son menores a la cantidad solicitada (%2).")
                                   .arg(stockLote)
                                   .arg(cantidad));
                msgBox.setInformativeText(tr("¿Desea continuar con la salida de todos modos?"));
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
                msgBox.setDefaultButton(QMessageBox::Yes);
                if (msgBox.exec() == QMessageBox::Cancel) {
                    return;
                }
            }
        }
    }

    // Preparar precio
    QString precio = ui->lineEditPrecio->text().trimmed();
    if (precio.isEmpty()) {
        precio = "0";
    }

    // Fecha actual para la salida (formato yyyy-MM-dd)
    QString fechaActual = QDate::currentDate().toString("yyyy-MM-dd");
    int idTienda = base.idTiendaDesdeNombre(QSqlDatabase::database(conf->getConexionLocal()),
                                           ui->comboBoxDestino->currentText());

    // Preparar lista de datos para salidaGenero_tmp:
    // [cod, fechaEntrada/Salida, descripcion, cantidad, fechaCaducidad, pvp, idTienda]
    QStringList datos;
    datos.append(codProducto);
    datos.append(fechaActual);
    datos.append(ui->lineEditDesc->text());
    datos.append(QString::number(cantidad, 'f', 2));
    datos.append(fechaFC);
    datos.append(precio);
    datos.append(QString::number(idTienda));

    // Insertar en la tabla temporal de salidas
    base.insertarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "salidaGenero_tmp", datos);

    QMessageBox::information(this, tr("Éxito"),
                             tr("Producto añadido correctamente a las salidas pendientes.\n"
                                "Destino: %1\nCantidad: %2")
                                 .arg(ui->comboBoxDestino->currentText())
                                 .arg(cantidad));

    accept();
}
