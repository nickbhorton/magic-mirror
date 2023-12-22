cmake_minimum_required(VERSION 3.20)

# Set project name and version
project(raytracer VERSION 1.0)
add_compile_options(-Wall -Wextra -Wpedantic -std=c++20 -O3)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# include direectory from linearmath
include_directories(ext/linearmath/include)
include_directories(include)

add_subdirectory(lib)

option(PACKAGE_TESTS "Build the tests" ON)
if(PACKAGE_TESTS)
    enable_testing()
    include(GoogleTest)
    add_subdirectory(tests)
endif()

add_executable(raytracer main.cpp)
target_link_libraries(raytracer PUBLIC file_lib maths_lib solution_lib)
