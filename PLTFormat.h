#pragma once

#include "Format.h"

class PLTFormat : public Format
{
    public:
        virtual std::string getBaseLayer();
        virtual void beginPolygon(std::string layer, bool isFirst);
        virtual void endPolygon(std::string layer, bool isFirst);
        virtual std::string output();
        virtual void beginPath();
        virtual void addPoint(double x, double y);
        virtual void endPath();
        
        virtual double getXRatio();
        virtual double getYRatio();

    protected:
        bool firstPoint;
        double fX, fY;
};
