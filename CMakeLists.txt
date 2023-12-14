cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(raytracer VERSION 1.0)
add_compile_options(-Wall -Wextra -Wpedantic -std=c++20 -g -O1)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_subdirectory(lib)

add_executable(raytracer main.cpp)

add_executable(vec_test lib/vec/vec_test.cpp)
target_link_libraries(raytracer PUBLIC file_lib maths_lib solution_lib)
