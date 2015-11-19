#ifndef DIALOGFECHA_H
#define DIALOGFECHA_H

#include <QDialog>
#include <QDate>

namespace Ui {
class DialogFecha;
}

class DialogFecha : public QDialog
{
    Q_OBJECT

public:
    explicit DialogFecha(QString producto, QWidget *parent = 0);
    ~DialogFecha();
    QDate fecha;
    QString lote;

private slots:
    //void on_buttonBox_accepted();

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_pushButton_clicked();

private:
    Ui::DialogFecha *ui;

};

#endif // DIALOGFECHA_H
