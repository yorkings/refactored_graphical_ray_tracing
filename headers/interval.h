#pragma once
#include "general.h"
class Interval{
    public:
        float min,max;
        Interval():min(+infinity),max(-infinity){};
        Interval(float a,float b):min(a),max(b){};
        Interval(const Interval& a, const Interval& b) {
            // Create the interval tightly enclosing the two input intervals.
            min = a.min <= b.min ? a.min : b.min;
            max = a.max >= b.max ? a.max : b.max;
        }
        float interval_length()const{return max-min;}
        bool contains(float x)const{return x>=min && x<=max;}
        bool surrounds(float x)const{return x>min && x<max;}
        float clamp(float x)const{
            if(x<min)return min;
            if(x>max)return max;
            return x;
        }
        Interval expand(float delta)const{
            auto padding=delta/2;
            return Interval(min-padding,max+padding);
        }
        static const Interval empty,universe;        
};
const Interval Interval::empty(+infinity,-infinity);
const Interval Interval::universe(-infinity,+infinity);
