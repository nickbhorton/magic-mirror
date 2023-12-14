#include "Texture.hpp"

Texture::Texture(const std::string& file_name) :
    contains_texture {false},
    interpolation {false},
    width {},
    height {},
    color_array {new std::vector<rgb8col>{}}
{
    std::ifstream file(file_name.c_str());
    if (file.good()){
        // std::cout << "texture found " << file_name << "\n";
        std::string input_string;
        file >> input_string;
        if (!(input_string == "P3")){
            std::cout << "Texture file not a P3 ppm texture" << "\n";
            return;
        }
        input_string.clear();
        file >> width;
        // std::cout << "texture width: " << width << "\n";
        file >> height;
        // std::cout << "texture height: " << height << "\n";
        file >> input_string;
        // dont care about maximum color
        input_string.clear();
        contains_texture = true;
        for (int i = 0; i < width * height; i++){
            int r, g, b;
            file >> r;
            file >> g;
            file >> b;
            // if (i==0 || i == width * height - 1){
            //     std::cout << (int) r << " " << (int) g << " " << (int) b << "\n";
            // }
            color_array->push_back(rgb8col((uint8_t)r, (uint8_t)g, (uint8_t)b));
        }
        // std::cout << file_name << ": " <<width * height << "\n";
    }
    else {
        std::cout << "error: texture not found " << file_name << "\n";
        contains_texture = false;
    }
}

vec3f Texture::get_color(float u, float v) const
{
    if (u < 0.0f){
        u = 0.0f;
    }
    if (v < 0.0f){
        v = 0.0f;
    }
    while (u > 1.0f){
        u = u - 1.0f;
    }
    while (v > 1.0f){
        v = v - 1.0f;
    }
    float x = u * (width - 1);
    float y = v * (height - 1);
    int i = std::floor(x);
    int j = std::floor(y);
    int i_round = std::round(x);
    int j_round = std::round(y);
    float alpha = x - static_cast<float>(i);
    float beta = y - static_cast<float>(j);
    if (interpolation){
        // texture wrapping
        if (i > width - 2){
            i = i % (width - 2);
        }
        if (j > height - 2){
            j = j % (height - 2);
        }
        assert(i <= width - 2);
        assert(j <= height - 2);
        return 
            (1.0f - alpha)*(1.0f - beta)*to_color_rgb_f(color_array->at(width * (j+0) + (i+0))) +
            (alpha)*(1.0f - beta)*to_color_rgb_f(color_array->at(width * (j+0) + (i+1))) +
            (1.0f - alpha)*(beta)*to_color_rgb_f(color_array->at(width * (j+1) + (i+0))) +
            (alpha)*(beta)*to_color_rgb_f(color_array->at(width * (j+1) + (i+1)));
    }
    else {
        // texture wrapping
        if (i_round > width - 1){
            i_round = i_round % (width - 1);
        }
        if (j_round > height - 1){
            j_round = j_round % (height - 1);
        }
        return to_color_rgb_f(color_array->at(width * j_round + i_round));
    }
}