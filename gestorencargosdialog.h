#ifndef GESTORENCARGOSDIALOG_H
#define GESTORENCARGOSDIALOG_H

#include <QDialog>
#include <QSqlQueryModel>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class GestorEncargosDialog;
}

class GestorEncargosDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GestorEncargosDialog(QString autoFilterCliente = "", QWidget *parent = nullptr);
    ~GestorEncargosDialog();

private slots:
    void on_comboBoxEstado_currentTextChanged(const QString &arg1);
    void on_lineEditFiltroCliente_textChanged(const QString &arg1);
    void on_btnMarcarRecibido_clicked();
    void on_btnMarcarEntregado_clicked();
    void on_btnBorrar_clicked();
    void on_btnCerrar_clicked();
    void on_btnNuevoEncargo_clicked();
    void on_btnCobrarTPV_clicked();
    void on_btnImprimirTicket_clicked();

private:
    Ui::GestorEncargosDialog *ui;
    QSqlQueryModel *modelEncargos;
    void refrescarModelo();
    void ajustarFiltro();
    baseDatos base;
};

#endif // GESTORENCARGOSDIALOG_H
