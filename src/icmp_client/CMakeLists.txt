cmake_minimum_required(VERSION 2.6)
 
project(icmp_client)

file(GLOB CPP_FILES *.c)

include_directories(../../lib/include)

add_executable(${PROJECT_NAME} ${CPP_FILES})

target_link_libraries(${PROJECT_NAME} skull)