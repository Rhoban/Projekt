cmake_minimum_required (VERSION 2.6.0)
project (stl2laser)

option (STATIC_LINKING
	"Static link" ON)

option (ENABLE_ARCUS
    "Enable arcus" OFF)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors")

set (SOURCES
    Generator.cpp
    Format.cpp
    SVGFormat.cpp
    PLTFormat.cpp
    DXFFormat.cpp

    wingetopt.cpp
    util.cpp
    main.cpp
    )
	
if (STATIC_LINKING)
	set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++ -static")
endif ()

add_executable (projekt ${SOURCES})
target_link_libraries (projekt _CuraEngine clipper)
install(TARGETS projekt DESTINATION bin)
