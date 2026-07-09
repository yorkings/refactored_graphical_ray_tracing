#pragma once
#include "general.h"
#include "interval.h"
#include <cmath>

using color = Vector3d;

void write_color(std::ostream &image, const color &pixel_color) {
    // 1. Apply Gamma Correction (sqrt)
    auto r = std::sqrt(pixel_color.get_r());
    auto g = std::sqrt(pixel_color.get_g());
    auto b = std::sqrt(pixel_color.get_b());
    // 2. Clamp values to [0.0, 1.0] to prevent overflow/underflow artifacts
    static const Interval intensity(0.000, 0.999);
    // 3. Scale to 0-255 range
    auto ir = static_cast<unsigned char>(255.999f * intensity.clamp(r));
    auto ig = static_cast<unsigned char>(255.999f * intensity.clamp(g));
    auto ib = static_cast<unsigned char>(255.999f * intensity.clamp(b));
    // 4. Write binary data
    unsigned char pixel[3] = {ir, ig, ib};
    image.write(reinterpret_cast<char*>(pixel), sizeof(pixel));
}   