#pragma once

#include "matrix.hpp"
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
            return vec::create(
                std::max(std::max(p0.get(0), p1.get(0)), p2.get(0)),
                std::max(std::max(p0.get(1), p1.get(1)), p2.get(1)), 
                std::max(std::max(p0.get(2), p1.get(2)), p2.get(2))
            );
        }

        inline vec3f get_min_point() {
            return vec::create(
                std::min(std::min(p0.get(0), p1.get(0)), p2.get(0)), 
                std::min(std::min(p0.get(1), p1.get(1)), p2.get(1)), 
                std::min(std::min(p0.get(2), p1.get(2)), p2.get(2))
            );
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
        os << "\tt0: " << t.t0.get(0) << " " << t.t0.get(1) << "\n";
        os << "\tt1: " << t.t1.get(0) << " " << t.t1.get(1) << "\n";
        os << "\tt2: " << t.t2.get(0) << " " << t.t2.get(1) << "\n";
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