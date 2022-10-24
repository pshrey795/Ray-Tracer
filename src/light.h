#pragma once
#include "world_object.h"

class light{

    public:
        light(){}
        light(point3d loc,color3d col){
            this->light_loc = loc;
            this->light_color = col;
        }

        point3d getLoc(){
            return this->light_loc;
        }
        color3d getColor(){
            return this->light_color;
        }
    private:
        point3d light_loc;
        color3d light_color;

};