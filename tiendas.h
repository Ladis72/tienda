#ifndef TIENDAS_H
#define TIENDAS_H

#include <QDialog>
#include "base_datos.h"
#include "conexionesremotas.h"
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class tiendas;
}

class tiendas : public QDialog
{
    Q_OBJECT

public:
    explicit tiendas(QWidget *parent = nullptr);
    ~tiendas();

private slots:
    void on_pushButtonNuevo_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_pushButtonAnterior_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_pushButtonRefrescar_clicked();

    void on_checkBoxMaster_stateChanged(int arg1);

    void on_checkBoxLocal_stateChanged(int arg1);

private:
    Ui::tiendas *ui;
    baseDatos *base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;

    void recargarTabla();
    void refrescarBotones(int i);
    void borrarFormulario();
    QStringList recogerDatos();
    QString nombreConexionMaster;
};

#endif // TIENDAS_H
