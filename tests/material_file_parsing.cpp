#include <gtest/gtest.h>
#include "parsing.hpp"

TEST(LEXING_MATERIAL_FILES, newmtl_token){
    auto m = parse_stripped_material_line(std::stringstream("newmtl Material"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::NewMaterial>(m.value()));
    EXPECT_EQ(std::get<LineTypes::NewMaterial>(m.value()).value, std::string("Material"));
}

TEST(LEXING_MATERIAL_FILES, Ns_token){
    auto m = parse_stripped_material_line(std::stringstream("Ns 250.000000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::SpecularExponent>(m.value()));
    EXPECT_EQ(std::get<LineTypes::SpecularExponent>(m.value()).value, 250.f);
}

TEST(LEXING_MATERIAL_FILES, Ka_token){
    auto m = parse_stripped_material_line(std::stringstream("Ka 1.000000 1.000000 1.000000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::AmbientColor>(m.value()));
    EXPECT_EQ(std::get<LineTypes::AmbientColor>(m.value()).value, vec::create(1.f, 1.f, 1.f));
}

TEST(LEXING_MATERIAL_FILES, Kd_token){
    auto m = parse_stripped_material_line(std::stringstream("Kd 0.800000 0.800000 0.800000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::DiffuseColor>(m.value()));
    EXPECT_EQ(std::get<LineTypes::DiffuseColor>(m.value()).value, vec::create(0.8f, 0.8f, 0.8f));
}

TEST(LEXING_MATERIAL_FILES, Ks_token){
    auto m = parse_stripped_material_line(std::stringstream("Ks 0.500000 0.500000 0.500000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::SpecularColor>(m.value()));
    EXPECT_EQ(std::get<LineTypes::SpecularColor>(m.value()).value, vec::create(0.5f, 0.5f, 0.5f));
}

TEST(LEXING_MATERIAL_FILES, Ni_token){
    auto m = parse_stripped_material_line(std::stringstream("Ni 1.450000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::IndexOfRefraction>(m.value()));
    EXPECT_EQ(std::get<LineTypes::IndexOfRefraction>(m.value()).value, 1.45f);
}

TEST(LEXING_MATERIAL_FILES, d_token){
    auto m = parse_stripped_material_line(std::stringstream("d 1.000000"), material_token_map, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<LineTypes::Opacity>(m.value()));
    EXPECT_EQ(std::get<LineTypes::Opacity>(m.value()).value, 1.f);
}

TEST(LEXING_MATERIAL_FILES, not_a_token){
    auto m = parse_stripped_material_line(std::stringstream("Ke 0.000000 0.000000 0.000000"), material_token_map, 1);
    EXPECT_FALSE(m.has_value());
}

TEST(LEXING_UTILS, strip) {
    std::string str {"d 1.000000 # this is the opacity"};
    str = strip(str);
    EXPECT_EQ(str, "d 1.000000 ");
    str = "#d 1.000000 # this is the opacity";
    str = strip(str);
    EXPECT_EQ(str, "");
}
