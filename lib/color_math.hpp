#pragma once

#include "vec/Vec.hpp"
#include "Light.hpp"

#include <vector>
#include <random>

rgb8col mix_rgb_additive(rgb8col col1, rgb8col col2, float col1_mix_probability);
rgb8col to_color_rgb_255(vec3f vec_of_unit_intervals);
vec3f to_color_rgb_f(rgb8col col);