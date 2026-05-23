cmake_minimum_required(VERSION 3.10)

project(default)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE SERVERS_SOURCES "sources/server/*.cpp")
file(GLOB_RECURSE CLIENTS_SOURCES "sources/client/*.cpp")


include_directories(headers)

add_executable(${PROJECT_NAME}_server ${SERVERS_SOURCES})

add_executable(${PROJECT_NAME}_client ${CLIENTS_SOURCES})

target_link_libraries(${PROJECT_NAME}_server fusionnet)
target_link_libraries(${PROJECT_NAME}_client fusionnet)