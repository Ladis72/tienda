#include "test_hashutil.h"
#include "hashutil.h"

#include <QtTest>
#include <QString>
#include <QRegularExpression>

/**
 * @brief Suite de pruebas unitarias para HashUtil (SEC-02).
 *
 * Verifica el hash SHA-256 con salt, la verificación de contraseñas,
 * la detección de hashes ya calculados y la generación de salts.
 */
void TestHashUtil::hashPassword_vectorConocido()
{
    // salt = "saltFijo12345678", clave = "secreto"
    // SHA256("saltFijo12345678secreto")
    const QString esperado = "a8735425614d6bd0ff455fae82184e1e1235f73090bb2c7af3129782c63e7729";
    QCOMPARE(HashUtil::hashPassword("secreto", "saltFijo12345678"), esperado);
}

void TestHashUtil::hashPassword_determinista()
{
    QCOMPARE(HashUtil::hashPassword("abc", "miSalt"),
             HashUtil::hashPassword("abc", "miSalt"));
}

void TestHashUtil::verificarPassword_correcta()
{
    QString salt = HashUtil::generarSalt();
    QString hash = HashUtil::hashPassword("claveSegura", salt);
    QVERIFY(HashUtil::verificarPassword("claveSegura", hash, salt));
}

void TestHashUtil::verificarPassword_incorrecta()
{
    QString salt = HashUtil::generarSalt();
    QString hash = HashUtil::hashPassword("claveSegura", salt);
    QVERIFY(!HashUtil::verificarPassword("claveErronea", hash, salt));
}

void TestHashUtil::esHashSHA256_valido()
{
    QVERIFY(HashUtil::esHashSHA256("a8735425614d6bd0ff455fae82184e1e1235f73090bb2c7af3129782c63e7729"));
}

void TestHashUtil::esHashSHA256_longitudIncorrecta()
{
    QVERIFY(!HashUtil::esHashSHA256("a8735425614d6bd0ff455fae82184e1e1235f73090bb2c7af3129782c63e772"));
    QVERIFY(!HashUtil::esHashSHA256(""));
}

void TestHashUtil::esHashSHA256_caracteresNoHex()
{
    QVERIFY(!HashUtil::esHashSHA256(QString(64, 'z')));
    QVERIFY(!HashUtil::esHashSHA256(QString(64, 'G')));
}

void TestHashUtil::generarSalt_formato()
{
    QString salt = HashUtil::generarSalt();
    QCOMPARE(salt.size(), 32);
    static const QRegularExpression hexRe("^[0-9a-f]{32}$");
    QVERIFY(hexRe.match(salt).hasMatch());
}

void TestHashUtil::generarSalt_aleatorio()
{
    QString a = HashUtil::generarSalt();
    QString b = HashUtil::generarSalt();
    QVERIFY(a != b);
}
