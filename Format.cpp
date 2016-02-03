#include "Format.h"

void Format::clear()
{
    stream.clear();
}

void Format::beginPolygon(std::string layer, bool isFirst)
{
}

void Format::endPolygon(std::string layer, bool isFirst)
{
}

virtual void Format::beginPath()
{
}

virtual void Format::endPath()
{
}

double Format::getXRatio()
{
    return 1.0;
}

double Format::getYRatio()
{
    return 1.0;
}

void Format::registerPoint(double x, double y)
{
    if (!hasM) {
        hasM = true;
        xMin = X; yMin = Y;
        xMax = X; yMax = Y;
    } else {
        if (X < xMin) xMin = X;
        if (Y < yMin) yMin = Y;
        if (X > xMax) xMax = X;
        if (Y > yMax) yMax = Y;
    }
}
