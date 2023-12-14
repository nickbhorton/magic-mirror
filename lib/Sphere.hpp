#pragma once

#include "vec/Vec.hpp"
#include "Ray.hpp"

#include <vector>
#include <bits/stdc++.h>

// id will store a unique number to speed up equivalence
class Sphere{
    public:
        Sphere();
        void print_to_cout() const;

        vec3f position;
        float radius;

        vec3f get_min_point();
        vec3f get_max_point();
};

namespace sphere {
    struct MinimumInformationForIntersection
    {
        float t1;
        float t2;
    };
}

std::optional<sphere::MinimumInformationForIntersection> intersect(const Sphere& s, const Ray& r);
