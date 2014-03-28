#ifndef OBJLOADER_H
#define OBJLOADER_H

#include"../include/Angel.h"
#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<sstream>

using namespace std;
using namespace Angel;


class objLoader
{
    vector<vec4> vertices;
    vector<vec4> normals;
    vector<vector<int> > face;
    vector<vec4> mesh;
    vector<vec4> meshnorm;
    vector<int> extractface(string);
    void reOrganizeVertices();
    public:
        objLoader(const char* dir);
        ~objLoader(){};
        int sizeVert();
        int sizeNorm();
        int sizeMesh();
        int sizeFace();
        vec4* Vert();
        vec4* Norm();
        vec4* Mesh();
        vec4* MeshNorm();

};




#endif
