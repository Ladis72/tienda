#include "buscarcliente.h"
#include <QSqlRecord>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include "ui_buscarcliente.h"

BuscarCliente::BuscarCliente(QWidget *parent, QSqlQuery query)
    : QDialog(parent)
    , ui(new Ui::BuscarCliente)
{
    ui->setupUi(this);
    this->query = std::move(query);
    modelo.setQuery(this->query);
    
    proxyModel.setSourceModel(&modelo);
    proxyModel.setFilterCaseSensitivity(Qt::CaseInsensitive);
    proxyModel.setFilterKeyColumn(-1); // Search in all columns
    
    ui->tableView->setModel(&proxyModel);
    
    int idCol = modelo.query().record().indexOf("idCliente");
    int nomCol = modelo.query().record().indexOf("nombre");
    int apeCol = modelo.query().record().indexOf("apellidos");
    int nifCol = modelo.query().record().indexOf("nif");
    int tlfCol = modelo.query().record().indexOf("telefono");

    if (idCol != -1) modelo.setHeaderData(idCol, Qt::Horizontal, "ID");
    if (nomCol != -1) modelo.setHeaderData(nomCol, Qt::Horizontal, "Nombre");
    if (apeCol != -1) modelo.setHeaderData(apeCol, Qt::Horizontal, "Apellidos");
    if (nifCol != -1) modelo.setHeaderData(nifCol, Qt::Horizontal, "NIF");
    if (tlfCol != -1) modelo.setHeaderData(tlfCol, Qt::Horizontal, "Teléfono");

    // Configuración de columnas: Ocultar todo excepto ID, Nombre, Apellidos, NIF y Teléfono
    for (int i = 0; i < modelo.columnCount(); ++i) {
        if (i == idCol || i == nomCol || i == apeCol || i == nifCol || i == tlfCol) {
            ui->tableView->showColumn(i);
        } else {
            ui->tableView->hideColumn(i);
        }
    }

    if (idCol != -1) ui->tableView->setColumnWidth(idCol, 60);
    if (nomCol != -1) ui->tableView->setColumnWidth(nomCol, 150);
    if (apeCol != -1) ui->tableView->setColumnWidth(apeCol, 200);
    if (nifCol != -1) ui->tableView->setColumnWidth(nifCol, 100);
    if (tlfCol != -1) ui->tableView->setColumnWidth(tlfCol, 100);

    ui->frame_detalles->setStyleSheet("QFrame#frame_detalles { border-left: 1px solid #ddd; background-color: #fff; }");
    
    // Connect selection change to update details (keyboard & mouse)
    connect(ui->tableView->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &BuscarCliente::selectionChanged);
    
    ui->lineEdit_buscar->setFocus();
}

BuscarCliente::~BuscarCliente()
{
    delete ui;
}

void BuscarCliente::selectionChanged(const QModelIndex &current, const QModelIndex &previous)
{
    Q_UNUSED(previous);
    mostrarDetalles(current);
}

void BuscarCliente::on_tableView_activated(const QModelIndex &index)
{
    QModelIndex sourceIndex = proxyModel.mapToSource(index);
    int idCol = modelo.record().indexOf("idCliente");
    if (idCol == -1) idCol = 0;
    QModelIndex idIndice = modelo.index(sourceIndex.row(), idCol);
    resultado = modelo.data(idIndice, Qt::EditRole).toString();
    this->accept();
}

void BuscarCliente::on_lineEdit_buscar_textChanged(const QString &arg1)
{
    proxyModel.setFilterFixedString(arg1);
    filtrar();
}

void BuscarCliente::filtrar()
{
    proxyModel.actualizar();
}

void BuscarCliente::mostrarDetalles(const QModelIndex &index)
{
    if (!index.isValid()) return;

    QModelIndex sourceIndex = proxyModel.mapToSource(index);
    QSqlRecord record = modelo.record(sourceIndex.row());
    
    QString id = record.value("idCliente").toString();
    QString nombre = record.value("nombre").toString();
    QString apellidos = record.value("apellidos").toString();
    QString direccion = record.value("direccion").toString();
    QString cp = record.value("cp").toString();
    QString localidad = record.value("localidad").toString();
    QString provincia = record.value("provincia").toString();
    QString nif = record.value("nif").toString();
    QString tlf1 = record.value("telefono").toString();
    QString tlf2 = record.value("telefono2").toString();
    QString mail = record.value("mail").toString();
    double descuento = record.value("descuento").toDouble();
    QString notas = record.value("notas").toString();

    resultado = id;

    // Mostrar Info
    QString info = QString("<b>ID Cliente:</b> %1<br><br>"
                           "<b>Nombre:</b> %2<br>"
                           "<b>Apellidos:</b> %3<br>"
                           "<b>NIF:</b> %4<br><br>"
                           "<b>Dirección:</b> %5<br>"
                           "<b>Localidad:</b> %6 (%7)<br>"
                           "<b>Provincia:</b> %8<br><br>"
                           "<b>Teléfono:</b> %9<br>"
                           "<b>Teléfono 2:</b> %10<br>"
                           "<b>Email:</b> %11<br><br>"
                           "<b>Descuento:</b> %12%<br><br>"
                           "<b>Notas:</b><br>%13")
                   .arg(id)
                   .arg(nombre)
                   .arg(apellidos)
                   .arg(nif)
                   .arg(direccion)
                   .arg(localidad)
                   .arg(cp)
                   .arg(provincia)
                   .arg(tlf1)
                   .arg(tlf2.isEmpty() ? "---" : tlf2)
                   .arg(mail.isEmpty() ? "---" : mail)
                   .arg(QString::number(descuento, 'f', 2))
                   .arg(notas.isEmpty() ? "---" : notas);
    
    ui->label_info->setText(info);
}

void BuscarCliente::reject()
{
    resultado = "";
    QDialog::reject();
}

void BuscarCliente::on_pushButtonCancelar_clicked()
{
    this->reject();
}

void BuscarCliente::on_pushButtonAceptar_clicked()
{
    on_tableView_activated(ui->tableView->currentIndex());
}
