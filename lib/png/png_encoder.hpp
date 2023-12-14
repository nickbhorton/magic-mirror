#pragma once

#include "../vec/Vec.hpp"

#include <fstream>

void write_color_array_to_png(std::fstream& file_stream, rgb8col* array, int number_of_pixels);