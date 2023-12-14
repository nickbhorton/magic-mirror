#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "Light.hpp"
#include "vec/Vec.hpp"
#include "Texture.hpp"
#include "Triangle.hpp"
#include "Sphere.hpp"
#include "Object.hpp"
#include "Tree.h"

struct SceneSettings {
    SceneSettings() :
    background_color{},
    max_number_of_reflections{3},
    number_of_shadow_rays{1},
    shadow_ray_variance{0.03}
    {};

    vec3f background_color;
    unsigned int max_number_of_reflections;
    unsigned int number_of_shadow_rays;
    float shadow_ray_variance;
};

struct Scene {
    Scene() :
    lights{},
    vertexs{},
    vertex_normals{},
    texture_coords{},
    textures{},
    objects{},
    triangles{},
    spheres{},
    object_tree{},
    settings{}
    {};
    
    std::vector<Light> lights;
    std::vector<vec3f> vertexs;
    std::vector<vec3f> vertex_normals;
    std::vector<vec3f> texture_coords;
    std::vector<std::unique_ptr<Texture>> textures;

    std::vector<Object> objects;
    std::vector<Triangle> triangles;
    std::vector<Sphere> spheres;

    std::unique_ptr<TreeNode> object_tree;

    SceneSettings settings;
};


#endif