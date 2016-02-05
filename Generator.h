#pragma once

#include <map>
#include <sstream>
#include <string>
#include <vector>
#include <MeshGroup.h>
#include <slicer.h>
#include "Format.h"

using namespace cura;

class Generator
{
    public:
        Generator();

        struct Engravure
        {
            double z;
            std::string color;
        };

        struct OffsetRepeat
        {
            int count;
            double spacing;
        };

        void openSTL(std::string filename, std::string plate);
        void setOutput(std::string filename);
        void setOutputFormat(std::string format);
        void addEngravure(double z, std::string color);
        void setZExtra(double zExtra);
        void setPOffset(double pOffset);
        void setAreaTreshold(double treshold);
        void setRepeat(std::string layer, int count, double spacing);
        bool detectCircle(PolygonRef polygon, double &x, double &y, double &r);
        void run();

        bool circleDetection;

    protected:
        Format *format;
        std::string output;
        double areaTreshold;
        double zOffset;
        double zExtra;
        double pOffset;
        Polygons previous;
        Mesh mesh;
        std::vector<Engravure> engravures;
        std::map<std::string, OffsetRepeat> repeats;

        Polygons slice(int z);
        void addPolygon(std::stringstream &data, Polygons polygon, bool isFirst, std::string layer);
        void addLayer(std::stringstream &data, bool isFirst, int z, std::string color);
};
