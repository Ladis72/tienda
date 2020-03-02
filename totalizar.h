#ifndef TOTALIZAR_H
#define TOTALIZAR_H

#include <QDialog>
#include <base_datos.h>
#include <QKeyEvent>

namespace Ui {
class totalizar;
}

class totalizar : public QDialog
{
    Q_OBJECT
    
public:
    explicit totalizar(QString datos, double vale , QWidget *parent = nullptr);
    ~totalizar();

    QString efectivo;
    double descuento,total,entrega,cambio,cantVale;
    int formaPago;
    QString facturacion;
    bool ticket;
    bool factura;
    bool valeUsado;
    
private slots:
    void on_pushButtonTicket_clicked();

    void on_lineEditDescuento_textChanged(const QString &arg1);

    void on_lineEditEntrega_textChanged(const QString &arg1);


    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_comboBox_currentIndexChanged(int index);

    void on_pushButtonCobrar_clicked();

    void on_pushButtonFactura_clicked();

    void on_checkBoxVale_stateChanged(int arg1);

private:
    Ui::totalizar *ui;
    QSqlQuery fpago;
    baseDatos *base;

    void keyPressEvent(QKeyEvent *e);

};

#endif // TOTALIZAR_H
