#pragma once
#include<iostream>
#include<cmath>
#include<cstdlib>

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
        inline Vector3d operator-()const{
            return Vector3d(-x,-y,-z);
        }
        inline Vector3d &operator+=(const Vector3d &vec){
            x+=vec.x;y+=vec.y;z+=vec.z;
            return *this;
        }
        inline Vector3d &operator*=(const float v){x*=v;y*=v;z*=v;return *this;}
        inline Vector3d &operator/=(const float v){return *this*=1/v;}
        inline float squared_length()const{return x*x+y*y+z*z;}
        inline float length()const{return std::sqrt(x*x+y*y+z*z);}
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