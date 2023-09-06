cmake_minimum_required(VERSION 2.6)

project(prac8_8)

add_definitions(-std=c++11)

option(CUDA_USE_STATIC_CUDA_RUNTIME OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

find_package(CUDA REQUIRED)

message("embed_platform off")
include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Ofast -Wfatal-errors -D_MWAITXINTRIN_H_INCLUDED")

find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

file(GLOB HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

add_executable(prac8_8 ${HEADERS} ${SOURCES})
target_link_libraries(prac8_8 nvinfer cudart ${OpenCV_LIBRARIES})

add_definitions(-O2 -pthread)

