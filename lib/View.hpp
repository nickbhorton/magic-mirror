#pragma once

#include "vec/Vec.hpp"
#include "file.hpp"

/**
 * @brief Holds information about the view port.
*/
struct View {
    View() :
    ul{},
    ur{},
    ll{},
    lr{},
    delta_h{},
    delta_v{}
    {};

    vec3f ul;
    vec3f ur;
    vec3f ll;
    vec3f lr;
    vec3f delta_h;
    vec3f delta_v;
};

View create_view(const CameraSettings& camera_settings, const ImageSettings& image_settings);