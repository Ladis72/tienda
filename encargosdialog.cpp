#include "encargosdialog.h"
#include "ui_encargosdialog.h"
#include "buscarproducto.h"
#include "buscarcliente.h"
#include "base_datos.h"
#include "configuracion.h"
#include <QTableWidgetItem>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QMessageBox>

extern Configuracion *conf;

EncargosDialog::EncargosDialog(QString codCliente, QString nombreCliente, QString codArticulo, QString descArticulo, QString empleado, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncargosDialog),
    m_codCliente(codCliente)
{
    ui->setupUi(this);
    
    // Rellenar datos del cliente
    if (codCliente.isEmpty() || codCliente == "0") {
        ui->lineEditCliente->setText("Cliente de Contado");
        m_codCliente = "0";
    } else {
        ui->lineEditCliente->setText(codCliente + " - " + nombreCliente);
    }
    
    ui->lineEditEmpleado->setText(empleado);

    // Configurar la tabla de productos
    ui->tableWidgetProductos->setColumnCount(5);
    QStringList headers;
    headers << tr("Código") << tr("Descripción") << tr("Cant.") << tr("PVP (€)") << tr("Total (€)");
    ui->tableWidgetProductos->setHorizontalHeaderLabels(headers);
    ui->tableWidgetProductos->horizontalHeader()->setStretchLastSection(true);

    // Conectar el cambio de celdas para recalcular totales cuando se modifique cantidad o PVP
    connect(ui->tableWidgetProductos, &QTableWidget::cellChanged, this, &EncargosDialog::recalcularTotal);

    // Si se especificó un artículo inicial (desde TPV), añadirlo a la lista
    if (!codArticulo.isEmpty()) {
        baseDatos base;
        QSqlRecord rec = base.consulta_producto(conf->getConexionLocal(), codArticulo);
        double pvp = rec.value("pvp").toDouble();
        if (descArticulo.isEmpty()) descArticulo = rec.value("descripcion").toString();
        
        LineaEncargo linea;
        linea.codArticulo = codArticulo;
        linea.descripcion = descArticulo;
        linea.cantidad = 1;
        linea.pvp = pvp;
        addLinea(linea);
    }

    // Cargar las formas de pago disponibles en la base de datos
    baseDatos base;
    QSqlQuery qFp = base.fpago(QSqlDatabase::database(conf->getConexionLocal()));
    while (qFp.next()) {
        QString nombreForma = qFp.value("tipo").toString();
        int idForma = qFp.value("id").toInt();
        ui->comboBoxFormaPago->addItem(nombreForma, idForma);
    }

    // Si no existen formas de pago configuradas en la BD, añadir las opciones habituales por defecto
    if (ui->comboBoxFormaPago->count() == 0) {
        ui->comboBoxFormaPago->addItem("Efectivo", 1);
        ui->comboBoxFormaPago->addItem("Tarjeta", 2);
        ui->comboBoxFormaPago->addItem("Bizum", 3);
        ui->comboBoxFormaPago->addItem("Transferencia", 4);
    }
}

EncargosDialog::~EncargosDialog()
{
    delete ui;
}

// Devuelve la suma total de cantidades de todas las líneas de productos
int EncargosDialog::getCantidad() const
{
    int totalCant = 0;
    QList<LineaEncargo> lineas = getLineas();
    for (const LineaEncargo &l : lineas) {
        totalCant += l.cantidad;
    }
    return (totalCant > 0) ? totalCant : 1;
}

double EncargosDialog::getAnticipo() const
{
    return ui->doubleSpinBoxAnticipo->value();
}

QString EncargosDialog::getNotas() const
{
    return ui->textEditNotas->toPlainText();
}

// Obtener el nombre textual de la forma de pago seleccionada
QString EncargosDialog::getFormaPago() const
{
    return ui->comboBoxFormaPago->currentText();
}

// Obtener el ID de la forma de pago seleccionada
int EncargosDialog::getFormaPagoId() const
{
    return ui->comboBoxFormaPago->currentData().toInt();
}

// Devuelve si se debe imprimir el comprobante en la impresora de tickets
bool EncargosDialog::getImprimirTicket() const
{
    return ui->checkBoxImprimirTicket->isChecked();
}

// Devuelve el código del primer artículo o "VARIOS" si hay múltiples productos
QString EncargosDialog::getCodArticulo() const
{
    QList<LineaEncargo> lineas = getLineas();
    if (lineas.isEmpty()) return "";
    if (lineas.size() == 1) return lineas.first().codArticulo;
    return lineas.first().codArticulo; // Devuelve el primer código como principal
}

// Devuelve la descripción del primer artículo o resumen si hay varios
QString EncargosDialog::getDescArticulo() const
{
    QList<LineaEncargo> lineas = getLineas();
    if (lineas.isEmpty()) return "";
    if (lineas.size() == 1) return lineas.first().descripcion;
    return QString("%1 (+%2 más)").arg(lineas.first().descripcion).arg(lineas.size() - 1);
}

// Devuelve la lista completa de líneas de productos presentes en la tabla
QList<LineaEncargo> EncargosDialog::getLineas() const
{
    QList<LineaEncargo> lista;
    for (int row = 0; row < ui->tableWidgetProductos->rowCount(); ++row) {
        LineaEncargo l;
        QTableWidgetItem *itemCod = ui->tableWidgetProductos->item(row, 0);
        QTableWidgetItem *itemDesc = ui->tableWidgetProductos->item(row, 1);
        QTableWidgetItem *itemCant = ui->tableWidgetProductos->item(row, 2);
        QTableWidgetItem *itemPvp = ui->tableWidgetProductos->item(row, 3);
        
        if (itemCod && itemDesc) {
            l.codArticulo = itemCod->text();
            l.descripcion = itemDesc->text();
            l.cantidad = itemCant ? itemCant->text().toInt() : 1;
            l.pvp = itemPvp ? itemPvp->text().toDouble() : 0.0;
            if (l.cantidad <= 0) l.cantidad = 1;
            lista.append(l);
        }
    }
    return lista;
}

// Añade un producto a la tabla del encargo
void EncargosDialog::addLinea(const LineaEncargo &linea)
{
    ui->tableWidgetProductos->blockSignals(true);
    int row = ui->tableWidgetProductos->rowCount();
    ui->tableWidgetProductos->insertRow(row);

    QTableWidgetItem *itemCod = new QTableWidgetItem(linea.codArticulo);
    itemCod->setFlags(itemCod->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem *itemDesc = new QTableWidgetItem(linea.descripcion);
    itemDesc->setFlags(itemDesc->flags() ^ Qt::ItemIsEditable);

    QTableWidgetItem *itemCant = new QTableWidgetItem(QString::number(linea.cantidad));
    QTableWidgetItem *itemPvp = new QTableWidgetItem(QString::number(linea.pvp, 'f', 2));
    
    double totalLinea = linea.cantidad * linea.pvp;
    QTableWidgetItem *itemTotal = new QTableWidgetItem(QString::number(totalLinea, 'f', 2));
    itemTotal->setFlags(itemTotal->flags() ^ Qt::ItemIsEditable);

    ui->tableWidgetProductos->setItem(row, 0, itemCod);
    ui->tableWidgetProductos->setItem(row, 1, itemDesc);
    ui->tableWidgetProductos->setItem(row, 2, itemCant);
    ui->tableWidgetProductos->setItem(row, 3, itemPvp);
    ui->tableWidgetProductos->setItem(row, 4, itemTotal);

    ui->tableWidgetProductos->blockSignals(false);
    recalcularTotal();
}

// Reemplaza todas las líneas actuales por las especificadas
void EncargosDialog::setLineas(const QList<LineaEncargo> &lineas)
{
    ui->tableWidgetProductos->blockSignals(true);
    ui->tableWidgetProductos->setRowCount(0);
    ui->tableWidgetProductos->blockSignals(false);
    for (const LineaEncargo &l : lineas) {
        addLinea(l);
    }
}

// Devuelve el importe total acumulado del encargo
double EncargosDialog::getTotalEncargo() const
{
    double total = 0.0;
    QList<LineaEncargo> lineas = getLineas();
    for (const LineaEncargo &l : lineas) {
        total += (l.cantidad * l.pvp);
    }
    return total;
}

// Recalcula el total del encargo y actualiza las etiquetas en pantalla
void EncargosDialog::recalcularTotal()
{
    ui->tableWidgetProductos->blockSignals(true);
    double total = 0.0;
    for (int row = 0; row < ui->tableWidgetProductos->rowCount(); ++row) {
        QTableWidgetItem *itemCant = ui->tableWidgetProductos->item(row, 2);
        QTableWidgetItem *itemPvp = ui->tableWidgetProductos->item(row, 3);
        int cant = itemCant ? itemCant->text().toInt() : 1;
        double pvp = itemPvp ? itemPvp->text().toDouble() : 0.0;
        double subtotal = cant * pvp;
        total += subtotal;
        
        QTableWidgetItem *itemSub = ui->tableWidgetProductos->item(row, 4);
        if (itemSub) {
            itemSub->setText(QString::number(subtotal, 'f', 2));
        }
    }
    ui->tableWidgetProductos->blockSignals(false);
    ui->labelTotalEncargo->setText(QString("Total: %1 €").arg(QString::number(total, 'f', 2)));
}

// Slot para buscar y añadir un nuevo producto al encargo
void EncargosDialog::on_btnAnadirArticulo_clicked()
{
    baseDatos base;
    QSqlQuery consulta = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "articulos", "cod", "");
    BuscarProducto *bb = new BuscarProducto(this, consulta);
    bb->exec();
    if (!bb->resultado.isEmpty()) {
        QString cod = bb->resultado;
        QSqlRecord rec = base.consulta_producto(conf->getConexionLocal(), cod);
        
        LineaEncargo linea;
        linea.codArticulo = cod;
        linea.descripcion = rec.value("descripcion").toString();
        linea.cantidad = 1;
        linea.pvp = rec.value("pvp").toDouble();
        addLinea(linea);
    }
    delete bb;
}

// Slot para eliminar la línea seleccionada de la tabla de productos
void EncargosDialog::on_btnEliminarArticulo_clicked()
{
    int currentRow = ui->tableWidgetProductos->currentRow();
    if (currentRow >= 0) {
        ui->tableWidgetProductos->removeRow(currentRow);
        recalcularTotal();
    } else {
        QMessageBox::warning(this, tr("Aviso"), tr("Seleccione primero un producto de la tabla para eliminarlo."));
    }
}

// Slot para buscar cliente
void EncargosDialog::on_btnBuscarCliente_clicked()
{
    baseDatos base;
    QSqlQuery consulta = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "clientes", "nombre", "");
    BuscarCliente *bc = new BuscarCliente(this, consulta);
    bc->exec();
    if (!bc->resultado.isEmpty()) {
        m_codCliente = bc->resultado;
        ui->lineEditCliente->setText(m_codCliente + " - Cliente seleccionado");
    }
    delete bc;
}
