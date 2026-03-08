#include "formatos.h"
#include "ui_formatos.h"

Formatos::Formatos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Formatos)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    modelo->setTable("formatos");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);
}

Formatos::~Formatos()
{
    delete ui;
}

void Formatos::on_pushButtonModificar_clicked()
{
    if (!modelo->submitAll()) {
        QMessageBox::warning(this,
                             "ATENCION",
                             "No se ha podido modificar el registro" + modelo->lastError().text());
    }
}

void Formatos::on_pushButtonnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void Formatos::on_lineEditFiltro_textChanged(const QString &arg1)
{
    modelo->setFilter("formato LIKE '%" + arg1 + "%'");
}

void Formatos::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_F11) {
        ui->pushButtonModificar->setEnabled(true);
        ui->pushButtonnadir->setEnabled(true);
    }
}
