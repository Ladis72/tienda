#include "verfacturas.h"
#include <QMessageBox>
#include <QStandardItemModel>
#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QFont>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include "imprimirfacturaproveedor.h"
#include "dialogcambiarproveedor.h"
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
        ui->pushButtonCambiarProveedor->hide();
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
    fechaInicial = ui->dateEditDesde->text();
    fechaFinal = ui->dateEditHasta->text();

    if (ui->dateEditDesde->date() > ui->dateEditHasta->date()) {
        QMessageBox msg;
        msg.setText("Error en la fecha");
        msg.setInformativeText("La fecha inicial no puede ser posterior a la final");
        msg.exec();
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery consulta(db);

    // Construcción de la sentencia SQL según el tipo de documento.
    // Se usa DATE_FORMAT para garantizar el formato 'yyyy-MM-dd' e impedir errores del protocolo binario en Qt 6.
    if (tipoDocumento == "verifactu_logs") {
        consulta.prepare("SELECT id, id_factura, DATE_FORMAT(fecha_hora, '%Y-%m-%d %H:%i:%s') AS fecha_hora_str, "
                         "hash_actual, usuario "
                         "FROM verifactu_logs "
                         "WHERE fecha_hora >= ? AND fecha_hora <= ? "
                         "ORDER BY id DESC");
        consulta.bindValue(0, fechaInicial + " 00:00:00");
        consulta.bindValue(1, fechaFinal + " 23:59:59");
    } else if (ui->checkBoxTodosProveedores->isChecked()) {
        if (tipoDocumento == "facturas") {
            consulta.prepare("SELECT *, DATE_FORMAT(fechaFactura, '%Y-%m-%d') AS fecha_str, "
                             "DATE_FORMAT(vencimiento, '%Y-%m-%d') AS vencimiento_str "
                             "FROM facturas WHERE fechaFactura >= ? AND fechaFactura <= ?");
        } else {
            consulta.prepare("SELECT *, DATE_FORMAT(fechaFactura, '%Y-%m-%d') AS fecha_str "
                             "FROM albaranes WHERE fechaFactura >= ? AND fechaFactura <= ?");
        }
        consulta.bindValue(0, fechaInicial);
        consulta.bindValue(1, fechaFinal);
    } else {
        idProveedor = base->idProveedor(ui->comboBoxProceedores->currentText(),
                                        conf->getConexionLocal());
        if (tipoDocumento == "facturas") {
            consulta.prepare("SELECT *, DATE_FORMAT(fechaFactura, '%Y-%m-%d') AS fecha_str, "
                             "DATE_FORMAT(vencimiento, '%Y-%m-%d') AS vencimiento_str "
                             "FROM facturas WHERE idProveedor = ? "
                             "AND fechaFactura >= ? AND fechaFactura <= ?");
        } else {
            consulta.prepare("SELECT *, DATE_FORMAT(fechaFactura, '%Y-%m-%d') AS fecha_str "
                             "FROM albaranes WHERE idProveedor = ? "
                             "AND fechaFactura >= ? AND fechaFactura <= ?");
        }
        consulta.bindValue(0, idProveedor);
        consulta.bindValue(1, fechaInicial);
        consulta.bindValue(2, fechaFinal);
    }

    if (!consulta.exec()) {
        qDebug() << "VerFacturas SQL error:" << consulta.lastError().text();
    }

    modeloTabla = new QStandardItemModel(this);
    QSqlQuery resultado = std::move(consulta);
    qDebug() << "Registros encontrados:" << resultado.numRowsAffected();

    if (tipoDocumento == "verifactu_logs") {
        // Llenado específico para logs de Verifactu (usando fecha_hora_str)
        int row = 0;
        while (resultado.next()) {
            modeloTabla->setItem(row, 0, new QStandardItem(resultado.value("id_factura").toString()));
            modeloTabla->setItem(row, 1, new QStandardItem(resultado.value("fecha_hora_str").toString()));
            modeloTabla->setItem(row, 2, new QStandardItem(resultado.value("hash_actual").toString()));
            
            QString nombreUsuario = base->nombreUsusario(resultado.value("usuario").toString(), 
                                                         conf->getConexionLocal());
            modeloTabla->setItem(row, 3, new QStandardItem(nombreUsuario));
            row++;
        }

        modeloTabla->setHorizontalHeaderLabels({"Nº Ticket", "Fecha / Hora", "Hash Encadenamiento", "Vendedor"});
    } else {
        // Llenado para facturas y albaranes de proveedores (usando fecha_str en yyyy-MM-dd)
        int row = 0;
        while (resultado.next()) {
            modeloTabla->setItem(row, 0, new QStandardItem(resultado.value("nFactura").toString()));
            
            // Extraer la fecha ya formateada como yyyy-MM-dd
            QString fechaFormateada = resultado.value("fecha_str").toString();
            if (fechaFormateada.isEmpty()) {
                fechaFormateada = resultado.value("fechaFactura").toString();
            }
            modeloTabla->setItem(row, 1, new QStandardItem(fechaFormateada));
            
            QString nombreProv = base->nombreProveedor(resultado.value("idProveedor").toString(),
                                                       conf->getConexionLocal());
            modeloTabla->setItem(row, 2, new QStandardItem(nombreProv));
            
            modeloTabla->setItem(row, 3, new QStandardItem(resultado.value("totalBase").toString()));
            modeloTabla->setItem(row, 4, new QStandardItem(resultado.value("totalIva").toString()));
            modeloTabla->setItem(row, 5, new QStandardItem(resultado.value("totalRe").toString()));
            modeloTabla->setItem(row, 6, new QStandardItem(resultado.value("total").toString()));

            if (tipoDocumento == "facturas") {
                QString vencFormateado = resultado.value("vencimiento_str").toString();
                if (vencFormateado.isEmpty()) {
                    vencFormateado = resultado.value("vencimiento").toString();
                }
                modeloTabla->setItem(row, 7, new QStandardItem(vencFormateado));
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
    if (idFactura == "") {
        return;
    }

    // Si es log de VeriFactu, mostramos el diálogo de detalles con el XML y los hashes
    if (tipoDocumento == "verifactu_logs") {
        QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
        query.prepare("SELECT hash_actual, hash_anterior, cadena_firmada FROM verifactu_logs WHERE id_factura = :id_factura");
        query.bindValue(":id_factura", idFactura.toInt());
        
        if (query.exec() && query.next()) {
            QString hashActual = query.value("hash_actual").toString();
            QString hashAnterior = query.value("hash_anterior").toString();
            QString xmlContent = query.value("cadena_firmada").toString();

            // Ventana de diálogo personalizada para mostrar el detalle del envío
            QDialog *dial = new QDialog(this);
            dial->setWindowTitle(tr("Detalles de Remisión VeriFactu - Ticket %1").arg(idFactura));
            dial->resize(750, 550);

            QVBoxLayout *layout = new QVBoxLayout(dial);
            
            QLabel *lblHashes = new QLabel(dial);
            lblHashes->setText(tr("<b>Hash Actual (Huella):</b> <code style='color:#0055ff;'>%1</code><br>"
                                  "<b>Hash Anterior (Encadenamiento):</b> <code>%2</code>")
                               .arg(hashActual, hashAnterior));
            lblHashes->setTextInteractionFlags(Qt::TextSelectableByMouse);
            layout->addWidget(lblHashes);

            QLabel *lblXml = new QLabel(tr("<b>Contenido XML enviado a la AEAT:</b>"), dial);
            layout->addWidget(lblXml);

            QTextEdit *txtXml = new QTextEdit(dial);
            txtXml->setReadOnly(true);
            txtXml->setPlainText(xmlContent);
            txtXml->setFont(QFont("Monospace", 9));
            layout->addWidget(txtXml);

            QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Close, dial);
            connect(btnBox, &QDialogButtonBox::rejected, dial, &QDialog::close);
            layout->addWidget(btnBox);

            dial->exec();
            delete dial;
        } else {
            QMessageBox::warning(this, tr("Advertencia"), tr("No se encontraron registros de VeriFactu en la base de datos para el ticket %1.").arg(idFactura));
        }
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

/**
 * @brief Permite a los administradores modificar el proveedor asignado al documento/factura seleccionado.
 */
void VerFacturas::on_pushButtonCambiarProveedor_clicked()
{
    // Restricción: Solo administradores (Rol 0) pueden modificar proveedores de facturas
    if (conf->getRol() != 0) {
        QMessageBox::warning(this, tr("Acceso denegado"),
                             tr("Solo los administradores pueden cambiar el proveedor de una factura o documento."));
        return;
    }

    if (idFactura.isEmpty() || tipoDocumento == "verifactu_logs") {
        QMessageBox::information(this, tr("Aviso"),
                                 tr("Seleccione un documento o factura de la tabla primero."));
        return;
    }

    // Obtener el proveedor actual a partir de la fila seleccionada (columna 2 es el nombre del proveedor)
    QString nombreProveedorActual = "";
    if (datos.size() > 2) {
        nombreProveedorActual = datos.at(2);
    }
    QString idProveedorViejo = base->idProveedor(nombreProveedorActual, conf->getConexionLocal());

    DialogCambiarProveedor dialog(idFactura, nombreProveedorActual, this);
    if (dialog.exec() == QDialog::Accepted) {
        QString idNuevo = dialog.getIdProveedorNuevo();
        QString nombreNuevo = dialog.getNombreProveedorNuevo();

        if (base->cambiarProveedorFactura(conf->getConexionLocal(), idFactura, idProveedorViejo, idNuevo)) {
            QMessageBox::information(this, tr("Proveedor actualizado"),
                                     tr("Se ha actualizado el proveedor del documento '%1' a '%2' correctamente.")
                                         .arg(idFactura, nombreNuevo));
            idFactura = "";
            datos.clear();
            llenarTabla();
        } else {
            QMessageBox::critical(this, tr("Error"),
                                  tr("No se pudo actualizar el proveedor en la base de datos."));
        }
    }
}

void VerFacturas::on_pushButtonCerrar_clicked()
{
    close();
}
