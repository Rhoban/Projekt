#pragma once

#include <sstream>
#include <string>
#include <vector>
#include <MeshGroup.h>
#include <slicer.h>

using namespace cura;

#define FORMAT_SVG	0
#define FORMAT_PLT	1

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
        void setOutput(std::string filename);
		void setOutputFormat(std::string format);
        void addEngravure(double z, std::string color);
		void setZExtra(double zExtra);
        void run();

    protected:
		int format;
        bool hasM;
        double xMin, yMin, xMax, yMax;
        std::string output;
        double zOffset;
		double zExtra;
        Polygons previous;
        Mesh mesh;
        std::vector<Engravure> engravures;

        Polygons slice(int z);
        void addLayer(std::stringstream &data, bool isFirst, int z, std::string color);
};
