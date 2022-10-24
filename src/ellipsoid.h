#pragma once
#include "world_object.h"

class ellipsoid : public world_object {

    public:
        ellipsoid(){}
        ellipsoid(point3d center, double a, double b, double c, material mat){
            this->center = center;
            this->a = a;
            this->b = b;
            this->c = c;
            this->mat = mat;
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            double t = (((pt[0]-center[0])*(pt[0]-center[0]))/(a*a))+(((pt[1]-center[1])*(pt[1]-center[1]))/(b*b))+(((pt[2]-center[2])*(pt[2]-center[2]))/(c*c));
            return checkEquality(1,t);
        }

        void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index){
            vec3d temp(b*b*c*c,a*a*c*c,a*a*b*b);
            vec3d temp2(a*a,b*b,c*c);
            double A = dot(temp,incident_ray.dir()*incident_ray.dir());                                            //t^2
            double B = 2 * dot(temp,incident_ray.dir()*(incident_ray.orig()-center));                              //t
            double C = dot(temp,(incident_ray.orig()-center)*(incident_ray.orig()-center)) - (a*a*b*b*c*c);        //1
            double D = B*B - 4*A*C;
            if(D < 0){
                return;
            }else{
                double t_low = (-B - sqrt(D))/(2*A);
                double t_high = (-B + sqrt(D))/(2*A);
                if(t_low > lower_limit && t_low < upper_limit){
                    record.t = t_low;
                    record.pt_of_intersection = incident_ray.locate(t_low);
                    record.normal = unit_vec(2*((incident_ray.orig()-center+t_low*incident_ray.dir())/temp2));
                    record.isIntersecting = true;
                    record.index = index;
                }else if(t_high > lower_limit && t_high < upper_limit){
                    record.t = t_high;
                    record.pt_of_intersection = incident_ray.locate(t_high);
                    record.normal = unit_vec(2*((incident_ray.orig()-center+t_high*incident_ray.dir())/temp2));
                    record.isIntersecting = true;
                    record.index = index;
                }else{
                    return;
                }
            }
        }

    private:
        point3d center;
        double a;
        double b;
        double c;
};