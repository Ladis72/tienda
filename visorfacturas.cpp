#include "visorfacturas.h"
#include "ui_visorfacturas.h"

VisorFacturas::VisorFacturas(QString nFactura, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VisorFacturas)
{
    ui->setupUi(this);
    QSqlQuery consulta(QSqlDatabase::database(conf->getConexionLocal()));
    consulta.prepare("SELECT id, nDocumento, idProveedor, cod, descripcion, cantidad, bonificacion, lote, "
                     "DATE_FORMAT(fc, '%Y-%m-%d') AS fc, costo, descuento1, base, tipoIva, "
                     "totalbase, iva, re, pvp FROM lineaspedido WHERE nDocumento = ?");
    consulta.bindValue(0, nFactura);
    consulta.exec();
    qDebug() << consulta.lastError();
    consulta.first();
    modelo.setQuery(consulta);

    // Asignar encabezados descriptivos a las columnas de la tabla de facturas/albaranes
    modelo.setHeaderData(3, Qt::Horizontal, "CÓDIGO");
    modelo.setHeaderData(4, Qt::Horizontal, "DESCRIPCIÓN");
    modelo.setHeaderData(5, Qt::Horizontal, "CANTIDAD");
    modelo.setHeaderData(6, Qt::Horizontal, "BONIF.");
    modelo.setHeaderData(7, Qt::Horizontal, "LOTE");
    modelo.setHeaderData(8, Qt::Horizontal, "FECHA CADUCIDAD");
    modelo.setHeaderData(9, Qt::Horizontal, "COSTO");
    modelo.setHeaderData(10, Qt::Horizontal, "DESC %");
    modelo.setHeaderData(12, Qt::Horizontal, "IVA %");
    modelo.setHeaderData(13, Qt::Horizontal, "TOTAL BASE");
    modelo.setHeaderData(16, Qt::Horizontal, "PVP");

    ui->tableView->setModel(&modelo);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->resizeColumnsToContents();
    QString tituloVentana = "Proveedor: ";
    tituloVentana += base->nombreProveedor(consulta.value(2).toString(), conf->getConexionLocal());
    tituloVentana += " Factura: " + consulta.value(1).toString();
    this->setWindowTitle(tituloVentana);
    QScreen *monitor = QGuiApplication::primaryScreen();

    this->setMinimumWidth(monitor->size().width() - 100);
}

VisorFacturas::~VisorFacturas()
{
    delete ui;
}
