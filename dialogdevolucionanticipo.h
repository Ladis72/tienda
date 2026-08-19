#ifndef DIALOGDEVOLUCIONANTICIPO_H
#define DIALOGDEVOLUCIONANTICIPO_H

#include <QDialog>

namespace Ui {
class DialogDevolucionAnticipo;
}

/**
 * @brief Diálogo para seleccionar la forma de pago utilizada al devolver el anticipo de un encargo cancelado.
 */
class DialogDevolucionAnticipo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogDevolucionAnticipo(double anticipo, QString formaPagoOriginal, QWidget *parent = nullptr);
    ~DialogDevolucionAnticipo();

    /**
     * @brief Devuelve si se debe registrar la devolución del anticipo en caja.
     */
    bool getDevolverAnticipo() const;

    /**
     * @brief Devuelve el nombre textual de la forma de pago seleccionada para la devolución.
     */
    QString getFormaPago() const;

    /**
     * @brief Devuelve el ID de la forma de pago seleccionada.
     */
    int getFormaPagoId() const;

private:
    Ui::DialogDevolucionAnticipo *ui;
};

#endif // DIALOGDEVOLUCIONANTICIPO_H
