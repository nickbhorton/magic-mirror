#include "parsing.hpp"

inline float from_string(const std::string& string, int line_number, int arg_number) {
    try {
        float a1 = std::stof(string);
        return a1;
    }
    catch (const std::invalid_argument& e){
        std::cout << "Error in line " << line_number 
            << " argument " << arg_number << ": " << "invalid argument\n"; 
    }
    std::exit(EXIT_FAILURE);
    return 0.0f;
}

std::string strip(const std::string& string) {
    size_t comment_start_position = string.find_first_of('#');
    if (comment_start_position == std::string::npos) {
        return string;
    }
    return string.substr(0, comment_start_position);
}

static std::optional<MaterialLineType> strings_to_variant(const std::vector<std::string>& strings, int line_number){
        if (strings[0] == "newmtl") {
            return LineTypes::NewMaterial{
                .value = strings[1]
            };
        }
        if (strings[0] == "Ns") {
            float a1 = from_string(strings[1], line_number, 1);
            return LineTypes::SpecularExponent{
                .value = a1
            };
        }
        if (strings[0] == "Ka") {
            float a1 = from_string(strings[1], line_number, 1);
            float a2 = from_string(strings[2], line_number, 2);
            float a3 = from_string(strings[3], line_number, 3);
            return LineTypes::AmbientColor{
                .value = vec::create(a1, a2, a3)
            };
        }
        if (strings[0] == "Kd") {
            float a1 = from_string(strings[1], line_number, 1);
            float a2 = from_string(strings[2], line_number, 2);
            float a3 = from_string(strings[3], line_number, 3);
            return LineTypes::DiffuseColor{
                .value = vec::create(a1, a2, a3)
            };
        }
        if (strings[0] == "Ks") {
            float a1 = from_string(strings[1], line_number, 1);
            float a2 = from_string(strings[2], line_number, 2);
            float a3 = from_string(strings[3], line_number, 3);
            return LineTypes::SpecularColor{
                .value = vec::create(a1, a2, a3)
            };
        }
        if (strings[0] == "Ni") {
            float a1 = from_string(strings[1], line_number, 1);
            return LineTypes::IndexOfRefraction{
                .value = a1
            };
        }
        if (strings[0] == "d") {
            float a1 = from_string(strings[1], line_number, 1);
            return LineTypes::Opacity{
                .value = a1
            };
        }
        return {};
}

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

std::optional<MaterialLineType> parse_stripped_material_line(
    std::stringstream line, 
    const std::vector<std::pair<std::string, int>>& tokens_argument_number_map,
    int line_number
)
{
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
            if (at(tokens_argument_number_map, strings[0]) == number_of_arguments){
                // This has a valid number of arguments for the valid token
                return strings_to_variant(strings, line_number);
            }
            else {
                std::cerr << "Error in line " << line_number << ": "
                    << "wrong number of arguments passed. "
                    << "Correct number of arguments: " << at(tokens_argument_number_map, strings[0]) - 1 
                    << ". Given number of arguments: " << number_of_arguments << "\n";
                return {}; 
            }
        }
        else {
            std::cerr << "Error in line " << line_number << ": "
                << "unknown word " << strings[0] << "\n";
            return {}; 
        }
    }
    return {}; 
}