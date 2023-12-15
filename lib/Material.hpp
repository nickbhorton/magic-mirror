#pragma once

#include "vec/Vec.hpp"

struct Material{
    Material():
    color_intrinsic {},
    color_specular_reflection {},
    k_ambient {},
    k_diffuse {},
    k_specular {},
    reflectiveness {},
    n {},
    opacity {},
    index_of_refraction {}
    {};

    vec3f color_intrinsic;
    vec3f color_specular_reflection;
    float k_ambient;
    float k_diffuse;
    float k_specular;
    float reflectiveness;
    unsigned int n;
    float opacity;
    float index_of_refraction;
};

inline bool operator==(const Material &s1, const Material &s2){
    return (s1.color_intrinsic == s2.color_intrinsic
        &&  s1.color_specular_reflection == s2.color_specular_reflection
        &&  s1.k_ambient == s2.k_ambient
        &&  s1.k_diffuse == s2.k_diffuse
        &&  s1.k_specular == s2.k_specular
        &&  s1.n == s2.n
        );
}

inline std::ostream& operator<<(std::ostream& os, const Material& m){
    os << "int col: " << m.color_intrinsic << "  ";
    os << "spec col: " << m.color_specular_reflection << "  ";
    os << "k_a: " << m.k_ambient << "  ";
    os << "k_d: " << m.k_diffuse << "  ";
    os << "k_s: " << m.k_specular << "  ";
    os << "n: " << m.n;
    return os;
}

