#include "buscarproducto.h"
#include <QSqlRecord>
#include <QDir>
#include <QPixmap>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include "ui_buscarproducto.h"

BuscarProducto::BuscarProducto(QWidget *parent, QSqlQuery query)
    : QDialog(parent)
    , ui(new Ui::BuscarProducto)
{
    ui->setupUi(this);
    this->query = query;
    modelo.setQuery(query);
    
    proxyModel.setSourceModel(&modelo);
    proxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel.setFilterKeyColumn(-1); // Search in all columns
    
    ui->tableView->setModel(&proxyModel);
    
    int stockCol = modelo.query().record().indexOf("stock_total");
    int caduCol = modelo.query().record().indexOf("fecha_caducidad");

    modelo.setHeaderData(0, Qt::Horizontal, "Código");
    modelo.setHeaderData(1, Qt::Horizontal, "Descripción");
    modelo.setHeaderData(2, Qt::Horizontal, "PVP");
    if (stockCol != -1) modelo.setHeaderData(stockCol, Qt::Horizontal, "Stock");
    if (caduCol != -1) modelo.setHeaderData(caduCol, Qt::Horizontal, "Caducidad");

    // Configuración de columnas: Ocultar todo excepto Cod, Desc, PVP, Stock y Caducidad
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


    
    // Connect selection change to update details (keyboard & mouse)
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &BuscarProducto::selectionChanged);
    
    ui->lineEdit_buscar->setFocus();
}

BuscarProducto::~BuscarProducto()
{
    delete ui;
}

void BuscarProducto::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    mostrarDetalles(current);
}

void BuscarProducto::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel.mapToSource(index);
    QModelIndex idIndice = modelo.index(sourceIndex.row(), 0);
    resultado = modelo.data(idIndice, Qt::EditRole).toString();
    this->accept();
}

void BuscarProducto::on_lineEdit_buscar_textChanged(const QString &arg1)
{
    proxyModel.setFilterFixedString(arg1);
    filtrar(); // Trigger stock filter as well
}

void BuscarProducto::on_checkBox_conStock_stateChanged(int arg1)
{
    Q_UNUSED(arg1);
    filtrar();
}

void BuscarProducto::filtrar()
{
    proxyModel.soloConStock = ui->checkBox_conStock->isChecked();
    proxyModel.actualizar();
}

void BuscarProducto::mostrarDetalles(const QModelIndex &index)
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

    // Cargar Foto
    if (!fotoPath.isEmpty()) {
        QString fullPath = QDir::currentPath() + "/" + fotoPath;
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

    // Obtener desglose de lotes
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

    // Mostrar Info
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




void BuscarProducto::on_pushButtonCancelar_clicked()
{
    resultado = "";
    this->reject();
}


void BuscarProducto::on_pushButtonAceptar_clicked()
{
    on_tableView_activated(ui->tableView->currentIndex());
}

