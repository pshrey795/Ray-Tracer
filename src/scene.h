#pragma once
#include "world_object.h"
#include "linker.h"
#include "camera.h"
#include "light.h"
#include<omp.h>
#define MAX_DEPTH 3
using namespace std;

//Scene object handles all the internal details of ray tracing
class scene {

    public:
        //Setting up camera and lights
        scene() {}
        scene(point3d view_pt, vec3d view_dir, vec3d orientation, int screen_width, int screen_height, int screen_depth){
            this->cam = camera(view_pt,view_dir,orientation,screen_width,screen_height,screen_depth);
        }

        //Objects can be added manually later
        void addSphere(point3d origin, double radius, material mat){
            sphere *newSphere = new sphere(origin,radius,mat);
            objects.push_back(newSphere);
        }

        void addEllipsoid(point3d center, double a, double b, double c, material mat){
            ellipsoid *newEllipsoid = new ellipsoid(center,a,b,c,mat);
            objects.push_back(newEllipsoid);
        }

        void addCuboid(point3d base_center, point3d corner1, point3d corner2, point3d corner3, material mat){
            cuboid *newCuboid = new cuboid(base_center, corner1, corner2, corner3, mat);
            objects.push_back(newCuboid);
        }

        void addCylinder(double x0,double z0,double h,double r, bool hollow, material mat){
            cylinder *newCylinder = new cylinder(x0,z0,h,r, hollow, mat);
            objects.push_back(newCylinder);
            if (!hollow){
                addCircle(x0,h,z0,r,mat);
                addCircle(x0,0,z0,r,mat);
            }
        }

        void addCone(double x0,double z0,double h,double r, material mat){
            cone *newCone = new cone(x0, z0, h, r, mat);
            objects.push_back(newCone);
        }

        void addQuadric(double A,double B,double C,double D,double E,double F,double G,double H,double I,double J, material mat){
            quadric *newQuadric = new quadric(A,B,C,D,E,F,G,H,I,J,mat);
            objects.push_back(newQuadric);
        }

        void addTriangle(point3d v1, point3d v2, point3d v3, material mat){
            triangle *newTriangle = new triangle(v1,v2,v3,mat);
            objects.push_back(newTriangle);
        }

        void addPolygon(long long n, vector<point3d> vertices, material mat){
            point3d center=vertices[0];
            for (int i=1;i<n;i++){
                center+=vertices[i];
            }
            center/=n;
            for (int i=0;i<n-1;i++){
                triangle *newTriangle = new triangle(vertices[i],vertices[i+1],center,mat);
                objects.push_back(newTriangle);
            }
            triangle *newTriangle = new triangle(vertices[n-1],vertices[0],center,mat);
            objects.push_back(newTriangle);
        }

        void addCircle(double x0, double y0, double z0, double r, material mat){
            circle *newCircle = new circle(x0,y0,z0,r,mat);
            objects.push_back(newCircle);
        }

        void addLight(point3d loc,color3d col){
            light *newLight = new light(loc,col);
            lights.push_back(newLight);
        }

        //Ray Tracing(to be called only after setting up the objects of the scene)
        void ray_cast(vector<vector<color3d>> &pixelMap, int samples_per_pixel, int n_threads){
            int screen_width = pixelMap[0].size();
            int screen_height = pixelMap.size();
            int window_length = (screen_height+n_threads-1)/n_threads;
            #pragma omp parallel num_threads(n_threads)
            {
                int thread_id = omp_get_thread_num();
                int y_high = min(screen_height,(thread_id+1)*window_length);
                int y_low = thread_id*window_length;
                for(int y=y_high-1;y>=y_low;y--){
                    for(int x=0;x<screen_width;x++){
                        color3d new_color(0.0,0.0,0.0);
                        for(int i=0;i<samples_per_pixel;i++){
                            double u = x - 0.5 + random_double();
                            double v = y - 0.5 + random_double();
                            ray new_ray = this->cam.generate_ray(u,v);
                            new_color += ray_color(new_ray,MAX_DEPTH, false);
                        }
                        pixelMap[y][x] = new_color / samples_per_pixel;
                    }
                }
            }
        }

        //For handling object intersection. Returns the intersection information with the closest object
        hit_record object_intersection(ray r,bool checkObstruction){
            hit_record hit_stats;
            double t_min = 0.0001;
            for(int i=0;i<objects.size();i++){
                if(checkObstruction){
                    if(!(objects[i]->getMat().getAttRefract()==color3d(0.0,0.0,0.0))){
                        continue;
                    }
                }
                double t_max = hit_stats.t;
                objects[i]->intersect(r,t_min,t_max,hit_stats,i);
            }
            return hit_stats;
        }

        //For handling intersection of light rays for local illumination
        vector<hit_record> light_intersection(point3d pt){
            vector<hit_record> v;
            for(int i=0;i<lights.size();i++){
                ray light_ray(lights[i]->getLoc(), pt - lights[i]->getLoc());
                v.push_back(object_intersection(light_ray,true));
            }
            return v;
        }

        //Calculating ray color(Lighting and Illumination)
        color3d ray_color(ray incident_ray, int ray_depth, bool isRefracted){
            color3d new_color(0,0,0);
            if(ray_depth>0){
                hit_record hit_stats = object_intersection(incident_ray,false);
                if(hit_stats.isIntersecting){
                    vector<hit_record> light_stats = light_intersection(hit_stats.pt_of_intersection);

                    //Local illumination using Phong's model
                    color3d ambient_color(0.0,0.0,0.0);
                    color3d diffuse_color(0.0,0.0,0.0);
                    color3d specular_color(0.0,0.0,0.0);
                    for(int i=0;i<lights.size();i++){
                        //Ambient component
                        ambient_color = ambient_color + lights[i]->getColor();
                        if(light_stats[i].index==hit_stats.index && checkDifference(hit_stats.pt_of_intersection,light_stats[i].pt_of_intersection)){
                            //Diffuse component
                            double cosine1 = abs(dot(unit_vec(hit_stats.pt_of_intersection - lights[i]->getLoc()),hit_stats.normal));
                            diffuse_color = diffuse_color + (cosine1 * lights[i]->getColor());

                            //Specular component
                            vec3d reflected = reflect(hit_stats.pt_of_intersection - lights[i]->getLoc(),hit_stats.normal);
                            double cosine2 = abs(dot(unit_vec(reflected),unit_vec(incident_ray.dir())));
                            int n = objects[i]->getMat().getN();
                            specular_color = specular_color + (pow(cosine2,n) * lights[i]->getColor());
                        }
                    }
                    color3d ka = objects[hit_stats.index]->getMat().getKa();
                    color3d kd = objects[hit_stats.index]->getMat().getKd();
                    color3d ks = objects[hit_stats.index]->getMat().getKs();
                    new_color = (ka * ambient_color) + (kd * diffuse_color) + (ks * specular_color);

                    //Global illumination using recursion

                    //Reflection
                    color3d attenuation1 = objects[hit_stats.index]->getMat().getAttReflect();
                    ray reflected_ray = ray(hit_stats.pt_of_intersection,reflect(incident_ray.dir(),hit_stats.normal));
                    color3d reflected_color = attenuation1 * ray_color(reflected_ray,ray_depth-1,false);
                    new_color = new_color + reflected_color;

                    //Refraction
                    color3d attenuation2 = objects[hit_stats.index]->getMat().getAttRefract();
                    if(!(attenuation2==color3d(0,0,0))){
                        ray refracted_ray;
                        if(dot(incident_ray.dir(),hit_stats.normal) < 0){
                            refracted_ray = ray(hit_stats.pt_of_intersection,refract(incident_ray.dir(),hit_stats.normal,objects[hit_stats.index]->getMat().get_refractive_index()));
                        }else{
                            vec3d inward_normal = (-1) * hit_stats.normal;
                            refracted_ray = ray(hit_stats.pt_of_intersection,refract(incident_ray.dir(),inward_normal,1/(objects[hit_stats.index]->getMat().get_refractive_index())));
                        }
                        color3d refracted_color = attenuation2 * ray_color(refracted_ray,ray_depth-1,true);
                        new_color = new_color + refracted_color;
                    }
                }else if(ray_depth==3 || isRefracted){
                    return color3d(0.5*255,0.7*255,0.9*255);
                }
            }
            return new_color;
        }

    private:
        camera cam;
        vector<world_object*> objects;
        vector<light*> lights;

        bool checkDifference(vec3d a,vec3d b){
            return (sqrt((a[0]-b[0])*(a[0]-b[0]) + (a[1]-b[1])*(a[1]-b[1]) + (a[2]-b[2])*(a[2]-b[2])) < 1e-2);
        }

};
