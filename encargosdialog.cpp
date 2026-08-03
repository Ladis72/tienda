#include "encargosdialog.h"
#include "ui_encargosdialog.h"
#include "buscarproducto.h"
#include "buscarcliente.h"
#include "base_datos.h"
#include "configuracion.h"
extern Configuracion *conf;

EncargosDialog::EncargosDialog(QString codCliente, QString nombreCliente, QString codArticulo, QString descArticulo, QString empleado, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EncargosDialog),
    m_codArticulo(codArticulo),
    m_descArticulo(descArticulo),
    m_codCliente(codCliente)
{
    ui->setupUi(this);
    
    // Rellenamos los datos
    if (codCliente.isEmpty() || codCliente == "0") {
        ui->lineEditCliente->setText("Cliente de Contado");
        m_codCliente = "0";
    } else {
        ui->lineEditCliente->setText(codCliente + " - " + nombreCliente);
    }
    
    if (codArticulo.isEmpty()) {
        ui->lineEditArticulo->setText("Seleccione primero un artículo en el TPV");
    } else {
        ui->lineEditArticulo->setText(codArticulo + " - " + descArticulo);
    }
    
    ui->lineEditEmpleado->setText(empleado);

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

int EncargosDialog::getCantidad() const
{
    return ui->spinBoxCantidad->value();
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

void EncargosDialog::on_btnBuscarArticulo_clicked()
{
    baseDatos base;
    QSqlQuery consulta = base.buscarEnTabla(QSqlDatabase::database(conf->getConexionLocal()), "articulos", "cod", "");
    BuscarProducto *bb = new BuscarProducto(this, consulta);
    bb->exec();
    if (!bb->resultado.isEmpty()) {
        m_codArticulo = bb->resultado;
        QSqlQuery qArt(QSqlDatabase::database(conf->getConexionLocal()));
        qArt.prepare("SELECT descripcion FROM articulos WHERE cod = ?");
        qArt.bindValue(0, m_codArticulo);
        if (qArt.exec() && qArt.next()) {
            m_descArticulo = qArt.value(0).toString();
            ui->lineEditArticulo->setText(m_codArticulo + " - " + m_descArticulo);
        } else {
            m_descArticulo = "Artículo seleccionado";
            ui->lineEditArticulo->setText(m_codArticulo + " - " + m_descArticulo);
        }
    }
    delete bb;
}

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
