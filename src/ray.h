#pragma once
#include "vec3d.h"

class ray {

    public:
        //Ray constructors
        ray() {}
        ray(point3d origin,vec3d direction){
            this->origin = origin;
            this->direction = unit_vec(direction);
            this->ray_color = color3d();
        }

        //Parametric form of ray
        point3d locate(double t){
            return origin + t * direction;
        }

        //Setting ray color
        void setColor(color3d newColor){
            this->ray_color = newColor;
        }

        //Access Methods
        point3d orig() {return this->origin;}
        vec3d dir() {return this->direction;}
        color3d color() {return this->ray_color;}

    private:
        point3d origin;
        vec3d direction;
        color3d ray_color;
        
};