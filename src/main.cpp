#include<bits/stdc++.h>
#include<GL/glut.h>
#include "scene.h"
using namespace std;

//Final color map to be displayed. Size depends on the resolution of image
vector<vector<color3d>> pixelMap;

//A default scene for testing 
scene generate_sample_scene(){
    //Resizing pixel map according to resolution
    pixelMap = vector<vector<color3d>>(600,vector<color3d>(800,color3d()));

    //Manual Scene Description
    scene s(point3d(1000,1000,0),vec3d(-1,-1,-8),vec3d(0,1,0),800,600,1500);
    material m(color3d(0.01,0.1,0.01),color3d(0,0,0),color3d(0.01,0.01,0.01),color3d(0.8,0.2,0.2),color3d(0.3,0.3,0.3),40,1.5);
    material m2(color3d(0.0,0.0,0.0),color3d(1,1,1),color3d(0.1,0.1,0.1),color3d(0.1,0.2,0.3),color3d(0,0,0),40,1.2);
    material m3(color3d(0.01,0.01,0.01),color3d(0,0,0),color3d(0.01,0.01,0.01),color3d(0.2,0.2,0.8),color3d(0.3,0.3,0.3),40,1.5);
    material m4(color3d(0.3,0.3,0.3),color3d(0,0,0),color3d(0.01,0.01,0.01),color3d(1,1,1),color3d(0.3,0.3,0.3),40,1.5);
    s.addSphere(point3d(0,600,-8000),600,m);
    s.addEllipsoid(point3d(1200,300,-8000), 700 ,300 ,500 ,m3);
    s.addSphere(point3d(500,500,-4000),300,m2);
    s.addSphere(point3d(0,-20000000,0),20000000,m4);
    s.addLight(point3d(-700,+700,-2000),color3d(255,255,255));
    return s;
}

//Helper function for parsing
point3d readPoint(string s){
    stringstream ss(s);
    double x,y,z;
    ss>>x>>y>>z;
    return point3d(x,y,z);
}

vec3d readVec(string s){
    stringstream ss(s);
    double x,y,z;
    ss>>x>>y>>z;
    return vec3d(x,y,z);
}

color3d readColor(string s){
    stringstream ss(s);
    double x,y,z;
    ss>>x>>y>>z;
    return color3d(x,y,z);
}

material readMaterial(ifstream& myfile){
    string s;
    getline(myfile,s);
    getline(myfile,s);
    color3d a_reflect = readColor(s);
    getline(myfile,s);
    color3d a_refract = readColor(s);
    getline(myfile,s);
    color3d ka = readColor(s);
    getline(myfile,s);
    color3d kd = readColor(s);
    getline(myfile,s);
    color3d ks = readColor(s);
    getline(myfile,s);
    int n = stoi(s);
    getline(myfile,s);
    double r = stod(s);
    getline(myfile,s);
    return material(a_reflect,a_refract,ka,kd,ks,n,r);
}

//Custom scene according to input description
//Parser for input file
scene generate_input_scene(string scene_file){
    ifstream myfile;
    myfile.open(scene_file);
    string line;

    //Reading camera parameters
    int screen_width, screen_height, screen_depth;
    point3d cam_pos;
    vec3d cam_dir, cam_up;
    getline(myfile,line);
    while(line==""  || line[0]=='#'){
        getline(myfile,line);
    }
    if(line.substr(0,line.length()-1)=="camera"){
        getline(myfile,line);
        cam_pos = readPoint(line);
        getline(myfile,line);
        cam_dir = readVec(line);
        getline(myfile,line);
        cam_up = readVec(line);
        getline(myfile,line);
        screen_width = stoi(line);
        getline(myfile,line);
        screen_height = stoi(line);
        getline(myfile,line);
        screen_depth = stoi(line);
    }
    while(line==""  || line[0]=='#'){
        getline(myfile,line);
    }

    scene s(cam_pos,cam_dir,cam_up,screen_width,screen_height,screen_depth);

    //Resizing pixel map according to resolution
    pixelMap = vector<vector<color3d>>(screen_height,vector<color3d>(screen_width,color3d()));

    //Reading objects
    while(line != "end"){
        while(line==""  || line[0]=='#'){
            getline(myfile,line);
        }
        if(line.substr(0,line.length()-1)=="light"){
            point3d light_pos;
            getline(myfile,line);
            light_pos = readPoint(line);
            color3d light_color;
            getline(myfile,line);
            light_color = readColor(line);
            s.addLight(light_pos,light_color);
        }else if(line.substr(0,line.length()-1)=="sphere"){
            point3d sphere_pos;
            getline(myfile,line);
            sphere_pos = readPoint(line);
            double sphere_radius;
            getline(myfile,line);
            sphere_radius = stod(line);
            material sphere_material = readMaterial(myfile);
            s.addSphere(sphere_pos,sphere_radius,sphere_material);
        }else if(line.substr(0,line.length()-1)=="ellipsoid"){
            point3d ellipsoid_pos;
            getline(myfile,line);
            ellipsoid_pos = readPoint(line);
            double ellipsoid_a, ellipsoid_b, ellipsoid_c;
            getline(myfile,line);
            ellipsoid_a = stod(line);
            getline(myfile,line);
            ellipsoid_b = stod(line);
            getline(myfile,line);
            ellipsoid_c = stod(line);
            material ellipsoid_material = readMaterial(myfile);
            s.addEllipsoid(ellipsoid_pos,ellipsoid_a,ellipsoid_b,ellipsoid_c,ellipsoid_material);   
        }else if(line.substr(0,line.length()-1)=="box"){
            point3d cuboid_pos, corner1, corner2, corner3;
            getline(myfile,line);
            cuboid_pos = readPoint(line);
            getline(myfile,line);
            corner1 = readPoint(line);
            getline(myfile,line);
            corner2 = readPoint(line);
            getline(myfile,line);
            corner3 = readPoint(line);
            material cuboid_material = readMaterial(myfile);
            s.addCuboid(cuboid_pos,corner1,corner2,corner3,cuboid_material);
        }else if(line.substr(0,line.length()-1)=="triangle"){
            point3d v1,v2,v3;
            getline(myfile,line);
            v1 = readPoint(line);
            getline(myfile,line);
            v2 = readPoint(line);
            getline(myfile,line);
            v3 = readPoint(line);
            material triangle_material = readMaterial(myfile);
            s.addTriangle(v1,v2,v3,triangle_material);
        }else if(line.substr(0,line.length()-1)=="polygon"){
            long long n;
            getline(myfile,line);
            n = stoll(line);
            vector<point3d> v;
            for(int i=0;i<n;i++){
                getline(myfile,line);
                v.push_back(readPoint(line));
            }
            material polygon_material = readMaterial(myfile);
            s.addPolygon(n,v,polygon_material);
        }else if(line.substr(0,line.length()-1)=="cylinder"){
            double x,z,h,r;
            int hollow;
            getline(myfile,line);
            x = stod(line);
            getline(myfile,line);
            z = stod(line);
            getline(myfile,line);
            h = stod(line);
            getline(myfile,line);
            r = stod(line);
            getline(myfile,line);
            hollow = stoi(line);
            material cylinder_material = readMaterial(myfile);
            s.addCylinder(x,z,h,r,hollow,cylinder_material);
        }else if(line.substr(0,line.length()-1)=="cone"){
            double x,z,h,r;
            getline(myfile,line);
            x = stod(line);
            getline(myfile,line);
            z = stod(line);
            getline(myfile,line);
            h = stod(line);
            getline(myfile,line);
            r = stod(line);
            material cone_material = readMaterial(myfile);
            s.addCone(x,z,h,r,cone_material);
        }else if(line.substr(0,line.length()-1)=="quadric"){
            double a,b,c,d,e,f,g,h,i,j;
            getline(myfile,line);
            a = stod(line);
            getline(myfile,line);
            b = stod(line);
            getline(myfile,line);
            c = stod(line);
            getline(myfile,line);
            d = stod(line);
            getline(myfile,line);
            e = stod(line);
            getline(myfile,line);
            f = stod(line);
            getline(myfile,line);
            g = stod(line);
            getline(myfile,line);
            h = stod(line);
            getline(myfile,line);
            i = stod(line);
            getline(myfile,line);
            j = stod(line);
            material quadric_material = readMaterial(myfile);
            s.addQuadric(a,b,c,d,e,f,g,h,i,j,quadric_material);
        }
        getline(myfile,line);
        getline(myfile,line);
        while(line=="" || line[0]=='#'){
            getline(myfile,line);
        }
    }
    myfile.close();
    return s;
}

double clamp(double a,double low, double high){
    if(a<low){
        return low;
    }else if(a>high){
        return high;
    }else{
        return a;
    }
}

void display(){
    int h = pixelMap.size();
    int w = pixelMap[0].size();
    cout<<"P3\n"<<w<<" "<<h<<"\n255\n";
    for(int j=h-1;j>=0;j--){
        for(int i=0;i<w;i++){
            int r = clamp(pixelMap[j][i][0],0,255);
            int g = clamp(pixelMap[j][i][1],0,255);
            int b = clamp(pixelMap[j][i][2],0,255);
            cout << r << " " << g << " " << b << "\n";
        }
    }
}

int main(int argc,char** argv){

    //Generating a scene
    scene s;

    if(argc < 2){
        s = generate_sample_scene();
    }else{
        s = generate_input_scene(argv[1]);
    }

    //Populating the pixel map by ray casting
    int samples_per_pixel = 8;
    int n_threads = 12;
    s.ray_cast(pixelMap,samples_per_pixel,n_threads);

    display();

    return 0;
}