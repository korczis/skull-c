cmake_minimum_required(VERSION 2.6)
 
 # See http://techminded.net/blog/modular-c-projects-with-cmake.html
 
set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/build)
 
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

# add_subdirectory(apps) 
add_subdirectory(lib)
add_subdirectory(src)
# add_subdirectory(tests)