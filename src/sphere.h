#pragma once
#include "world_object.h"

class sphere : public world_object {

    public:
        sphere(){}
        sphere(point3d center, double radius, material mat){
            this->center = center;
            this->radius = radius;
            this->mat = mat;
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            vec3d v = pt - this->center;
            double t = v.length();
            return checkEquality(t,this->radius);
        }

        void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index){
            vec3d diff = incident_ray.orig() - this->center;
            double A = 1;                                               //t^2
            double B = 2 * dot(incident_ray.dir(),diff);                //t
            double C = dot(diff,diff) - this->radius * this->radius;    //1
            double D = B*B - 4*A*C;
            if(D < 0){
                return;
            }else{
                double t_low = (-B - sqrt(D))/(2*A);
                double t_high = (-B + sqrt(D))/(2*A);
                if(t_low > lower_limit && t_low < upper_limit){
                    record.t = t_low;
                    record.pt_of_intersection = incident_ray.locate(t_low);
                    record.normal = unit_vec(record.pt_of_intersection - this->center);
                    record.isIntersecting = true;
                    record.index = index;
                }else if(t_high > lower_limit && t_high < upper_limit){
                    record.t = t_high;
                    record.pt_of_intersection = incident_ray.locate(t_high);
                    record.normal = unit_vec(record.pt_of_intersection - this->center);
                    record.isIntersecting = true;
                    record.index = index;
                }else{
                    return;
                }
            }
        }

    private:
        point3d center;
        double radius;

};