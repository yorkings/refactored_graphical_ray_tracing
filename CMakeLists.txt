cmake_minimum_required(VERSION 3.12) 
project(GraphicsRework)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)  # Use strict C++20

# Force executable to the source directory root
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR})


find_package(OpenMP REQUIRED)

add_executable(GraphicsRework main.cpp )

# Apply OpenMP flags and optimization
target_link_libraries(GraphicsRework PRIVATE OpenMP::OpenMP_CXX)
target_compile_options(GraphicsRework PRIVATE -O3)

# Optional: Add more optimization flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(GraphicsRework PRIVATE -march=native -mtune=native)
endif()

# Print status to verify flags
if(OpenMP_CXX_FOUND)
    message(STATUS "OpenMP found: ${OpenMP_CXX_FLAGS}")
    message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
endif()

# For debugging (optional)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(GraphicsRework PRIVATE -g -O0)
else()
    target_compile_options(GraphicsRework PRIVATE -O3)
endif()