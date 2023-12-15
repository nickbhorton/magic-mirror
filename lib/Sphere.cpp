#include "Sphere.hpp"

std::optional<sphere::MinimumInformationForIntersection> intersect(const Sphere& s, const Ray& r)
{
    const vec3f& c = s.position;
    const vec3f& n = r.origin;
    const vec3f& d = r.direction;
    const float B = 2 * (d.x * (n.x - c.x) + d.y * (n.y - c.y) + d.z * (n.z - c.z));
    const float C = pow((n.x - c.x), 2) + pow((n.y - c.y), 2) + pow((n.z - c.z), 2) - pow(s.radius, 2);
    const float discriminant = pow(B, 2) - 4.0*C;
    if (discriminant >= 0.0f){
        const float t1 = (-B + sqrt(discriminant))/(2.0f);
        const float t2 = (-B - sqrt(discriminant))/(2.0f);
        if (t1 >= 0.0f || t2 >= 0.0f){
            return sphere::MinimumInformationForIntersection{t1, t2};
        }
    }
    return {};
}