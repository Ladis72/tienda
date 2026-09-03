#ifndef DIALOGLOGSIA_H
#define DIALOGLOGSIA_H

#include <QDialog>
#include <QVariantMap>
#include <QList>

namespace Ui {
class DialogLogsIA;
}

/**
 * @brief Diálogo para visualizar el historial de peticiones a la IA, auditar respuestas,
 * evaluar su precisión (👍 / 👎) y exportar reportes de mejoras para el desarrollo de nuevas herramientas.
 */
class DialogLogsIA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogLogsIA(QWidget *parent = nullptr);
    ~DialogLogsIA();

private slots:
    void on_pushButtonFiltrar_clicked();
    void on_pushButtonCopiarPortapapeles_clicked();
    void on_pushButtonExportar_clicked();
    void on_pushButtonMarcarCorrecta_clicked();
    void on_pushButtonMarcarIncorrecta_clicked();
    void on_pushButtonGuardarFeedback_clicked();
    void on_pushButtonCerrar_clicked();
    void on_tableWidgetLogs_itemSelectionChanged();
    void on_comboBoxFiltroEstado_currentIndexChanged(int index);

private:
    Ui::DialogLogsIA *ui;
    QList<QVariantMap> m_logsActuales;
    qint64 m_idLogSeleccionado;

    void cargarLogs();
    void mostrarDetalleLog(int fila);
    void actualizarEvaluacion(int evaluacion, const QString &comentario);
};

#endif // DIALOGLOGSIA_H
