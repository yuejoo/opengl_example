#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <iostream>
#include "../include/Angel.h"
#include <vector>
#include <string>
#include "ppm_lib.h"
#define HHJL 32
using namespace std;


class heightmap
{
    int _width;
    int _max;
    float _scaling_varible;
    unsigned char *_data;

    public:
        heightmap(char*,float);
        void loadHeightmap(char*);
        void saveHeightmap(char*);
        void unloadHeightmap();
        inline void setScale(int s)
        { _scaling_varible = s; }
        inline int Width()
        { return _width;}
        inline float Scale()
        { return _scaling_varible;}
        inline unsigned int getHeight(int x, int y)
        { return _data[(x*_width+y)*3]; }
        ~heightmap(){ unloadHeightmap(); }
};

#endif 
