#include "formatos.h"
#include "ui_formatos.h"
#include "saneadorglobal.h"

// Escapa un literal de cadena MySQL para usarlo dentro de un setFilter
// de QSqlTableModel (que no admite parámetros enlazados).
static QString escSQL(const QString &s) {
  QString r = s;
  r.replace("\\", "\\\\");
  r.replace("'", "''");
  return r;
}

Formatos::Formatos(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Formatos)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modelo->setTable("formatos");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);

    ui->pushButtonnadir->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonModificar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButtonCerrar->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
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
    modelo->setFilter("formato LIKE '%" + escSQL(arg1) + "%'");
}

void Formatos::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraFormatos(), this);
    unif.exec();
    modelo->select();
}

void Formatos::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraFormatos(), this);
    saneador.exec();
    modelo->select();
}

void Formatos::keyPressEvent(QKeyEvent *e)
{
    if (e->modifiers() == Qt::ControlModifier && e->key() == Qt::Key_F11) {
        ui->pushButtonModificar->setEnabled(true);
        ui->pushButtonnadir->setEnabled(true);
    }
}
