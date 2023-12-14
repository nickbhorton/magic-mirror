#include <iostream>
#include <fstream>
#include <complex>
#include <algorithm>
#include <execution>
#include <bits/stdc++.h>
#include <thread>

#include "lib/file.hpp"
#include "lib/maths.hpp"
#include "lib/color_math.hpp"
#include "lib/View.hpp"
#include "lib/Object.hpp"
#include "lib/Tree.h"


#ifdef DEBUG
extern long long number_of_intersect_node_calls; // from tree.cpp
extern long long number_of_solve_object_calls; // from solution.cpp
extern long total_clock_ticks_in_intersect_object; // from solution.cpp
extern long total_clock_ticks_in_get_smallest_pos_sol;
extern long total_clock_ticks_in_queue_while_loop;
#endif

int get_color_call_number = 0;

int current_pixel_x = 0;
int current_pixel_y = 0;

// -1 means don't inquire about a pixel
int pixel_inquiry_x = -1;
int pixel_inquiry_y = -1;

vec3f get_color_from_scene(const PPMFileArguments& ppm_args, const Ray& ray, int recursion_depth);

void write_color_array_to_file(const std::string& image_file_name, const std::vector<rgb8col>& color_vector, int width, int height){
    std::fstream image_file;
    image_file.open(image_file_name, std::ios::out);

    image_file << "P3 ";
    image_file << width << " " << height << " " << 255 << "\n";

    for (int i = 0; i < width * height; i++){
        if (i%width == pixel_inquiry_x && i/height == pixel_inquiry_y) {
            image_file 
            << (int) 255 << " "
            << (int) 0 << " "
            << (int) 255 << "\n";
        }
        else {
            image_file 
            << (int) color_vector[i].r << " "
            << (int) color_vector[i].g << " "
            << (int) color_vector[i].b << "\n";
        }

    }
    image_file.close();
}

void work_on_pixel_vector(std::vector<rgb8col>& pixel_vector, int start_index, int number_of_pixels_computed,
    const PPMFileArguments& ppm_args,
    const View& view
    )
{
    long clock_ticks = 0;
    for (int index = 0; index < number_of_pixels_computed; index++){
        int y = (start_index + index) / ppm_args.width;
        int x = (start_index + index) % ppm_args.width;
        vec3f view_point = view.ul + ((float)x)*view.delta_h + ((float)y)*view.delta_v;
        vec3f ray_dir = view_point - ppm_args.eye;
        ray_dir = ray_dir.normalize();
        Ray ray {};
        ray.origin = ppm_args.eye;
        ray.direction = ray_dir;
        // std::cout << x << " " << y << ":\n";
        current_pixel_x = x;
        current_pixel_y = y;
        clock_t start, end {};
        start = clock();
        pixel_vector[index] = to_color_rgb_255(get_color_from_scene(ppm_args, ray, 0));
        end = clock();
        clock_ticks += end - start;
    }
    std::cout << "images construction time (micro sec): " << clock_ticks << "\n";
}

int main(int argc, char* argv[]){
    ProgramArguments prog_args = parse_cmd_args(argc, argv);
    PPMFileArguments ppm_args = parse_input_file_args(prog_args.input_file, prog_args.relative_path_to_textures);

    // Construct BVH
    clock_t start, end {};
    start = clock();
    ppm_args.scene.object_tree = get_trunk_node(ppm_args.scene.objects);
    end = clock();
    // std::cout << "tree construction ticks: " << end-start << "\n";

    // Calculate view
    View v = create_view(ppm_args);
    int num_of_pixels = ppm_args.width * ppm_args.height;

    // Construct vector to put colors in
    std::vector<rgb8col> pixel_arrays_vector;
    pixel_arrays_vector.resize(num_of_pixels);

    // Add pixels to vector
    work_on_pixel_vector(pixel_arrays_vector, 0, num_of_pixels, ppm_args, v);

    // Write vector to a file
    std::string image_file_name{};
    if (ppm_args.image_name.size() > 0){
        image_file_name = ppm_args.image_name;
    }
    else {
        image_file_name = prog_args.output_file;
    }
    write_color_array_to_file(image_file_name, pixel_arrays_vector, ppm_args.width, ppm_args.height);
    return 0;
}

/**
 * currently turned off because not working with the refractions
*/
float get_in_shadow(const Scene& scene, const Ray &r, float distance_to_light){
    float shadow = 0;
    std::optional<Solution> sol = get_solution_from_tree(scene.object_tree, r, scene);
    if (sol.has_value()){
        float t = sol.value().get_smallest_t();
        if (t < distance_to_light){
            shadow = sol.value().object_intersected.mat.opacity;
        }
    }
    return shadow;
}


float get_num_rays_in_shadow(
    const PPMFileArguments& ppm_args, 
    const vec3f& intersection_point, 
    const Object& o, 
    const Light& light,
    const Ray& ray, 
    float distance_to_light
)
{
    float num_rays_in_shadow = 0;
    for (unsigned int shad_num = 0; shad_num < ppm_args.number_of_shadow_rays; shad_num++){                
            vec3f l_pos = light.position;
            l_pos.x += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * ppm_args.shadow_ray_variance;
            l_pos.y += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * ppm_args.shadow_ray_variance;
            l_pos.z += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * ppm_args.shadow_ray_variance;
            Ray r = ray;
            r.origin = intersection_point;
            r.direction = (l_pos - intersection_point).normalize();
            num_rays_in_shadow += get_in_shadow(ppm_args.scene, r, distance_to_light);
    }
    return num_rays_in_shadow;
}

vec3f get_local_illumination_color_f(
    const PPMFileArguments& ppm_args,
    const Solution& s
)
{
    vec3f total_color {};
    vec3f texture_color {};
    if (s.texture_coords_computed){
        texture_color = ppm_args.scene.textures[s.get_object_intersected().texture_index]->get_color(s.get_tex_u(), s.get_tex_v());
    }
    for (unsigned int i = 0; i < ppm_args.scene.lights.size(); i++){
        Ray direct_ray = s.ray;
        float distance_to_light {};

        if (ppm_args.scene.lights[i].point_light){
            vec3f dir = (ppm_args.scene.lights[i].position - s.get_position());
            direct_ray.direction = dir.normalize();
            direct_ray.origin = s.position;
            distance_to_light = dir.length();
        }
        else {
            direct_ray.direction = (-ppm_args.scene.lights[i].position).normalize();
            direct_ray.origin = s.position;
            distance_to_light = MAX_FLOAT;
        }
        vec3f L = direct_ray.direction;

        float sum_rays_shadow_intensity = 0.0f;
        float direct_ray_shadow_intensity = get_in_shadow(ppm_args.scene, direct_ray, distance_to_light);
        if (direct_ray_shadow_intensity > 0.001 && ppm_args.number_of_shadow_rays > 1){
            sum_rays_shadow_intensity = get_num_rays_in_shadow(
                ppm_args, s.get_position(), s.get_object_intersected(), 
                ppm_args.scene.lights[i], direct_ray, distance_to_light);
        }
        else if (direct_ray_shadow_intensity > 0.001){
            sum_rays_shadow_intensity = direct_ray_shadow_intensity;
        }

        // Assumption: Should already be normalized!
        vec3f N = s.get_normal();
        vec3f V = s.get_direction_to_ray_origin();
        V = V.normalize();
        vec3f H = (L + V).normalize();

        float light_intensity = 1.0f;
        if (sum_rays_shadow_intensity){
            float sum_rays_light_intensity = ppm_args.number_of_shadow_rays - sum_rays_shadow_intensity; 
            light_intensity = ((float) sum_rays_light_intensity) / ((float) ppm_args.number_of_shadow_rays);
        }

        vec3f current_diffuse_color;
        vec3f current_specular_color;
        if (s.texture_coords_computed){
            current_diffuse_color = light_intensity * s.get_object_intersected().mat.k_diffuse * std::max(0.0f, (N & L)) * texture_color;
            current_specular_color = light_intensity * s.get_object_intersected().mat.k_specular * ((float)pow(std::max(0.0f, (N & H)), s.get_object_intersected().mat.n)) * s.get_object_intersected().mat.color_specular_reflection;
            total_color = total_color + ((ppm_args.scene.lights[i].color) * (current_diffuse_color + current_specular_color));
        }
        else {
            current_diffuse_color = light_intensity * s.get_object_intersected().mat.k_diffuse * std::max(0.0f, (N & L)) * s.get_object_intersected().mat.color_intrinsic;
            current_specular_color = light_intensity * s.get_object_intersected().mat.k_specular * ((float)pow(std::max(0.0f, (N & H)), s.get_object_intersected().mat.n)) * s.get_object_intersected().mat.color_specular_reflection;
            total_color = total_color + ((ppm_args.scene.lights[i].color) * (current_diffuse_color + current_specular_color));
        }

    }
    if (s.texture_coords_computed){
        vec3f final_color = 
            s.get_object_intersected().mat.k_ambient * texture_color + 
            total_color;
        return final_color;
    }
    else {
        vec3f final_color = 
            s.get_object_intersected().mat.k_ambient * s.get_object_intersected().mat.color_intrinsic + 
            total_color;
        return final_color;
    }
    
}

vec3f get_color_from_scene(const PPMFileArguments& ppm_args, const Ray& ray, int recursion_depth){
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
        ++get_color_call_number;
        std::cout << "get_color() call number: " << get_color_call_number << " recursion depth: " << recursion_depth << "\n";
    }
    std::optional<Solution> sol = get_solution_from_tree(ppm_args.scene.object_tree, ray, ppm_args.scene);
    if (sol.has_value()){
        vec3f local_illumination_color = get_local_illumination_color_f(ppm_args, sol.value());
        // Reflection addition
        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
            std::cout << "solution ray object intersection id: " << sol.value().ray.intersected_obj_id;
            std::cout << " t -> " << sol.value().get_smallest_t() << "\n";
        }
        if (sol.value().object_intersected.mat.index_of_refraction > 1 && sol.value().object_intersected.mat.k_specular > 0.001){
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "reflect: \n";
            if (recursion_depth <= ppm_args.max_number_of_reflections){
                // handling the index of refractions 
                float ni = ppm_args.camera_index_of_refraction;
                float nt = sol.value().object_intersected.mat.index_of_refraction;

                if (!sol.value().ray.spheres_inside.empty()){
                    ni = ppm_args.scene.objects.at(sol.value().ray.spheres_inside.top()).mat.index_of_refraction;
                }

                Ray reflected_ray = get_reflected_ray(sol.value().ray, sol.value().position, sol.value().normal);
                
                float fresnel_reflectance = get_fresnel_reflectance(-sol.value().ray.direction,  sol.value().normal, ni, nt);

                if (fresnel_reflectance > 0.01){
                    local_illumination_color = local_illumination_color + 
                    fresnel_reflectance * 
                    get_color_from_scene(ppm_args, reflected_ray, recursion_depth + 1);
                }
            }
            else {
                if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
                    std::cout << "no reflect recursion limit\n";
                }
            }
        }
        // Refraction addition
        if (sol.value().object_intersected.mat.opacity < 0.999){
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "refract: \n";
            if (recursion_depth <= ppm_args.max_number_of_reflections){
                // handling the index of refractions 
                float ni = ppm_args.camera_index_of_refraction;
                float nt = sol.value().object_intersected.mat.index_of_refraction;

                if (!sol.value().ray.spheres_inside.empty()){
                    ni = ppm_args.scene.objects.at(sol.value().ray.spheres_inside.top()).mat.index_of_refraction;
                    if (sol.value().object_intersected.object_id == sol.value().ray.spheres_inside.top()){
                        sol.value().ray.spheres_inside.pop();
                        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "pop!\n";
                        if (!sol.value().ray.spheres_inside.empty()){
                            nt = ppm_args.scene.objects.at(sol.value().ray.spheres_inside.top()).mat.index_of_refraction;
                        }
                        else {
                            nt = ppm_args.camera_index_of_refraction;
                        }            
                    }
                    else {
                        sol.value().ray.spheres_inside.push(sol.value().object_intersected.object_id);
                    }
                }
                else if (sol.value().ray.spheres_inside.empty()){
                    sol.value().ray.spheres_inside.push(sol.value().object_intersected.object_id);
                }
                if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "ni: " << ni << "\n";
                if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "nt: " << nt << "\n";
                if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "size of stack: " << sol.value().ray.spheres_inside.size() << "\n";

                Ray refracted_ray = get_refracted_ray(sol.value().ray, sol.value().position, sol.value().normal, ni, nt);

                float fresnel_reflectance = 0.0f;
                if (sol.value().object_intersected.mat.k_specular > 0.001) {
                    fresnel_reflectance = get_fresnel_reflectance(-sol.value().ray.direction,  sol.value().normal, ni, nt);
                }

                float attenuation = (1.0f - sol.value().object_intersected.mat.opacity) * (1.0f - fresnel_reflectance);
                if (attenuation > 0.01){
                    local_illumination_color = local_illumination_color + attenuation * 
                    get_color_from_scene(ppm_args, refracted_ray, recursion_depth + 1);
                }
            }
            else {
                if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
                    std::cout << "no reflect recursion limit\n";
                }
            }
        }
            // terminate it number of reflections is too high
        if (recursion_depth > ppm_args.max_number_of_reflections){
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
                std::cout << "returned color '0s\n";
            }
            return vec3f(0, 0, 0);
        }
        else {
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "\treturned local illumination color\n";
            return vec3f(
                std::min(local_illumination_color.x, 1.0f),
                std::min(local_illumination_color.y, 1.0f),
                std::min(local_illumination_color.z, 1.0f)
            );
        }
    }
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "\tray ended in void\n";
    return ppm_args.bkgcolor;
}

