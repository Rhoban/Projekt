#include "DXFFormat.h"
        
std::string DXFFormat::getBaseLayer()
{
    return "0";
}

std::string DXFFormat::output()
{
    std::stringstream outstr;
    
    outstr << "999" << std::endl;
    outstr << "DXF created from Projekt" << std::endl;

    outstr << "0" << std::endl;
    outstr << "SECTION" << std::endl;
    outstr << "2" << std::endl;
    outstr << "HEADER" << std::endl;
    outstr << "9" << std::endl;
    outstr << "$ACADVER" << std::endl;
    outstr << "1" << std::endl;
    outstr << "AC1006" << std::endl;
	outstr << "9" << std::endl;
    outstr << "$INSUNITS" << std::endl;
	outstr << "70" << std::endl;
    outstr << "4" << std::endl;
    outstr << "9" << std::endl;
    outstr << "$INSBASE" << std::endl;
    outstr << "10" << std::endl;
    outstr << "0.0" << std::endl;
    outstr << "20" << std::endl;
    outstr << "0.0" << std::endl;
    outstr << "30" << std::endl;
    outstr << "0.0" << std::endl;
    outstr << "9" << std::endl;
    outstr << "$EXTMIN" << std::endl;
    outstr << "10" << std::endl;
    outstr << xMin << std::endl;
    outstr << "20" << std::endl;
    outstr << yMin << std::endl;
    outstr << "9" << std::endl;
    outstr << "$EXTMAX" << std::endl;
    outstr << "10" << std::endl;
    outstr << xMax << std::endl;
    outstr << "20" << std::endl;
    outstr << yMax << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDSEC" << std::endl;

    outstr << "0" << std::endl;
    outstr << "SECTION" << std::endl;
    outstr << "2" << std::endl;
    outstr << "TABLES" << std::endl;
    outstr << "0" << std::endl;
    outstr << "TABLE" << std::endl;
    outstr << "2" << std::endl;
    outstr << "LTYPE" << std::endl;
    outstr << "70" << std::endl;
    outstr << "1" << std::endl;
    outstr << "0" << std::endl;
    outstr << "LTYPE" << std::endl;
    outstr << "2" << std::endl;
    outstr << "CONTINUOUS" << std::endl;
    outstr << "70" << std::endl;
    outstr << "64" << std::endl;
    outstr << "3" << std::endl;
    outstr << "Solid line" << std::endl;
    outstr << "72" << std::endl;
    outstr << "65" << std::endl;
    outstr << "73" << std::endl;
    outstr << "0" << std::endl;
    outstr << "40" << std::endl;
    outstr << "0.000000" << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDTAB" << std::endl;

    outstr << "0" << std::endl;
    outstr << "TABLE" << std::endl;
    outstr << "2" << std::endl;
    outstr << "LAYER" << std::endl;
    outstr << "70" << std::endl;
    outstr << "6" << std::endl;
    outstr << "0" << std::endl;
    outstr << "LAYER" << std::endl;
    outstr << "2" << std::endl;
    outstr << "1" << std::endl;
    outstr << "70" << std::endl;
    outstr << "64" << std::endl;
    outstr << "62" << std::endl;
    outstr << "7" << std::endl;
    outstr << "6" << std::endl;
    outstr << "CONTINUOUS" << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDTAB" << std::endl;
    outstr << "0" << std::endl;
    outstr << "TABLE" << std::endl;
    outstr << "2" << std::endl;
    outstr << "STYLE" << std::endl;
    outstr << "70" << std::endl;
    outstr << "0" << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDTAB" << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDSEC" << std::endl;

    outstr << "0" << std::endl;
    outstr << "SECTION" << std::endl;
    outstr << "2" << std::endl;
    outstr << "BLOCKS" << std::endl;
    outstr << "0" << std::endl;
    outstr << "ENDSEC" << std::endl;

    outstr << "0" << std::endl;
    outstr << "SECTION" << std::endl;
    outstr << "2" << std::endl;
    outstr << "ENTITIES" << std::endl;
    outstr << stream.str();		
    outstr << "0" << std::endl;
    outstr << "ENDSEC" << std::endl;
    outstr << "0" << std::endl;
    outstr << "EOF" << std::endl;

    return outstr.str();
}

void DXFFormat::beginPolygon(std::string layer, bool isFirst)
{
    currentLayer = layer;
}

void DXFFormat::endPolygon(std::string layer, bool isFirst)
{
}

void DXFFormat::beginPath()
{
    firstPoint = true;
}

void DXFFormat::addPoint(double x, double y)
{
    if (firstPoint) {
        lX = fX = x;
        lY = fY = y;
        firstPoint = false;
    } else {
        stream << "0" << std::endl;
        stream << "LINE" << std::endl;
        stream << "8" << std::endl;
        stream << "1" << std::endl;
        stream << "10" << std::endl;
        stream << lX << std::endl;
        stream << "20" << std::endl;
        stream << lY << std::endl;
        stream << "11" << std::endl;
        stream << x << std::endl;
        stream << "21" << std::endl;
        stream << y << std::endl;
        stream << "62" << std::endl;
        stream << currentLayer << std::endl;

        lX = x;
        lY = y;
    }
}

void DXFFormat::endPath()
{
    if (!firstPoint) {
        stream << "0" << std::endl;
        stream << "LINE" << std::endl;
        stream << "8" << std::endl;
        stream << "1" << std::endl;
        stream << "10" << std::endl;
        stream << lX << std::endl;
        stream << "20" << std::endl;
        stream << lY << std::endl;
        stream << "11" << std::endl;
        stream << fX << std::endl;
        stream << "21" << std::endl;
        stream << fY << std::endl;
        stream << "62" << std::endl;
        stream << currentLayer << std::endl;
    }
}
        
bool DXFFormat::supportsCircle()
{
    return true;
}

void DXFFormat::addCircle(std::string layer, double x, double y, double r)
{
    stream << "0" << std::endl;
    stream << "CIRCLE" << std::endl;
    stream << "8" << std::endl;
    stream << "1" << std::endl;
    stream << "10" << std::endl;
    stream << x << std::endl;
    stream << "20" << std::endl;
    stream << y << std::endl;
    stream << "40" << std::endl;
    stream << r << std::endl;
    stream << "62" << std::endl;
    stream << layer << std::endl;
}
