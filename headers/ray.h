#pragma once
#include<iostream>
#include "general.h"
using vec3=Vector3d;
class Ray{
    private:
        vec3 origin;
        vec3 direction;
        float time;
    public:       
        Ray() {};
        Ray(const vec3 &o,const vec3 &d,float tm):origin(o),direction(d),time(tm){}
        Ray(const vec3 &o,const vec3 &d):Ray(o,d,0){};
        inline vec3 get_origin() const { return origin; }
        inline vec3 get_direction() const { return direction; }
        inline float get_time()const{return time;}
        inline vec3 at(float t) const {
            return origin + t*direction;
        }
};