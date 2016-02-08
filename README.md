# Projekt

Projekt is a tool based on Cura that allow you to slice STL parts into SVG for
laser machines, with cutting and engraving.

## Getting it

First, install [deps](https://github.com/Rhoban/deps), then:

	deps install rhoban/projekt

## Usage

Basically:

    projekt input.stl > out.svg

Will produce a svg with (red) cutting lines for your laser cutting machines.

You can also specify the output using `-o`:

    projekt -o out.svg input.stl

Note that some software use XZ plane as ground, you can use it with `-z`:
    
    projekt -z -o out.svg input.stl

You can then add engraving layers using the `-e` option, for instance:

    projekt -e 2.5:black -o out.svg input.stl

Will generate red lines for cutting and black filled polygons to engrave the difference
between the first layer and the layer at 2.5mm from the bottom of your STL.

You can specify multiple engraving, like this:

    projekt -e 2.5:black -e 2.9:lightgrey -o out.svg input.stl
	
You can also output (simplified) PLT format:

	projekt -f plt input.stl > out.plt
