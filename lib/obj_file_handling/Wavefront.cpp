#include "parsing.hpp"

WavefrontMaterial::WavefrontMaterial(const std::string& mtl_file_path) :
    spectral_exponent{1.0f},
    opacity{1.0f},
    index_of_refraction{1.0f},
    ambient_color{},
    diffuse_color{},
    spectral_color{},
    name{}
{
    std::ifstream f;
    f.open(mtl_file_path, std::ifstream::in);
    if (!f.good()){
        std::cerr << "File " << mtl_file_path << " could not be found\n";
    }
    std::vector<std::string> lines = parsing::get_lines(f);
    std::vector<MaterialToken> tokens = parsing::get_tokens(lines, parsing::material::strings_to_variant_material);
    for (const auto& token : tokens) {
        if (std::holds_alternative<MaterialLineTypes::NewMaterial>(token)){
            name = std::get<MaterialLineTypes::NewMaterial>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::SpecularExponent>(token)){
            spectral_exponent = std::get<MaterialLineTypes::SpecularExponent>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::IndexOfRefraction>(token)){
            index_of_refraction = std::get<MaterialLineTypes::IndexOfRefraction>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::Opacity>(token)){
            opacity = std::get<MaterialLineTypes::Opacity>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::AmbientColor>(token)){
            ambient_color = std::get<MaterialLineTypes::AmbientColor>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::DiffuseColor>(token)){
            diffuse_color = std::get<MaterialLineTypes::DiffuseColor>(token).value;
        }
        else if (std::holds_alternative<MaterialLineTypes::SpecularColor>(token)){
            spectral_color = std::get<MaterialLineTypes::SpecularColor>(token).value;
        }
    }
    if (name.length() == 0){
        std::cerr << "Semantic Error : Material files require a name field. Add a line newmtl <name>\n";
        std::exit(EXIT_FAILURE);
    }
}