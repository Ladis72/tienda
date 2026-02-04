// printermanager.cpp - VERSIÓN CORREGIDA SIN QTEXTCODEC
#include "printermanager.h"
#include <QDebug>
#include <QBuffer>
#include <QStringConverter>

// Si estás en Qt6, necesitas incluir QTextCodec desde core5compat
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QTextCodec>
#else
#include <QTextCodec>
#endif

PrinterManager::PrinterManager(QObject *parent)
    : QObject(parent), m_dispositivo("/dev/usb/lp0"), m_conectado(false)
{
}

PrinterManager::~PrinterManager()
{
    cerrarImpresora();
}

bool PrinterManager::abrirImpresora()
{
    if (m_conectado) {
        cerrarImpresora();
    }

    m_impresora.setFileName(m_dispositivo);
    if (!m_impresora.open(QIODevice::WriteOnly)) {
        qDebug() << "No se pudo abrir la impresora:" << m_impresora.errorString();
        m_conectado = false;
        return false;
    }

    m_conectado = true;

    // Inicializar impresora
    enviarComando(QByteArray::fromHex("1b40")); // ESC @ - Initialize
    return true;
}

void PrinterManager::cerrarImpresora()
{
    if (m_impresora.isOpen()) {
        m_impresora.close();
    }
    m_conectado = false;
}

bool PrinterManager::isPrinterReady() const
{
    return m_conectado && m_impresora.isOpen();
}

bool PrinterManager::enviarComando(const QByteArray &comando)
{
    if (!isPrinterReady()) {
        if (!abrirImpresora()) {
            return false;
        }
    }

    qint64 bytesEscritos = m_impresora.write(comando);
    m_impresora.flush();

    return bytesEscritos == comando.size();
}

QByteArray PrinterManager::convertirTexto(const QString &texto)
{
    // MÉTODO 1: Usar QTextCodec (Qt5 y Qt6 con core5compat)
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    // En Qt6, QTextCodec está en Qt::Core5Compat
    QTextCodec *codec = QTextCodec::codecForName("IBM850");
    if (!codec) {
        codec = QTextCodec::codecForName("Latin-1");
    }

    if (codec) {
        return codec->fromUnicode(texto);
    }

    // Fallback: usar toLocal8Bit
    return texto.toLocal8Bit();

#else
    // En Qt5
    QTextCodec *codec = QTextCodec::codecForName("IBM850");
    if (!codec) {
        codec = QTextCodec::codecForName("Latin-1");
    }

    if (codec) {
        return codec->fromUnicode(texto);
    }

    return texto.toLocal8Bit();
#endif
}

bool PrinterManager::imprimirTexto(const QString &texto)
{
    QByteArray datos = convertirTexto(texto);
    return enviarComando(datos);
}

bool PrinterManager::imprimirLinea(const QString &linea)
{
    return imprimirTexto(linea + "\n");
}

bool PrinterManager::imprimirLineaCentrada(const QString &linea)
{
    // ESC a 1 - Center alignment
    enviarComando(QByteArray::fromHex("1b6101"));
    bool resultado = imprimirLinea(linea);
    // ESC a 0 - Left alignment
    enviarComando(QByteArray::fromHex("1b6100"));
    return resultado;
}

bool PrinterManager::imprimirLineaDerecha(const QString &linea)
{
    // ESC a 2 - Right alignment
    enviarComando(QByteArray::fromHex("1b6102"));
    bool resultado = imprimirLinea(linea);
    // ESC a 0 - Left alignment
    enviarComando(QByteArray::fromHex("1b6100"));
    return resultado;
}

bool PrinterManager::imprimirLineaSeparadora(char caracter, int longitud)
{
    QString separador = QString(longitud, caracter);
    return imprimirLinea(separador);
}

void PrinterManager::setNegrita(bool activar)
{
    if (activar) {
        enviarComando(QByteArray::fromHex("1b4501")); // ESC E 1 - Bold on
    } else {
        enviarComando(QByteArray::fromHex("1b4500")); // ESC E 0 - Bold off
    }
}

void PrinterManager::setTamanio(int ancho, int alto)
{
    // GS ! n - Set character size
    ancho = qBound(1, ancho, 8);
    alto = qBound(1, alto, 8);

    int anchoVal = (ancho - 1) * 16;
    int altoVal = alto - 1;
    int n = anchoVal | altoVal;

    QByteArray comando;
    comando.append(0x1D); // GS
    comando.append(0x21); // !
    comando.append(static_cast<char>(n));

    enviarComando(comando);
}

void PrinterManager::setAlineacion(int alineacion)
{
    alineacion = qBound(0, alineacion, 2);

    QByteArray comando;
    comando.append(0x1B); // ESC
    comando.append(0x61); // a
    comando.append(static_cast<char>(alineacion));

    enviarComando(comando);
}

bool PrinterManager::imprimirImagen(const QString &rutaImagen, bool centrado)
{
    QImage imagen(rutaImagen);
    if (imagen.isNull()) {
        qDebug() << "No se pudo cargar la imagen:" << rutaImagen;
        return false;
    }

    return imprimirImagen(imagen, centrado);
}

bool PrinterManager::imprimirImagen(const QImage &imagen, bool centrado)
{
    QByteArray datosImagen = procesarImagenParaImpresora(imagen, centrado);
    if (datosImagen.isEmpty()) {
        return false;
    }

    return enviarComando(datosImagen);
}

bool PrinterManager::imprimirLogoEmpresa(const QString &rutaLogo)
{
    QString logoPath = rutaLogo;

    if (logoPath.isEmpty()) {
        // Buscar logo por defecto
        QStringList posiblesLogos = {
            "logo.png", "logo.jpg", "logo.bmp",
            "img/logo.png", "img/logo.jpg",
            "/usr/share/miempresa/logo.png"
        };

        for (const QString &path : posiblesLogos) {
            if (QFile::exists(path)) {
                logoPath = path;
                break;
            }
        }
    }

    if (logoPath.isEmpty() || !QFile::exists(logoPath)) {
        qDebug() << "Logo no encontrado";
        return false;
    }

    return imprimirImagen(logoPath, true);
}

bool PrinterManager::cortarPapel(bool parcial)
{
    // GS V m - Cut paper
    QByteArray comando;
    comando.append(0x1D); // GS
    comando.append(0x56); // V
    comando.append(parcial ? char(0x00) : char(0x01));

    return enviarComando(comando);
}

bool PrinterManager::alimentarLineas(int lineas)
{
    lineas = qMax(1, lineas);
    QByteArray lineasBytes(lineas, '\n');
    return enviarComando(lineasBytes);
}

bool PrinterManager::abrirCajon()
{
    // ESC p m t1 t2 - Pulse
    QByteArray comando;
    comando.append(0x1B); // ESC
    comando.append(0x70); // p
    comando.append(char(0x00)); // m=0
    comando.append(char(0x19)); // t1=25
    comando.append(char(0x19)); // t2=25

    return enviarComando(comando);
}

QByteArray PrinterManager::procesarImagenParaImpresora(const QImage &imagen, bool centrado)
{
    // 1. Redimensionar para impresora térmica (384px ancho para 58mm)
    const int ANCHO_MAXIMO = 384;

    QImage img = imagen;
    if (img.width() > ANCHO_MAXIMO || img.width() < ANCHO_MAXIMO) {
        // Redimensionar manteniendo aspecto
        int nuevoAlto = (img.height() * ANCHO_MAXIMO) / img.width();
        img = img.scaled(ANCHO_MAXIMO, nuevoAlto, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // Convertir a escala de grises
    img = img.convertToFormat(QImage::Format_Grayscale8);

    // Crear imagen binaria
    QImage imgBinaria(img.size(), QImage::Format_Mono);
    imgBinaria.fill(1); // 1 = blanco en QImage::Format_Mono

    // Binarizar (umbral 128)
    for (int y = 0; y < img.height(); ++y) {
        const uchar *line = img.constScanLine(y);
        uchar *binLine = imgBinaria.scanLine(y);

        for (int x = 0; x < img.width(); ++x) {
            if (line[x] < 128) { // Pixel negro
                // En QImage::Format_Mono, cada pixel es un bit
                // 0 = negro, 1 = blanco
                int byteIndex = x / 8;
                int bitIndex = 7 - (x % 8); // MSB first
                binLine[byteIndex] &= ~(1 << bitIndex); // Poner a 0 (negro)
            }
        }
    }

    // Preparar datos ESC/POS
    const int altura = imgBinaria.height();
    const int ancho = imgBinaria.width();

    // Bytes por línea (1 byte = 8 pixels)
    const int bytesPorLinea = (ancho + 7) / 8;
    const int bytesMaximos = ANCHO_MAXIMO / 8; // 48 bytes

    QByteArray datosImagen;

    // Comando gráfico ESC/POS: GS v 0
    datosImagen.append(0x1D); // GS
    datosImagen.append(0x76); // v
    datosImagen.append(0x30); // 0

    // Dimensiones
    int xL = bytesMaximos % 256;
    int xH = bytesMaximos / 256;
    int yL = altura % 256;
    int yH = altura / 256;

    datosImagen.append(static_cast<char>(xL));
    datosImagen.append(static_cast<char>(xH));
    datosImagen.append(static_cast<char>(yL));
    datosImagen.append(static_cast<char>(yH));

    // Generar datos de imagen
    for (int y = 0; y < altura; ++y) {
        QByteArray lineaBytes(bytesMaximos, 0x00); // Inicializar con ceros (blanco)

        const uchar *lineaBits = imgBinaria.constScanLine(y);

        if (centrado) {
            // Calcular margen para centrar
            int margenBytes = (bytesMaximos - bytesPorLinea) / 2;

            // Copiar bytes de la imagen al centro
            for (int i = 0; i < bytesPorLinea; ++i) {
                if (i < bytesPorLinea) {
                    lineaBytes[margenBytes + i] = lineaBits[i];
                }
            }
        } else {
            // Sin centrar
            for (int i = 0; i < qMin(bytesPorLinea, bytesMaximos); ++i) {
                lineaBytes[i] = lineaBits[i];
            }
        }

        datosImagen.append(lineaBytes);
    }

    return datosImagen;
}
