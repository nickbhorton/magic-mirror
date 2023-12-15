#pragma once

#include "vec/Vec.hpp"
#include "Ray.hpp"

#include <bits/stdc++.h>

class Triangle{
    public:
        Triangle() :
        p0 {}, p1 {}, p2 {},
        has_normals {false},
        n0 {}, n1 {}, n2 {},
        has_texture {false}
        {};

        vec3f p0, p1, p2;

        bool has_normals;
        vec3f n0, n1, n2;

        bool has_texture;
        vec3f t0, t1, t2;

        inline vec3f get_max_point() {
            return vec3f{std::max(std::max(p0.x, p1.x), p2.x), std::max(std::max(p0.y, p1.y), p2.y), std::max(std::max(p0.z, p1.z), p2.z)};
        }

        inline vec3f get_min_point() {
            return vec3f{std::min(std::min(p0.x, p1.x), p2.x), std::min(std::min(p0.y, p1.y), p2.y), std::min(std::min(p0.z, p1.z), p2.z)};
        }
};

inline std::ostream& operator<<(std::ostream& os, const Triangle& t)
{
    os << "Triangle:\n";
    os << "\tp0: " << t.p0 << "\n";
    os << "\tp1: " << t.p1 << "\n";
    os << "\tp2: " << t.p2 << "\n";
    if (t.has_normals){
        os << "\tn0: " << t.n0 << "\n";
        os << "\tn1: " << t.n1 << "\n";
        os << "\tn2: " << t.n2 << "\n";
    }
    if (t.has_texture){
        os << "\tt0: " << t.t0.x << " " << t.t0.y << "\n";
        os << "\tt1: " << t.t1.x << " " << t.t1.y << "\n";
        os << "\tt2: " << t.t2.x << " " << t.t2.y << "\n";
    }
    return os;
}


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