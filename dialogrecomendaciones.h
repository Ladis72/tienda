#ifndef DIALOGRECOMENDACIONES_H
#define DIALOGRECOMENDACIONES_H

#include <QDialog>
#include <QStandardItemModel>
#include "monitorcaducidades.h"

namespace Ui {
class DialogRecomendaciones;
}

class DialogRecomendaciones : public QDialog
{
    Q_OBJECT

public:
    explicit DialogRecomendaciones(QWidget *parent = nullptr);
    ~DialogRecomendaciones();

    void cargarRecomendaciones(const QList<RecomendacionCaducidad> &recomendaciones);

private:
    Ui::DialogRecomendaciones *ui;
    QStandardItemModel *m_modelo;
};

#endif // DIALOGRECOMENDACIONES_H
