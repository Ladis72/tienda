#include "dialogdetallearqueo.h"
#include "ui_dialogdetallearqueo.h"
#include "configuracion.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

DialogDetalleArqueo::DialogDetalleArqueo(int idArqueo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogDetalleArqueo),
    m_idArqueo(idArqueo)
{
    ui->setupUi(this);
    
    modeloDetalle = new QSqlQueryModel(this);
    
    cargarDetalle();
}

DialogDetalleArqueo::~DialogDetalleArqueo()
{
    delete ui;
}

void DialogDetalleArqueo::cargarDetalle()
{
    // Acceder a la configuración global de la conexión
    extern Configuracion *conf;
    
    // Consulta para obtener las monedas, billetes, cantidades y el cálculo del total.
    // Se ordena descendentemente por denominación (billetes más grandes primero).
    QString sql = QString(
        "SELECT "
        "  denominacion AS 'Moneda/Billete', "
        "  cantidad AS 'Cantidad', "
        "  ROUND(denominacion * cantidad, 2) AS 'Total (€)' "
        "FROM arqueos_detalle "
        "WHERE idArqueo = %1 "
        "ORDER BY denominacion DESC"
    ).arg(m_idArqueo);
    
    modeloDetalle->setQuery(sql, QSqlDatabase::database(conf->getConexionLocal()));
    
    if (modeloDetalle->lastError().isValid()) {
        qDebug() << "Error cargando desglose de arqueo:" << modeloDetalle->lastError().text();
    }
    
    ui->tableViewDetalle->setModel(modeloDetalle);
    ui->tableViewDetalle->resizeColumnsToContents();
}
