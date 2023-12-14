#ifndef SOLUTION_H
#define SOLUTION_H

#include "vec/Vec.hpp"
#include "Object.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Scene.h"
#include "Tree.h"

#include <queue>

class Solution {
    public:
        float get_smallest_t() const;

        vec3f get_position() const;
        vec3f get_normal() const;
        vec3f get_direction_to_ray_origin() const;
        float get_tex_u() const;
        float get_tex_v() const;
        Object get_object_intersected() const;

        bool texture_coords_computed;

        Ray ray;
        std::vector<float> ts;
        Object object_intersected;

        vec3f position;
        vec3f normal;
        float tex_u, tex_v;
};

std::optional<Solution> get_smallest_positive_solution(Ray r, const Scene& scene);

std::optional<Solution> solve_object(const Object& o, const Ray& r, const Scene& scene);
std::optional<Solution> solve_sphere(const Object& o, const Sphere& s, const Ray& r);
std::optional<Solution> solve_triangle(const Object& o, const Triangle& s, const Ray& r);

std::optional<Solution> get_solution_from_tree(const std::unique_ptr<TreeNode>& tn, const Ray& r, const Scene& scene);

#endif