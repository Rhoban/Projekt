#include <iostream>
#include <getopt.h>
#include <unistd.h>
#include "Generator.h"
#include "util.h"

void usage()
{
    std::cout << "Projekt - v1.0" << std::endl;
    std::cout << "usage: projekt input.stl" << std::endl;
    std::cout << std::endl;
    std::cout << "-z: Use the XZ plane as bottom instead of XY" << std::endl;
    std::cout << "-e z:color: Add an engravure" << std::endl;
    exit(0);
}

int main(int argc, char *argv[])
{
    Generator generator;
    int index;
    std::string input = "";
    std::string plate = "xy";

    while ((index = getopt(argc, argv, "ze:")) != -1) {
        switch (index) {
            case 'z':
                plate = "xz";
                break;
            case 'e': {
                auto parts = split(std::string(optarg), ':');
                if (parts.size() != 2) usage();
                generator.addEngravure(atof(parts[0].c_str()), parts[1]);
                }
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
