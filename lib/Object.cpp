#include "Object.hpp"

// Object stuff

Object::Object(
        int object_number, 
        unsigned int type_number, 
        int texture_number, 
        Material object_material, 
        ObjectType object_type, 
        vec3f max,
        vec3f min
    )
{
    mat = object_material;
    object_id = object_number;
    type = object_type;
    type_id = type_number;
    texture_index = texture_number;
    max_point = max;
    min_point = min;
    average_position = min_point + 0.5f*(max_point - min_point);
}

Object::Object()
{
    mat = Material();
    object_id = -2;
    type = ObjectType::NoObject;
    type_id = ~0;
    texture_index = -1;
    max_point = vec3f{};
    min_point = vec3f{};
    average_position = vec3f{};
}