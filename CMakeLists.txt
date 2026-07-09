cmake_minimum_required(VERSION 3.10)
project(GraphicsRework)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenMP REQUIRED)
# FORCE executable to the source directory root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})

add_executable(GraphicsRework main.cpp)

# Apply OpenMP flags and -O3 optimization to the target
target_link_libraries(GraphicsRework PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(GraphicsRework PRIVATE -O3)

# Optional: Print status to verify flags
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_CXX_FLAGS}")
endif()