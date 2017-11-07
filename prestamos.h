#ifndef PRESTAMOS_H
#define PRESTAMOS_H

#include <QDialog>

namespace Ui {
class Prestamos;
}

class Prestamos : public QDialog
{
    Q_OBJECT

public:
    explicit Prestamos(QWidget *parent = 0);
    ~Prestamos();

private:
    Ui::Prestamos *ui;
};

#endif // PRESTAMOS_H
