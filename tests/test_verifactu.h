#ifndef TEST_VERIFACTU_H
#define TEST_VERIFACTU_H

#include <QObject>

class TestVerifactu : public QObject
{
    Q_OBJECT
private slots:
    void formatearDecimal_entero();
    void formatearDecimal_media();
    void formatearDecimal_conDosDecimales();
    void formatearDecimal_cero();
    void huellaAlta_primerRegistro();
    void huellaAlta_registroEncadenado();
    void huellaAlta_normaliza64Ceros();
    void urlQR_entornoPruebas();
    void urlQR_entornoProduccion();
    void urlQR_nifMayusculas();
    void fechaHoraHuso_formato();
};

#endif // TEST_VERIFACTU_H
