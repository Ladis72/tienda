#ifndef VERIFACTUCLASS_H
#define VERIFACTUCLASS_H

#include <QObject>
#include <QString>
#include <QDateTime>
#include <QImage>

/**
 * @brief Estructura de configuración local para VeriFactu (SEC-01).
 * Almacena el modo, entorno de la AEAT, datos del emisor y certificado digital.
 */
struct VeriFactuConfig {
    int modo;              // 0 = Desactivado, 1 = VERI*FACTU (remisión), 2 = No VERI*FACTU (local)
    int entorno;           // 0 = Pruebas (Pre-producción), 1 = Producción
    QString emisorNif;     // NIF del emisor
    QString emisorNombre;  // Razón social del emisor
    QString certificadoRuta; // Ruta del fichero de certificado (.p12/.pfx)
    QString certificadoClave; // Contraseña del certificado
};

/**
 * @brief Clase de soporte para el cumplimiento de VeriFactu (Ley Antifraude de la AEAT).
 * 
 * Implementa la generación de XML de alta, el encadenamiento de huellas (hashing SHA-256)
 * según especificaciones técnicas oficiales de la AEAT, la generación de códigos QR
 * y la comunicación telemática HTTPS mediante SOAP.
 */
class verifactuClass : public QObject
{
    Q_OBJECT
public:
    explicit verifactuClass(QObject *parent = nullptr);

    /// Carga la configuración desde tienda.ini de manera limpia (SEC-01)
    static VeriFactuConfig cargarConfiguracion();

    /// Formatea decimales sin ceros superfluos a la derecha del punto decimal, según AEAT
    static QString formatearDecimalAEAT(double valor);

    /// Calcula la huella SHA-256 encadenada de un registro de facturación de alta
    static QString calcularHuellaAlta(const QString &nifEmisor,
                                      const QString &numSerieFactura,
                                      const QString &fechaExpedicion,
                                      const QString &tipoFactura,
                                      const QString &cuotaTotal,
                                      const QString &importeTotal,
                                      const QString &huellaAnterior,
                                      const QString &fechaHoraGen);

    /// Genera la cadena XML para el envío de alta de facturación según XSD oficial
    static QString generarXmlAlta(const VeriFactuConfig &config,
                                  const QString &numSerieFactura,
                                  const QString &fechaExpedicion,
                                  const QString &horaExpedicion,
                                  double totalBase,
                                  double totalIva,
                                  double totalImporte,
                                  const QString &tipoFactura,
                                  const QString &huellaAnterior,
                                  const QString &hashActual,
                                  const QString &fechaHoraGen,
                                  const QString &numSerieAnterior = QString(),
                                  const QString &fechaExpedicionAnterior = QString());

    /// Envía el XML al web service SOAP de la AEAT mediante HTTPS y cliente certificado
    static bool enviarAEAT(const QString &xmlContent, const VeriFactuConfig &config, QString &errStr);

    /// Procesa y reenvía en segundo plano los registros pendientes (estado_envio = 0)
    static void procesarEnviosPendientes(const QString &conexionOriginal);

    /// Genera la URL de cotejo que irá en el código QR
    static QString generarUrlQR(const QString &nifEmisor,
                                const QString &numSerieFactura,
                                const QString &fechaExpedicion,
                                double importeTotal,
                                int entorno);

    /// Genera una imagen QImage del código QR a partir de una URL usando libqrencode
    static QImage generarCodigoQR(const QString &url);

    /// Genera la fecha/hora en formato ISO-8601 con el huso horario local (ej: "2026-06-06T22:45:00+02:00")
    static QString obtenerFechaHoraHusoActual();
};

#endif // VERIFACTUCLASS_H
