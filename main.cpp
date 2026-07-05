#include "conexion.h"
#include "configuracion.h"
#include "login.h"
#include "tienda.h"
#include "configlocal.h"

#include <QApplication>
#include <QFile>

Configuracion *conf;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Apply global stylesheet
    QFile styleFile(":/documentos/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
    }

    conf = new Configuracion;
    
    // Si no se puede establecer la conexión inicial (por falta de tienda.ini o error)
    while (!createConnection()) {
        QMessageBox::StandardButton res = QMessageBox::question(
            nullptr, 
            "Configurar conexión",
            "¿Deseas configurar los parámetros de la base de datos local ahora?\n"
            "Si cancelas, la aplicación se cerrará.",
            QMessageBox::Yes | QMessageBox::No
        );
        
        if (res == QMessageBox::Yes) {
            ConfigLocal dial;
            // Si el usuario acepta e introduce los datos en el diálogo de configuración
            if (dial.exec() == QDialog::Accepted) {
                // Reintentar en el siguiente ciclo del bucle
                continue;
            } else {
                return 1; // El usuario canceló el diálogo de configuración
            }
        } else {
            return 1; // El usuario eligió no configurar y salir
        }
    }

    Tienda w;
    w.show();

    return a.exec();
}

