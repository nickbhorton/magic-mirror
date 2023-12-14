#include "Triangle.hpp"

Triangle::Triangle()
{
    p0 = vec3f(0.0, 0.0, 0.0);
    p1 = vec3f(0.0, 0.0, 0.0);
    p2 = vec3f(0.0, 0.0, 0.0);
    has_normals = false;
    n0 = vec3f(0.0, 0.0, 0.0);
    n1 = vec3f(0.0, 0.0, 0.0);
    n2 = vec3f(0.0, 0.0, 0.0);
    has_texture = false;
}

void Triangle::print_to_cout() const
{
    std::cout << "Triangle:\n";
    std::cout << "\tp0: " << p0 << "\n";
    std::cout << "\tp1: " << p1 << "\n";
    std::cout << "\tp2: " << p2 << "\n";
    if (has_normals){
        std::cout << "\tn0: " << n0 << "\n";
        std::cout << "\tn1: " << n1 << "\n";
        std::cout << "\tn2: " << n2 << "\n";
    }
    if (has_texture){
        std::cout << "\tt0: " << t0.x << " " << t0.y << "\n";
        std::cout << "\tt1: " << t1.x << " " << t1.y << "\n";
        std::cout << "\tt2: " << t2.x << " " << t2.y << "\n";
    }
}

std::optional<triangle::MinimumInformationForIntersection> intersect(const Triangle& t, const Ray& r)
{
    vec3f e1 = t.p1 - t.p0;
    vec3f e2 = t.p2 - t.p0;
    vec3f n = e1 | e2;
    float D = -(t.p0 & n);
    float denominator = (n & r.direction);
    if (denominator == 0.0f || denominator == -0.0f || denominator == 0){
        // Ray is parallel
        return {};
    }
    else {
        float t_val = -(D + (n & r.origin))/denominator;
        if (t_val < 0.0f){
            // If its negitive just return here and pretend we didn't compute the solution (triangle is behind camera)
            return {};
        }
        // This could need to be normalized somehow
        vec3f p = r.origin + (t_val * r.direction); 

        vec3f ep = p - t.p0;
        float d11 = e1 & e1;
        float d12 = e1 & e2;
        float d22 = e2 & e2;
        float d1p = e1 & ep;
        float d2p = e2 & ep;

        float det = d11*d22 - d12*d12;
        if (det == 0.0f || det == -0.0f || det == 0){
            std::cout << "det 0 ";
            return {};
        }
        else {
            float beta = (d22*d1p - d12*d2p)/det;
            float gamma = (d11*d2p - d12*d1p)/det;
            if (beta < 0.0f || beta > 1.0f || gamma < 0.0f || gamma > 1.0f || beta + gamma > 1.0f || beta + gamma < 0.0f){
                return {};
            }
            else {
                return triangle::MinimumInformationForIntersection {
                    1 - (beta + gamma),
                    beta,
                    gamma,
                    t_val,
                    p
                };
            }
        }
    }
    return {};
}

vec3f Triangle::get_max_point() {
    vec3f max{};
    max.x = std::max(std::max(p0.x, p1.x), p2.x);
    max.y = std::max(std::max(p0.y, p1.y), p2.y);
    max.z = std::max(std::max(p0.z, p1.z), p2.z);
    return max;
}

vec3f Triangle::get_min_point() {
    vec3f min{};
    min.x = std::min(std::min(p0.x, p1.x), p2.x);
    min.y = std::min(std::min(p0.y, p1.y), p2.y);
    min.z = std::min(std::min(p0.z, p1.z), p2.z);
    return min;
}