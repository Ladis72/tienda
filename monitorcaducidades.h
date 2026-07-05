#ifndef MONITORCADUCIDADES_H
#define MONITORCADUCIDADES_H

#include <QObject>
#include <QThread>
#include <QTimer>
#include <QDate>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QSharedMemory>

struct RecomendacionCaducidad {
    QString ean;
    QString lote;
    QString descripcion;
    QDate fechaCaducidad;
    double stockLote;
    double mediaVentaMensualLocal;
    int diasRestantes;
    QString sugerencia;
    int nivelAlerta; // 0: Normal, 1: Aviso (1 año), 2: Crítico (6 meses)
};

Q_DECLARE_METATYPE(QList<RecomendacionCaducidad>)

class MonitorCaducidades : public QObject
{
    Q_OBJECT
public:
    explicit MonitorCaducidades(QString conexionLocal, QObject *parent = nullptr);
    ~MonitorCaducidades();

    void iniciar();

signals:
    void analisisCompletado(QList<RecomendacionCaducidad> recomendaciones);
    void errorOcurrido(QString mensaje);
    void finished();

public slots:
    void ejecutarAnalisis();

private:
    QString m_driver;
    QString m_host;
    int m_port;
    QString m_dbName;
    QString m_user;
    QString m_password;
};

#endif // MONITORCADUCIDADES_H
