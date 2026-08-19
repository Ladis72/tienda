#ifndef ENCARGOSDIALOG_H
#define ENCARGOSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class EncargosDialog;
}

/**
 * @brief Estructura que representa un producto dentro de un encargo multiproducto.
 */
struct LineaEncargo {
    QString codArticulo;
    QString descripcion;
    int cantidad;
    double pvp;
};

class EncargosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncargosDialog(QString codCliente, QString nombreCliente, QString codArticulo, QString descArticulo, QString empleado, QWidget *parent = nullptr);
    ~EncargosDialog();

    int getCantidad() const;
    double getAnticipo() const;
    QString getNotas() const;

    /**
     * @brief Devuelve la forma de pago seleccionada para el adelanto (ej. Efectivo, Tarjeta, etc.)
     */
    QString getFormaPago() const;

    /**
     * @brief Devuelve el ID de la forma de pago seleccionada según la tabla fpago
     */
    int getFormaPagoId() const;

    /**
     * @brief Indica si el usuario ha seleccionado imprimir el comprobante del encargo por la impresora de tickets.
     */
    bool getImprimirTicket() const;

    /**
     * @brief Devuelve la lista completa de productos añadidos al encargo.
     */
    QList<LineaEncargo> getLineas() const;

    /**
     * @brief Añade una línea de producto a la tabla del encargo.
     */
    void addLinea(const LineaEncargo &linea);

    /**
     * @brief Reemplaza todas las líneas del encargo.
     */
    void setLineas(const QList<LineaEncargo> &lineas);

    /**
     * @brief Devuelve el importe total acumulado de todos los productos del encargo.
     */
    double getTotalEncargo() const;

    QString getCodArticulo() const;
    QString getDescArticulo() const;
    QString getCodCliente() const { return m_codCliente; }
    
private slots:
    void on_btnAnadirArticulo_clicked();
    void on_btnEliminarArticulo_clicked();
    void on_btnBuscarCliente_clicked();
    void recalcularTotal();

private:
    Ui::EncargosDialog *ui;
    QString m_codCliente;
};

#endif // ENCARGOSDIALOG_H
