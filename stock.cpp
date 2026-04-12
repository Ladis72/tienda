#include "stock.h"
#include "ui_stock.h"
#include <QMessageBox>
#include <QVBoxLayout>
#include <QSqlError>
#include <QHeaderView>

Stock::Stock(QString cod, baseDatos *db, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Stock)
    , base(db)
{
    ui->setupUi(this);
    
    // Obtener datos del producto
    QSqlRecord registroProd = base->consulta_producto(conf->getConexionLocal(), cod);
    if (!registroProd.isEmpty()) {
        codProducto = registroProd.value("cod").toString();
        ui->labelProductoHeader->setText(QString("Producto: %1 (EAN: %2)")
                           .arg(registroProd.value("descripcion").toString())
                           .arg(codProducto));
    }

    // Configurar tabla de lotes (solo lectura)
    modeloLotes = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modeloLotes->setTable("lotes");
    modeloLotes->setFilter("ean = '" + codProducto + "'");
    refrescarLotes();
    
    ui->tableView->setModel(modeloLotes);
    ui->tableView->hideColumn(0); // id
    ui->tableView->hideColumn(1); // ean
    
    // Nombres de columnas más legibles
    modeloLotes->setHeaderData(2, Qt::Horizontal, "Nº Lote");
    modeloLotes->setHeaderData(3, Qt::Horizontal, "Caducidad");
    modeloLotes->setHeaderData(4, Qt::Horizontal, "Cantidad");
    
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->setAlternatingRowColors(true);

    // Inicializar formulario
    ui->dateEditNewCad->setDate(QDate::currentDate());
    limpiarFormulario();
}

Stock::~Stock()
{
    delete ui;
}

void Stock::refrescarLotes() {
    modeloLotes->select();
}

void Stock::limpiarFormulario() {
    ui->lineEditLoteSel->clear();
    ui->lineEditCantAct->clear();
    ui->doubleSpinBoxNewCant->setValue(0);
    ui->dateEditNewCad->setDate(QDate::currentDate());
    ui->comboBoxMotivo->setCurrentIndex(0);
    currentLoteId = "";
    filaSeleccionada = -1;
    ui->groupBoxAjuste->setEnabled(false);
}

void Stock::on_tableView_clicked(const QModelIndex &index)
{
    filaSeleccionada = index.row();
    QSqlRecord record = modeloLotes->record(filaSeleccionada);
    
    currentLoteId = record.value("id").toString();
    ui->lineEditLoteSel->setText(record.value("lote").toString());
    ui->dateEditNewCad->setDate(record.value("fecha").toDate());
    ui->lineEditCantAct->setText(record.value("cantidad").toString());
    ui->doubleSpinBoxNewCant->setValue(record.value("cantidad").toDouble());
    
    ui->groupBoxAjuste->setEnabled(true);
    ui->pushButtonValidar->setText("REGISTRAR AJUSTE");
}

void Stock::on_pushButtonValidar_clicked()
{
    if (codProducto.isEmpty()) return;

    QString lote = ui->lineEditLoteSel->text();
    QDate newCad = ui->dateEditNewCad->date();
    double newCant = ui->doubleSpinBoxNewCant->value();
    QString motivo = ui->comboBoxMotivo->currentText();
    QString usuario = conf->getUsuario();

    double oldCant = 0;
    QDate oldCad;
    
    if (filaSeleccionada != -1) {
        QSqlRecord oldRec = modeloLotes->record(filaSeleccionada);
        oldCant = oldRec.value("cantidad").toDouble();
        oldCad = oldRec.value("fecha").toDate();
    }

    QSqlDatabase db = QSqlDatabase::database(conf->getConexionLocal());
    db.transaction();

    QSqlQuery q(db);
    bool ok = false;

    if (filaSeleccionada != -1) {
        // Actualizar lote existente
        q.prepare("UPDATE lotes SET cantidad = ?, fecha = ?, lote = ? WHERE id = ?");
        q.addBindValue(newCant);
        q.addBindValue(newCad.toString("yyyy-MM-dd"));
        q.addBindValue(lote);
        q.addBindValue(currentLoteId);
        ok = q.exec();
    } else {
        // Crear nuevo lote
        q.prepare("INSERT INTO lotes (ean, lote, fecha, cantidad) VALUES (?, ?, ?, ?)");
        q.addBindValue(codProducto);
        q.addBindValue(lote);
        q.addBindValue(newCad.toString("yyyy-MM-dd"));
        q.addBindValue(newCant);
        ok = q.exec();
    }

    if (ok) {
        // Registrar en histórico
        q.prepare("INSERT INTO historico_stock (ean, lote, fecha_caducidad_ant, fecha_caducidad_new, "
                  "stock_ant, stock_new, motivo, usuario) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        q.addBindValue(codProducto);
        q.addBindValue(lote);
        if (filaSeleccionada != -1) {
            q.addBindValue(oldCad.toString("yyyy-MM-dd"));
            q.addBindValue(newCad.toString("yyyy-MM-dd"));
            q.addBindValue(oldCant);
        } else {
            q.addBindValue(QVariant(QMetaType::fromType<QDate>())); // NULL en Qt 6
            q.addBindValue(newCad.toString("yyyy-MM-dd"));
            q.addBindValue(0.0);
        }
        q.addBindValue(newCant);
        q.addBindValue(motivo);
        q.addBindValue(usuario);
        ok = q.exec();
    }

    if (ok) {
        db.commit();
        QMessageBox::information(this, "Éxito", "Ajuste registrado correctamente.");
        refrescarLotes();
        limpiarFormulario();
    } else {
        db.rollback();
        QMessageBox::critical(this, "Error", "No se pudo registrar el ajuste:\n" + q.lastError().text());
    }
}

void Stock::on_pushButtonNuevoLote_clicked()
{
    limpiarFormulario();
    ui->groupBoxAjuste->setEnabled(true);
    ui->lineEditLoteSel->setReadOnly(false);
    ui->lineEditLoteSel->setFocus();
    ui->pushButtonValidar->setText("CREAR LOTE Y AJUSTAR");
}

void Stock::on_pushButtonHistory_clicked()
{
    QDialog *historyDlg = new QDialog(this);
    historyDlg->setWindowTitle("Histórico de Ajustes - " + codProducto);
    historyDlg->resize(800, 400);
    
    QVBoxLayout *layout = new QVBoxLayout(historyDlg);
    QTableView *view = new QTableView(historyDlg);
    
    QSqlQueryModel *model = new QSqlQueryModel(historyDlg);
    QString query = QString("SELECT fecha_hora as 'Fecha/Hora', usuario as 'Usuario', lote as 'Lote', "
                            "stock_ant as 'Cant. Ant.', stock_new as 'Cant. New', motivo as 'Motivo', "
                            "fecha_caducidad_ant as 'Cad. Ant.', fecha_caducidad_new as 'Cad. New' "
                            "FROM historico_stock WHERE ean = '%1' ORDER BY fecha_hora DESC")
                    .arg(codProducto);
    model->setQuery(query, QSqlDatabase::database(conf->getConexionLocal()));
    
    view->setModel(model);
    view->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    view->setAlternatingRowColors(true);
    
    layout->addWidget(view);
    
    QPushButton *btnClose = new QPushButton("Cerrar", historyDlg);
    connect(btnClose, &QPushButton::clicked, historyDlg, &QDialog::accept);
    layout->addWidget(btnClose);
    
    historyDlg->exec();
}

void Stock::on_pushButtonCerrar_clicked()
{
    accept();
}
