#ifndef HISTORIALPRECIOS_H
#define HISTORIALPRECIOS_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class HistorialPrecios;
}

class HistorialPrecios : public QDialog
{
    Q_OBJECT

public:
    explicit HistorialPrecios(QString codArticulo, QWidget *parent = nullptr);
    ~HistorialPrecios();

private:
    Ui::HistorialPrecios *ui;
    QSqlQueryModel *modeloHistorial;
    QString m_codArticulo;

    void cargarHistorial();
};

#endif // HISTORIALPRECIOS_H
