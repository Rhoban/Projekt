#pragma once

#include <map>
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
		void setRepeat(std::string layer, int count, double spacing);
		void setPOffset(double pOffset);
        void run();

    protected:
		int format;
        bool hasM;
        double xMin, yMin, xMax, yMax;
        std::string output;
        double zOffset;
		double zExtra;
		double pOffset;
        Polygons previous;
        Mesh mesh;
        std::vector<Engravure> engravures;
		std::map<std::string, OffsetRepeat> repeats;

        Polygons slice(int z);
		void addPolygon(std::stringstream &data, Polygons polygon, bool isFirst, std::string color);
        void addLayer(std::stringstream &data, bool isFirst, int z, std::string color);
};
