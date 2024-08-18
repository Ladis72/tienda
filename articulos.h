#ifndef ARTICULOS_H
#define ARTICULOS_H

#include "base_datos.h"
#include "buscarproducto.h"
#include "conexionesremotas.h"
#include "configuracion.h"

#include "ClickableLabel.h"
#include "comprasventasremoto.h"
#include "fabricantes.h"
#include "familias.h"
#include "stock.h"
#include "visorfacturas.h"
#include "visorimagenes.h"

#include <QDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QTreeWidget>

extern Configuracion *conf;
namespace Ui {
class Articulos;
}

class Articulos : public QDialog
{
    Q_OBJECT

public:
    explicit Articulos(QWidget *parent = nullptr);
    ~Articulos();
    void borrarFormulario();

private slots:
    void on_pushButtonAnterior_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_pushButtonPonerFoto_clicked();

    void on_pushButtonBorrarFoto_clicked();

    void on_pushButtonRefrescar_clicked();

    void on_lineEditCodFamila_textChanged(const QString &arg1);

    void on_lineEditCodFabricante_textChanged(const QString &arg1);

    void on_lineEditDesc_returnPressed();

    void on_lineEditCod_returnPressed();

    void on_pushButtonBuscarFamilia_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtonNuevo_clicked();

    void on_radioButtonVentasDia_clicked();

    void on_radioButtonVentasMes_clicked();

    void on_radioButtonVentasAno_clicked();

    void on_radioButtonFacturas_clicked();

    void on_radioButtonMeses_clicked();

    void on_radioButtonAnos_clicked();

    void on_pushButtonVer_clicked();

    void on_pushButtonAnadir_clicked();

    void on_pushButtonEliminar_clicked();

    void on_tableViewCompras_clicked(const QModelIndex &index);

    void on_pushButtonEtiqueta_clicked();

    void on_pushButtonCaducados_clicked();

    void on_pushButtonVer_2_clicked();
    void mostrarFoto();

    void on_pushButtonVerFactura_clicked();

    void on_checkBoxRemoto_stateChanged(int arg1);

    void on_treeWidgetStockTiendas_itemDoubleClicked(QTreeWidgetItem *item, int column);

private:
    Ui::Articulos *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QSqlQueryModel modeloVentas;
    QSqlQueryModel modeloCompras;
    QSqlTableModel *modeloAux;
    QDataWidgetMapper mapper;
    QSqlQuery consulta, consultaRemota;
    Stock *stock;
    VisorImagenes *visor;
    VisorFacturas *factura;
    comprasVentasRemoto *cvr;
    QString nFactura, idProveedor;
    conexionesRemotas *conexiones;
    QStringList listaConexionesRemotas;
    bool remoto;

    //Familias *F;

    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();
    void recargarTabla();
    void cargarVentas();
    bool eventFilter(QObject *obj, QEvent *event);
    void keyPressEvent(QKeyEvent *e);
    void cargarCompras();
    void cargarCodAux();
    void llenarComboFormatos();
    void llenarStockRemoto(QString ean);
    static QStringList crearConexionesRemotas(QSqlQuery consultaRemota);
};

#endif // ARTICULOS_H
