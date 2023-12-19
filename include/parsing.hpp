#ifndef PARSING_HEADER
#define PARSING_HEADER

#include <unordered_map>
#include <string>
#include <variant>
#include <optional>

#include "Wavefront.hpp"
#include "matrix.hpp"

const std::vector<std::pair<std::string, int>> material_token_map = {
    {"newmtl", 1},
    {"Ns", 1},
    {"Ka", 3},
    {"Kd", 3},
    {"Ks", 3},
    {"Ni", 1},
    {"d", 1},
};

namespace LineTypes {
    struct NewMaterial {
        std::string value;
    };
    struct AmbientColor {
        vec3f value;
    };
    struct DiffuseColor {
        vec3f value;
    };
    struct SpecularColor {
        vec3f value;
    };
    struct SpecularExponent {
        float value;
    };
    struct IndexOfRefraction {
        float value;
    };
    struct Opacity {
        float value;
    };
}

typedef std::variant<
    LineTypes::NewMaterial,
    LineTypes::AmbientColor,
    LineTypes::DiffuseColor,
    LineTypes::SpecularColor,
    LineTypes::SpecularExponent,
    LineTypes::IndexOfRefraction,
    LineTypes::Opacity
> MaterialLineType;

std::optional<MaterialLineType> parse_stripped_material_line(
    std::stringstream line, 
    const std::vector<std::pair<std::string, int>>& tokens_argument_number_map,
    int line_number
);

/**
 * @brief Removes comments in the for of '# this is a comment'
*/
std::string strip(const std::string& string);

#endif