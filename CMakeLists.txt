cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(raytracer VERSION 1.0)
add_compile_options(-Wall -Wextra -Wpedantic -std=c++20 -g -O3)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# add googletest directory
add_subdirectory(ext/googletest)

# include direectory from linearmath
include_directories(ext/linearmath/include)

add_subdirectory(lib)
add_subdirectory(tests)
add_executable(raytracer main.cpp)

target_link_libraries(raytracer PUBLIC file_lib maths_lib solution_lib)
