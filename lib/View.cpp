#include "View.hpp"

inline bool print_debug_stuff = false;

View create_view(const PPMFileArguments& ppm_args){
    View view {};
    vec3f viewing_direction = ppm_args.viewdir;

    vec3f u = (viewing_direction | ppm_args.updir);
    u = u.normalize();
    if (print_debug_stuff) std::cout << "\nu: " << u << "\n";
    vec3f v = (u | viewing_direction);
    v = v.normalize();
    if (print_debug_stuff) std::cout << "v: " << v << "\n";
    vec3f n = viewing_direction.normalize();
    if (print_debug_stuff) std::cout << "n: " << n << "\n";

    // 180 is max degrees for width or height of viewing window
    float aspect_ratio = (float) ppm_args.width / (float) ppm_args.height;
    float d = 5.0;
    float h = 2 * d * tan(((0.5) * ppm_args.vfov) * (PI / 180.0));
    float w = h * aspect_ratio;

    if (print_debug_stuff) std::cout << "h:" << h << "\n";
    if (print_debug_stuff) std::cout << "w:" << w << "\n";

    vec3f ul = ppm_args.eye + (d * n) - ((w/2) * u) + ((h/2) * v);
    vec3f ur = ppm_args.eye + (d * n) + ((w/2) * u) + ((h/2) * v);
    vec3f ll = ppm_args.eye + (d * n) - ((w/2) * u) - ((h/2) * v);
    vec3f lr = ppm_args.eye + (d * n) + ((w/2) * u) - ((h/2) * v);

    if (print_debug_stuff) std::cout << "ul: " << ul << "\n";
    if (print_debug_stuff) std::cout << "ur: " << ur << "\n";
    if (print_debug_stuff) std::cout << "ll: " << ll << "\n";
    if (print_debug_stuff) std::cout << "lr: " << lr << "\n";

    vec3f delta_h =  (float)(1.0 / (float)(ppm_args.width - 0.999)) * (ur - ul);
    vec3f delta_v =  (float)(1.0 / (float)(ppm_args.height - 0.999)) * (ll - ul);

    view.ll = ll;
    view.lr = lr;
    view.ul = ul;
    view.ur = ur;
    view.delta_h = delta_h;
    view.delta_v = delta_v;
    return view;
}