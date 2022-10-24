#pragma once
#include "world_object.h"

class circle : public world_object {

    public:
        pair<vec3d,point3d> p_front, p_back;
        circle(){}
        circle(double x0, double y0, double z0, double r, material mat){
            this->r=r;
            this->x0=x0;
            this->y0=y0;
            this->z0=z0;
            this->normal1 = vec3d(0,1,0);
            this->normal2 = vec3d(0,-1,0);
            this->p_front={normal1,point3d(x0,y0,z0)};
            this->p_back={normal2,point3d(x0,y0,z0)};
            this->mat = mat;
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            if (checkEquality(y0,pt[1])){
                if (((pt[0]-x0)*(pt[0]-x0))+((pt[2]-z0)*(pt[2]-z0))-(r*r)<=0){
                    return 1;
                }else{
                    return 0;
                }
            }else{
                return 0;
            }
        }

        void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index){
            double t_cur,t_in,t_out;
            pair<vec3d,point3d> p_in,p_out;
            bool did_intersect=0;

            pair<vec3d,point3d> p_cur=p_front;
            if (dot(p_cur.first,incident_ray.dir())<0){
                t_cur = dot(p_cur.second-incident_ray.orig(),p_cur.first)/dot(incident_ray.dir(),p_cur.first);
                if (checkPoint((incident_ray.dir()*t_cur)+incident_ray.orig())){
                    t_in=t_cur;
                    p_in=p_cur;
                    did_intersect=1;
                }
            }else if(dot(p_cur.first,incident_ray.dir())>0){
                t_cur = dot(p_cur.second-incident_ray.orig(),p_cur.first)/dot(incident_ray.dir(),p_cur.first);
                if (checkPoint((incident_ray.dir()*t_cur)+incident_ray.orig())){
                    t_out=t_cur;
                    p_out=p_cur;
                    did_intersect=1;
                }
            }

            p_cur=p_back;
            if (dot(p_cur.first,incident_ray.dir())<0){
                t_cur = dot(p_cur.second-incident_ray.orig(),p_cur.first)/dot(incident_ray.dir(),p_cur.first);
                if (checkPoint((incident_ray.dir()*t_cur)+incident_ray.orig())){
                    t_in=t_cur;
                    p_in=p_cur;
                    did_intersect=1;
                }
            }else if(dot(p_cur.first,incident_ray.dir())>0){
                t_cur = dot(p_cur.second-incident_ray.orig(),p_cur.first)/dot(incident_ray.dir(),p_cur.first);
                if (checkPoint((incident_ray.dir()*t_cur)+incident_ray.orig())){
                    t_out=t_cur;
                    p_out=p_cur;
                    did_intersect=1;
                }
            }

            if (did_intersect){
                if(t_in > lower_limit && t_in < upper_limit){
                    record.t = t_in;
                    record.pt_of_intersection = incident_ray.locate(t_in);
                    record.normal = unit_vec(p_in.first);
                    record.isIntersecting = true;
                    record.index = index;
                }else if(t_out > lower_limit && t_out < upper_limit){
                    record.t = t_out;
                    record.pt_of_intersection = incident_ray.locate(t_out);
                    record.normal = unit_vec(p_out.first);
                    record.isIntersecting = true;
                    record.index = index;
                }else{
                    return;
                }
            }else{
                return;
            }
        }

    private:
        double x0,y0,z0,r;
        vec3d normal1, normal2;
        
};