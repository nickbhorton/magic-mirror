#include <gtest/gtest.h>
#include "parsing.hpp"

TEST(LEXING_OBJECT_FILES, mtllib_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("mtllib ../../objects/cube.mtl"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::Material>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.name, "Material");
    EXPECT_FLOAT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.spectral_exponent, 250.f);
    EXPECT_FLOAT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.index_of_refraction, 1.45f);
    EXPECT_FLOAT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.opacity, 1.f);
    EXPECT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.ambient_color, vec::create(1.f, 1.f, 1.f));
    EXPECT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.diffuse_color, vec::create(0.8f, 0.8f, 0.8f));
    EXPECT_EQ(std::get<ObjectLineTypes::Material>(m.value()).value.spectral_color, vec::create(0.5f, 0.5f, 0.5f));
}

TEST(LEXING_OBJECT_FILES, o_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("o Cube"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::ObjectName>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::ObjectName>(m.value()).value, std::string("Cube"));
}

TEST(LEXING_OBJECT_FILES, v_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("v 1.000000 1.000000 -1.000000"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::Vertex>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::Vertex>(m.value()).value, vec::create(1.f, 1.f, -1.f));
}

TEST(LEXING_OBJECT_FILES, vn_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("vn 1.000000 1.000000 -1.000000"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::Normal>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::Normal>(m.value()).value, vec::create(1.f, 1.f, -1.f));
}

TEST(LEXING_OBJECT_FILES, vt_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("vt 1.000000 1.000000"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::TextureCoord>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::TextureCoord>(m.value()).value, vec::create(1.f, 1.f));
}

TEST(LEXING_OBJECT_FILES, usemtl_token){
    auto m = parsing::parse_stripped_lines(std::stringstream("usemtl Material"), object_token_map, parsing::object::strings_to_variant_object, 1);
    EXPECT_TRUE(m.has_value());
    EXPECT_TRUE(std::holds_alternative<ObjectLineTypes::MaterialCall>(m.value()));
    EXPECT_EQ(std::get<ObjectLineTypes::MaterialCall>(m.value()).value, "Material");
}