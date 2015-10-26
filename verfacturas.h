#ifndef VERFACTURAS_H
#define VERFACTURAS_H

#include <QDialog>

namespace Ui {
class VerFacturas;
}

class VerFacturas : public QDialog
{
    Q_OBJECT

public:
    explicit VerFacturas(QWidget *parent = 0);
    ~VerFacturas();

private:
    Ui::VerFacturas *ui;
};

#endif // VERFACTURAS_H
