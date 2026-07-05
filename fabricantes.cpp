#include "fabricantes.h"
#include <QMessageBox>
#include "saneadorglobal.h"
#include <QSqlError>
#include <QDebug>
#include <QStyle>
#include "ui_fabricantes.h"

/**
 * @brief Constructor de la ventana de Fabricantes.
 * Configura el modelo de datos, la interfaz y los iconos estándar.
 */
Fabricantes::Fabricantes(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Fabricantes)
{
    ui->setupUi(this);

    // Configurar el modelo de datos vinculado a la tabla 'fabricantes'
    modelo = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionLocal()));
    modelo->setTable("fabricantes");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // Nombres de cabecera más profesionales
    modelo->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelo->setHeaderData(1, Qt::Horizontal, tr("Nombre del Fabricante"));
    
    modelo->select();

    // Configurar la vista de tabla
    ui->tableView->setModel(modelo);
    ui->tableView->setColumnWidth(0, 60); // ID pequeño
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);

    // Asignar iconos estándar de Qt para un look profesional
    ui->pushButtonSeleccionar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonAnadir->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->pushButtonDeshacer->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->pushButtonModificar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lineEdit->setFocus();
}

Fabricantes::~Fabricantes()
{
    delete ui;
}

/**
 * @brief Cierra el diálogo aceptando la selección del fabricante.
 */
void Fabricantes::on_pushButtonSeleccionar_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione un fabricante primero."));
        return;
    }
    
    QModelIndex indice = modelo->index(selection.at(0).row(), 0);
    resultado = modelo->data(indice).toString();
    accept();
}

/**
 * @brief Selección rápida mediante doble clic.
 */
void Fabricantes::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(), 0);
    resultado = modelo->data(indice).toString();
    accept();
}

/**
 * @brief Añade una nueva fila al final de la tabla de fabricantes.
 */
void Fabricantes::on_pushButtonAnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
    ui->tableView->scrollToBottom();
}

/**
 * @brief Guarda los cambios realizados en los fabricantes con feedback claro.
 */
void Fabricantes::on_pushButtonModificar_clicked()
{
    if (modelo->submitAll()) {
        QMessageBox::information(this, tr("Éxito"), tr("Los fabricantes se han guardado correctamente."));
    } else {
        QMessageBox::critical(this, tr("Error"), 
                             tr("No se han podido guardar los cambios:\n") + modelo->lastError().text());
    }
}

/**
 * @brief Revierte los cambios no guardados tras confirmación.
 */
void Fabricantes::on_pushButtonDeshacer_clicked()
{
    if (modelo->isDirty()) {
        if (QMessageBox::question(this, tr("Deshacer"), 
                                 tr("¿Está seguro de que desea descartar los cambios no guardados?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
            modelo->revertAll();
        }
    }
}

/**
 * @brief Elimina el fabricante seleccionado con advertencia de seguridad.
 */
void Fabricantes::on_pushButtonBorrar_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Debe seleccionar un fabricante de la lista para borrarlo."));
        return;
    }

    if (QMessageBox::warning(this, tr("Confirmar borrado"), 
                              tr("¿Seguro que desea eliminar el fabricante seleccionado?\n"
                                 "Recuerde guardar cambios después para que sea permanente."),
                              QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        modelo->removeRow(selection.at(0).row());
    }
}

/**
 * @brief Registra la selección en la captura por clic (opcional, para compatibilidad).
 */
void Fabricantes::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(), 0);
    resultado = modelo->data(indice).toString();
}

/**
 * @brief Búsqueda en tiempo real de fabricantes por nombre.
 */
void Fabricantes::on_lineEdit_textChanged(const QString &arg1)
{
    modelo->setFilter(QString("nombre LIKE '%%1%'").arg(arg1));
}

void Fabricantes::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraFabricantes(), this);
    unif.exec();
    modelo->select();
}

void Fabricantes::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraFabricantes(), this);
    saneador.exec();
    modelo->select();
}
