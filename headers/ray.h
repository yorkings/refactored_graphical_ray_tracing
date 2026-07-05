#include<iostream>
#include "vec3.h"

class Ray{
    public:
        Vector3d origin;
        Vector3d direction;
        Ray(){};
        Ray(const Vector3d &o,const Vector3d &d):origin(o),direction(d){};
        inline Vector3d get_origin() const { return origin; }
        inline Vector3d get_direction() const { return direction; }
        inline Vector3d at(float t) const {
            return origin + t*direction;
        }
};