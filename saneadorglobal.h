#ifndef SANEADORGLOBAL_H
#define SANEADORGLOBAL_H

#include <QDialog>
#include <QSqlDatabase>
#include <QTableWidgetItem>
#include "unificarmaestros.h"

namespace Ui {
class SaneadorGlobal;
}

class Configuracion;
extern Configuracion *conf;

/**
 * @brief El SaneadorGlobal permite confrontar y alinear IDs de maestros entre tiendas.
 * 
 * Esta herramienta lee los registros de la tienda local (maestra) y de una tienda remota,
 * permitiendo al usuario igualar IDs, clonar registros faltantes o fusionar conceptos.
 */
class SaneadorGlobal : public QDialog
{
    Q_OBJECT

public:
    explicit SaneadorGlobal(const UnificarMaestrosConfig &config, QWidget *parent = nullptr);
    ~SaneadorGlobal();
    void aplicarPermisos();

private slots:
    void on_comboBoxTienda_currentIndexChanged(int index);
    void on_pushButtonAnalizar_clicked();
    void on_pushButtonIgualar_clicked();
    void on_pushButtonIgualarLocal_clicked();
    void on_pushButtonClonar_clicked();
    void on_pushButtonClonarLocal_clicked();
    void on_pushButtonCerrar_clicked();
    void on_tableWidget_itemSelectionChanged();

private:
    Ui::SaneadorGlobal *ui;
    UnificarMaestrosConfig m_config;
    QString m_nombreConexionRemota;

    void log(const QString &msg);
    void refrescarComparativa();
    
    /// Algoritmo único de alineación: cambia un ID en targetDb actualizando sus dependencias
    bool ejecutarAlineacion(QSqlDatabase targetDb, int idNuevo, int idActual, const QString &nombre);
    
    /// Algoritmo único de clonación: copia de srcDb a destDb
    bool ejecutarClonacion(QSqlDatabase srcDb, QSqlDatabase destDb, int idSource, const QString &nombre);

    /// Desplaza un ID ocupado en la base de datos indicada para dejar sitio al ID deseado
    bool desplazarID(QSqlDatabase db, int idOcupado);

    QSqlDatabase dbLocal();
    QSqlDatabase dbRemota();
};

#endif // SANEADORGLOBAL_H
