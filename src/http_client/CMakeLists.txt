cmake_minimum_required(VERSION 2.6)
 
project(http_client)

file(GLOB CPP_FILES *.c)

add_executable(${PROJECT_NAME} ${CPP_FILES})
