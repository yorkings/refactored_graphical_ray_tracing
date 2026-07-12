#pragma once
#include "general.h"

#include "hitter.h"

class Sphere : public Hittable {
    private:
        Ray center;
        float radius;
        shared_ptr<Material>material;
    public:  
        //stationary sphere      
        Sphere(const vec3& static_cen, float r,shared_ptr<Material> mat) : center(static_cen,point3(0,0,0)), radius(std::fmax(0,r)), material(mat) {}  
        //moving sphere
        Sphere(const vec3& cen1,const vec3 &cen2, float r,shared_ptr<Material> mat) : center(cen1,(cen2-cen1)), radius(std::fmax(0,r)), material(mat) {} 
        bool hit(const Ray& r, Interval ray_t, HitRecord& record) const override {
            point3 current_center=center.at(r.get_time());
            vec3 oc = r.get_origin()- current_center;
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
            vec3 outward_normal = (record.point - current_center) / radius;
            record.set_face_normal(r, outward_normal);
            record.mat=material;
            return true;
        } 
};     