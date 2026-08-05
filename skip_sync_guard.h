#ifndef SKIP_SYNC_GUARD_H
#define SKIP_SYNC_GUARD_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QString>

/**
 * @brief Guard RAII para la variable de sesión MySQL @skip_sync.
 *
 * Establece SET @skip_sync = 1 sobre la conexión indicada al construirse y
 * garantiza que se restablece a 0 al salir del ámbito (también en early returns
 * y excepciones). Evita que los triggers de sincronización queden desactivados
 * silenciosamente si una función abandona la ejecución a mitad de camino.
 */
class SkipSyncGuard
{
public:
    explicit SkipSyncGuard(const QString &conexion)
        : m_conexion(conexion)
    {
        QSqlQuery q(QSqlDatabase::database(m_conexion));
        q.exec("SET @skip_sync = 1");
    }

    ~SkipSyncGuard()
    {
        QSqlQuery q(QSqlDatabase::database(m_conexion));
        q.exec("SET @skip_sync = 0");
    }

private:
    QString m_conexion;
};

#endif // SKIP_SYNC_GUARD_H
