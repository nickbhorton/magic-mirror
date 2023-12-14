#include "Sphere.hpp"

Sphere::Sphere()
{
    this->position = vec3f();
    this->radius = 0.0f;
}

void Sphere::print_to_cout() const
{
    std::cout << "Sphere:\n";
    std::cout << "\tposition: " << position <<"\n";
    std::cout << "\tradius: " << radius << "\n";
}

std::optional<sphere::MinimumInformationForIntersection> intersect(const Sphere& s, const Ray& r)
{
    vec3f c = s.position;
    vec3f n = r.origin;
    vec3f d = r.direction;
    float B = 2 * (d.x * (n.x - c.x) + d.y * (n.y - c.y) + d.z * (n.z - c.z));
    float C = pow((n.x - c.x), 2) + pow((n.y - c.y), 2) + pow((n.z - c.z), 2) - pow(s.radius, 2);
    float discriminant = pow(B, 2) - 4.0*C;
    if (discriminant >= 0.0f){
        float t1 = (-B + sqrt(discriminant))/(2.0f);
        float t2 = (-B - sqrt(discriminant))/(2.0f);
        if (t1 >= 0.0f || t2 >= 0.0f){
            return sphere::MinimumInformationForIntersection{t1, t2};
        }
    }
    return {};
}

vec3f Sphere::get_max_point() {
    vec3f max{};
    max.x = position.x + radius;
    max.y = position.y + radius;
    max.z = position.z + radius;
    return max;
}

vec3f Sphere::get_min_point() {
    vec3f min{};
    min.x = position.x - radius;
    min.y = position.y - radius;
    min.z = position.z - radius;
    return min;
}
