#include "conexion.h"
#include "configuracion.h"
#include "login.h"
#include "tienda.h"
#include "dialogasistenteinstalacion.h"

#include <QApplication>
#include <QDir>
#include <QFile>

Configuracion *conf;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Fijar el directorio de trabajo en el directorio base compartido (p. ej. /opt/tienda)
    // para que las rutas relativas de imágenes y documentos funcionen desde cualquier lanzador
    QDir::setCurrent(Configuracion::rutaDirectorioBase());

    // Aplicar hoja de estilo global
    QFile styleFile(":/documentos/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
    }

    conf = new Configuracion;
    
    // Si no se puede establecer la conexión inicial (por falta de tienda.ini o error)
    while (!createConnection()) {
        DialogAsistenteInstalacion asistente;
        if (asistente.exec() == QDialog::Accepted) {
            // Reintentar la conexión con los nuevos parámetros guardados
            continue;
        } else {
            return 0; // El usuario canceló la instalación
        }
    }

    // Requerir autenticación previa antes de abrir la aplicación
    Login login;
    if (login.exec() != QDialog::Accepted) {
        return 0; // Si se pulsa Escape, Cancelar o se cierra la ventana, salir inmediatamente
    }

    Tienda w;
    w.show();

    return a.exec();
}

