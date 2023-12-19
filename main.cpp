#include <iostream>
#include <fstream>
#include <complex>
#include <algorithm>
#include <execution>
#include <bits/stdc++.h>
#include <thread>

#include "file.hpp"
#include "maths.hpp"
#include "View.hpp"
#include "Object.hpp"
#include "Tree.h"


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

vec3f get_color_from_scene(const Settings& settings, const Ray& ray, int recursion_depth);

void write_color_array_to_file(const std::string& image_file_name, const std::vector<vec3u8>& color_vector, int width, int height){
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
            << (int) color_vector[i].x() << " "
            << (int) color_vector[i].y() << " "
            << (int) color_vector[i].z() << "\n";
        }

    }
    image_file.close();
}

void work_on_pixel_vector(
    std::vector<vec3u8>& pixel_vector, 
    int start_index, 
    int number_of_pixels_computed,
    const Settings& settings,
    const View& view
    )
{
    long clock_ticks = 0;
    for (int index = 0; index < number_of_pixels_computed; index++){
        const int y = (start_index + index) / settings.image.pixel_width;
        const int x = (start_index + index) % settings.image.pixel_width;
        const vec3f view_point = view.ul + ((float)x)*view.delta_h + ((float)y)*view.delta_v;
        const vec3f ray_dir = vec::normalize(view_point - settings.camera.position);
        Ray ray {};
        ray.origin = settings.camera.position;
        ray.direction = ray_dir;
        // std::cout << x << " " << y << ":\n";
        current_pixel_x = x;
        current_pixel_y = y;
        clock_t start, end {};
        start = clock();
        pixel_vector[index] = to_vec3u8(get_color_from_scene(settings, ray, 0));
        end = clock();
        clock_ticks += end - start;
    }
    std::cout << "images construction time (micro sec): " << clock_ticks << "\n";
}

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
    const Scene& scene, 
    const vec3f& intersection_point, 
    const vec3f& light_position,
    const Ray& ray, 
    float distance_to_light
)
{
    float num_rays_in_shadow = 0;
    for (unsigned int shad_num = 0; shad_num < scene.settings.number_of_shadow_rays; shad_num++){                
            vec3f l_pos = light_position;
            l_pos[0] += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * scene.settings.shadow_ray_variance;
            l_pos[1] += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * scene.settings.shadow_ray_variance;
            l_pos[2] += ((rand() / double(RAND_MAX))*2.0f - 1.0f) * scene.settings.shadow_ray_variance;
            Ray r = ray;
            r.origin = intersection_point;
            r.direction = vec::normalize(l_pos - intersection_point);
            num_rays_in_shadow += get_in_shadow(scene, r, distance_to_light);
    }
    return num_rays_in_shadow;
}

vec3f get_local_illumination_color(const Scene& scene,const Solution& s)
{
    vec3f total_color {};
    // Either gets color from objects texture or material
    const vec3f& material_color = s.texture_coords_computed ? 
        scene.textures[s.get_object_intersected().texture_index]->get_color(s.get_tex_u(), s.get_tex_v()) :
        s.get_object_intersected().mat.color_intrinsic;

    for (unsigned int i = 0; i < scene.lights.size(); i++){
        Ray direct_ray = s.ray;
        float distance_to_light {};

        if (scene.lights[i].point_light){
            vec3f dir = (scene.lights[i].position - s.get_position());
            direct_ray.direction = vec::normalize(dir);
            direct_ray.origin = s.position;
            distance_to_light = vec::length(dir);
        }
        else {
            direct_ray.direction = vec::normalize(-scene.lights[i].position);
            direct_ray.origin = s.position;
            distance_to_light = MAX_FLOAT;
        }
        vec3f L = direct_ray.direction;

        float sum_rays_shadow_intensity = 0.0f;
        const float direct_ray_shadow_intensity = get_in_shadow(scene, direct_ray, distance_to_light);
        if (direct_ray_shadow_intensity > 0.001 && scene.settings.number_of_shadow_rays > 1){
            sum_rays_shadow_intensity = get_num_rays_in_shadow(
                scene, 
                s.get_position(), 
                scene.lights[i].position, 
                direct_ray, 
                distance_to_light
            );
        }
        else if (direct_ray_shadow_intensity > 0.001){
            sum_rays_shadow_intensity = direct_ray_shadow_intensity;
        }

        // Assumption: Should already be normalized!
        const vec3f& N = s.get_normal();
        const vec3f& V = vec::normalize(s.get_direction_to_ray_origin());
        const vec3f& H = vec::normalize(L + V);

        float light_intensity {1.0f};
        if (sum_rays_shadow_intensity){
            float sum_rays_light_intensity = scene.settings.number_of_shadow_rays - sum_rays_shadow_intensity; 
            light_intensity = ((float) sum_rays_light_intensity) / ((float) scene.settings.number_of_shadow_rays);
        }

        vec3f current_diffuse_color = light_intensity 
                * s.get_object_intersected().mat.k_diffuse 
                * std::max(0.0f, vec::dot(N, L)) 
                * material_color;

        vec3f current_specular_color = light_intensity 
            * s.get_object_intersected().mat.k_specular 
            * ((float)pow(std::max(0.0f, vec::dot(N, H)), s.get_object_intersected().mat.n)) 
            * s.get_object_intersected().mat.color_specular_reflection;
        total_color[0] = total_color.x() + ((scene.lights[i].color.x()) * (current_diffuse_color + current_specular_color).x());
        total_color[1] = total_color.y() + ((scene.lights[i].color.y()) * (current_diffuse_color + current_specular_color).y());
        total_color[2] = total_color.z() + ((scene.lights[i].color.z()) * (current_diffuse_color + current_specular_color).z());
    }
    return vec3f{s.get_object_intersected().mat.k_ambient * material_color + total_color};
}

// TODO: Rework this function for readability
// This is the work horse of the whole engine.
vec3f get_color_from_scene(const Settings& ppm_args, const Ray& ray, int recursion_depth){
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
        ++get_color_call_number;
        std::cout << "get_color() call number: " << get_color_call_number << " recursion depth: " << recursion_depth << "\n";
    }
    std::optional<Solution> sol = get_solution_from_tree(ppm_args.scene.object_tree, ray, ppm_args.scene);
    if (sol.has_value()){
        vec3f local_illumination_color = get_local_illumination_color(ppm_args.scene, sol.value());
        // Reflection addition
        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
            std::cout << "solution ray object intersection id: " << sol.value().ray.intersected_obj_id;
            std::cout << " t -> " << sol.value().get_smallest_t() << "\n";
        }
        if (sol.value().object_intersected.mat.index_of_refraction > 1 && sol.value().object_intersected.mat.k_specular > 0.001){
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "reflect: \n";
            if (recursion_depth <= ppm_args.scene.settings.max_number_of_reflections){
                // handling the index of refractions 
                float ni = ppm_args.scene.settings.max_number_of_reflections;
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
            if (recursion_depth <= ppm_args.scene.settings.max_number_of_reflections){
                // handling the index of refractions 
                float ni = ppm_args.camera.index_of_refraction;
                float nt = sol.value().object_intersected.mat.index_of_refraction;

                if (!sol.value().ray.spheres_inside.empty()){
                    ni = ppm_args.scene.objects.at(sol.value().ray.spheres_inside.top()).mat.index_of_refraction;
                    if (sol.value().object_intersected.id == sol.value().ray.spheres_inside.top()){
                        sol.value().ray.spheres_inside.pop();
                        if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "pop!\n";
                        if (!sol.value().ray.spheres_inside.empty()){
                            nt = ppm_args.scene.objects.at(sol.value().ray.spheres_inside.top()).mat.index_of_refraction;
                        }
                        else {
                            nt = ppm_args.camera.index_of_refraction;
                        }            
                    }
                    else {
                        sol.value().ray.spheres_inside.push(sol.value().object_intersected.id);
                    }
                }
                else if (sol.value().ray.spheres_inside.empty()){
                    sol.value().ray.spheres_inside.push(sol.value().object_intersected.id);
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
        if (recursion_depth > ppm_args.scene.settings.max_number_of_reflections){
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) {
                std::cout << "returned color '0s\n";
            }
            return vec::create(0.f, 0.f, 0.f);
        }
        else {
            if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "\treturned local illumination color\n";
            return vec::create(
                std::min(local_illumination_color.get(0), 1.0f),
                std::min(local_illumination_color.get(1), 1.0f),
                std::min(local_illumination_color.get(2), 1.0f)
            );
        }
    }
    if (current_pixel_x == pixel_inquiry_x && current_pixel_y == pixel_inquiry_y) std::cout << "\tray ended in void\n";
    return ppm_args.scene.settings.background_color;
}

int main(int argc, char* argv[]){
    const auto arguments_from_command_line = parse_cmd_args(argc, argv);
    auto settings = parse_input_file_args(arguments_from_command_line.input_file, arguments_from_command_line.relative_path_to_textures);

    // Construct BVH
    settings.scene.object_tree = get_trunk_node(settings.scene.objects);
    
    // Calculate view
    const View v = create_view(settings.camera, settings.image);
    const int num_of_pixels = settings.image.pixel_width * settings.image.pixel_height;

    std::vector<vec3u8> pixel_arrays_vector;
    pixel_arrays_vector.resize(num_of_pixels);

    // Add pixels to vector
    work_on_pixel_vector(pixel_arrays_vector, 0, num_of_pixels, settings, v);

    // Write vector to a file
    std::string image_file_name{};
    if (settings.image.name.size() > 0){
        image_file_name = settings.image.name;
    }
    else {
        image_file_name = arguments_from_command_line.output_file;
    }
    write_color_array_to_file(image_file_name, pixel_arrays_vector, settings.image.pixel_width, settings.image.pixel_height);
    return 0;
}