# Projekt

Projekt is a tool based on Cura that allow you to slice STL parts into SVG for
laser machines, with cutting and engraving.

## Building

First, clone the repositories:

    git clone https://github.com/Rhoban/CuraEngine.git --depth=1
    git clone https://github.com/Rhoban/Projekt.git

Then, go in the `Projekt` directory and run cmake:

    cd Projekt
    mkdir build
    cd build
    cmake ..
    make

## Usage

Basically:

    projekt input.stl > out.svg

Will produce a svg with (red) cutting lines for your laser cutting machines.

You can also specify the output using `-o`:

    projekt input.stl -o out.svg

Note that some software use XZ plane as ground, you can use it with `-z`:
    
    projekt -z input.stl -o out.svg

You can then add engraving layers using the `-e` option, for instance:

    projekt -e 2.5:black input.stl -o out.svg

Will generate red lines for cutting and black filled polygons to engrave the difference
between the first layer and the layer at 2.5mm from the bottom of your STL.

You can specify multiple engraving, like this:

    projekt -e 2.5:black -e 2.9:lightgrey input.stl -o out.svg
