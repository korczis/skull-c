cmake_minimum_required(VERSION 2.6)
 
find_package (Threads)

project(thread)

set(CMAKE_C_FLAGS "-std=c99") 

file(GLOB CPP_FILES *.c)

add_executable(${PROJECT_NAME} ${CPP_FILES})

TARGET_LINK_LIBRARIES(${PROJECT_NAME}
	${CMAKE_THREAD_LIBS_INIT}
)