#include <iostream>
#include<iomanip>
#include <chrono>
#include <ctime>
#include <sstream>
#include "headers/vec3.h"
#include "headers/ray.h"
#include "headers/camera.h"



std::string get_current_time() {
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::tm* local_time = std::localtime(&now_time);
    std::ostringstream oss;
    oss << std::put_time(local_time, "%Y_%m_%d_%H_%M_%S");
    return oss.str();
}


int main() {
    Camera cam;
    cam.image_width = 0; // Set the desired image width
    cam.image_height = 720; // Set the desired image height
    cam.focal_length = 0.4f; // Set the desired focal length
    cam.render();
    return 0;
}
