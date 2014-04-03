#ifndef VIEWCONTROL_H
#define VIEWCONTROL_H

#include "../include/Angel.h"
#include <iostream>

using namespace Angel;
using namespace std;
class viewControl
{
    public:
    vec4 _v;
    vec4 _u;
    vec4 _n;
    vec4 _cam_pos;
    mat4 _cam_view;
    mat4 _cam_proj;
    mat4 _model_view;

    viewControl()
    {
        _model_view = mat4(1.0f);
        _v=vec4(0.0f,0.0f,1.0f,0.0f);
        _u=vec4(0.0f,1.0f,0.0f,0.0f);
        _cam_pos=vec4(1.0,0.0f,1.0f,1.0f);
        _n= vec4(-1.0f,0.0f,0.0f,0.0f);
        _cam_proj = Frustum(-0.1024f, 0.1024f, -0.1024f, 0.1024f, 0.2f, 200.0f);
        _cam_view = LookAt(_cam_pos,_cam_pos+_n,_v);    
    }
    viewControl(const viewControl&);
    ~viewControl(){};
    
    void cam_Translate(float,float,float);
    void cam_RotateZ(const float&);    
    void cam_RotateU(const float&);
    void cam_Forward(const float&);
    void cam_Backward(const float&);

};

class interfaceControl: public viewControl
{
    private:
        static bool _key[256];
        float _fstep;
        float _dstep;
        static void keyboard(unsigned char, int, int);
        static void keyboardup(unsigned char, int, int);
    public:

        interfaceControl(float f=0.5f,float d= 5.0f):_fstep(f),_dstep(d){};
        void initKeyboard();
        void keyboardMonitor(); 

};




#endif
