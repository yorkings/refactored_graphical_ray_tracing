#include <iostream>
#include<iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/bvh.h"
#include "headers/camera.h"
#include "headers/hitter.h"
#include "headers/sphere.h"
#include "headers/materials.h"
#include "headers/texture.h"


using point3=Vector3d;

std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y_%m_%d_%H_%M_%S");
    return oss.str();
}


void bouncing_spheres(const std::string &filename) {
    HitList world;
    auto ground_texture = make_shared<checker_texture>(0.22, color(.9, .3, .1), color(.9, .9, .9));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambertian>(ground_texture)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_float();
            point3 center(a + 0.9*random_float(), 0.2, b + 0.9*random_float());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> Sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = random_color() * random_color();
                    Sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + vec3(0, random_float(0,.5), 0);
                    world.add(make_shared<Sphere>(center, center2, 0.2, Sphere_material));

                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = random_color();
                    auto fuzz = random_float(0, 0.5);
                    Sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                } else {
                    // glass
                    Sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, Sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(4, 1, 0), 1.0, material3));
    world = HitList(make_shared<BVHNode>(world));

    Camera cam;
    cam.filename=filename;
    cam.image_width = 400; // Set the desired image width
    cam.image_height =0 ; // Set the desired image height
    cam.max_depth = 50; // Set the maximum depth for ray tracing
    cam.samples_per_pixel=100;//
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);
    
    cam.defocus_angle = 0.6;// Set the defocus angle for depth of field
    cam.focus_dist    = 10;// Set the focus distance for depth of field
    cam.render(world); // Pass the world to the render function
}
void checkered_spheres(const std::string &filename) {
    HitList world;
    auto checker = make_shared<checker_texture>(0.32, color(.8, .2, .1), color(.9, .9, .9));

    world.add(make_shared<Sphere>(point3(0,-10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));

    Camera cam;
    cam.filename=filename;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;
    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}
void perlin_spheres(const std::string &filename) {
    HitList world;
    auto pertext = make_shared<noise_texture>();
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, make_shared<Lambertian>(pertext)));
    world.add(make_shared<Sphere>(point3(0,2,0), 2, make_shared<Lambertian>(pertext)));

    Camera cam;
    cam.filename=filename;
    cam.image_width       = 400;
    cam.samples_per_pixel = 100;
    cam.max_depth         = 50;

    cam.vfov     = 20;
    cam.lookfrom = point3(13,2,3);
    cam.lookat   = point3(0,0,0);
    cam.vup      = vec3(0,1,0);

    cam.defocus_angle = 0;

    cam.render(world);
}

int main(){
    std::string user_filename;   
    std::cout << "Enter save file name (without extension): ";
    if (!(std::cin >> user_filename)) {
        std::cerr << "Error reading input. Using default filename." << std::endl;
        user_filename = "render";
    }
    std::string filename = "data/" + user_filename + "_" + get_current_time() + ".ppm";
    switch (3) {
        case 1: bouncing_spheres(filename);  break;
        case 2: checkered_spheres(filename); break;
        case 3:perlin_spheres(filename);break;
    }
    return 0;
}