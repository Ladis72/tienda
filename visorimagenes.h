#ifndef VISORIMAGENES_H
#define VISORIMAGENES_H

#include <QDialog>

namespace Ui {
class VisorImagenes;
}

class VisorImagenes : public QDialog
{
    Q_OBJECT

public:
    explicit VisorImagenes(QString direccion, QWidget *parent = 0);
    ~VisorImagenes();

private:
    Ui::VisorImagenes *ui;
};

#endif // VISORIMAGENES_H
