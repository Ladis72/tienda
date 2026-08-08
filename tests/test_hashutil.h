#ifndef TEST_HASHUTIL_H
#define TEST_HASHUTIL_H

#include <QObject>

class TestHashUtil : public QObject
{
    Q_OBJECT
private slots:
    void hashPassword_vectorConocido();
    void hashPassword_determinista();
    void verificarPassword_correcta();
    void verificarPassword_incorrecta();
    void esHashSHA256_valido();
    void esHashSHA256_longitudIncorrecta();
    void esHashSHA256_caracteresNoHex();
    void generarSalt_formato();
    void generarSalt_aleatorio();
};

#endif // TEST_HASHUTIL_H
