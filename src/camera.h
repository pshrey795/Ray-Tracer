#pragma once
#include "vec3d.h"
#include "ray.h"

class camera { 

    public:
        camera() {} 
        camera(point3d origin, vec3d view_dir, vec3d upward_dir, int screen_width, int screen_height, int screen_depth){
            this->eye = origin;
            this->view_direction = unit_vec(view_dir);
            this->vertical = unit_vec(upward_dir);
            this->horizontal = unit_vec(cross(view_direction,vertical));
            this->view_origin = origin + screen_depth * this->view_direction - vertical * (screen_height/2) - horizontal * (screen_width/2);
        }

        //Generating the rays for each pixel from the camera using ray casting
        ray generate_ray(double s,double t){
            return ray(eye,view_origin + s*horizontal + t*vertical - eye);
        }

    private:
        point3d eye;
        vec3d view_direction,horizontal,vertical;
        point3d view_origin;

};