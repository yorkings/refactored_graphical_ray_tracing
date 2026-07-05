#include <iostream>
#include "vec3.h"
#include "ray.h"
#include <fstream>
#include "coloring.h"
class Camera{
    public:
        int image_width = 200;
        void render(){
            initialize();
            
            std::string filename="data/output_"+get_current_time()+".ppm";
            std::ofstream image(filename,std::ios::binary);
            
            if(!image){
                std::cerr<<"Error: Could not open file for writing: "<<filename<<std::endl;
                return;
            }
            image<<"P6\n"<<image_width<<' '<<image_height<<"\n255\n";
            for(int j=image_height-1;j>=0;j--){
                std::cerr<<"\rScanlines remaining: "<<j<<' '<<std::flush;
                for(int i=0;i<image_width;i++){
                    auto pixel_center=pixel00_loc+i*pixel_delta_u+j*pixel_delta_v;
                    auto ray_direction=pixel_center-camera_center;
                    Ray r(camera_center,ray_direction);
                    color pixel_color = ray_color(r);
                    write_color(image,pixel_color);
                }
            }
        }
    private:
        int image_height;
        Vector3d camera_center;//where the camera is located in the world  
        Vector3d pixel00_loc;//location of the center of the pixel at (0,0) in the world 
        Vector3d pixel_delta_u; 
        Vector3d pixel_delta_v;                                
        void initialize(){
            auto aspect_ratio = 16.0f/9.0f;
            image_height = static_cast<int>(image_width / aspect_ratio);
            auto viewport_height = 2.0f;
            auto viewport_width = (image_width / static_cast<float>(image_height)) * viewport_height;
            auto focal_length = 1.0f;
            camera_center = Vector3d(0,0,0);
            auto viewport_u = Vector3d(viewport_width,0,0);
            auto viewport_v = Vector3d(0,viewport_height,0);
            pixel_delta_u = viewport_u / (viewport_width - 1);
            pixel_delta_v = viewport_v / (viewport_height - 1);
            auto View_port_lower_left_corner = camera_center - viewport_u/2 - viewport_v/2 - Vector3d(0,0,focal_length);
            pixel00_loc=View_port_lower_left_corner+0.5*(pixel_delta_u+pixel_delta_v);
        }  
        Vector3d ray_color(const Ray& r) {
            Vector3d unit_direction =unit_vector(r.get_direction());
            float t = 0.5f * (unit_direction.get_y() + 1.0f);
            return (1.0f - t) * Vector3d(1.0f, 1.0f, 1.0f) + t * Vector3d(0.5f, 0.7f, 1.0f);
        }

                     
};  