#pragma once
#include "vec3d.h"

class material {

    public:
        material(){}
        material(color3d att_reflect, color3d att_refract, color3d k1, color3d k2, color3d k3, int N, double myu){
            this->att_reflect = att_reflect;
            this->att_refract = att_refract;
            this->ka = k1;
            this->kd = k2;
            this->ks = k3;
            this->N = N;
            this->refractive_index = myu;
        }

        color3d getKa(){
            return this->ka;
        }
        color3d getKd(){
            return this->kd;
        }
        color3d getKs(){
            return this->ks;
        }
        double get_refractive_index(){
            return this->refractive_index;
        }
        int getN(){
            return this->N;
        }
        color3d getAttReflect(){
            return this->att_reflect;
        }
        color3d getAttRefract(){
            return this->att_refract;
        }

    private:
        double refractive_index;
        color3d ka;
        color3d kd;
        color3d ks;
        int N;
        color3d att_reflect;
        color3d att_refract;

};