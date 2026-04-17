#include "dialogcambiocodigo.h"
#include "ui_dialogcambiocodigo.h"
#include "buscarproducto.h"
#include <QMessageBox>

DialogCambioCodigo::DialogCambioCodigo(const QString& codActual, const QString& descripcion, const QStringList& tiendas, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogCambioCodigo)
{
    ui->setupUi(this);
    conf = new Configuracion();
    ui->labelInfo->setText("Código actual: " + codActual);
    ui->labelArticulo->setText(descripcion);

    // Llenar combo de fuentes
    ui->comboBoxFuente->addItem("NUBE");
    if (!tiendas.isEmpty()) {
        ui->comboBoxFuente->addItems(tiendas);
    }
}

DialogCambioCodigo::~DialogCambioCodigo()
{
    delete conf;
    delete ui;
}

QString DialogCambioCodigo::getNuevoCodigo() const
{
    return ui->lineEditNuevoCodigo->text().trimmed();
}

void DialogCambioCodigo::on_pushButtonBuscarNube_clicked()
{
    QString fuente = ui->comboBoxFuente->currentText();
    QString conName = fuente;

    if (fuente == "NUBE") {
        if (!QSqlDatabase::database("NUBE").isOpen()) {
            conName = conf->getConexionMaster();
        }
    }

    if (!QSqlDatabase::database(conName).isOpen()) {
         QMessageBox::warning(this, "Aviso", QString("No se pudo conectar con: %1").arg(fuente));
         return;
    }

    // Usar la búsqueda de producto en la fuente seleccionada
    QSqlQuery consulta = base.buscarProducto(QSqlDatabase::database(conName), "articulos", ""); 
    BuscarProducto *buscar = new BuscarProducto(this, consulta);
    if (buscar->exec() == QDialog::Accepted) {
        ui->lineEditNuevoCodigo->setText(buscar->resultado);
    }
    delete buscar;
}
