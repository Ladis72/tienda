#ifndef DIALOGCAMBIOCODIGO_H
#define DIALOGCAMBIOCODIGO_H

#include <QDialog>
#include <QStringList>
#include "base_datos.h"
#include "configuracion.h"

namespace Ui {
class DialogCambioCodigo;
}

class DialogCambioCodigo : public QDialog
{
    Q_OBJECT

public:
    explicit DialogCambioCodigo(const QString& codActual, const QString& descripcion, const QStringList& tiendas, QWidget *parent = nullptr);
    ~DialogCambioCodigo();

    QString getNuevoCodigo() const;

private slots:
    void on_pushButtonBuscarNube_clicked();

private:
    Ui::DialogCambioCodigo *ui;
    baseDatos base;
    Configuracion *conf;
};

#endif // DIALOGCAMBIOCODIGO_H
