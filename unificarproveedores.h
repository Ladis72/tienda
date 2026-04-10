#ifndef UNIFICARPROVEEDORES_H
#define UNIFICARPROVEEDORES_H

#include <QDialog>
#include <QSqlDatabase>
#include <QMap>

#include "base_datos.h"

namespace Ui {
class UnificarProveedores;
}

class UnificarProveedores : public QDialog
{
    Q_OBJECT

public:
    explicit UnificarProveedores(QWidget *parent = nullptr);
    ~UnificarProveedores();

private slots:
    // Fase 0: Saneamiento
    void on_pushButtonNormalizarNIF_clicked();
    void on_pushButtonDetectarNIF_clicked();

    // Fase 1: Unificación
    void on_pushButtonAnalizarIDs_clicked();
    void on_pushButtonGenerarSQL_clicked();
    void on_pushButtonEjecutarUnificacion_clicked();

private:
    Ui::UnificarProveedores *ui;
    baseDatos base;
    void log(const QString &msg);
    QString normalizarNIF(QString nif);
    bool resolverConflictoID(QSqlDatabase db, int targetId, QString exceptNif);
    
    struct Discrepancia {
        QString nombre;
        QString nif;
        int idLocal;
        int idRemoto;
        QString accion; // "Sincronizar", "Clonar", "Importar"
    };
    QList<Discrepancia> discrepanciasActuales;
};

#endif // UNIFICARPROVEEDORES_H
