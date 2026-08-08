#ifndef TEST_FORMATEO_H
#define TEST_FORMATEO_H

#include <QObject>

class TestFormateo : public QObject
{
    Q_OBJECT
private slots:
    void redondear_cero();
    void redondear_dosDecimales();
    void redondear_mediaSuperior();
    void redondear_entero();
    void redondear_negativo();
    void redondear_unDecimal();
};

#endif // TEST_FORMATEO_H
