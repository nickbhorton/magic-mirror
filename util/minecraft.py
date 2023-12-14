def print_face(int_list_original, orientation_list, orientation_list_index, t_n, texture_offset, overall_texture_offset):
    texture_offset = texture_offset * 4
    int_list = []
    for num in int_list_original:
        int_list.append(num - 1)

    if (orientation_list[orientation_list_index] == 0):
        print("f", str(t_n + int_list[0]) + "/" + str(1+texture_offset+overall_texture_offset), str(t_n + int_list[1]) + "/" + str(2+texture_offset+overall_texture_offset), str(t_n + int_list[2]) + "/" + str(3+texture_offset+overall_texture_offset))
        print("f", str(t_n + int_list[2]) + "/" + str(3+texture_offset+overall_texture_offset), str(t_n + int_list[3]) + "/" + str(4+texture_offset+overall_texture_offset), str(t_n + int_list[0]) + "/" + str(1+texture_offset+overall_texture_offset))
    elif (orientation_list[orientation_list_index] == 1):
        print("f", str(t_n + int_list[0]) + "/" + str(2+texture_offset+overall_texture_offset), str(t_n + int_list[1]) + "/" + str(3+texture_offset+overall_texture_offset), str(t_n + int_list[2]) + "/" + str(4+texture_offset+overall_texture_offset))
        print("f", str(t_n + int_list[2]) + "/" + str(4+texture_offset+overall_texture_offset), str(t_n + int_list[3]) + "/" + str(1+texture_offset+overall_texture_offset), str(t_n + int_list[0]) + "/" + str(2+texture_offset+overall_texture_offset))
    elif (orientation_list[orientation_list_index] == 2):
        print("f", str(t_n + int_list[0]) + "/" + str(3+texture_offset+overall_texture_offset), str(t_n + int_list[1]) + "/" + str(4+texture_offset+overall_texture_offset), str(t_n + int_list[2]) + "/" + str(1+texture_offset+overall_texture_offset))
        print("f", str(t_n + int_list[2]) + "/" + str(1+texture_offset+overall_texture_offset), str(t_n + int_list[3]) + "/" + str(2+texture_offset+overall_texture_offset), str(t_n + int_list[0]) + "/" + str(3+texture_offset+overall_texture_offset))
    elif (orientation_list[orientation_list_index] == 3):
        print("f", str(t_n + int_list[0]) + "/" + str(4+texture_offset+overall_texture_offset), str(t_n + int_list[1]) + "/" + str(1+texture_offset+overall_texture_offset), str(t_n + int_list[2]) + "/" + str(2+texture_offset+overall_texture_offset))
        print("f", str(t_n + int_list[2]) + "/" + str(2+texture_offset+overall_texture_offset), str(t_n + int_list[3]) + "/" + str(3+texture_offset+overall_texture_offset), str(t_n + int_list[0]) + "/" + str(4+texture_offset+overall_texture_offset))

def print_cube(x, y, z, tex_x, tex_y, tex_indexs, t_n, orientation_num, oto):
    texture_width = 16
    texture_height = 64
    tex_x_float = 1/texture_width
    tex_y_float = 1/texture_height

    print("v", x+0, y+0, z+0)
    print("v", x+0, y+0, z+1)
    print("v", x+0, y+1, z+0)
    print("v", x+0, y+1, z+1)
    print("v", x+1, y+0, z+0)
    print("v", x+1, y+0, z+1)
    print("v", x+1, y+1, z+0)
    print("v", x+1, y+1, z+1)

    overall_texture_addition = 0
    for i in range(len(tex_x)):
        print("vt", tex_x[i]*tex_x_float, tex_y[i]*tex_y_float)
        top_left = str(0 + t_n)
        print("vt", tex_x[i]*tex_x_float + tex_x_float, tex_y[i]*tex_y_float)
        top_right = str(1 + t_n)
        print("vt", tex_x[i]*tex_x_float + tex_x_float, tex_y[i]*tex_y_float + tex_y_float)
        bot_right = str(2 + t_n)
        print("vt", tex_x[i]*tex_x_float, tex_y[i]*tex_y_float + tex_y_float)
        bot_left = str(3 + t_n)
        overall_texture_addition += 4

    print_face([8,7,3,4], orientation_num, 0, t_n, tex_indexs[0], oto)
    print_face([8,6,5,7], orientation_num, 1, t_n, tex_indexs[1], oto)
    print_face([8,4,2,6], orientation_num, 2, t_n, tex_indexs[2], oto)
    print_face([7,5,1,3], orientation_num, 3, t_n, tex_indexs[3], oto)
    print_face([4,3,1,2], orientation_num, 4, t_n, tex_indexs[4], oto)
    print_face([1,5,6,2], orientation_num, 5, t_n, tex_indexs[5], oto)
    t_n += 8
    return (t_n, oto + overall_texture_addition)
    


    
t_n = 1
overall_texture_addition = 0
#                                                                                          +Y +X +Z -Z -X -Y
# TNT
(t_n, overall_texture_addition) = print_cube( 0, 0, 0,[8,9,10],[0,0,0],[1,0,0,0,0,2], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0, 0,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1, 0,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0, 1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1, 1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube(-1, 0,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0,-1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1,-1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube(-1, 1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube(-1,-1,-2,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)

(t_n, overall_texture_addition) = print_cube(-1,-2,-1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube(-1,-2, 0,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube(-1,-2, 1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0,-2,-1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0,-2, 0,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 0,-2, 1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1,-2,-1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1,-2, 0,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
(t_n, overall_texture_addition) = print_cube( 1,-2, 1,[8]     ,[1]    ,[0,0,0,0,0,0], t_n, [0, 1, 0, 1, 0, 0], overall_texture_addition)
# t_n = print_cube(1,0,0,8,0, t_n, [1])
# t_n = print_cube(2,0,0,8,0, t_n, [2])
# t_n = print_cube(3,0,0,8,0, t_n, [3])