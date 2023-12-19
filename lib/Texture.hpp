#pragma once

#include "matrix.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <memory>
#include <vector>
#include <assert.h>

class Texture {
    public:
        Texture(const std::string& file_name);

        vec3f get_color(float u, float v) const;

        bool contains_texture;
        bool interpolation;
    private:
        int width;
        int height;
        std::unique_ptr<std::vector<vec3u8>> color_array;
};

static constexpr uint8_t span_u8(const float& f) {
    if (f > 1.0f){
        return 255;
    }
    else if (f < 0.0f){
        return 0;
    }
    else {
        return static_cast<uint8_t>(f * 255);
    }
}

inline vec3u8 to_vec3u8(const vec3f& color){
    return vec::create(span_u8(color.get(0)), span_u8(color.get(1)), span_u8(color.get(2)));
}

inline vec3f to_vec3f(vec3u8 col){
    return vec::create(((float)col.get(0) / 255.0f), ((float)col.get(1) / 255.0f), ((float)col.get(2) / 255.0f));
}