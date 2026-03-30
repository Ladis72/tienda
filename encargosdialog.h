#ifndef ENCARGOSDIALOG_H
#define ENCARGOSDIALOG_H

#include <QDialog>
#include <QSqlDatabase>

namespace Ui {
class EncargosDialog;
}

class EncargosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EncargosDialog(QString codCliente, QString nombreCliente, QString codArticulo, QString descArticulo, QString empleado, QWidget *parent = nullptr);
    ~EncargosDialog();

    int getCantidad() const;
    double getAnticipo() const;
    QString getNotas() const;
    QString getCodArticulo() const { return m_codArticulo; }
    QString getDescArticulo() const { return m_descArticulo; }
    QString getCodCliente() const { return m_codCliente; }
    
private slots:
    void on_btnBuscarArticulo_clicked();
    void on_btnBuscarCliente_clicked();

private:
    Ui::EncargosDialog *ui;
    QString m_codArticulo;
    QString m_descArticulo;
    QString m_codCliente;
};

#endif // ENCARGOSDIALOG_H
