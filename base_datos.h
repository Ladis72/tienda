#ifndef BASE_DATOS_H
#define BASE_DATOS_H

#include <QDataWidgetMapper>
#include <QMap>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlTableModel>
#include "configuracion.h"

#include <QDebug>

extern Configuracion *conf;

class baseDatos
{
public:
    baseDatos();
    bool base_datos_abierta();
    static bool conectar(
        QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    bool guardarDatosConexion(
        QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    bool guardarDatosConexionMaster(
        QString host, QString puerto, QString baseDatos, QString usuario, QString clave);
    QStringList datosConexionMaster();
    QStringList datosConexionLocal();
    QStringList datosConexion();
    QSqlQuery usuarios(QSqlDatabase db);
    QSqlQuery fpago(QSqlDatabase db);
    QSqlDatabase conexion();
    QStringList datosTiendaLocal(QString db);

    //FUNCIONES ARTÍCULOS
    QSqlQuery consulta_producto(QString nombreConnexion, QString cod);
    QSqlQuery buscarProducto(QSqlDatabase db, QString tabla, QString nombre);
    bool insertarUsuario(QSqlDatabase db, QStringList datos);
    bool modificarUsuaruio(QSqlDatabase db, QStringList datos, QString dato);
    bool modificarFotoUsusario(QString foto, int id);
    bool modificarArticulo(QSqlDatabase db, QStringList datos, QString dato);
    bool insertarArticulo(QSqlDatabase db, QStringList datos);
    bool descontarArticulo(QString db, QString cod, int uds);
    bool actualizarFechaVentaArticulo(QString db, QString cod, QString fecha);
    bool actualizarArticulosDesdeCompras(QString base,QStringList datos);
    bool modificarFotoArticulo(QString foto, QString dato);

    //FUNCIONES CLIENTES

    QSqlQuery ventasClientes(QString nombreConexion, QDate fechaI, QDate fechaF);
    double valeCliente(QString nombreConexion, QString idCLiente);
    bool caducarVales(QString nombreConexion);
    int idVale(QString nombreConexion, QString idCliente);
    bool usarVale(QString nombreConexion, int idVale);
    // valesPendientesMarcar, hayValesPendientesMarcar, valesPendientes y borrarValePendiente
    // han sido eliminados: la propagaci\u00f3n del estado del vale se hace v\u00eda nube (SyncManager)
    QSqlQuery tickesPorCLiente(QString nombreConexion,
                               QString fechaI,
                               QString fechaF,
                               QString idCliente);
    QSqlQuery productosPorClienteCantidad(QString nombreConexion, QString idCliente, QDate fechaI, QDate fechaF);
    QSqlQuery productosPorClienteFecha(QString nombreConexion, QString idCliente, QDate fechaI, QDate fechaF);

    //FUNCIONES USUARIOS

    bool borrarUsusario(QSqlDatabase db, int dato);
    bool borrarArticulo(QSqlDatabase db, QString dato);
    QString nombreFamilia(QString id);
    QString nombreFabricante(QString id);
    QString nombreUsusario(QString id, QString base);
    QString nombreCliente(QString id);
    QString etiquetaCliente(QString idCliente);
    QString nombreFormaPago(QString id, QString base);
    QString idFormaPago(QString fpago, QString base);
    bool insertarEtiqueta(QString base, QString etiqueta);
    bool modificarTienda(QStringList datos);
    bool borrarTienda(QString dato);
    bool crearTienda(QStringList datos);
    QSqlQuery tiendas(QSqlDatabase db);
    int idTiendaDesdeNombre(QSqlDatabase db, QString nombreTienda);
    //Funciones PROVEEDORES
    QString nombreProveedor(QString id, QString base);
    QString idProveedor(QString nombre, QString base);
    bool modificarProveedor(QSqlDatabase db, QStringList datos, QString dato);
    bool borrarProveedor(QSqlDatabase db, QString dato);
    QString descuentoProveedor(QString proveedor);
    QStringList listadoProveedores(QString db);
    QString codigoParaNuevoProveedor();
    QString codigoDesdeAux(QString base, QString aux);
    //Funciones TICKETS
    QSqlQuery datosTicket(QString base, QString nTicket);
    QSqlQuery consultarLineasTicket(QString base, QString nTicket);
    double obtenerNumeroUltimoTicket(QSqlDatabase db);
    QSqlQuery tcketsPendientes(QSqlDatabase db);
    int maxTicketPendiente(QSqlDatabase db);
    bool nuevoTicketTmp(int orden, int cliente, int vendedor);
    bool grabarTicket(QString base, QString serie, QStringList datos);
    bool grabarLineaTicket(QStringList datos);
    bool borrarTicketTmp(int ticket);
    bool modificarCliente(QSqlDatabase db, QStringList datos, QString dato);
    bool borrarCliente(QSqlDatabase db, int idCliente);
    bool crearCliente(QSqlDatabase db, QStringList datos);
    double descuentoCliente(QString idCliente);
    bool crearProveedor(QSqlDatabase db, QStringList datos);
    double sumarColumna(QString base,
                        QString tabla,
                        QString campo,
                        QString campoCondicion = NULL,
                        QString condicion = "%%");
    int contarLineas(QString tabla,
                     QString base,
                     QString campoCondicion = NULL,
                     QString condicion = "%%");
    bool insertarES(QStringList datos, QString base, QString usuario);
    QStringList recuperarConfigTicket(QString base);
    bool ticketPromo(QString base);
    bool grabarConfiguracionTicket(QStringList configTicket);
    //Funciones PEDIDOS
    QSqlQuery recuperarPedidos(QString base);
    bool crearPedido(QString proveedor, QString nPedido, QString fecha, QString base);
    bool borrarPedido(QString base, QString numeroPedido);
    bool grabarLineaPedido(QString base, QStringList datos);
    float sumarIvasPedido(QString base, QString idPedido, QString tipoIva);
    float sumarRePedido(QString base, QString idPedido, QString tipoIva);
    float sumarBasesPedido(QString base, QString idPedido, QString tipoIva);
    bool borrarLineaPedido(QString base, QString idLinea);
    bool contabilizarPedido(QString base, QStringList datos);
    bool grabarFactura(QString base, QStringList datos);
    bool grabarAlbaran(QString base, QStringList datos);
    bool borrarAlbaranTmp(QString idAlbaran);
    bool pasarLineaPedidoAHistorico(QString base, QStringList datos);
    bool modificarLineaPedido(QString base, QStringList datos);
    QStringList listadoPrestamistas();
    QStringList datosFactura(QSqlDatabase db, QString nFactura);

    //Funciones CAJAS
    QSqlQuery ventas(QString fecha, QString base);
    QSqlQuery ventasPorUsusario(QString fecha, QString base);
    QSqlQuery ventasDesdeUltimoArqueo(QString fechaI, QString horaI, QString tabla, QString base);
    QSqlQuery recuperarDatosUltimoArqueo(QString base);
    bool grabarArqueo(QStringList datos, QString base,
                      const QMap<double, int> &desglose = QMap<double, int>());
    QSqlQuery ventasEntreFechas(QString fechaI, QString FechaF, QString tabla, QString base);
    int nTarjetasDesdeUltimoArqueo(QString fechaI, QString horaI, QString base);
    //Funciones GENERALISTAS
    QSqlQuery devolverTablaCompleta(QString base, QString nombreTabla);
    QSqlQuery ejecutarSentencia(QString sentencia, QString base);
    QSqlQuery ejecutarSentencia(QString sentencia, QSqlDatabase db);
    double ESdesdeFecha(QString fecha, QString hora, QString base);
    bool existeDatoEnTabla(QSqlDatabase db, QString tabla, QString columna, QString dato);
    void insertarEnTabla(QSqlDatabase db, QString tabla, QStringList datos);
    void vaciarTabla(QString tabla, QSqlDatabase db);
    QSqlQuery buscarEnTabla(QSqlDatabase db, QString tabla, QString campo, QString dato);

    //Funciones LOTES
    QString idLote(QString base,
                   QString cod,
                   QString lote,
                   QString fecha); //Devuelve el id del lote igual o 0 si no existe
    void aumentarLote(QString base, QString idLote, int uds);
    void disminuirLote(QString cod, QString fecha, int uds);
    void crearLote(QString base, QString ean, QString lote, QString fecha, QString uds);
    QSqlQuery lotesProducto(QString cod, QString nombreConnexion);
    QString sumarStockArticulo(QString id, QString nombreConnexion);
    int unidadesLote(QString base, QString idLote);
    bool borrarLotesArticulo(QString nombreConexion, QString codigo);

    //Funciones LISTADOS
    QString ticketCercanoFecha(QString tabla, QString fecha, QString cuando);
    QSqlQuery estadisticasVentaProductos(QString nPrimerTicket,
                                         QString nUltimoTicket,
                                         QString nPrimerTicketB,
                                         QString nUltimoTicketB);
    QSqlQuery listadoVentaArticulos(QString inicio, QString final, QString nombreDB);
    QSqlQuery listadoMovimientosEfectivo(QString db, QString inicio, QString final);
    QSqlQuery listadoCaducados(QString base, QString desde, QString hasta);

    // Funciones ESTADÍSTICAS.
    // El parámetro 'consolidado' indica si incluir las tablas ticketss y lineasticketss
    // además de tickets y lineasticket (se activa con F2 en el panel de estadísticas).
    QSqlQuery estadisticasVentasPorPeriodo(const QString &db, const QDate &desde, const QDate &hasta, const QString &agrupacion, bool consolidado = false);
    QSqlQuery estadisticasVentasPorUsuario(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    QSqlQuery estadisticasVentasPorFormaPago(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    QSqlQuery estadisticasVentasPorFamilia(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    QSqlQuery estadisticasTopArticulosVendidos(const QString &db, const QDate &desde, const QDate &hasta, int limite, bool consolidado = false);
    QSqlQuery estadisticasTopArticulosRentables(const QString &db, const QDate &desde, const QDate &hasta, int limite, bool consolidado = false);
    QSqlQuery estadisticasArticulosSinMovimiento(const QString &db, int diasSinVenta);
    QSqlQuery estadisticasMejoresClientes(const QString &db, const QDate &desde, const QDate &hasta, int limite, bool consolidado = false);
    double estadisticasTotalVentas(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    int estadisticasNumeroTickets(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    int estadisticasTotalArticulosStock(const QString &db);
    int estadisticasClientesActivos(const QString &db, const QDate &desde, const QDate &hasta, bool consolidado = false);
    double estadisticasTotalCompras(const QString &db, const QDate &desde, const QDate &hasta);
    int estadisticasNumeroPedidos(const QString &db, const QDate &desde, const QDate &hasta);

    //Funciones de configuración
    QString leerConfiguracion();
    bool GuardarConfiguracion(int datos);
    bool guardarDirectorios(QString base, QMap<QString, QString> directorios);
    QMap<QString, QString> cargarDirectorios(QString base);
    QString devolverDirectorio(QString tipo);
    QString nombreConexionMaster();
    QString nombreConexionLocal();
    bool copiaSeguridad(QString base, QString nombre);

    //Funciones Log
    bool insertarLog(const QString db,
                     const QString &categoria,
                     const QString &usuario,
                     const QString &mensaje);
    QSqlQueryModel *consultarLog(const QString db,
                                 const QString &categoria,
                                 const QString &usuario,
                                 const QDateTime &desde,
                                 const QDateTime &hasta);

    //Funciones VERIFACTU
    QString obtenerUltimoHash(const QString db);
    QString registrarTickeckVerifactu(const QString db,
                                      const int ticket,
                                      const QString fecha,
                                      const QString hora,
                                      const QString hashActual,
                                      const QString hashAnterior,
                                      const QString datosFactura,
                                      const int ususario);

    //Funciones NOTAS
    bool crearNota(const QString &db, const QString &titulo, const QString &descripcion,
                   const QString &usuario, const QString &fechaLimite, const QString &prioridad);
    bool actualizarEstadoNota(const QString &db, int idNota, const QString &estado);
    bool actualizarPrioridadNota(const QString &db, int idNota, const QString &prioridad);
    bool editarNota(const QString &db, int idNota, const QString &titulo,
                    const QString &descripcion, const QString &fechaLimite, const QString &prioridad);
    bool eliminarNota(const QString &db, int idNota);
    QSqlQueryModel *consultarNotas(const QString &db, const QString &filtroEstado = "");
    int contarNotasPendientes(const QString &db);
    QString getLastError() const { return m_lastError; }

private:
    QString m_lastError;
    QSqlDatabase db;
    QSqlQuery consulta;
    QSqlQueryModel modelo;
    QString cod;
};

#endif // BASE_DATOS_H
