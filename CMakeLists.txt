cmake_minimum_required(VERSION 3.10)
project(GraphicsRework)

set(CMAKE_CXX_STANDARD 17)

# FORCE executable to the source directory root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(GraphicsRework main.cpp)