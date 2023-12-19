#include "Texture.hpp"

Texture::Texture(const std::string& file_name) :
    contains_texture {false},
    interpolation {false},
    width {},
    height {},
    color_array {std::make_unique<std::vector<vec3u8>>()}
{
    std::ifstream file(file_name);
    if (file.good()){
        std::string input_string;
        file >> input_string;
        if (!(input_string == "P3")){
            std::cout << "Texture file not a P3 ppm texture, didn't have magic number" << "\n";
            return;
        }
        input_string.clear();
        file >> width;
        file >> height;
        // 4th parameter is color size, support only for u8
        file >> input_string;
        input_string.clear();
        contains_texture = true;
        for (int i = 0; i < width * height; i++){
            int r, g, b;
            file >> r;
            file >> g;
            file >> b;
            color_array->push_back(vec::create((uint8_t)r, (uint8_t)g, (uint8_t)b));
        }
    }
    else {
        std::cerr << "error: texture not found " << file_name << "\n";
        contains_texture = false;
    }
}

vec3f Texture::get_color(float u, float v) const
{
    const float x = u * (width - 1);
    const float y = v * (height - 1);
    if (interpolation){
        int i = std::floor(x);
        int j = std::floor(y);
        float alpha = x - static_cast<float>(i);
        float beta = y - static_cast<float>(j);
        // texture wrapping
        if (i > width - 2){
            i %= (width - 2);
        }
        if (j > height - 2){
            j %= (height - 2);
        }
        assert(i <= width - 2);
        assert(j <= height - 2);
        assert(i >= 0);
        assert(j >= 0);
        return 
            (1.0f - alpha)*(1.0f - beta)*to_vec3f(color_array->at(width * (j+0) + (i+0))) +
            (alpha)       *(1.0f - beta)*to_vec3f(color_array->at(width * (j+0) + (i+1))) +
            (1.0f - alpha)*(beta)       *to_vec3f(color_array->at(width * (j+1) + (i+0))) +
            (alpha)*(beta)              *to_vec3f(color_array->at(width * (j+1) + (i+1)));
    }
    else {
        int i = std::round(x);
        int j = std::round(y);
        // texture wrapping
        if (i > width - 2){
            i %= (width - 2);
        }
        if (j > height - 2){
            j %= (height - 2);
        }
        assert(i <= width - 2);
        assert(j <= height - 2);
        assert(i >= 0);
        assert(j >= 0);
        return to_vec3f(color_array->at(width * j + i));
    }
}