#ifndef FILE_HEADER
#define FILE_HEADER

#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "Light.hpp"
#include "Object.hpp"
#include "Texture.hpp"
#include "Scene.h"

// Args that come in from command line
struct ProgramArguments {
    std::string input_file;
    std::string output_file;
    std::string relative_path_to_textures;
};

// Args that are parsed from the input file
struct PPMFileArguments{
    unsigned int width;
    unsigned int height;
    vec3f eye;
    vec3f viewdir;
    vec3f updir;
    float vfov;
    vec3f bkgcolor;
    vec3f mtlcolor;
    Scene scene;
    unsigned int max_number_of_reflections;
    unsigned int number_of_shadow_rays;
    float shadow_ray_variance;
    std::string image_name;
    float camera_index_of_refraction;
};

ProgramArguments parse_cmd_args(int argc, char* argv[]);
PPMFileArguments parse_input_file_args(std::string file_name, std::string relative_path_to_textures);

void usage(const char* program);

template<typename T> T get_arg_from_file(std::ifstream& file, std::string parameter_name, bool print = false){
    T parameter;
    try {
        file >> parameter;
        if (print) std::cout << parameter_name << ": " << parameter << "\n";
        return parameter;
    }
    catch (const std::invalid_argument& e){
        std::cerr << "Error - " << e.what() << " Failed on parameter: " << parameter_name << "\n"
        << "terminating process with exit code EXIT_FAILURE\n";
        exit(EXIT_FAILURE);
    }
}

template <typename T, size_t N> Vec<T, 3> get_vec_from_file(std::ifstream& file, std::string vec_name, bool print = false){
    T data_array[3];
    for (unsigned int i = 0; i < 3; i++){
        data_array[i] = get_arg_from_file<T>(file,"", false);
    }
    Vec<T, 3> v = Vec<T, 3>(data_array[0], data_array[1], data_array[2]);
    if (print) std::cout << vec_name << ": " << v << "\n";
    return v;
}

#endif
