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
        static float trilinear_interp(float c[2][2][2], float u, float v, float w) {
            auto accum = 0.0;
            for (int i=0; i < 2; i++)
                for (int j=0; j < 2; j++)
                    for (int k=0; k < 2; k++)
                        accum += (i*u + (1-i)*(1-u)) * (j*v + (1-j)*(1-v)) * (k*w + (1-k)*(1-w)) * c[i][j][k];
            return accum;
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
            auto u=p.get_x() - std::floor(p.get_x()); 
            auto v=p.get_y() - std::floor(p.get_y()) ;
            auto w=p.get_z() - std::floor(p.get_z()) ;
            //hermatian smoothing
            u = u*u*(3-2*u);
            v = v*v*(3-2*v);
            w = w*w*(3-2*w);

            auto i = static_cast<int>(std::floor(p.get_x())) & 255;
            auto j = static_cast<int>(std::floor(p.get_y())) & 255;
            auto k = static_cast<int>(std::floor(p.get_z())) & 255;
            float c[2][2][2];            
            
            for (int di=0; di < 2; di++)
                for (int dj=0; dj < 2; dj++)
                    for (int dk=0; dk < 2; dk++)
                        c[di][dj][dk] = randfloat[perm_x[(i+di) & 255] ^ perm_y[(j+dj) & 255] ^ perm_z[(k+dk) & 255]];

            return trilinear_interp(c,u,v,w);
        }
};