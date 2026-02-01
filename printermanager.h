// printermanager.h (igual que antes)
#ifndef PRINTERMANAGER_H
#define PRINTERMANAGER_H

#include <QObject>
#include <QFile>
#include <QImage>
#include <QPainter>
#include <QByteArray>

class PrinterManager : public QObject
{
    Q_OBJECT
public:
    explicit PrinterManager(QObject *parent = nullptr);
    ~PrinterManager();

    bool abrirImpresora();
    void cerrarImpresora();
    bool isPrinterReady() const;

    // Métodos de impresión
    bool imprimirTexto(const QString &texto);
    bool imprimirLinea(const QString &linea);
    bool imprimirLineaCentrada(const QString &linea);
    bool imprimirLineaDerecha(const QString &linea);
    bool imprimirLineaSeparadora(char caracter = '-', int longitud = 40);

    // Formato
    void setNegrita(bool activar);
    void setTamanio(int ancho, int alto);
    void setAlineacion(int alineacion);

    // Imágenes
    bool imprimirImagen(const QString &rutaImagen, bool centrado = true);
    bool imprimirImagen(const QImage &imagen, bool centrado = true);
    bool imprimirLogoEmpresa(const QString &rutaLogo = "");

    // Comandos especiales
    bool cortarPapel(bool parcial = true);
    bool alimentarLineas(int lineas = 3);
    bool abrirCajon();
    bool enviarComando(const QByteArray &comando);

private:
    QFile m_impresora;
    QString m_dispositivo;
    bool m_conectado;

    QByteArray convertirTexto(const QString &texto);
    QByteArray procesarImagenParaImpresora(const QImage &imagen, bool centrado);
};

#endif // PRINTERMANAGER_H
