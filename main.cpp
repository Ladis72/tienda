#include "tienda.h"
#include "conexion.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);


    if (!createConnection())
        return 1;

    Tienda w;
    w.show();
    
    return a.exec();
}
