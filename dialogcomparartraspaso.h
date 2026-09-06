#ifndef DIALOGCOMPARARTRASPASO_H
#define DIALOGCOMPARARTRASPASO_H

#include <QDialog>
#include <QList>
#include <QMap>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include "configuracion.h"

extern Configuracion *conf;

namespace Ui {
class DialogCompararTraspaso;
}

/**
 * @brief Estructura que representa una línea de mercancía en traspaso (entrada o salida).
 */
struct LineaMercancia {
    int id = 0;
    QString cod;
    QString fechaEntrada;
    QString descripcion;
    double cantidad = 0.0;
    QString fechaCaducidad;
    double pvp = 0.0;
    int idTienda = 0;
};

/**
 * @brief Diálogo visual para comparar en paralelo las líneas de mercancía
 * entre la tienda local y la remota, permitiendo solventar diferencias con acciones directas.
 */
class DialogCompararTraspaso : public QDialog
{
    Q_OBJECT

public:
    /**
     * @brief Constructor del comparador de traspasos.
     * @param nombreLocal Nombre descriptivo de la tienda local.
     * @param nombreRemota Nombre descriptivo de la tienda remota.
     * @param tablaLocal Nombre de la tabla local ("entradaGenero_tmp" o "salidaGenero_tmp").
     * @param tablaRemota Nombre de la tabla remota ("salidaGenero_tmp" o "entradaGenero_tmp").
     * @param idTiendaRemotaEnLocal ID de la tienda remota en la base de datos local.
     * @param idTiendaLocalEnRemota ID de la tienda local en la base de datos remota.
     * @param connRemota Nombre de la conexión Qt a la base de datos de la tienda remota.
     * @param parent Widget padre.
     */
    explicit DialogCompararTraspaso(const QString &nombreLocal,
                                   const QString &nombreRemota,
                                   const QString &tablaLocal,
                                   const QString &tablaRemota,
                                   int idTiendaRemotaEnLocal,
                                   int idTiendaLocalEnRemota,
                                   const QString &connRemota,
                                   QWidget *parent = nullptr);
    ~DialogCompararTraspaso();

signals:
    /// Emitida cuando se realizan cambios en la base de datos local (importar, ajustar o igualar)
    void datosModificados();

private slots:
    void on_btnImportarFaltantes_clicked();
    void on_btnAjustarCantidades_clicked();
    void on_btnIgualarTodo_clicked();

private:
    Ui::DialogCompararTraspaso *ui;

    QString m_nombreLocal;
    QString m_nombreRemota;
    QString m_tablaLocal;
    QString m_tablaRemota;
    int m_idTiendaRemotaEnLocal;
    int m_idTiendaLocalEnRemota;
    QString m_connRemota;

    QMap<QString, LineaMercancia> m_locales;
    QMap<QString, LineaMercancia> m_remotas;

    void cargarDatos();
    void compararYMostrar();
};

#endif // DIALOGCOMPARARTRASPASO_H
