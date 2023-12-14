#pragma once

#include "vec/Vec.hpp"
#include "file.hpp"

struct View {
    vec3f ul;
    vec3f ur;
    vec3f ll;
    vec3f lr;
    vec3f delta_h;
    vec3f delta_v;
};

View create_view(const PPMFileArguments& ppm_args);