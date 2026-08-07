#include "verifactudialog.h"
#include "ui_verifactudialog.h"
#include "verifactuclass.h"
#include <QCoreApplication>
#include <QSettings>
#include <QFileDialog>
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QDateTime>
#include <QVBoxLayout>
#include <QLabel>
#include <QTextEdit>
#include <QDialogButtonBox>
#include <QFont>
#include <QDir>
#include <QDebug>
#include <QBrush>
#include <QColor>

/**
 * @brief Constructor de la clase VerifactuDialog.
 *        Configura la UI, inicializa fechas y carga los datos de configuración guardados.
 * @param parent Puntero al widget padre de Qt.
 */
VerifactuDialog::VerifactuDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VerifactuDialog)
{
    ui->setupUi(this);

    // Ajustamos la fecha inicial y final al día actual por defecto
    ui->dateEditDesde->setDate(QDate::currentDate());
    ui->dateEditHasta->setDate(QDate::currentDate());

    // Cargar los parámetros guardados de VeriFactu desde tienda.ini (SEC-01)
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("VeriFactu");
    ui->comboBoxModo->setCurrentIndex(settings.value("modo", 0).toInt());
    ui->comboBoxEntorno->setCurrentIndex(settings.value("entorno", 0).toInt());
    ui->lineEditNIF->setText(settings.value("emisor_nif", "").toString());
    ui->lineEditNombre->setText(settings.value("emisor_nombre", "").toString());
    ui->lineEditCertificado->setText(settings.value("certificado_ruta", "").toString());
    ui->lineEditClave->setText(settings.value("certificado_clave", "").toString());
    settings.endGroup();

    // Inicializamos el modelo para la visualización de logs en la tabla
    modeloTabla = new QStandardItemModel(this);
    ui->tableView->setModel(modeloTabla);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);

    // Rellenamos la tabla con los logs del día actual
    llenarTabla();
    idFactura = "";
}

/**
 * @brief Destructor de la clase VerifactuDialog.
 */
VerifactuDialog::~VerifactuDialog()
{
    delete ui;
}

/**
 * @brief Consulta la base de datos local y rellena la tabla de logs según
 *        el rango de fechas seleccionado.
 */
void VerifactuDialog::llenarTabla()
{
    modeloTabla->clear();

    QString fechaInicial = ui->dateEditDesde->text(); // El formato es "yyyy-MM-dd"
    QString fechaFinal = ui->dateEditHasta->text();   // El formato es "yyyy-MM-dd"

    // Validación del rango de fechas
    if (ui->dateEditDesde->date() > ui->dateEditHasta->date()) {
        QMessageBox::warning(this, tr("Rango de fechas erróneo"),
                             tr("La fecha inicial no puede ser posterior a la fecha final."));
        return;
    }

    // Consulta SQL para obtener los logs dentro del rango usando DATE_FORMAT para evitar problemas con Qt 6 y MariaDB
    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    QSqlQuery consulta(db);
    consulta.prepare("SELECT id, id_factura, DATE_FORMAT(fecha_hora, '%Y-%m-%d %H:%i:%s') AS fecha_hora_str, "
                     "hash_actual, usuario, estado_envio "
                     "FROM verifactu_logs "
                     "WHERE fecha_hora >= ? AND fecha_hora <= ? "
                     "ORDER BY id DESC");
    consulta.bindValue(0, fechaInicial + " 00:00:00");
    consulta.bindValue(1, fechaFinal + " 23:59:59");

    if (!consulta.exec()) {
        qDebug() << "VerifactuDialog SQL error:" << consulta.lastError().text();
    }
    QSqlQuery resultado = std::move(consulta);

    int row = 0;
    while (resultado.next()) {
        // Almacenamos el número de factura como texto visible y su ID de log único en el UserRole
        QStandardItem *itemFactura = new QStandardItem(resultado.value("id_factura").toString());
        itemFactura->setData(resultado.value("id"), Qt::UserRole);
        modeloTabla->setItem(row, 0, itemFactura);

        // Extraer fecha_hora_str ya formateada (yyyy-MM-dd HH:mm:ss)
        QString fechaHoraStr = resultado.value("fecha_hora_str").toString();
        if (fechaHoraStr.isEmpty()) {
            fechaHoraStr = resultado.value("fecha_hora").toString();
        }
        modeloTabla->setItem(row, 1, new QStandardItem(fechaHoraStr));
        modeloTabla->setItem(row, 2, new QStandardItem(resultado.value("hash_actual").toString()));
        
        // Convertimos el id de usuario a su nombre legible
        QString nombreUsuario = base.nombreUsusario(resultado.value("usuario").toString(), 
                                                     conf->getConexionLocal());
        modeloTabla->setItem(row, 3, new QStandardItem(nombreUsuario));

        // Mostramos el estado de remisión según la semántica de estado_envio:
        // 0 = Pendiente/Error (rojo), 1 = Enviado (verde), 2 = Aceptado con errores (ámbar),
        // 3 = Local/No remitido (azul).
        int estado = resultado.value("estado_envio").toInt();
        QString estadoText;
        QBrush estadoColor;
        switch (estado) {
        case verifactuClass::EstadoPendiente:
            estadoText = tr("Pendiente / Error");
            estadoColor = QBrush(QColor("#d32f2f")); // Rojo
            break;
        case verifactuClass::EstadoEnviado:
            estadoText = tr("Enviado");
            estadoColor = QBrush(QColor("#2e7d32")); // Verde
            break;
        case verifactuClass::EstadoAceptadoConErrores:
            estadoText = tr("Aceptado con errores");
            estadoColor = QBrush(QColor("#f57c00")); // Ámbar
            break;
        case verifactuClass::EstadoLocal:
            estadoText = tr("Local (no remitido)");
            estadoColor = QBrush(QColor("#1565c0")); // Azul
            break;
        default:
            estadoText = tr("Desconocido");
            estadoColor = QBrush(QColor("#616161")); // Gris
            break;
        }
        QStandardItem *estadoItem = new QStandardItem(estadoText);
        estadoItem->setForeground(estadoColor);
        modeloTabla->setItem(row, 4, estadoItem);
        row++;
    }

    // Cabeceras descriptivas de la tabla de logs (con columna de estado)
    modeloTabla->setHorizontalHeaderLabels({tr("Nº Ticket"), tr("Fecha / Hora"), tr("Hash Encadenamiento"), tr("Vendedor"), tr("Estado AEAT")});
    ui->tableView->resizeColumnsToContents();
}

/**
 * @brief Slot que se dispara al pulsar el botón "Ver Logs".
 */
void VerifactuDialog::on_pushButtonVer_clicked()
{
    idFactura = "";
    llenarTabla();
}

/**
 * @brief Slot para inspeccionar a detalle el log (doble clic). Abre una ventana con
 *        los hashes y la remisión XML oficial de la AEAT.
 * @param index Índice de la fila seleccionada.
 */
void VerifactuDialog::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex idx = modeloTabla->index(index.row(), 0);
    QVariant logIdVal = modeloTabla->data(idx, Qt::UserRole);
    idFactura = modeloTabla->data(idx, Qt::EditRole).toString();

    if (!logIdVal.isValid()) {
        return;
    }
    int logId = logIdVal.toInt();

    // Consultamos los datos criptográficos y XML del registro usando el ID único de log
    QSqlQuery query(QSqlDatabase::database(conf->getConexionLocal()));
    query.prepare("SELECT hash_actual, hash_anterior, cadena_firmada FROM verifactu_logs WHERE id = :id");
    query.bindValue(":id", logId);
    
    if (query.exec() && query.next()) {
        QString hashActual = query.value("hash_actual").toString();
        QString hashAnterior = query.value("hash_anterior").toString();
        QString xmlContent = query.value("cadena_firmada").toString();

        // Ventana de diálogo flotante para visualizar los detalles
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
        QMessageBox::warning(this, tr("Advertencia"), 
                             tr("No se encontraron detalles de VeriFactu para el registro %1 en la base de datos.")
                             .arg(logId));
    }
}

/**
 * @brief Abre un explorador de archivos para buscar y seleccionar el archivo de certificado digital.
 */
void VerifactuDialog::on_pushButtonCertificado_clicked()
{
    QString file = QFileDialog::getOpenFileName(this,
        tr("Seleccionar Certificado Digital"),
        QDir::homePath(),
        tr("Certificados (*.p12 *.pfx *.pem);;Todos los archivos (*.*)"));
    if (!file.isEmpty()) {
        ui->lineEditCertificado->setText(file);
    }
}

/**
 * @brief Guarda los datos de configuración en tienda.ini y cierra el diálogo.
 */
void VerifactuDialog::accept()
{
    // Guardar datos en el archivo de configuración tienda.ini (SEC-01)
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("VeriFactu");
    settings.setValue("modo", ui->comboBoxModo->currentIndex());
    settings.setValue("entorno", ui->comboBoxEntorno->currentIndex());
    settings.setValue("emisor_nif", ui->lineEditNIF->text().trimmed().toUpper());
    settings.setValue("emisor_nombre", ui->lineEditNombre->text().trimmed());
    settings.setValue("certificado_ruta", ui->lineEditCertificado->text().trimmed());
    settings.setValue("certificado_clave", ui->lineEditClave->text());
    settings.endGroup();
    settings.sync();

    QDialog::accept();
}
