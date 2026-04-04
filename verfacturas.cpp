#include "verfacturas.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include "imprimirfacturaproveedor.h"
#include "ui_verfacturas.h"

VerFacturas::VerFacturas(QString docType, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VerFacturas)
{
    ui->setupUi(this);
    tipoDocumento = docType;

    // Ajustes de UI según el tipo de documento
    if (tipoDocumento == "verifactu_logs") {
        this->setWindowTitle(tr("Logs de Verifactu (Encadenamiento de Facturas)"));
        ui->label_3->hide();
        ui->comboBoxProceedores->hide();
        ui->checkBoxTodosProveedores->hide();
        ui->pushButtonVerFactura->hide();
        ui->pushButtonPagar->hide();
    }

    llenarProveedores();
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());
    
    // Si es log, llenamos la tabla directamente tras configurar las fechas por defecto
    llenarTabla();
    
    idFactura = "";
}

VerFacturas::~VerFacturas()
{
    delete ui;
}

void VerFacturas::llenarProveedores()
{
    if (tipoDocumento == "verifactu_logs")
        return;

    ui->comboBoxProceedores->addItems(base->listadoProveedores(conf->getConexionLocal()));
}

void VerFacturas::llenarTabla()
{
    QString sentenciaSql;
    fechaInicial = ui->dateEditDesde->text();
    fechaFinal = ui->dateEditHasta->text();

    if (ui->dateEditDesde->date() > ui->dateEditHasta->date()) {
        QMessageBox msg;
        msg.setText("Error en la fecha");
        msg.setInformativeText("La fecha inicial no puede ser posterior a la final");
        msg.exec();
        return;
    }

    // Construcción de la sentencia SQL según el tipo de documento
    if (tipoDocumento == "verifactu_logs") {
        sentenciaSql = QString("SELECT id, id_factura, fecha_hora, hash_actual, usuario "
                               "FROM verifactu_logs "
                               "WHERE fecha_hora >= '%1 00:00:00' AND fecha_hora <= '%2 23:59:59' "
                               "ORDER BY id DESC")
                           .arg(fechaInicial)
                           .arg(fechaFinal);
    } else if (ui->checkBoxTodosProveedores->isChecked()) {
        if (tipoDocumento == "facturas") {
            sentenciaSql = QString("SELECT * FROM facturas WHERE fechaFactura >= '%1' "
                                   "AND fechaFactura <= '%2'")
                               .arg(fechaInicial)
                               .arg(fechaFinal);
        } else {
            sentenciaSql = QString("SELECT * FROM albaranes WHERE fechaFactura >= '%1' "
                                   "AND fechaFactura <= '%2'")
                               .arg(fechaInicial)
                               .arg(fechaFinal);
        }
    } else {
        idProveedor = base->idProveedor(ui->comboBoxProceedores->currentText(),
                                        conf->getConexionLocal());
        if (tipoDocumento == "facturas") {
            sentenciaSql = QString("SELECT * FROM facturas WHERE idProveedor = '%1' "
                                   "AND fechaFactura >= '%2' AND fechaFactura <= '%3'")
                               .arg(idProveedor)
                               .arg(fechaInicial)
                               .arg(fechaFinal);
        } else {
            sentenciaSql = QString("SELECT * FROM albaranes WHERE idProveedor = '%1' "
                                   "AND fechaFactura >= '%2' AND fechaFactura <= '%3'")
                               .arg(idProveedor)
                               .arg(fechaInicial)
                               .arg(fechaFinal);
        }
    }

    modeloTabla = new QStandardItemModel(this);
    QSqlQuery resultado = base->ejecutarSentencia(sentenciaSql, conf->getConexionLocal());
    qDebug() << "VerFacturas SQL:" << sentenciaSql;
    qDebug() << "Registros encontrados:" << resultado.numRowsAffected();

    if (tipoDocumento == "verifactu_logs") {
        // Llenado específico para logs de Verifactu
        int row = 0;
        while (resultado.next()) {
            modeloTabla->setItem(row, 0, new QStandardItem(resultado.value("id_factura").toString()));
            modeloTabla->setItem(row, 1, new QStandardItem(resultado.value("fecha_hora").toString()));
            modeloTabla->setItem(row, 2, new QStandardItem(resultado.value("hash_actual").toString()));
            
            QString nombreUsuario = base->nombreUsusario(resultado.value("usuario").toString(), 
                                                         conf->getConexionLocal());
            modeloTabla->setItem(row, 3, new QStandardItem(nombreUsuario));
            row++;
        }

        modeloTabla->setHorizontalHeaderLabels({"Nº Ticket", "Fecha / Hora", "Hash Encadenamiento", "Vendedor"});
    } else {
        // Llenado para facturas y albaranes de proveedores
        int row = 0;
        while (resultado.next()) {
            modeloTabla->setItem(row, 0, new QStandardItem(resultado.value("nFactura").toString()));
            modeloTabla->setItem(row, 1, new QStandardItem(resultado.value("fechaFactura").toString()));
            
            QString nombreProv = base->nombreProveedor(resultado.value("idProveedor").toString(),
                                                       conf->getConexionLocal());
            modeloTabla->setItem(row, 2, new QStandardItem(nombreProv));
            
            modeloTabla->setItem(row, 3, new QStandardItem(resultado.value("totalBase").toString()));
            modeloTabla->setItem(row, 4, new QStandardItem(resultado.value("totalIva").toString()));
            modeloTabla->setItem(row, 5, new QStandardItem(resultado.value("totalRe").toString()));
            modeloTabla->setItem(row, 6, new QStandardItem(resultado.value("total").toString()));

            if (tipoDocumento == "facturas") {
                modeloTabla->setItem(row, 7, new QStandardItem(resultado.value("vencimiento").toString()));
                modeloTabla->setItem(row, 8, new QStandardItem(resultado.value("pagada").toString()));
            } else {
                modeloTabla->setItem(row, 7, new QStandardItem(resultado.value("facturada").toString()));
            }
            row++;
        }

        QStringList etiquetas;
        if (tipoDocumento == "facturas") {
            etiquetas << "Nº Factura" << "Fecha" << "Proveedor" << "Base" << "I.V.A" << "R.E."
                      << "Total" << "Fecha vencimiento" << "Pagada";
        } else {
            etiquetas << "Nº Factura" << "Fecha" << "Proveedor" << "Base" << "I.V.A" << "R.E."
                      << "Total" << "Facturada";
        }
        modeloTabla->setHorizontalHeaderLabels(etiquetas);
    }

    ui->tableView->setModel(modeloTabla);
    ui->tableView->resizeColumnsToContents();
}

void VerFacturas::on_checkBoxTodosProveedores_clicked(bool checked)
{
    if (checked == false) {
        ui->comboBoxProceedores->setEnabled(true);
    } else {
        ui->comboBoxProceedores->setDisabled(true);
    }
}

void VerFacturas::on_pushButtonVer_clicked()
{
    idFactura = "";

    llenarTabla();
}

void VerFacturas::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTabla->index(index.row(), 0);
    idFactura = modeloTabla->data(indice, Qt::EditRole).toString();
    if (idFactura == "" || tipoDocumento == "verifactu_logs") {
        // En logs no hay visor de PDF de momento, se puede añadir un dial de detalles
        return;
    }

    imprimirFacturaProveedor factura(conf->getConexionLocal(), datos, idFactura);
}

void VerFacturas::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modeloTabla->index(index.row(), 0);
    idFactura = modeloTabla->data(indice, Qt::EditRole).toString();
    datos.clear();
    for (int i = 0; i < modeloTabla->columnCount(); i++) {
        indice = modeloTabla->index(index.row(), i);
        datos << modeloTabla->data(indice, Qt::EditRole).toString();
    }
}

void VerFacturas::on_pushButtonVerFactura_clicked()
{
    if (idFactura == "" || tipoDocumento == "verifactu_logs") {
        return;
    }

    imprimirFacturaProveedor factura(conf->getConexionLocal(), datos, idFactura);
}
