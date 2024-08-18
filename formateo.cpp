#include "formateo.h"
#include <QDebug>

Formateo::Formateo() {}

double Formateo::redondear(double num, int dec)
{
    {
        if (num == 0)
            return 0;
        double z;
        int a, c, b, cont = 1;
        c = dec;

        for (int i = 0; i <= dec; i++)
            cont = cont * 10;

        if (c != num) {
            a = num * cont;
            z = a / 10;
            b = a % 10;
            cont = cont / 10;
            if (b >= 5) {
                z = z + 1;
            }
            num = z / cont;
        }

        return num;
    }
}
