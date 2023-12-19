#include "View.hpp"

constexpr bool debug = false;
constexpr float PI = 3.14159265359;

View create_view(const CameraSettings& camera_settings, const ImageSettings& image_settings){
    // Constructing camera axis
    const vec3f u = vec::normalize(vec::cross(camera_settings.view_direction, camera_settings.up_direction));
    const vec3f v = vec::normalize(vec::cross(u, camera_settings.view_direction));
    const vec3f n = vec::normalize(camera_settings.view_direction);

    // Calculating width and height of viewport
    // 180 is the largest angle for width or height of viewing window
    const float aspect_ratio = (float) image_settings.pixel_width / (float) image_settings.pixel_height;
    // TODO: give these better names
    constexpr float d = 5.0;
    const float h = 2 * d * tan(((0.5) * image_settings.vertical_fov) * (PI / 180.0));
    const float w = h * aspect_ratio;

    View view {};
    view.ul = camera_settings.position + (d * n) - ((w/2) * u) + ((h/2) * v);
    view.ur = camera_settings.position + (d * n) + ((w/2) * u) + ((h/2) * v);
    view.ll = camera_settings.position + (d * n) - ((w/2) * u) - ((h/2) * v);
    view.lr = camera_settings.position + (d * n) + ((w/2) * u) - ((h/2) * v);
    view.delta_h =  (float)(1.0 / (float)(image_settings.pixel_width - 0.999)) * (view.ur - view.ul);
    view.delta_v  =  (float)(1.0 / (float)(image_settings.pixel_height - 0.999)) * (view.ll - view.ul);

    // TODO: improve debug messages
    if constexpr (debug) {
        std::cout << "\nu: " << u << "\n";
        std::cout << "v: " << v << "\n";
        std::cout << "n: " << n << "\n";

        std::cout << "h:" << h << "\n";
        std::cout << "w:" << w << "\n";

        std::cout << "ul: " << view.ul << "\n";
        std::cout << "ur: " << view.ur << "\n";
        std::cout << "ll: " << view.ll << "\n";
        std::cout << "lr: " << view.lr << "\n";
    }
    return view;
}