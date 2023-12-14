#include "Light.hpp"

std::ostream &operator<<(std::ostream &os, Light &l)
{
    os << "position: " << l.position << "\n";
    os << "color: " << l.color << "\n";
    os << "is point light: " << l.point_light;
    os << "intensity: " << l.intensity;
    return os;
}
