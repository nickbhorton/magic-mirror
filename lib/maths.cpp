#include "maths.hpp"

extern int current_pixel_x;
extern int current_pixel_y;
extern int pixel_inquiry_x;
extern int pixel_inquiry_y;

static float get_fresnel_naught(float ni, float nt){
    return pow((nt-ni)/(nt+ni),2);
}

/**
 * @brief Calculates the fresnel reflectance for an intersection with an object.
 * Should only be calculated if the material of the object has a specular coefficient that is greater
 * than 0. 
 * 
 * @param direction_of_ray_origin 
 * @param normal 
 * @param F0 
 * @return float range from [F0, 1]
 */
float get_fresnel_reflectance(const vec3f& direction_of_ray_origin, const vec3f& normal, float ni, float nt){
    float F0 = get_fresnel_naught(ni, nt);
    return F0 + (1.0f - F0) * std::pow(1 - std::abs((direction_of_ray_origin & normal)), 5);
}


Ray get_reflected_ray(const Ray& incoming_ray, const vec3f& point_of_reflection, const vec3f& normal_vector){
    vec3f I = -incoming_ray.direction;
    vec3f N = (normal_vector & I) > 0.0f ? normal_vector : -normal_vector;
    float cos_theta = N & I;
    Ray outgoing_ray = incoming_ray;
    outgoing_ray.origin = point_of_reflection;
    outgoing_ray.direction = ((2.0f*cos_theta) * N) - I;
    return outgoing_ray;
}

Ray get_refracted_ray(const Ray& incoming_ray, const vec3f& point_of_reflection, const vec3f& normal_vector, float ni, float nt){
    Ray outgoing_ray = incoming_ray;
    
    float n = ni/nt;
    vec3f I = -incoming_ray.direction;
    vec3f N = (normal_vector & I) > 0.0f ? normal_vector : -normal_vector;
    float cos_theta = (N & I);
    outgoing_ray.origin = point_of_reflection;
    float coeff = sqrt(1.0 - (pow(n,2) * (1.0 - pow(cos_theta, 2))));
    // If there is not solution to coeff
    // for total internal reflection
    if (coeff != coeff){
        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
            std::cout << "total internal reflection: \n";
        }
        Ray outgoing_ray = get_reflected_ray(incoming_ray, point_of_reflection, normal_vector);
        // We just pushed the sphere onto the stack in the caller function because we intersected with the sphere
        // however actually we have total internal reflection so we reflect instead. This means we must remove the
        // Sphere we just added
        // Refactor TODO: this logic should be in caller function
        if (!outgoing_ray.spheres_inside.empty()) {
            outgoing_ray.spheres_inside.pop();
        }
        return outgoing_ray;
    }
    outgoing_ray.direction = coeff * (-N) + n * ((cos_theta * N) - I);
    return outgoing_ray;
}

