#ifndef FACTURARALBARANES_H
#define FACTURARALBARANES_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QStandardItemModel>
#include "base_datos.h"
#include "configuracion.h"

extern Configuracion *conf;

class QComboBox;
class QTableView;
class QLineEdit;
class QDateEdit;
class QPushButton;
class QLabel;

class FacturarAlbaranes : public QDialog
{
    Q_OBJECT

public:
    explicit FacturarAlbaranes(QWidget *parent = nullptr);
    ~FacturarAlbaranes();

private slots:
    void onProveedorChanged(int index);
    void calcTotals();
    void facturar();

private:
    void setupUi();
    void loadProveedores();
    void loadAlbaranes(QString Proveedor);

    QComboBox *comboProveedores;
    QTableView *tablaAlbaranes;
    QStandardItemModel *modeloAlbaranes;

    QLineEdit *leFactura;
    QDateEdit *dateFactura;
    QDateEdit *dateVencimiento;

    QLabel *lblTotalBase;
    QLabel *lblTotalIva;
    QLabel *lblTotalRe;
    QLabel *lblTotal;

    QPushButton *btnFacturar;
    QPushButton *btnCancelar;

    baseDatos base;

    QString proveedor;
};

#endif // FACTURARALBARANES_H
