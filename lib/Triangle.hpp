#pragma once

#include "vec/Vec.hpp"
#include "Ray.hpp"

#include <bits/stdc++.h>

class Triangle{
    public:
        Triangle();
        void print_to_cout() const;

        vec3f p0, p1, p2;

        bool has_normals;
        vec3f n0, n1, n2;

        bool has_texture;
        vec3f t0, t1, t2;

        vec3f get_min_point();
        vec3f get_max_point();
};

namespace triangle {
    struct MinimumInformationForIntersection
    {
        float alpha;
        float beta;
        float gamma;
        float t_val;
        vec3f solution_position;
    };
}

std::optional<triangle::MinimumInformationForIntersection> intersect(const Triangle& t, const Ray& r);