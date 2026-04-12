#include "tiposentradassalidas.h"
#include "ui_tiposentradassalidas.h"
#include "saneadorglobal.h"
#include <QStyle>

TiposEntradasSalidas::TiposEntradasSalidas(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::TiposEntradasSalidas)
{
    ui->setupUi(this);
    modelo = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    modelo->setTable("motivosEntrada");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelo->select();
    ui->tableView->setModel(modelo);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);

    ui->pushButton->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButton_2->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButton_3->setIcon(style()->standardIcon(QStyle::SP_DialogCloseButton));
    ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
}

TiposEntradasSalidas::~TiposEntradasSalidas()
{
    delete ui;
}

void TiposEntradasSalidas::on_pushButton_clicked()
{
    modelo->insertRow(modelo->rowCount());
}

void TiposEntradasSalidas::on_pushButton_2_clicked()
{
    if (!modelo->submitAll()) {
        QMessageBox::warning(this,
                             "ATENCION",
                             "No se ha podido modificar el registro" + modelo->lastError().text());
    }
}

void TiposEntradasSalidas::on_lineEdit_textChanged(const QString &arg1)
{
    modelo->setFilter("descripcion LIKE '%" + arg1 + "%'");
}

void TiposEntradasSalidas::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraMotivosEntrada(), this);
    unif.exec();
    modelo->select();
}

void TiposEntradasSalidas::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraMotivosEntrada(), this);
    saneador.exec();
    modelo->select();
}
