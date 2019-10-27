#ifndef CLIENTES_H
#define CLIENTES_H
#include "base_datos.h"
#include "buscarproducto.h"

#include <QDialog>

namespace Ui {
class Clientes;
}

class Clientes : public QDialog
{
    Q_OBJECT

public:
    explicit Clientes(QWidget *parent = nullptr);
    explicit Clientes(QWidget *parent , QString codigo );
    ~Clientes();

private slots:
    void on_pushButtonAnterior_clicked();

    void on_pushButtonNuevo_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonBorrar_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_lineEditNombre_returnPressed();


    void on_lineEditCod_editingFinished();

    void on_radioButtonMeses_clicked();

    void on_radioButtonAnos_clicked();

    void on_radioButtonFechas_clicked();

    void on_dateEditHasta_userDateChanged(const QDate &date);

    void on_dateEditDesde_userDateChanged(const QDate &date);

private:
    Ui::Clientes *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QSqlQueryModel modeloCompras;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;


    void recargarTabla();
    void borrarFormulario();
    bool eventFilter(QObject *obj, QEvent *event);
    void refrescarBotones(int i);
    void cargarCompras();
    QStringList recogerDatosFormulario();

};

#endif // CLIENTES_H
