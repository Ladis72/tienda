#include "dialoganadirapedido.h"
#include "ui_dialoganadirapedido.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QDate>
#include "pedidonuevo.h"
#include "configuracion.h"

extern Configuracion *conf; // Variable de configuracion global

DialogAnadirAPedido::DialogAnadirAPedido(QString codArticulo, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogAnadirAPedido)
    , m_codArticulo(codArticulo)
{
    ui->setupUi(this);
    base = new baseDatos();
    llenarComboPedidos();
}

DialogAnadirAPedido::~DialogAnadirAPedido()
{
    delete base;
    delete ui;
}

void DialogAnadirAPedido::llenarComboPedidos()
{
    ui->comboBoxPedidos->clear();
    QSqlQuery consulta = base->recuperarPedidos(conf->getConexionLocal());
    while (consulta.next()) {
        QString idPedido = consulta.value(0).toString();
        QString idProveedor = consulta.value(1).toString();
        QString albaran = consulta.value(2).toString();
        QString fecha = consulta.value(3).toString();
        QString nombreProveedor = base->nombreProveedor(idProveedor, conf->getConexionLocal());
        
        QString texto = QString("%1 - %2 - %3").arg(nombreProveedor).arg(albaran).arg(fecha);
        ui->comboBoxPedidos->addItem(texto, QVariant(idPedido));
    }
}

void DialogAnadirAPedido::on_pushButtonNuevo_clicked()
{
    PedidoNuevo *nuevo = new PedidoNuevo(this);
    nuevo->exec();
    delete nuevo;
    llenarComboPedidos(); 
}

void DialogAnadirAPedido::on_pushButtonAceptar_clicked()
{
    if (ui->comboBoxPedidos->currentIndex() == -1) {
        QMessageBox::warning(this, "Aviso", "No hay ningún pedido seleccionado");
        return;
    }
    QString idPedido = ui->comboBoxPedidos->currentData().toString();
    
    // Obtenemos los datos del producto para añadirlos al pedido seleccionado
    QSqlRecord registro = base->consulta_producto(conf->getConexionLocal(), m_codArticulo);
    if (registro.isEmpty()) {
        QMessageBox::critical(this, "Error", "No se puede añadir: Producto no encontrado en la base de datos.");
        return;
    }
    
    double pvp = registro.value("pvp").toDouble();
    double pvt = registro.value("precio_compra").toDouble();
    if (pvt == 0) pvt = registro.value("precio_venta").toDouble();
    
    QString tipoIva = registro.value("iva").toString();
    if (tipoIva.isEmpty()) tipoIva = "21"; // Valor por defecto
    
    // Configurar unidad por defecto
    double cantidad = ui->doubleSpinBoxCantidad->value();
    double bonificacion = ui->doubleSpinBoxBonificacion->value();
    
    QString paramUds = QString::number(cantidad, 'f', 2);
    QString paramBonif = QString::number(bonificacion, 'f', 2);
    QString paramLote = "";
    QString paramFecha = QDate::currentDate().toString("yyyy-MM-dd");
    QString paramCosto = QString::number(pvt, 'f', 4);
    QString paramDescuento = "0";
    
    double baseTotal = pvt * cantidad;
    QString paramBase = QString::number(baseTotal, 'f', 4);
    
    // Calcular impuestos
    double ivaVal = 0;
    double reVal = 0;
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare("SELECT porcentaje, recargo FROM impuestos WHERE porcentaje = :por");
    q.bindValue(":por", tipoIva);
    if (q.exec() && q.first()) {
        ivaVal = baseTotal * q.value(0).toDouble() / 100.0;
        reVal = baseTotal * q.value(1).toDouble() / 100.0;
    } else {
        ivaVal = baseTotal * tipoIva.toDouble() / 100.0;
    }

    QStringList datos;
    datos.append(idPedido);
    datos.append(m_codArticulo);
    datos.append(registro.value("descripcion").toString());
    datos.append(paramUds);
    datos.append(paramBonif);
    datos.append(paramLote);
    datos.append(paramFecha);
    datos.append(paramCosto);
    datos.append(paramDescuento);
    datos.append(paramBase);
    datos.append(tipoIva);
    datos.append(QString::number(baseTotal, 'f', 2));
    datos.append(QString::number(ivaVal, 'f', 2));
    datos.append(QString::number(reVal, 'f', 2));
    datos.append(QString::number(pvp, 'f', 2));
    
    // Añadimos linea de pedido llamando a base de datos
    base->grabarLineaPedido(conf->getConexionLocal(), datos);
    
    QMessageBox::information(this, "Éxito", QString("Artículo añadido al pedido correctamente.\n(Uds: %1)").arg(cantidad));
    accept();
}

void DialogAnadirAPedido::on_pushButtonCancelar_clicked()
{
    reject();
}
