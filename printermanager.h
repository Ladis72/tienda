#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <QObject>
#include <QFile>
#include <QImage>

class PrinterManager : public QObject
{
    Q_OBJECT

public:
    explicit PrinterManager(QObject *parent = nullptr);
    ~PrinterManager();

    // Configuración
    void setDevicePath(const QString &path) { m_dispositivo = path; }
    QString devicePath() const { return m_dispositivo; }

    // Conexión
    bool abrirImpresora();
    void cerrarImpresora();
    bool isPrinterReady() const;

    // Comandos básicos
    bool enviarComando(const QByteArray &comando);
    bool imprimirTexto(const QString &texto);
    bool imprimirLinea(const QString &linea);
    bool imprimirLineaCentrada(const QString &linea);
    bool imprimirLineaDerecha(const QString &linea);
    bool imprimirLineaSeparadora(char caracter = '-', int longitud = 48);

    // Formato
    void setNegrita(bool activar);
    void setTamanio(int ancho, int alto); // 1-8 para cada dimensión
    void setAlineacion(int alineacion);   // 0=izq, 1=centro, 2=der

    // Imágenes
    bool imprimirImagen(const QString &rutaImagen, bool centrado = false);
    bool imprimirImagen(const QImage &imagen, bool centrado = false);
    bool imprimirLogoEmpresa(const QString &rutaLogo = "");

    // Control de papel
    bool cortarPapel(bool parcial = false);
    bool alimentarLineas(int lineas = 3);

    // Periféricos
    bool abrirCajon();

    void restaurarMargenIzquierdo();
private:
    QByteArray convertirTexto(const QString &texto);
    QByteArray procesarImagenParaImpresora(const QImage &imagen); // ¡SIN centrado aquí!

    QFile m_impresora;
    QString m_dispositivo;
    bool m_conectado;

    // Constantes
    static constexpr int MAX_WIDTH_58MM = 384;  // Píxeles para papel de 58mm
    static constexpr int MAX_WIDTH_80MM = 576;  // Píxeles para papel de 80mm
};

#endif // PRINTERMANAGER_H
