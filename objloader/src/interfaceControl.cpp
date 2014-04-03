#include "viewControl.h"

void interfaceControl::initMouse()
{
    glutMotionFunc(&interfaceControl::mouseMotion);
    glutMouseFunc(&interfaceControl::mouseButton);
}

bool interfaceControl::_mouseTriger = 0;
int interfaceControl::_mousex=0;
int interfaceControl::_mousey=0;
int interfaceControl::_dx=0;
int interfaceControl::_dy=0;

void interfaceControl::mouseMotion(int x, int y)
{
    if(_mouseTriger)
    {
        _dx=x-_mousex;
        _dy=y-_mousey;
        _mousex = x;
        _mousey = y;
    }
}

void interfaceControl::mouseButton(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            _mouseTriger = 1;
            _mousex=x;
            _mousey=y;
        }
        if(state == GLUT_UP)
        {
            _dx=0;
            _dy=0;
            _mouseTriger = 0;
        }
    }
}

void interfaceControl::mouseMonitor()
{
    if(_dx!=0)
    {
        mod_RotateZ(float(_dx)*_dstep/10.0f);
    }
    if(_dy!=0)
    {
        mod_RotateY(float(_dy)*_dstep/10.0f);
    }
}



bool interfaceControl::_key[256]={0};

void interfaceControl::keyboard(unsigned char key, int x,int y)
{
    _key[key] = 1;
}

void interfaceControl::keyboardup(unsigned char key, int x, int y)
{
    _key[key] = 0;
}

void interfaceControl::initKeyboard()
{
    glutKeyboardFunc(&interfaceControl::keyboard);
    glutKeyboardUpFunc(&interfaceControl::keyboardup);
}

void interfaceControl::keyboardMonitor()
{
    if(_key['w'])
    {
        cam_Forward(_fstep);
    }
    if(_key['s'])
    {
        cam_Backward(_fstep);
    }
    if(_key['q'])
    {
        cam_RotateU(_dstep);
    }
    if(_key['e'])
    {
        cam_RotateU(-_dstep);
    }
    if(_key['a'])
    {
        cam_RotateZ(_dstep);
    }
    if(_key['d'])
    {
        cam_RotateZ(-_dstep);
    }
    if(_key['r'])
    {
        exit(EXIT_SUCCESS);
    }
}
