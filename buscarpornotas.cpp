#include "buscarpornotas.h"
#include "ui_buscarpornotas.h"
#include <QSqlRecord>
#include <QDir>
#include <QPixmap>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>

// Constructor de la clase BuscarPorNotas
BuscarPorNotas::BuscarPorNotas(QWidget *parent, QSqlQuery query)
    : QDialog(parent)
    , ui(new Ui::BuscarPorNotas)
{
    ui->setupUi(this);
    this->query = std::move(query);
    modelo.setQuery(std::move(this->query));
    
    // Configurar el modelo proxy para búsquedas dinámicas e inteligentes
    proxyModel.setSourceModel(&modelo);
    ui->tableView->setModel(&proxyModel);
    
    int stockCol = modelo.query().record().indexOf("stock_total");
    int caduCol = modelo.query().record().indexOf("fecha_caducidad");

    modelo.setHeaderData(0, Qt::Horizontal, "Código");
    modelo.setHeaderData(1, Qt::Horizontal, "Descripción");
    modelo.setHeaderData(2, Qt::Horizontal, "PVP");
    if (stockCol != -1) modelo.setHeaderData(stockCol, Qt::Horizontal, "Stock");
    if (caduCol != -1) modelo.setHeaderData(caduCol, Qt::Horizontal, "Caducidad");

    // Ocultar columnas secundarias de la vista de tabla para mayor claridad
    for (int i = 0; i < modelo.columnCount(); ++i) {
        if (i == 0 || i == 1 || i == 2 || i == stockCol || i == caduCol) {
            ui->tableView->showColumn(i);
        } else {
            ui->tableView->hideColumn(i);
        }
    }

    ui->tableView->setColumnWidth(1, 320);
    if (stockCol != -1) ui->tableView->setColumnWidth(stockCol, 80);
    if (caduCol != -1) ui->tableView->setColumnWidth(caduCol, 100);

    // Conectar el cambio de selección en la tabla para actualizar la vista de detalles
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &BuscarPorNotas::selectionChanged);
    
    ui->lineEdit_buscar->setFocus();
}

BuscarPorNotas::~BuscarPorNotas()
{
    delete ui;
}

// Evento ejecutado al cambiar la fila seleccionada
void BuscarPorNotas::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    mostrarDetalles(current);
}

// Evento al hacer doble clic o pulsar Enter en una fila
void BuscarPorNotas::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel.mapToSource(index);
    QModelIndex idIndice = modelo.index(sourceIndex.row(), 0);
    resultado = modelo.data(idIndice, Qt::EditRole).toString();
    this->accept();
}

// Evento cuando cambia el texto del campo de búsqueda inteligente
void BuscarPorNotas::on_lineEdit_buscar_textChanged(const QString &arg1)
{
    // Re-ejecutar la consulta SQL optimizada con el nuevo texto para obtener resultados inmediatos desde la base de datos
    this->query = base.buscarPorNotas(QSqlDatabase::database(conf->getConexionLocal()), arg1);
    modelo.setQuery(std::move(this->query));
    proxyModel.setTextoFiltro(arg1);
    filtrar();
}

// Evento cuando cambia el estado del filtro de productos con stock
void BuscarPorNotas::on_checkBox_conStock_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    filtrar();
}

// Aplica los filtros activos sobre el modelo proxy
void BuscarPorNotas::filtrar()
{
    proxyModel.soloConStock = ui->checkBox_conStock->isChecked();
    proxyModel.actualizar();
}

// Muestra los detalles completos del producto seleccionado en el panel lateral
void BuscarPorNotas::mostrarDetalles(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QModelIndex sourceIndex = proxyModel.mapToSource(index);
    QSqlRecord record = modelo.record(sourceIndex.row());
    
    QString cod = record.value("cod").toString();
    QString desc = record.value("descripcion").toString();
    double pvp = record.value("pvp").toDouble();
    QString fotoPath = record.value("foto").toString();
    double stock = record.value("stock_total").toDouble();
    QString notas = record.value("notas").toString();
    QString formato = record.value("formato").toString();
    double cantFormato = record.value("cantformato").toDouble();

    resultado = cod;

    // Cargar la imagen del producto si existe
    if (!fotoPath.isEmpty()) {
        QString fullPath = base.resolverRutaImagen(fotoPath);
        QPixmap pixmap(fullPath);
        if (!pixmap.isNull()) {
            ui->label_foto->setPixmap(pixmap.scaled(ui->label_foto->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            ui->label_foto->setText("Foto no encontrada");
        }
    } else {
        ui->label_foto->setText("Sin foto");
    }

    QString caducidad = record.value("fecha_caducidad").toString();

    // Obtener desglose de lotes para el producto seleccionado
    QString desgloseLotes = "";
    QSqlQuery qLotes = base.lotesProducto(cod, conf->getConexionLocal());
    while (qLotes.next()) {
        QString l_lote = qLotes.value("lote").toString();
        QString l_fecha = qLotes.value("fecha").toString();
        double l_cant = qLotes.value("cantidad").toDouble();
        if (l_cant != 0) {
            desgloseLotes += QString("<br>&nbsp;&nbsp;• %1: <b>%2</b> uds %3")
                                 .arg(l_fecha)
                                 .arg(l_cant)
                                 .arg(l_lote.isEmpty() ? "" : " (Lote: " + l_lote + ")");
        }
    }

    if (desgloseLotes.isEmpty()) desgloseLotes = "<br>&nbsp;&nbsp;<i>Sin lotes registrados</i>";

    // Formatear información detallada con HTML
    QString info = QString("<b>Código:</b> %1<br>"
                           "<b>Descripción:</b> %2<br>"
                           "<b>PVP:</b> %3 €<br>"
                           "<b>Stock Total:</b> <font color='%4'>%5</font><br>"
                           "<b>Detalle por Caducidad:</b>%10<br>"
                           "<b>Caducidad próxima:</b> %9<br>"
                           "<b>Notas:</b> %6<br>"
                           "<b>Formato:</b> %7 (%8)")
                   .arg(cod)
                   .arg(desc)
                   .arg(QString::number(pvp, 'f', 2))
                   .arg(stock > 0 ? "green" : "red")
                   .arg(stock)
                   .arg(notas.isEmpty() ? "---" : notas)
                   .arg(formato)
                   .arg(cantFormato)
                   .arg(caducidad.isEmpty() ? "No disponible" : caducidad)
                   .arg(desgloseLotes);
    
    ui->label_info->setText(info);
}

// Cancela la búsqueda y limpia el resultado
void BuscarPorNotas::reject()
{
    resultado = "";
    QDialog::reject();
}

void BuscarPorNotas::on_pushButtonCancelar_clicked()
{
    this->reject();
}

void BuscarPorNotas::on_pushButtonAceptar_clicked()
{
    on_tableView_activated(ui->tableView->currentIndex());
}
