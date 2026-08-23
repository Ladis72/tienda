#ifndef DIALOGCONOCIMIENTOIA_H
#define DIALOGCONOCIMIENTOIA_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class DialogConocimientoIA;
}

/**
 * @brief Diálogo para gestionar y editar la Base de Conocimiento de Fitoterapia y Salud del Asistente IA.
 */
class DialogConocimientoIA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogConocimientoIA(QWidget *parent = nullptr);
    ~DialogConocimientoIA();

    /**
     * @brief Garantiza que la tabla ia_conocimiento exista en la base de datos y tenga datos iniciales.
     */
    static void asegurarTablaBaseDatos();

signals:
    void conocimientoModificado();

private slots:
    void on_pushButtonAnadir_clicked();
    void on_pushButtonEditar_clicked();
    void on_pushButtonEliminar_clicked();
    void on_pushButtonRestablecer_clicked();
    void on_pushButtonGuardar_clicked();
    void on_pushButtonCerrar_clicked();
    void on_tableWidgetReglas_cellDoubleClicked(int row, int column);

private:
    Ui::DialogConocimientoIA *ui;
    void cargarDatos();
    void configurarTabla();
    void guardarDatos();
};

#endif // DIALOGCONOCIMIENTOIA_H
