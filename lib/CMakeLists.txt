add_library(file_lib file.cpp Texture.cpp obj_file_handling/Wavefront.hpp)
add_library(solution_lib View.cpp Triangle.cpp Sphere.cpp Solution.cpp Tree.cpp)
# add_library(solution_lib Object.cpp Material.cpp Light.cpp View.cpp Triangle.cpp Sphere.cpp Scene.cpp Solution.cpp)
add_library(maths_lib maths.cpp) 
