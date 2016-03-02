cmake_minimum_required(VERSION 3.2)
project(gbemuc11)
set(CMAKE_CXX_COMPILER "clang++")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "./build")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++ -v")
file(GLOB SOURCE_FILES src/*.cpp src/*.hpp)
add_executable(gbemuc11 ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
include_directories(${GLFW_INCLUDE_DIRS})

target_link_libraries(gbemuc11 ${GLFW_LIBRARIES})


