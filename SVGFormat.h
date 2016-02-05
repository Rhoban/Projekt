#pragma once

#include <vector>
#include "Format.h"

class SVGFormat : public Format
{
    public:
        virtual std::string getBaseLayer();
        virtual void beginPolygon(std::string layer, bool isFirst);
        virtual void endPolygon(std::string layer, bool isFirst);
        virtual std::string output();
        
        virtual void beginPath();
        virtual void addPoint(double x, double y);
        virtual void endPath();
        virtual bool supportsCircle();
        virtual void addCircle(std::string layer, double x, double y, double r);
        
        virtual double getXRatio();
        virtual double getYRatio();

        struct Circle {
            double x, y, r;
        };

    protected:
        std::vector<Circle> circles;
        bool firstPoint;

        void appendStroke(std::string layer, bool isFirst);
};
