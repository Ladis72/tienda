#ifndef CADUCADOS_H
#define CADUCADOS_H

#include <QDialog>

namespace Ui {
class Caducados;
}

class Caducados : public QDialog
{
    Q_OBJECT

public:
    explicit Caducados(QWidget *parent = 0);
    ~Caducados();

private:
    Ui::Caducados *ui;
};

#endif // CADUCADOS_H
