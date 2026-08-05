#include "historialprecios.h"
#include "ui_historialprecios.h"
#include "configuracion.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

HistorialPrecios::HistorialPrecios(QString codArticulo, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HistorialPrecios),
    m_codArticulo(codArticulo)
{
    ui->setupUi(this);
    
    // Utilizar la variable global conf para acceder a la conexión local
    extern Configuracion *conf;
    modeloHistorial = new QSqlQueryModel(this);
    
    cargarHistorial();
}

HistorialPrecios::~HistorialPrecios()
{
    delete ui;
}

void HistorialPrecios::cargarHistorial()
{
    extern Configuracion *conf;
    
    // Consulta para obtener el historial de precios del artículo ordenado por fecha descendente
    // Se usa DATE_FORMAT para asegurar el formato yyyy-MM-dd
    QSqlQuery q(QSqlDatabase::database(conf->getConexionLocal()));
    q.prepare(
        "SELECT "
        "  DATE_FORMAT(fecha_cambio, '%Y-%m-%d %H:%i:%s') AS Fecha, "
        "  tipo AS Tipo, "
        "  precio_viejo AS 'Precio Anterior', "
        "  precio_nuevo AS 'Precio Nuevo' "
        "FROM historico_precios "
        "WHERE cod_articulo = ? "
        "ORDER BY fecha_cambio DESC"
    );
    q.bindValue(0, m_codArticulo);
    if (!q.exec()) {
        qDebug() << "Error cargando historial de precios:" << q.lastError().text();
    }
    modeloHistorial->setQuery(q);
    
    ui->tableViewHistorial->setModel(modeloHistorial);
    ui->tableViewHistorial->resizeColumnsToContents();
}
