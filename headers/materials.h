#pragma once
#include "general.h"
#include "vec3.h"
#include "ray.h"
#include "hitter.h"
using color=Vector3d;

class Material{
    public:
        virtual ~Material()=default;
        virtual bool scatter( const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered)const{return false;}
};

//lambertian mean difuuse
class Lambertian:public Material{
    private:
         color albedo;
    public:
        Lambertian(const color &a):albedo(a){};
        bool scatter(const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered)const override {
            auto scatter_direction = rec.normal + random_unit_vector();
             if (scatter_direction.near_zero()){scatter_direction = rec.normal;}
            scattered = Ray(rec.point, scatter_direction);
            attenuation = albedo;
            return true;
        }        
};

class metal : public Material {
  public:
    metal(const color& albedo) : albedo(albedo) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, color& attenuation, Ray & scattered)
    const override {
        vec3 reflected = reflect(r_in.get_direction(), rec.normal);
        scattered = Ray(rec.point, reflected);
        attenuation = albedo;
        return true;
    }
  private:
    color albedo;
};

class dielectric : public Material {
  public:
    dielectric(float refraction_index) : refraction_index(refraction_index) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered)const override {
        attenuation = color(1.0, 1.0, 1.0);
        float ri = rec.front_face ? (1.0/refraction_index) : refraction_index;
        vec3 unit_direction = unit_vector(r_in.get_direction());
        float cos_theta=dot(-unit_direction, rec.normal);
        float sin_theta = std::sqrt(1.0f - cos_theta * cos_theta);
        float unrefractable=ri * sin_theta > 1.0f;
        if (unrefractable || reflectance(cos_theta, ri) > random_float()) {
            vec3 reflected = reflect(unit_direction, rec.normal);
            scattered = Ray(rec.point, reflected);
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, ri);
        scattered = Ray(rec.point, refracted);
        return true;

    }

  private:
    // Refractive index in vacuum or air, or the ratio of the material's refractive index over the refractive index of the enclosing media
    float refraction_index;
    static float reflectance(float cosine, float refraction_index) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - refraction_index) / (1 + refraction_index);
        r0 = r0*r0;
        return r0 + (1-r0)*std::pow((1 - cosine),5);
    }
};