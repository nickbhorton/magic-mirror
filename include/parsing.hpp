#ifndef PARSING_HEADER
#define PARSING_HEADER

#include <unordered_map>
#include <string>
#include <variant>
#include <optional>
#include <fstream>
#include <vector>
#include <array>

#include "matrix.hpp"
#include "Triangle.hpp"

class WavefrontMaterial {
public:
    WavefrontMaterial(const std::string& mtl_file_path);

    float spectral_exponent;
    float opacity;
    float index_of_refraction;
    vec3f ambient_color;
    vec3f diffuse_color;
    vec3f spectral_color;
    std::string name;
};

class WavefrontObject {
private:
    std::unordered_map<std::string, WavefrontMaterial> materials;
    std::unique_ptr<std::vector<vec3f>> vertexes;
    std::unique_ptr<std::vector<vec3f>> normals;
    std::unique_ptr<std::vector<vec2f>> texture_coordinates;
    bool smooth_shading;
};


const std::vector<std::pair<std::string, int>> material_token_map = {
    {"newmtl", 1},
    {"Ns", 1},
    {"Ka", 3},
    {"Kd", 3},
    {"Ks", 3},
    {"Ni", 1},
    {"d", 1},
};

const std::vector<std::pair<std::string, int>> object_token_map = {
    {"mtllib", 1},
    {"o", 1},
    {"v", 3},
    {"vn", 3},
    {"vt", 2},
    {"s", 1},
    {"usemtl", 1},
    {"f", 3},
};

namespace MaterialLineTypes {
    struct NewMaterial {std::string value;};
    struct AmbientColor {vec3f value;};
    struct DiffuseColor {vec3f value;};
    struct SpecularColor {vec3f value;};
    struct SpecularExponent {float value;};
    struct IndexOfRefraction {float value;};
    struct Opacity {float value;};
}

typedef std::variant<
    MaterialLineTypes::NewMaterial,
    MaterialLineTypes::AmbientColor,
    MaterialLineTypes::DiffuseColor,
    MaterialLineTypes::SpecularColor,
    MaterialLineTypes::SpecularExponent,
    MaterialLineTypes::IndexOfRefraction,
    MaterialLineTypes::Opacity
> MaterialToken;

namespace ObjectLineTypes {
    struct Material {WavefrontMaterial value;};
    struct ObjectName {std::string value;};
    struct Vertex {vec3f value;};
    struct Normal {vec3f value;};
    struct TextureCoord {vec2f value;};
    struct SmoothShading {bool value;};
    struct MaterialCall {std::string value;};
    struct FaceToTriangle {Triangle value;};
    struct FaceToQuad {std::pair<Triangle, Triangle> value;};
}

typedef std::variant<
    ObjectLineTypes::Material,
    ObjectLineTypes::ObjectName,
    ObjectLineTypes::Vertex,
    ObjectLineTypes::Normal,
    ObjectLineTypes::TextureCoord,
    ObjectLineTypes::SmoothShading,
    ObjectLineTypes::MaterialCall,
    ObjectLineTypes::FaceToTriangle,
    ObjectLineTypes::FaceToQuad
> ObjectToken;


namespace parsing {
    // std::vector will never be of a size large enough to have a std::unordered_map to be worth overhead
    inline bool contains(const std::vector<std::pair<std::string, int>>& key_value_vector, std::string key){
        for (const auto& e : key_value_vector){
            if (e.first == key) {
                return true;
            }
        }
        return false;
    }

    inline int at(const std::vector<std::pair<std::string, int>>& key_value_vector, std::string key){
        for (const auto& e : key_value_vector){
            if (e.first == key) {
                return e.second;
            }
        }
        return 0;
    }

    inline float from_string(const std::string& string, int line_number, int arg_number) {
        try {
            float a1 = std::stof(string);
            return a1;
        }
        catch (const std::invalid_argument& e){
            std::cout << "Error on line " << line_number 
                << " argument " << arg_number << ": " << "invalid argument\n"; 
        }
        std::exit(EXIT_FAILURE);
        return 0.0f;
    }

    /**
     * @brief Removes comments in the for of '# this is a comment'
    */
    inline std::string strip(const std::string& string) {
        size_t comment_start_position = string.find_first_of('#');
        if (comment_start_position == std::string::npos) {
            return string;
        }
        return string.substr(0, comment_start_position);
    }

    std::vector<std::string> get_lines(std::ifstream& input_file);
    namespace material {
        std::optional<MaterialToken> strings_to_variant_material(const std::vector<std::string>& strings, int line_number);
    }

    namespace object {
        std::optional<ObjectToken> strings_to_variant_object(const std::vector<std::string>& strings, int line_number);
    }

    template<typename TokenType>
    std::optional<TokenType> parse_stripped_lines(
        std::stringstream line, 
        const std::vector<std::pair<std::string, int>>& tokens_argument_number_map,
        std::optional<TokenType> (*strings_to_variant) (const std::vector<std::string> &, int),
        int line_number
    ) {
        std::vector<std::string> strings;
        std::string buffer;
        while (line >> buffer){
            strings.push_back(buffer);
            buffer.clear();
        }
        const int number_of_strings = strings.size();
        if (number_of_strings > 0){
            if (contains(tokens_argument_number_map, strings[0])) {
                const int number_of_arguments = number_of_strings - 1;
                if (at(tokens_argument_number_map, strings[0]) <= number_of_arguments){
                    // This has a valid number of arguments for the valid token
                    return strings_to_variant(strings, line_number);
                }
                else {
                    std::cerr << "Error on line " << line_number << ": "
                        << "wrong number of arguments passed. "
                        << "Correct number of arguments: " << at(tokens_argument_number_map, strings[0]) - 1 
                        << ". Given number of arguments: " << number_of_arguments << "\n";
                    return {}; 
                }
            }
            else {
                std::cerr << "Warning on line " << line_number << ": "
                    << "unknown word " << strings[0] << "\n";
                return {}; 
            }
        }
        return {}; 
    }

    template<typename TokenType>
    std::vector<TokenType> get_tokens(
        const std::vector<std::string>& lines,
        std::optional<TokenType> (*strings_to_variant) (const std::vector<std::string> &, int)
    ) {
        std::vector<TokenType> tokens {};
        for (unsigned int i = 0; i < lines.size(); i++){
            auto result = parse_stripped_lines(std::stringstream(strip(lines[i])), material_token_map, strings_to_variant, i);
            if (result.has_value()){
                tokens.push_back(result.value());
            }
        }
        return tokens;
    }
}

#endif