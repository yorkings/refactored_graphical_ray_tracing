#pragma once
#include "hitter.h"
//q=starting coner,v=side 1 and u=side2

class Quad:public Hittable{
    private:
        point3 Q;
        vec3 u, v,w;
        vec3 normal;
        float D;
        shared_ptr<Material> mat;
        AABB bbox;
    public:
        Quad(const point3 &q,const vec3 &u, const vec3&v,shared_ptr<Material> mat):Q(q), u(u), v(v), mat(mat){
            auto n=cross(u,v);
            normal=unit_vector(n);
            D=dot(normal,Q);
            w = n / dot(n,n);
            set_bounding_box();
        }
        virtual void set_bounding_box() {
            auto bbox_diagonal1 = AABB(Q, Q + u + v);
            auto bbox_diagonal2 = AABB(Q + u, Q + v);
            bbox = AABB(bbox_diagonal1, bbox_diagonal2);
        }
        bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
            auto denom = dot(normal, r.get_direction());
            // No hit if the ray is parallel to the plane.
            if (std::fabs(denom) < 1e-8) return false;
            // Return false if the hit point parameter t is outside the ray interval.
            auto t = (D - dot(normal, r.get_origin())) / denom;
            if (!ray_t.contains(t)) 
                return false;
            // Determine if the hit point lies within the planar shape using its plane coordinates.
            auto intersection = r.at(t);
            vec3 planar_hitpt_vector = intersection - Q;
            auto alpha = dot(w, cross(planar_hitpt_vector, v));
            auto beta = dot(w, cross(u, planar_hitpt_vector));
            if (!is_interior(alpha, beta, rec))
                return false;
            rec.t = t;
            rec.point = intersection;
            rec.mat = mat;
            rec.set_face_normal(r, normal);
            return true;
        }
        
        virtual bool is_interior(float a, float b, HitRecord& rec) const {
            Interval unit_interval = Interval(0, 1);
            // Given the hit point in plane coordinates, return false if it is outside the primitive, otherwise set the hit record UV coordinates and return true.
            if (!unit_interval.contains(a) || !unit_interval.contains(b))
                return false;

            rec.u = a;
            rec.v = b;
            return true;
        }
        AABB bounding_box()const override{return bbox;}

};