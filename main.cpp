#include <iostream>
#if defined(_WIN32) || defined(_WIN64)
#include "wingetopt.h"
#else
#include <getopt.h>
#endif
#include <unistd.h>
#include "Generator.h"
#include "util.h"

void usage()
{
    std::cout << "Projekt - v0.1" << std::endl;
    std::cout << "usage: projekt input.stl" << std::endl;
    std::cout << std::endl;
    std::cout << "-Z: Adds a Z-offset for the bottom" << std::endl;
    std::cout << "-y: Use the YZ plane as bottom instead of XY" << std::endl;
    std::cout << "-z: Use the XZ plane as bottom instead of XY" << std::endl;
    std::cout << "-e z:color: Add an engravure at height z with given color" << std::endl;
    std::cout << "-o output.svg: Specify output name (default to stdout)" << std::endl;
    std::cout << "-f [svg|plt|dxf]: Specify output format" << std::endl;
    std::cout << "-O offset: customize polygon offset" << std::endl;
    std::cout << "-r l:n:e: offset repeat n time on layer l with e spacing" << std::endl;
    std::cout << "-a [treshold]: area treshold (mm^2) to delete small polygons" << std::endl;
    std::cout << std::endl;
    std::cout << "Examples:" << std::endl;
    std::cout << "projekt input.stl > out.svg" << std::endl;
    std::cout << "projekt input.stl -e 2.5:black -o out.svg" << std::endl;
    std::cout << "projekt input.stl -e 2.5:black -e 2.9:grey -z -o out.svg" << std::endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    Generator generator;
    int index;
    std::string input = "";
    std::string plate = "xy";

    while ((index = getopt(argc, argv, "yze:o:f:Z:O:r:a:")) != -1) {
        switch (index) {
            case 'r': {
                          auto parts = split(std::string(optarg), ':');
                          if (parts.size() == 3) {
                              generator.setRepeat(parts[0], atoi(parts[1].c_str()), atof(parts[2].c_str()));
                          }
                      }
                      break;
            case 'a':
                      generator.setAreaTreshold(atof(optarg));
                      break;
            case 'O':
                      generator.setPOffset(atof(optarg));
                      break;
            case 'f':
                      generator.setOutputFormat(std::string(optarg));
                      break;
            case 'z':
                      plate = "xz";
                      break;
            case 'y':
                      plate = "yz";
                      break;
            case 'e': {
                          auto parts = split(std::string(optarg), ':');
                          if (parts.size() != 2) usage();
                          generator.addEngravure(atof(parts[0].c_str()), parts[1]);
                      }
                      break;
            case 'o':
                      generator.setOutput(std::string(optarg));
                      break;
            case 'Z':
                      generator.setZExtra(atof(optarg));
                      break;
        }
    }

    if (optind != argc) {
        generator.openSTL(std::string(argv[optind]), plate);
    } else {
        usage();
    }

    generator.run();
}
