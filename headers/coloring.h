#pragma once
#include <iostream>
#include  "vec3.h"
using color =Vector3d;

void write_color(std::ostream &image ,const color &pixel_color){
    auto ir = static_cast<unsigned char>(255.999f * pixel_color.get_r());
    auto ig = static_cast<unsigned char>(255.999f * pixel_color.get_g());
    auto ib = static_cast<unsigned char>(255.999f * pixel_color.get_b());
    unsigned char pixel[3]={ir,ig,ib};
    image.write(reinterpret_cast<char*>(pixel),sizeof(pixel));
}