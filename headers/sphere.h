#pragma once
#include "general.h"

#include "hitter.h"

class Sphere : public Hittable {
    private:
        vec3 center;
        float radius;
        shared_ptr<Material>material;
    public:        
        Sphere(const vec3& cen, float r,shared_ptr<Material> mat) : center(cen), radius(r), material(mat) {}  
        bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
            vec3 oc = r.get_origin() - center;
            float a = r.get_direction().squared_length();
            float b = dot(oc, r.get_direction());
            float c = oc.squared_length() - radius * radius;
            float discriminant = b * b -  a * c;
            if (discriminant < 0)return false;
            float sqrt_discriminant = std::sqrt(discriminant);            
            float root = (-b - sqrt_discriminant) /a;
            if (!ray_t.surrounds(root)) {
                root = (-b + sqrt_discriminant) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }
            record.t = root;
            record.point = r.at(record.t);
            vec3 outward_normal = (record.point - center) / radius;
            record.set_face_normal(r, outward_normal);
            record.mat=material;
            return true;
        } 
};     