# This is CMakeLists.txt (CMake Makefile) for lib folder

cmake_minimum_required (VERSION 2.6)
project (skull)

# The version number.
set(Skull_VERSION_MAJOR 0)
set(Skull_VERSION_MINOR 1)

include_directories(include/skull)

set(${PROJECTNAME}_SOURCES
    src/skull.c
    src/helpers/helpers.c
)

# add the executable
add_library(skull STATIC ${${PROJECTNAME}_SOURCES})
