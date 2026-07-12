#pragma once
#include "general.h"
#include "vec3.h"
#include "ray.h"
#include "hitter.h"
#include "texture.h"

using color=Vector3d;

class Material{
    public:
        virtual ~Material()=default;
        virtual bool scatter( const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered)const{return false;}
};

//lambertian mean difuuse
class Lambertian:public Material{
    private:
         shared_ptr<texture>tex;
    public:
        Lambertian(const color &albedo):tex(make_shared<solid_color>(albedo)){};
        Lambertian(shared_ptr<texture> tex) : tex(tex) {}
        bool scatter(const Ray& r_in, const HitRecord& rec, color& attenuation, Ray& scattered)const override {
            auto scatter_direction = rec.normal + random_unit_vector();
             if (scatter_direction.near_zero()){scatter_direction = rec.normal;}
            scattered = Ray(rec.point, scatter_direction,r_in.get_time());
            attenuation = tex->value(rec.u,rec.v,rec.point);
            return true;
        }        
};

class metal : public Material {
  public:
    metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}
    bool scatter(const Ray& r_in, const HitRecord& rec, color& attenuation, Ray & scattered)
    const override {
        vec3 reflected = reflect(r_in.get_direction(), rec.normal);
        reflected =unit_vector(reflected)+(fuzz*random_unit_vector());
        scattered = Ray(rec.point, reflected,r_in.get_time());
        attenuation = albedo;
        return (dot(scattered.get_direction(), rec.normal) > 0);
    }
  private:
    color albedo;
    float fuzz;
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
            scattered = Ray(rec.point, reflected,r_in.get_time());
            return true;
        }
        vec3 refracted = refract(unit_direction, rec.normal, ri);
        scattered = Ray(rec.point, refracted,r_in.get_time());
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