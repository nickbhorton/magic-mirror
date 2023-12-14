cmake_minimum_required(VERSION 3.20)

# Set project name and version
add_compile_options(-Wall -Wextra -Wpedantic -std=c++20)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

add_library(file_lib file.cpp Texture.cpp)
add_library(solution_lib Object.cpp Material.cpp Light.cpp View.cpp Triangle.cpp Sphere.cpp Scene.cpp Solution.cpp Tree.cpp)
# add_library(solution_lib Object.cpp Material.cpp Light.cpp View.cpp Triangle.cpp Sphere.cpp Scene.cpp Solution.cpp)
add_library(maths_lib maths.cpp color_math.cpp) 
