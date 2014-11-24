#ifndef TABWIDGET_H
#define TABWIDGET_H

#include <QTabWidget>
#include "base_datos.h"
#include "buscarproducto.h"




namespace Ui {
class Ususarios;
}

class Ususarios : public QTabWidget
{
    Q_OBJECT
    
public:
    explicit Ususarios(QWidget *parent = 0);
    ~Ususarios();
    
private slots:
    void on_pushButtonAnterior_clicked();

    void on_pushButtonSiguiente_clicked();

    void on_pushButtonModificar_clicked();

    void on_pushButtonRefrescar_clicked();

    void on_pushButtonNuevo_clicked();

    void on_lineEditNombre_returnPressed();

    void on_pushButtonCargarImagen_clicked();

    void on_pushButtonBorrarImagen_clicked();

    void on_pushButtonBorrar_clicked();

    void on_lineEditCod_editingFinished();

private:
    Ui::Ususarios *ui;

    baseDatos base;
    QSqlQueryModel *modeloTabla;
    QDataWidgetMapper mapper;
    QSqlQuery consulta;

    void refrescarBotones(int i);
    QStringList recogerDatosFormulario();
    void recargarTabla();
    void ajustarMapper();
    bool eventFilter(QObject *obj, QEvent *event);
    void borrarFormulario();
};

#endif // TABWIDGET_H
