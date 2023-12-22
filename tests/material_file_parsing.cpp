#include <gtest/gtest.h>
#include "parsing.hpp"

TEST(LEXING_MATERIAL_FILES, newmtl_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("newmtl Material"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::NewMaterial>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::NewMaterial>(m.value()).value, std::string("Material"));
}

TEST(LEXING_MATERIAL_FILES, Ns_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Ns 250.000000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::SpecularExponent>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::SpecularExponent>(m.value()).value, 250.f);
}

TEST(LEXING_MATERIAL_FILES, Ka_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Ka 1.000000 1.000000 1.000000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::AmbientColor>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::AmbientColor>(m.value()).value, vec::create(1.f, 1.f, 1.f));
}

TEST(LEXING_MATERIAL_FILES, Kd_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Kd 0.800000 0.800000 0.800000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::DiffuseColor>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::DiffuseColor>(m.value()).value, vec::create(0.8f, 0.8f, 0.8f));
}

TEST(LEXING_MATERIAL_FILES, Ks_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Ks 0.500000 0.500000 0.500000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::SpecularColor>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::SpecularColor>(m.value()).value, vec::create(0.5f, 0.5f, 0.5f));
}

TEST(LEXING_MATERIAL_FILES, Ni_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Ni 1.450000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::IndexOfRefraction>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::IndexOfRefraction>(m.value()).value, 1.45f);
}

TEST(LEXING_MATERIAL_FILES, d_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("d 1.000000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<MaterialLineTypes::Opacity>(m.value()));
    EXPECT_EQ(std::get<MaterialLineTypes::Opacity>(m.value()).value, 1.f);
}

TEST(LEXING_MATERIAL_FILES, not_a_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("Ke 0.000000 0.000000 0.000000"), material_token_map, parsing::material::strings_to_variant_material, 1);
    EXPECT_FALSE(m.has_value());
}

TEST(LEXING_UTILS, strip) {
    std::string str {"d 1.000000 # this is the opacity"};
    str = parsing::strip(str);
    EXPECT_EQ(str, "d 1.000000 ");
    str = "#d 1.000000 # this is the opacity";
    str = parsing::strip(str);
    EXPECT_EQ(str, "");
}

TEST(LEXING_UTILS, basic_file) {
    std::ifstream f;
    f.open("../../objects/cube.mtl", std::ifstream::in);
    EXPECT_TRUE(f.good());
    std::vector<std::string> lines = parsing::get_lines(f);
    EXPECT_EQ(lines.size(), 11);
    for (unsigned int i = 0; i < lines.size(); i++){
        std::cout << lines[i] << "\n";
    }
    EXPECT_EQ(lines[0], "# Blender 4.0.2 MTL File: 'None'");
    EXPECT_EQ(lines[1], "# www.blender.org");
    EXPECT_EQ(lines[2], "");
    EXPECT_EQ(lines[3], "newmtl Material");
    EXPECT_EQ(lines[4], "Ns 250.000000 # specular exponent");
    EXPECT_EQ(lines[5], "Ka 1.000000 1.000000 1.000000");
    EXPECT_EQ(lines[6], "Kd 0.800000 0.800000 0.800000");
    EXPECT_EQ(lines[7], "Ks 0.500000 0.500000 0.500000");
    EXPECT_EQ(lines[8], "Ke 0.000000 0.000000 0.000000");
    EXPECT_EQ(lines[9], "Ni 1.450000 # Index of refraction");
    EXPECT_EQ(lines[10], "d 1.000000 # opacity");
    for (unsigned int i = 0; i < lines.size(); i++){
        lines[i] = parsing::strip(lines[i]);
    }
    EXPECT_EQ(lines[0], "");
    EXPECT_EQ(lines[1], "");
    EXPECT_EQ(lines[2], "");
    EXPECT_EQ(lines[3], "newmtl Material");
    EXPECT_EQ(lines[4], "Ns 250.000000 ");
    EXPECT_EQ(lines[5], "Ka 1.000000 1.000000 1.000000");
    EXPECT_EQ(lines[6], "Kd 0.800000 0.800000 0.800000");
    EXPECT_EQ(lines[7], "Ks 0.500000 0.500000 0.500000");
    EXPECT_EQ(lines[8], "Ke 0.000000 0.000000 0.000000");
    EXPECT_EQ(lines[9], "Ni 1.450000 ");
    EXPECT_EQ(lines[10], "d 1.000000 ");
}

TEST(LEXING_UTILS, cube_material) {
    auto material = WavefrontMaterial(std::string("../../objects/cube.mtl"));
    EXPECT_EQ(material.name, "Material");
    EXPECT_FLOAT_EQ(material.spectral_exponent, 250.f);
    EXPECT_FLOAT_EQ(material.index_of_refraction, 1.45f);
    EXPECT_FLOAT_EQ(material.opacity, 1.f);
    EXPECT_EQ(material.ambient_color, vec::create(1.f, 1.f, 1.f));
    EXPECT_EQ(material.diffuse_color, vec::create(0.8f, 0.8f, 0.8f));
    EXPECT_EQ(material.spectral_color, vec::create(0.5f, 0.5f, 0.5f));
}
