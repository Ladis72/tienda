#ifndef DIALOGCOMPARARARTICULOS_H
#define DIALOGCOMPARARARTICULOS_H

#include <QDialog>
#include <QSqlRecord>

namespace Ui {
class DialogCompararArticulos;
}

class DialogCompararArticulos : public QDialog
{
    Q_OBJECT

public:
    enum Resultado {
        Cancelar,
        UsarNube,
        SobrescribirNube
    };

    explicit DialogCompararArticulos(const QSqlRecord &local, const QSqlRecord &nube, QWidget *parent = nullptr);
    ~DialogCompararArticulos();

    Resultado getResultado() const { return m_resultado; }

private slots:
    void on_btnSincronizarLocal_clicked();
    void on_btnSobrescribirNube_clicked();
    void on_btnCancelar_clicked();

private:
    Ui::DialogCompararArticulos *ui;
    Resultado m_resultado;
};

#endif // DIALOGCOMPARARARTICULOS_H
