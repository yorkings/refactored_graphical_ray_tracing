#pragma once
#include<iostream>
#include "general.h"
using vec3=Vector3d;
class Ray{
    private:
        vec3 origin;
        vec3 direction;
    public:       
        Ray() {};
        Ray(const vec3 &o,const vec3 &d):origin(o),direction(d){};
        inline vec3 get_origin() const { return origin; }
        inline vec3 get_direction() const { return direction; }
        inline vec3 at(float t) const {
            return origin + t*direction;
        }
};