cmake_minimum_required(VERSION 2.6)
 
project(dns_lookup)

set(CMAKE_C_FLAGS "-std=c99") 

file(GLOB CPP_FILES *.c)

add_executable(${PROJECT_NAME} ${CPP_FILES})
