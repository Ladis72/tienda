#include "printermanager.h"
#include <QDebug>
#include <QPainter>
#include <QBuffer>

// Qt6 requiere QtCore5Compat para QTextCodec
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QtCore5Compat/QTextCodec>
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
        qDebug() << "No se pudo abrir la impresora:" << m_impresora.errorString()
        << "Ruta:" << m_dispositivo;
        m_conectado = false;
        return false;
    }

    m_conectado = true;
    enviarComando(QByteArray::fromHex("1b40")); // ESC @ - Inicializar
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
    if (!isPrinterReady() && !abrirImpresora()) {
        return false;
    }

    qint64 bytesEscritos = m_impresora.write(comando);
    m_impresora.flush();

    if (bytesEscritos != comando.size()) {
        qDebug() << "Error al enviar comando: bytes esperados" << comando.size()
        << "bytes escritos" << bytesEscritos;
        return false;
    }
    return true;
}

QByteArray PrinterManager::convertirTexto(const QString &texto)
{
    QTextCodec *codec = QTextCodec::codecForName("IBM850");
    if (!codec) {
        codec = QTextCodec::codecForName("ISO-8859-1");
    }
    if (codec) {
        return codec->fromUnicode(texto);
    }
    return texto.toLocal8Bit();
}

bool PrinterManager::imprimirTexto(const QString &texto)
{
    return enviarComando(convertirTexto(texto));
}

bool PrinterManager::imprimirLinea(const QString &linea)
{
    return imprimirTexto(linea + "\n");
}

bool PrinterManager::imprimirLineaCentrada(const QString &linea)
{
    setAlineacion(1); // Centrar
    bool ok = imprimirLinea(linea);
    setAlineacion(0); // Volver a izquierda
    return ok;
}

bool PrinterManager::imprimirLineaDerecha(const QString &linea)
{
    setAlineacion(2); // Derecha
    bool ok = imprimirLinea(linea);
    setAlineacion(0); // Volver a izquierda
    return ok;
}

bool PrinterManager::imprimirLineaSeparadora(char caracter, int longitud)
{
    QString separador = QString(longitud, caracter);
    return imprimirLinea(separador);
}

void PrinterManager::setNegrita(bool activar)
{
    enviarComando(QByteArray::fromHex(activar ? "1b4501" : "1b4500"));
}

void PrinterManager::setTamanio(int ancho, int alto)
{
    ancho = qBound(1, ancho, 8);
    alto = qBound(1, alto, 8);
    int n = ((ancho - 1) << 4) | (alto - 1);

    QByteArray cmd;
    cmd.append(0x1D).append(0x21).append(static_cast<char>(n));
    enviarComando(cmd);
}

void PrinterManager::setAlineacion(int alineacion)
{
    alineacion = qBound(0, alineacion, 2);
    QByteArray cmd;
    cmd.append(0x1B).append(0x61).append(static_cast<char>(alineacion));
    enviarComando(cmd);
}

// 🔑 CORRECCIÓN CRÍTICA: El centrado se hace CON COMANDOS, no con padding de datos
bool PrinterManager::imprimirImagen(const QString &rutaImagen, bool centrado)
{
    QImage imagen(rutaImagen);
    if (imagen.isNull()) {
        qDebug() << "Error: No se pudo cargar la imagen:" << rutaImagen;
        return false;
    }
    return imprimirImagen(imagen, centrado);
}

// 🔑 CORRECCIÓN CRÍTICA: Centrado gestionado aquí, NO en procesarImagenParaImpresora
bool PrinterManager::imprimirImagen(const QImage &imagen, bool centrado)
{
    if (imagen.isNull()) return false;

    // 1. Preparar imagen en escala de grises y calcular dimensiones reales
    QImage img = imagen;
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }

    const int MAX_WIDTH = MAX_WIDTH_80MM; // Papel 58mm (ajustar a 576 para 80mm)
    int anchoReal = img.width();
    int altoReal = img.height();

    // Escalar MANTENIENDO PROPORCIÓN si excede el ancho máximo
    if (anchoReal > MAX_WIDTH) {
        altoReal = qRound(altoReal * (qreal)MAX_WIDTH / anchoReal);
        anchoReal = MAX_WIDTH;
        img = img.scaled(anchoReal, altoReal, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // 2. Centrado: calcular offset y enviar comando de posicionamiento ABSOLUTO
    if (centrado && anchoReal < MAX_WIDTH) {
        int offset = (MAX_WIDTH - anchoReal) / 2;

        // ESC $ nL nH - Posicionamiento absoluto en puntos (little-endian)
        QByteArray posCmd;
        posCmd.append(static_cast<char>(0x1B));  // ESC
        posCmd.append('$');
        posCmd.append(static_cast<char>(offset & 0xFF));       // nL (LSB)
        posCmd.append(static_cast<char>((offset >> 8) & 0xFF)); // nH (MSB)

        if (!enviarComando(posCmd)) {
            qDebug() << "Advertencia: Falló comando de centrado (offset:" << offset << ")";
        }
    }

    // 3. Enviar imagen PROCESADA (ya escalada, sin doble procesamiento)
    QByteArray datos = procesarImagenParaImpresora(img);
    if (datos.isEmpty()) {
        restaurarMargenIzquierdo(); // Restaurar posición incluso en error
        return false;
    }

    bool ok = enviarComando(datos);
    //alimentarLineas(1);

    // 4. Restaurar margen izquierdo para siguiente contenido
    restaurarMargenIzquierdo();

    return ok;
}

// Método auxiliar para evitar duplicar código de restauración
void PrinterManager::restaurarMargenIzquierdo()
{
    QByteArray cmd;
    cmd.append(static_cast<char>(0x1B)); // ESC
    cmd.append('$');
    cmd.append(static_cast<char>(0x00)); // nL = 0
    cmd.append(static_cast<char>(0x00)); // nH = 0
    enviarComando(cmd);
}

bool PrinterManager::imprimirLogoEmpresa(const QString &rutaLogo)
{
    QString logoPath = rutaLogo;

    if (logoPath.isEmpty()) {
        QStringList rutas = {
            ":/images/logo.png",
            ":/logo.png",
            "logo.png",
            "img/logo.png",
            "/usr/share/app/logo.png"
        };

        for (const QString &path : rutas) {
            if (QFile::exists(path)) {
                logoPath = path;
                break;
            }
        }
    }

    if (logoPath.isEmpty() || !QFile::exists(logoPath)) {
        qDebug() << "Advertencia: Logo no encontrado, imprimiendo texto alternativo";
        setNegrita(true);
        imprimirLineaCentrada("MI EMPRESA");
        setNegrita(false);
        imprimirLineaCentrada("www.miempresa.com");
        alimentarLineas(1);
        return true;
    }

    return imprimirImagen(logoPath, true);
}

bool PrinterManager::cortarPapel(bool parcial)
{
    QByteArray cmd;
    cmd.append(0x1D).append(0x56).append(parcial ? char(0x00) : char(0x01));
    return enviarComando(cmd);
}

bool PrinterManager::alimentarLineas(int lineas)
{
    lineas = qMax(1, lineas);
    return enviarComando(QByteArray(lineas, '\n'));
}

bool PrinterManager::abrirCajon()
{
    // ESC p m t1 t2 (m=0 para cajón 1, t1/t2 en décimas de segundo)
    QByteArray cmd;
    cmd.append(0x1B).append(0x70).append(char(0x00)).append(char(0x19)).append(char(0x19));
    return enviarComando(cmd);
}

// 🔑 FUNCIÓN CORREGIDA: Sin padding, sin distorsión, bits MSB primero
QByteArray PrinterManager::procesarImagenParaImpresora(const QImage &imagen)
{
    if (imagen.isNull()) {
        qDebug() << "Error: Imagen nula en procesarImagenParaImpresora";
        return QByteArray();
    }

    // 1. Convertir a RGB32 si es necesario
    QImage img = imagen;
    if (img.format() != QImage::Format_Grayscale8) {
        img = img.convertToFormat(QImage::Format_Grayscale8);
    }

    // 2. Redimensionar manteniendo proporción (máx 384px para 58mm)
    const int MAX_WIDTH = MAX_WIDTH_80MM;
    if (img.width() > MAX_WIDTH) {
        int newHeight = qRound(img.height() * (qreal)MAX_WIDTH / img.width());
        img = img.scaled(MAX_WIDTH, newHeight, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    }

    // 3. Padding a múltiplo de 8 SIN DISTORSIONAR (usar QPainter)
    int width = img.width();
    int height = img.height();
    int paddedWidth = ((width + 7) / 8) * 8;
    int bytesPerLine = paddedWidth / 8;

    QImage padded;
    if (paddedWidth != width) {
        padded = QImage(paddedWidth, height, QImage::Format_RGB32);
        padded.fill(Qt::white);
        QPainter painter(&padded);
        painter.drawImage(0, 0, img);
        painter.end();
    } else {
        padded = img;
    }

    width = padded.width();
//    int bytesPerLine = width / 8;

    // 4. Binarizar manualmente con control total de bits (MSB primero)
    QByteArray bits(bytesPerLine * height, 0x00);
    // bits.resize(bytesPerLine * height);
    // bits.fill(0x00); // Blanco = todos los bits a 1

    for (int y = 0; y < height; ++y) {
        const uchar *scanLine = img.constScanLine(y);
        for (int x = 0; x < width; ++x) {
            if (scanLine[x] < 200) { // Negro
                int byteIdx = y * bytesPerLine + (x / 8);
                int bitPos = 7 - (x % 8); // MSB primero
                bits[byteIdx] |= (1 << bitPos); // ✅ Activar bit para imprimir
            }
        }
        // ✅ Bytes adicionales por padding permanecen en 0x00 (blanco)
    }

    // 5. Comando ESC/POS: GS v 0 m xL xH yL yH d1...dk
    // m=1: modo doble densidad (mejor calidad)
    QByteArray cmd;
    cmd.append(0x1D).append(0x76).append(0x30);
    cmd.append(static_cast<char>(0x00)); // GS v 0 m=1
    cmd.append(static_cast<char>(bytesPerLine & 0xFF));      // xL
    cmd.append(static_cast<char>((bytesPerLine >> 8) & 0xFF)); // xH
    cmd.append(static_cast<char>(height & 0xFF));            // yL
    cmd.append(static_cast<char>((height >> 8) & 0xFF));     // yH
    cmd.append(bits);

    qDebug() << "Imagen procesada:" << width << "x" << height << "px,"
             << bytesPerLine << "bytes/linea," << cmd.size() << "bytes totales";

    // DEBUG: Guardar imagen binaria para verificación (solo desarrollo)
    // QImage debug(width, height, QImage::Format_Mono);
    // debug.setColorCount(2);
    // debug.setColor(0, qRgb(0,0,0));
    // debug.setColor(1, qRgb(255,255,255));
    // for (int y=0; y<height; ++y) {
    //     uchar *line = debug.scanLine(y);
    //     memcpy(line, bits.constData() + y*bytesPerLine, bytesPerLine);
    // }
    // debug.save("debug_bw.png");

    return cmd;
}
