#include "monitorcaducidades.h"
#include <QSqlRecord>
#include <QUuid>

MonitorCaducidades::MonitorCaducidades(QString conexionLocal, QObject *parent)
    : QObject(parent)
{
    // Obtener parámetros de conexión en el hilo principal para usarlos luego en el hilo secundario
    QSqlDatabase baseOriginal = QSqlDatabase::database(conexionLocal);
    m_driver = baseOriginal.driverName();
    m_host = baseOriginal.hostName();
    m_port = baseOriginal.port();
    m_dbName = baseOriginal.databaseName();
    m_user = baseOriginal.userName();
    m_password = baseOriginal.password();
}

MonitorCaducidades::~MonitorCaducidades()
{
}

void MonitorCaducidades::iniciar()
{
    // Función de inicialización simplificada (ahora se lanza ejecutarAnalisis directamente)
    ejecutarAnalisis();
}

void MonitorCaducidades::ejecutarAnalisis()
{
    qDebug() << "MonitorCaducidades: Ejecutando análisis inteligente en hilo secundario...";
    QList<RecomendacionCaducidad> listaRecomendaciones;
    QDate hoy = QDate::currentDate();
    
    // Clonar conexión base de datos para usar en este hilo sin colisionar con TPV principal ni otros hilos muertos
    QString connectionName = "Monitor_Local_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
    QSqlDatabase dbLocal = QSqlDatabase::addDatabase(m_driver, connectionName);
    dbLocal.setHostName(m_host);
    if (m_port > 0) dbLocal.setPort(m_port);
    dbLocal.setDatabaseName(m_dbName);
    dbLocal.setUserName(m_user);
    dbLocal.setPassword(m_password);

    if (!dbLocal.open()) {
        emit errorOcurrido("MonitorCaducidades: No se pudo abrir la BD local.");
        return;
    }

    // Usar un cerrojo (lock) a nivel de base de datos para evitar que múltiples 
    // instancias de la aplicación ejecuten este hilo a la vez. 
    // Si la app crashea, MariaDB libera el lock automáticamente al perder la conexión.
    QSqlQuery qLock(dbLocal);
    qLock.exec("SELECT GET_LOCK('monitor_caducidades_lock', 0)");
    if (qLock.next() && qLock.value(0).toInt() != 1) {
        dbLocal.close();
        emit errorOcurrido("El análisis de caducidades ya se está ejecutando en otra instancia o en otra máquina en este momento. Por favor, inténtelo de nuevo más tarde.");
        emit finished();
        return;
    }

    // OPTIMIZACIÓN: Pre-conectar a todas las tiendas remotas (una sola vez)
    // para evitar el delay por timeout en cada lote si una tienda está apagada.
    QList<QString> conexionesRemotasActivas;
    QMap<QString, QString> nombresTiendasRemotas;
    QSqlQuery qTiendas(dbLocal);
    qTiendas.exec("SELECT id, nombre, baseDatos, usuario, password, ip, puerto FROM tiendas WHERE local = 0");
    while (qTiendas.next()) {
        QString connRemote = "Monitor_Remote_" + qTiendas.value("id").toString() + "_" + QUuid::createUuid().toString(QUuid::WithoutBraces);
        QSqlDatabase dbRemote = QSqlDatabase::addDatabase("QMYSQL", connRemote);
        dbRemote.setHostName(qTiendas.value("ip").toString());
        dbRemote.setPort(qTiendas.value("puerto").toInt());
        dbRemote.setDatabaseName(qTiendas.value("baseDatos").toString());
        dbRemote.setUserName(qTiendas.value("usuario").toString());
        dbRemote.setPassword(qTiendas.value("password").toString());
        dbRemote.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT=2"); 
        
        if (dbRemote.open()) {
            conexionesRemotasActivas.append(connRemote);
            nombresTiendasRemotas[connRemote] = qTiendas.value("nombre").toString();
        }
    }

    // Buscar lotes próximos a caducar (en menos de 1 año) con stock > 0
    QSqlQuery qLotes(dbLocal);
    qLotes.prepare("SELECT ean, lote, DATE_FORMAT(fecha, '%Y-%m-%d') as fecha_str, cantidad FROM lotes "
                   "WHERE fecha <= DATE_ADD(CURDATE(), INTERVAL 1 YEAR) AND cantidad > 0");
    if (!qLotes.exec()) {
        qDebug() << "MonitorCaducidades Error SQL:" << qLotes.lastError().text();
        dbLocal.close();
        return;
    }

    // Ya están declaradas arriba: listaRecomendaciones y QDate hoy = QDate::currentDate();

    while (qLotes.next()) {
        RecomendacionCaducidad rec;
        rec.ean = qLotes.value("ean").toString();
        rec.lote = qLotes.value("lote").toString();
        rec.fechaCaducidad = QDate::fromString(qLotes.value("fecha_str").toString(), "yyyy-MM-dd");
        rec.stockLote = qLotes.value("cantidad").toDouble();
        
        rec.diasRestantes = hoy.daysTo(rec.fechaCaducidad);
        
        // Ignorar fechas inválidas o lotes que caducan en más de 1 año, 
        // incluso si las ventas son 0 o la BD los ha filtrado mal.
        if (!rec.fechaCaducidad.isValid() || rec.diasRestantes > 365) {
            continue;
        }
        
        if (rec.diasRestantes <= 180) {
            rec.nivelAlerta = 2; // Crítico (6 meses)
        } else {
            rec.nivelAlerta = 1; // Aviso (1 año)
        }

        // Obtener descripción
        QSqlQuery qArt(dbLocal);
        qArt.prepare("SELECT descripcion FROM articulos WHERE cod = ?");
        qArt.bindValue(0, rec.ean);
        if (qArt.exec() && qArt.first()) {
            rec.descripcion = qArt.value(0).toString();
        } else {
            rec.descripcion = "Articulo Desconocido";
        }

        // Calcular velocidad de venta del ÚLTIMO AÑO
        QSqlQuery qVentas(dbLocal);
        qVentas.prepare("SELECT SUM(cantidad) FROM lineasticket "
                        "WHERE cod = ? AND fecha >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR)");
        qVentas.bindValue(0, rec.ean);
        int ventasAnuales = 0;
        if (qVentas.exec() && qVentas.first()) {
            ventasAnuales = qVentas.value(0).toInt();
        }
        
        rec.mediaVentaMensualLocal = ventasAnuales / 12.0;
        
        // Restar un margen de consumo para el cliente (ej. 30 días)
        int diasUtiles = rec.diasRestantes - 30;
        if (diasUtiles < 0) diasUtiles = 0;
        
        // Venta estimada local durante los días útiles
        double ventaPrevistaLocal = (ventasAnuales / 365.0) * diasUtiles;
        double excedenteLocal = rec.stockLote - ventaPrevistaLocal;
        
        // Si hay excedente (no da tiempo a venderlo localmente con margen)
        if (excedenteLocal > 0) {
            double mejorCapacidadAbsorcion = 0;
            QString mejorTienda = "";
            double ventasMesMejorTienda = 0;

            // Usar las conexiones remotas precargadas
            for (const QString &connRemote : conexionesRemotasActivas) {
                QSqlDatabase dbRemote = QSqlDatabase::database(connRemote);
                
                // 1. Obtener ventas anuales de esta tienda remota
                QSqlQuery qVentasR(dbRemote);
                qVentasR.prepare("SELECT SUM(cantidad) FROM lineasticket WHERE cod = ? AND fecha >= DATE_SUB(CURDATE(), INTERVAL 1 YEAR)");
                qVentasR.bindValue(0, rec.ean);
                int ventasAnualesR = 0;
                if (qVentasR.exec() && qVentasR.first()) {
                    ventasAnualesR = qVentasR.value(0).toInt();
                }
                
                double ventasDiariasR = ventasAnualesR / 365.0;
                
                // Si la tienda apenas vende este producto, ignorar
                if (ventasDiariasR <= 0) continue;
                
                // 2. Obtener el stock actual remanente de la tienda remota para este EAN
                QSqlQuery qStockR(dbRemote);
                qStockR.prepare("SELECT SUM(cantidad) FROM lotes WHERE ean = ? AND cantidad > 0 AND fecha >= CURDATE()");
                qStockR.bindValue(0, rec.ean);
                double stockRemoto = 0;
                if (qStockR.exec() && qStockR.first()) {
                    stockRemoto = qStockR.value(0).toDouble();
                }
                
                // 3. Capacidad de Absorción
                // Días que la tienda remota tardará en liquidar su stock actual
                double diasAgotarPropio = stockRemoto / ventasDiariasR;
                
                // Días sobrantes desde que agotan su stock hasta nuestra fecha de caducidad útil
                double diasSobrantes = diasUtiles - diasAgotarPropio;
                
                if (diasSobrantes > 0) {
                    // Tienen un hueco de X días donde no tienen stock y pueden vender el nuestro
                    double capacidadAbsorcion = diasSobrantes * ventasDiariasR;
                    
                    if (capacidadAbsorcion > mejorCapacidadAbsorcion) {
                        mejorCapacidadAbsorcion = capacidadAbsorcion;
                        mejorTienda = nombresTiendasRemotas[connRemote];
                        ventasMesMejorTienda = ventasAnualesR / 12.0;
                    }
                }
            } // fin búsqueda tiendas remotas

            if (!mejorTienda.isEmpty()) {
                int cantidadMandar = qMin((int)qRound(excedenteLocal), (int)qRound(mejorCapacidadAbsorcion));
                if (cantidadMandar > 0) {
                    rec.sugerencia = QString("Traspasar %1 uds a %2 (Venden %3/mes y tienen déficit de stock)").arg(cantidadMandar).arg(mejorTienda).arg(ventasMesMejorTienda, 0, 'f', 1);
                } else {
                    rec.sugerencia = QString("Riesgo inminente. Sugerencia: Aplicar promoción. (Sobran: %1 uds)").arg(qRound(excedenteLocal));
                }
            } else {
                rec.sugerencia = QString("Riesgo inminente. Sugerencia: Aplicar promoción. (Sobran: %1 uds)").arg(qRound(excedenteLocal));
            }
            
            listaRecomendaciones.append(rec);
        }
    }
    
    // Limpiar y cerrar las conexiones remotas que abrimos
    for (const QString &connRemote : conexionesRemotasActivas) {
        QSqlDatabase::database(connRemote).close();
    }

    dbLocal.close();
    
    // Notificamos a la UI si hay recomendaciones (o lista vacía)
    emit analisisCompletado(listaRecomendaciones);
    emit finished();
}
