cmake_minimum_required(VERSION 3.25)
project(posixx)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS yes)

include_directories(deps/magic_enum/include)
include_directories(include)

add_executable(demo demo.cpp demo.cpp)
