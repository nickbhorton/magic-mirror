#pragma once

#include "Material.hpp"
#include "Ray.hpp"

#include <vector>

#define MAX_FLOAT 3.402823466e38

class NoSolution : public std::exception {
    std::string what() {
        return "Solution not computed";
    }
};

class NoTextureCoords : public std::exception {
    std::string what() {
        return "Solution not computed";
    }
};

enum class ObjectType {
    NoObject,
    Sphere,
    Triangle,
};

class Object {
public:
    Object(
        int object_number, 
        unsigned int type_number, 
        int texture_number, 
        Material object_material, 
        ObjectType object_type, 
        vec3f max_point,
        vec3f min_point
    );
    Object();


    Material mat;
    ObjectType type;
    /**
     * @brief Contains the index into the Scene Object vector.
     */
    int object_id;
    /**
     * @brief Contains the index into the Scene (Sphere | Triangle) vector.
     */
    int type_id;

    int texture_index;

    vec3f max_point;
    vec3f min_point;
    vec3f average_position;
};