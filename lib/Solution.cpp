#include "Solution.h"

extern int current_pixel_x;
extern int current_pixel_y;
extern int pixel_inquiry_x;
extern int pixel_inquiry_y;

#ifdef DEBUG
long long number_of_solve_object_calls = 0;
long total_clock_ticks_in_intersect_object = 0;
long total_clock_ticks_in_get_smallest_pos_sol = 0;
long total_clock_ticks_in_queue_while_loop = 0;
#endif

static std::optional<Solution> get_smallest_positive_solution(Ray r, const Scene& scene, const std::vector<Object>& objects){
    std::optional<Solution> solution = {};
    for (unsigned int i = 0; i < objects.size(); i++){
        if (!solution.has_value()){
            solution = solve_object(objects[i], r, scene);
        }
        else {
            std::optional<Solution> current_solution = solve_object(objects[i], r, scene);
            if (current_solution.has_value() && current_solution.value().get_smallest_t() < solution.value().get_smallest_t()) {
                solution = current_solution;
            }
        }
    }
    return solution;
}

std::optional<Solution> get_smallest_positive_solution(Ray r, const Scene& scene){
    return get_smallest_positive_solution(r, scene, scene.objects);
}

std::optional<Solution> get_smallest_positive_solution(std::vector<Solution> sols){
    if (sols.size() == 0) return {};
    Solution smallest_solution {sols[0]};
    for (Solution sol : sols){
        if (sol.get_smallest_t() < smallest_solution.get_smallest_t()){
            smallest_solution = sol;
        }
    }
    return smallest_solution;
}



std::optional<Solution> get_solution_from_tree(const std::unique_ptr<TreeNode>& tn, const Ray& r, const Scene& scene){
    if(!intersect_node(tn, r)) return {};
    if (tn->object.has_value()){
        // this is will copy the object to the solve object function (potentially optimization)
        return solve_object(*(tn->object.value().get()), r, scene);
    }
    else if (tn->children.size() > 0){
        std::vector<Solution> sols;
        for (unsigned int i = 0; i < tn->children.size(); i++){
            std::optional<Solution> sol = get_solution_from_tree(tn->children[i], r, scene);
            if (sol.has_value()){
                sols.push_back(sol.value());
            }
        }
        return get_smallest_positive_solution(sols);
    }
    assert(1 == 0);
    return {};
}

std::optional<Solution> solve_object(const Object& o, const Ray& r, const Scene& scene)
{
    #ifdef DEBUG
    ++number_of_solve_object_calls;
    #endif
    switch (o.type)
    {
    case ObjectType::Triangle:
        return solve_triangle(o, scene.triangles[o.type_id], r);
        break;
    case ObjectType::Sphere:
        return solve_sphere(o, scene.spheres[o.type_id], r);
        break;
    
    default:
        std::cerr << "ObjectType is not a value number.\n inside Object::solve\nreturning no object\n";
        return {};
        break;
    }
}

std::optional<Solution> solve_sphere(const Object& o, const Sphere& s, const Ray& r) {
    // If rays object of origination is the object we are solving
    // for... don't
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "checking intersection with obj " << o.id << "\n";
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "object id contained in ray: " << r.intersected_obj_id << "\n";
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y && !r.spheres_inside.empty()) std::cout << "top of obj inside: " << r.spheres_inside.top() << "\n";
    
    bool inside_sphere = false;
    if (!r.spheres_inside.empty() && o.id == r.spheres_inside.top()) {
        inside_sphere = true;
        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "inside sphere!\n";
    }
    if (!inside_sphere && r.intersected_obj_id == o.id) {return {};}
    auto intersection_info = intersect(s, r);
    if (!intersection_info.has_value()){return {};}

    Solution sol {};
    sol.object_intersected = o;
    sol.ray = r;
    sol.ray.intersected_obj_id = o.id;
    // makes sure that the smallest t is at position 1 in the ts vector 
    if (intersection_info.value().t1 >= 0.0f && intersection_info.value().t2 >= 0.0f){
        if (intersection_info.value().t1 < intersection_info.value().t2){
            sol.ts.push_back(intersection_info.value().t1);
            sol.ts.push_back(intersection_info.value().t2);
        }
        else {
            sol.ts.push_back(intersection_info.value().t2);
            sol.ts.push_back(intersection_info.value().t1);
        }
    }
    else if (intersection_info.value().t1 >= 0.0f){
        sol.ts.push_back(intersection_info.value().t1);
    }
    else if (intersection_info.value().t2 >= 0.0f){
        sol.ts.push_back(intersection_info.value().t2);
    }
    else {
        return {};
    }
    if (inside_sphere){
        std::reverse(sol.ts.begin(), sol.ts.end());
    }
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "t0: " << sol.get_smallest_t() << "\n";
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y && sol.ts.size() >= 2) std::cout << "t1: " << sol.ts[1] << "\n";
    sol.position = sol.ray.origin + (sol.get_smallest_t() * sol.ray.direction);
    sol.normal = (sol.position - s.position) / s.radius;
    if (o.texture_index >= 0){
        sol.tex_u = (std::atan2(sol.normal.y, sol.normal.x) + 2*PI) / (2.0f * PI);
        sol.tex_v = std::acos(sol.normal.z) / PI;
        sol.texture_coords_computed = true;
    }
    else {
        sol.tex_u = 0;
        sol.tex_v = 0;
        sol.texture_coords_computed = false;
    }
    return sol;
}

std::optional<Solution> solve_triangle(const Object& o, const Triangle& t, const Ray& r) {
    // If rays object of origination is the object we are solving
    // for... don't
    if (r.intersected_obj_id == o.id) {return{};}
    
    auto intersection_info = intersect(t, r);
    if (!intersection_info.has_value()){return {};}

    Solution sol {};
    sol.ts.push_back(intersection_info.value().t_val);
    sol.object_intersected = o;
    sol.ray = r;
    sol.ray.intersected_obj_id = o.id;
    sol.position = sol.ray.origin + (sol.get_smallest_t() * sol.ray.direction);
    if (t.has_normals){
        sol.normal = ((intersection_info.value().alpha * t.n0) + (intersection_info.value().beta * t.n1) + (intersection_info.value().gamma * t.n2)).normalize();
    }
    else {
        sol.normal = ((sol.position - t.p0) | (t.p2-t.p0)).normalize();
    }
    if (o.texture_index >= 0){
        sol.tex_u = intersection_info.value().alpha*t.t0.x + intersection_info.value().beta*t.t1.x + intersection_info.value().gamma*t.t2.x;
        sol.tex_v = intersection_info.value().alpha*t.t0.y + intersection_info.value().beta*t.t1.y + intersection_info.value().gamma*t.t2.y;
        sol.texture_coords_computed = true;
    }
    else {
        sol.tex_u = 0;
        sol.tex_v = 0;
        sol.texture_coords_computed = false;
    }
    return sol;
}

float Solution::get_smallest_t() const
{
    return ts[0];
}

vec3f Solution::get_position() const
{
    return position;
}

vec3f Solution::get_normal() const
{
    return normal;
}

vec3f Solution::get_direction_to_ray_origin() const
{
    return -ray.direction;
}

float Solution::get_tex_u() const
{
    if (!texture_coords_computed) {
        throw NoTextureCoords();
    }
    return tex_u;
}

float Solution::get_tex_v() const
{
    if (!texture_coords_computed) {
        throw NoTextureCoords();
    }
    return tex_v;
}

Object Solution::get_object_intersected() const
{
    return object_intersected;
}
