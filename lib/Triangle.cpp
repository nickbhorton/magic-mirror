#include "Triangle.hpp"

std::optional<triangle::MinimumInformationForIntersection> intersect(const Triangle& t, const Ray& r)
{
    const vec3f& e1 = t.p1 - t.p0;
    const vec3f& e2 = t.p2 - t.p0;
    const vec3f& n = e1 | e2;
    const float D = -(t.p0 & n);
    const float denominator = (n & r.direction);
    // TODO: this seems dangerous as floats are weird
    if (denominator == 0.0f || denominator == -0.0f || denominator == 0){
        // Ray is parallel
        return {};
    }
    else {
        const float t_val = -(D + (n & r.origin))/denominator;
        if (t_val < 0.0f){
            // If its negitive just return here and pretend we didn't compute the solution (triangle is behind camera)
            return {};
        }
        // This could need to be normalized somehow
        const vec3f& p = r.origin + (t_val * r.direction); 

        const vec3f& ep = p - t.p0;
        const float d11 = e1 & e1;
        const float d12 = e1 & e2;
        const float d22 = e2 & e2;
        const float d1p = e1 & ep;
        const float d2p = e2 & ep;

        const float det = d11*d22 - d12*d12;
        if (det == 0.0f || det == -0.0f || det == 0){
            std::cout << "det 0 ";
            return {};
        }
        else {
            const float beta = (d22*d1p - d12*d2p)/det;
            const float gamma = (d11*d2p - d12*d1p)/det;
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