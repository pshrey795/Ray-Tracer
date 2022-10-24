#pragma once
#include "world_object.h"

class quadric : public world_object {
    public:
        quadric(){}
        quadric(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J, material mat){
            this->A=A;
            this->B=B;
            this->C=C;
            this->D=D;
            this->E=E;
            this->F=F;
            this->G=G;
            this->H=H;
            this->I=I;
            this->J=J;
            this->mat=mat;
        }

        //Checking if a point is on this object
        bool checkPoint(point3d pt){
            double xo,yo,zo;
            xo=pt[0]; yo=pt[1]; zo=pt[2];
            double temp = A*xo*xo+2*B*xo*yo+2*C*xo*zo+2*D*xo+E*yo*yo+2*F*yo*zo+2*G*yo+H*zo*zo+2*I*zo+J;
            return checkEquality(temp,0);
        }

        void intersect(ray incident_ray, double lower_limit, double upper_limit, hit_record& record,int index){
            double xo,yo,zo,xd,yd,zd;
            xo=incident_ray.orig()[0];
            yo=incident_ray.orig()[1];
            zo=incident_ray.orig()[2];
            xd=incident_ray.dir()[0];
            yd=incident_ray.dir()[1];
            zd=incident_ray.dir()[2];
            double a = (A*xd*xd)+(E*yd*yd)+(H*zd*zd)+2*((B*xd*yd)+(C*xd*zd)+(F*yd*zd));  
            double b = 2*((A*xo*xd)+(E*yo*yd)+(H*zo*zd)+B*((xo*yd)+(xd*yo))+C*((xo*zd)+(xd*zo))+F*((yo*zd)+(yd*zo))+(D*xd)+(G*yd)+(I*zd));
            double c = (A*xo*xo)+(E*yo*yo)+(H*zo*zo)+2*((B*xo*yo)+(C*xo*zo)+(F*yo*zo))+2*((D*xo)+(G*yo)+(I*zo))+J;
            
            if (checkEquality(a,0)){
                double t = (-1*c)/b;
                if(t > lower_limit && t < upper_limit){
                    record.t = t;
                    record.pt_of_intersection = incident_ray.locate(t);
                    double Nx = 2*(A*(xo+xd*t)+B*(yo+yd*t)+C*(zo+zd*t)+D);
                    double Ny = 2*(B*(xo+xd*t)+E*(yo+yd*t)+F*(zo+zd*t)+G);
                    double Nz = 2*(C*(xo+xd*t)+F*(yo+yd*t)+H*(zo+zd*t)+I);
                    record.normal = unit_vec(vec3d(Nx,Ny,Nz));
                    record.isIntersecting = true;
                    record.index = index;
                }
            }else{
                double d = b*b - 4*a*c;
                if(d < 0){
                    return;
                }else{
                    double t_low = (-b - sqrt(d))/(2*a);
                    double t_high = (-b + sqrt(d))/(2*a);
                    if(t_low > lower_limit && t_low < upper_limit){
                        record.t = t_low;
                        record.pt_of_intersection = incident_ray.locate(t_low);
                        double Nx = 2*(A*(xo+xd*t_low)+B*(yo+yd*t_low)+C*(zo+zd*t_low)+D);
                        double Ny = 2*(B*(xo+xd*t_low)+E*(yo+yd*t_low)+F*(zo+zd*t_low)+G);
                        double Nz = 2*(C*(xo+xd*t_low)+F*(yo+yd*t_low)+H*(zo+zd*t_low)+I);
                        record.normal = unit_vec(vec3d(Nx,Ny,Nz));
                        record.isIntersecting = true;
                        record.index = index;
                    }else if(t_high > lower_limit && t_high < upper_limit){
                        record.t = t_high;
                        record.pt_of_intersection = incident_ray.locate(t_high);
                        double Nx = 2*(A*(xo+xd*t_high)+B*(yo+yd*t_high)+C*(zo+zd*t_high)+D);
                        double Ny = 2*(B*(xo+xd*t_high)+E*(yo+yd*t_high)+F*(zo+zd*t_high)+G);
                        double Nz = 2*(C*(xo+xd*t_high)+F*(yo+yd*t_high)+H*(zo+zd*t_high)+I);
                        record.normal = unit_vec(vec3d(Nx,Ny,Nz));
                        record.isIntersecting = true;
                        record.index = index;
                    }else{
                        return;
                    }
                }
            }
        }

    private:
        double A,B,C,D,E,F,G,H,I,J;
        // equation: Ax^2 + 2Bxy + 2Cxz + 2Dx + Ey^2 + 2Fyz + 2Gy + Hz^2 +2Iz + J
};