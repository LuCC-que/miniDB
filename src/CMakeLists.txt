cmake_minimum_required(VERSION 3.0.0)

project(miniDB VERSION 1.0.0)

set (CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_BUILD_TYPE Debug)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
# set(CMAKE_CXX_FLAGS_RELEASE "-O")

add_subdirectory(utils)
add_subdirectory(frontend)

add_executable(miniDB main.cpp)

target_link_libraries(miniDB PRIVATE utils frontend)

# enable_testing()
# add_executable(tests tests/test.cpp)
# target_link_libraries(tests PRIVATE frontend utils)
# add_test(Tester tests)