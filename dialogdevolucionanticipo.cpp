#include "dialogdevolucionanticipo.h"
#include "ui_dialogdevolucionanticipo.h"
#include "base_datos.h"
#include "configuracion.h"
#include <QSqlQuery>
#include <QSqlDatabase>

extern Configuracion *conf;

DialogDevolucionAnticipo::DialogDevolucionAnticipo(double anticipo, QString formaPagoOriginal, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDevolucionAnticipo)
{
    ui->setupUi(this);

    // Mostrar el importe del anticipo con formato de 2 decimales y símbolo €
    ui->labelValorAnticipo->setText(QString("%1 €").arg(QString::number(anticipo, 'f', 2)));

    // Cargar las formas de pago disponibles desde la base de datos
    baseDatos base;
    QSqlQuery qFp = base.fpago(QSqlDatabase::database(conf->getConexionLocal()));
    int indexOriginal = -1;
    int count = 0;

    while (qFp.next()) {
        QString nombreForma = qFp.value("tipo").toString();
        int idForma = qFp.value("id").toInt();
        ui->comboBoxFormaPago->addItem(nombreForma, idForma);

        if (!formaPagoOriginal.isEmpty() && nombreForma.contains(formaPagoOriginal, Qt::CaseInsensitive)) {
            indexOriginal = count;
        }
        count++;
    }

    // Fallback por defecto si no existen en la BD
    if (ui->comboBoxFormaPago->count() == 0) {
        ui->comboBoxFormaPago->addItem("Efectivo", 1);
        ui->comboBoxFormaPago->addItem("Tarjeta", 2);
        ui->comboBoxFormaPago->addItem("Bizum", 3);
        ui->comboBoxFormaPago->addItem("Transferencia", 4);
    }

    // Preseleccionar la forma de pago original con la que se cobró el anticipo si está en la lista
    if (indexOriginal >= 0) {
        ui->comboBoxFormaPago->setCurrentIndex(indexOriginal);
    }
}

DialogDevolucionAnticipo::~DialogDevolucionAnticipo()
{
    delete ui;
}

// Devuelve si el usuario activó la casilla para registrar la devolución
bool DialogDevolucionAnticipo::getDevolverAnticipo() const
{
    return ui->checkBoxDevolver->isChecked();
}

// Devuelve la forma de pago elegida para devolver el anticipo
QString DialogDevolucionAnticipo::getFormaPago() const
{
    return ui->comboBoxFormaPago->currentText();
}

// Devuelve el ID de la forma de pago elegida
int DialogDevolucionAnticipo::getFormaPagoId() const
{
    return ui->comboBoxFormaPago->currentData().toInt();
}
