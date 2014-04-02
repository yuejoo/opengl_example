#include "viewControl.h"

void viewControl::cam_Translate(float x,float y, float z)
{
    mat4 trans = Translate(x,y,z);
    _cam_pos = trans * _cam_pos;
    _cam_view = trans * _cam_view;
}


void viewControl::cam_RotateZ(float cita)
{
    mat4 rot = RotateZ(cita);
    _n = normalize(rot*_n);
    _v = normalize(rot*_v);
    _u = normalize(rot*_u);
    _cam_view = LookAt(_cam_pos,_cam_pos+_n,_v);   
}


void viewControl::cam_RotateU(const float& cita)
{
    mat4 rot = rotateN(_u,cita/180.0f*M_PI);
    _n = normalize(rot*_n);
    _v = normalize(rot*_v);
    _u = normalize(rot*_u);
    _cam_view = LookAt(_cam_pos,_cam_pos+_n,_v);   
}

void viewControl::cam_Forward(const float& s)
{    
    _cam_pos += s*_n;
    _cam_view = LookAt(_cam_pos,_cam_pos+_n,_v);   
}

void viewControl::cam_Backward(const float& s)
{    
    _cam_pos -= s*_n;
    _cam_view = LookAt(_cam_pos,_cam_pos+_n,_v);  
}
