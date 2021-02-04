#include "cercle.h"

Cercle::Cercle(int* xParam, int* yParam, int* rminParam, int* rmaxParam)
{
    x[0] = xParam[0];
    x[1] = xParam[1];

    y[0] = yParam[0];
    y[1] = yParam[1];

    minRadius[0] = rminParam[0];
    minRadius[1] = rminParam[1];

    maxRadius[0] = rmaxParam[0];
    maxRadius[1] = rmaxParam[1];
}

QString Cercle::getError()
{
    return error;
}
