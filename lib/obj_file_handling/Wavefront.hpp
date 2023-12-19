#ifndef WAVEFRONT_HEADER
#define WAVEFRONT_HEADER

#include <memory>
#include <vector>
#include <unordered_map>

#include "matrix.hpp"

class WavefrontMaterial {
    float spectral_exponent;
    float opacity;
    float index_of_refraction;
    vec3f ambient_color;
    vec3f diffuse_color;
    vec3f spectral_color;
};

class WavefrontObjectFile {
private:
    std::unordered_map<std::string, WavefrontMaterial> materials;
    std::unique_ptr<std::vector<vec3f>> vertexes;
    std::unique_ptr<std::vector<vec3f>> normals;
    std::unique_ptr<std::vector<vec2f>> texture_coordinates;
    bool smooth_shading;
};

#endif