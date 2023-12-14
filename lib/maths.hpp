#pragma once

#include "vec/Vec.hpp"
#include "Object.hpp"
#include "Ray.hpp"
#include "Solution.h"
#include "Scene.h"
#include "file.hpp"

#include <complex>
#include <vector>
#include <tuple>

// Ray get_reflected_ray(const Ray& incoming_ray, const vec3f& point_of_reflection, const vec3f& normal_vector);
Ray get_reflected_ray(const Ray& incoming_ray, const vec3f& point_of_reflection, const vec3f& normal_vector);
Ray get_refracted_ray(const Ray& incoming_ray, const vec3f& point_of_reflection, const vec3f& normal_vector, float ni, float nt);
float get_fresnel_reflectance(const vec3f& direction_of_ray_origin, const vec3f& normal, float ni, float nt);