#ifndef DIALOGDETALLEARQUEO_H
#define DIALOGDETALLEARQUEO_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class DialogDetalleArqueo;
}

/**
 * @class DialogDetalleArqueo
 * @brief Diálogo que muestra el desglose de monedas y billetes para un arqueo de caja específico.
 */
class DialogDetalleArqueo : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param idArqueo ID del arqueo en la base de datos.
     * @param parent Widget padre.
     */
    explicit DialogDetalleArqueo(int idArqueo, QWidget *parent = nullptr);
    ~DialogDetalleArqueo();

private:
    Ui::DialogDetalleArqueo *ui;      ///< Interfaz del diálogo
    int m_idArqueo;                   ///< Identificador del arqueo consultado
    QSqlQueryModel *modeloDetalle;    ///< Modelo para la consulta SQL del desglose

    /**
     * @brief Carga el desglose de monedas y billetes desde la tabla arqueos_detalle.
     */
    void cargarDetalle();
};

#endif // DIALOGDETALLEARQUEO_H
