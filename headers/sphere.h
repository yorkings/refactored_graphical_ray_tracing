#pragma once
#include "general.h"

#include "hitter.h"

class Sphere : public Hittable {
    private:
        vec3 center;
        float radius;
    public:        
        Sphere(const vec3& cen, float r) : center(cen), radius(r) {}  
        bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
            vec3 oc = r.get_origin() - center;
            float a = r.get_direction().squared_length();
            float b = 2.0f * dot(oc, r.get_direction());
            float c = oc.squared_length() - radius * radius;
            float discriminant = b * b - 4 * a * c;
            if (discriminant > 0) {
                float sqrt_discriminant = std::sqrt(discriminant);
                float root = (-b - sqrt_discriminant) / (2.0f * a);
                if (!ray_t.surrounds(root)) {
                    record.point = r.at(root);
                    vec3 outward_normal = (record.point - center) / radius;
                    record.set_face_normal(r, outward_normal);
                    record.t = root;
                    return true;
                }
                root = (-b + sqrt_discriminant) / (2.0f * a);
                if (!ray_t.surrounds(root)) {
                    record.point = r.at(root);
                    vec3 outward_normal = (record.point - center) / radius;
                    record.set_face_normal(r, outward_normal);
                    record.t = root;
                    return true;
                }
            }
            return false;
        } 
};     