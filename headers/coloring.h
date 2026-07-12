#pragma once
#include "general.h"
#include "interval.h"
#include <cmath>



void write_color(std::ostream &image, const color &pixel_color) {

    auto r = pixel_color.get_r();
    auto g = pixel_color.get_g();
    auto b = pixel_color.get_b();
    // 1. Clamp values to [0.0, 1.0] to prevent overflow/underflow artifacts
    static const Interval intensity(0.000, 0.999);
    r=intensity.clamp(r);
    g=intensity.clamp(g);
    b=intensity.clamp(b);
    //gamma correction
    r = std::sqrt(r);
    g = std::sqrt(g);
    b = std::sqrt(b);

    // 3. Scale to 0-255 range
    auto ir = static_cast<unsigned char>(255.999f *r );
    auto ig = static_cast<unsigned char>(255.999f * g);
    auto ib = static_cast<unsigned char>(255.999f * b);
    // 4. Write binary data
    unsigned char pixel[3] = {ir, ig, ib};
    image.write(reinterpret_cast<char*>(pixel), sizeof(pixel));
}   