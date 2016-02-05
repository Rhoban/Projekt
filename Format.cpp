#include "Format.h"
        
Format::~Format()
{
}

void Format::clear()
{
	hasM = false;
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
    return 1.0/1000.0;
}

double Format::getYRatio()
{
    return 1.0/1000.0;
}
        
bool Format::supportsCircle()
{
    return false;
}

void Format::addCircle(std::string layer, double x, double y, double r)
{
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
