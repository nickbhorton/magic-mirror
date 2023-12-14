#pragma once

#include "vec/Vec.hpp"

struct Material{
    vec3f color_intrinsic;
    vec3f color_specular_reflection;
    float k_ambient;
    float k_diffuse;
    float k_specular;
    float reflectiveness;
    unsigned int n;
    float opacity;
    float index_of_refraction;

    Material();
};

bool operator==(const Material &s1, const Material &s2);
std::ostream& operator<<(std::ostream& os, const Material& m);

