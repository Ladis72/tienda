#ifndef TOTALIZAR_H
#define TOTALIZAR_H

#include <QDialog>
#include <base_datos.h>

namespace Ui {
class totalizar;
}

class totalizar : public QDialog
{
    Q_OBJECT
    
public:
    explicit totalizar(QStringList datos, QWidget *parent = 0);
    ~totalizar();

    QString efectivo;
    
private slots:
    void on_pushButtonTicket_clicked();

    void on_lineEditDescuento_textChanged(const QString &arg1);

    void on_lineEditEntrega_textChanged(const QString &arg1);


    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::totalizar *ui;
    QSqlQuery fpago;
    baseDatos *base;

};

#endif // TOTALIZAR_H
