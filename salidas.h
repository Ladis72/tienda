#ifndef SALIDAS_H
#define SALIDAS_H

#include <QDialog>
#include "articulos.h"
#include "base_datos.h"
#include "buscarproducto.h"

extern Configuracion *conf;

namespace Ui {
class Salidas;
}

class Salidas : public QDialog
{
    Q_OBJECT

public:
    explicit Salidas(QWidget *parent = nullptr);
    ~Salidas();
    void aplicarPermisos();

private slots:
    void on_lineEditCod_returnPressed();

    void on_pushButtonAgregar_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonEnviar_clicked();

    void on_pushButtonBorrar_clicked();


    void actualizarTotales();

    void on_comboBoxDestino_currentIndexChanged(int index);
    void on_pushButtonCerrar_clicked();
    void on_pushButtonComparar_clicked();
    void on_pushButtonAceptarAmbas_clicked();

private:
    Ui::Salidas *ui;
    baseDatos base;
    QSqlQuery consulta;
    QSqlTableModel *mTablaSalidas;
    QSqlQueryModel *mTablaRemota;
    Articulos *articulo;
    int lineas;
    double productos;

    int m_idTiendaRemotaEnLocal;
    int m_idTiendaLocalEnRemota;
    QString m_nombreTiendaRemota;
    QString m_connRemota;

    void actualizarTabla();
    void actualizarTablaRemota();
    void llenarComboTiendas();
    bool asegurarConexionRemota(const QString &nombreTienda);
    int obtenerIdLocalEnRemota(const QString &connRemota);
    bool procesarEntradaRemota(const QString &connRemota, int idLocalEnRemota);
};

#endif // SALIDAS_H
