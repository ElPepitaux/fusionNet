cmake_minimum_required(VERSION 3.10)

project(fusionnet)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_STATIC_LIBS "Build static libraries instead of shared ones" OFF)
option(BUILD_EXAMPLES "Build examples" OFF)

file(GLOB_RECURSE SOURCES "sources/*.cpp")

include_directories(headers)

if (BUILD_STATIC_LIBS)
    add_library(${PROJECT_NAME} STATIC ${SOURCES})
else()
    add_library(${PROJECT_NAME} SHARED ${SOURCES})
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()