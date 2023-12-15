#pragma once

#include "vec/Vec.hpp"
#include <stack>

struct Ray {
    Ray() :
    origin {},
    direction {},
    intersected_obj_id {-1},
    spheres_inside {}
    {};

    vec3f origin;
    vec3f direction;
    int intersected_obj_id;
    std::stack<int> spheres_inside;
};