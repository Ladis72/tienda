/**
 * @file dialogcambiarproveedor.h
 * @brief Declaración del diálogo para modificar el proveedor asignado a una factura o albarán.
 */

#ifndef DIALOGCAMBIARPROVEEDOR_H
#define DIALOGCAMBIARPROVEEDOR_H

#include <QDialog>
#include <QString>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class DialogCambiarProveedor;
}

/**
 * @class DialogCambiarProveedor
 * @brief Permite a los administradores seleccionar y reasignar un nuevo proveedor a un documento.
 */
class DialogCambiarProveedor : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del diálogo.
     * @param nDoc Número de factura o documento a modificar.
     * @param nombreProveedorActual Nombre del proveedor que tiene actualmente asignado.
     * @param parent Widget padre.
     */
    explicit DialogCambiarProveedor(const QString &nDoc, const QString &nombreProveedorActual, QWidget *parent = nullptr);
    ~DialogCambiarProveedor();

    /**
     * @brief Retorna el ID del nuevo proveedor seleccionado.
     */
    QString getIdProveedorNuevo() const;

    /**
     * @brief Retorna el nombre del nuevo proveedor seleccionado.
     */
    QString getNombreProveedorNuevo() const;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::DialogCambiarProveedor *ui;
    baseDatos base;
    QString m_nDoc;
    QString m_proveedorActual;
    QString m_idProveedorNuevo;
    QString m_nombreProveedorNuevo;

    void cargarProveedores();
};

#endif // DIALOGCAMBIARPROVEEDOR_H
