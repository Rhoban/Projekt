cmake_minimum_required (VERSION 2.6.0)

option (STATIC_LINKING
	"Static link" ON)

set (CMAKE_CXX_FLAGS "-std=c++11 -Wall -Wno-reorder -pedantic -fno-elide-constructors -fPIE")

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
target_link_libraries (projekt clipper _CuraEngine)
install(TARGETS projekt DESTINATION bin)
