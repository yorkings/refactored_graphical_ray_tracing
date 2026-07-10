#include <iostream>
#include "general.h"
#include <fstream>
#include "coloring.h"
#include "hitter.h"
#include "sphere.h"
#include "materials.h"
using vec3 =Vector3d;
using point3=Vector3d;

class Camera{
    public:
        std::string filename;
        int image_width=0;
        int image_height=0;
        int samples_per_pixel=10;
        int max_depth=50;  
        float vfov=90;//vertical view angle  
        point3 lookfrom = point3(0,0,0);   // Point camera is looking from
        point3 lookat   = point3(0,0,-1);  // Point camera is looking at
        vec3   vup      = vec3(0,1,0);     // Camera-relative "up" direction    
        float defocus_angle = 0;  // Variation angle of rays through each pixel
        float focus_dist = 10;    // Distance from camera lookfrom point to plane of perfect focus
   
        void render(HitList world){
            initialize();            
            std::ofstream image(filename,std::ios::binary);            
            if(!image){
                std::cerr<<"Error: Could not open file for writing: "<<filename<<std::endl;
                return;
            }
            image<<"P6\n"<<image_width<<' '<<image_height<<"\n255\n";
            std::vector<color> image_buffer(image_width * image_height);
            #pragma omp parallel for schedule(static)
            for(int j=image_height-1;j>=0;j--){
                std::cerr<<"\rScanlines remaining: "<<j<<' '<<std::flush;
                for(int i=0;i<image_width;i++){
                    color pixel_color(0,0,0);
                    for(int s=0;s<samples_per_pixel;s++){
                        auto ray=get_ray(i,j);
                        pixel_color+=ray_color(ray,world,max_depth);
                    }
                    pixel_color/=static_cast<float>(samples_per_pixel);//average the color over the samples
                    int buffer_row = image_height - 1 - j;
                    image_buffer[buffer_row * image_width + i] = pixel_color;
                }
            }
            for(int j=image_height-1;j>=0;j--){
                for(int i=0;i<image_width;i++){
                    write_color(image,image_buffer[j * image_width + i]);
                }
            }
            std::cerr<<"\nDone.\n";

        }
    private:
        
        vec3 camera_center;//where the camera is located in the world  
        vec3 pixel00_loc;//location of the center of the pixel at (0,0) in the world 
        vec3 pixel_delta_u; 
        vec3 pixel_delta_v;   
        float pixel_sample_scale;            
        vec3   u, v, w;              // Camera frame basis vectors
        vec3   defocus_disk_u;       // Defocus disk horizontal radius
        vec3   defocus_disk_v;       // Defocus disk vertical radius

    
        void initialize(){
            auto aspect_ratio = 16.0f/9.0f;
            pixel_sample_scale=1.0/samples_per_pixel;
            if (image_width > 0 && image_height <= 0) {
                image_height = static_cast<int>(image_width / aspect_ratio);
            }else if (image_height > 0 && image_width <= 0) {
                image_width = static_cast<int>(image_height * aspect_ratio);
            } else if (image_width <= 0 && image_height <= 0) {
                image_width = 400; // Default width
                image_height = static_cast<int>(image_width / aspect_ratio); // Calculate height based on aspect ratio
            }

            camera_center = lookfrom;
            auto theta = degrees_to_radians(vfov);
            auto h = std::tan(theta/2);
            auto viewport_height = 2 * h * focus_dist;
            auto viewport_width = (image_width / static_cast<float>(image_height)) * viewport_height;
            // Calculate the u,v,w unit basis vectors for the camera coordinate frame.
            w = unit_vector(lookfrom - lookat);
            u = unit_vector(cross(vup, w));
            v = cross(w, u);                        
            auto viewport_u = viewport_width*u;
            auto viewport_v = viewport_height* -v;
            pixel_delta_u = viewport_u / image_width;
            pixel_delta_v = viewport_v / image_height;
            auto View_port_lower_left_corner = camera_center-(focus_dist*w) - viewport_u/2 - viewport_v/2 ;
            pixel00_loc=View_port_lower_left_corner+0.5*(pixel_delta_u+pixel_delta_v);
            // Calculate the camera defocus disk basis vectors.
            auto defocus_radius = focus_dist * std::tan(degrees_to_radians(defocus_angle / 2));
            defocus_disk_u = u * defocus_radius;
            defocus_disk_v = v * defocus_radius;

        }  
        Ray get_ray(int i,int j){
            auto offset=sample_square();
            auto pixel_sample=pixel00_loc+((i + offset.get_x()) * pixel_delta_u)+((j + offset.get_y()) * pixel_delta_v);
            auto ray_origin=(defocus_angle <= 0) ? camera_center : defocus_disk_sample();
            auto ray_direction=pixel_sample- ray_origin;
            return Ray(ray_origin,ray_direction);
        }
        color ray_color(const Ray& r, Hittable& world,int depth){ 
            HitRecord record;

            if(depth<=0)return color(0,0,0);
            if(world.hit(r,Interval(0,infinity), record)) {
                Ray scattered;
                color attenuation;
                if(record.mat->scatter(r,record,attenuation,scattered)){
                    return attenuation*ray_color(scattered,world,depth-1);
                }
                return color(0,0,0);
            }
            vec3 unit_direction =unit_vector(r.get_direction());
            float t = 0.5f * (unit_direction.get_y() + 1.0f);
            return (1.0f - t) * color(1.0f, 1.0f, 1.0f) + t * color(0.5f, 0.7f, 1.0f);
        }

        vec3 sample_square(){
            return vec3(random_float() - 0.5,random_float() - 0.5,0);
        }
        point3 defocus_disk_sample() const {
        // Returns a random point in the camera defocus disk.
        auto p = random_in_unit_disk();
        return camera_center + (p.get_x() * defocus_disk_u) + (p.get_y() * defocus_disk_v);
        }

                     
};  