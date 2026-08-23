#include "asistenteia.h"
#include "configuracion.h"
#include "base_datos.h"
#include "syncmanager.h"
#include <QCoreApplication>
#include <QDate>
#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkRequest>
#include <QSettings>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QUrl>

extern Configuracion *conf;

/**
 * @brief Constructor de AsistenteIA.
 * Carga los parámetros configurados en tienda.ini e inicializa el gestor de red.
 */
AsistenteIA::AsistenteIA(QObject *parent)
    : QObject(parent),
      m_netManager(new QNetworkAccessManager(this)),
      m_procesando(false),
      m_profundidadToolCalls(0)
{
    // Cargar configuración de Ollama desde tienda.ini
    QString iniPath = QCoreApplication::applicationDirPath() + "/tienda.ini";
    QSettings settings(iniPath, QSettings::IniFormat);
    settings.beginGroup("Ollama");
    m_baseUrl = settings.value("url", "http://localhost:11434").toString().trimmed();
    if (m_baseUrl.isEmpty()) {
        m_baseUrl = "http://localhost:11434";
    }
    m_modelo = settings.value("modelo", "llama3.1:8b").toString().trimmed();
    if (m_modelo.isEmpty()) {
        m_modelo = "llama3.1:8b";
    }
    settings.endGroup();

    // Inicializar historial con mensaje de sistema contextual
    limpiarHistorial();
}

/**
 * @brief Destructor de AsistenteIA.
 */
AsistenteIA::~AsistenteIA()
{
}

void AsistenteIA::setBaseUrl(const QString &url)
{
    m_baseUrl = url.trimmed();
    if (m_baseUrl.endsWith("/")) {
        m_baseUrl.chop(1);
    }
}

QString AsistenteIA::baseUrl() const
{
    return m_baseUrl;
}

/**
 * @brief Descarga un modelo específico de la memoria VRAM/RAM de Ollama enviando keep_alive = 0.
 * @param nombreModelo Nombre del modelo a descargar (ej. "llama3.1:8b").
 */
void AsistenteIA::descargarModelo(const QString &nombreModelo)
{
    QString mod = nombreModelo.trimmed();
    if (mod.isEmpty()) return;

    QString endpoint = m_baseUrl + "/api/generate";
    QNetworkRequest req((QUrl(endpoint)));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QJsonObject obj;
    obj["model"] = mod;
    obj["keep_alive"] = 0;

    QByteArray body = QJsonDocument(obj).toJson(QJsonDocument::Compact);
    QNetworkReply *reply = m_netManager->post(req, body);
    connect(reply, &QNetworkReply::finished, reply, &QNetworkReply::deleteLater);
    qDebug() << "AsistenteIA: Solicitada descarga de VRAM del modelo:" << mod;
}

/**
 * @brief Consulta /api/ps y descarga todos los modelos que estén actualmente ocupando memoria en Ollama.
 */
void AsistenteIA::descargarModelosEnMemoria()
{
    QString endpoint = m_baseUrl + "/api/ps";
    QNetworkRequest req((QUrl(endpoint)));
    QNetworkReply *reply = m_netManager->get(req);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        reply->deleteLater();
        if (reply->error() != QNetworkReply::NoError) return;
        QJsonDocument doc = QJsonDocument::fromJson(reply->readAll());
        if (!doc.isObject()) return;
        QJsonArray models = doc.object().value("models").toArray();
        for (const QJsonValue &v : models) {
            QString name = v.toObject().value("name").toString();
            if (!name.isEmpty()) {
                descargarModelo(name);
            }
        }
    });
}

/**
 * @brief Establece el nuevo modelo de IA.
 * Si el modelo anterior es diferente, solicita su descarga de la memoria VRAM antes de cambiar.
 */
void AsistenteIA::setModelo(const QString &modelo)
{
    QString nuevo = modelo.trimmed();
    if (!m_modelo.isEmpty() && m_modelo != nuevo) {
        descargarModelo(m_modelo);
    }
    m_modelo = nuevo;
}

QString AsistenteIA::modelo() const
{
    return m_modelo;
}

/**
 * @brief Obtiene la conexión activa y abierta a la base de datos local.
 */
QSqlDatabase AsistenteIA::obtenerBaseDatos()
{
    QString conn = conf ? conf->getConexionLocal() : "DB";
    if (conn.isEmpty() || !QSqlDatabase::contains(conn)) {
        conn = "DB";
    }
    if (QSqlDatabase::contains(conn) && QSqlDatabase::database(conn).isOpen()) {
        return QSqlDatabase::database(conn);
    }
    return QSqlDatabase::database();
}

/**
 * @brief Reinicia la conversación y añade el mensaje de sistema inicial.
 */
void AsistenteIA::limpiarHistorial()
{
    m_historial = QJsonArray();
    m_historial.append(construirMensajeSistema());
}

/**
 * @brief Construye el prompt de sistema con la información contextual del TPV y de la red de tiendas.
 */
QJsonObject AsistenteIA::construirMensajeSistema()
{
    QString fechaActual = QDate::currentDate().toString("yyyy-MM-dd");
    QDate hoy = QDate::currentDate();
    QString inicioMesActual = QDate(hoy.year(), hoy.month(), 1).toString("yyyy-MM-dd");
    QString finMesActual = QDate(hoy.year(), hoy.month(), hoy.daysInMonth()).toString("yyyy-MM-dd");
    QString horaActual = QTime::currentTime().toString("hh:mm:ss");

    QString usuarioActual = conf ? conf->getUsuario() : "Ladis";
    int rolActual = conf ? conf->getRol() : 0;
    QStringList tiendasConectadas = conf ? conf->getNombreConexionesActivas() : QStringList();
    QString strTiendas = tiendasConectadas.isEmpty() ? "Solo tienda local" : tiendasConectadas.join(", ");

    QString prompt = QString(
        "Eres el asistente inteligente integrado en el software TPV y de gestión de la tienda.\n"
        "Contexto del sistema:\n"
        "- Fecha actual: %1 (formato estricto yyyy-MM-dd). Año en curso: %8.\n"
        "- Mes actual: del %2 al %3. Hora actual: %4.\n"
        "- Usuario: %5 (Rol: %6). Red de Tiendas: %7.\n"
        "- Tiendas físicas de la empresa: Únicamente 'Emeicjac', 'Casablanca', 'Cervantes' y 'Sucursal 1'. Marcas como Nova Diet, Santiveri, Sotya, Plameca, Drasanvi, etc. son FABRICANTES, no tiendas.\n\n"
        "REGLA DE ORO DE EJECUCIÓN (TOOL-CALLING OBLIGATORIO):\n"
        "Tienes ESTRICTAMENTE PROHIBIDO responder de memoria o inventar códigos, precios, productos o cifras de ventas. "
        "Ante CUALQUIER consulta del usuario sobre productos, dolencias, stock, ventas, clientes o compras, DEBES ejecutar INMEDIATAMENTE la herramienta correspondiente en cada turno de conversación.\n\n"
        "MAPEO DE HERRAMIENTAS:\n"
        "1. Stock y Precios -> 'consultar_stock' (ej. término: 'Colestia', 'Lecidol').\n"
        "2. Fitoterapia, Dolencias, Síntomas e Ingredientes -> 'buscar_por_indicacion' (ej. indicacion: 'colesterol', 'monacolina', 'garcinia', 'articulaciones', 'dormir'). Presenta SIEMPRE los resultados en formato TABLA Markdown (| Código | Producto | Marca | Stock | PVP |) mostrando tanto artículos con stock como referencias con stock 0.\n"
        "3. Ventas y Facturación -> 'resumen_ventas' (fecha_inicio, fecha_fin, tienda).\n"
        "4. Ranking de Productos Más Vendidos y Ventas por Marca/Fabricante -> 'productos_mas_vendidos' (fabricante: 'Kenoi' o 'Nova Diet', ano: %8, fecha_inicio, fecha_fin, limite, tienda, familia).\n"
        "   - Si preguntan por ventas de una marca/fabricante (ej. 'Kenoi', 'Nova Diet'), DEBES pasar fabricante: 'Kenoi'.\n"
        "   - Si se pide 'no alimentación', pasa familia: 'no alimentacion'.\n"
        "   - Si se pide 'mes a mes', pasa desglosar_por_mes: true.\n"
        "5. Comparativas Temporales (vs año pasado) -> 'comparativa_ventas' (calcula automáticamente periodo homólogo exacto).\n"
        "6. Clientes, Compradores de Marca/Producto y Rankings -> 'ultimas_compras_cliente' (para compras de un cliente: cliente: 'Sonsoles', producto: 'Lecidol'; para saber qué clientes compraron una marca/fabricante: fabricante: 'Kenoi'; para ranking de mayores clientes: cliente: 'top', limite: 5).\n"
        "7. Previsión, Cobertura y Venta Media Mensual de Productos -> 'prevision_cobertura_stock' (producto: 'nombre de producto, marca o fabricante', ej. 'Deneurome', 'Nova Diet', 'Colestia'). Calcula automáticamente la venta media mensual (unidades/mes) en cada tienda y global, meses de cobertura y previsión hasta fin de año.\n"
        "8. Caducidades y Lotes -> 'consultar_caducidades' (producto: 'nombre').\n"
        "9. Horas Pico o Mejor Día de la Semana -> 'facturacion_por_horas' (agrupar_por: 'dia_semana' para saber el mejor día de la semana o 'horas' para franja horaria pico, fecha_inicio, fecha_fin, tienda).\n"
        "10. Proveedores, Precios de Compra y Márgenes/Rentabilidad -> 'consultar_compras_proveedor' (producto: 'nombre', proveedor: 'nombre', margenes_bajos: true). Si te piden detectar productos con márgenes bajos o analizar rentabilidad, pasa margenes_bajos: true.\n"
        "11. Consultas SQL Especiales a Medida -> 'ejecutar_consulta_sql' (SELECT sobre 'vista_ventas_detalladas' [columnas: id_tienda, tienda, ticket, fecha, hora, id_cliente, cliente, telefono_cliente, codigo_articulo, producto, cantidad, pvp_unitario, total, fabricante, familia], 'vista_stock_tiendas', 'vista_compras_clientes').\n\n"
        "FORMATO DE RESPUESTA:\n"
        "- Responde siempre en español, de forma concisa, educada y profesional.\n"
        "- Fechas siempre en formato 'yyyy-MM-dd'.\n"
        "- Usa tablas Markdown ordenadas para catálogos, resúmenes y comparativas.\n"
    ).arg(fechaActual, inicioMesActual, finMesActual, horaActual, usuarioActual).arg(rolActual).arg(strTiendas).arg(hoy.year());

    QJsonObject obj;
    obj["role"] = "system";
    obj["content"] = prompt;
    return obj;
}

/**
 * @brief Define el catálogo de herramientas (Tool Calling) en formato JSON Schema.
 */
QJsonArray AsistenteIA::construirDefinicionHerramientas()
{
    QJsonArray tools;

    // 1. Tool: consultar_stock
    {
        QJsonObject func;
        func["name"] = "consultar_stock";
        func["description"] = "Consulta información de artículos (código, descripción, PVP, precio de compra, stock actual, mínimos y fabricante) en la tienda local, en otra tienda específica o en todas las tiendas.";
        
        QJsonObject props;
        QJsonObject propTermino;
        propTermino["type"] = "string";
        propTermino["description"] = "Texto a buscar en el nombre, descripción o código del producto";
        props["termino"] = propTermino;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar: 'local', 'todas', o el nombre específico (ej. 'Casablanca', 'Cervantes', 'Emeicjac'). Por defecto 'local'.";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;
        params["required"] = QJsonArray({"termino"});

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 2. Tool: articulos_bajo_minimo
    {
        QJsonObject func;
        func["name"] = "articulos_bajo_minimo";
        func["description"] = "Obtiene la lista de artículos cuyo stock actual es menor o igual al stock mínimo configurado.";
        
        QJsonObject props;
        QJsonObject propLimite;
        propLimite["type"] = "integer";
        propLimite["description"] = "Número máximo de artículos a devolver (por defecto 20)";
        props["limite"] = propLimite;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar ('local', 'todas' o nombre de la tienda)";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 3. Tool: resumen_ventas
    {
        QJsonObject func;
        func["name"] = "resumen_ventas";
        func["description"] = "Obtiene el resumen de ventas e importes totales entre dos fechas en formato 'yyyy-MM-dd' para la tienda actual, otra tienda o todas las tiendas.";
        
        QJsonObject props;
        QJsonObject propFechaI;
        propFechaI["type"] = "string";
        propFechaI["description"] = "Fecha de inicio en formato yyyy-MM-dd";
        props["fecha_inicio"] = propFechaI;

        QJsonObject propFechaF;
        propFechaF["type"] = "string";
        propFechaF["description"] = "Fecha de fin en formato yyyy-MM-dd (opcional, por defecto igual a la fecha de inicio)";
        props["fecha_fin"] = propFechaF;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar ('local', 'todas' o nombre de tienda específica como 'Casablanca', 'Cervantes', 'Emeicjac')";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;
        params["required"] = QJsonArray({"fecha_inicio"});

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 4. Tool: ultimos_arqueos
    {
        QJsonObject func;
        func["name"] = "ultimos_arqueos";
        func["description"] = "Consulta los últimos arqueos de caja realizados (totales, ventas efectivo, tarjeta y descuadres).";
        
        QJsonObject props;
        QJsonObject propLimite;
        propLimite["type"] = "integer";
        propLimite["description"] = "Número de arqueos recientes a devolver (por defecto 5)";
        props["limite"] = propLimite;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar ('local' o nombre de la tienda)";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 5. Tool: buscar_clientes
    {
        QJsonObject func;
        func["name"] = "buscar_clientes";
        func["description"] = "Busca clientes por código, nombre, teléfono o CIF/NIF.";
        
        QJsonObject props;
        QJsonObject propTermino;
        propTermino["type"] = "string";
        propTermino["description"] = "Nombre, teléfono, NIF o código del cliente";
        props["termino"] = propTermino;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;
        params["required"] = QJsonArray({"termino"});

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 6. Tool: ultimas_compras_cliente
    {
        QJsonObject func;
        func["name"] = "ultimas_compras_cliente";
        func["description"] = "Consulta el historial de compras de un cliente ('Sonsoles'), el ranking de clientes con mayor volumen de compras/gasto ('top', 'ranking'), o los compradores de una marca/producto ('Kenoi').";
        
        QJsonObject props;
        QJsonObject propCli;
        propCli["type"] = "string";
        propCli["description"] = "Nombre del cliente (ej. 'Sonsoles') O 'top' / 'ranking' para ranking de mayores compradores O marca/producto (ej. 'Kenoi')";
        props["cliente"] = propCli;

        QJsonObject propFab;
        propFab["type"] = "string";
        propFab["description"] = "Nombre del fabricante o marca para saber qué clientes compraron productos de esa marca (ej. 'Kenoi')";
        props["fabricante"] = propFab;

        QJsonObject propProd;
        propProd["type"] = "string";
        propProd["description"] = "Nombre o parte del producto para consultar cuándo y cuántas veces lo ha comprado el cliente (ej. 'Lecidol', 'Colamag')";
        props["producto"] = propProd;

        QJsonObject propFechaI;
        propFechaI["type"] = "string";
        propFechaI["description"] = "Fecha de inicio para ranking o compras (formato yyyy-MM-dd)";
        props["fecha_inicio"] = propFechaI;

        QJsonObject propFechaF;
        propFechaF["type"] = "string";
        propFechaF["description"] = "Fecha de fin para ranking o compras (formato yyyy-MM-dd)";
        props["fecha_fin"] = propFechaF;

        QJsonObject propAno;
        propAno["type"] = "integer";
        propAno["description"] = "Año concreto a consultar (ej. 2025 o 2026)";
        props["ano"] = propAno;

        QJsonObject propIdCliente;
        propIdCliente["type"] = "integer";
        propIdCliente["description"] = "ID numérico del cliente si se conoce";
        props["id_cliente"] = propIdCliente;

        QJsonObject propLim;
        propLim["type"] = "integer";
        propLim["description"] = "Número máximo de artículos o clientes a devolver (por defecto 10 o 5 en rankings)";
        props["limite"] = propLim;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 7. Tool: buscar_por_indicacion
    {
        QJsonObject func;
        func["name"] = "buscar_por_indicacion";
        func["description"] = "Busca productos recomendados o indicados para un síntoma, necesidad o dolencia específica.";
        
        QJsonObject props;
        QJsonObject propSintoma;
        propSintoma["type"] = "string";
        propSintoma["description"] = "Síntoma o indicación (ej. insomnio, articulaciones, memoria, digestión)";
        props["indicacion"] = propSintoma;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;
        params["required"] = QJsonArray({"indicacion"});

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 8. Tool: productos_mas_vendidos
    {
        QJsonObject func;
        func["name"] = "productos_mas_vendidos";
        func["description"] = "Obtiene el ranking de los productos más vendidos (top ventas) por unidades o importe, filtrando opcionalmente por fabricante/marca (ej. 'Nova Diet'), familia, rango de fechas y tienda.";
        
        QJsonObject props;
        QJsonObject propFechaI;
        propFechaI["type"] = "string";
        propFechaI["description"] = "Fecha de inicio en formato yyyy-MM-dd";
        props["fecha_inicio"] = propFechaI;

        QJsonObject propFechaF;
        propFechaF["type"] = "string";
        propFechaF["description"] = "Fecha de fin en formato yyyy-MM-dd";
        props["fecha_fin"] = propFechaF;

        QJsonObject propFab;
        propFab["type"] = "string";
        propFab["description"] = "Nombre o parte del fabricante o marca (ej. 'Kenoi', 'Nova Diet', 'Soria Natural')";
        props["fabricante"] = propFab;

        QJsonObject propAno;
        propAno["type"] = "integer";
        propAno["description"] = "Año a consultar (ej. 2026). Ajusta automáticamente fecha_inicio y fecha_fin a todo el año.";
        props["ano"] = propAno;

        QJsonObject propFam;
        propFam["type"] = "string";
        propFam["description"] = "Nombre o parte de la familia del producto (ej. 'Vitaminas y minerales', 'Plantas')";
        props["familia"] = propFam;

        QJsonObject propTerm;
        propTerm["type"] = "string";
        propTerm["description"] = "Texto de búsqueda en el nombre del artículo";
        props["termino"] = propTerm;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar: 'todas', 'local', o específica (ej. 'Casablanca', 'Cervantes', 'Emeicjac'). Por defecto 'todas'.";
        props["tienda"] = propTienda;

        QJsonObject propLim;
        propLim["type"] = "integer";
        propLim["description"] = "Número máximo de artículos a devolver (por defecto 10)";
        props["limite"] = propLim;

        QJsonObject propDesgMes;
        propDesgMes["type"] = "boolean";
        propDesgMes["description"] = "Si es true, devuelve el ranking de los más vendidos desglosado para cada mes dentro del periodo seleccionado (ej. de enero a diciembre).";
        props["desglosar_por_mes"] = propDesgMes;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 9. Tool: comparativa_ventas
    {
        QJsonObject func;
        func["name"] = "comparativa_ventas";
        func["description"] = "Compara las ventas y tickets de un periodo actual frente al periodo homólogo del año anterior o mes anterior (calculando automáticamente variaciones en euros y porcentajes matemáticos exactos por tienda y a nivel global).";
        
        QJsonObject props;
        QJsonObject propFechaI;
        propFechaI["type"] = "string";
        propFechaI["description"] = "Fecha de inicio del periodo actual en formato yyyy-MM-dd (por defecto inicio del mes actual)";
        props["fecha_inicio"] = propFechaI;

        QJsonObject propFechaF;
        propFechaF["type"] = "string";
        propFechaF["description"] = "Fecha de fin del periodo actual en formato yyyy-MM-dd (por defecto la fecha actual/hoy)";
        props["fecha_fin"] = propFechaF;

        QJsonObject propComp;
        propComp["type"] = "string";
        propComp["description"] = "Tipo de comparativa: 'ano_anterior' (mismo rango de días del año pasado, por defecto) o 'mes_anterior'.";
        props["comparar_con"] = propComp;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar: 'todas', 'local', o específica (ej. 'Casablanca', 'Cervantes', 'Emeicjac'). Por defecto 'todas'.";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 10. Tool: ejecutar_consulta_sql
    {
        QJsonObject func;
        func["name"] = "ejecutar_consulta_sql";
        func["description"] = "Ejecuta una consulta SQL SELECT (solo lectura) en la base de datos para análisis avanzados, estadísticas personalizadas, datos históricos cruzados, rankings o consultas no cubiertas por otras herramientas.";
        
        QJsonObject props;
        QJsonObject propQuery;
        propQuery["type"] = "string";
        propQuery["description"] = "Consulta SQL SELECT completa y válida a ejecutar (ej. SELECT ... FROM ... WHERE ... GROUP BY ... ORDER BY ... LIMIT ...)";
        props["query"] = propQuery;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;
        params["required"] = QJsonArray({"query"});

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 11. Tool: prevision_cobertura_stock
    {
        QJsonObject func;
        func["name"] = "prevision_cobertura_stock";
        func["description"] = "Calcula la venta media mensual (ritmo de unidades/mes vendidas en cada tienda y global), historial de rotación y cobertura de stock para un producto (ej. 'Deneurome') o fabricante (ej. 'Nova Diet'). Determina si el stock en tiendas es suficiente o qué productos están en riesgo de rotura.";
        
        QJsonObject props;
        QJsonObject propProd;
        propProd["type"] = "string";
        propProd["description"] = "Nombre del producto, marca o fabricante a analizar (ej. 'Deneurome', 'Nova Diet', 'Colestia forte', 'Santiveri')";
        props["producto"] = propProd;

        QJsonObject propFab;
        propFab["type"] = "string";
        propFab["description"] = "Fabricante o marca a analizar en bloque (ej. 'Nova Diet', 'Santiveri')";
        props["fabricante"] = propFab;

        QJsonObject propMeses;
        propMeses["type"] = "number";
        propMeses["description"] = "Número de meses a proyectar (opcional; por defecto calcula automáticamente hasta final de año)";
        props["meses_proyeccion"] = propMeses;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 12. Tool: consultar_caducidades
    {
        QJsonObject func;
        func["name"] = "consultar_caducidades";
        func["description"] = "Consulta la fecha de caducidad y número de lote de artículos o revisa qué productos van a caducar próximamente en las tiendas.";
        
        QJsonObject props;
        QJsonObject propProd;
        propProd["type"] = "string";
        propProd["description"] = "Nombre, marca o código del producto a consultar caducidad (ej. 'Pilogastril', 'Colestia', '8425652530088'). Si se omite, busca los productos que caducan antes.";
        props["producto"] = propProd;

        QJsonObject propDias;
        propDias["type"] = "number";
        propDias["description"] = "Días límite para filtrar próximas caducidades (opcional, por defecto 365 días)";
        props["dias_limite"] = propDias;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar: 'todas', 'local', o específica (ej. 'Casablanca', 'Cervantes', 'Emeicjac'). Por defecto 'todas'.";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 13. Tool: consultar_compras_proveedor
    {
        QJsonObject func;
        func["name"] = "consultar_compras_proveedor";
        func["description"] = "Consulta a qué proveedores se ha comprado un artículo o producto, el histórico de precios de compra (costo), descuentos y márgenes comerciales. Detecta productos con márgenes bajos o reducidos comparando PVP vs coste.";
        
        QJsonObject props;
        QJsonObject propProd;
        propProd["type"] = "string";
        propProd["description"] = "Nombre o código del producto a consultar compras o márgenes";
        props["producto"] = propProd;

        QJsonObject propProv;
        propProv["type"] = "string";
        propProv["description"] = "Nombre o ID del proveedor para consultar compras (ej. 'Asturdiet', 'FELIUBADALO')";
        props["proveedor"] = propProv;

        QJsonObject propMb;
        propMb["type"] = "boolean";
        propMb["description"] = "Si es true o si se pide detectar márgenes bajos/rentabilidad, devuelve los artículos con menor margen comercial.";
        props["margenes_bajos"] = propMb;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    // 14. Tool: facturacion_por_horas
    {
        QJsonObject func;
        func["name"] = "facturacion_por_horas";
        func["description"] = "Obtiene el ranking del mejor día de la semana (agrupar_por: 'dia_semana') o de las horas/franjas horarias con más facturación (agrupar_por: 'horas'), filtrando por fechas o tienda.";
        
        QJsonObject props;
        QJsonObject propAgrupar;
        propAgrupar["type"] = "string";
        propAgrupar["description"] = "'dia_semana' (para saber el mejor día de la semana: Lunes, Miércoles, etc.) o 'horas' (para franjas horarias: 10:00-11:00, etc.). Por defecto 'horas'.";
        props["agrupar_por"] = propAgrupar;

        QJsonObject propLim;
        propLim["type"] = "integer";
        propLim["description"] = "Número de resultados a devolver (por defecto 3 o 7)";
        props["limite"] = propLim;

        QJsonObject propFechaI;
        propFechaI["type"] = "string";
        propFechaI["description"] = "Fecha de inicio opcional en formato yyyy-MM-dd (ej. '2026-01-01')";
        props["fecha_inicio"] = propFechaI;

        QJsonObject propFechaF;
        propFechaF["type"] = "string";
        propFechaF["description"] = "Fecha de fin opcional en formato yyyy-MM-dd (ej. '2026-12-31')";
        props["fecha_fin"] = propFechaF;

        QJsonObject propAno;
        propAno["type"] = "integer";
        propAno["description"] = "Año a consultar (ej. 2026)";
        props["ano"] = propAno;

        QJsonObject propTienda;
        propTienda["type"] = "string";
        propTienda["description"] = "Tienda a consultar: 'todas', 'local', o específica (ej. 'Casablanca', 'Cervantes', 'Emeicjac'). Por defecto 'todas'.";
        props["tienda"] = propTienda;

        QJsonObject params;
        params["type"] = "object";
        params["properties"] = props;

        QJsonObject tool;
        tool["type"] = "function";
        tool["function"] = func;
        tools.append(tool);
    }

    return tools;
}

/**
 * @brief Consulta la lista de modelos locales a Ollama vía GET /api/tags.
 */
void AsistenteIA::consultarModelosDisponibles()
{
    QUrl url(m_baseUrl + "/api/tags");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    QNetworkReply *reply = m_netManager->get(request);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onTagsReplyFinished(reply);
    });
}

/**
 * @brief Procesa la respuesta de /api/tags y emite la lista de modelos.
 */
void AsistenteIA::onTagsReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        emit errorOcurrido(QString("Error conectando con Ollama en %1: %2")
                               .arg(m_baseUrl, reply->errorString()));
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        emit errorOcurrido("Respuesta inválida al consultar modelos en Ollama.");
        return;
    }

    QJsonArray modelsArray = doc.object().value("models").toArray();
    QStringList nombresModelos;
    for (const QJsonValue &val : modelsArray) {
        QString name = val.toObject().value("name").toString();
        if (!name.isEmpty()) {
            nombresModelos.append(name);
        }
    }

    emit modelosRecibidos(nombresModelos);
}

/**
 * @brief Envía un nuevo mensaje del usuario a la IA.
 */
void AsistenteIA::enviarMensaje(const QString &mensajeUsuario)
{
    if (m_procesando) {
        emit errorOcurrido("El asistente ya está procesando una consulta. Espera un momento.");
        return;
    }

    QString texto = mensajeUsuario.trimmed();
    if (texto.isEmpty()) return;

    // Añadir mensaje del usuario al historial
    QJsonObject userMsg;
    userMsg["role"] = "user";
    userMsg["content"] = texto;
    m_historial.append(userMsg);

    m_procesando = true;
    m_profundidadToolCalls = 0;
    emit estadoCambiado("Pensando...");

    enviarPeticionChat();
}

/**
 * @brief Envía la petición POST /api/chat a Ollama con el estado actual.
 */
void AsistenteIA::enviarPeticionChat()
{
    QUrl url(m_baseUrl + "/api/chat");
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(180000); // 180 segundos de timeout máximo para modelos locales en CPU/GPU

    QJsonObject payload;
    payload["model"] = m_modelo;
    payload["messages"] = m_historial;
    payload["stream"] = false;
    // Solo enviar herramientas en el primer turno; en el segundo paso forzar síntesis de respuesta
    if (m_profundidadToolCalls == 0) {
        payload["tools"] = construirDefinicionHerramientas();
    }
    payload["keep_alive"] = "24h"; // Mantiene el modelo cargado en memoria RAM/VRAM para responder al instante

    // Parámetros de inferencia para respuestas deterministas y precisas
    QJsonObject options;
    options["temperature"] = 0.1;
    options["top_p"] = 0.9;
    options["repeat_penalty"] = 1.15; // Evita bucles degenerativos de tokens repetidos
    options["repeat_last_n"] = 64;
    options["num_predict"] = 1500; // Límite de seguridad contra respuestas infinitas
    options["num_ctx"] = 8192; // 8K tokens: Garantiza 100% GPU (0% CPU, 48/48 capas) en la RTX 4060 Ti 16GB
    payload["options"] = options;

    QByteArray jsonData = QJsonDocument(payload).toJson(QJsonDocument::Compact);

    QNetworkReply *reply = m_netManager->post(request, jsonData);
    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        onChatReplyFinished(reply);
    });
}

/**
 * @brief Procesa la respuesta de /api/chat (mensajes de texto o llamadas a herramientas).
 */
void AsistenteIA::onChatReplyFinished(QNetworkReply *reply)
{
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError) {
        m_procesando = false;
        emit estadoCambiado("Error");
        emit errorOcurrido(QString("Error en consulta a Ollama: %1").arg(reply->errorString()));
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isObject()) {
        m_procesando = false;
        emit estadoCambiado("Error");
        emit errorOcurrido("Respuesta de Ollama no tiene formato JSON válido.");
        return;
    }

    QJsonObject root = doc.object();
    QJsonObject message = root.value("message").toObject();
    QString role = message.value("role").toString();
    QString content = message.value("content").toString();
    QJsonArray toolCalls = message.value("tool_calls").toArray();

    // Fallback inteligente: si Ollama devolvió el tool call embebido en texto (ej. {"name": "...", "arguments": ...})
    if (toolCalls.isEmpty() && !content.isEmpty()) {
        int idxJson = content.indexOf("{\"name\"");
        if (idxJson == -1) idxJson = content.indexOf("{\"function\"");
        if (idxJson == -1) idxJson = content.indexOf("{\"tool\"");
        
        if (idxJson != -1) {
            int finJson = content.lastIndexOf("}");
            if (finJson > idxJson) {
                QString jsonSub = content.mid(idxJson, finJson - idxJson + 1);
                QJsonDocument subDoc = QJsonDocument::fromJson(jsonSub.toUtf8());
                if (subDoc.isObject()) {
                    QJsonObject subObj = subDoc.object();
                    QString toolName = subObj.value("name").toString();
                    if (toolName.isEmpty() && subObj.contains("function")) {
                        toolName = subObj.value("function").toObject().value("name").toString();
                    }
                    if (!toolName.isEmpty()) {
                        QJsonObject parsedArgs = subObj.value("arguments").toObject();
                        if (parsedArgs.isEmpty() && subObj.value("arguments").isString()) {
                            parsedArgs = QJsonDocument::fromJson(subObj.value("arguments").toString().toUtf8()).object();
                        }
                        QJsonObject synthFunc;
                        synthFunc["name"] = toolName;
                        synthFunc["arguments"] = parsedArgs;
                        QJsonObject synthTc;
                        synthTc["function"] = synthFunc;
                        toolCalls.append(synthTc);
                        message["tool_calls"] = toolCalls;
                        message["content"] = "";
                    }
                }
            }
        }
    }

    // Guardar la respuesta del asistente en el historial
    m_historial.append(message);

    // Caso 1: La IA solicita ejecutar una o más herramientas (Tool Calls)
    if (!toolCalls.isEmpty()) {
        emit estadoCambiado("Consultando base de datos...");

        // Ejecutar hasta un máximo de 2 herramientas para evitar saturar el contexto de la CPU
        int ejecutadas = 0;
        QSet<QString> herramientasLlamadas;

        for (const QJsonValue &tcVal : toolCalls) {
            if (ejecutadas >= 2) break;
            QJsonObject tc = tcVal.toObject();
            QJsonObject func = tc.value("function").toObject();
            QString funcName = func.value("name").toString();
            QJsonObject args = func.value("arguments").toObject();

            // Si es la misma herramienta con argumentos idénticos, omitir duplicado
            QString firma = funcName + "_" + QString::fromUtf8(QJsonDocument(args).toJson(QJsonDocument::Compact));
            if (herramientasLlamadas.contains(firma)) continue;
            herramientasLlamadas.insert(firma);

            qDebug() << "AsistenteIA: Tool call solicitada por IA:" << funcName << "Args:" << args;
            emit herramientaEjecutada(funcName, QString("Consultando: %1").arg(funcName));

            // Ejecutar la herramienta en C++
            QJsonObject resultado = ejecutarHerramienta(funcName, args);
            qDebug() << "AsistenteIA: Resultado Tool:" << resultado;

            // Añadir el resultado de la herramienta al historial con role: "tool"
            QJsonObject toolMsg;
            toolMsg["role"] = "tool";
            toolMsg["content"] = QString::fromUtf8(QJsonDocument(resultado).toJson(QJsonDocument::Compact));
            m_historial.append(toolMsg);
            ejecutadas++;
        }

        m_profundidadToolCalls++;
        // Volver a enviar a Ollama para que sintetice la respuesta final en lenguaje natural
        enviarPeticionChat();
        return;
    }

    // Caso 2: Respuesta final de texto completada
    m_procesando = false;
    m_profundidadToolCalls = 0;
    emit estadoCambiado("Listo");
    if (!content.trimmed().isEmpty()) {
        emit respuestaRecibida(content);
    } else {
        emit respuestaRecibida("He procesado los datos de la consulta correctamente.");
    }
}

/**
 * @brief Despacha la ejecución de herramientas a los métodos correspondientes.
 */
QJsonObject AsistenteIA::ejecutarHerramienta(const QString &nombre, const QJsonObject &argumentos)
{
    if (nombre == "consultar_stock") {
        return toolConsultarStock(argumentos);
    } else if (nombre == "articulos_bajo_minimo") {
        return toolArticulosBajoMinimo(argumentos);
    } else if (nombre == "resumen_ventas") {
        return toolResumenVentas(argumentos);
    } else if (nombre == "ultimos_arqueos") {
        return toolUltimosArqueos(argumentos);
    } else if (nombre == "buscar_clientes") {
        return toolBuscarClientes(argumentos);
    } else if (nombre == "ultimas_compras_cliente") {
        return toolUltimasComprasCliente(argumentos);
    } else if (nombre == "buscar_por_indicacion") {
        return toolBuscarPorSintoma(argumentos);
    } else if (nombre == "productos_mas_vendidos") {
        return toolProductosMasVendidos(argumentos);
    } else if (nombre == "comparativa_ventas") {
        return toolComparativaVentas(argumentos);
    } else if (nombre == "prevision_cobertura_stock") {
        return toolPrevisionCoberturaStock(argumentos);
    } else if (nombre == "consultar_caducidades" || nombre == "consultar_caducidades_lotes") {
        return toolConsultarCaducidades(argumentos);
    } else if (nombre == "consultar_compras_proveedor" || nombre == "proveedores_producto" || nombre == "historial_compras_proveedor") {
        return toolConsultarComprasProveedor(argumentos);
    } else if (nombre == "facturacion_por_horas" || nombre == "horas_mas_ventas" || nombre == "horas_pico_ventas") {
        return toolFacturacionPorHoras(argumentos);
    } else if (nombre == "ejecutar_consulta_sql") {
        return toolEjecutarConsultaSql(argumentos);
    }

    QJsonObject err;
    err["error"] = QString("Herramienta desconocida: %1").arg(nombre);
    return err;
}

// ─────────────────────────────────────────────────────────────────────────────
// Implementación de Herramientas de Negocio (Tools con soporte Multi-Tienda)
// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Limpia etiquetas HTML y recorta el texto a una longitud máxima para no saturar a la IA pero incluir composición e ingredientes.
 */
static QString limpiarTextoHtml(const QString &html, int maxLongitud = 450)
{
    if (html.isEmpty()) return "";
    QString txt = html;
    txt.remove(QRegularExpression("<style.*?</style>", QRegularExpression::DotMatchesEverythingOption));
    txt.remove(QRegularExpression("<script.*?</script>", QRegularExpression::DotMatchesEverythingOption));
    txt.remove(QRegularExpression("<[^>]*>"));
    txt = txt.simplified();
    if (txt.length() > maxLongitud) {
        txt = txt.left(maxLongitud) + "...";
    }
    return txt;
}

/**
 * @brief Resuelve las conexiones de bases de datos a consultar según el parámetro 'tienda'.
 */
static QStringList resolverConexiones(const QString &tiendaFiltro)
{
    QStringList conexiones;
    QString t = tiendaFiltro.trimmed().toLower();

    QStringList activas = conf ? conf->getNombreConexionesActivas() : QStringList();
    QString localConn = conf ? conf->getConexionLocal() : "DB";
    if (localConn.isEmpty()) localConn = "DB";

    if (t.isEmpty() || t == "local" || t == "esta" || t == "actual") {
        conexiones.append(localConn);
    } else if (t.contains("toda") || t.contains("all")) {
        conexiones.append(localConn);
        for (const QString &c : activas) {
            if (c != localConn && !conexiones.contains(c)) {
                conexiones.append(c);
            }
        }
    } else {
        // Buscar coincidencia en las conexiones remotas activas o local
        bool encontrada = false;
        if (localConn.toLower().contains(t)) {
            conexiones.append(localConn);
            encontrada = true;
        }
        for (const QString &c : activas) {
            if (c.toLower().contains(t)) {
                if (!conexiones.contains(c)) conexiones.append(c);
                encontrada = true;
            }
        }
        if (!encontrada) {
            if (QSqlDatabase::contains(tiendaFiltro)) {
                conexiones.append(tiendaFiltro);
            } else {
                conexiones.append(localConn);
            }
        }
    }

    return conexiones;
}

/**
 * @brief Consulta información detallada de stock y precios de artículos en una o varias tiendas.
 */
QJsonObject AsistenteIA::toolConsultarStock(const QJsonObject &args)
{
    QString termino = args.value("termino").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("producto").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("articulo").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("codigo").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("cod").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("item").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("description").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("query").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("search").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("nombre").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("fabricante").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("marca").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("text").toString().trimmed();

    // Fallback universal: si la IA usa cualquier otra clave desconocida para el nombre/producto
    if (termino.isEmpty()) {
        for (auto it = args.begin(); it != args.end(); ++it) {
            if (it.key() != "tienda" && it.key() != "store" && it.value().isString()) {
                QString val = it.value().toString().trimmed();
                if (!val.isEmpty()) {
                    termino = val;
                    break;
                }
            }
        }
    }

    // Fallback del historial si el término viene vacío o con palabras conversacionales
    if (termino.isEmpty() || termino.contains("previst") || termino.contains("rest") || termino.contains("suficiente") || termino.contains("ano")) {
        for (int i = m_historial.size() - 1; i >= 0; --i) {
            QJsonObject msg = m_historial[i].toObject();
            if (msg.value("role").toString() == "tool") {
                QJsonDocument doc = QJsonDocument::fromJson(msg.value("content").toString().toUtf8());
                if (doc.isObject()) {
                    QJsonObject tRes = doc.object();
                    if (tRes.contains("articulos")) {
                        QJsonArray arts = tRes.value("articulos").toArray();
                        if (!arts.isEmpty()) {
                            termino = arts[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                    if (tRes.contains("productos")) {
                        QJsonArray prods = tRes.value("productos").toArray();
                        if (!prods.isEmpty()) {
                            termino = prods[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                }
            }
        }
    }

    // Si el término tiene múltiples nombres unidos por " y ", tomar la parte principal
    if (termino.contains(" y ")) {
        termino = termino.split(" y ")[0].trimmed();
    }

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;
    else if (tNorm == "local" || tNorm.contains("sucursal")) idTiendaFiltro = (conf ? conf->getIdTienda() : 4);

    QJsonObject res;
    QJsonArray articulosArray;

    if (termino.isEmpty()) {
        res["error"] = "Término de búsqueda vacío";
        return res;
    }

    bool consultadoNube = false;

    // 1. Si la nube está disponible, consultar stock_tiendas_nube con filtro de tienda opcional
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        QSqlQuery q(dbNube);
        QString sql = "SELECT s.id_tienda, COALESCE(t.nombre, CONCAT('Tienda ', s.id_tienda)) as nombre_tienda, "
                      "a.cod, a.descripcion, a.pvp, a.precio_compra, s.stock, s.min, s.max, a.notas, "
                      "COALESCE(b.nombre, '') as fabricante, COALESCE(f.descripcion, '') as familia "
                      "FROM stock_tiendas_nube s "
                      "JOIN articulos a ON s.cod = a.cod "
                      "LEFT JOIN tiendas t ON s.id_tienda = t.id "
                      "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                      "LEFT JOIN familias f ON a.familia = f.id "
                      "WHERE (a.descripcion LIKE :t1 OR a.cod = :t2 OR b.nombre LIKE :t3 OR f.descripcion LIKE :t4) ";
        if (idTiendaFiltro > 0) {
            sql += QString("AND s.id_tienda = %1 ").arg(idTiendaFiltro);
        }
        sql += "ORDER BY s.id_tienda ASC, s.stock DESC LIMIT 30";

        q.prepare(sql);
        q.bindValue(":t1", "%" + termino + "%");
        q.bindValue(":t2", termino);
        q.bindValue(":t3", "%" + termino + "%");
        q.bindValue(":t4", "%" + termino + "%");

        if (q.exec()) {
            consultadoNube = true;
            while (q.next()) {
                QJsonObject art;
                art["tienda"] = q.value("nombre_tienda").toString();
                art["codigo"] = q.value("cod").toString();
                art["nombre"] = q.value("descripcion").toString();
                art["pvp"] = QString::number(q.value("pvp").toDouble(), 'f', 2) + " €";
                
                if (conf && conf->permisos() && conf->permisos()->tiene("articulos.coste")) {
                    art["coste_pvd"] = QString::number(q.value("precio_compra").toDouble(), 'f', 2) + " €";
                }
                
                art["stock_actual"] = q.value("stock").toDouble();
                art["stock_minimo"] = q.value("min").toDouble();
                art["stock_maximo"] = q.value("max").toDouble();
                art["notas"] = limpiarTextoHtml(q.value("notas").toString());
                art["familia"] = q.value("familia").toString();
                art["fabricante"] = q.value("fabricante").toString();
                articulosArray.append(art);
            }
        }
    }

    if (!consultadoNube) {
        QStringList conexiones = resolverConexiones(tiendaFiltro);

        for (const QString &connName : conexiones) {
            if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) {
                continue;
            }

            QSqlDatabase db = QSqlDatabase::database(connName);
            QSqlQuery q(db);
            q.prepare("SELECT a.cod, a.descripcion, a.pvp, a.precio_compra, a.min, a.max, "
                      "a.notas, f.descripcion as familia, b.nombre as fabricante, "
                      "(SELECT COALESCE(SUM(l.cantidad), 0) FROM lotes l WHERE l.ean = a.cod) as stock_real "
                      "FROM articulos a "
                      "LEFT JOIN familias f ON a.familia = f.id "
                      "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                      "WHERE a.descripcion LIKE :t1 OR a.cod = :t2 OR b.nombre LIKE :t3 OR f.descripcion LIKE :t4 OR a.cod IN "
                      "(SELECT c.cod FROM codaux c WHERE c.aux = :t5) "
                      "LIMIT 15");
            q.bindValue(":t1", "%" + termino + "%");
            q.bindValue(":t2", termino);
            q.bindValue(":t3", "%" + termino + "%");
            q.bindValue(":t4", "%" + termino + "%");
            q.bindValue(":t5", termino);

            if (q.exec()) {
                while (q.next()) {
                    QJsonObject art;
                    art["tienda"] = (connName == "DB" || (conf && connName == conf->getConexionLocal())) ? "Tienda Local" : connName;
                    art["codigo"] = q.value("cod").toString();
                    art["nombre"] = q.value("descripcion").toString();
                    art["pvp"] = QString::number(q.value("pvp").toDouble(), 'f', 2) + " €";
                    
                    // Solo mostrar coste si el usuario tiene permisos
                    if (conf && conf->permisos() && conf->permisos()->tiene("articulos.coste")) {
                        art["coste_pvd"] = QString::number(q.value("precio_compra").toDouble(), 'f', 2) + " €";
                    }
                    
                    art["stock_actual"] = q.value("stock_real").toDouble();
                    art["stock_minimo"] = q.value("min").toDouble();
                    art["stock_maximo"] = q.value("max").toDouble();
                    art["notas"] = limpiarTextoHtml(q.value("notas").toString());
                    art["familia"] = q.value("familia").toString();
                    art["fabricante"] = q.value("fabricante").toString();
                    articulosArray.append(art);
                }
            } else {
                qDebug() << "toolConsultarStock SQL Error en" << connName << ":" << q.lastError().text();
            }
        }
    }

    res["total_encontrados"] = articulosArray.size();
    res["articulos"] = articulosArray;
    return res;
}

/**
 * @brief Consulta los artículos cuyo stock está bajo mínimos.
 */
QJsonObject AsistenteIA::toolArticulosBajoMinimo(const QJsonObject &args)
{
    int limite = args.value("limite").toInt();
    if (limite <= 0) limite = args.value("limit").toInt(15);
    if (limite <= 0 || limite > 50) limite = 20;

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();

    QJsonObject res;
    QJsonArray articulosArray;

    QStringList conexiones = resolverConexiones(tiendaFiltro);

    for (const QString &connName : conexiones) {
        if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) {
            continue;
        }

        QSqlDatabase db = QSqlDatabase::database(connName);
        QSqlQuery q(db);
        q.prepare("SELECT a.cod, a.descripcion, a.pvp, a.min, a.max, "
                  "(SELECT COALESCE(SUM(l.cantidad), 0) FROM lotes l WHERE l.ean = a.cod) as stock_real "
                  "FROM articulos a "
                  "WHERE a.min > 0 AND ((SELECT COALESCE(SUM(l.cantidad), 0) FROM lotes l WHERE l.ean = a.cod) <= a.min) "
                  "ORDER BY ((SELECT COALESCE(SUM(l.cantidad), 0) FROM lotes l WHERE l.ean = a.cod) - a.min) ASC "
                  "LIMIT :limite");
        q.bindValue(":limite", limite);

        if (q.exec()) {
            while (q.next()) {
                QJsonObject art;
                art["tienda"] = (connName == "DB" || (conf && connName == conf->getConexionLocal())) ? "Tienda Local" : connName;
                art["codigo"] = q.value("cod").toString();
                art["nombre"] = q.value("descripcion").toString();
                art["pvp"] = QString::number(q.value("pvp").toDouble(), 'f', 2) + " €";
                art["stock_actual"] = q.value("stock_real").toDouble();
                art["stock_minimo"] = q.value("min").toDouble();
                articulosArray.append(art);
            }
        }
    }

    res["total_bajo_minimo"] = articulosArray.size();
    res["articulos"] = articulosArray;
    return res;
}

/**
 * @brief Resumen de ventas entre dos fechas con soporte de tiendas remotas.
 */
QJsonObject AsistenteIA::toolResumenVentas(const QJsonObject &args)
{
    QJsonObject res;

    // Validación de permisos
    if (conf && conf->permisos() && !conf->permisos()->tiene("estadisticas") && !conf->permisos()->tiene("cajas")) {
        res["error"] = "Permiso denegado: El usuario activo no tiene permiso para consultar estadísticas o ventas.";
        return res;
    }

    // Extracción tolerante de fecha de inicio
    QString fechaI = args.value("fecha_inicio").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("date_from").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("start_date").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("desde").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("fecha").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("date").toString().trimmed();
    if (fechaI.isEmpty()) {
        fechaI = QDate::currentDate().toString("yyyy-MM-dd");
    }

    // Extracción tolerante de fecha de fin
    QString fechaF = args.value("fecha_fin").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("date_to").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("end_date").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("hasta").toString().trimmed();
    if (fechaF.isEmpty()) {
        fechaF = fechaI;
    }

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;
    else if (tNorm == "local" || tNorm.contains("sucursal")) idTiendaFiltro = (conf ? conf->getIdTienda() : 4);

    int totalGlobalTickets = 0;
    double totalGlobalVentas = 0.0;
    double totalGlobalEfectivo = 0.0;
    double totalGlobalTarjeta = 0.0;
    QJsonArray desgloseTiendas;
    bool consultadoNube = false;

    // 1. Priorizar consulta directa a la base consolidada en la nube si está disponible
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        QSqlQuery q(dbNube);
        QString sql = "SELECT t.id_tienda, COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) as nombre_tienda, "
                      "COUNT(*) as num_tickets, "
                      "COALESCE(SUM(t.total), 0) as total_ventas, "
                      "COALESCE(SUM(CASE WHEN t.fpago = 1 THEN t.total ELSE 0 END), 0) as total_efectivo, "
                      "COALESCE(SUM(CASE WHEN t.fpago != 1 THEN t.total ELSE 0 END), 0) as total_tarjeta "
                      "FROM tickets_nube t "
                      "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                      "WHERE t.fecha >= :f1 AND t.fecha <= :f2 ";
        if (idTiendaFiltro > 0) {
            sql += QString("AND t.id_tienda = %1 ").arg(idTiendaFiltro);
        }
        sql += "GROUP BY t.id_tienda, ti.nombre ORDER BY t.id_tienda ASC";

        q.prepare(sql);
        q.bindValue(":f1", fechaI);
        q.bindValue(":f2", fechaF);

        if (q.exec()) {
            consultadoNube = true;
            while (q.next()) {
                int numT = q.value("num_tickets").toInt();
                double vTot = q.value("total_ventas").toDouble();
                double vEfec = q.value("total_efectivo").toDouble();
                double vTarj = q.value("total_tarjeta").toDouble();

                totalGlobalTickets += numT;
                totalGlobalVentas += vTot;
                totalGlobalEfectivo += vEfec;
                totalGlobalTarjeta += vTarj;

                QJsonObject tInfo;
                tInfo["tienda"] = q.value("nombre_tienda").toString();
                tInfo["id_tienda"] = q.value("id_tienda").toInt();
                tInfo["numero_tickets"] = numT;
                tInfo["total_ventas"] = QString::number(vTot, 'f', 2) + " €";
                tInfo["total_efectivo"] = QString::number(vEfec, 'f', 2) + " €";
                tInfo["total_tarjeta"] = QString::number(vTarj, 'f', 2) + " €";
                desgloseTiendas.append(tInfo);
            }
        }
    }

    // 2. Fallback a conexiones locales / remotas activas
    if (!consultadoNube) {
        QStringList conexiones = resolverConexiones(tiendaFiltro);

        for (const QString &connName : conexiones) {
            if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) {
                continue;
            }

            QSqlDatabase db = QSqlDatabase::database(connName);
            QSqlQuery q(db);
            q.prepare("SELECT COUNT(*) as num_tickets, "
                      "COALESCE(SUM(t.total), 0) as total_ventas, "
                      "COALESCE(SUM(CASE WHEN f.efectivo = 1 THEN t.total ELSE 0 END), 0) as total_efectivo, "
                      "COALESCE(SUM(CASE WHEN f.efectivo = 0 OR f.efectivo IS NULL THEN t.total ELSE 0 END), 0) as total_tarjeta "
                      "FROM tickets t "
                      "LEFT JOIN fpago f ON t.fpago = f.id "
                      "WHERE t.fecha >= :f1 AND t.fecha <= :f2");
            q.bindValue(":f1", fechaI);
            q.bindValue(":f2", fechaF);

            if (q.exec() && q.next()) {
                int numT = q.value("num_tickets").toInt();
                double vTot = q.value("total_ventas").toDouble();
                double vEfec = q.value("total_efectivo").toDouble();
                double vTarj = q.value("total_tarjeta").toDouble();

                totalGlobalTickets += numT;
                totalGlobalVentas += vTot;
                totalGlobalEfectivo += vEfec;
                totalGlobalTarjeta += vTarj;

                QJsonObject tInfo;
                tInfo["tienda"] = (connName == "DB" || (conf && connName == conf->getConexionLocal())) ? "Tienda Local" : connName;
                tInfo["numero_tickets"] = numT;
                tInfo["total_ventas"] = QString::number(vTot, 'f', 2) + " €";
                tInfo["total_efectivo"] = QString::number(vEfec, 'f', 2) + " €";
                tInfo["total_tarjeta"] = QString::number(vTarj, 'f', 2) + " €";
                desgloseTiendas.append(tInfo);
            } else {
                qDebug() << "toolResumenVentas SQL Error en" << connName << ":" << q.lastError().text();
            }
        }
    }

    res["tienda_solicitada"] = tiendaFiltro;
    res["fecha_inicio"] = fechaI;
    res["fecha_fin"] = fechaF;
    res["total_global_ventas"] = QString::number(totalGlobalVentas, 'f', 2) + " €";
    res["total_global_efectivo"] = QString::number(totalGlobalEfectivo, 'f', 2) + " €";
    res["total_global_tarjeta"] = QString::number(totalGlobalTarjeta, 'f', 2) + " €";
    res["total_global_tickets"] = totalGlobalTickets;
    res["desglose_por_tienda"] = desgloseTiendas;

    return res;
}

/**
 * @brief Consulta los últimos arqueos de caja registrados.
 */
QJsonObject AsistenteIA::toolUltimosArqueos(const QJsonObject &args)
{
    QJsonObject res;

    // Validación de permisos
    if (conf && conf->permisos() && !conf->permisos()->tiene("cajas.ver_arqueos") && !conf->permisos()->tiene("cajas")) {
        res["error"] = "Permiso denegado: El usuario activo no tiene permiso para consultar arqueos de caja.";
        return res;
    }

    int limite = args.value("limite").toInt();
    if (limite <= 0) limite = args.value("limit").toInt(10);
    if (limite <= 0 || limite > 30) limite = 10;

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;
    else if (tNorm == "local" || tNorm.contains("sucursal")) idTiendaFiltro = (conf ? conf->getIdTienda() : 4);

    QJsonArray arqueosArray;
    QJsonArray ultimosPorTiendaArray;
    bool consultadoNube = false;

    // 1. Consultar nube si está disponible
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        
        // Si se pide 'todas', obtener el último arqueo de CADA tienda para que ninguna quede fuera
        if (idTiendaFiltro == 0) {
            QSqlQuery qCada(dbNube);
            QString sqlCada = "SELECT a.id_local as id, a.id_tienda, DATE_FORMAT(a.fecha, '%Y-%m-%d') as fecha_fmt, a.hora, a.usuario, "
                              "a.ventasEfectivo, a.ventasTarjeta, a.entradas, a.efectivoReal, a.descuadre, a.efectivoContado, "
                              "(COALESCE(a.ventasEfectivo, 0) + COALESCE(a.ventasTarjeta, 0)) as totalVentas, "
                              "COALESCE(t.nombre, CONCAT('Tienda ', a.id_tienda)) as nombre_tienda "
                              "FROM arqueos_nube a "
                              "LEFT JOIN tiendas t ON a.id_tienda = t.id "
                              "INNER JOIN ( "
                              "    SELECT id_tienda, MAX(CONCAT(fecha, ' ', hora)) as max_fh "
                              "    FROM arqueos_nube GROUP BY id_tienda "
                              ") m ON a.id_tienda = m.id_tienda AND CONCAT(a.fecha, ' ', a.hora) = m.max_fh "
                              "ORDER BY a.id_tienda ASC";
            if (qCada.exec(sqlCada)) {
                while (qCada.next()) {
                    QJsonObject arq;
                    arq["tienda"] = qCada.value("nombre_tienda").toString();
                    arq["id"] = qCada.value("id").toInt();
                    arq["fecha"] = qCada.value("fecha_fmt").toString();
                    arq["hora"] = qCada.value("hora").toString();
                    arq["usuario"] = qCada.value("usuario").toString();
                    arq["total_ventas"] = QString::number(qCada.value("totalVentas").toDouble(), 'f', 2) + " €";
                    arq["ventas_efectivo"] = QString::number(qCada.value("ventasEfectivo").toDouble(), 'f', 2) + " €";
                    arq["ventas_tarjeta"] = QString::number(qCada.value("ventasTarjeta").toDouble(), 'f', 2) + " €";
                    arq["entradas_caja"] = QString::number(qCada.value("entradas").toDouble(), 'f', 2) + " €";
                    arq["efectivo_real"] = QString::number(qCada.value("efectivoReal").toDouble(), 'f', 2) + " €";
                    arq["descuadre"] = QString::number(qCada.value("descuadre").toDouble(), 'f', 2) + " €";
                    arq["efectivo_contado"] = QString::number(qCada.value("efectivoContado").toDouble(), 'f', 2) + " €";
                    ultimosPorTiendaArray.append(arq);
                }
            }
        }

        // Obtener historial reciente
        QSqlQuery q(dbNube);
        QString sql = "SELECT a.id_local as id, DATE_FORMAT(a.fecha, '%Y-%m-%d') as fecha_fmt, a.hora, a.usuario, "
                      "a.ventasEfectivo, a.ventasTarjeta, a.entradas, a.efectivoReal, a.descuadre, a.efectivoContado, "
                      "(COALESCE(a.ventasEfectivo, 0) + COALESCE(a.ventasTarjeta, 0)) as totalVentas, "
                      "COALESCE(t.nombre, CONCAT('Tienda ', a.id_tienda)) as nombre_tienda "
                      "FROM arqueos_nube a "
                      "LEFT JOIN tiendas t ON a.id_tienda = t.id ";
        if (idTiendaFiltro > 0) {
            sql += QString("WHERE a.id_tienda = %1 ").arg(idTiendaFiltro);
        }
        sql += "ORDER BY a.fecha DESC, a.hora DESC LIMIT :limite";

        q.prepare(sql);
        q.bindValue(":limite", limite);

        if (q.exec()) {
            consultadoNube = true;
            while (q.next()) {
                QJsonObject arq;
                arq["tienda"] = q.value("nombre_tienda").toString();
                arq["id"] = q.value("id").toInt();
                arq["fecha"] = q.value("fecha_fmt").toString();
                arq["hora"] = q.value("hora").toString();
                arq["usuario"] = q.value("usuario").toString();
                arq["total_ventas"] = QString::number(q.value("totalVentas").toDouble(), 'f', 2) + " €";
                arq["ventas_efectivo"] = QString::number(q.value("ventasEfectivo").toDouble(), 'f', 2) + " €";
                arq["ventas_tarjeta"] = QString::number(q.value("ventasTarjeta").toDouble(), 'f', 2) + " €";
                arq["entradas_caja"] = QString::number(q.value("entradas").toDouble(), 'f', 2) + " €";
                arq["efectivo_real"] = QString::number(q.value("efectivoReal").toDouble(), 'f', 2) + " €";
                arq["descuadre"] = QString::number(q.value("descuadre").toDouble(), 'f', 2) + " €";
                arq["efectivo_contado"] = QString::number(q.value("efectivoContado").toDouble(), 'f', 2) + " €";
                arqueosArray.append(arq);
            }
        }
    }

    if (!consultadoNube) {
        QStringList conexiones = resolverConexiones(tiendaFiltro);

        for (const QString &connName : conexiones) {
            if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) {
                continue;
            }

            QSqlDatabase db = QSqlDatabase::database(connName);
            QSqlQuery q(db);
            q.prepare("SELECT id, DATE_FORMAT(fecha, '%Y-%m-%d') as fecha_fmt, hora, usuario, "
                      "ventasEfectivo, ventasTarjeta, entradas, efectivoReal, descuadre, efectivoContado, "
                      "(COALESCE(ventasEfectivo, 0) + COALESCE(ventasTarjeta, 0)) as totalVentas "
                      "FROM arqueos "
                      "ORDER BY fecha DESC, hora DESC "
                      "LIMIT :limite");
            q.bindValue(":limite", limite);

            if (q.exec()) {
                while (q.next()) {
                    QJsonObject arq;
                    arq["tienda"] = (connName == "DB" || (conf && connName == conf->getConexionLocal())) ? "Tienda Local" : connName;
                    arq["id"] = q.value("id").toInt();
                    arq["fecha"] = q.value("fecha_fmt").toString();
                    arq["hora"] = q.value("hora").toString();
                    arq["usuario"] = q.value("usuario").toString();
                    arq["total_ventas"] = QString::number(q.value("totalVentas").toDouble(), 'f', 2) + " €";
                    arq["ventas_efectivo"] = QString::number(q.value("ventasEfectivo").toDouble(), 'f', 2) + " €";
                    arq["ventas_tarjeta"] = QString::number(q.value("ventasTarjeta").toDouble(), 'f', 2) + " €";
                    arq["entradas_caja"] = QString::number(q.value("entradas").toDouble(), 'f', 2) + " €";
                    arq["efectivo_real"] = QString::number(q.value("efectivoReal").toDouble(), 'f', 2) + " €";
                    arq["descuadre"] = QString::number(q.value("descuadre").toDouble(), 'f', 2) + " €";
                    arq["efectivo_contado"] = QString::number(q.value("efectivoContado").toDouble(), 'f', 2) + " €";
                    arqueosArray.append(arq);
                }
            } else {
                qDebug() << "toolUltimosArqueos SQL Error en" << connName << ":" << q.lastError().text();
            }
        }
    }

    res["total_arqueos"] = arqueosArray.size();
    if (!ultimosPorTiendaArray.isEmpty()) {
        res["ultimo_arqueo_de_cada_tienda"] = ultimosPorTiendaArray;
    }
    res["historial_arqueos_recientes"] = arqueosArray;
    res["arqueos"] = arqueosArray;
    return res;
}

/**
 * @brief Busca clientes por nombre, apellidos, teléfono o CIF/NIF (con soporte nube).
 */
QJsonObject AsistenteIA::toolBuscarClientes(const QJsonObject &args)
{
    QString termino = args.value("termino").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("query").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("search").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("nombre").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("cliente").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("cif").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("nif").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("telefono").toString().trimmed();

    QJsonObject res;
    QJsonArray clientesArray;

    if (termino.isEmpty()) {
        res["error"] = "Término de búsqueda de cliente vacío";
        return res;
    }

    QSqlDatabase db;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "Base de datos no disponible";
        return res;
    }

    QSqlQuery q(db);
    QStringList palabras = termino.split(" ", Qt::SkipEmptyParts);
    QString sql = "SELECT idCliente, nombre, apellidos, telefono, telefono2, mail, nif, direccion, localidad, provincia FROM clientes WHERE ";
    
    if (palabras.size() > 1) {
        QStringList condiciones;
        for (int i = 0; i < palabras.size(); ++i) {
            condiciones.append(QString("(nombre LIKE :p%1 OR apellidos LIKE :p%1)").arg(i));
        }
        sql += "(" + condiciones.join(" AND ") + ") OR (CONCAT(nombre, ' ', apellidos) LIKE :termCompleto) ";
    } else {
        sql += "(nombre LIKE :termCompleto OR apellidos LIKE :termCompleto OR telefono LIKE :termCompleto OR telefono2 LIKE :termCompleto OR nif LIKE :termCompleto OR idCliente = :idDirecto) ";
    }
    sql += "ORDER BY idCliente ASC LIMIT 20";

    q.prepare(sql);
    q.bindValue(":termCompleto", "%" + termino + "%");
    q.bindValue(":idDirecto", termino.toInt());
    for (int i = 0; i < palabras.size(); ++i) {
        q.bindValue(QString(":p%1").arg(i), "%" + palabras[i] + "%");
    }

    if (q.exec()) {
        while (q.next()) {
            QJsonObject cli;
            cli["id_cliente"] = q.value("idCliente").toInt();
            cli["nombre_completo"] = (q.value("nombre").toString() + " " + q.value("apellidos").toString()).trimmed();
            
            QString tel1 = q.value("telefono").toString().trimmed();
            QString tel2 = q.value("telefono2").toString().trimmed();
            cli["telefono"] = !tel1.isEmpty() ? tel1 : (!tel2.isEmpty() ? tel2 : "No registrado");

            QString mail = q.value("mail").toString().trimmed();
            cli["email"] = !mail.isEmpty() ? mail : "No registrado";

            QString nif = q.value("nif").toString().trimmed();
            cli["nif"] = !nif.isEmpty() ? nif : "No registrado";

            QString dir = q.value("direccion").toString().trimmed();
            cli["direccion"] = !dir.isEmpty() ? dir : "No registrada";

            QString loc = q.value("localidad").toString().trimmed();
            cli["localidad"] = !loc.isEmpty() ? loc : "No registrada";

            QString prov = q.value("provincia").toString().trimmed();
            cli["provincia"] = !prov.isEmpty() ? prov : "No registrada";
            
            clientesArray.append(cli);
        }
    } else {
        qDebug() << "toolBuscarClientes SQL Error:" << q.lastError().text();
    }

    res["total_encontrados"] = clientesArray.size();
    res["clientes"] = clientesArray;
    return res;
}

/**
 * @brief Consulta el historial de compras y los artículos más comprados por un cliente (resuelve por ID o por nombre), o busca compradores de una marca/producto.
 */
QJsonObject AsistenteIA::toolUltimasComprasCliente(const QJsonObject &args)
{
    QJsonObject res;
    int idCliente = args.value("id_cliente").toInt();
    if (idCliente <= 0) idCliente = args.value("idCliente").toInt();

    QString nombreCli = args.value("cliente").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("nombre").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("fabricante").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("marca").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("producto").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("articulo").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("brand").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("query").toString().trimmed();
    if (nombreCli.isEmpty()) nombreCli = args.value("termino").toString().trimmed();

    int limite = args.value("limite").toInt();
    if (limite <= 0) limite = args.value("limit").toInt();
    if (limite <= 0 || limite > 50) limite = 10;

    QSqlDatabase db;
    bool esNube = false;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        esNube = true;
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "Base de datos no disponible";
        return res;
    }

    // 0. Comprobar si se solicita un RANKING / TOP de clientes por volumen de compras/gasto
    QString norm = nombreCli.toLower();
    bool esRanking = args.value("top_clientes").toBool() || args.value("ranking").toBool() ||
                     norm.contains("top") || norm.contains("ranking") || norm.contains("mayor") ||
                     norm.contains("gasto") || norm.contains("volumen") || norm.contains("todos") ||
                     (idCliente <= 0 && nombreCli.isEmpty());

    if (esRanking) {
        QDate hoy = QDate::currentDate();
        int ano = args.value("ano").toInt();
        if (ano <= 0) ano = args.value("year").toInt();
        if (ano <= 0) ano = args.value("anyo").toInt();

        // 1. Extraer año si venía en el término de búsqueda
        static const QRegularExpression regAno("\\b(20[123]\\d)\\b");
        QRegularExpressionMatch mTerm = regAno.match(nombreCli);
        if (ano <= 0 && mTerm.hasMatch()) {
            ano = mTerm.captured(1).toInt();
        }

        // 2. Extraer año si venía en el último mensaje del usuario
        if (ano <= 0 && args.value("fecha_inicio").toString().isEmpty()) {
            for (int idx = m_historial.size() - 1; idx >= 0; --idx) {
                QJsonObject msgObj = m_historial[idx].toObject();
                if (msgObj.value("role").toString() == "user") {
                    QString txt = msgObj.value("content").toString();
                    QRegularExpressionMatch mUser = regAno.match(txt);
                    if (mUser.hasMatch()) {
                        ano = mUser.captured(1).toInt();
                        break;
                    }
                }
            }
        }

        QString fechaI = args.value("fecha_inicio").toString().trimmed();
        if (fechaI.isEmpty()) fechaI = args.value("desde").toString().trimmed();
        if (fechaI.isEmpty()) {
            if (ano > 2000) {
                fechaI = QString("%1-01-01").arg(ano);
            } else {
                fechaI = hoy.addYears(-1).toString("yyyy-MM-dd");
            }
        }
        QString fechaF = args.value("fecha_fin").toString().trimmed();
        if (fechaF.isEmpty()) fechaF = args.value("hasta").toString().trimmed();
        if (fechaF.isEmpty()) {
            if (ano > 2000) {
                fechaF = QString("%1-12-31").arg(ano);
            } else {
                fechaF = hoy.toString("yyyy-MM-dd");
            }
        }

        QSqlQuery q(db);
        QString sql;
        if (esNube) {
            sql = "SELECT id_cliente, cliente, COALESCE(telefono_cliente, '') as telefono, "
                  "SUM(total) as volumen_compras, COUNT(DISTINCT ticket) as total_tickets "
                  "FROM vista_ventas_detalladas "
                  "WHERE fecha >= :f1 AND fecha <= :f2 AND id_cliente > 1 "
                  "GROUP BY id_cliente, cliente "
                  "ORDER BY volumen_compras DESC "
                  "LIMIT :limite";
        } else {
            sql = "SELECT c.idCliente as id_cliente, CONCAT(c.nombre, ' ', COALESCE(c.apellidos, '')) as cliente, "
                  "COALESCE(c.telefono, '') as telefono, "
                  "SUM(l.pvp * l.cantidad - l.descuento) as volumen_compras, COUNT(DISTINCT t.ticket) as total_tickets "
                  "FROM lineasticket l "
                  "JOIN tickets t ON l.nticket = t.ticket "
                  "JOIN clientes c ON t.cliente = c.idCliente "
                  "WHERE l.fecha >= :f1 AND l.fecha <= :f2 AND c.idCliente > 1 "
                  "GROUP BY c.idCliente "
                  "ORDER BY volumen_compras DESC "
                  "LIMIT :limite";
        }
        q.prepare(sql);
        q.bindValue(":f1", fechaI);
        q.bindValue(":f2", fechaF);
        q.bindValue(":limite", limite);

        QJsonArray rankingArray;
        if (q.exec()) {
            int pos = 1;
            while (q.next()) {
                QJsonObject item;
                item["posicion"] = pos++;
                item["id_cliente"] = q.value("id_cliente").toInt();
                item["cliente"] = q.value("cliente").toString().trimmed();
                item["volumen_compras"] = QString::number(q.value("volumen_compras").toDouble(), 'f', 2) + " €";
                item["total_tickets"] = q.value("total_tickets").toInt();
                QString tel = q.value("telefono").toString().trimmed();
                item["telefono"] = !tel.isEmpty() ? tel : "No registrado";
                rankingArray.append(item);
            }
        }
        res["tipo_resultado"] = "ranking_top_clientes_compras";
        res["periodo"] = QString("Del %1 al %2").arg(fechaI, fechaF);
        res["total_clientes"] = rankingArray.size();
        res["top_clientes"] = rankingArray;
        return res;
    }

    if (idCliente <= 0 && nombreCli.isEmpty()) {
        res["error"] = "Debes indicar el nombre del cliente, ID de cliente, o el fabricante/producto para buscar sus compradores.";
        return res;
    }

    // Si 'cliente' es un número en string
    bool okNum = false;
    int idDesdeString = nombreCli.toInt(&okNum);
    if (okNum && idDesdeString > 0) {
        idCliente = idDesdeString;
    }

    QString nombreCompleto = "";

    // 1. Si no tenemos idCliente, buscarlo por nombre/apellidos exacto o parcial
    if (idCliente <= 0 && !nombreCli.isEmpty()) {
        QSqlQuery qCli(db);
        QString sqlCli = "SELECT idCliente, CONCAT(nombre, ' ', apellidos) as nom FROM clientes "
                         "WHERE nombre LIKE :n1 OR apellidos LIKE :n2 OR CONCAT(nombre, ' ', apellidos) LIKE :n3 "
                         "ORDER BY idCliente ASC LIMIT 1";
        qCli.prepare(sqlCli);
        qCli.bindValue(":n1", "%" + nombreCli + "%");
        qCli.bindValue(":n2", "%" + nombreCli + "%");
        qCli.bindValue(":n3", "%" + nombreCli + "%");
        if (qCli.exec() && qCli.next()) {
            idCliente = qCli.value("idCliente").toInt();
            nombreCompleto = qCli.value("nom").toString().trimmed();
        } else {
            // Intentar con la primera palabra si hay varias
            QStringList partes = nombreCli.split(" ", Qt::SkipEmptyParts);
            if (!partes.isEmpty() && partes.first().length() >= 3) {
                QSqlQuery qP(db);
                qP.prepare("SELECT idCliente, CONCAT(nombre, ' ', apellidos) as nom FROM clientes "
                           "WHERE nombre LIKE :p OR apellidos LIKE :p ORDER BY idCliente ASC LIMIT 1");
                qP.bindValue(":p", "%" + partes.first() + "%");
                if (qP.exec() && qP.next()) {
                    idCliente = qP.value("idCliente").toInt();
                    nombreCompleto = qP.value("nom").toString().trimmed();
                }
            }
        }
    }

    // 2. Si no es un cliente, buscar si es un fabricante o producto para listar los clientes que lo compraron
    if (idCliente <= 0) {
        QSqlQuery qFab(db);
        QString sqlFab;
        if (esNube) {
            sqlFab = "SELECT DISTINCT id_cliente, cliente, COALESCE(telefono, '') as telefono, "
                     "producto as producto_comprado, fabricante, "
                     "DATE_FORMAT(fecha, '%Y-%m-%d') as fecha, tienda "
                     "FROM vista_compras_clientes "
                     "WHERE (fabricante LIKE :term OR producto LIKE :term2) "
                     "ORDER BY fecha DESC LIMIT 20";
        } else {
            sqlFab = "SELECT DISTINCT c.idCliente, CONCAT(c.nombre, ' ', COALESCE(c.apellidos, '')) as cliente, "
                     "COALESCE(c.telefono, '') as telefono, a.descripcion as producto_comprado, "
                     "COALESCE(b.nombre, '') as fabricante, "
                     "DATE_FORMAT(l.fecha, '%Y-%m-%d') as fecha, 'Local' as tienda "
                     "FROM lineasticket l "
                     "JOIN tickets t ON l.nticket = t.ticket "
                     "JOIN clientes c ON t.cliente = c.idCliente "
                     "JOIN articulos a ON l.cod = a.cod "
                     "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                     "WHERE (b.nombre LIKE :term OR a.descripcion LIKE :term2) AND c.idCliente != 1 "
                     "ORDER BY l.fecha DESC LIMIT 20";
        }
        qFab.prepare(sqlFab);
        qFab.bindValue(":term", "%" + nombreCli + "%");
        qFab.bindValue(":term2", "%" + nombreCli + "%");
        if (qFab.exec()) {
            QMap<int, QJsonObject> clientesMap;
            int totalCompras = 0;
            while (qFab.next()) {
                int idC = qFab.value(0).toInt();
                QString nombre = qFab.value("cliente").toString().trimmed();
                totalCompras++;

                if (!clientesMap.contains(idC)) {
                    QJsonObject cObj;
                    cObj["id_cliente"] = idC;
                    cObj["cliente"] = nombre;
                    cObj["telefono"] = qFab.value("telefono").toString();
                    cObj["fabricante"] = qFab.value("fabricante").toString();
                    cObj["compras"] = QJsonArray();
                    clientesMap[idC] = cObj;
                }

                QJsonObject compraDetalle;
                compraDetalle["producto"] = qFab.value("producto_comprado").toString();
                compraDetalle["fecha"] = qFab.value("fecha").toString();
                compraDetalle["tienda"] = qFab.value("tienda").toString();

                QJsonObject curObj = clientesMap[idC];
                QJsonArray arr = curObj["compras"].toArray();
                arr.append(compraDetalle);
                curObj["compras"] = arr;
                clientesMap[idC] = curObj;
            }

            if (!clientesMap.isEmpty()) {
                QJsonArray clientesFinales;
                for (auto it = clientesMap.begin(); it != clientesMap.end(); ++it) {
                    clientesFinales.append(it.value());
                }
                res["tipo_resultado"] = "clientes_compradores_por_producto_o_fabricante";
                res["fabricante_o_producto_buscado"] = nombreCli;
                res["total_clientes_distintos"] = clientesFinales.size();
                res["total_compras_registradas"] = totalCompras;
                res["clientes_compradores"] = clientesFinales;
                return res;
            }
        }

        res["error"] = QString("No se encontró ningún cliente ni fabricante/producto registrado con el nombre '%1'.").arg(nombreCli);
        return res;
    }

    if (nombreCompleto.isEmpty()) {
        QSqlQuery qNom(db);
        qNom.prepare("SELECT CONCAT(nombre, ' ', apellidos) as nom FROM clientes WHERE idCliente = :id");
        qNom.bindValue(":id", idCliente);
        if (qNom.exec() && qNom.next()) {
            nombreCompleto = qNom.value("nom").toString().trimmed();
        }
    }

    QString fechaI = args.value("fecha_inicio").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("date_from").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("start_date").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("desde").toString().trimmed();

    QString fechaF = args.value("fecha_fin").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("date_to").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("end_date").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("hasta").toString().trimmed();

    int ano = args.value("ano").toInt();
    if (ano <= 0) ano = args.value("year").toInt();
    if (ano > 2000) {
        if (fechaI.isEmpty()) fechaI = QString("%1-01-01").arg(ano);
        if (fechaF.isEmpty()) fechaF = QString("%1-12-31").arg(ano);
    }

    // 2. Si se consulta si o cuándo el cliente ha comprado un producto concreto (ej. 'Lecidol', 'Colamag')
    QString productoFiltro = args.value("producto").toString().trimmed();
    if (productoFiltro.isEmpty()) productoFiltro = args.value("articulo").toString().trimmed();
    if (productoFiltro.isEmpty()) productoFiltro = args.value("item").toString().trimmed();

    if (!productoFiltro.isEmpty()) {
        QJsonArray comprasProdArray;
        QSqlQuery qP(db);
        QString sqlP;
        if (esNube) {
            sqlP = "SELECT v.fecha, v.hora, v.tienda, v.producto, v.cantidad, v.pvp_unitario, v.total, v.ticket "
                   "FROM vista_ventas_detalladas v "
                   "WHERE v.id_cliente = :idCli AND (v.producto LIKE :p1 OR v.codigo_articulo LIKE :p2 OR v.fabricante LIKE :p3) ";
            if (!fechaI.isEmpty()) sqlP += "AND v.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlP += "AND v.fecha <= :f2 ";
            sqlP += "ORDER BY v.fecha DESC, v.hora DESC LIMIT 20";
        } else {
            sqlP = "SELECT l.fecha, t.hora, 'Local' as tienda, a.descripcion as producto, l.cantidad, l.pvp as pvp_unitario, "
                   "(l.pvp * l.cantidad - l.descuento) as total, t.ticket "
                   "FROM lineasticket l "
                   "JOIN tickets t ON l.nticket = t.ticket "
                   "JOIN articulos a ON l.cod = a.cod "
                   "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                   "WHERE t.cliente = :idCli AND (a.descripcion LIKE :p1 OR a.cod LIKE :p2 OR b.nombre LIKE :p3) ";
            if (!fechaI.isEmpty()) sqlP += "AND l.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlP += "AND l.fecha <= :f2 ";
            sqlP += "ORDER BY l.fecha DESC, t.hora DESC LIMIT 20";
        }
        qP.prepare(sqlP);
        qP.bindValue(":idCli", idCliente);
        qP.bindValue(":p1", "%" + productoFiltro + "%");
        qP.bindValue(":p2", "%" + productoFiltro + "%");
        qP.bindValue(":p3", "%" + productoFiltro + "%");
        if (!fechaI.isEmpty()) qP.bindValue(":f1", fechaI);
        if (!fechaF.isEmpty()) qP.bindValue(":f2", fechaF);

        double unidadesTotales = 0.0;
        double gastoTotal = 0.0;

        if (qP.exec()) {
            while (qP.next()) {
                QJsonObject cObj;
                cObj["fecha"] = qP.value("fecha").toString();
                cObj["hora"] = qP.value("hora").toString();
                cObj["tienda"] = qP.value("tienda").toString();
                cObj["producto"] = qP.value("producto").toString();
                double cant = qP.value("cantidad").toDouble();
                double tot = qP.value("total").toDouble();
                unidadesTotales += cant;
                gastoTotal += tot;
                cObj["cantidad"] = cant;
                cObj["pvp_unitario"] = QString::number(qP.value("pvp_unitario").toDouble(), 'f', 2) + " €";
                cObj["total_ticket"] = QString::number(tot, 'f', 2) + " €";
                cObj["ticket"] = qP.value("ticket").toString();
                comprasProdArray.append(cObj);
            }
        }

        res["tipo_resultado"] = "compras_producto_especifico_cliente";
        res["cliente"] = nombreCompleto;
        res["id_cliente"] = idCliente;
        res["producto_consultado"] = productoFiltro;
        res["ha_comprado_este_producto"] = !comprasProdArray.isEmpty();
        res["total_veces_comprado"] = comprasProdArray.size();
        res["total_unidades_compradas"] = unidadesTotales;
        res["gasto_total_en_producto"] = QString::number(gastoTotal, 'f', 2) + " €";
        if (!comprasProdArray.isEmpty()) {
            res["ultima_compra_fecha"] = comprasProdArray.first().toObject().value("fecha").toString();
            res["primera_compra_fecha"] = comprasProdArray.last().toObject().value("fecha").toString();
        }
        res["historial_compras_del_producto"] = comprasProdArray;
        return res;
    }

    // 2. Obtener ranking de artículos más comprados por el cliente
    QJsonArray topArticulosArray;
    {
        QSqlQuery qTop(db);
        QString sqlTop;
        if (esNube) {
            sqlTop = "SELECT l.cod, l.descripcion, SUM(l.cantidad) as total_unidades, SUM(l.totallinea) as total_euros, "
                     "COUNT(DISTINCT t.ticket) as num_tickets_comprado "
                     "FROM lineasticket_nube l "
                     "JOIN tickets_nube t ON l.id_tienda = t.id_tienda AND l.nticket = t.ticket "
                     "WHERE t.cliente = :idCli AND l.cod != '9999999999999' ";
            if (!fechaI.isEmpty()) sqlTop += "AND l.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlTop += "AND l.fecha <= :f2 ";
            sqlTop += "GROUP BY l.cod, l.descripcion "
                      "ORDER BY total_unidades DESC LIMIT :limite";
        } else {
            sqlTop = "SELECT l.cod, l.descripcion, SUM(l.cantidad) as total_unidades, SUM(l.totallinea) as total_euros, "
                     "COUNT(DISTINCT t.ticket) as num_tickets_comprado "
                     "FROM lineasticket l "
                     "JOIN tickets t ON l.nticket = t.ticket "
                     "WHERE t.cliente = :idCli AND l.cod != '9999999999999' ";
            if (!fechaI.isEmpty()) sqlTop += "AND l.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlTop += "AND l.fecha <= :f2 ";
            sqlTop += "GROUP BY l.cod, l.descripcion "
                      "ORDER BY total_unidades DESC LIMIT :limite";
        }
        qTop.prepare(sqlTop);
        qTop.bindValue(":idCli", idCliente);
        if (!fechaI.isEmpty()) qTop.bindValue(":f1", fechaI);
        if (!fechaF.isEmpty()) qTop.bindValue(":f2", fechaF);
        qTop.bindValue(":limite", limite);
        if (qTop.exec()) {
            int pos = 1;
            while (qTop.next()) {
                QJsonObject art;
                art["posicion"] = pos++;
                art["codigo"] = qTop.value("cod").toString();
                art["descripcion"] = qTop.value("descripcion").toString();
                art["unidades_totales_compradas"] = qTop.value("total_unidades").toDouble();
                art["gasto_total"] = QString::number(qTop.value("total_euros").toDouble(), 'f', 2) + " €";
                art["veces_comprado"] = qTop.value("num_tickets_comprado").toInt();
                topArticulosArray.append(art);
            }
        }
    }

    // 3. Obtener últimos tickets
    QJsonArray ticketsArray;
    {
        QSqlQuery qT(db);
        QString sqlT;
        if (esNube) {
            sqlT = "SELECT t.id_tienda, t.ticket, DATE_FORMAT(t.fecha, '%Y-%m-%d') as fecha_fmt, t.hora, t.total, "
                   "COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) as nombre_tienda "
                   "FROM tickets_nube t "
                   "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                   "WHERE t.cliente = :idCli ORDER BY t.fecha DESC, t.hora DESC LIMIT 5";
        } else {
            sqlT = "SELECT t.ticket, DATE_FORMAT(t.fecha, '%Y-%m-%d') as fecha_fmt, t.hora, t.total, 'Local' as nombre_tienda "
                   "FROM tickets t WHERE t.cliente = :idCli ORDER BY t.fecha DESC, t.hora DESC LIMIT 5";
        }
        qT.prepare(sqlT);
        qT.bindValue(":idCli", idCliente);
        if (qT.exec()) {
            while (qT.next()) {
                QJsonObject tkt;
                int nTicket = qT.value("ticket").toInt();
                tkt["ticket"] = nTicket;
                if (esNube) tkt["id_tienda"] = qT.value("id_tienda").toInt();
                tkt["tienda"] = qT.value("nombre_tienda").toString();
                tkt["fecha"] = qT.value("fecha_fmt").toString();
                tkt["hora"] = qT.value("hora").toString();
                tkt["total"] = QString::number(qT.value("total").toDouble(), 'f', 2) + " €";
                ticketsArray.append(tkt);
            }
        }
    }

    res["id_cliente"] = idCliente;
    res["cliente_nombre"] = nombreCompleto;
    res["articulos_mas_comprados"] = topArticulosArray;
    res["ultimos_tickets"] = ticketsArray;
    return res;
}

struct AreaSaludNatural {
    QString categoria;
    QStringList detonantes;
    QStringList terminosClave;
};

static QList<AreaSaludNatural> g_cacheConocimiento;
static bool g_conocimientoCargado = false;

/**
 * @brief Obtiene la base de conocimiento activa desde la base de datos (con caché y fallback).
 */
static QList<AreaSaludNatural> obtenerConocimientoFitoterapia()
{
    if (g_conocimientoCargado && !g_cacheConocimiento.isEmpty()) {
        return g_cacheConocimiento;
    }

    g_cacheConocimiento.clear();

    QSqlDatabase db = QSqlDatabase::database();
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    } else {
        QString connLocal = conf ? conf->getConexionLocal() : "DB";
        if (connLocal.isEmpty()) connLocal = "DB";
        if (QSqlDatabase::contains(connLocal)) {
            db = QSqlDatabase::database(connLocal);
        }
    }

    if (db.isOpen()) {
        QSqlQuery q(db);
        if (q.exec("SELECT categoria, detonantes, terminos_clave FROM ia_conocimiento WHERE activo = 1 ORDER BY id ASC")) {
            while (q.next()) {
                AreaSaludNatural a;
                a.categoria = q.value("categoria").toString();

                QString detStr = q.value("detonantes").toString();
                for (const QString &d : detStr.split(",", Qt::SkipEmptyParts)) {
                    QString limpio = d.trimmed().toLower();
                    if (!limpio.isEmpty()) a.detonantes.append(limpio);
                }

                QString termStr = q.value("terminos_clave").toString();
                for (const QString &t : termStr.split(",", Qt::SkipEmptyParts)) {
                    QString limpio = t.trimmed().toLower();
                    if (!limpio.isEmpty()) a.terminosClave.append(limpio);
                }

                if (!a.detonantes.isEmpty() && !a.terminosClave.isEmpty()) {
                    g_cacheConocimiento.append(a);
                }
            }
        }
    }

    // Fallback con valores predeterminados de fitoterapia si la BD está vacía
    if (g_cacheConocimiento.isEmpty()) {
        struct DefaultEntry { const char *cat; const char *det; const char *term; };
        static const DefaultEntry DEFAULTS[] = {
            {"Articulaciones", "articul, hueso, dolor, rodilla, espalda, lumbar, artrosis, artritis, cartilag, reuma, tendon, bursitis, tendinitis, fascitis, ciatica, esguince", "colamag, curcurina, artripol, cannalges, colagen, curcuma, harpagofit, glucosamin, msm, membrana, silici, magnesi"},
            {"Colesterol", "colesterol, triglicerid, lipido, cardio, corazon, arterial, tension", "colestia, lecidol, ometrix, nivelcol, cardiepa, monacolina, levadura, arroz rojo, bergamota, omega, fitosterol, ajo negro"},
            {"Digestión", "digest, pesadez, gas, vientre, hinchazon, higado, hepatic, vesicula, reflujo, ardor, acidez, estomago, transito, estrenimient", "carbomag, despatic, drenadiet, alivia tus gases, como un reloj, vientre plano, alcachofa, cardo mariano, desmodium, boldo, hinojo, probiotico, carbon vegetal, plantago"},
            {"Inmunidad / Garganta", "resfriad, gripe, tos, garganta, mucus, mucosidad, congestion, defensa, inmune, inmunidad, catarro, afon", "propol, echinacea, equinacea, tomillo, drosera, malvavisco, eucalipt, llanten, vitamina c, reishi, inmuno, bronpul"},
            {"Sueño / Nervios", "dormir, insomni, sueno, nervio, ansiedad, estres, relaj, depres, animo", "sedaner, nervaplant, melatonina, valeriana, pasiflora, amapola, ashwagandha, triptofano, gaba, melisa, hiperico"},
            {"Circulación", "circulac, piernas cansadas, pesadez piernas, varices, hemorroid, retencion, celulit", "circular, flebodiet, ginkgo, castaño de indias, vid roja, rusco, hamamelis, centella, cola caballo, drenadiet"},
            {"Vías Urinarias", "cistitis, orina, urinari, prostata, infeccion orina, arandano", "arandano rojo, cranberry, d-manosa, gayuba, sabal, epilobio, calabaza"},
            {"Control de Peso", "adelgazar, peso, grasa, quemagrasa, sacian, dieta, retencion liquidos", "drenadiet, controla tu peso, carnitina, glucomanano, garcinia, te verde, fucus, chitosan"},
            {"Memoria / Energía", "memoria, estudio, concentrac, cansanci, fatiga, agotamient, energia, vitalidad", "aktiner, jalea, ginseng, eleuterococo, bacopa, fosfatidilserina, coenzima q10, guarana, maca"},
            {"Menopausia", "menopaus, sofocos, regla, menstrua, ciclo, hormon", "isoflavonas, cimicifuga, onagra, salvia, sauzgatillo, probidiet intimo"}
        };
        for (const auto &d : DEFAULTS) {
            AreaSaludNatural a;
            a.categoria = QString::fromUtf8(d.cat);
            for (const QString &det : QString::fromUtf8(d.det).split(",", Qt::SkipEmptyParts)) a.detonantes.append(det.trimmed().toLower());
            for (const QString &term : QString::fromUtf8(d.term).split(",", Qt::SkipEmptyParts)) a.terminosClave.append(term.trimmed().toLower());
            g_cacheConocimiento.append(a);
        }
    }

    g_conocimientoCargado = true;
    return g_cacheConocimiento;
}

void AsistenteIA::recargarConocimiento()
{
    g_conocimientoCargado = false;
    obtenerConocimientoFitoterapia();
    qDebug() << "AsistenteIA: Base de conocimiento recargada con" << g_cacheConocimiento.size() << "categorías activas.";
}

QJsonObject AsistenteIA::toolBuscarPorSintoma(const QJsonObject &args)
{
    QString indicacion = args.value("indicacion").toString().trimmed();
    if (indicacion.isEmpty()) indicacion = args.value("sintoma").toString().trimmed();
    if (indicacion.isEmpty()) indicacion = args.value("symptom").toString().trimmed();
    if (indicacion.isEmpty()) indicacion = args.value("termino").toString().trimmed();
    if (indicacion.isEmpty()) indicacion = args.value("query").toString().trimmed();
    if (indicacion.isEmpty()) indicacion = args.value("producto").toString().trimmed();

    QJsonObject res;
    QJsonArray productosArray;

    if (indicacion.isEmpty()) {
        res["error"] = "Indicación o término de búsqueda vacío";
        return res;
    }

    QSqlDatabase db;
    bool esNube = false;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        esNube = true;
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "Base de datos no disponible";
        return res;
    }

    QString stockSubquery = esNube 
        ? "COALESCE((SELECT SUM(s.stock) FROM stock_tiendas_nube s WHERE s.cod = a.cod), 0)"
        : "COALESCE((SELECT SUM(l.cantidad) FROM lotes l WHERE l.ean = a.cod), 0)";

    // 1. Extraer tokens limpios sin stop words
    static const QSet<QString> stopWords = {
        "para", "el", "la", "los", "las", "un", "una", "unos", "unas", "de", "del",
        "al", "o", "y", "en", "con", "por", "sobre", "que", "se", "lo", "como", "sin",
        "mas", "menos", "este", "esta", "estos", "estas", "algo", "algun", "alguna",
        "tenemos", "tienes", "hay", "buscar", "buscame", "dame", "producto", "productos", "llevan", "contienen", "indicados", "indicado"
    };

    QString limpio = indicacion.toLower();
    limpio.replace(QRegularExpression("[^a-záéíóúüñ0-9]+"), " ");
    QStringList palabras = limpio.split(" ", Qt::SkipEmptyParts);
    QStringList tokens;
    for (const QString &p : palabras) {
        if (!stopWords.contains(p) && p.length() >= 3) {
            tokens.append(p);
        }
    }
    if (tokens.isEmpty() && !indicacion.trimmed().isEmpty()) {
        tokens.append(indicacion.trimmed());
    }

    // 2. Multi-factor Scoring:
    // - Coincidencia de token en Nombre del producto (a.descripcion): +50 pts
    // - Coincidencia de token en Composición / Indicaciones (a.notas): +30 pts
    // - Coincidencia de token en Familia de producto (f.descripcion): +20 pts
    // - Coincidencia de token en Fabricante / Marca (b.nombre): +10 pts
    // - Coincidencia con Áreas de Fitoterapia: Si el token activa un área de salud, sus principios reciben bono de +15 pts en nombre y +10 pts en notas.

    QStringList areaBonusTerms;
    QList<AreaSaludNatural> areas = obtenerConocimientoFitoterapia();
    for (const AreaSaludNatural &area : areas) {
        bool areaActiva = false;
        for (const QString &det : area.detonantes) {
            for (const QString &t : tokens) {
                if (t.contains(det) || det.contains(t)) {
                    areaActiva = true;
                    break;
                }
            }
            if (areaActiva) break;
        }
        if (areaActiva) {
            for (const QString &tk : area.terminosClave) {
                if (!areaBonusTerms.contains(tk)) areaBonusTerms.append(tk);
            }
        }
    }

    QStringList whereConditions;
    QStringList scoreExpressions;

    for (int i = 0; i < tokens.size(); ++i) {
        whereConditions.append(QString("(a.descripcion LIKE :tk_desc_%1 OR a.notas LIKE :tk_notas_%1 OR f.descripcion LIKE :tk_fam_%1 OR b.nombre LIKE :tk_fab_%1)").arg(i));
        scoreExpressions.append(QString("(CASE WHEN a.descripcion LIKE :tk_s_desc_%1 THEN 50 ELSE 0 END)").arg(i));
        scoreExpressions.append(QString("(CASE WHEN a.notas LIKE :tk_s_notas_%1 THEN 30 ELSE 0 END)").arg(i));
        scoreExpressions.append(QString("(CASE WHEN f.descripcion LIKE :tk_s_fam_%1 THEN 20 ELSE 0 END)").arg(i));
        scoreExpressions.append(QString("(CASE WHEN b.nombre LIKE :tk_s_fab_%1 THEN 10 ELSE 0 END)").arg(i));
    }

    for (int j = 0; j < areaBonusTerms.size(); ++j) {
        whereConditions.append(QString("(a.descripcion LIKE :ab_desc_%1 OR a.notas LIKE :ab_notas_%1)").arg(j));
        scoreExpressions.append(QString("(CASE WHEN a.descripcion LIKE :ab_s_desc_%1 THEN 15 WHEN a.notas LIKE :ab_s_notas_%1 THEN 10 ELSE 0 END)").arg(j));
    }

    QString sql = QString(
        "SELECT a.cod, a.descripcion, a.pvp, a.notas, "
        "COALESCE(b.nombre, '') as fabricante, "
        "COALESCE(f.descripcion, '') as familia, "
        "%1 as stock_real, "
        "(%2) as relevancia "
        "FROM articulos a "
        "LEFT JOIN familias f ON a.familia = f.id "
        "LEFT JOIN fabricantes b ON a.fabricante = b.id "
        "WHERE %3 "
        "HAVING relevancia > 0 "
        "ORDER BY relevancia DESC, stock_real DESC "
        "LIMIT 12").arg(stockSubquery, scoreExpressions.join(" + "), whereConditions.join(" OR "));

    QSqlQuery q(db);
    q.prepare(sql);
    for (int i = 0; i < tokens.size(); ++i) {
        QString v = "%" + tokens[i] + "%";
        q.bindValue(QString(":tk_desc_%1").arg(i), v);
        q.bindValue(QString(":tk_notas_%1").arg(i), v);
        q.bindValue(QString(":tk_fam_%1").arg(i), v);
        q.bindValue(QString(":tk_fab_%1").arg(i), v);
        q.bindValue(QString(":tk_s_desc_%1").arg(i), v);
        q.bindValue(QString(":tk_s_notas_%1").arg(i), v);
        q.bindValue(QString(":tk_s_fam_%1").arg(i), v);
        q.bindValue(QString(":tk_s_fab_%1").arg(i), v);
    }
    for (int j = 0; j < areaBonusTerms.size(); ++j) {
        QString vb = "%" + areaBonusTerms[j] + "%";
        q.bindValue(QString(":ab_desc_%1").arg(j), vb);
        q.bindValue(QString(":ab_notas_%1").arg(j), vb);
        q.bindValue(QString(":ab_s_desc_%1").arg(j), vb);
        q.bindValue(QString(":ab_s_notas_%1").arg(j), vb);
    }

    if (q.exec()) {
        while (q.next()) {
            QJsonObject prod;
            prod["codigo"] = q.value("cod").toString();
            prod["producto"] = q.value("descripcion").toString();
            prod["nombre"] = q.value("descripcion").toString();
            prod["pvp"] = QString::number(q.value("pvp").toDouble(), 'f', 2) + " €";
            prod["stock"] = q.value("stock_real").toDouble();
            prod["fabricante"] = q.value("fabricante").toString();
            prod["familia"] = q.value("familia").toString();
            prod["notas"] = limpiarTextoHtml(q.value("notas").toString(), 180);
            productosArray.append(prod);
        }
    }

    res["tipo_resultado"] = "catalogo_fitoterapia_indicaciones";
    res["indicacion_buscada"] = indicacion;
    res["total_encontrados"] = productosArray.size();
    res["productos"] = productosArray;
    return res;
}

/**
 * @brief Obtiene el ranking de artículos más vendidos con filtros por fecha, fabricante, familia o tienda.
 */
QJsonObject AsistenteIA::toolProductosMasVendidos(const QJsonObject &args)
{
    QJsonObject res;

    // Validación de permisos
    if (conf && conf->permisos() && !conf->permisos()->tiene("estadisticas") && !conf->permisos()->tiene("articulos")) {
        res["error"] = "Permiso denegado: El usuario activo no tiene permiso para consultar estadísticas de ventas.";
        return res;
    }

    QDate hoy = QDate::currentDate();
    QString inicioMes = QDate(hoy.year(), hoy.month(), 1).toString("yyyy-MM-dd");
    QString finMes = QDate(hoy.year(), hoy.month(), hoy.daysInMonth()).toString("yyyy-MM-dd");

    int ano = args.value("ano").toInt();
    if (ano <= 0) ano = args.value("year").toInt();
    if (ano <= 0) ano = args.value("anyo").toInt();

    // 1. Extraer año si venía en el historial de usuario
    if (ano <= 0 && args.value("fecha_inicio").toString().isEmpty()) {
        static const QRegularExpression regAno("\\b(20[123]\\d)\\b");
        for (int idx = m_historial.size() - 1; idx >= 0; --idx) {
            QJsonObject msgObj = m_historial[idx].toObject();
            if (msgObj.value("role").toString() == "user") {
                QString txt = msgObj.value("content").toString().toLower();
                if (txt.contains("este año") || txt.contains("año actual")) {
                    ano = hoy.year();
                    break;
                }
                QRegularExpressionMatch mUser = regAno.match(txt);
                if (mUser.hasMatch()) {
                    ano = mUser.captured(1).toInt();
                    break;
                }
            }
        }
    }

    QString fechaI = args.value("fecha_inicio").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("date_from").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("start_date").toString().trimmed();
    if (fechaI.isEmpty()) fechaI = args.value("desde").toString().trimmed();
    if (fechaI.isEmpty()) {
        if (ano > 2000) {
            fechaI = QString("%1-01-01").arg(ano);
        } else {
            fechaI = inicioMes;
        }
    }

    QString fechaF = args.value("fecha_fin").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("date_to").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("end_date").toString().trimmed();
    if (fechaF.isEmpty()) fechaF = args.value("hasta").toString().trimmed();
    if (fechaF.isEmpty()) {
        if (ano > 2000) {
            fechaF = QString("%1-12-31").arg(ano);
        } else {
            fechaF = finMes;
        }
    }

    QString fabricante = args.value("fabricante").toString().trimmed();
    if (fabricante.isEmpty()) fabricante = args.value("marca").toString().trimmed();
    if (fabricante.isEmpty()) fabricante = args.value("brand").toString().trimmed();
    if (fabricante.isEmpty()) fabricante = args.value("proveedor").toString().trimmed();

    // 2. Extraer fabricante del historial si no se pasó y el usuario preguntaba por una marca
    if (fabricante.isEmpty()) {
        for (int idx = m_historial.size() - 1; idx >= 0; --idx) {
            QJsonObject msgObj = m_historial[idx].toObject();
            if (msgObj.value("role").toString() == "user") {
                QString txt = msgObj.value("content").toString();
                if (txt.contains("kenoi", Qt::CaseInsensitive)) { fabricante = "Kenoi"; break; }
                if (txt.contains("nova diet", Qt::CaseInsensitive) || txt.contains("novadiet", Qt::CaseInsensitive)) { fabricante = "Nova Diet"; break; }
                if (txt.contains("santiveri", Qt::CaseInsensitive)) { fabricante = "Santiveri"; break; }
                if (txt.contains("soria natural", Qt::CaseInsensitive)) { fabricante = "Soria Natural"; break; }
                if (txt.contains("sotya", Qt::CaseInsensitive)) { fabricante = "Sotya"; break; }
            }
        }
    }

    QString familia = args.value("familia").toString().trimmed();
    bool familiaNegativa = false;
    QString familiaNorm = familia.toLower();
    if (familiaNorm.startsWith("no ") || familiaNorm.startsWith("sin ") || familiaNorm.contains("distinto") || familiaNorm.contains("no aliment") || familiaNorm.contains("no-aliment")) {
        familiaNegativa = true;
        if (familiaNorm.contains("aliment")) {
            familia = "alimenta";
        } else {
            familia = familia.mid(3).trimmed();
        }
    }

    QString termino = args.value("termino").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("producto").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("articulo").toString().trimmed();

    int limite = args.value("limite").toInt();
    if (limite <= 0) limite = args.value("limit").toInt(10);
    if (limite <= 0 || limite > 50) limite = 10;

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;
    else if (tNorm == "local" || tNorm.contains("sucursal")) idTiendaFiltro = (conf ? conf->getIdTienda() : 4);
    else if (tNorm != "todas" && tNorm != "all" && tNorm != "red" && !tNorm.isEmpty()) {
        // La IA confundió 'fabricante' o 'producto' y lo pasó en el argumento 'tienda' (ej. tienda: "Santiveri")
        if (fabricante.isEmpty()) {
            fabricante = tiendaFiltro;
        } else if (termino.isEmpty()) {
            termino = tiendaFiltro;
        }
        tiendaFiltro = "todas";
        idTiendaFiltro = 0;
    }

    bool desglosarMeses = args.value("desglosar_por_mes").toBool(false);
    if (!desglosarMeses) desglosarMeses = args.value("por_meses").toBool(false);
    if (!desglosarMeses) desglosarMeses = args.value("cada_mes").toBool(false);
    if (!desglosarMeses) desglosarMeses = args.value("monthly").toBool(false);

    bool consultadoNube = false;
    QJsonArray rankingArray;
    double totalUnidadesGlobal = 0.0;
    double totalEurosGlobal = 0.0;
    int totalReferenciasGlobal = 0;
    QJsonArray totalesTiendasArray;

    // 1. Intentar consultar directamente la base consolidada en la nube si está conectada
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        
        // 1.1 Si se solicita o detecta desglose mensual (ej. 'cada mes de 2025')
        if (desglosarMeses) {
            QSqlQuery qM(dbNube);
            QString sqlM = "SELECT DATE_FORMAT(fecha, '%Y-%m') AS mes, "
                           "codigo_articulo, producto, SUM(cantidad) AS unidades, SUM(total) AS total_euros "
                           "FROM vista_ventas_detalladas "
                           "WHERE fecha >= :f1 AND fecha <= :f2 ";
            if (idTiendaFiltro > 0) {
                sqlM += QString("AND id_tienda = %1 ").arg(idTiendaFiltro);
            }
            if (!fabricante.isEmpty()) sqlM += "AND (fabricante LIKE :fab OR producto LIKE :fab2) ";
            if (!familia.isEmpty()) {
                if (familiaNegativa) sqlM += "AND (familia NOT LIKE :fam OR familia IS NULL) ";
                else sqlM += "AND (familia LIKE :fam OR producto LIKE :fam2) ";
            }
            if (!termino.isEmpty()) sqlM += "AND (producto LIKE :term OR codigo_articulo = :term2) ";
            sqlM += "GROUP BY mes, codigo_articulo, producto "
                    "ORDER BY mes ASC, unidades DESC";

            qM.prepare(sqlM);
            qM.bindValue(":f1", fechaI);
            qM.bindValue(":f2", fechaF);
            if (!fabricante.isEmpty()) {
                qM.bindValue(":fab", "%" + fabricante + "%");
                qM.bindValue(":fab2", "%" + fabricante + "%");
            }
            if (!familia.isEmpty()) {
                qM.bindValue(":fam", "%" + familia + "%");
                if (!familiaNegativa) qM.bindValue(":fam2", "%" + familia + "%");
            }
            if (!termino.isEmpty()) {
                qM.bindValue(":term", "%" + termino + "%");
                qM.bindValue(":term2", termino);
            }

            if (qM.exec()) {
                consultadoNube = true;
                QMap<QString, QJsonArray> mesesMap;
                QMap<QString, int> conteoMes;
                while (qM.next()) {
                    QString mes = qM.value("mes").toString();
                    int cnt = conteoMes.value(mes, 0);
                    if (cnt < limite) {
                        conteoMes[mes] = cnt + 1;
                        QJsonObject item;
                        item["posicion"] = cnt + 1;
                        item["mes"] = mes;
                        item["producto"] = qM.value("producto").toString();
                        item["codigo"] = qM.value("codigo_articulo").toString();
                        item["unidades_vendidas"] = qM.value("unidades").toDouble();
                        item["total_facturado"] = QString::number(qM.value("total_euros").toDouble(), 'f', 2) + " €";
                        mesesMap[mes].append(item);
                    }
                }

                QJsonObject mesesObj;
                for (auto it = mesesMap.begin(); it != mesesMap.end(); ++it) {
                    mesesObj[it.key()] = it.value();
                }
                res["ranking_por_meses"] = mesesObj;
            }
        }

        // 1.2 Totales globales del filtro (sin límite de ranking)
        {
            QSqlQuery qTot(dbNube);
            QString sqlTot = "SELECT SUM(l.cantidad) as total_unidades, SUM(l.totallinea) as total_euros, "
                             "COUNT(DISTINCT l.cod) as total_refs "
                             "FROM lineasticket_nube l "
                             "LEFT JOIN articulos a ON l.cod = a.cod "
                             "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                             "LEFT JOIN familias f ON a.familia = f.id "
                             "WHERE l.fecha >= :f1 AND l.fecha <= :f2 ";
            if (idTiendaFiltro > 0) {
                sqlTot += QString("AND l.id_tienda = %1 ").arg(idTiendaFiltro);
            }
            if (!fabricante.isEmpty()) sqlTot += "AND (b.nombre LIKE :fab OR l.descripcion LIKE :fab2) ";
            if (!familia.isEmpty()) {
                if (familiaNegativa) sqlTot += "AND (f.descripcion NOT LIKE :fam OR f.descripcion IS NULL) ";
                else sqlTot += "AND (f.descripcion LIKE :fam OR l.descripcion LIKE :fam2) ";
            }
            if (!termino.isEmpty()) sqlTot += "AND (l.descripcion LIKE :term OR l.cod = :term2) ";

            qTot.prepare(sqlTot);
            qTot.bindValue(":f1", fechaI);
            qTot.bindValue(":f2", fechaF);
            if (!fabricante.isEmpty()) {
                qTot.bindValue(":fab", "%" + fabricante + "%");
                qTot.bindValue(":fab2", "%" + fabricante + "%");
            }
            if (!familia.isEmpty()) {
                qTot.bindValue(":fam", "%" + familia + "%");
                if (!familiaNegativa) qTot.bindValue(":fam2", "%" + familia + "%");
            }
            if (!termino.isEmpty()) {
                qTot.bindValue(":term", "%" + termino + "%");
                qTot.bindValue(":term2", termino);
            }
            if (qTot.exec() && qTot.next()) {
                totalUnidadesGlobal = qTot.value("total_unidades").toDouble();
                totalEurosGlobal = qTot.value("total_euros").toDouble();
                totalReferenciasGlobal = qTot.value("total_refs").toInt();
            }
        }

        QJsonArray totalesTiendasArray;
        if (idTiendaFiltro == 0) {
            QSqlQuery qTotT(dbNube);
            QString sqlTotT = "SELECT l.id_tienda, COALESCE(t.nombre, CONCAT('Tienda ', l.id_tienda)) as tienda, "
                              "SUM(l.cantidad) as total_unidades, SUM(l.totallinea) as total_euros "
                              "FROM lineasticket_nube l "
                              "LEFT JOIN articulos a ON l.cod = a.cod "
                              "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                              "LEFT JOIN familias f ON a.familia = f.id "
                              "LEFT JOIN tiendas t ON l.id_tienda = t.id "
                              "WHERE l.fecha >= :f1 AND l.fecha <= :f2 ";
            if (!fabricante.isEmpty()) sqlTotT += "AND (b.nombre LIKE :fab OR l.descripcion LIKE :fab2) ";
            if (!familia.isEmpty()) {
                if (familiaNegativa) sqlTotT += "AND (f.descripcion NOT LIKE :fam OR f.descripcion IS NULL) ";
                else sqlTotT += "AND (f.descripcion LIKE :fam OR l.descripcion LIKE :fam2) ";
            }
            if (!termino.isEmpty()) sqlTotT += "AND (l.descripcion LIKE :term OR l.cod = :term2) ";
            sqlTotT += "GROUP BY l.id_tienda, t.nombre ORDER BY total_euros DESC";

            qTotT.prepare(sqlTotT);
            qTotT.bindValue(":f1", fechaI);
            qTotT.bindValue(":f2", fechaF);
            if (!fabricante.isEmpty()) {
                qTotT.bindValue(":fab", "%" + fabricante + "%");
                qTotT.bindValue(":fab2", "%" + fabricante + "%");
            }
            if (!familia.isEmpty()) {
                qTotT.bindValue(":fam", "%" + familia + "%");
                if (!familiaNegativa) qTotT.bindValue(":fam2", "%" + familia + "%");
            }
            if (!termino.isEmpty()) {
                qTotT.bindValue(":term", "%" + termino + "%");
                qTotT.bindValue(":term2", termino);
            }
            if (qTotT.exec()) {
                while (qTotT.next()) {
                    QJsonObject stTot;
                    stTot["tienda"] = qTotT.value("tienda").toString();
                    stTot["total_unidades"] = qTotT.value("total_unidades").toDouble();
                    stTot["total_facturado"] = QString::number(qTotT.value("total_euros").toDouble(), 'f', 2) + " €";
                    totalesTiendasArray.append(stTot);
                }
            }
        }

        QSqlQuery q(dbNube);
        QString sql = "SELECT l.cod, l.descripcion, SUM(l.cantidad) as total_unidades, "
                      "SUM(l.totallinea) as total_euros, "
                      "COALESCE(b.nombre, '') as fabricante, COALESCE(f.descripcion, '') as familia "
                      "FROM lineasticket_nube l "
                      "LEFT JOIN articulos a ON l.cod = a.cod "
                      "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                      "LEFT JOIN familias f ON a.familia = f.id "
                      "WHERE l.fecha >= :f1 AND l.fecha <= :f2 ";
        if (idTiendaFiltro > 0) {
            sql += QString("AND l.id_tienda = %1 ").arg(idTiendaFiltro);
        }
        if (!fabricante.isEmpty()) sql += "AND (b.nombre LIKE :fab OR l.descripcion LIKE :fab2) ";
        if (!familia.isEmpty()) {
            if (familiaNegativa) sql += "AND (f.descripcion NOT LIKE :fam OR f.descripcion IS NULL) ";
            else sql += "AND (f.descripcion LIKE :fam OR l.descripcion LIKE :fam2) ";
        }
        if (!termino.isEmpty()) sql += "AND (l.descripcion LIKE :term OR l.cod = :term2) ";
        sql += "GROUP BY l.cod, l.descripcion, b.nombre, f.descripcion "
               "ORDER BY total_unidades DESC LIMIT :limite";

        q.prepare(sql);
        q.bindValue(":f1", fechaI);
        q.bindValue(":f2", fechaF);
        if (!fabricante.isEmpty()) {
            q.bindValue(":fab", "%" + fabricante + "%");
            q.bindValue(":fab2", "%" + fabricante + "%");
        }
        if (!familia.isEmpty()) {
            q.bindValue(":fam", "%" + familia + "%");
            if (!familiaNegativa) q.bindValue(":fam2", "%" + familia + "%");
        }
        if (!termino.isEmpty()) {
            q.bindValue(":term", "%" + termino + "%");
            q.bindValue(":term2", termino);
        }
        q.bindValue(":limite", limite);

        if (q.exec()) {
            consultadoNube = true;
            int pos = 1;
            while (q.next()) {
                QJsonObject item;
                item["posicion"] = pos++;
                item["codigo"] = q.value("cod").toString();
                item["nombre"] = q.value("descripcion").toString();
                item["unidades_vendidas"] = q.value("total_unidades").toDouble();
                item["total_facturado"] = QString::number(q.value("total_euros").toDouble(), 'f', 2) + " €";
                item["fabricante"] = q.value("fabricante").toString();
                item["familia"] = q.value("familia").toString();
                rankingArray.append(item);
            }

            // Desglose por cada tienda si se consultan todas
            if (idTiendaFiltro == 0) {
                QSqlQuery qT(dbNube);
                QString sqlT = "SELECT l.id_tienda, COALESCE(t.nombre, CONCAT('Tienda ', l.id_tienda)) as tienda, "
                               "l.cod, l.descripcion, SUM(l.cantidad) as total_unidades, SUM(l.totallinea) as total_euros "
                               "FROM lineasticket_nube l "
                               "LEFT JOIN articulos a ON l.cod = a.cod "
                               "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                               "LEFT JOIN familias f ON a.familia = f.id "
                               "LEFT JOIN tiendas t ON l.id_tienda = t.id "
                               "WHERE l.fecha >= :f1 AND l.fecha <= :f2 ";
                if (!fabricante.isEmpty()) sqlT += "AND (b.nombre LIKE :fab OR l.descripcion LIKE :fab2) ";
                if (!familia.isEmpty()) {
                    if (familiaNegativa) sqlT += "AND (f.descripcion NOT LIKE :fam OR f.descripcion IS NULL) ";
                    else sqlT += "AND (f.descripcion LIKE :fam OR l.descripcion LIKE :fam2) ";
                }
                if (!termino.isEmpty()) sqlT += "AND (l.descripcion LIKE :term OR l.cod = :term2) ";
                sqlT += "GROUP BY l.id_tienda, t.nombre, l.cod, l.descripcion "
                        "ORDER BY l.id_tienda ASC, total_unidades DESC";

                qT.prepare(sqlT);
                qT.bindValue(":f1", fechaI);
                qT.bindValue(":f2", fechaF);
                if (!fabricante.isEmpty()) {
                    qT.bindValue(":fab", "%" + fabricante + "%");
                    qT.bindValue(":fab2", "%" + fabricante + "%");
                }
                if (!familia.isEmpty()) {
                    qT.bindValue(":fam", "%" + familia + "%");
                    if (!familiaNegativa) qT.bindValue(":fam2", "%" + familia + "%");
                }
                if (!termino.isEmpty()) {
                    qT.bindValue(":term", "%" + termino + "%");
                    qT.bindValue(":term2", termino);
                }

                if (qT.exec()) {
                    QMap<int, int> conteoPorTienda;
                    QMap<int, QString> nombresTienda;
                    QMap<int, QJsonArray> productosPorTienda;
                    QJsonArray desgloseTiendasTop1;

                    int maxItemsPorTienda = qBound(3, limite, 10);

                    while (qT.next()) {
                        int idTi = qT.value("id_tienda").toInt();
                        int actual = conteoPorTienda.value(idTi, 0);
                        if (actual < maxItemsPorTienda) {
                            conteoPorTienda[idTi] = actual + 1;
                            nombresTienda[idTi] = qT.value("tienda").toString();

                            QJsonObject pItem;
                            pItem["posicion"] = actual + 1;
                            pItem["producto"] = qT.value("descripcion").toString();
                            pItem["codigo"] = qT.value("cod").toString();
                            pItem["unidades_vendidas"] = qT.value("total_unidades").toDouble();
                            pItem["total_facturado"] = QString::number(qT.value("total_euros").toDouble(), 'f', 2) + " €";
                            
                            productosPorTienda[idTi].append(pItem);

                            if (actual == 0) {
                                QJsonObject tItem = pItem;
                                tItem["tienda"] = qT.value("tienda").toString();
                                desgloseTiendasTop1.append(tItem);
                            }
                        }
                    }

                    if (!desgloseTiendasTop1.isEmpty()) {
                        res["producto_mas_vendido_en_cada_tienda"] = desgloseTiendasTop1;
                    }

                    QJsonArray rankingPorTiendaArray;
                    for (auto it = productosPorTienda.begin(); it != productosPorTienda.end(); ++it) {
                        QJsonObject storeObj;
                        storeObj["id_tienda"] = it.key();
                        storeObj["tienda"] = nombresTienda.value(it.key());
                        storeObj["top_productos"] = it.value();
                        rankingPorTiendaArray.append(storeObj);
                    }
                    if (!rankingPorTiendaArray.isEmpty()) {
                        res["ranking_por_tienda"] = rankingPorTiendaArray;
                    }
                }
            }
        } else {
            qDebug() << "toolProductosMasVendidos nube error:" << q.lastError().text();
        }
    }

    // 2. Si no se consultó por nube, consultar bases locales / remotas activas
    if (!consultadoNube) {
        QStringList conexiones = resolverConexiones(tiendaFiltro);
        struct ProdVendido {
            QString cod;
            QString nombre;
            double unidades = 0.0;
            double euros = 0.0;
            QString fabricante;
            QString familia;
        };
        QMap<QString, ProdVendido> acumulado;

        for (const QString &connName : conexiones) {
            if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) continue;
            QSqlDatabase db = QSqlDatabase::database(connName);
            QSqlQuery q(db);
            QString sql = "SELECT l.cod, l.descripcion, SUM(l.cantidad) as total_unidades, "
                          "SUM(l.totallinea) as total_euros, "
                          "COALESCE(b.nombre, '') as fabricante, COALESCE(f.descripcion, '') as familia "
                          "FROM lineasticket l "
                          "LEFT JOIN articulos a ON l.cod = a.cod "
                          "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                          "LEFT JOIN familias f ON a.familia = f.id "
                          "WHERE l.fecha >= :f1 AND l.fecha <= :f2 ";
            if (!fabricante.isEmpty()) sql += "AND (b.nombre LIKE :fab OR l.descripcion LIKE :fab2) ";
            if (!familia.isEmpty()) sql += "AND (f.descripcion LIKE :fam OR l.descripcion LIKE :fam2) ";
            if (!termino.isEmpty()) sql += "AND (l.descripcion LIKE :term OR l.cod = :term2) ";
            sql += "GROUP BY l.cod, l.descripcion, b.nombre, f.descripcion";

            q.prepare(sql);
            q.bindValue(":f1", fechaI);
            q.bindValue(":f2", fechaF);
            if (!fabricante.isEmpty()) {
                q.bindValue(":fab", "%" + fabricante + "%");
                q.bindValue(":fab2", "%" + fabricante + "%");
            }
            if (!familia.isEmpty()) {
                q.bindValue(":fam", "%" + familia + "%");
                q.bindValue(":fam2", "%" + familia + "%");
            }
            if (!termino.isEmpty()) {
                q.bindValue(":term", "%" + termino + "%");
                q.bindValue(":term2", termino);
            }

            if (q.exec()) {
                while (q.next()) {
                    QString cod = q.value("cod").toString();
                    if (!acumulado.contains(cod)) {
                        ProdVendido pv;
                        pv.cod = cod;
                        pv.nombre = q.value("descripcion").toString();
                        pv.fabricante = q.value("fabricante").toString();
                        pv.familia = q.value("familia").toString();
                        acumulado[cod] = pv;
                    }
                    acumulado[cod].unidades += q.value("total_unidades").toDouble();
                    acumulado[cod].euros += q.value("total_euros").toDouble();
                }
            }
        }

        QList<ProdVendido> lista = acumulado.values();
        std::sort(lista.begin(), lista.end(), [](const ProdVendido &a, const ProdVendido &b) {
            return a.unidades > b.unidades;
        });

        int n = qMin(limite, lista.size());
        for (int i = 0; i < n; ++i) {
            QJsonObject item;
            item["posicion"] = i + 1;
            item["codigo"] = lista[i].cod;
            item["nombre"] = lista[i].nombre;
            item["unidades_vendidas"] = lista[i].unidades;
            item["total_facturado"] = QString::number(lista[i].euros, 'f', 2) + " €";
            item["fabricante"] = lista[i].fabricante;
            item["familia"] = lista[i].familia;
            rankingArray.append(item);
        }
    }

    res["tienda"] = tiendaFiltro;
    res["fecha_inicio"] = fechaI;
    res["fecha_fin"] = fechaF;
    if (!fabricante.isEmpty()) res["fabricante_filtrado"] = fabricante;
    if (!familia.isEmpty()) res["familia_filtrada"] = familia;
    res["total_unidades_acumuladas_todas_referencias"] = totalUnidadesGlobal;
    res["total_facturado_acumulado_todas_referencias"] = QString::number(totalEurosGlobal, 'f', 2) + " €";
    res["total_referencias_vendidas"] = totalReferenciasGlobal;
    if (!totalesTiendasArray.isEmpty()) {
        res["totales_facturacion_por_tienda"] = totalesTiendasArray;
    }
    if (!rankingArray.isEmpty()) {
        res["producto_numero_1_mas_vendido"] = rankingArray.first().toObject();
    }
    res["total_articulos_ranking"] = rankingArray.size();
    res["ranking"] = rankingArray;

    return res;
}

/**
 * @brief Compara periodos homólogos (ej. mes actual vs mismo mes año anterior) con cálculo exacto de variaciones.
 */
QJsonObject AsistenteIA::toolComparativaVentas(const QJsonObject &args)
{
    QJsonObject res;

    // Validación de permisos
    if (conf && conf->permisos() && !conf->permisos()->tiene("estadisticas") && !conf->permisos()->tiene("cajas")) {
        res["error"] = "Permiso denegado: El usuario activo no tiene permiso para consultar estadísticas o ventas.";
        return res;
    }

    QDate hoy = QDate::currentDate();
    QDate inicioMes = QDate(hoy.year(), hoy.month(), 1);

    QString fechaI_act = args.value("fecha_inicio").toString().trimmed();
    if (fechaI_act.isEmpty()) fechaI_act = args.value("start_date").toString().trimmed();
    if (fechaI_act.isEmpty()) fechaI_act = inicioMes.toString("yyyy-MM-dd");

    QString fechaF_act = args.value("fecha_fin").toString().trimmed();
    if (fechaF_act.isEmpty()) fechaF_act = args.value("end_date").toString().trimmed();
    if (fechaF_act.isEmpty()) fechaF_act = hoy.toString("yyyy-MM-dd");

    QDate dtI_act = QDate::fromString(fechaI_act, "yyyy-MM-dd");
    QDate dtF_act = QDate::fromString(fechaF_act, "yyyy-MM-dd");
    if (!dtI_act.isValid()) dtI_act = inicioMes;
    if (!dtF_act.isValid()) dtF_act = hoy;

    QString compararCon = args.value("comparar_con").toString().trimmed().toLower();
    QDate dtI_ant, dtF_ant;
    if (compararCon.contains("mes")) {
        dtI_ant = dtI_act.addMonths(-1);
        dtF_ant = dtF_act.addMonths(-1);
    } else {
        // Por defecto comparar con el año anterior (periodo homólogo)
        dtI_ant = dtI_act.addYears(-1);
        dtF_ant = dtF_act.addYears(-1);
    }

    QString fechaI_ant = dtI_ant.toString("yyyy-MM-dd");
    QString fechaF_ant = dtF_ant.toString("yyyy-MM-dd");

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;
    else if (tNorm == "local" || tNorm.contains("sucursal")) idTiendaFiltro = (conf ? conf->getIdTienda() : 4);

    struct DatosPeriodo {
        QString nombreTienda;
        int tickets = 0;
        double ventas = 0.0;
        double efectivo = 0.0;
        double tarjeta = 0.0;
    };

    QMap<int, DatosPeriodo> datosActual;
    QMap<int, DatosPeriodo> datosAnterior;

    QMap<int, QString> nombresTiendas;
    nombresTiendas[1] = "Emeicjac";
    nombresTiendas[2] = "Casablanca";
    nombresTiendas[3] = "Cervantes";
    nombresTiendas[4] = "Tienda Local";

    // 1. Consultar base consolidada de la nube si está disponible
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        
        // Consulta periodo actual
        {
            QSqlQuery q(dbNube);
            QString sql = "SELECT t.id_tienda, COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) as nombre_tienda, "
                          "COUNT(*) as num_tickets, COALESCE(SUM(t.total), 0) as total_ventas, "
                          "COALESCE(SUM(CASE WHEN t.fpago = 1 THEN t.total ELSE 0 END), 0) as total_efectivo, "
                          "COALESCE(SUM(CASE WHEN t.fpago != 1 THEN t.total ELSE 0 END), 0) as total_tarjeta "
                          "FROM tickets_nube t "
                          "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                          "WHERE t.fecha >= :f1 AND t.fecha <= :f2 ";
            if (idTiendaFiltro > 0) sql += QString("AND t.id_tienda = %1 ").arg(idTiendaFiltro);
            sql += "GROUP BY t.id_tienda, ti.nombre";
            q.prepare(sql);
            q.bindValue(":f1", fechaI_act);
            q.bindValue(":f2", fechaF_act);
            if (q.exec()) {
                while (q.next()) {
                    int idT = q.value("id_tienda").toInt();
                    DatosPeriodo dp;
                    dp.nombreTienda = q.value("nombre_tienda").toString();
                    dp.tickets = q.value("num_tickets").toInt();
                    dp.ventas = q.value("total_ventas").toDouble();
                    dp.efectivo = q.value("total_efectivo").toDouble();
                    dp.tarjeta = q.value("total_tarjeta").toDouble();
                    datosActual[idT] = dp;
                    nombresTiendas[idT] = dp.nombreTienda;
                }
            }
        }

        // Consulta periodo anterior homologo
        {
            QSqlQuery q(dbNube);
            QString sql = "SELECT t.id_tienda, COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) as nombre_tienda, "
                          "COUNT(*) as num_tickets, COALESCE(SUM(t.total), 0) as total_ventas, "
                          "COALESCE(SUM(CASE WHEN t.fpago = 1 THEN t.total ELSE 0 END), 0) as total_efectivo, "
                          "COALESCE(SUM(CASE WHEN t.fpago != 1 THEN t.total ELSE 0 END), 0) as total_tarjeta "
                          "FROM tickets_nube t "
                          "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                          "WHERE t.fecha >= :f1 AND t.fecha <= :f2 ";
            if (idTiendaFiltro > 0) sql += QString("AND t.id_tienda = %1 ").arg(idTiendaFiltro);
            sql += "GROUP BY t.id_tienda, ti.nombre";
            q.prepare(sql);
            q.bindValue(":f1", fechaI_ant);
            q.bindValue(":f2", fechaF_ant);
            if (q.exec()) {
                while (q.next()) {
                    int idT = q.value("id_tienda").toInt();
                    DatosPeriodo dp;
                    dp.nombreTienda = q.value("nombre_tienda").toString();
                    dp.tickets = q.value("num_tickets").toInt();
                    dp.ventas = q.value("total_ventas").toDouble();
                    dp.efectivo = q.value("total_efectivo").toDouble();
                    dp.tarjeta = q.value("total_tarjeta").toDouble();
                    datosAnterior[idT] = dp;
                    nombresTiendas[idT] = dp.nombreTienda;
                }
            }
        }
    }

    // Calcular comparativas por tienda
    QSet<int> todosIds;
    for (int k : datosActual.keys()) todosIds.insert(k);
    for (int k : datosAnterior.keys()) todosIds.insert(k);
    if (idTiendaFiltro > 0) todosIds.insert(idTiendaFiltro);

    double totVentasAct = 0.0, totVentasAnt = 0.0;
    int totTicketsAct = 0, totTicketsAnt = 0;
    QJsonArray desgloseArray;

    for (int idT : todosIds) {
        if (idTiendaFiltro > 0 && idT != idTiendaFiltro) continue;
        DatosPeriodo act = datosActual.value(idT);
        DatosPeriodo ant = datosAnterior.value(idT);
        QString nomT = nombresTiendas.value(idT, QString("Tienda %1").arg(idT));

        totVentasAct += act.ventas;
        totVentasAnt += ant.ventas;
        totTicketsAct += act.tickets;
        totTicketsAnt += ant.tickets;

        double difEuros = act.ventas - ant.ventas;
        double pctVar = 0.0;
        if (ant.ventas > 0.0) {
            pctVar = (difEuros / ant.ventas) * 100.0;
        }

        QJsonObject tObj;
        tObj["id_tienda"] = idT;
        tObj["tienda"] = nomT;
        tObj["ventas_periodo_actual"] = QString::number(act.ventas, 'f', 2) + " €";
        tObj["tickets_periodo_actual"] = act.tickets;
        tObj["ventas_periodo_anterior_homologo"] = QString::number(ant.ventas, 'f', 2) + " €";
        tObj["tickets_periodo_anterior_homologo"] = ant.tickets;
        tObj["diferencia_euros"] = (difEuros >= 0 ? "+" : "") + QString::number(difEuros, 'f', 2) + " €";
        tObj["porcentaje_variacion"] = (pctVar >= 0 ? "+" : "") + QString::number(pctVar, 'f', 1) + " %";
        tObj["tendencia"] = (difEuros > 0 ? "CRECIMIENTO" : (difEuros < 0 ? "DESCENSO" : "IGUAL"));
        desgloseArray.append(tObj);
    }

    double difGlobalEuros = totVentasAct - totVentasAnt;
    double pctGlobalVar = (totVentasAnt > 0.0) ? ((difGlobalEuros / totVentasAnt) * 100.0) : 0.0;

    res["periodo_actual"] = QString("%1 al %2").arg(fechaI_act, fechaF_act);
    res["periodo_anterior_homologo"] = QString("%1 al %2").arg(fechaI_ant, fechaF_ant);
    res["tienda_solicitada"] = tiendaFiltro;
    res["total_global_actual"] = QString::number(totVentasAct, 'f', 2) + " €";
    res["tickets_global_actual"] = totTicketsAct;
    res["total_global_anterior"] = QString::number(totVentasAnt, 'f', 2) + " €";
    res["tickets_global_anterior"] = totTicketsAnt;
    res["diferencia_global_euros"] = (difGlobalEuros >= 0 ? "+" : "") + QString::number(difGlobalEuros, 'f', 2) + " €";
    res["porcentaje_variacion_global"] = (pctGlobalVar >= 0 ? "+" : "") + QString::number(pctGlobalVar, 'f', 1) + " %";
    res["tendencia_global"] = (difGlobalEuros > 0 ? "CRECIMIENTO" : (difGlobalEuros < 0 ? "DESCENSO" : "IGUAL"));
    res["desglose_por_tienda"] = desgloseArray;

    return res;
}

/**
 * @brief Analiza el ritmo de ventas y la cobertura de stock para el resto del año o meses indicados.
 */
QJsonObject AsistenteIA::toolPrevisionCoberturaStock(const QJsonObject &args)
{
    QJsonObject res;

    QString termino = args.value("producto").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("fabricante").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("marca").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("familia").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("termino").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("articulo").toString().trimmed();
    if (termino.isEmpty()) termino = args.value("codigo").toString().trimmed();

    // Fallback contextual del historial
    if (termino.isEmpty() || termino.contains("previst") || termino.contains("rest") || termino.contains("suficiente") || termino.contains("ano")) {
        for (int i = m_historial.size() - 1; i >= 0; --i) {
            QJsonObject msg = m_historial[i].toObject();
            if (msg.value("role").toString() == "tool") {
                QJsonDocument doc = QJsonDocument::fromJson(msg.value("content").toString().toUtf8());
                if (doc.isObject()) {
                    QJsonObject tRes = doc.object();
                    if (tRes.contains("articulos")) {
                        QJsonArray arts = tRes.value("articulos").toArray();
                        if (!arts.isEmpty()) {
                            termino = arts[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                    if (tRes.contains("productos")) {
                        QJsonArray prods = tRes.value("productos").toArray();
                        if (!prods.isEmpty()) {
                            termino = prods[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                }
            }
        }
    }

    // Limpiar frases comunes de consulta que el modelo pueda haber arrastrado en el nombre
    static const QStringList frasesRelleno = {
        "por tienda", "en cada tienda", "de cada tienda", "en todas las tiendas", "por cada tienda",
        "venta media de", "venta media", "unidades de", "unidades", "prevision de", "cobertura de",
        "prevision", "cobertura", "stock de"
    };
    for (const QString &fr : frasesRelleno) {
        if (termino.contains(fr, Qt::CaseInsensitive)) {
            termino.replace(fr, "", Qt::CaseInsensitive);
        }
    }
    termino = termino.trimmed();

    // Simplificar si tiene múltiples productos concatenados
    if (termino.contains(" y ")) {
        termino = termino.split(" y ")[0].trimmed();
    }

    if (termino.isEmpty()) {
        res["error"] = "No se pudo identificar el producto o fabricante a proyectar.";
        return res;
    }

    QDate hoy = QDate::currentDate();
    int diaDelAno = hoy.dayOfYear();
    double mesesTranscurridos = qMax(1.0, diaDelAno / 30.416); // Meses transcurridos en el año actual
    
    int diasRestantes = (QDate(hoy.year(), 12, 31).dayOfYear() - diaDelAno);
    double mesesRestantes = qMax(0.5, diasRestantes / 30.416); // Meses restantes hasta final de año

    if (args.contains("meses_proyeccion") && args.value("meses_proyeccion").toDouble() > 0) {
        mesesRestantes = args.value("meses_proyeccion").toDouble();
    }

    QJsonArray analisisProductos;

    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        QSqlDatabase dbNube = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        
        // 1. Buscar artículos coincidentes (ordenando por ventas en 2026 para priorizar los productos activos)
        QSqlQuery qArt(dbNube);
        qArt.prepare(
            "SELECT a.cod, a.descripcion, a.pvp, "
            "COALESCE(b.nombre, '') as fabricante, "
            "COALESCE((SELECT SUM(s.stock) FROM stock_tiendas_nube s WHERE s.cod = a.cod), 0) as stock_total "
            "FROM articulos a "
            "LEFT JOIN fabricantes b ON a.fabricante = b.id "
            "LEFT JOIN familias f ON a.familia = f.id "
            "WHERE a.descripcion LIKE :t1 OR a.cod = :t2 OR b.nombre LIKE :t3 OR f.descripcion LIKE :t4 "
            "ORDER BY (SELECT COALESCE(SUM(l.cantidad), 0) FROM lineasticket_nube l WHERE l.cod = a.cod AND l.fecha >= :fInicio) DESC "
            "LIMIT 15");
        qArt.bindValue(":t1", "%" + termino + "%");
        qArt.bindValue(":t2", termino);
        qArt.bindValue(":t3", "%" + termino + "%");
        qArt.bindValue(":t4", "%" + termino + "%");
        qArt.bindValue(":fInicio", QString("%1-01-01").arg(hoy.year()));

        if (qArt.exec()) {
            while (qArt.next()) {
                QString cod = qArt.value("cod").toString();
                QString desc = qArt.value("descripcion").toString();
                double stockTotal = qArt.value("stock_total").toDouble();
                double pvp = qArt.value("pvp").toDouble();

                // 2. Ventas año actual (2026)
                double ventasAnoActual = 0.0;
                QSqlQuery qV26(dbNube);
                qV26.prepare("SELECT COALESCE(SUM(cantidad), 0) FROM lineasticket_nube WHERE cod = :cod AND fecha >= :fInicio");
                qV26.bindValue(":cod", cod);
                qV26.bindValue(":fInicio", QString("%1-01-01").arg(hoy.year()));
                if (qV26.exec() && qV26.next()) {
                    ventasAnoActual = qV26.value(0).toDouble();
                }

                // 3. Ventas mismo periodo año anterior (Sep-Dic 2025)
                double ventasPeriodoAnoAnterior = 0.0;
                QSqlQuery qV25(dbNube);
                qV25.prepare("SELECT COALESCE(SUM(cantidad), 0) FROM lineasticket_nube WHERE cod = :cod AND fecha >= :f1 AND fecha <= :f2");
                qV25.bindValue(":cod", cod);
                qV25.bindValue(":f1", QString("%1-%2-%3").arg(hoy.year() - 1).arg(hoy.month(), 2, 10, QChar('0')).arg(hoy.day(), 2, 10, QChar('0')));
                qV25.bindValue(":f2", QString("%1-12-31").arg(hoy.year() - 1));
                if (qV25.exec() && qV25.next()) {
                    ventasPeriodoAnoAnterior = qV25.value(0).toDouble();
                }

                // 4. Desglose de ventas, ritmo mensual y stock por tienda
                QJsonArray desgloseTiendasArray;
                QSqlQuery qStk(dbNube);
                qStk.prepare("SELECT t.id as id_tienda, t.nombre as tienda, "
                             "COALESCE(s.stock, 0) as stock_tienda, "
                             "COALESCE((SELECT SUM(l.cantidad) FROM lineasticket_nube l WHERE l.id_tienda = t.id AND l.cod = :cod1 AND l.fecha >= :f1), 0) as ventas_tienda "
                             "FROM tiendas t "
                             "LEFT JOIN stock_tiendas_nube s ON s.id_tienda = t.id AND s.cod = :cod2 "
                             "ORDER BY t.id ASC");
                qStk.bindValue(":cod1", cod);
                qStk.bindValue(":cod2", cod);
                qStk.bindValue(":f1", QString("%1-01-01").arg(hoy.year()));
                if (qStk.exec()) {
                    while (qStk.next()) {
                        QJsonObject st;
                        st["tienda"] = qStk.value("tienda").toString();
                        st["stock"] = qStk.value("stock_tienda").toDouble();
                        double vT = qStk.value("ventas_tienda").toDouble();
                        st["ventas_ano_actual"] = vT;
                        double ritmoT = vT / mesesTranscurridos;
                        st["venta_media_mensual"] = QString::number(ritmoT, 'f', 1) + " uds/mes";
                        desgloseTiendasArray.append(st);
                    }
                }

                // Cálculos de previsión y cobertura
                double ritmoMensual = (ventasAnoActual > 0) ? (ventasAnoActual / mesesTranscurridos) : (ventasPeriodoAnoAnterior / 4.0);
                double demandaEstimadaRestoAno = ritmoMensual * mesesRestantes;
                double mesesCobertura = (ritmoMensual > 0.05) ? (stockTotal / ritmoMensual) : (stockTotal > 0 ? 99.0 : 0.0);
                double balance = stockTotal - demandaEstimadaRestoAno;

                QJsonObject item;
                item["codigo"] = cod;
                item["producto"] = desc;
                item["pvp"] = QString::number(pvp, 'f', 2) + " €";
                item["stock_actual_total"] = stockTotal;
                item["venta_media_mensual_global"] = QString::number(ritmoMensual, 'f', 1) + " uds/mes";
                item["desglose_por_tienda"] = desgloseTiendasArray;
                item["ventas_ano_actual"] = ventasAnoActual;
                item["ventas_mismo_periodo_ano_anterior"] = ventasPeriodoAnoAnterior;
                item["ritmo_ventas_mensual_medio"] = QString::number(ritmoMensual, 'f', 1) + " uds/mes";
                item["demanda_prevista_resto_ano"] = QString::number(demandaEstimadaRestoAno, 'f', 1) + " uds";
                item["meses_cobertura_con_stock_actual"] = QString::number(mesesCobertura, 'f', 1) + " meses";
                
                if (stockTotal == 0) {
                    item["diagnostico"] = "❌ SIN STOCK (Agotado). Se requiere pedido a proveedor.";
                    item["hay_suficiente"] = false;
                } else if (balance >= 0) {
                    item["diagnostico"] = QString("✅ SÍ HAY SUFICIENTE STOCK (Sobra un margen estimado de %1 uds, cobertura de %2 meses).")
                                            .arg(QString::number(balance, 'f', 0))
                                            .arg(QString::number(mesesCobertura, 'f', 1));
                    item["hay_suficiente"] = true;
                } else {
                    item["diagnostico"] = QString("⚠️ STOCK INSUFICIENTE. Se prevé rotura en aprox. %1 meses. Faltan ~%2 unidades para cubrir hasta fin de año.")
                                            .arg(QString::number(mesesCobertura, 'f', 1))
                                            .arg(QString::number(-balance, 'f', 0));
                    item["hay_suficiente"] = false;
                }

                analisisProductos.append(item);
            }
        }
    } else {
        QSqlDatabase dbLocal = obtenerBaseDatos();
        if (dbLocal.isOpen()) {
            QSqlQuery qArt(dbLocal);
            qArt.prepare(
                "SELECT a.cod, a.descripcion, a.pvp, "
                "COALESCE(b.nombre, '') as fabricante, "
                "COALESCE(s.stock, 0) as stock_total "
                "FROM articulos a "
                "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                "LEFT JOIN familias f ON a.familia = f.id "
                "LEFT JOIN stock s ON s.cod = a.cod "
                "WHERE a.descripcion LIKE :t1 OR a.cod = :t2 OR b.nombre LIKE :t3 OR f.descripcion LIKE :t4 "
                "ORDER BY (SELECT COALESCE(SUM(l.cantidad), 0) FROM lineasticket l WHERE l.cod = a.cod AND l.fecha >= :fInicio) DESC "
                "LIMIT 15");
            qArt.bindValue(":t1", "%" + termino + "%");
            qArt.bindValue(":t2", termino);
            qArt.bindValue(":t3", "%" + termino + "%");
            qArt.bindValue(":t4", "%" + termino + "%");
            qArt.bindValue(":fInicio", QString("%1-01-01").arg(hoy.year()));

            if (qArt.exec()) {
                while (qArt.next()) {
                    QString cod = qArt.value("cod").toString();
                    QString desc = qArt.value("descripcion").toString();
                    double stockTotal = qArt.value("stock_total").toDouble();
                    double pvp = qArt.value("pvp").toDouble();

                    double ventasAnoActual = 0.0;
                    QSqlQuery qV(dbLocal);
                    qV.prepare("SELECT COALESCE(SUM(cantidad), 0) FROM lineasticket WHERE cod = :cod AND fecha >= :fInicio");
                    qV.bindValue(":cod", cod);
                    qV.bindValue(":fInicio", QString("%1-01-01").arg(hoy.year()));
                    if (qV.exec() && qV.next()) {
                        ventasAnoActual = qV.value(0).toDouble();
                    }

                    double ritmoMensual = (ventasAnoActual > 0) ? (ventasAnoActual / mesesTranscurridos) : 0.0;
                    double demandaEstimadaRestoAno = ritmoMensual * mesesRestantes;
                    double mesesCobertura = (ritmoMensual > 0.05) ? (stockTotal / ritmoMensual) : (stockTotal > 0 ? 99.0 : 0.0);
                    double balance = stockTotal - demandaEstimadaRestoAno;

                    QJsonObject item;
                    item["codigo"] = cod;
                    item["producto"] = desc;
                    item["pvp"] = QString::number(pvp, 'f', 2) + " €";
                    item["stock_actual_total"] = stockTotal;
                    item["ventas_ano_actual"] = ventasAnoActual;
                    item["ritmo_ventas_mensual_medio"] = QString::number(ritmoMensual, 'f', 1) + " uds/mes";
                    item["demanda_prevista_resto_ano"] = QString::number(demandaEstimadaRestoAno, 'f', 1) + " uds";
                    item["meses_cobertura_con_stock_actual"] = QString::number(mesesCobertura, 'f', 1) + " meses";
                    
                    if (stockTotal == 0) {
                        item["diagnostico"] = "❌ SIN STOCK (Agotado).";
                        item["hay_suficiente"] = false;
                    } else if (balance >= 0) {
                        item["diagnostico"] = QString("✅ SÍ HAY SUFICIENTE STOCK (Sobra un margen estimado de %1 uds, cobertura de %2 meses).")
                                                .arg(QString::number(balance, 'f', 0))
                                                .arg(QString::number(mesesCobertura, 'f', 1));
                        item["hay_suficiente"] = true;
                    } else {
                        item["diagnostico"] = QString("⚠️ STOCK INSUFICIENTE. Se prevé rotura en aprox. %1 meses. Faltan ~%2 unidades.")
                                                .arg(QString::number(mesesCobertura, 'f', 1))
                                                .arg(QString::number(-balance, 'f', 0));
                        item["hay_suficiente"] = false;
                    }

                    analisisProductos.append(item);
                }
            }
        }
    }

    res["termino_analizado"] = termino;
    res["fecha_actual"] = hoy.toString("yyyy-MM-dd");
    res["meses_restantes_ano"] = QString::number(mesesRestantes, 'f', 1);
    res["productos_analizados"] = analisisProductos;
    res["total_productos"] = analisisProductos.size();
    return res;
}

/**
 * @brief Ejecuta una consulta SQL SELECT generada por la IA con filtros estrictos de seguridad.
 */
QJsonObject AsistenteIA::toolEjecutarConsultaSql(const QJsonObject &args)
{
    QJsonObject res;

    // Validación de permisos para consultas directas
    if (conf && conf->permisos() && !conf->permisos()->tiene("estadisticas") && !conf->permisos()->tiene("articulos")) {
        res["error"] = "Permiso denegado: El usuario activo no tiene permiso para ejecutar consultas analíticas avanzadas.";
        return res;
    }

    QString queryStr = args.value("query").toString().trimmed();
    if (queryStr.isEmpty()) queryStr = args.value("sql").toString().trimmed();
    if (queryStr.isEmpty()) queryStr = args.value("consulta").toString().trimmed();

    if (queryStr.isEmpty()) {
        res["error"] = "Consulta SQL vacía";
        return res;
    }

    // 1. Filtro de seguridad estricto: Solo consultas de lectura (SELECT / WITH / SHOW / DESCRIBE)
    QString queryUpper = queryStr.toUpper().trimmed();
    if (!queryUpper.startsWith("SELECT") && !queryUpper.startsWith("WITH") && !queryUpper.startsWith("SHOW") && !queryUpper.startsWith("DESCRIBE")) {
        res["error"] = "Seguridad: Solo se permiten consultas SQL de solo lectura (SELECT).";
        return res;
    }

    static const QStringList forbiddenWords = {
        "INSERT ", "UPDATE ", "DELETE ", "DROP ", "ALTER ", "TRUNCATE ", "REPLACE ",
        "CREATE ", "GRANT ", "REVOKE ", "LOCK ", "UNLOCK ", "INTO OUTFILE", "INTO DUMPFILE"
    };

    for (const QString &word : forbiddenWords) {
        if (queryUpper.contains(word)) {
            res["error"] = QString("Seguridad: La consulta contiene una instrucción no permitida (%1).").arg(word.trimmed());
            return res;
        }
    }

    // Asegurar un límite para evitar bloqueos por resultados masivos
    if (!queryUpper.contains("LIMIT ") && !queryUpper.startsWith("DESCRIBE") && !queryUpper.startsWith("SHOW")) {
        queryStr += " LIMIT 50";
    }

    // 2. Ejecutar contra nube si está disponible, o contra local
    QSqlDatabase db;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "No hay conexión a la base de datos disponible.";
        return res;
    }

    // Corregir automáticamente alias comunes que los LLM suelen confundir en las vistas
    QString queryAjustada = queryStr;
    queryAjustada.replace(QRegularExpression("\\bmarca\\b", QRegularExpression::CaseInsensitiveOption), "fabricante");
    queryAjustada.replace(QRegularExpression("\\bcliente_nombre\\b", QRegularExpression::CaseInsensitiveOption), "cliente");
    queryAjustada.replace(QRegularExpression("\\bfecha_venta\\b", QRegularExpression::CaseInsensitiveOption), "fecha");
    queryAjustada.replace(QRegularExpression("\\bimporte_total\\b", QRegularExpression::CaseInsensitiveOption), "total");

    QSqlQuery q(db);
    if (!q.exec(queryAjustada)) {
        res["error"] = "Error SQL: " + q.lastError().text();
        res["sql_ejecutado"] = queryAjustada;
        return res;
    }

    QSqlRecord rec = q.record();
    int colCount = rec.count();
    QJsonArray filasArray;

    while (q.next()) {
        QJsonObject fila;
        for (int i = 0; i < colCount; ++i) {
            QString colName = rec.fieldName(i);
            QVariant val = q.value(i);
            if (val.isNull()) {
                fila[colName] = QJsonValue::Null;
            } else if (val.typeId() == QMetaType::Int || val.typeId() == QMetaType::LongLong) {
                fila[colName] = val.toLongLong();
            } else if (val.typeId() == QMetaType::Double || val.typeId() == QMetaType::Float) {
                fila[colName] = QString::number(val.toDouble(), 'f', 2);
            } else {
                fila[colName] = val.toString();
            }
        }
        filasArray.append(fila);
    }

    res["sql_ejecutado"] = queryStr;
    res["filas_obtenidas"] = filasArray.size();
    res["resultados"] = filasArray;
    return res;
}

/**
 * @brief Consulta lotes y fechas de caducidad de productos específicos o próximos a caducar.
 */
QJsonObject AsistenteIA::toolConsultarCaducidades(const QJsonObject &args)
{
    QString producto = args.value("producto").toString().trimmed();
    if (producto.isEmpty()) producto = args.value("articulo").toString().trimmed();
    if (producto.isEmpty()) producto = args.value("query").toString().trimmed();
    if (producto.isEmpty()) producto = args.value("nombre").toString().trimmed();
    if (producto.isEmpty()) producto = args.value("termino").toString().trimmed();

    // Fallback contextual: si viene vacío o conversacional, recuperar el producto del historial
    if (producto.isEmpty() || producto.contains("caduc") || producto.contains("cuando") || producto.contains("lote")) {
        for (int i = m_historial.size() - 1; i >= 0; --i) {
            QJsonObject msg = m_historial[i].toObject();
            if (msg.value("role").toString() == "tool") {
                QJsonDocument doc = QJsonDocument::fromJson(msg.value("content").toString().toUtf8());
                if (doc.isObject()) {
                    QJsonObject tRes = doc.object();
                    if (tRes.contains("articulos")) {
                        QJsonArray arts = tRes.value("articulos").toArray();
                        if (!arts.isEmpty()) {
                            producto = arts[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                    if (tRes.contains("productos")) {
                        QJsonArray prods = tRes.value("productos").toArray();
                        if (!prods.isEmpty()) {
                            producto = prods[0].toObject().value("nombre").toString();
                            break;
                        }
                    }
                    if (tRes.contains("articulo_consultado")) {
                        producto = tRes.value("articulo_consultado").toString();
                        break;
                    }
                }
            } else if (msg.value("role").toString() == "user") {
                QString uTxt = msg.value("content").toString();
                if (uTxt.contains("Pilogastril", Qt::CaseInsensitive)) { producto = "Pilogastril"; break; }
                if (uTxt.contains("Colestia", Qt::CaseInsensitive)) { producto = "Colestia"; break; }
                if (uTxt.contains("Madurit", Qt::CaseInsensitive)) { producto = "Madurit"; break; }
            }
        }
    }

    int diasLimite = args.value("dias_limite").toInt(365);
    if (diasLimite <= 0) diasLimite = 365;

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = args.value("store").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    QJsonObject res;
    QJsonArray lotesArray;
    QDate hoy = QDate::currentDate();

    QStringList conexiones = resolverConexiones(tiendaFiltro);

    for (const QString &connName : conexiones) {
        if (!QSqlDatabase::contains(connName) || !QSqlDatabase::database(connName).isOpen()) {
            continue;
        }

        QSqlDatabase db = QSqlDatabase::database(connName);
        QSqlQuery q(db);
        QString sql;

        if (!producto.isEmpty()) {
            sql = "SELECT l.id, l.ean, a.descripcion, l.lote, DATE_FORMAT(l.fecha, '%Y-%m-%d') as fecha_str, l.cantidad "
                  "FROM lotes l "
                  "LEFT JOIN articulos a ON l.ean = a.cod "
                  "WHERE (a.descripcion LIKE :p OR l.ean = :p2) "
                  "ORDER BY l.fecha ASC LIMIT 30";
            q.prepare(sql);
            q.bindValue(":p", "%" + producto + "%");
            q.bindValue(":p2", producto);
        } else {
            sql = "SELECT l.id, l.ean, a.descripcion, l.lote, DATE_FORMAT(l.fecha, '%Y-%m-%d') as fecha_str, l.cantidad "
                  "FROM lotes l "
                  "LEFT JOIN articulos a ON l.ean = a.cod "
                  "WHERE l.fecha <= DATE_ADD(CURDATE(), INTERVAL :dias DAY) AND l.cantidad > 0 "
                  "ORDER BY l.fecha ASC LIMIT 30";
            q.prepare(sql);
            q.bindValue(":dias", diasLimite);
        }

        if (q.exec()) {
            while (q.next()) {
                QJsonObject item;
                item["tienda"] = (connName == "DB" || (conf && connName == conf->getConexionLocal())) ? "Tienda Local" : connName;
                item["codigo"] = q.value("ean").toString();
                item["producto"] = q.value("descripcion").toString();
                item["lote"] = q.value("lote").toString().isEmpty() ? "Lote General" : q.value("lote").toString();
                
                QString fStr = q.value("fecha_str").toString();
                item["fecha_caducidad"] = fStr;
                
                double cant = q.value("cantidad").toDouble();
                item["unidades_stock"] = cant;

                QDate fCad = QDate::fromString(fStr, "yyyy-MM-dd");
                if (fCad.isValid() && fStr != "2000-01-01") {
                    qint64 diasFaltan = hoy.daysTo(fCad);
                    item["dias_restantes"] = diasFaltan;
                    if (diasFaltan < 0) {
                        item["estado"] = QString("CADUCADO (hace %1 días)").arg(-diasFaltan);
                    } else if (diasFaltan <= 60) {
                        item["estado"] = QString("⚠️ CADUCIDAD INMINENTE (%1 días)").arg(diasFaltan);
                    } else if (diasFaltan <= 180) {
                        item["estado"] = QString("Próximo a caducar (%1 meses)").arg(QString::number(diasFaltan / 30.0, 'f', 1));
                    } else {
                        item["estado"] = QString("Vigente (%1 meses)").arg(QString::number(diasFaltan / 30.0, 'f', 1));
                    }
                } else {
                    item["estado"] = "Sin fecha de lote específica";
                }

                lotesArray.append(item);
            }
        }
    }

    if (lotesArray.isEmpty() && !producto.isEmpty()) {
        res["mensaje"] = QString("El producto '%1' existe en el catálogo pero actualmente no tiene lotes con fecha de caducidad registrados o su stock en lotes es 0.").arg(producto);
    }

    res["producto_consultado"] = producto;
    res["total_lotes_encontrados"] = lotesArray.size();
    res["lotes"] = lotesArray;
    return res;
}

/**
 * @brief Consulta el historial de proveedores, precios de costo, descuentos y compras de un producto.
 */
QJsonObject AsistenteIA::toolConsultarComprasProveedor(const QJsonObject &args)
{
    QJsonObject res;
    QString prod = args.value("producto").toString().trimmed();
    if (prod.isEmpty()) prod = args.value("articulo").toString().trimmed();
    if (prod.isEmpty()) prod = args.value("termino").toString().trimmed();
    if (prod.isEmpty()) prod = args.value("codigo").toString().trimmed();
    if (prod.isEmpty()) prod = args.value("ean").toString().trimmed();
    if (prod.isEmpty()) prod = args.value("query").toString().trimmed();

    QString prov = args.value("proveedor").toString().trimmed();

    bool buscarMargenesBajos = args.value("margenes_bajos").toBool(false) ||
                              args.value("margen_bajo").toBool(false) ||
                              prod.contains("margen", Qt::CaseInsensitive) ||
                              prod.contains("rentabil", Qt::CaseInsensitive);

    // Recuperación contextual si el usuario no repitió el nombre del producto
    if (prod.isEmpty() && prov.isEmpty() && !buscarMargenesBajos) {
        for (int i = m_historial.size() - 1; i >= 0; --i) {
            QJsonObject msg = m_historial.at(i).toObject();
            QString content = msg.value("content").toString();
            if (content.contains("margen", Qt::CaseInsensitive)) { buscarMargenesBajos = true; break; }
            if (content.contains("Barrita proteica", Qt::CaseInsensitive)) { prod = "Barrita proteica"; break; }
            if (content.contains("Pilogastril", Qt::CaseInsensitive)) { prod = "Pilogastril"; break; }
            if (content.contains("Colestia", Qt::CaseInsensitive)) { prod = "Colestia"; break; }
            if (content.contains("Curcurina", Qt::CaseInsensitive)) { prod = "Curcurina"; break; }
            if (content.contains("Nutri Sport", Qt::CaseInsensitive)) { prod = "Nutri Sport"; break; }
            if (content.contains("Nova Diet", Qt::CaseInsensitive)) { prod = "Nova Diet"; break; }
            if (content.contains("Santiveri", Qt::CaseInsensitive)) { prod = "Santiveri"; break; }
            if (content.contains("Soria", Qt::CaseInsensitive)) { prod = "Soria"; break; }
        }
    }

    QSqlDatabase db;
    bool esNube = false;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        esNube = true;
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "Base de datos no disponible";
        return res;
    }

    // 1. Detección automática de productos con márgenes comerciales más bajos
    if (buscarMargenesBajos || (prod.isEmpty() && prov.isEmpty())) {
        QJsonArray margenesArray;
        QSqlQuery qM(db);
        QString sqlM;
        if (esNube) {
            sqlM = "SELECT a.cod, a.descripcion, a.precio_compra, a.pvp, "
                   "ROUND(((a.pvp - a.precio_compra) / a.pvp) * 100, 1) as margen_pct, "
                   "COALESCE(b.nombre, 'Sin Fabricante') as fabricante, "
                   "COALESCE((SELECT SUM(s.stock) FROM stock_tiendas_nube s WHERE s.cod = a.cod), 0) as stock_total "
                   "FROM articulos a "
                   "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                   "WHERE a.precio_compra > 0 AND a.pvp > a.precio_compra ";
            if (!prod.isEmpty() && !prod.contains("margen", Qt::CaseInsensitive)) {
                sqlM += "AND (a.descripcion LIKE :p1 OR b.nombre LIKE :p2) ";
            }
            sqlM += "ORDER BY margen_pct ASC LIMIT 15";
        } else {
            sqlM = "SELECT a.cod, a.descripcion, a.precio_compra, a.pvp, "
                   "ROUND(((a.pvp - a.precio_compra) / a.pvp) * 100, 1) as margen_pct, "
                   "COALESCE(b.nombre, 'Sin Fabricante') as fabricante, "
                   "COALESCE(s.stock, 0) as stock_total "
                   "FROM articulos a "
                   "LEFT JOIN fabricantes b ON a.fabricante = b.id "
                   "LEFT JOIN stock s ON s.cod = a.cod "
                   "WHERE a.precio_compra > 0 AND a.pvp > a.precio_compra ";
            if (!prod.isEmpty() && !prod.contains("margen", Qt::CaseInsensitive)) {
                sqlM += "AND (a.descripcion LIKE :p1 OR b.nombre LIKE :p2) ";
            }
            sqlM += "ORDER BY margen_pct ASC LIMIT 15";
        }
        qM.prepare(sqlM);
        if (!prod.isEmpty() && !prod.contains("margen", Qt::CaseInsensitive)) {
            qM.bindValue(":p1", "%" + prod + "%");
            qM.bindValue(":p2", "%" + prod + "%");
        }
        if (qM.exec()) {
            int pos = 1;
            while (qM.next()) {
                QJsonObject item;
                item["posicion"] = pos++;
                item["codigo"] = qM.value("cod").toString();
                item["producto"] = qM.value("descripcion").toString();
                item["fabricante"] = qM.value("fabricante").toString();
                item["precio_compra"] = QString::number(qM.value("precio_compra").toDouble(), 'f', 2) + " €";
                item["pvp"] = QString::number(qM.value("pvp").toDouble(), 'f', 2) + " €";
                item["margen_comercial_porcentaje"] = QString::number(qM.value("margen_pct").toDouble(), 'f', 1) + " %";
                item["stock_actual"] = qM.value("stock_total").toDouble();
                margenesArray.append(item);
            }
        }

        res["tipo_resultado"] = "productos_con_menor_margen_comercial";
        res["total_productos"] = margenesArray.size();
        res["productos_margen_bajo"] = margenesArray;
        return res;
    }

    QJsonArray comprasArray;
    QSqlQuery q(db);
    QString sql;
    if (esNube) {
        sql = "SELECT lp.cod, lp.descripcion, p.idProveedor, COALESCE(p.nombre, 'Sin Proveedor') AS proveedor, "
              "lp.costo AS precio_costo, lp.descuento1 AS descuento, "
              "DATE_FORMAT(lp.fc, '%Y-%m-%d') AS fecha_compra, "
              "COALESCE(t.nombre, CONCAT('Tienda ', lp.id_tienda)) AS tienda "
              "FROM lineaspedido_nube lp "
              "LEFT JOIN proveedores p ON lp.idProveedor = p.idProveedor "
              "LEFT JOIN tiendas t ON lp.id_tienda = t.id "
              "WHERE 1=1 ";
        if (!prod.isEmpty()) sql += "AND (lp.descripcion LIKE :p1 OR lp.cod = :p2) ";
        if (!prov.isEmpty()) sql += "AND (p.nombre LIKE :pr1 OR p.idProveedor = :pr2) ";
        sql += "ORDER BY lp.fc DESC LIMIT 15";
    } else {
        sql = "SELECT lp.cod, lp.descripcion, p.idProveedor, COALESCE(p.nombre, 'Sin Proveedor') AS proveedor, "
              "lp.costo AS precio_costo, lp.descuento1 AS descuento, "
              "DATE_FORMAT(lp.fc, '%Y-%m-%d') AS fecha_compra, 'Local' AS tienda "
              "FROM lineaspedido lp "
              "LEFT JOIN proveedores p ON lp.idProveedor = p.idProveedor "
              "WHERE 1=1 ";
        if (!prod.isEmpty()) sql += "AND (lp.descripcion LIKE :p1 OR lp.cod = :p2) ";
        if (!prov.isEmpty()) sql += "AND (p.nombre LIKE :pr1 OR p.idProveedor = :pr2) ";
        sql += "ORDER BY lp.fc DESC LIMIT 15";
    }

    q.prepare(sql);
    if (!prod.isEmpty()) {
        q.bindValue(":p1", "%" + prod + "%");
        q.bindValue(":p2", prod);
    }
    if (!prov.isEmpty()) {
        q.bindValue(":pr1", "%" + prov + "%");
        q.bindValue(":pr2", prov.toInt());
    }

    if (q.exec()) {
        while (q.next()) {
            QJsonObject item;
            item["codigo"] = q.value("cod").toString();
            item["producto"] = q.value("descripcion").toString();
            item["id_proveedor"] = q.value("idProveedor").toInt();
            item["proveedor"] = q.value("proveedor").toString();
            item["precio_compra_costo"] = QString::number(q.value("precio_costo").toDouble(), 'f', 2) + " €";
            item["descuento_porcentaje"] = QString::number(q.value("descuento").toDouble(), 'f', 1) + " %";
            item["fecha_compra"] = q.value("fecha_compra").toString();
            item["tienda"] = q.value("tienda").toString();
            comprasArray.append(item);
        }
    }

    // Si no hay pedidos registrados en lineaspedido, consultar precio_compra en maestro articulos
    if (comprasArray.isEmpty() && !prod.isEmpty()) {
        QSqlQuery qArt(db);
        qArt.prepare("SELECT a.cod, a.descripcion, a.precio_compra, a.pvp, COALESCE(p.nombre, 'Sin asignar') AS proveedor "
                     "FROM articulos a "
                     "LEFT JOIN proveedores p ON a.fabricante = p.idProveedor "
                     "WHERE a.descripcion LIKE :p1 OR a.cod = :p2 LIMIT 5");
        qArt.bindValue(":p1", "%" + prod + "%");
        qArt.bindValue(":p2", prod);
        if (qArt.exec()) {
            while (qArt.next()) {
                QJsonObject item;
                item["codigo"] = qArt.value("cod").toString();
                item["producto"] = qArt.value("descripcion").toString();
                item["proveedor"] = qArt.value("proveedor").toString();
                item["precio_compra_costo"] = QString::number(qArt.value("precio_compra").toDouble(), 'f', 2) + " €";
                item["pvp"] = QString::number(qArt.value("pvp").toDouble(), 'f', 2) + " €";
                comprasArray.append(item);
            }
        }
    }

    res["tipo_resultado"] = "compras_a_proveedores";
    res["producto_consultado"] = prod;
    res["total_registros"] = comprasArray.size();
    res["historial_compras"] = comprasArray;
    return res;
}

/**
 * @brief Obtiene el ranking o desglose de facturación por horas del día (horas punta / pico).
 */
QJsonObject AsistenteIA::toolFacturacionPorHoras(const QJsonObject &args)
{
    QJsonObject res;
    int limite = args.value("limite").toInt();
    if (limite <= 0) limite = args.value("limit").toInt(3);
    if (limite <= 0 || limite > 24) limite = 3;

    QDate hoy = QDate::currentDate();
    int ano = args.value("ano").toInt();
    if (ano <= 0) ano = args.value("year").toInt();

    // 1. Extraer año si venía en el historial de usuario
    if (ano <= 0 && args.value("fecha_inicio").toString().isEmpty()) {
        static const QRegularExpression regAno("\\b(20[123]\\d)\\b");
        for (int idx = m_historial.size() - 1; idx >= 0; --idx) {
            QJsonObject msgObj = m_historial[idx].toObject();
            if (msgObj.value("role").toString() == "user") {
                QString txt = msgObj.value("content").toString().toLower();
                if (txt.contains("este año") || txt.contains("año actual")) {
                    ano = hoy.year();
                    break;
                }
                QRegularExpressionMatch mUser = regAno.match(txt);
                if (mUser.hasMatch()) {
                    ano = mUser.captured(1).toInt();
                    break;
                }
            }
        }
    }

    QString fechaI = args.value("fecha_inicio").toString().trimmed();
    QString fechaF = args.value("fecha_fin").toString().trimmed();
    if (ano > 2000) {
        if (fechaI.isEmpty()) fechaI = QString("%1-01-01").arg(ano);
        if (fechaF.isEmpty()) fechaF = QString("%1-12-31").arg(ano);
    }

    QString tiendaFiltro = args.value("tienda").toString().trimmed();
    if (tiendaFiltro.isEmpty()) tiendaFiltro = "todas";

    int idTiendaFiltro = 0;
    QString tNorm = tiendaFiltro.toLower();
    if (tNorm.contains("emeic")) idTiendaFiltro = 1;
    else if (tNorm.contains("casa")) idTiendaFiltro = 2;
    else if (tNorm.contains("cerv")) idTiendaFiltro = 3;

    QSqlDatabase db;
    bool esNube = false;
    if (QSqlDatabase::contains(SyncManager::CONEXION_NUBE) && QSqlDatabase::database(SyncManager::CONEXION_NUBE).isOpen()) {
        db = QSqlDatabase::database(SyncManager::CONEXION_NUBE);
        esNube = true;
    } else {
        db = obtenerBaseDatos();
    }

    if (!db.isOpen()) {
        res["error"] = "Base de datos no disponible";
        return res;
    }

    QString agruparPor = args.value("agrupar_por").toString().trimmed().toLower();
    if (agruparPor.isEmpty()) agruparPor = args.value("tipo").toString().trimmed().toLower();
    if (agruparPor.isEmpty()) agruparPor = args.value("criterio").toString().trimmed().toLower();
    
    bool esDiasSemana = agruparPor.contains("dia") || agruparPor.contains("semana");

    if (esDiasSemana) {
        QJsonArray diasArray;
        QSqlQuery q(db);
        QString sql;
        if (esNube) {
            sql = "SELECT DAYOFWEEK(fecha) AS num_dia, "
                  "CASE DAYOFWEEK(fecha) "
                  "  WHEN 1 THEN 'Domingo' "
                  "  WHEN 2 THEN 'Lunes' "
                  "  WHEN 3 THEN 'Martes' "
                  "  WHEN 4 THEN 'Miércoles' "
                  "  WHEN 5 THEN 'Jueves' "
                  "  WHEN 6 THEN 'Viernes' "
                  "  WHEN 7 THEN 'Sábado' "
                  "END AS dia_semana, "
                  "COUNT(DISTINCT CONCAT(id_tienda, '_', ticket)) AS total_tickets, "
                  "ROUND(SUM(total), 2) AS total_facturado, "
                  "ROUND(AVG(total), 2) AS ticket_medio "
                  "FROM tickets_nube "
                  "WHERE 1=1 ";
            if (idTiendaFiltro > 0) sql += QString("AND id_tienda = %1 ").arg(idTiendaFiltro);
            if (!fechaI.isEmpty()) sql += "AND fecha >= :f1 ";
            if (!fechaF.isEmpty()) sql += "AND fecha <= :f2 ";
            sql += "GROUP BY DAYOFWEEK(fecha) ORDER BY total_facturado DESC";
        } else {
            sql = "SELECT DAYOFWEEK(fecha) AS num_dia, "
                  "CASE DAYOFWEEK(fecha) "
                  "  WHEN 1 THEN 'Domingo' "
                  "  WHEN 2 THEN 'Lunes' "
                  "  WHEN 3 THEN 'Martes' "
                  "  WHEN 4 THEN 'Miércoles' "
                  "  WHEN 5 THEN 'Jueves' "
                  "  WHEN 6 THEN 'Viernes' "
                  "  WHEN 7 THEN 'Sábado' "
                  "END AS dia_semana, "
                  "COUNT(DISTINCT ticket) AS total_tickets, "
                  "ROUND(SUM(total), 2) AS total_facturado, "
                  "ROUND(AVG(total), 2) AS ticket_medio "
                  "FROM tickets "
                  "WHERE 1=1 ";
            if (!fechaI.isEmpty()) sql += "AND fecha >= :f1 ";
            if (!fechaF.isEmpty()) sql += "AND fecha <= :f2 ";
            sql += "GROUP BY DAYOFWEEK(fecha) ORDER BY total_facturado DESC";
        }
        q.prepare(sql);
        if (!fechaI.isEmpty()) q.bindValue(":f1", fechaI);
        if (!fechaF.isEmpty()) q.bindValue(":f2", fechaF);

        if (q.exec()) {
            int pos = 1;
            while (q.next()) {
                QJsonObject dObj;
                dObj["posicion"] = pos++;
                dObj["dia_semana"] = q.value("dia_semana").toString();
                dObj["total_facturado"] = QString::number(q.value("total_facturado").toDouble(), 'f', 2) + " €";
                dObj["total_tickets"] = q.value("total_tickets").toInt();
                dObj["ticket_medio"] = QString::number(q.value("ticket_medio").toDouble(), 'f', 2) + " €";
                diasArray.append(dObj);
            }
        }

        if (esNube && idTiendaFiltro == 0) {
            QSqlQuery qT(db);
            QString sqlT = "SELECT t.id_tienda, COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS nombre_tienda, "
                           "CASE DAYOFWEEK(t.fecha) "
                           "  WHEN 1 THEN 'Domingo' "
                           "  WHEN 2 THEN 'Lunes' "
                           "  WHEN 3 THEN 'Martes' "
                           "  WHEN 4 THEN 'Miércoles' "
                           "  WHEN 5 THEN 'Jueves' "
                           "  WHEN 6 THEN 'Viernes' "
                           "  WHEN 7 THEN 'Sábado' "
                           "END AS dia_semana, "
                           "COUNT(DISTINCT t.ticket) AS total_tickets, "
                           "ROUND(SUM(t.total), 2) AS total_facturado "
                           "FROM tickets_nube t "
                           "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                           "WHERE 1=1 ";
            if (!fechaI.isEmpty()) sqlT += "AND t.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlT += "AND t.fecha <= :f2 ";
            sqlT += "GROUP BY t.id_tienda, DAYOFWEEK(t.fecha) "
                    "ORDER BY t.id_tienda ASC, total_facturado DESC";
            qT.prepare(sqlT);
            if (!fechaI.isEmpty()) qT.bindValue(":f1", fechaI);
            if (!fechaF.isEmpty()) qT.bindValue(":f2", fechaF);
            if (qT.exec()) {
                QMap<QString, QJsonArray> tiendasMap;
                while (qT.next()) {
                    QString nTienda = qT.value("nombre_tienda").toString();
                    QJsonObject tItem;
                    tItem["dia_semana"] = qT.value("dia_semana").toString();
                    tItem["total_facturado"] = QString::number(qT.value("total_facturado").toDouble(), 'f', 2) + " €";
                    tItem["total_tickets"] = qT.value("total_tickets").toInt();
                    tiendasMap[nTienda].append(tItem);
                }
                QJsonObject desTiendas;
                for (auto it = tiendasMap.begin(); it != tiendasMap.end(); ++it) {
                    desTiendas[it.key()] = it.value();
                }
                res["desglose_por_tienda"] = desTiendas;
            }
        }

        res["tipo_resultado"] = "ranking_dias_semana_facturacion";
        res["tienda_consultada"] = tiendaFiltro;
        res["periodo"] = QString("Del %1 al %2").arg(!fechaI.isEmpty() ? fechaI : "inicio", !fechaF.isEmpty() ? fechaF : "fin");
        res["total_dias_devueltos"] = diasArray.size();
        if (!diasArray.isEmpty()) {
            res["mejor_dia_semana"] = diasArray.first().toObject().value("dia_semana").toString();
        }
        res["dias_semana"] = diasArray;
        return res;
    }

    QJsonArray horasArray;
    QSqlQuery q(db);
    QString sql;
    if (esNube) {
        sql = "SELECT HOUR(hora) AS franja_horaria, "
              "CONCAT(LPAD(HOUR(hora), 2, '0'), ':00 - ', LPAD(HOUR(hora)+1, 2, '0'), ':00') AS intervalo, "
              "COUNT(DISTINCT CONCAT(id_tienda, '_', ticket)) AS total_tickets, "
              "ROUND(SUM(total), 2) AS total_facturado, "
              "ROUND(AVG(total), 2) AS ticket_medio "
              "FROM tickets_nube "
              "WHERE hora IS NOT NULL AND hora != '00:00:00' ";
        if (idTiendaFiltro > 0) sql += QString("AND id_tienda = %1 ").arg(idTiendaFiltro);
        if (!fechaI.isEmpty()) sql += "AND fecha >= :f1 ";
        if (!fechaF.isEmpty()) sql += "AND fecha <= :f2 ";
        sql += "GROUP BY HOUR(hora) ORDER BY total_facturado DESC LIMIT :limite";
    } else {
        sql = "SELECT HOUR(hora) AS franja_horaria, "
              "CONCAT(LPAD(HOUR(hora), 2, '0'), ':00 - ', LPAD(HOUR(hora)+1, 2, '0'), ':00') AS intervalo, "
              "COUNT(DISTINCT ticket) AS total_tickets, "
              "ROUND(SUM(total), 2) AS total_facturado, "
              "ROUND(AVG(total), 2) AS ticket_medio "
              "FROM tickets "
              "WHERE hora IS NOT NULL AND hora != '00:00:00' ";
        if (!fechaI.isEmpty()) sql += "AND fecha >= :f1 ";
        if (!fechaF.isEmpty()) sql += "AND fecha <= :f2 ";
        sql += "GROUP BY HOUR(hora) ORDER BY total_facturado DESC LIMIT :limite";
    }

    q.prepare(sql);
    if (!fechaI.isEmpty()) q.bindValue(":f1", fechaI);
    if (!fechaF.isEmpty()) q.bindValue(":f2", fechaF);
    q.bindValue(":limite", limite);

    if (q.exec()) {
        int pos = 1;
        while (q.next()) {
            QJsonObject hObj;
            hObj["posicion"] = pos++;
            hObj["hora"] = q.value("franja_horaria").toInt();
            hObj["intervalo_horario"] = q.value("intervalo").toString();
            hObj["total_facturado"] = QString::number(q.value("total_facturado").toDouble(), 'f', 2) + " €";
            hObj["total_tickets"] = q.value("total_tickets").toInt();
            hObj["ticket_medio"] = QString::number(q.value("ticket_medio").toDouble(), 'f', 2) + " €";
            horasArray.append(hObj);
        }
    }

        if (esNube && idTiendaFiltro == 0) {
            QSqlQuery qT(db);
            QString sqlT = "SELECT t.id_tienda, COALESCE(ti.nombre, CONCAT('Tienda ', t.id_tienda)) AS nombre_tienda, "
                           "HOUR(t.hora) AS franja_horaria, "
                           "CONCAT(LPAD(HOUR(t.hora), 2, '0'), ':00 - ', LPAD(HOUR(t.hora)+1, 2, '0'), ':00') AS intervalo, "
                           "COUNT(DISTINCT t.ticket) AS total_tickets, "
                           "ROUND(SUM(t.total), 2) AS total_facturado "
                           "FROM tickets_nube t "
                           "LEFT JOIN tiendas ti ON t.id_tienda = ti.id "
                           "WHERE t.hora IS NOT NULL AND t.hora != '00:00:00' ";
            if (!fechaI.isEmpty()) sqlT += "AND t.fecha >= :f1 ";
            if (!fechaF.isEmpty()) sqlT += "AND t.fecha <= :f2 ";
            sqlT += "GROUP BY t.id_tienda, HOUR(t.hora) "
                    "ORDER BY t.id_tienda ASC, total_facturado DESC";
            qT.prepare(sqlT);
            if (!fechaI.isEmpty()) qT.bindValue(":f1", fechaI);
            if (!fechaF.isEmpty()) qT.bindValue(":f2", fechaF);
            if (qT.exec()) {
                QMap<QString, QJsonArray> tiendasMap;
                QMap<QString, int> conteoTienda;
                while (qT.next()) {
                    QString nTienda = qT.value("nombre_tienda").toString();
                    int cnt = conteoTienda.value(nTienda, 0);
                    if (cnt < limite) {
                        conteoTienda[nTienda] = cnt + 1;
                        QJsonObject tItem;
                        tItem["posicion"] = cnt + 1;
                        tItem["intervalo_horario"] = qT.value("intervalo").toString();
                        tItem["total_facturado"] = QString::number(qT.value("total_facturado").toDouble(), 'f', 2) + " €";
                        tItem["total_tickets"] = qT.value("total_tickets").toInt();
                        tiendasMap[nTienda].append(tItem);
                    }
                }
                QJsonObject desTiendas;
                for (auto it = tiendasMap.begin(); it != tiendasMap.end(); ++it) {
                    desTiendas[it.key()] = it.value();
                }
                res["desglose_por_tienda"] = desTiendas;
            }
        }

        res["tipo_resultado"] = "ranking_horas_facturacion";
        res["tienda_consultada"] = tiendaFiltro;
        res["total_franjas_devueltas"] = horasArray.size();
        res["horas_mas_facturacion"] = horasArray;
        return res;
}


