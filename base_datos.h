﻿#ifndef BASE_DATOS_H
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
    static bool conectar(QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    bool guardarDatosConexion(QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    bool guardarDatosConexionMaster(QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    QStringList datosConexionMaster();
    QStringList datosConexionLocal();
    QStringList datosConexion();
    QSqlQuery usuarios(QSqlDatabase db);
    QSqlQuery fpago(QSqlDatabase db);
    QSqlDatabase conexion();
    QStringList datosTiendaLocal(QString db);


//FUNCIONES ARTÍCULOS
    QSqlQuery consulta_producto(QString nombreConnexion, QString cod);
    QSqlQuery buscarProducto (QSqlDatabase db, QString tabla, QString nombre);
    bool insertarUsuario(QSqlDatabase db, QStringList datos);
    bool modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato);
    bool modificarFotoUsusario(QString foto, int id);
    bool modificarArticulo(QSqlDatabase db, QStringList datos, QString dato);
    bool insertarArticulo(QSqlDatabase db, QStringList datos);
    bool descontarArticulo(QString cod, int uds);
    bool actualizarFechaVentaArticulo(QString cod, QString fecha);
    bool actualizarArticulosDesdeCompras(QStringList datos);
    bool modificarFotoArticulo(QString foto, QString dato);

//FUNCIONES CLIENTES

    QSqlQuery ventasClientes(QString nombreConexion , QDate fechaI , QDate fechaF);
    double valeCliente(QString nombreConexion, QString idCLiente);
    bool caducarVales(QString nombreConexion);
    int idVale(QString nombreConexion , QString idCliente);
    bool usarVale(QString nombreConexion , int idVale);
    bool valesPendientesMarcar(QString nombreConexion , QString tienda , int idVale);
    bool hayValesPendientesMarcar(QString nombreConexion);
    QSqlQuery valesPendientes(QString nombreConexion);
    bool borrarValePendiente(QString nombreConexion, int vale);

//FUNCIONES USUARIOS

    bool borrarUsusario(QSqlDatabase db, int dato);
    bool borrarArticulo(QSqlDatabase db, QString dato);
    QString nombreFamilia(QString id);
    QString nombreFabricante(QString id);
    QString nombreUsusario(QString id);
    QString nombreCliente(QString id);
    QString etiquetaCliente(QString idCliente);
    QString nombreFormaPago(QString id);
    QString idFormaPago(QString fpago);
    bool insertarEtiqueta(QString etiqueta);
    bool modificarTienda(QStringList datos);
    bool borrarTienda(QString dato);
    bool crearTienda(QStringList datos);
    QSqlQuery tiendas(QSqlDatabase db);
    int idTiendaDesdeNombre(QSqlDatabase db , QString nombreTienda);
//Funciones PROVEEDORES
    QString nombreProveedor(QString id);
    QString idProveedor(QString nombre);
    bool modificarProveedor(QSqlDatabase db, QStringList datos, QString dato);
    bool borrarProveedor(QSqlDatabase db , QString dato);
    QString descuentoProveedor(QString proveedor);
    QStringList listadoProveedores();
    QString codigoParaNuevoProveedor();
    QString codigoDesdeAux(QString aux);
//Funciones TICKETS
    QSqlQuery datosTicket(QString nTicket);
    QSqlQuery consultarLineasTicket(QString nTicket);
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
    bool crearProveedor(QSqlDatabase db, QStringList datos);
    double sumarColumna(QString tabla, QString campo, QString campoCondicion = NULL, QString condicion ="%%");
    int contarLineas(QString tabla, QString campoCondicion = NULL, QString condicion ="%%");
    bool insertarES(QStringList datos);
    QStringList recuperarConfigTicket();
    bool ticketPromo();
    bool grabarConfiguracionTicket(QStringList configTicket);
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
    bool grabarAlbaran(QStringList datos);
    bool borrarAlbaranTmp(QString idAlbaran);
    bool pasarLineaPedidoAHistorico(QStringList datos);
    bool modificarLineaPedido(QStringList datos);
    QStringList listadoPrestamistas();
    QStringList datosFactura(QSqlDatabase db, QString nFactura);

//Funciones CAJAS
    QSqlQuery ventas(QString fecha);
    QSqlQuery ventasPorUsusario(QString fecha);
    QSqlQuery ventasDesdeUltimoArqueo(QString fechaI, QString horaI, QString tabla);
    QSqlQuery recuperarDatosUltimoArqueo();
    bool grabarArqueo(QStringList datos);
    QSqlQuery ventasEntreFechas(QString fechaI , QString FechaF, QString tabla);
    int nTarjetasDesdeUltimoArqueo(QString fechaI , QString horaI);
//Funciones GENERALISTAS
    QSqlQuery devolverTablaCompleta(QString nombreTabla);
    QSqlQuery ejecutarSentencia(QString sentencia);
    QSqlQuery ejecutarSentencia(QString sentencia , QSqlDatabase db);
    double ESdesdeFecha(QString fecha, QString hora);
    bool existeDatoEnTabla(QSqlDatabase db, QString tabla, QString columna, QString dato);
    void insertarEnTabla(QSqlDatabase db , QString tabla , QStringList datos);
    void vaciarTabla(QString tabla , QSqlDatabase db);
    QSqlQuery buscarEnTabla(QSqlDatabase db, QString tabla, QString campo, QString dato);


//Funciones LOTES
    QString idLote(QString cod, QString lote, QString fecha);//Devuelve el id del lote igual o 0 si no existe
    void aumentarLote(QString idLote, int uds);
    void disminuirLote(QString cod, QString fecha , int uds);
    void crearLote(QString ean,QString lote,QString fecha,QString uds);
    QSqlQuery lotesProducto(QString cod, QString nombreConnexion);
    QString sumarStockArticulo(QString id, QString nombreConnexion);
    int unidadesLote(QString idLote);
    bool borrarLotesArticulo(QString nombreConexion, QString codigo);

//Funciones LISTADOS
    QString ticketCercanoFecha(QString tabla, QString fecha, QString cuando);
    QSqlQuery estadisticasVentaProductos(QString nPrimerTicket , QString nUltimoTicket, QString nPrimerTicketB, QString nUltimoTicketB);
    QSqlQuery listadoVentaArticulos(QString inicio , QString final, QString nombreDB);
    QSqlQuery listadoMovimientosEfectivo(QString inicio , QString final);
    QSqlQuery listadoCaducados(QString desde , QString hasta);

//Funciones de configuración
    QString leerConfiguracion();
    bool GuardarConfiguracion(int datos);
    bool guardarDirectorios(QStringList directorios);
    QStringList cargarDirectorios();
    QString devolverDirectorio(QString tipo);
    QString nombreConexionMaster();
    QString nombreConexionLocal();

private:

    QSqlDatabase db;
    QSqlQuery consulta;
    QSqlQueryModel modelo;
    QString cod;
};

#endif // BASE_DATOS_H
