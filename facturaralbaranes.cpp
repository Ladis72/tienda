#include "facturaralbaranes.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QComboBox>
#include <QTableView>
#include <QLineEdit>
#include <QDateEdit>
#include <QPushButton>
#include <QLabel>
#include <QHeaderView>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

FacturarAlbaranes::FacturarAlbaranes(QWidget *parent) : QDialog(parent)
{
    setupUi();
    loadProveedores();
    connect(comboProveedores, SIGNAL(currentIndexChanged(int)), this, SLOT(onProveedorChanged(int)));
    connect(modeloAlbaranes, SIGNAL(itemChanged(QStandardItem*)), this, SLOT(calcTotals()));
}

FacturarAlbaranes::~FacturarAlbaranes() {}

void FacturarAlbaranes::setupUi()
{
    this->setWindowTitle("Facturar Albaranes Agrupados");
    this->resize(800, 600);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // Filter section
    QHBoxLayout *filterLayout = new QHBoxLayout();
    filterLayout->addWidget(new QLabel("Proveedor:"));
    comboProveedores = new QComboBox();
    filterLayout->addWidget(comboProveedores);
    mainLayout->addLayout(filterLayout);

    // Table section
    tablaAlbaranes = new QTableView();
    modeloAlbaranes = new QStandardItemModel(0, 8, this);
    modeloAlbaranes->setHorizontalHeaderLabels({"Sel", "ID", "Albarán", "Fecha", "Base", "IVA", "RE", "Total"});
    tablaAlbaranes->setModel(modeloAlbaranes);
    tablaAlbaranes->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tablaAlbaranes->setSelectionBehavior(QAbstractItemView::SelectRows);
    tablaAlbaranes->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    mainLayout->addWidget(tablaAlbaranes);

    // Invoice details section
    QFormLayout *formLayout = new QFormLayout();
    leFactura = new QLineEdit();
    dateFactura = new QDateEdit(QDate::currentDate());
    dateFactura->setDisplayFormat("yyyy-MM-22");
    dateFactura->setCalendarPopup(true);
    dateVencimiento = new QDateEdit(QDate::currentDate());
    dateVencimiento->setCalendarPopup(true);
    dateVencimiento->setDisplayFormat("yyyy-MM-dd");

    formLayout->addRow("Número Factura:", leFactura);
    formLayout->addRow("Fecha:", dateFactura);
    formLayout->addRow("Vencimiento:", dateVencimiento);
    mainLayout->addLayout(formLayout);

    // Totals section
    QHBoxLayout *totalsLayout = new QHBoxLayout();
    lblTotalBase = new QLabel("Base: 0.00");
    lblTotalIva = new QLabel("IVA: 0.00");
    lblTotalRe = new QLabel("RE: 0.00");
    lblTotal = new QLabel("TOTAL: 0.00");
    totalsLayout->addWidget(lblTotalBase);
    totalsLayout->addWidget(lblTotalIva);
    totalsLayout->addWidget(lblTotalRe);
    totalsLayout->addWidget(lblTotal);
    mainLayout->addLayout(totalsLayout);

    // Buttons
    QHBoxLayout *buttonsLayout = new QHBoxLayout();
    btnFacturar = new QPushButton("Facturar");
    btnCancelar = new QPushButton("Cancelar");
    buttonsLayout->addStretch();
    buttonsLayout->addWidget(btnFacturar);
    buttonsLayout->addWidget(btnCancelar);
    mainLayout->addLayout(buttonsLayout);

    connect(btnFacturar, SIGNAL(clicked()), this, SLOT(facturar()));
    connect(btnCancelar, SIGNAL(clicked()), this, SLOT(reject()));
}

void FacturarAlbaranes::loadProveedores()
{
    QStringList proveedores = base.listadoProveedores(conf->getConexionLocal());
    for (const QString& prov : proveedores) {
        QString id = base.idProveedor(prov, conf->getConexionLocal());
        comboProveedores->addItem(prov, id);
    }
}

void FacturarAlbaranes::onProveedorChanged(int index)
{
    if (index < 0) return;
    QString proveedor = comboProveedores->itemText(index);
    qDebug() << proveedor;
    loadAlbaranes(proveedor);
}

void FacturarAlbaranes::loadAlbaranes(QString proveedor)
{
    int idProveedor = base.idProveedor(proveedor, conf->getConexionLocal()).toInt();
    qDebug() << idProveedor;
    modeloAlbaranes->removeRows(0, modeloAlbaranes->rowCount());
    if (idProveedor > 0) {
        QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
        QSqlQuery q(db);
        q.prepare("SELECT * FROM albaranes WHERE idProveedor = ? AND (idFactura IS NULL OR idFactura = '0' OR idFactura = '')");
        q.bindValue(0, idProveedor);
        if (q.exec()) {
            qDebug() << "Consulta ejecutada:" << q.lastQuery();
            int count = 0;
            while (q.next()) {
                count++;
                qDebug() << "Cargando albarán ID:" << q.value(0).toString();
                QList<QStandardItem*> row;
                
                QStandardItem *chk = new QStandardItem("");
                chk->setCheckable(true);
                chk->setCheckState(Qt::Unchecked);
                
                row << chk
                    << new QStandardItem(q.value(0).toString()) // ID 
                    << new QStandardItem(q.value(1).toString()) // nAlbaran
                    << new QStandardItem(q.value(2).toString()) // fecha
                    << new QStandardItem(q.value(4).toString()) // base
                    << new QStandardItem(q.value(5).toString()) // iva
                    << new QStandardItem(q.value(6).toString()) // re
                    << new QStandardItem(q.value(7).toString()); // total 
                    
                modeloAlbaranes->appendRow(row);
            }
            qDebug() << "Total registros añadidos al modelo:" << count;
        } else {
            qDebug() << "Error en loadAlbaranes:" << q.lastError().text();
        }
    }
    calcTotals();
}

void FacturarAlbaranes::calcTotals()
{
    double sBase = 0, sIva = 0, sRe = 0, sTotal = 0;
    for (int i = 0; i < modeloAlbaranes->rowCount(); i++) {
        if (modeloAlbaranes->item(i, 0)->checkState() == Qt::Checked) {
            sBase += modeloAlbaranes->item(i, 4)->text().toDouble();
            sIva += modeloAlbaranes->item(i, 5)->text().toDouble();
            sRe += modeloAlbaranes->item(i, 6)->text().toDouble();
            sTotal += modeloAlbaranes->item(i, 7)->text().toDouble();
        }
    }
    lblTotalBase->setText(QString("Base: %1").arg(sBase, 0, 'f', 2));
    lblTotalIva->setText(QString("IVA: %1").arg(sIva, 0, 'f', 2));
    lblTotalRe->setText(QString("RE: %1").arg(sRe, 0, 'f', 2));
    lblTotal->setText(QString("TOTAL: %1").arg(sTotal, 0, 'f', 2));
}

void FacturarAlbaranes::facturar()
{
    if (leFactura->text().isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe introducir un número de factura.");
        return;
    }

    QList<QString> albaranesSeleccionados;
    double sBase = 0, sIva = 0, sRe = 0, sTotal = 0;
    
    for (int i = 0; i < modeloAlbaranes->rowCount(); i++) {
        if (modeloAlbaranes->item(i, 0)->checkState() == Qt::Checked) {
            albaranesSeleccionados.append(modeloAlbaranes->item(i, 1)->text()); // Usar ID de la columna 1
            sBase += modeloAlbaranes->item(i, 4)->text().toDouble();
            sIva += modeloAlbaranes->item(i, 5)->text().toDouble();
            sRe += modeloAlbaranes->item(i, 6)->text().toDouble();
            sTotal += modeloAlbaranes->item(i, 7)->text().toDouble();
        }
    }

    if (albaranesSeleccionados.isEmpty()) {
        QMessageBox::warning(this, "Error", "Debe seleccionar al menos un albarán.");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    db.transaction();

    QStringList datosFactura;
    datosFactura << leFactura->text()
                 << dateFactura->date().toString("yyyy-MM-dd")
                 << QString::number(comboProveedores->currentData().toInt())
                 << QString::number(sBase, 'f', 2)
                 << QString::number(sIva, 'f', 2)
                 << QString::number(sRe, 'f', 2)
                 << QString::number(sTotal, 'f', 2)
                 << dateVencimiento->date().toString("yyyy-MM-dd")
                 << "0"; 

    if (!base.grabarFactura(conf->getConexionLocal(), datosFactura)) {
        db.rollback();
        QMessageBox::critical(this, "Error", "No se pudo crear la factura.");
        return;
    }

    for (const QString& idAlb : albaranesSeleccionados) {
        QSqlQuery q(db);
        q.prepare("UPDATE albaranes SET idFactura = ? WHERE id = ?");
        q.bindValue(0, leFactura->text());
        q.bindValue(1, idAlb);
        if (!q.exec()) {
            db.rollback();
            QMessageBox::critical(this, "Error", "No se pudieron actualizar los albaranes.");
            return;
        }
    }

    db.commit();
    QMessageBox::information(this, "Éxito", "Factura creada y albaranes agrupados correctamente.");
    accept();
}
