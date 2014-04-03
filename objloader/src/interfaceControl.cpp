#include "viewControl.h"

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
