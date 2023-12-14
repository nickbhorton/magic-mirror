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

class Scene {
    public:
        Scene();

        std::vector<Light> lights;
        std::vector<vec3f> vertexs;
        std::vector<vec3f> vertex_normals;
        std::vector<vec3f> texture_coords;
        std::vector<std::unique_ptr<Texture>> textures;

        std::vector<Object> objects;
        std::vector<Triangle> triangles;
        std::vector<Sphere> spheres;

        std::unique_ptr<TreeNode> object_tree;
};


#endif