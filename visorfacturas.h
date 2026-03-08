#ifndef VISORFACTURAS_H
#define VISORFACTURAS_H

#include <QDialog>
#include <QScreen>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;
namespace Ui {
class VisorFacturas;
}

class VisorFacturas : public QDialog
{
    Q_OBJECT

public:
    explicit VisorFacturas(QString nFactura, QWidget *parent = 0);
    ~VisorFacturas();

private:
    Ui::VisorFacturas *ui;
    QSqlQuery consulta;
    QSqlQueryModel modelo;
    baseDatos *base;
    QScreen *monitor;
};

#endif // VISORFACTURAS_H
