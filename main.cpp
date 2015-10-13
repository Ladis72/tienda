#include "tienda.h"
#include "conexion.h"
#include "configuracion.h"
#include <QApplication>

Configuracion *conf;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    conf = new Configuracion;
    if (!createConnection())
        return 1;

    Tienda w;
    w.show();
    
    return a.exec();
}
