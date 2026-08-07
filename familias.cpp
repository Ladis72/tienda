#include "familias.h"
#include <QMessageBox>
#include "saneadorglobal.h"
#include <QSqlError>
#include <QDebug>
#include <QStyle>
#include "ui_familias.h"

// Escapa un literal de cadena MySQL para usarlo dentro de un setFilter
// de QSqlTableModel (que no admite parámetros enlazados).
static QString escSQL(const QString &s) {
  QString r = s;
  r.replace("\\", "\\\\");
  r.replace("'", "''");
  return r;
}

/**
 * @brief Constructor de la ventana de Familias.
 * Configura el modelo de datos, la interfaz y los iconos.
 */
Familias::Familias(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Familias)
{
    ui->setupUi(this);

    // Configurar el modelo de datos vinculado a la tabla 'familias'
    modelo = new QSqlTableModel(this, QSqlDatabase::database(conf->getConexionCommon()));
    modelo->setTable("familias");
    modelo->setEditStrategy(QSqlTableModel::OnManualSubmit);
    
    // Nombres de cabecera más profesionales
    modelo->setHeaderData(0, Qt::Horizontal, tr("ID"));
    modelo->setHeaderData(1, Qt::Horizontal, tr("Descripción de la Familia"));
    
    modelo->select();

    // Configurar la vista de tabla
    ui->tableView->setModel(modelo);
    ui->tableView->setColumnWidth(0, 60); // ID pequeño
    ui->tableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tableView->verticalHeader()->setVisible(false);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setAlternatingRowColors(true);
    
    // Ocultar ID si se prefiere, pero lo dejaremos visible y pequeño para profesionalismo
    // ui->tableView->hideColumn(0);

    // Asignar iconos estándar de Qt para un look profesional sin recursos externos
    ui->pushButtonAnadir->setIcon(style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    ui->pushButtonBorrar->setIcon(style()->standardIcon(QStyle::SP_TrashIcon));
    ui->pushButtonDeshacer->setIcon(style()->standardIcon(QStyle::SP_ArrowBack));
    ui->pushButtonGuardar->setIcon(style()->standardIcon(QStyle::SP_DialogSaveButton));
    ui->pushButtonSeleccionar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonUnificar->setIcon(style()->standardIcon(QStyle::SP_DialogApplyButton));
    ui->pushButtonSaneador->setIcon(style()->standardIcon(QStyle::SP_BrowserReload));

    ui->lineEdit->setFocus();
    aplicarPermisos();
}

Familias::~Familias()
{
    delete ui;
}

/**
 * @brief Aplica las restricciones de permisos a los botones del formulario de familias.
 */
void Familias::aplicarPermisos() {
    if (!conf || !conf->permisos())
        return;

    ui->pushButtonAnadir->setEnabled(conf->permisos()->tiene("familias.crear"));
    ui->pushButtonGuardar->setEnabled(conf->permisos()->tiene("familias.modificar"));
    ui->pushButtonBorrar->setEnabled(conf->permisos()->tiene("familias.borrar"));
    ui->pushButtonUnificar->setEnabled(conf->permisos()->tiene("familias.unificar"));
    ui->pushButtonSaneador->setEnabled(conf->permisos()->tiene("saneador_global"));
}

/**
 * @brief Cierra el diálogo aceptando la selección actual.
 */
void Familias::on_pushButtonSeleccionar_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione una familia primero."));
        return;
    }
    
    QModelIndex indice = modelo->index(selection.at(0).row(), 0);
    resultado = modelo->data(indice).toString();
    accept();
}

/**
 * @brief Selecciona automáticamente al hacer doble clic.
 */
void Familias::on_tableView_doubleClicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(), 0);
    resultado = modelo->data(indice).toString();
    accept();
}

/**
 * @brief Añade una nueva fila al final de la tabla.
 */
void Familias::on_pushButtonAnadir_clicked()
{
    modelo->insertRow(modelo->rowCount());
    ui->tableView->scrollToBottom();
}

/**
 * @brief Guarda todos los cambios pendientes en la base de datos.
 */
void Familias::on_pushButtonGuardar_clicked()
{
    if (modelo->submitAll()) {
        QMessageBox::information(this, tr("Éxito"), tr("Los cambios se han guardado correctamente."));
    } else {
        QMessageBox::critical(this, tr("Error"), 
                             tr("No se han podido guardar los cambios:\n") + modelo->lastError().text());
    }
}

/**
 * @brief Revierte los cambios no guardados.
 */
void Familias::on_pushButtonDeshacer_clicked()
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
 * @brief Borra la familia seleccionada tras confirmación.
 */
void Familias::on_pushButtonBorrar_clicked()
{
    QModelIndexList selection = ui->tableView->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::warning(this, tr("Atención"), tr("Por favor, seleccione una fila para borrar."));
        return;
    }

    if (QMessageBox::question(this, tr("Confirmar borrado"), 
                             tr("¿Está seguro de que desea borrar la familia seleccionada?\n"
                                "Esta acción no se puede deshacer una vez guardada."),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
        modelo->removeRow(selection.at(0).row());
    }
}

/**
 * @brief Registra la selección actual.
 */
void Familias::on_tableView_clicked(const QModelIndex &index)
{
    QModelIndex indice = modelo->index(index.row(), 0);
    resultado = modelo->data(indice).toString();
}

/**
 * @brief Filtra la tabla en tiempo real.
 */
void Familias::on_lineEdit_textChanged(const QString &arg1)
{
    // Filtro insensible a mayúsculas/minúsculas y más seguro
    modelo->setFilter(QString("descripcion LIKE '%%1%'").arg(escSQL(arg1)));
}

/**
 * @brief Abre el diálogo de unificación interactiva de familias (Local).
 */
void Familias::on_pushButtonUnificar_clicked()
{
    UnificarMaestros unif(UnificarMaestrosConfig::paraFamilias(), this);
    unif.exec();
    modelo->select();
}

/**
 * @brief Abre el diálogo de alineación multi-tienda de familias (Global).
 */
void Familias::on_pushButtonSaneador_clicked()
{
    SaneadorGlobal saneador(UnificarMaestrosConfig::paraFamilias(), this);
    saneador.exec();
    modelo->select();
}
