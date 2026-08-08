#ifndef DIALOGDETALLESVERIFICADOR_H
#define DIALOGDETALLESVERIFICADOR_H

#include <QDialog>
#include <QStringList>

namespace Ui {
class DialogDetallesVerificador;
}

/**
 * @class DialogDetallesVerificador
 * @brief Diálogo personalizable y redimensionable para visualizar en detalle los registros
 *        incoherentes o errores encontrados en las comprobaciones del verificador de base de datos.
 */
class DialogDetallesVerificador : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del diálogo de detalles.
     * @param titulo Título o nombre del diagnóstico de la comprobación.
     * @param totalErrores Cantidad total de registros incoherentes detectados.
     * @param detalles Lista de cadenas con los detalles de cada inconsistencia.
     * @param parent Widget padre opcional.
     */
    explicit DialogDetallesVerificador(const QString &titulo,
                                       int totalErrores,
                                       const QStringList &detalles,
                                       QWidget *parent = nullptr);
    ~DialogDetallesVerificador();

private:
    Ui::DialogDetallesVerificador *ui; ///< Puntero a la interfaz gráfica generada desde el archivo .ui
};

#endif // DIALOGDETALLESVERIFICADOR_H
