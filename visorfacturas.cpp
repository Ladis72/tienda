#include "visorfacturas.h"
#include "ui_visorfacturas.h"

VisorFacturas::VisorFacturas(QString nFactura , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VisorFacturas)
{
    ui->setupUi(this);
    QSqlQuery consulta = base->ejecutarSentencia("SELECT * FROM lineaspedido WHERE nDocumento = '"+nFactura+"'");
    qDebug() << consulta.lastError();
    consulta.first();
    modelo.setQuery(consulta);
    ui->tableView->setModel(&modelo);
    ui->tableView->hideColumn(0);
    ui->tableView->hideColumn(1);
    ui->tableView->hideColumn(2);
    ui->tableView->resizeColumnsToContents();
    QString tituloVentana = "Proveedor: ";
    tituloVentana += base->nombreProveedor(consulta.value(2).toString());
    tituloVentana += " Factura: "+consulta.value(1).toString();
    this->setWindowTitle(tituloVentana);
    QScreen *monitor = QGuiApplication::primaryScreen();

    this->setMinimumWidth(monitor->size().width()-100);
}

VisorFacturas::~VisorFacturas()
{
    delete ui;
}
