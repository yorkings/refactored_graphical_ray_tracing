#pragma once
#include "general.h"
class Interval{
    public:
        float min,max;
        Interval():min(+infinity),max(-infinity){};
        Interval(float a,float b):min(a),max(b){};
        float interval_length()const{return max-min;}
        bool contains(float x)const{return x>=min && x<=max;}
        bool surrounds(float x)const{return x<min || x>max;}
        float clamp(float x)const{
            if(x<min)return min;
            if(x>max)return max;
        }
        static const Interval empty,universe;        
};
const Interval Interval::empty(+infinity,-infinity);
const Interval Interval::universe(-infinity,+infinity);
