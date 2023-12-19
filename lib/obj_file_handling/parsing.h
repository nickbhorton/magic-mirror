#ifndef WAVEFRONT_HEADER
#define WAVEFRONT_HEADER

#include <unordered_map>
#include <string>

#include "Wavefront.hpp"


enum class MaterialLineTokens {
    None,
    NewMaterial,
    AmbientColor,
    DefuseColor,
    SpecularColor,
    SpecularExponent,
    IndexOfRefraction,
    Opacity
};

const static std::unordered_map<std::string, MaterialLineTokens> token_map {
    {"newmtl", MaterialLineTokens::NewMaterial},
    {"Ka", MaterialLineTokens::AmbientColor},
    {"Kd", MaterialLineTokens::DefuseColor},
    {"Ks", MaterialLineTokens::SpecularColor},
    {"Ns", MaterialLineTokens::SpecularExponent},
    {"Ni", MaterialLineTokens::IndexOfRefraction},
    {"d", MaterialLineTokens::Opacity}
};

#endif