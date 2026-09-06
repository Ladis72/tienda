#ifndef ENTRADAMERCANCIA_H
#define ENTRADAMERCANCIA_H

#include <QDialog>
#include "articulos.h"
#include "base_datos.h"
#include "buscarproducto.h"

extern Configuracion *conf;

namespace Ui {
class EntradaMercancia;
}

class EntradaMercancia : public QDialog
{
    Q_OBJECT

public:
    explicit EntradaMercancia(QWidget *parent = 0);
    ~EntradaMercancia();
    void aplicarPermisos();

private slots:
    void on_pushButtonAceptar_clicked();

    void on_lineEditCod_returnPressed();

    void on_pushButtonAgregarLinea_clicked();

    void on_lineEditDesc_returnPressed();

    void on_pushButtonBorrar_clicked();

    void on_dateEditCaducidad_editingFinished();

    //void on_comboBoxProcedencia_activated(const QString &arg1);

    void actualizarTotales();
    void procesarLineaEntrada(const QSqlRecord &registro);
    void actualizarArticulo(const QString &cod, const QString &descripcion, const QString &precio);
    void guardarArticulo(int idTienda);
    void limpiarTabla(int idTienda);

    void on_comboBoxProcedencia_currentIndexChanged(int index);
    void on_pushButtonSalir_clicked();
    void on_pushButtonComparar_clicked();
    void on_pushButtonAceptarAmbas_clicked();

private:
    Ui::EntradaMercancia *ui;
    baseDatos *base;
    QSqlTableModel *mTablaEntradas;
    QSqlQueryModel *mTablaRemota;
    QSqlQuery consulta;
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
    bool procesarSalidaRemota(const QString &connRemota, int idLocalEnRemota);
};

#endif // ENTRADAMERCANCIA_H
