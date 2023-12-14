#include "png_encoder.hpp"

void write_ihdr_chunk(std::fstream &file_stream, unsigned int width, unsigned int height){
    file_stream << 0x49484452;
    // Width	4 bytes
    char* width_ptr = reinterpret_cast<char*>(&width);
    file_stream << width_ptr[0];
    file_stream << width_ptr[1];
    file_stream << width_ptr[2];
    file_stream << width_ptr[3];
    // Height	4 bytes
    char* height_ptr = reinterpret_cast<char*>(&height);
    file_stream << height_ptr[0];
    file_stream << height_ptr[1];
    file_stream << height_ptr[2];
    file_stream << height_ptr[3];
    // Bit depth	1 byte
    file_stream << (char) 8;
    // Colour type	1 byte
        // True color is 2 RGB triplet
    file_stream << (char) 2;
    // Compression method	1 byte
    file_stream << (char) 0;
    // Filter method	1 byte
    file_stream << (char) 0;
    // Interlace method	1 byte
    file_stream << (char) 0;

}

void write_color_array_to_png(std::fstream &file_stream, rgb8col *array, unsigned int width, unsigned int height)
{
    file_stream << 0x89504E470D0A1A0A;
}

