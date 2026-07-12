#pragma once
#include <iostream>
#include <iomanip>
#include <vector>
#include <memory>
#include <limits>
#include <cmath>    
#include<atomic>
#include <random>   
#include <omp.h>   
#include <chrono> 
#include <algorithm>
using std::shared_ptr;
using std::make_shared;
const float infinity = std::numeric_limits<float>::infinity();

inline float random_float() {
    thread_local std::mt19937 gen(std::random_device{}());
    thread_local std::uniform_real_distribution<float> dis(0.0f, 1.0f);
    return dis(gen);
}

inline  float random_float(float min, float max) {
    return min + (max - min) * random_float();
}

inline float degrees_to_radians(float degrees) {
    return degrees * M_PI / 180.0f;
}
inline int random_int(int min,int max){
    return int(random_float(min,max+1));
}