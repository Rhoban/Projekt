#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <MeshGroup.h>
#include <slicer.h>

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

        void openSTL(std::string filename, std::string plate);
        void addEngravure(double z, std::string color);
        void run();

    protected:
        double xMin, yMin, xMax, yMax;
        bool hasM;
        Polygons previous;
        Mesh mesh;
        std::vector<Engravure> engravures;

        Polygons slice(int z);
        void addLayer(std::stringstream &svg, bool isFirst, int z, std::string color);
};
