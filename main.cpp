#include "conexion.h"
#include "configuracion.h"
#include "login.h"
#include "tienda.h"

#include <QApplication>
#include <QFile>

Configuracion *conf;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // Apply global stylesheet
    QFile styleFile("/home/ladis/AndroidStudioProjects/tienda/style.qss");
    if (styleFile.open(QFile::ReadOnly)) {
        QString styleSheet = QLatin1String(styleFile.readAll());
        a.setStyleSheet(styleSheet);
    }

    conf = new Configuracion;
    if (!createConnection())
        return 1;

    Tienda w;
    w.show();

    return a.exec();
}
