#ifndef VERIFACTUDIALOG_H
#define VERIFACTUDIALOG_H

#include <QDialog>
#include <QStandardItemModel>
#include "base_datos.h"
#include "configuracion.h"

// Referencia a la configuración global de la aplicación
extern Configuracion *conf;

namespace Ui {
class VerifactuDialog;
}

/**
 * @brief Clase VerifactuDialog que unifica la gestión de logs de remisión a la AEAT
 *        y la configuración de los parámetros del módulo de VeriFactu.
 */
class VerifactuDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VerifactuDialog(QWidget *parent = nullptr);
    ~VerifactuDialog();

private slots:
    // Slot para actualizar el listado de logs en base al rango de fechas
    void on_pushButtonVer_clicked();

    // Slot para ver los detalles de remisión (XML y hashes) al hacer doble clic en una fila
    void on_tableView_doubleClicked(const QModelIndex &index);

    // Slot para abrir el cuadro de diálogo de selección del certificado digital (.p12/.pfx)
    void on_pushButtonCertificado_clicked();

    // Slot que se ejecuta al presionar "Aceptar", guardando la configuración en tienda.ini
    void accept() override;

private:
    // Método auxiliar para consultar y listar los logs de la base de datos local
    void llenarTabla();

    Ui::VerifactuDialog *ui;
    baseDatos base;                  // Instancia directa para comunicación con la base de datos
    QStandardItemModel *modeloTabla; // Modelo para mostrar los logs en la tabla
    QString idFactura;               // ID de la factura seleccionada actual
};

#endif // VERIFACTUDIALOG_H
