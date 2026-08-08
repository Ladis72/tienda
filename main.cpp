#include "conexion.h"
#include "configuracion.h"
#include "login.h"
#include "tienda.h"
#include "dialogasistenteinstalacion.h"

#include <QApplication>
#include <QFile>

Configuracion *conf;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

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

    Tienda w;
    w.show();

    return a.exec();
}

