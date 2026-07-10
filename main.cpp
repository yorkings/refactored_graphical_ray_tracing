#include <iostream>
#include<iomanip>
#include <chrono>
#include <ctime>
#include <sstream>

#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/camera.h"
#include "headers/hitter.h"
#include "headers/sphere.h"
#include "headers/materials.h"

using point3=Vector3d;

std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y_%m_%d_%H_%M_%S");
    return oss.str();
}


int main() {
    HitList world;
    std::string user_filename;   
    std::cout << "Enter save file name (without extension): ";
    if (!(std::cin >> user_filename)) {
        std::cerr << "Error reading input. Using default filename." << std::endl;
        user_filename = "render";
    }
    std::string filename = "data/" + user_filename + "_" + get_current_time() + ".ppm";

    auto material_ground = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   =make_shared<dielectric>(1.00/1.33);//air to water
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2));
    auto material_bubble = make_shared<dielectric>(1.00 / 1.50);


   
    world.add(make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0,    0.0, -1.0),   0.4, material_bubble));
    world.add(make_shared<Sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));


    Camera cam;
    cam.filename=filename;
    cam.image_width = 0; // Set the desired image width
    cam.image_height = 720; // Set the desired image height
    cam.focal_length = 0.4f; // Set the desired focal length
    cam.max_depth = 50; // Set the maximum depth for ray tracing
    cam.samples_per_pixel=200;
    cam.vfov     = 90;
    cam.lookfrom = point3(-2,2,1);
    cam.lookat   = point3(0,0,-1);
    cam.vup      = vec3(0,1,0);
    cam.render(world); // Pass the world to the render function
    return 0;
}
