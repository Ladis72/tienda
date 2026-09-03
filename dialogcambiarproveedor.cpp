/**
 * @file dialogcambiarproveedor.cpp
 * @brief Implementación del diálogo para cambiar el proveedor de una factura o albarán.
 */

#include "dialogcambiarproveedor.h"
#include "ui_dialogcambiarproveedor.h"
#include <QMessageBox>
#include <QCompleter>

DialogCambiarProveedor::DialogCambiarProveedor(const QString &nDoc, const QString &nombreProveedorActual, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::DialogCambiarProveedor)
    , m_nDoc(nDoc)
    , m_proveedorActual(nombreProveedorActual)
{
    ui->setupUi(this);

    // Configurar información inicial en las etiquetas
    ui->lblNDoc->setText(m_nDoc);
    ui->lblProveedorActual->setText(m_proveedorActual.isEmpty() ? tr("(Sin asignar)") : m_proveedorActual);

    // Cargar la lista de proveedores disponibles
    cargarProveedores();
}

DialogCambiarProveedor::~DialogCambiarProveedor()
{
    delete ui;
}

void DialogCambiarProveedor::cargarProveedores()
{
    ui->comboBoxNuevoProveedor->clear();
    QStringList proveedores = base.listadoProveedores(conf->getConexionLocal());
    ui->comboBoxNuevoProveedor->addItems(proveedores);

    // Habilitar autocompletado para facilitar la búsqueda
    QCompleter *completer = new QCompleter(proveedores, this);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setFilterMode(Qt::MatchContains);
    ui->comboBoxNuevoProveedor->setCompleter(completer);

    // Posicionar en el proveedor actual si existe en la lista
    int index = ui->comboBoxNuevoProveedor->findText(m_proveedorActual);
    if (index != -1) {
        ui->comboBoxNuevoProveedor->setCurrentIndex(index);
    }
}

QString DialogCambiarProveedor::getIdProveedorNuevo() const
{
    return m_idProveedorNuevo;
}

QString DialogCambiarProveedor::getNombreProveedorNuevo() const
{
    return m_nombreProveedorNuevo;
}

void DialogCambiarProveedor::on_buttonBox_accepted()
{
    m_nombreProveedorNuevo = ui->comboBoxNuevoProveedor->currentText().trimmed();

    if (m_nombreProveedorNuevo.isEmpty()) {
        QMessageBox::warning(this, tr("Proveedor inválido"),
                             tr("Debe seleccionar un proveedor válido de la lista."));
        return;
    }

    if (m_nombreProveedorNuevo.compare(m_proveedorActual, Qt::CaseInsensitive) == 0) {
        QMessageBox::information(this, tr("Sin cambios"),
                                tr("El proveedor seleccionado es el mismo que el actual."));
        reject();
        return;
    }

    m_idProveedorNuevo = base.idProveedor(m_nombreProveedorNuevo, conf->getConexionLocal());
    if (m_idProveedorNuevo.isEmpty() || m_idProveedorNuevo == "0") {
        QMessageBox::warning(this, tr("Proveedor no encontrado"),
                             tr("No se ha encontrado el ID para el proveedor seleccionado."));
        return;
    }

    // Confirmación explícita antes de proceder
    QMessageBox::StandardButton respuesta = QMessageBox::question(
        this,
        tr("Confirmar cambio de proveedor"),
        tr("¿Está seguro de que desea cambiar el proveedor de la factura '%1'?\n\n"
           "Proveedor actual: %2\n"
           "Nuevo proveedor: %3\n\n"
           "Esta acción modificará la factura/albarán, el pedido y las líneas de compra históricas.")
            .arg(m_nDoc, m_proveedorActual, m_nombreProveedorNuevo),
        QMessageBox::Yes | QMessageBox::No,
        QMessageBox::No);

    if (respuesta == QMessageBox::Yes) {
        accept();
    }
}
