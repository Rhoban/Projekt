#include <fstream>
#include <unistd.h>
#include "Generator.h"
#include "PLTFormat.h"
#include "SVGFormat.h"
#include "DXFFormat.h"

Generator::Generator()
    : mesh(nullptr), output(""), zExtra(0.0), pOffset(50)
{
    format = new SVGFormat;
}

void Generator::openSTL(std::string filename, std::string plate)
{
    MeshGroup meshgroup(nullptr);
    FMatrix3x3 mat;
    if (plate == "xz") {
        mat.m[1][1] = 0;
        mat.m[2][2] = 0;
        mat.m[1][2] = 1;
        mat.m[2][1] = -1;
    }
    if (plate == "yz") {
        mat.m[0][0] = 0;
        mat.m[0][2] = 1;
        mat.m[2][0] = -1;
        mat.m[2][2] = 0;
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

void Generator::setOutput(std::string filename)
{
    output = filename;
}

void Generator::setOutputFormat(std::string formatName)
{
    delete format;
    if (formatName == "svg") {
        format = new SVGFormat;
    } else if (formatName == "plt") {
        format = new PLTFormat;
    } else if (formatName == "dxf") {
        format = new DXFFormat;
    } else {
        std::cerr << "Unknown format: " << formatName << std::endl;
    }
}

void Generator::addEngravure(double z, std::string color)
{
    Engravure e;
    e.z = z;
    e.color = color;
    engravures.push_back(e);
}

void Generator::setZExtra(double zExtra_)
{
    zExtra = zExtra_*1000.0;
}

void Generator::setRepeat(std::string layer, int count, double spacing)
{
    repeats[layer].count = count;
    repeats[layer].spacing = spacing;
}

void Generator::setPOffset(double pOffset_)
{
    pOffset = pOffset_;
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

void Generator::addPolygon(std::stringstream &data, Polygons polygon, bool isFirst, std::string layer)
{
    format->beginPolygon(layer, isFirst);

    for (auto path : polygon) {
        format->beginPath();
        for (auto point : path) {
            double X = point.X*format->getXRatio();
            double Y = point.Y*format->getYRatio();

            format->registerPoint(X, Y);
            format->addPoint(X, Y);
        }
        format->endPath();
    }
    format->endPolygon(layer, isFirst);
}

void Generator::addLayer(std::stringstream &data, bool isFirst, int z, std::string color)
{
    z += zOffset + zExtra;
    auto sliced = slice(z);
    auto polygon = sliced;

    if (!isFirst) {
        polygon = previous.difference(polygon.offset(pOffset));
    } else {
        sliced = sliced.offset(10);
        polygon = sliced;
    }

    addPolygon(data, polygon, isFirst, color);
    if (repeats.count(color)) {
        auto repeat = repeats[color];
        for (int k=0; k<repeat.count; k++) {
            addPolygon(data, polygon.offset(-(k+1)*repeat.spacing*1000.0), isFirst, color);
        }
    }

    previous = sliced;
}

void Generator::run()
{
    format->clear();
    std::ostream *os = &std::cout;
    std::ofstream ofs;
    if (output != "") {
        ofs.open(output);
        os = &ofs;
    }

    std::stringstream data;

    auto min = mesh.min();
    zOffset = min.z;

    Polygons empty;
    previous = empty;

    addLayer(data, true, 1, format->getBaseLayer());
    for (auto e : engravures) {
        addLayer(data, false, e.z*1000, e.color);
    }

    *os << format->output();

    if (output != "") {
        ofs.close();
    }
}
