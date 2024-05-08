cmake_minimum_required(VERSION 3.29.1)

project(fusion_demo)

set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES_COMMON "src/common/*.cpp")

file(GLOB_RECURSE SOURCES_MASTER "src/server/*.cpp")

file(GLOB_RECURSE SOURCES_SLAVE "src/client/*.cpp")

add_compile_options(-ggdb)

set(INCLUDE_MASTER
    "include/server"
    "include/common"
    "include"
)

set(INCLUDE_SLAVE
    "include/client"
    "include/common"
    "include"
)

include_directories(include)

include_directories(${INCLUDE_MASTER})
add_executable(master ${SOURCES_MASTER} ${SOURCES_COMMON})

include_directories(${INCLUDE_SLAVE})
add_executable(slave ${SOURCES_SLAVE} ${SOURCES_COMMON})
