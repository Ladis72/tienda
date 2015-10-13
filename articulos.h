#ifndef ARTICULOS_H
#define ARTICULOS_H

#include "base_datos.h"
#include "buscarproducto.h"

#include "familias.h"
#include "fabricantes.h"


#include <QDialog>

namespace Ui {
class Articulos;
}

class Articulos : public QDialog
{
    Q_OBJECT
    
public:
    explicit Articulos(QWidget *parent = 0);
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

    void on_lineEditCod_editingFinished();

    void on_pushButtonBuscarFamilia_clicked();

    void on_pushButton_2_clicked();

    void on_pushButtonNuevo_clicked();

private:
    Ui::Articulos *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;
    //Familias *F;

    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();
    void recargarTabla();
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // ARTICULOS_H
