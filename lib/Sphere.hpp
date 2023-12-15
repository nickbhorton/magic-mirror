#pragma once

#include "vec/Vec.hpp"
#include "Ray.hpp"

#include <vector>
#include <bits/stdc++.h>

// id will store a unique number to speed up equivalence
class Sphere{
    public:
        Sphere() :
        position{},
        radius{}
        {};

        vec3f position;
        float radius;

        inline vec3f get_max_point() const {
            return vec3f{position.x + radius, position.y + radius, position.z + radius};
        }

        inline vec3f get_min_point() const {
            return vec3f{position.x - radius, position.y - radius, position.z - radius};
        }

};

inline std::ostream& operator<<(std::ostream& os, const Sphere& s)
{
    os << "Sphere:\n";
    os << "\tposition: " << s.position <<"\n";
    os << "\tradius: " << s.radius << "\n";
    return os;
}

namespace sphere {
    struct MinimumInformationForIntersection
    {
        float t1;
        float t2;
    };
}

std::optional<sphere::MinimumInformationForIntersection> intersect(const Sphere& s, const Ray& r);
