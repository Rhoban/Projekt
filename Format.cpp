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

void Format::beginPath()
{
}

void Format::endPath()
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
        xMin = x; yMin = y;
        xMax = x; yMax = y;
    } else {
        if (x < xMin) xMin = x;
        if (y < yMin) yMin = y;
        if (x > xMax) xMax = x;
        if (y > yMax) yMax = y;
    }
}
