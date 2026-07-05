#ifndef VISORLOG_H
#define VISORLOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDateTime>
#include <QDate>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class VisorLog;
}

/**
 * @brief Diálogo visual para auditar y buscar registros en la tabla 'logs' del sistema.
 *
 * Características:
 *  - Filtros dinámicos por categorías (cargadas mediante SELECT DISTINCT).
 *  - Filtros dinámicos por usuarios que han registrado logs.
 *  - Búsqueda en texto libre de mensajes.
 *  - Purgado seguro de logs antiguos previa confirmación para administradores.
 *  - Exportación de logs filtrados a formatos universales (CSV) e impresión formateada (PDF).
 */
class VisorLog : public QDialog
{
    Q_OBJECT

public:
    explicit VisorLog(QWidget *parent = nullptr);
    ~VisorLog();

private slots:
    /// Aplica los filtros seleccionados en la UI y recarga el table view
    void on_pushButtonFiltrar_clicked();

    /// Exporta los datos actualmente visibles a un archivo CSV en el directorio de documentos
    void on_pushButtonCSV_clicked();

    /// Genera un documento PDF formateado con el informe de logs y lo abre
    void on_pushButtonImprimir_clicked();

    /// Elimina de forma permanente logs antiguos de más de 30 días, previa confirmación
    void on_pushButtonLimpiar_clicked();

private:
    Ui::VisorLog *ui;
    baseDatos base;
    QSqlQueryModel *modeloTabla;

    /// Carga dinámicamente las categorías y usuarios disponibles en los ComboBoxes
    void cargarCombos();

    /// Configura el diseño inicial y los anchos de columnas de la tabla
    void configurarTabla();
};

#endif // VISORLOG_H
