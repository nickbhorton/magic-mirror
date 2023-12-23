#include "parsing.hpp"
#include "Triangle.hpp"

std::optional<MaterialToken> parsing::material::strings_to_variant_material(const std::vector<std::string>& strings, int line_number){
    if (strings[0] == "newmtl") {
        return MaterialLineTypes::NewMaterial{
            .value = strings[1]
        };
    }
    else if (strings[0] == "Ns") {
        float a1 = from_string(strings[1], line_number, 1);
        return MaterialLineTypes::SpecularExponent{
            .value = a1
        };
    }
    else if (strings[0] == "Ka") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        float a3 = from_string(strings[3], line_number, 3);
        return MaterialLineTypes::AmbientColor{
            .value = vec::create(a1, a2, a3)
        };
    }
    else if (strings[0] == "Kd") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        float a3 = from_string(strings[3], line_number, 3);
        return MaterialLineTypes::DiffuseColor{
            .value = vec::create(a1, a2, a3)
        };
    }
    else if (strings[0] == "Ks") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        float a3 = from_string(strings[3], line_number, 3);
        return MaterialLineTypes::SpecularColor{
            .value = vec::create(a1, a2, a3)
        };
    }
    else if (strings[0] == "Ni") {
        float a1 = from_string(strings[1], line_number, 1);
        return MaterialLineTypes::IndexOfRefraction{
            .value = a1
        };
    }
    else if (strings[0] == "d") {
        float a1 = from_string(strings[1], line_number, 1);
        return MaterialLineTypes::Opacity{
            .value = a1
        };
    }
    return {};
}

// TODO: error handling
inline vec3i get_vertex(const std::string & str) {
    constexpr int none = -1;
    int v = none;
    int vt = none;
    int vn = none;

    std::cout << "STRING: " << str << "\n";
    vec3i result = vec::create(v, vt, vn);
    if (sscanf(str.c_str(), "%d/%d/%d", &v, &vt, &vn) == 3) {
        std::cout << "%d/%d/%d IF STATEMENT" << "\n";
        return vec::create(v, vt, vn);
    }
    else if (sscanf(str.c_str(), "%d//%d", &v, &vn) == 2) {
        std::cout << "%d//%d IF STATEMENT" << "\n";
        return vec::create(v, vt, vn);
    }
    else if (sscanf(str.c_str(), "%d/%d", &v, &vt) == 2) {
        std::cout << "%d/%d IF STATEMENT" << "\n";
        return vec::create(v, vt, vn);
    }
    else if (sscanf(str.c_str(), "%d", &v) == 1) {
        std::cout << "%d IF STATEMENT" << "\n";
        return vec::create(v, vt, vn);
    }
    std::cerr << "Error: vertex was not formatted correctly\n";
    return result;
}

std::optional<ObjectToken> parsing::object::strings_to_variant_object(const std::vector<std::string>& strings, int line_number){
    if (strings[0] == "mtllib") {
        return ObjectLineTypes::Material{
            // TODO: error handling for file paths
            .value = WavefrontMaterial(strings[1])
        };
    }
    if (strings[0] == "o") {
        return ObjectLineTypes::ObjectName{
            .value = strings[1]
        };
    }
    else if (strings[0] == "v") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        float a3 = from_string(strings[3], line_number, 3);
        return ObjectLineTypes::Vertex{
            .value = vec::create(a1, a2, a3)
        };
    }
    else if (strings[0] == "vn") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        float a3 = from_string(strings[3], line_number, 3);
        return ObjectLineTypes::Normal{
            .value = vec::create(a1, a2, a3)
        };
    }
    else if (strings[0] == "vt") {
        float a1 = from_string(strings[1], line_number, 1);
        float a2 = from_string(strings[2], line_number, 2);
        return ObjectLineTypes::TextureCoord{
            .value = vec::create(a1, a2)
        };
    }
    else if (strings[0] == "s") {
        // TODO: do this correctly
        return ObjectLineTypes::SmoothShading{
            .value = false
        };
    }
    else if (strings[0] == "usemtl") {
        return ObjectLineTypes::MaterialCall{
            .value = strings[1]
        };
    }
    else if (strings[0] == "f") {
        Face f = {
            .v1 = get_vertex(strings[1]),
            .v2 = get_vertex(strings[2]),
            .v3 = get_vertex(strings[3])
        };
        return ObjectLineTypes::FaceType {
            .value = f
        };
    }
    return {};
}

std::vector<std::string> parsing::get_lines(std::ifstream& input_file){
    std::vector<std::string> lines {};
    std::string buffer {};
    while (std::getline(input_file, buffer)) {
        lines.push_back(buffer);
        buffer.clear();
    }
    return lines;
}