#pragma once

#include "Material.hpp"
#include "Ray.hpp"

#include <vector>

constexpr float MAX_FLOAT {3.402823466e38};

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
        int type_number, 
        int texture_number, 
        Material object_material, 
        ObjectType object_type, 
        vec3f max_point,
        vec3f min_point
    );

    Object() :
    mat{},
    type{ObjectType::NoObject},
    id{-2},
    type_id{~0},
    texture_index{-1},
    max_point{},
    min_point{},
    average_position{}
    {};

    Material mat;
    ObjectType type;
    /**
     * @brief Contains the index into the Scene Object vector.
     */
    int id;
    /**
     * @brief Contains the index into the Scene (Sphere | Triangle) vector.
     */
    int type_id;

    int texture_index;

    vec3f max_point;
    vec3f min_point;
    vec3f average_position;
};

inline Object::Object(
        int object_number, 
        int type_number, 
        int texture_number, 
        Material object_material, 
        ObjectType object_type, 
        vec3f max,
        vec3f min
    ) :
    mat{object_material},
    type{object_type},
    id{object_number},
    type_id{type_number},
    texture_index{texture_number},
    max_point{max},
    min_point{min},
    average_position{}
{
    average_position = min_point + 0.5f*(max_point - min_point);
}