#pragma once

#include "vec/Vec.hpp"

#include <vector>

struct Light {
    vec3f position;
    vec3f color;
    bool point_light;
    // Not specified by user atm!
    float intensity;
};

std::ostream& operator<<(std::ostream& os, Light& m);