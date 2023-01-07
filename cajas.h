#ifndef CAJAS_H
#define CAJAS_H

#include <QDialog>
#include "base_datos.h"
#include "configuracion.h"
#include "entradasalida.h"
#include <QKeyEvent>

extern Configuracion *conf;
namespace Ui {
class Cajas;
}

class Cajas : public QDialog
{
    Q_OBJECT

public:
    explicit Cajas(QWidget *parent = 0);
    ~Cajas();

private slots:
    void on_spinBox20_valueChanged(int arg1);
    void on_spinBox500_valueChanged(int arg1);
    void on_spinBox200_valueChanged(int arg1);
    void on_spinBox100_valueChanged(int arg1);
    void on_spinBox50_valueChanged(int arg1);
    void on_spinBox10_valueChanged(int arg1);
    void on_spinBox5_valueChanged(int arg1);
    void on_spinBox2_valueChanged(int arg1);
    void on_spinBox1_valueChanged(int arg1);
    void on_spinBox50c_valueChanged(int arg1);
    void on_spinBox20c_valueChanged(int arg1);
    void on_spinBox10c_valueChanged(int arg1);
    void on_spinBox5c_valueChanged(int arg1);
    void on_spinBox2c_valueChanged(int arg1);
    void on_spinBox1c_valueChanged(int arg1);
    void on_doubleSpinBoxCambios_valueChanged(double arg1);
    void on_doubleSpinBoxCajaF_valueChanged(const QString &arg1);

    void actualizarEfectivo();



    void on_pushButtonRetirarDinero_clicked();

    void on_pushButtonAceptar_clicked();

private:
    Ui::Cajas *ui;

    baseDatos *base;
    double efectivoReal, totalEfectivo, ventasEfectivo, ventasTarjeta, saldoAnterior, salidas, descuadre , ventasB;
    int nTarjetas;
    QString fechaUltimoArqueo, horaUltimoArqueo;

    bool recuperarDatosUltimoArqueo();
    void ventas();
    void ES();
    EntradaSalida *entSal;
    void keyPressEvent(QKeyEvent *e);
};

#endif // CAJAS_H
