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
    explicit Clientes(QWidget *parent = 0);
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

private:
    Ui::Clientes *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;


    void recargarTabla();
    void borrarFormulario();
    bool eventFilter(QObject *obj, QEvent *event);
    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();

};

#endif // CLIENTES_H
