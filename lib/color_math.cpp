#include "color_math.hpp"

rgb8col mix_rgb_additive(rgb8col col1, rgb8col col2, float col1_mix_probability)
{
    return rgb8col(
        (uint8_t) ( ((float)col1.r * col1_mix_probability) + ((float)col2.r * (1.0f - col1_mix_probability))),
        (uint8_t) ( ((float)col1.g * col1_mix_probability) + ((float)col2.g * (1.0f - col1_mix_probability))),
        (uint8_t) ( ((float)col1.b * col1_mix_probability) + ((float)col2.b * (1.0f - col1_mix_probability)))
    );
}

rgb8col to_color_rgb_255(vec3f vec_of_unit_intervals){
    if (vec_of_unit_intervals.x > 1.0f){
        vec_of_unit_intervals.x = 1.0f;
    }
    if (vec_of_unit_intervals.x < 0.0f){
        vec_of_unit_intervals.x = 0.0f;
    }
    if (vec_of_unit_intervals.y > 1.0f){
        vec_of_unit_intervals.y = 1.0f;
    }
    if (vec_of_unit_intervals.y < 0.0f){
        vec_of_unit_intervals.y = 0.0f;
    }
    if (vec_of_unit_intervals.z > 1.0f){
        vec_of_unit_intervals.z = 1.0f;
    }
    if (vec_of_unit_intervals.z < 0.0f){
        vec_of_unit_intervals.z = 0.0f;
    }
    return rgb8col(
                (uint8_t) (vec_of_unit_intervals.x * 255.0),
                (uint8_t) (vec_of_unit_intervals.y * 255.0),
                (uint8_t) (vec_of_unit_intervals.z * 255.0)
            );
}

vec3f to_color_rgb_f(rgb8col col){
    return vec3f(
                ((float)col.x / 255.0f),
                ((float)col.y / 255.0f),
                ((float)col.z / 255.0f)
            );
}