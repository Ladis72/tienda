#include "test_verifactu.h"
#include "verifactuclass.h"

#include <QtTest>
#include <QString>
#include <QRegularExpression>
#include <QCryptographicHash>

/**
 * @brief Suite de pruebas unitarias para las funciones estáticas de VeriFactu.
 *
 * Verifica el formateo de decimales AEAT, la cadena canónica de huella SHA-256
 * (encadenamiento con el caso de primer registro), la URL de cotejo del QR y
 * el formato ISO-8601 con huso horario.
 */
void TestVerifactu::formatearDecimal_entero()
{
    QCOMPARE(verifactuClass::formatearDecimalAEAT(12.00), QString("12"));
}

void TestVerifactu::formatearDecimal_media()
{
    QCOMPARE(verifactuClass::formatearDecimalAEAT(12.50), QString("12.5"));
}

void TestVerifactu::formatearDecimal_conDosDecimales()
{
    QCOMPARE(verifactuClass::formatearDecimalAEAT(12.55), QString("12.55"));
}

void TestVerifactu::formatearDecimal_cero()
{
    QCOMPARE(verifactuClass::formatearDecimalAEAT(0.00), QString("0"));
}

void TestVerifactu::huellaAlta_primerRegistro()
{
    // Vector oficial AEAT (caso "primer registro", Huella vacía).
    // Cadena canónica:
    // IDEmisorFactura=B12345678&NumSerieFactura=2026F1&FechaExpedicionFactura=01-08-2026
    // &TipoFactura=F1&CuotaTotal=17.36&ImporteTotal=116.36&Huella=
    // &FechaHoraHusoGenRegistro=2026-08-01T12:00:00+02:00
    const QString esperado = "3FAE17C7E994300002BC8C88E93B73767EE219D58FD8801501F714AB7D23D08C";
    QString actual = verifactuClass::calcularHuellaAlta(
        "b12345678",       // NIF en minúsculas -> se normaliza a mayúsculas
        "2026F1",
        "01-08-2026",
        "F1",
        "17.36",
        "116.36",
        "",                 // primer registro: huella vacía
        "2026-08-01T12:00:00+02:00");
    QCOMPARE(actual, esperado);
}

void TestVerifactu::huellaAlta_registroEncadenado()
{
    // Segundo registro: la huella anterior es la del registro previo.
    const QString huellaAnterior = "3FAE17C7E994300002BC8C88E93B73767EE219D58FD8801501F714AB7D23D08C";
    const QString esperado = "45F5EEF5DC47887950D4F221B38B6DE10470C27298785C11F4AE3A150B7CE3AE";
    QString actual = verifactuClass::calcularHuellaAlta(
        "B12345678",
        "2026F2",
        "02-08-2026",
        "F2",
        "5.00",
        "30.00",
        huellaAnterior,
        "2026-08-02T10:30:00+02:00");
    QCOMPARE(actual, esperado);
}

void TestVerifactu::huellaAlta_normaliza64Ceros()
{
    // El antiguo sentinela de 64 ceros debe tratarse como huella vacía
    // (equivale al primer registro de la serie).
    const QString sesentaYCuatroCeros = QString(64, '0');
    QCOMPARE(verifactuClass::calcularHuellaAlta(
                 "B12345678", "2026F1", "01-08-2026", "F1",
                 "17.36", "116.36", sesentaYCuatroCeros,
                 "2026-08-01T12:00:00+02:00"),
             verifactuClass::calcularHuellaAlta(
                 "B12345678", "2026F1", "01-08-2026", "F1",
                 "17.36", "116.36", "",
                 "2026-08-01T12:00:00+02:00"));
}

void TestVerifactu::urlQR_entornoPruebas()
{
    QString url = verifactuClass::generarUrlQR(
        "b12345678", "2026F1", "01-08-2026", 116.36, 0);
    QVERIFY(url.startsWith("https://prewww2.aeat.es/wlpl/TIKE-CONT/ValidarQR"));
    QVERIFY(url.contains("nif=B12345678"));
    QVERIFY(url.contains("numserie=2026F1"));
    QVERIFY(url.contains("fecha=01-08-2026"));
    QVERIFY(url.contains("importe=116.36"));
}

void TestVerifactu::urlQR_entornoProduccion()
{
    QString url = verifactuClass::generarUrlQR(
        "B12345678", "2026F1", "01-08-2026", 116.36, 1);
    QVERIFY(url.startsWith("https://www2.agenciatributaria.gob.es/wlpl/TIKE-CONT/ValidarQR"));
}

void TestVerifactu::urlQR_nifMayusculas()
{
    QString url = verifactuClass::generarUrlQR(
        "b12345678", "2026F1", "01-08-2026", 10, 1);
    QVERIFY(url.contains("nif=B12345678"));
}

void TestVerifactu::fechaHoraHuso_formato()
{
    // Formato AEAT: YYYY-MM-DDTHH:mm:ss+HH:MM
    static const QRegularExpression re(
        "^\\d{4}-\\d{2}-\\d{2}T\\d{2}:\\d{2}:\\d{2}[+-]\\d{2}:\\d{2}$");
    QString s = verifactuClass::obtenerFechaHoraHusoActual();
    QVERIFY2(re.match(s).hasMatch(), qPrintable(s));
}
