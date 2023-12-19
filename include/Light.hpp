#pragma once

#include "matrix.hpp"

#include <vector>

struct Light {
    Light() :
    position{},
    color{},
    point_light{false},
    intensity{}
    {};
    
    vec3f position;
    vec3f color;
    bool point_light;
    // Not specified by user atm!
    float intensity;
};

inline std::ostream& operator<<(std::ostream& os, const Light& l)
{
    os << "position: " << l.position << "\n";
    os << "color: " << l.color << "\n";
    os << "is point light: " << l.point_light;
    os << "intensity: " << l.intensity;
    return os;
}
