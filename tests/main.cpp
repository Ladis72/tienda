#include "test_hashutil.h"
#include "test_verifactu.h"
#include "test_formateo.h"

#include <QtTest>
#include <QApplication>

/**
 * @brief Runner de la suite de tests unitarios.
 *
 * Ejecuta las tres suites (HashUtil, VeriFactu y Formateo) y devuelve
 * un código de salida distinto de 0 si alguna falla (usado por el CI).
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    int status = 0;

    TestHashUtil tHash;
    status |= QTest::qExec(&tHash, argc, argv);

    TestVerifactu tVerifactu;
    status |= QTest::qExec(&tVerifactu, argc, argv);

    TestFormateo tFormateo;
    status |= QTest::qExec(&tFormateo, argc, argv);

    return status;
}
