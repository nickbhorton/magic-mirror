#include "file.hpp"

inline bool print_debug_stuff = false;

ProgramArguments parse_cmd_args(int argc, char* argv[]) {
    ProgramArguments returned_struct {
        .input_file{},
        .output_file{},
        .relative_path_to_textures{}
    };

    int opt{0};
    while ((opt = getopt(argc, argv, "i:o:p:")) != -1) {
        switch (opt) {
            case 'i': returned_struct.input_file = optarg; break;
            case 'o': returned_struct.output_file = optarg; break;
            case 'p': returned_struct.relative_path_to_textures = optarg; break;
            default: usage(argv[0]); exit(EXIT_FAILURE);
        }
    }
    if (returned_struct.input_file.size() <= 0){
        if (print_debug_stuff) std::cout << "need a input file name!\nexiting process\n";
        exit(EXIT_FAILURE);
    }
    if (returned_struct.output_file.size() <= 0){
        returned_struct.output_file = "default_name.ppm";
    }
    else {
        if (print_debug_stuff) std::cout << "output file: " << returned_struct.output_file << "\n";
    }
    return returned_struct;
}

bool get_some_vertex(std::ifstream& input_file, const PPMFileArguments& ppm_args, vec3f& p, vec3f& n, vec3f& t){
    std::string s;
    input_file >> s;
    int int_array[3];
    if (print_debug_stuff) std::cout << "string: " << s << "\n";
    if (sscanf(s.c_str(), "%d/%d/%d", int_array + 0, int_array + 1, int_array + 2) == 3){
        if (print_debug_stuff) std::cout << int_array[0] << " " << int_array[1] << " " << int_array[2] << " \n";
        p = ppm_args.scene.vertexs[int_array[0] - 1];
        n = ppm_args.scene.vertex_normals[int_array[1] - 1];
        t = ppm_args.scene.texture_coords[int_array[2] - 1];
        return true;
    }
    else if (sscanf(s.c_str(), "%d//%d", int_array + 0,  int_array + 1) == 2){
        if (print_debug_stuff) std::cout << int_array[0] << " " << int_array[1] << "\n";
        p = ppm_args.scene.vertexs[int_array[0] - 1];
        n = ppm_args.scene.vertex_normals[int_array[1] - 1];
        return true;
    }
    else if (sscanf(s.c_str(), "%d/%d", int_array + 0,  int_array + 1) == 2){
        if (print_debug_stuff) std::cout << int_array[0] << " " << int_array[1] << "\n";
        p = ppm_args.scene.vertexs[int_array[0] - 1];
        t = ppm_args.scene.texture_coords[int_array[1] - 1];
        return false;
    }
    else if (sscanf(s.c_str(), "%d", int_array + 0) == 1){
        if (print_debug_stuff) std::cout << int_array[0] << "\n";
        p = ppm_args.scene.vertexs[int_array[0] - 1];
        return false;
    }
    else throw "No matched pattern for triangle input in input file";
}

PPMFileArguments parse_input_file_args(std::string file_name, std::string relative_path){
    PPMFileArguments ppm_args{
        .width = 0,
        .height = 0,
        .eye = vec3f(0.0f, 0.0f, 0.0f),
        .viewdir = vec3f(0.0f, 0.0f, 0.0f),
        .updir = vec3f(0.0f, 0.0f, 0.0f),
        .vfov = 0,
        .bkgcolor = vec3f(0.0f, 0.0f, 0.0f),
        .mtlcolor = vec3f(0.0f, 0.0f, 0.0f),
        .scene = Scene(),
        .max_number_of_reflections = 3,
        .number_of_shadow_rays = 1,
        .shadow_ray_variance = 0.03,
        .image_name {},
        .camera_index_of_refraction {}
    };
    std::ifstream input_file(file_name.c_str());
    if (!input_file.good()){
        if (print_debug_stuff) std::cout << "Errors with the input file passed\nExiting process\n";
        exit(EXIT_FAILURE);
    }

    if (input_file.is_open()){
        std::string word;

        Material current_mat = Material();
        bool texture_interpolation = true;

        while(input_file >> word) {
            if (word[0] == '#'){
                // Allows for comments 
                // after a # character the rest of the line is thrown away
                std::string line {};
                std::getline(input_file, line);
            }
            if (word == "image_name"){
                ppm_args.image_name = get_arg_from_file<std::string>(input_file, "image name");
            }
            if (word == "interp"){
                texture_interpolation = (bool) get_arg_from_file<int>(input_file, "interpolation");
            }
            if (word == "imsize"){
                ppm_args.width = get_arg_from_file<int>(input_file, "width");
                ppm_args.height = get_arg_from_file<int>(input_file, "height");
            }
            if (word == "eye"){
                ppm_args.eye = get_vec_from_file<float, 3>(input_file, "eye");
            }
            if (word == "viewdir"){
                ppm_args.viewdir = get_vec_from_file<float, 3>(input_file, "viewdir");
            }
            if (word == "updir"){
                ppm_args.updir = get_vec_from_file<float, 3>(input_file, "updir");
            }
            if (word == "vfov"){
                ppm_args.vfov = get_arg_from_file<float>(input_file, "vfov");
            }
            if (word == "bkgcolor"){
                ppm_args.bkgcolor = get_vec_from_file<float, 3>(input_file, "bkgcolor");
                ppm_args.camera_index_of_refraction = get_arg_from_file<float>(input_file, "bkg index of reflection");
            }
            if (word == "mtlcolor" || word == "materialcolor"){
                current_mat.color_intrinsic = get_vec_from_file<float, 3>(input_file, "mtlcolor color_intrinsic");
                current_mat.color_specular_reflection = get_vec_from_file<float, 3>(input_file, "mtlcolor color_specular_reflection");
                current_mat.k_ambient = get_arg_from_file<float>(input_file, "mtlcolor k_ambient");
                current_mat.k_diffuse = get_arg_from_file<float>(input_file, "mtlcolor k_diffuse");
                current_mat.k_specular = get_arg_from_file<float>(input_file, "mtlcolor k_specular");
                current_mat.n = get_arg_from_file<unsigned int>(input_file, "mtlcolor n");
                current_mat.opacity = get_arg_from_file<float>(input_file, "opacity");
                current_mat.index_of_refraction = get_arg_from_file<float>(input_file, "index of refraction");
            }
            if (word == "light"){
                Light l;
                l.position = get_vec_from_file<float, 3>(input_file, "light position");
                int is_point_light = get_arg_from_file<int>(input_file, "light is_point_light");
                is_point_light ? l.point_light = true : l.point_light = false;
                l.color = get_vec_from_file<float, 3>(input_file, "light color");
                ppm_args.scene.lights.push_back(l);   
            }
            if (word == "mtlreflectiveness"){
                current_mat.reflectiveness = get_arg_from_file<float>(input_file, "reflectiveness");
            }
            if (word == "reflections"){
                ppm_args.max_number_of_reflections = get_arg_from_file<int>(input_file, "reflections");
            }
            if (word == "shadowrays"){
                ppm_args.number_of_shadow_rays = get_arg_from_file<int>(input_file, "shadow ray number");
            }
            if (word == "shadowrayvariance"){
                ppm_args.shadow_ray_variance = get_arg_from_file<float>(input_file, "shadow ray variance");
            }
            if (word == "sphere"){
                Sphere sphere = Sphere();
                sphere.position = get_vec_from_file<float, 3>(input_file, "sphere position");
                sphere.radius = get_arg_from_file<float>(input_file, "sphere radius");
                Object object = Object(
                    ppm_args.scene.objects.size(),
                    ppm_args.scene.spheres.size(), 
                    ppm_args.scene.textures.size() - 1,
                    current_mat, ObjectType::Sphere,
                    sphere.get_max_point(),
                    sphere.get_min_point()
                );
                ppm_args.scene.objects.push_back(object);
                ppm_args.scene.spheres.push_back(sphere);
            }
            if (word == "v"){
                ppm_args.scene.vertexs.push_back(get_vec_from_file<float, 3>(input_file, "vertex"));
            }
            if (word == "vn"){
                ppm_args.scene.vertex_normals.push_back(get_vec_from_file<float, 3>(input_file, "vertex normal"));
            }
            if (word == "vt"){
                vec3f vt = vec3f();
                vt.x = get_arg_from_file<float>(input_file, "texture u");
                vt.y = get_arg_from_file<float>(input_file, "texture v");
                ppm_args.scene.texture_coords.push_back(vt);
            }
            if (word == "f"){
                Triangle triangle = Triangle();
                bool has_normals1 = get_some_vertex(input_file, ppm_args, triangle.p0, triangle.n0, triangle.t0);
                bool has_normals2 = get_some_vertex(input_file, ppm_args, triangle.p1, triangle.n1, triangle.t1);
                bool has_normals3 = get_some_vertex(input_file, ppm_args, triangle.p2, triangle.n2, triangle.t2);
                triangle.has_normals = has_normals1 && has_normals2 && has_normals3;

                Object object = Object(
                    ppm_args.scene.objects.size(),
                    ppm_args.scene.triangles.size(), 
                    ppm_args.scene.textures.size() - 1,
                    current_mat, ObjectType::Triangle,
                    triangle.get_max_point(),
                    triangle.get_min_point()
                    );

                ppm_args.scene.objects.push_back(object);
                ppm_args.scene.triangles.push_back(triangle);
            }
            if (word == "texture"){
                std::string file_name;
                input_file >> file_name;
                std::unique_ptr<Texture> tex {new Texture(relative_path + file_name)};
                tex->interpolation = texture_interpolation;
                ppm_args.scene.textures.push_back(std::move(tex));
            }
        }
        input_file.close();
    }
    return ppm_args;
}

void usage(const char* program) {
    std::cerr << "Usage:\n"
        << "    " << program << "\n"
        << "\t-i input file name\n"
        << "\t-o output file name\n"
        << "\t-p relative path to texture directory\n";
}
