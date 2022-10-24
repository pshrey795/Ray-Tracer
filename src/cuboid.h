#pragma once
#include "world_object.h"

class cuboid : public world_object {

    public:

        cuboid(){}
        cuboid(point3d base_corner, point3d corner1, point3d corner2, point3d corner3, material mat){
            this->base_corner = base_corner;
            this->corner1 = corner1;
            this->corner2 = corner2;
            this->corner3 = corner3;
            this->mat = mat;
            this-> e1 =(corner1-base_corner).length();
            this-> e2 =(corner2-base_corner).length();
            this-> e3 =(corner3-base_corner).length();
            this->p1={unit_vec(cross(corner1-base_corner,corner2-base_corner)),base_corner};
            this->p2={unit_vec(cross(corner2-base_corner,corner3-base_corner)),base_corner};
            this->p3={unit_vec(cross(corner3-base_corner,corner1-base_corner)),base_corner};
            this->p4={unit_vec(cross(corner2-base_corner,corner1-base_corner)),corner3};
            this->p5={unit_vec(cross(corner3-base_corner,corner2-base_corner)),corner1};
            this->p6={unit_vec(cross(corner1-base_corner,corner3-base_corner)),corner2};
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            double dis_p1 = abs(dot(pt-p1.second, p1.first));
            double dis_p2 = abs(dot(pt-p2.second, p2.first));
            double dis_p3 = abs(dot(pt-p3.second, p3.first));
            double dis_p4 = abs(dot(pt-p4.second, p4.first));
            double dis_p5 = abs(dot(pt-p5.second, p5.first));
            double dis_p6 = abs(dot(pt-p6.second, p6.first));
            //cout<<dis_p1<<" "<<dis_p2<<" "<<dis_p3<<" "<<dis_p4<<" "<<dis_p5<<" "<<dis_p6<<endl;
            bool b=0;
            if (checkEquality(0,dis_p1) || checkEquality(0,dis_p4) ){
                b = b || (checkEquality(dis_p2+dis_p5,e1) && checkEquality(dis_p3+dis_p6,e2));
            }
            if (checkEquality(0,dis_p2) || checkEquality(0,dis_p5) ){
                b = b || (checkEquality(dis_p3+dis_p6,e2) && checkEquality(dis_p1+dis_p4,e3));
            }
            if (checkEquality(0,dis_p3) || checkEquality(0,dis_p6) ){
                b = b || (checkEquality(dis_p1+dis_p4,e3) && checkEquality(dis_p2+dis_p5,e1));
            }
            return b;
        }

        void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index){
            double t_cur,t_in,t_out;
            pair<vec3d,point3d> p_in,p_out;
            bool did_intersect=0;

            pair<vec3d,point3d> p_cur=p1;
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

            p_cur=p2;
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

            p_cur=p3;
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

            p_cur=p4;
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

            p_cur=p5;
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

            p_cur=p6;
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
        point3d base_corner;
        point3d corner1;
        point3d corner2;        
        point3d corner3;        //according to left hand system
        double e1,e2,e3;
        
        // plane is given by direction of normal=plane.first, a point on plane = plane.second

        pair<vec3d,point3d> p1, p2, p3, p4, p5, p6;

        // vector<pair<vec3d,point3d>> planes={p1,p2,p3,p4,p5,p6};
};