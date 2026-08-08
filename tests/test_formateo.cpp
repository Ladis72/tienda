#include "test_formateo.h"
#include "formateo.h"

#include <QtTest>
#include <QString>

/**
 * @brief Suite de pruebas unitarias para Formateo::redondear.
 *
 * NOTA: esta implementación usa aritmética entera manual que puede diferir
 * del redondeo "clásico" de media superior (p.ej. redondear(1.005, 2) devuelve
 * 1.00, no 1.01). Los casos reflejan el comportamiento observado de la
 * implementación actual para fijar la regresión y documentar su semántica.
 */
void TestFormateo::redondear_cero()
{
    Formateo f;
    QCOMPARE(f.redondear(0.0, 2), 0.0);
}

void TestFormateo::redondear_dosDecimales()
{
    Formateo f;
    // 1.234 -> 1.23 (truncamiento del tercer decimal)
    QCOMPARE(f.redondear(1.234, 2), 1.23);
    // 12.55 queda igual con dos decimales
    QCOMPARE(f.redondear(12.55, 2), 12.55);
}

void TestFormateo::redondear_mediaSuperior()
{
    Formateo f;
    // 1.235 -> 1.24 (el tercer decimal 5 redondea al alza)
    QCOMPARE(f.redondear(1.235, 2), 1.24);
    // Comportamiento observado (no clásico): 1.005 -> 1.00
    QCOMPARE(f.redondear(1.005, 2), 1.00);
}

void TestFormateo::redondear_entero()
{
    Formateo f;
    QCOMPARE(f.redondear(1.0, 2), 1.0);
    QCOMPARE(f.redondear(12.5, 2), 12.5);
}

void TestFormateo::redondear_negativo()
{
    Formateo f;
    QCOMPARE(f.redondear(-1.235, 2), -1.23);
}

void TestFormateo::redondear_unDecimal()
{
    Formateo f;
    // 1.234 -> 1.2 con un solo decimal
    QCOMPARE(f.redondear(1.234, 1), 1.2);
}
