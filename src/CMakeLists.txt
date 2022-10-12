cmake_minimum_required(VERSION 3.0.0)

project(miniDB VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 17)

add_subdirectory(utils)
add_subdirectory(frontend)

add_executable(miniDB main.cpp)

target_link_libraries(miniDB PRIVATE utils frontend)