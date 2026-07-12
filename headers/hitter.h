#pragma once
#include "general.h"
#include "aabb.h"
using vec3=Vector3d;

class Material;

struct HitRecord {
    vec3 point;
    vec3 normal;
    shared_ptr<Material>mat;
    float t;
    bool front_face;
    inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
        front_face = dot(r.get_direction(), outward_normal) < 0;
        normal = front_face ? outward_normal : -outward_normal;
    }
};

class Hittable{
    public:
        virtual ~Hittable() = default;
        virtual bool hit(const Ray& r, Interval ray_t, HitRecord& record) const = 0;
        virtual AABB bounding_box() const = 0;
};

class HitList:public Hittable{
    private:
        AABB bbox;
    public:
        HitList(){}
        HitList(shared_ptr<Hittable> object){add(object);}

        std::vector<shared_ptr<Hittable>> objects;
        //actually add the object to the list of hittable objects
        void add(shared_ptr<Hittable> object){
            objects.push_back(object);
            bbox=AABB(bbox,object->bounding_box());
        }
        void clear(){
            objects.clear();
        }
        virtual bool hit(const Ray& r,Interval ray_t, HitRecord& record) const override {
            bool hit_anything = false;
            float closest_so_far = ray_t.max;
            for (const auto& object : objects) {
                HitRecord temp_record;
                if (object->hit(r, Interval(ray_t.min,closest_so_far), temp_record)) {
                    hit_anything = true;
                    closest_so_far = temp_record.t;
                    record = temp_record;
                }
            }
            return hit_anything;
        }
        AABB bounding_box() const override { return bbox; }
};

