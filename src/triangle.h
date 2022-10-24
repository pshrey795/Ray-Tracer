#pragma once
#include "world_object.h"

class triangle : public world_object {

    public:
        pair<vec3d,point3d> p_front, p_back;
        triangle(){}
        triangle(point3d v1,point3d v2,point3d v3, material mat){
            this->v1=v1;
            this->v2=v2;
            this->v3=v3;
            this->normal1 = unit_vec(cross(v2-v1,v3-v1));
            this->normal2 = unit_vec(cross(v3-v1,v2-v1));
            this->p_front={normal1,v1};
            this->p_back={normal2,v1};
            this->mat = mat;
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            return checkEquality(area(v1,v2,v3), area(pt,v2,v3)+ area(v1,pt,v3)+ area(v1,v2,pt));
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
        point3d v1,v2,v3;
        vec3d normal1, normal2;
        
};