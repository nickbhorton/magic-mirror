#pragma once

#include "vec/Vec.hpp"
#include "color_math.hpp"

#include <string>
#include <fstream>
#include <iostream>
#include <memory>

class Texture {
    public:
        Texture(const std::string& file_name);

        vec3f get_color(float u, float v) const;

        bool contains_texture;
        bool interpolation;
    private:
        int width;
        int height;
        std::unique_ptr<std::vector<rgb8col>> color_array;
};