#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQueryModel>

#include <QDebug>

class baseDatos
{
public:
    baseDatos();
    bool base_datos_abierta();
    QSqlDatabase conectar();
    QSqlQuery usuarios(QSqlDatabase db);
    QSqlQuery fpago(QSqlDatabase db);
    QSqlDatabase conexion();
    QSqlQuery consulta_producto(QSqlDatabase db, QString cod);
    QSqlQuery buscarProducto (QSqlDatabase db, QString tabla, QString nombre);
    bool insertarUsuario(QSqlDatabase db, QStringList datos);
    QSqlQuery buscarEnTabla(QSqlDatabase db, QString tabla, QString campo, QString dato);
    bool modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato);
    bool modificarFotoUsusario(QString foto, int id);
    bool modificarArticulo(QSqlDatabase db, QStringList datos, QString dato);
    bool insertarArticulo(QSqlDatabase db, QStringList datos);
    bool descontarArticulo(QString cod, int uds);
    bool actualizarFechaVentaArticulo(QString cod, QString fecha);
    bool actualizarArticulosDesdeCompras(QStringList datos);
    bool borrarUsusario(QSqlDatabase db, int dato);
    bool borrarArticulo(QSqlDatabase db, QString dato);
    bool modificarFotoArticulo(QString foto, QString dato);
    QString nombreFamilia(QString id);
    QString nombreFabricante(QString id);
    QString nombreUsusario(QString id);
    QString nombreCliente(QString id);
    QString nombreFormaPago(QString id);
    QString idFormaPago(QString fpago);
//Funciones PROVEEDORES
    QString nombreProveedor(QString id);
    QString idProveedor(QString nombre);
    bool modificarProveedor(QSqlDatabase db, QStringList datos, QString dato);
    QString descuentoProveedor(QString proveedor);
    QStringList listadoProveedores();
    QString codigoParaNuevoProveedor();

    double obtenerNumeroUltimoTicket(QSqlDatabase db);
    QSqlQuery tcketsPendientes(QSqlDatabase db);
    int maxTicketPendiente(QSqlDatabase db);
    bool nuevoTicketTmp (int orden, int cliente, int vendedor);
    bool grabarTicket(QString serie, QStringList datos);
    bool grabarLineaTicket(QStringList datos);
    bool borrarTicketTmp(int ticket);
    bool modificarCliente(QSqlDatabase db, QStringList datos, QString dato);
    bool borrarCliente(QSqlDatabase db, int idCliente);
    bool crearCliente(QSqlDatabase db, QStringList datos);
    double descuentoCliente(QString idCliente);
    bool existeDatoEnTabla(QSqlDatabase db, QString tabla, QString columna, QString dato);
    bool crearProveedor(QSqlDatabase db, QStringList datos);
    double sumarColumna(QString tabla, QString campo, QString campoCondicion = NULL, QString condicion ="%%");
    int contarLineas(QString tabla, QString campoCondicion = NULL, QString condicion ="%%");
    bool insertarES(QStringList datos);
//Funciones PEDIDOS
    QSqlQuery recuperarPedidos();
    bool crearPedido(QString proveedor, QString nPedido, QString fecha);
    bool borrarPedido(QString numeroPedido);
    bool grabarLineaPedido(QStringList datos);
    float sumarIvasPedido(QString idPedido, QString tipoIva);
    float sumarRePedido(QString idPedido, QString tipoIva);
    float sumarBasesPedido(QString idPedido, QString tipoIva);
    bool borrarLineaPedido(QString idLinea);
    bool contabilizarPedido(QStringList datos);
    bool grabarFactura(QStringList datos);
    bool borrarAlbaranTmp(QString idAlbaran);
    bool pasarLineaPedidoAHistorico(QStringList datos);
    bool modificarLineaPedido(QStringList datos);

//Funciones CAJAS
    QSqlQuery ventas(QString fecha);
    QSqlQuery ventasPorUsusario(QString fecha);
    QSqlQuery ventasDesdeUltimoArqueo(QString fechaI, QString horaI, QString tabla);
    QSqlQuery recuperarDatosUltimoArqueo();
    bool grabarArqueo(QStringList datos);
    QSqlQuery ventasEntreFechas(QString fechaI , QString FechaF);
//Funciones GENERALISTAS
    QSqlQuery devolverTablaCompleta(QString nombreTabla);
    QSqlQuery ejecutarSentencia(QString sentencia);
    double ESdesdeFecha(QString fecha, QString hora);

//Funciones LOTES
    QString idLote(QString cod, QString lote, QString fecha);//Devuelve el id del lote igual o 0 si no existe
    void aumentarLote(QString idLote, int uds);
    void crearLote(QString ean,QString lote,QString fecha,QString uds);

//Funciones LISTADOS

private:

    QSqlDatabase db;
    QSqlQuery consulta;
    QSqlQueryModel modelo;
    QString cod;
};

#endif // BASE_DATOS_H
