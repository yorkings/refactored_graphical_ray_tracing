#pragma once
#include "general.h"
#include "vec3.h"
#include "perlin_noise.h"
#include "rtw_stb_image.h"

class texture{
    public:
        virtual ~texture() = default;
        virtual color value(float u, float v, const point3& p) const = 0;
};

class solid_color : public texture {
    public:
      solid_color(const color& albedo) : albedo(albedo) {}
      solid_color(float red, float green, float blue) : solid_color(color(red,green,blue)) {}
      color value(float u, float v, const point3& p) const override {
          return albedo;
      }
    private:
      color albedo;
};

class checker_texture : public texture {
  public:
    checker_texture(float scale, shared_ptr<texture> even, shared_ptr<texture> odd):inv_scale(1.0 / scale), even(even), odd(odd) {}
    checker_texture(float scale, const color& c1, const color& c2):checker_texture(scale, make_shared<solid_color>(c1), make_shared<solid_color>(c2)) {}
    color value(float u, float v, const point3& p) const override {
        auto xInteger = int(std::floor(inv_scale * p.get_x()));
        auto yInteger = int(std::floor(inv_scale * p.get_y()));
        auto zInteger = int(std::floor(inv_scale * p.get_z()));
        bool isEven = (xInteger + yInteger + zInteger) % 2 == 0;
        return isEven ? even->value(u, v, p) : odd->value(u, v, p);
    }

  private:
    float inv_scale;
    shared_ptr<texture> even;
    shared_ptr<texture> odd;
};

class noise_texture:public texture{
  private:
    perlin noise;
  public:
    noise_texture(){}
    color value(float u, float v, const point3& p) const override {
      return color(1,1,1) * noise.noise(p);
    }

};

class image_texture : public texture {
  private:
    rtw_image image;
  public:
    image_texture(const char* filename) : image(filename) {}
    color value(float u, float v, const point3& p) const override {
      // If no image data, return cyan (debug color)
      if (image.height() <= 0) return color(0, 1, 1);
      // Clamp input texture coordinates to [0,1]
      u = Interval(0, 1).clamp(u);
      v = 1.0 - Interval(0, 1).clamp(v);  // Flip V to match image coordinates
      // Convert UV to pixel coordinates
      auto i = static_cast<int>(u * image.width());
      auto j = static_cast<int>(v * image.height());
      // Get pixel data (returns pointer to RGB bytes)
      auto pixel = image.pixel_data(i, j);
      // Scale RGB bytes [0, 255] to color [0.0, 1.0]
      auto color_scale = 1.0 / 255.0;
      return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
    }
};