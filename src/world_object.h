#pragma once
#include<bits/stdc++.h>
#include "vec3d.h"
#include "ray.h"
#include "material.h"
#define INF DBL_MAX

struct hit_record{
    point3d pt_of_intersection;
    vec3d normal;
    bool isIntersecting;
    double t;
    int index;

    hit_record(){
        isIntersecting = false;
        t = INF;
        index = -1;
    }

};

class world_object {
    protected:
        bool checkEquality(double v1,double v2){
            return (abs(v1-v2) <= threshold);
        }
        material mat;

    public:

        virtual void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index) = 0;

        material getMat(){
            return this->mat;
        }

    private:
        const double threshold = 1e-3; 
        

};