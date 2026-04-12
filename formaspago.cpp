#include "formaspago.h"
#include <QMessageBox>
#include "ui_formaspago.h"
#include "saneadorglobal.h"

FormasPago::FormasPago(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FormasPago)
{
    ui->setupUi(this);

    modelolista = new QSqlTableModel(this, QSqlDatabase::database("DB"));
    modelolista->setTable("fpago");
    modelolista->setEditStrategy(QSqlTableModel::OnManualSubmit);
    modelolista->select();
    ui->tableView->setModel(modelolista);
    ui->tableView->resizeColumnsToContents();
    ui->tableView->hideColumn(0);

    ui->pushButtonAnadir->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->pushButtonModificar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));
}

FormasPago::~FormasPago()
{
    delete ui;
}

void FormasPago::on_pushButtonModificar_clicked()
{
    if (!modelolista->submitAll()) {
        QMessageBox::warning(this,
                             "ATENCION",
                             "No se ha podido modificar el registro"
                                 + modelolista->lastError().text());
    }
}

void FormasPago::on_pushButtonAnadir_clicked()
{
    modelolista->insertRow(modelolista->rowCount());
}

void FormasPago::on_pushButtonBorrar_clicked()
{
    modelolista->removeRow(Qt::EditRole);
}

void FormasPago::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelolista->index(index.row(), 0);
    resultado = modelolista->data(indice).toString();
}

void FormasPago::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraFormasPago(), this);
    unif.exec();
    modelolista->select();
}

void FormasPago::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraFormasPago(), this);
    saneador.exec();
    modelolista->select();
}
