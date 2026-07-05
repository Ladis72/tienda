#include "visorlog.h"
#include "ui_visorlog.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QTextDocument>
#include <QPrinter>
#include <QProcess>
#include <QSqlRecord>
#include <QDate>
#include <QSortFilterProxyModel>

// Variable del modelo proxy de filtrado para búsqueda en tiempo real
static QSortFilterProxyModel *proxyModel = nullptr;

/******************************************************************************
 * CONSTRUCTOR
 * Inicializa la UI, configura el rango de fechas al mes en curso,
 * carga dinámicamente los combos y conecta el buscador en tiempo real.
 ******************************************************************************/
VisorLog::VisorLog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::VisorLog)
    , modeloTabla(nullptr)
{
    ui->setupUi(this);
    
    // Configurar proxy model para filtrado local en tiempo real del mensaje
    proxyModel = new QSortFilterProxyModel(this);
    
    // Configurar fechas por defecto: desde el día 1 del mes actual hasta hoy
    QDate hoy = QDate::currentDate();
    ui->dateEditDesde->setDate(QDate(hoy.year(), hoy.month(), 1));
    ui->dateEditHasta->setDate(hoy);

    // Cargar listas desplegables de categorías y usuarios desde la BD
    cargarCombos();

    // Carga inicial de logs en la tabla
    on_pushButtonFiltrar_clicked();

    // Conectar el buscador para que filtre dinámicamente en tiempo real
    connect(ui->lineEditBuscar, &QLineEdit::textChanged, this, [this](const QString &text) {
        if (proxyModel) {
            proxyModel->setFilterFixedString(text.trimmed());
        }
    });

    // Los botones se conectan de forma automática a través del mecanismo on_objeto_senal de Qt setupUi(this)

    // Restringir el botón de limpieza solo a administradores o usuarios con permiso granular
    bool tienePermisoLimpieza = conf->permisos()->tiene("visor_log.limpiar");
    ui->pushButtonLimpiar->setVisible(tienePermisoLimpieza);
    ui->pushButtonLimpiar->setEnabled(tienePermisoLimpieza);
}

/******************************************************************************
 * DESTRUCTOR
 * Libera la memoria de la interfaz generada por Qt.
 ******************************************************************************/
VisorLog::~VisorLog()
{
    delete ui;
}

/******************************************************************************
 * CARGA DE COMBOS DINÁMICOS
 * Consulta las tablas de logs para obtener categorías y usuarios reales.
 ******************************************************************************/
void VisorLog::cargarCombos()
{
    // 1. Cargar Categorías
    ui->comboBoxCategoria->clear();
    ui->comboBoxCategoria->addItem(tr("Todas"), "");

    QSqlQuery qCat = base.ejecutarSentencia(
        "SELECT DISTINCT categoria FROM logs WHERE categoria IS NOT NULL AND categoria != '' ORDER BY categoria",
        conf->getConexionLocal()
    );
    while (qCat.next()) {
        QString cat = qCat.value(0).toString();
        ui->comboBoxCategoria->addItem(cat, cat);
    }

    // 2. Cargar Usuarios
    ui->comboBoxUsuario->clear();
    ui->comboBoxUsuario->addItem(tr("Todos"), "");

    QSqlQuery qUsr = base.ejecutarSentencia(
        "SELECT DISTINCT usuario FROM logs WHERE usuario IS NOT NULL AND usuario != '' ORDER BY usuario",
        conf->getConexionLocal()
    );
    while (qUsr.next()) {
        QString usr = qUsr.value(0).toString();
        ui->comboBoxUsuario->addItem(usr, usr);
    }
}

/******************************************************************************
 * APLICAR FILTROS Y CARGAR DATOS
 * Obtiene los parámetros de la UI, llama a baseDatos::consultarLog y los carga.
 ******************************************************************************/
void VisorLog::on_pushButtonFiltrar_clicked()
{
    QString categoria = ui->comboBoxCategoria->currentData().toString();
    QString usuario = ui->comboBoxUsuario->currentData().toString();
    
    // Fechas en formato "yyyy-MM-dd" según la regla confqt.md
    QDateTime desde = QDateTime(ui->dateEditDesde->date(), QTime(0, 0, 0));
    QDateTime hasta = QDateTime(ui->dateEditHasta->date(), QTime(23, 59, 59));

    // Consultar logs a la capa de datos
    QSqlQueryModel *nuevoModelo = base.consultarLog(
        conf->getConexionLocal(),
        categoria,
        usuario,
        desde,
        hasta
    );

    if (nuevoModelo) {
        // Eliminar modelo anterior de forma segura para evitar memory leaks
        if (modeloTabla) {
            modeloTabla->deleteLater();
        }
        modeloTabla = nuevoModelo;
        
        // Asignar el modelo fuente al proxy
        proxyModel->setSourceModel(modeloTabla);
        proxyModel->setFilterKeyColumn(4); // Columna 4 es la descripción/mensaje
        proxyModel->setFilterCaseSensitivity(Qt::CaseInsensitive);
        
        // Aplicar el texto de búsqueda actual si existe
        proxyModel->setFilterFixedString(ui->lineEditBuscar->text().trimmed());

        // Asignar proxy a la vista de la tabla
        ui->tableView->setModel(proxyModel);
        configurarTabla();
    } else {
        QMessageBox::critical(this, tr("Error"), tr("No se pudieron cargar los registros de log de la base de datos."));
    }
}

/******************************************************************************
 * CONFIGURAR TABLA
 * Ajusta los anchos de columna para optimizar la visualización de los mensajes.
 ******************************************************************************/
void VisorLog::configurarTabla()
{
    if (ui->tableView->horizontalHeader()) {
        ui->tableView->setColumnWidth(0, 60);  // ID
        ui->tableView->setColumnWidth(1, 150); // Fecha
        ui->tableView->setColumnWidth(2, 100); // Categoría
        ui->tableView->setColumnWidth(3, 100); // Usuario
        
        // El mensaje ocupa todo el espacio restante
        ui->tableView->horizontalHeader()->setSectionResizeMode(4, QHeaderView::Stretch);
    }
}

/******************************************************************************
 * EXPORTAR A CSV
 * Guarda los logs filtrados en un archivo CSV separado por punto y coma.
 ******************************************************************************/
void VisorLog::on_pushButtonCSV_clicked()
{
    if (!proxyModel || proxyModel->rowCount() == 0) {
        QMessageBox::information(this, tr("Exportar CSV"), tr("No hay datos cargados para exportar."));
        return;
    }

    // Ruta de guardado por defecto en la carpeta de documentos configurada
    QString dirDefecto = base.devolverDirectorio("documentos");
    if (dirDefecto.isEmpty()) {
        dirDefecto = QDir::homePath();
    }

    QString nombreArchivo = QFileDialog::getSaveFileName(
        this,
        tr("Guardar exportación de logs"),
        dirDefecto + "/logs_export.csv",
        tr("Archivos CSV (*.csv)")
    );

    if (nombreArchivo.isEmpty()) {
        return; // Cancelado por el usuario
    }

    QFile archivo(nombreArchivo);
    if (!archivo.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo abrir el archivo para escribir los datos."));
        return;
    }

    QTextStream out(&archivo);
    // Codificación UTF-8 para caracteres especiales
    out.setEncoding(QStringConverter::Utf8);

    // Escribir cabecera CSV
    out << "ID;Fecha;Categoria;Usuario;Mensaje\n";

    // Escribir los registros visibles en el proxy (respetando filtros de búsqueda)
    for (int r = 0; r < proxyModel->rowCount(); ++r) {
        QString id = proxyModel->data(proxyModel->index(r, 0)).toString();
        QString fecha = proxyModel->data(proxyModel->index(r, 1)).toString();
        QString cat = proxyModel->data(proxyModel->index(r, 2)).toString();
        QString usr = proxyModel->data(proxyModel->index(r, 3)).toString();
        QString msg = proxyModel->data(proxyModel->index(r, 4)).toString();

        // Escapar comillas dobles y saltos de línea para el estándar CSV
        msg.replace("\"", "\"\"");
        msg.replace("\n", " ");
        msg.replace("\r", "");

        out << QString("%1;\"%2\";\"%3\";\"%4\";\"%5\"\n")
               .arg(id)
               .arg(fecha)
               .arg(cat)
               .arg(usr)
               .arg(msg);
    }

    archivo.close();
    QMessageBox::information(this, tr("Éxito"), tr("Los datos se han exportado correctamente a:\n%1").arg(nombreArchivo));
}

/******************************************************************************
 * IMPRIMIR INFORME PDF
 * Diseña un informe estructurado HTML y lo exporta a PDF abriéndolo de inmediato.
 ******************************************************************************/
void VisorLog::on_pushButtonImprimir_clicked()
{
    if (!proxyModel || proxyModel->rowCount() == 0) {
        QMessageBox::information(this, tr("Imprimir"), tr("No hay datos para generar el informe."));
        return;
    }

    // Cabecera HTML atractiva usando la paleta del estilo global
    QString html = R"(
<html>
<head>
  <meta charset='utf-8'>
  <style>
    body { font-family: Arial, sans-serif; font-size: 9pt; color: #333; }
    .header { text-align: center; padding: 10px; border-bottom: 2px solid #2c3e50; margin-bottom: 20px; }
    h2 { color: #2c3e50; margin-bottom: 5px; }
    table { width: 100%; border-collapse: collapse; margin-top: 10px; }
    th { background-color: #2c3e50; color: white; padding: 6px; font-size: 8pt; text-align: left; }
    td { border: 1px solid #ccc; padding: 5px; font-size: 8pt; vertical-align: top; }
    .col-id { width: 5%; }
    .col-fecha { width: 15%; }
    .col-cat { width: 10%; }
    .col-usr { width: 10%; }
    .col-msg { width: 60%; }
  </style>
</head>
<body>
  <div class='header'>
    <h2>Informe de Auditoría de logs</h2>
    <div>Desde: %DESDE% | Hasta: %HASTA% | Categoría: %CAT% | Usuario: %USR%</div>
  </div>

  <table>
    <thead>
      <tr>
        <th class='col-id'>ID</th>
        <th class='col-fecha'>FECHA</th>
        <th class='col-cat'>CATEGORÍA</th>
        <th class='col-usr'>USUARIO</th>
        <th class='col-msg'>MENSAJE</th>
      </tr>
    </thead>
    <tbody>
      %FILAS%
    </tbody>
  </table>
</body>
</html>
)";

    // Reemplazar filtros en el HTML
    html.replace("%DESDE%", ui->dateEditDesde->text());
    html.replace("%HASTA%", ui->dateEditHasta->text());
    html.replace("%CAT%", ui->comboBoxCategoria->currentText());
    html.replace("%USR%", ui->comboBoxUsuario->currentText());

    // Generar filas
    QString filas = "";
    for (int r = 0; r < proxyModel->rowCount(); ++r) {
        filas += "<tr>";
        filas += "<td>" + proxyModel->data(proxyModel->index(r, 0)).toString() + "</td>";
        filas += "<td>" + proxyModel->data(proxyModel->index(r, 1)).toString() + "</td>";
        filas += "<td>" + proxyModel->data(proxyModel->index(r, 2)).toString() + "</td>";
        filas += "<td>" + proxyModel->data(proxyModel->index(r, 3)).toString() + "</td>";
        filas += "<td>" + proxyModel->data(proxyModel->index(r, 4)).toString().toHtmlEscaped() + "</td>";
        filas += "</tr>";
    }
    html.replace("%FILAS%", filas);

    QTextDocument document;
    document.setHtml(html);

    // Obtener ruta destino
    QString dirDefecto = base.devolverDirectorio("documentos");
    if (dirDefecto.isEmpty()) {
        dirDefecto = QDir::homePath();
    }
    QString rutaPdf = dirDefecto + "/Informe_Auditoria_Logs.pdf";

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName(rutaPdf);
    document.print(&printer);

    // Abrir automáticamente el visor de PDF nativo
    QProcess::startDetached("xdg-open", QStringList() << rutaPdf);
}

/******************************************************************************
 * PURGA DE LOGS ANTIGUOS
 * Borra logs de más de 30 días con confirmación y auditoría automática.
 ******************************************************************************/
void VisorLog::on_pushButtonLimpiar_clicked()
{
    // Confirmación de seguridad
    int respuesta = QMessageBox::warning(
        this,
        tr("Purga de Logs antiguos"),
        tr("¿Está seguro de que desea eliminar permanentemente todos los logs con una antigüedad superior a 30 días?\n\nEsta acción no se puede deshacer."),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No
    );

    if (respuesta != QMessageBox::Yes) {
        return;
    }

    // Borrado físico de logs anteriores a 30 días
    QSqlQuery query = base.ejecutarSentencia(
        "DELETE FROM logs WHERE timestamp < DATE_SUB(NOW(), INTERVAL 30 DAY)",
        conf->getConexionLocal()
    );

    if (query.lastError().isValid()) {
        QMessageBox::critical(this, tr("Error"), tr("No se pudo realizar la limpieza:\n%1").arg(query.lastError().text()));
    } else {
        int filasAfectadas = query.numRowsAffected();
        
        // Registrar la acción de limpieza en el log de auditoría
        base.insertarLog(
            conf->getConexionLocal(),
            "Advertencia",
            conf->getUsuario(),
            QString("Limpieza de logs antiguos realizada. Registros eliminados: %1").arg(filasAfectadas)
        );

        QMessageBox::information(
            this,
            tr("Limpieza completada"),
            tr("Se han eliminado correctamente %1 registros de logs antiguos.").arg(filasAfectadas)
        );

        // Recargar combos y tabla
        cargarCombos();
        on_pushButtonFiltrar_clicked();
    }
}
