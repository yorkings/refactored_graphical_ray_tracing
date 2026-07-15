#pragma once
#include<iostream>

#include "general.h"

std::string get_current_time();
class Vector3d{
    public:
        float x,y,z;
        Vector3d(){};
        Vector3d(float e0,float e1,float e2):x(e0),y(e1),z(e2){};
        // i decided that  we should represent x,y,z and r,g,b with same floats respectively cause they do same thing in screen and represent at same time 
        inline float get_x() const { return x; }
        inline float get_y() const { return y; }
        inline float get_z() const { return z; }
        inline float get_r() const { return x; }
        inline float get_g() const { return y; }
        inline float get_b() const { return z; }
        //operations
        inline Vector3d operator-()const{return Vector3d(-x,-y,-z);}
        float operator[](int i) const { return (i==0)?x:((i==1)?y:z); } 
        float& operator[](int i) { return (i==0)?x:((i==1)?y:z); }        
        inline Vector3d &operator+=(const Vector3d &vec){
            x+=vec.x;y+=vec.y;z+=vec.z;
            return *this;
        }
        inline Vector3d &operator*=(const float v){x*=v;y*=v;z*=v;return *this;}
        inline Vector3d &operator/=(const float v){return *this*=1/v;}
        inline float squared_length()const{return x*x+y*y+z*z;}
        inline float length()const{return std::sqrt(x*x+y*y+z*z);}
        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
        }

}; 

inline Vector3d operator+(const Vector3d &v1, const Vector3d &v2) {
    return Vector3d(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
}
inline Vector3d operator-(const Vector3d &v1, const Vector3d &v2) {
    return Vector3d(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
}
inline Vector3d operator*(const Vector3d &v1, const Vector3d &v2) {
    return Vector3d(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
}
inline Vector3d operator*(float t, const Vector3d &v) {
    return Vector3d(t * v.x, t * v.y, t * v.z);
}
inline Vector3d operator*(const Vector3d &v, float t) {
    return Vector3d(t * v.x, t * v.y, t * v.z);
}
inline Vector3d operator/(const Vector3d &v, float t) { 
    return Vector3d(v.x / t, v.y / t, v.z / t);
}
inline float dot(const Vector3d &v1, const Vector3d &v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
inline Vector3d cross(const Vector3d &v1, const Vector3d &v2) {
    return Vector3d((v1.y * v2.z - v1.z * v2.y), (-(v1.x * v2.z - v1.z * v2.x)), (v1.x * v2.y - v1.y * v2.x));
}

inline std::istream &operator>>(std::istream &is,Vector3d &t){
    is>>t.x>>t.y>>t.z;
    return is;
}

inline std::ostream &operator<<(std::ostream &os,const Vector3d &t){
    os<<t.x<<" "<<t.y<<" "<<t.z;
    return os;
}
inline Vector3d unit_vector(Vector3d v){
    float len=v.length();
    return len>0 ? (v/len) : Vector3d(0,0,0);
}
//random vector generation

inline Vector3d random_vector(){
    return Vector3d(random_float(),random_float(),random_float());
}

inline Vector3d random_vector(float min,float max){
    return Vector3d(random_float(min,max),random_float(min,max),random_float(min,max));
}

inline Vector3d random_unit_vector(){
    while(true){
        auto p=random_vector(-1,1);
        auto len_squared=p.squared_length();
        if(1e-8<len_squared && len_squared<1)return unit_vector(p);
    }
}
inline Vector3d random_in_unit_disk() {
    while (true) {
        auto p = Vector3d(random_float(-1,1), random_float(-1,1), 0);
        if (p.squared_length() < 1)
            return p;
    }
}


inline Vector3d random_on_hemisphere(const Vector3d& normal) {
    Vector3d in_unit_sphere = random_unit_vector();
    if (dot(in_unit_sphere, normal) > 0.0f) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

//reflection and refraction
inline Vector3d reflect(const Vector3d& v, const Vector3d& n) {
    return v - 2 * dot(v, n) * n;
}
inline Vector3d refract(const Vector3d& uv, const Vector3d& n, float etai_over_etat) {
    float cos_theta = fmin(dot(-uv, n), 1.0f);
    Vector3d r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3d r_out_parallel = -sqrt(fabs(1.0f - r_out_perp.squared_length())) * n;
    return r_out_perp + r_out_parallel;
}

//
using color = Vector3d;
using vec3 =Vector3d;
using point3=Vector3d;

inline color random_color() {
    return color(random_float(), random_float(), random_float());
}

inline color random_color(float min, float max) {
    return color(random_float(min, max), random_float(min, max), random_float(min, max));
}