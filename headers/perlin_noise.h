#pragma once
#include "general.h"
#include "vec3.h"

class perlin{
    private:
        static const int point_count = 256;
        float randfloat[point_count];
        int perm_x[point_count];
        int perm_y[point_count];
        int perm_z[point_count];
        static void perlin_generate_perm(int* p) {
            for (int i = 0; i < point_count; i++)
                p[i] = i;            
            permute(p, point_count);
        }
        static void permute(int* p, int n) {
            for (int i = n-1; i > 0; i--) {
                int target = random_int(0, i);
                int tmp = p[i];
                p[i] = p[target];
                p[target] = tmp;
            }
        }
    public:
        perlin() {
            for (int i = 0; i < point_count; i++) {
                randfloat[i] = random_float();
            }        
            perlin_generate_perm(perm_x);
            perlin_generate_perm(perm_y);
            perlin_generate_perm(perm_z);
        }
        float  noise(const point3& p) const {
            auto i = int(4*p.get_x()) & 255;
            auto j = int(4*p.get_y()) & 255;
            auto k = int(4*p.get_z()) & 255;
            return randfloat[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
        }
};