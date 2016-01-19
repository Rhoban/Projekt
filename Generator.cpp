#include <fstream>
#include <unistd.h>
#include "Generator.h"

Generator::Generator()
    : mesh(nullptr), output(""), format(FORMAT_SVG), zExtra(0.0), pOffset(50)
{
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
	if (formatName == "svg") {
		format = FORMAT_SVG;
	} else if (formatName == "plt") {
		 format = FORMAT_PLT;
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
	zExtra = zExtra_;
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

void Generator::addPolygon(std::stringstream &data, Polygons polygon, bool isFirst, std::string color)
{
	double fX, fY;
		
	double xRatio = 3.543307/1000.0;
	double yRatio = -xRatio;
	if (format == FORMAT_PLT) {
		xRatio = 40.0/1000.0;
		yRatio = 40.0/1000.0;
	}
	
    if (format == FORMAT_SVG) data << "<path d=\"";
	if (format == FORMAT_PLT) data << "SP" << color << ";" << std::endl;

    for (auto path : polygon) {
        if (format == FORMAT_SVG) data << "M ";
        bool first = true;
        for (auto point : path) {
            double X = point.X*xRatio;
            double Y = point.Y*yRatio;

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
				fX = X;
				fY = Y;
                first = false;
				if (format == FORMAT_PLT) data << "PU";
            } else {
                if (format == FORMAT_SVG) data << "L ";
				if (format == FORMAT_PLT) data << "PD";
            }
            if (format == FORMAT_SVG) data << X << " " << Y << " ";
			if (format == FORMAT_PLT) data << (int)X << " " << (int)Y << " ";
			if (format == FORMAT_PLT) data << ";" << std::endl;
        }
        if (format == FORMAT_SVG) data << "z " << std::endl;
		if (!first && format == FORMAT_PLT) {
			data << "PD" << (int)fX << " " << (int)fY << " ";
		}
    }
	if (format == FORMAT_SVG) {
		if (isFirst) {
			data << "\" stroke=\"" << color << "\" fill=\"none\" stroke-width=\"0.1\" />";
		} else {
			data << "\" stroke=\"none\" fill=\"" << color << "\" stroke-width=\"0.1\" />";
		}
	}
    if (format == FORMAT_SVG) data << std::endl;
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

    hasM = false;
	std::string baseColor = "red";
	if (format == FORMAT_PLT) baseColor = "1";
    addLayer(data, true, 1, baseColor);
    for (auto e : engravures) {
        addLayer(data, false, e.z*1000, e.color);
    }

    double width = (xMax-xMin);
    double height = (yMax-yMin);
	
	if (format == FORMAT_SVG) {
		*os << "<svg xmlns=\"http://www.w3.org/2000/svg\" viewBox=\"" << xMin << 
			" " << yMin << " " << width << " " << height << "\">" << std::endl;
		*os << data.str();
		*os << "</svg>" << std::endl;
	} else if (format == FORMAT_PLT) {
		*os << data.str();		
		*os << "SP0;" << std::endl;
	}

    if (output != "") {
        ofs.close();
    }
}
