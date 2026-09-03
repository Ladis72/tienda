#ifndef DIALOGASISTENTEIA_H
#define DIALOGASISTENTEIA_H

#include <QDialog>
#include <QStringList>
#include "asistenteia.h"

namespace Ui {
class DialogAsistenteIA;
}

/**
 * @brief Diálogo visual del Asistente de IA local para TPV Tienda.
 *
 * Muestra el historial de conversación en formato enriquecido, gestiona
 * el envío de preguntas, la selección de modelos de Ollama y ofrece
 * accesos rápidos a consultas habituales de ventas, stock y arqueos.
 */
class DialogAsistenteIA : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAsistenteIA(QWidget *parent = nullptr);
    ~DialogAsistenteIA();

private slots:
    void on_pushButtonEnviar_clicked();
    void on_lineEditPregunta_returnPressed();
    void on_pushButtonLimpiar_clicked();
    void on_pushButtonConocimiento_clicked();
    void on_pushButtonVerLogs_clicked();
    void on_pushButtonSugerenciaVentas_clicked();
    void on_pushButtonSugerenciaStock_clicked();
    void on_pushButtonSugerenciaArqueos_clicked();
    void on_pushButtonCerrar_clicked();

    // --- Slots conectados al motor AsistenteIA y UI ---
    void slotRespuestaRecibida(const QString &respuesta, qint64 idLog = 0);
    void slotEstadoCambiado(const QString &estado);
    void slotErrorOcurrido(const QString &mensajeError);
    void slotHerramientaEjecutada(const QString &nombreHerramienta, const QString &resumen);
    void slotAnchorClicked(const QUrl &url);

private:
    Ui::DialogAsistenteIA *ui;
    AsistenteIA *m_asistente;
    QString m_htmlChat;

    void inicializarChat();
    void agregarBurbuja(const QString &remitente, const QString &texto, const QString &colorFondo, const QString &colorBorde, bool esUsuario, qint64 idLog = 0);
    void agregarNotificacionTool(const QString &resumen);
    void desplazarChatAlFinal();
};

#endif // DIALOGASISTENTEIA_H
