//Axis-Aligned Bounding Boxes (AABBs)
#pragma once
#include "vec3.h"
#include "ray.h"
#include "interval.h"


class AABB{
    public:
        Interval x,y,z;        
        AABB(){};
        AABB(const Interval &x,const Interval &y,const Interval &z):x(x),y(y),z(z){};
        AABB(const AABB& box0, const AABB& box1) {
            x = Interval(box0.x, box1.x);
            y = Interval(box0.y, box1.y);
            z = Interval(box0.z, box1.z);
        }
        AABB(const point3 &a,const point3 &b ){
            // Treat the two points a and b as extrema for the bounding box, so we don't require a particular minimum/maximum coordinate order.
            x=(a.get_x()<b.get_x())?Interval(a.get_x(),b.get_x()):Interval(b.get_x(),a.get_x());
            y=(a.get_y()<b.get_y())?Interval(a.get_y(),b.get_y()):Interval(b.get_y(),a.get_y());
            z=(a.get_z()<b.get_z())?Interval(a.get_z(),b.get_z()):Interval(b.get_z(),a.get_z());
        }
        const Interval &axis_Interval(int n)const{
            if(n==1)return y;
            if(n==2)return z;
            return x;
        }
        bool hit(const Ray& r, Interval ray_t) const {
            const point3 &ray_origin=r.get_origin();
            const vec3 &ray_dir=r.get_direction();
            for(int axis=0;axis<3;axis++){
                const Interval &ax=axis_Interval(axis);
                const double adinv = 1.0 / ray_dir[axis];
                auto t0 = (ax.min - ray_origin[axis]) * adinv;
                auto t1 = (ax.max - ray_origin[axis]) * adinv;
                if (t0 < t1) {
                    if (t0 > ray_t.min) ray_t.min = t0;
                    if (t1 < ray_t.max) ray_t.max = t1;
                } else {
                    if (t1 > ray_t.min) ray_t.min = t1;
                    if (t0 < ray_t.max) ray_t.max = t0;
                }
                if(ray_t.max<=ray_t.min)return false;                
            }
            return true;
        }
        int longest_axis() const {
            // Returns the index of the longest axis of the bounding box.
            if (x.interval_length() > y.interval_length())
                return x.interval_length() > z.interval_length() ? 0 : 2;
            else
                return y.interval_length() > z.interval_length() ? 1 : 2;
        }
        static const AABB empty, universe;
};
const AABB AABB::empty    = AABB(Interval::empty,    Interval::empty,    Interval::empty);
const AABB AABB::universe = AABB(Interval::universe, Interval::universe, Interval::universe);