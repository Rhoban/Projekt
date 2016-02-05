#include <iostream>
#include "SVGFormat.h"
        
std::string SVGFormat::getBaseLayer()
{
    return "red";
}

std::string SVGFormat::output()
{
    std::stringstream outstr;

    double width = xMax-xMin;
    double height = yMax-yMin;

    outstr << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" << xMin << 
        " " << yMin << " " << width << " " << height << "\">" << std::endl;
    outstr << stream.str();
    outstr << "</svg>" << std::endl;

    return outstr.str();
}

void SVGFormat::beginPolygon(std::string layer, bool isFirst)
{
    circles.clear();
    stream << "<path d=\"";
}

void SVGFormat::appendStroke(std::string layer, bool isFirst)
{
    //if (isFirst) {
        stream << " stroke=\"" << layer << "\" fill=\"none\" stroke-width=\"0.1\" />";
    //} else {
    //    stream << " stroke=\"\" fill=\"" << layer << "\" stroke-width=\"0.1\" />";
    // }
}

void SVGFormat::endPolygon(std::string layer, bool isFirst)
{
    stream << "\""; // This closes the path tag
    appendStroke(layer, isFirst);

    for (auto circle : circles) {
        stream << "<circle cx=\"" << circle.x << "\" cy=\"" << circle.y << "\" r=\"" << circle.r << "\" ";
        appendStroke(layer, isFirst);
    }
    
    stream << std::endl;
}

void SVGFormat::beginPath()
{
    firstPoint = true;
    stream << "M ";
}

void SVGFormat::addPoint(double x, double y)
{
    if (firstPoint) {
        firstPoint = false;
    } else {
        stream << "L ";
    }
            
    stream << x << " " << y << " ";
}

void SVGFormat::endPath()
{
    stream << "z " << std::endl;
}

bool SVGFormat::supportsCircle()
{
    return true;
}

void SVGFormat::addCircle(std::string layer, double x, double y, double r)
{
    Circle c;
    c.x = x;
    c.y = y;
    c.r = r;
    circles.push_back(c);
}
        
double SVGFormat::getXRatio()
{
    return 3.543307/1000.0;
}

double SVGFormat::getYRatio()
{
    return -getXRatio();
}
