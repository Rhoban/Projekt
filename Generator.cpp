#include <unistd.h>
#include "Generator.h"

Generator::Generator()
    : mesh(nullptr)
{
}

void Generator::openSTL(std::string filename, std::string plate)
{
    MeshGroup meshgroup(nullptr);
    FMatrix3x3 mat;
    if (plate == "xz") {
        mat.m[1][1] = 0;
        mat.m[2][2] = 0;
        mat.m[1][2] = -1;
        mat.m[2][1] = 1;
    }
    loadMeshIntoMeshGroup(&meshgroup, filename.c_str(), mat);

    if (meshgroup.meshes.size() != 1) {
        std::cerr << "Unable to open " << filename << std::endl;
        exit(0);
    } else {
        mesh = meshgroup.meshes[0];

        // (Hacky), close stderr and set settings to avoid warnings
        int tmp = dup(2);
        close(2);
        mesh.setSetting("magic_mesh_surface_mode", "0");
        mesh.setSetting("xy_offset", "0");
        dup(tmp);
    }
}
        
void Generator::addEngravure(double z, std::string color)
{
    Engravure e;
    e.z = z;
    e.color = color;
    engravures.push_back(e);
}

Polygons Generator::slice(int z)
{
    Slicer slicer(&mesh, z, 200000, 1, false, false);

    if (slicer.layers.size() == 1) {
        return slicer.layers[0].polygonList;
    } else {
        Polygons result;
        return result;
    }
}
        
void Generator::addLayer(std::stringstream &svg, bool isFirst, int z, std::string color)
{
    auto sliced = slice(z);
    auto polygon = sliced;

    if (!isFirst) {
        polygon = previous.difference(polygon.offset(10));
    }

    svg << "<path d=\"";
    for (auto path : polygon) {
        svg << "M ";
        bool first = true;
        for (auto point : path) {
            double X = point.X*3.543307/1000.0;
            double Y = point.Y*3.543307/1000.0;

            if (!hasM) {
                hasM = true;
                xMin = X; yMin = Y;
                xMax = X; yMax = Y;
            } else {
                if (X < xMin) xMin = X;
                if (Y < yMin) yMin = Y;
                if (X > xMax) xMax = X;
                if (Y > yMax) yMax = Y;
            }

            if (first) {
                first = false;
            } else {
                svg << "L ";
            }
            svg << X << " " << Y << " ";
        }
        svg << "z " << std::endl;
    }
    if (isFirst) {
        svg << "\" stroke=\"" << color << "\" fill=\"none\" stroke-width=\"0.1\" />";
    } else {
        svg << "\" stroke=\"none\" fill=\"" << color << "\" stroke-width=\"0.1\" />";
    }

    previous = sliced;
}

void Generator::run()
{
    hasM = false;
    std::stringstream svg;

    Polygons empty;
    previous = empty;

    addLayer(svg, true, 1, "red");
    for (auto e : engravures) {
        addLayer(svg, false, e.z*1000, e.color);
    }
    
    xMin -= 0.1; yMin -= 0.1;
    xMax += 0.1; yMax += 0.1;
    double width = (xMax-xMin);
    double height = (yMax-yMin);
    std::cout << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" << xMin << 
        " " << yMin << " " << width << " " << height << "\">" << std::endl;
    std::cout << svg.str();
    std::cout << "</svg>" << std::endl;
}
