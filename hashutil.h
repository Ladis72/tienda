#ifndef HASHUTIL_H
#define HASHUTIL_H

#include <QCryptographicHash>
#include <QRandomGenerator>
#include <QRegularExpression>
#include <QString>

/**
 * @brief Utilidad de hashing de contraseñas (SEC-02).
 *
 * Proporciona funciones estáticas para generar un salt aleatorio,
 * calcular el hash SHA-256 de una contraseña con salt, y verificar
 * una contraseña contra un hash almacenado.
 *
 * El hash se calcula como: SHA256(salt + contraseña)
 * El salt es un string hexadecimal de 32 caracteres (16 bytes).
 *
 * Uso:
 * @code
 *   // Al crear/modificar un usuario:
 *   QString salt = HashUtil::generarSalt();
 *   QString hash = HashUtil::hashPassword(contraseña, salt);
 *   // Guardar salt y hash en la tabla usuarios
 *
 *   // Al verificar en el login:
 *   bool ok = HashUtil::verificarPassword(contraseñaIntroducida, hashAlmacenado, saltAlmacenado);
 * @endcode
 */
class HashUtil
{
public:
    /**
     * @brief Genera un salt aleatorio de 16 bytes en formato hexadecimal.
     * @return String de 32 caracteres hex (ej: "a3b1c4d5e6f7...")
     */
    static QString generarSalt()
    {
        QByteArray saltBytes;
        saltBytes.resize(16);
        // Rellenar con bytes aleatorios criptográficamente seguros
        for (int i = 0; i < 16; ++i) {
            saltBytes[i] = static_cast<char>(QRandomGenerator::securelySeeded().bounded(256));
        }
        return QString(saltBytes.toHex());
    }

    /**
     * @brief Calcula el hash SHA-256 de una contraseña con salt.
     * @param password Contraseña en texto plano.
     * @param salt Salt en formato hexadecimal.
     * @return Hash SHA-256 en formato hexadecimal (64 caracteres).
     */
    static QString hashPassword(const QString &password, const QString &salt)
    {
        QByteArray datos = (salt + password).toUtf8();
        QByteArray hash = QCryptographicHash::hash(datos, QCryptographicHash::Sha256);
        return QString(hash.toHex());
    }

    /**
     * @brief Verifica una contraseña contra un hash almacenado.
     * @param password Contraseña introducida por el usuario.
     * @param hashAlmacenado Hash almacenado en la BD.
     * @param salt Salt almacenado en la BD.
     * @return true si la contraseña coincide con el hash.
     */
    static bool verificarPassword(const QString &password,
                                  const QString &hashAlmacenado,
                                  const QString &salt)
    {
        QString hashCalculado = hashPassword(password, salt);
        return hashCalculado == hashAlmacenado;
    }

    /**
     * @brief Comprueba si una clave parece estar ya hasheada.
     *
     * Un hash SHA-256 en hex tiene exactamente 64 caracteres y solo
     * contiene dígitos hexadecimales [0-9a-f].
     *
     * @param clave Valor almacenado en la columna 'clave'.
     * @return true si parece un hash SHA-256.
     */
    static bool esHashSHA256(const QString &clave)
    {
        if (clave.length() != 64) return false;
        static const QRegularExpression hexRegex("^[0-9a-f]{64}$");
        return hexRegex.match(clave).hasMatch();
    }
};

#endif // HASHUTIL_H
