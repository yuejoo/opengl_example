#ifndef MODELNODE_H
#define MODELNODE_H

#include "../include/Angel.h"
#include "viewControl.h"
#include <iostream>
#include <vector>
#include <map>
using namespace Angel;
using namespace std;
class modelNode
{
    typedef vec4* _nodeptr;
    typedef vec2* _tcrdptr;
    GLuint _vbo[4];
    GLuint _vao;
    GLuint _program;
    GLuint _loc[4];
    GLuint _uniformLoc[8];
    enum {VERTICE = 0, TEXCOORD = 1, INDEX = 2, NORMAL = 3, POINTSIZE = sizeof(vec4), COORDSIZE = sizeof(vec2), MODVIEW = 0, VCOLOR = 1, PROJ = 2, CAMVIEW = 3, CAMPOS = 4, DIFF = 5, AMBI= 6, SPECU = 7  };

    GLuint _camloc; 
    void vboInit();
    void vaoInit();

    public:
    modelNode* sibling;
    modelNode* child;
    inline mat4 transform()
    {
        return _modelview;
    }
    inline void initransform(const mat4& mv)
    {
        _modelview = mv;
    }
    mat4 _modelview;
    vector<unsigned int> _index;
    vector<vec2> _texcoord; 
    static vector<vec4> _normals;
    static vector<vec4> _points;
    static map<vec2,vec4> _pointmap;

    modelNode(){};
    void bindVAO();
    void bindTobuffer();
    void initShader(const char*,const char*);// vshader, fshader
    void initUniform(const viewControl&);
    void updateView(const viewControl&, const mat4&);
    void config();
    virtual void Mesh(){};
    virtual void Render(const viewControl&, const mat4&){};
    void Texture(){};
};

class mycube: public modelNode
{
    float x,y,z;
    float w,h,l;

    public:

    mycube()
    {
        x=0;
        y=0;
        z=0;
        w=1;
        l=1;
        h=1;
        _modelview = mat4(1.0f); 
        sibling = NULL;
        child = NULL;
    }

    mycube(const mat4& init, float wi, float hi, float li)
    {
        x=0;
        y=0;
        z=0;
        w=wi;
        l=hi;
        h=li;
        _modelview = init; 
        sibling = NULL;
        child = NULL;
    };


// naive implementation didn't fix the static member issue
    void Mesh(const vec2& temp) 
    {
        int i = _points.size();
        w=temp.x;
        l=temp.y;
        for(int i=0; i<1; i++)
        { 
            _points.push_back( vec4(x,y,z+i*h,1) );
            _normals.push_back( vec4( 0, 0, 1, 0));
            _points.push_back( vec4(x+w,y,z+i*h,1) );
            _normals.push_back( vec4( 0, 0, 1, 0));
            _points.push_back( vec4(x+w,y+l,z+i*h,1) );
            _normals.push_back( vec4( 0, 0, 1, 0));
            _points.push_back( vec4(x,y+l,z+i*h,1) );            
            _normals.push_back( vec4( 0, 0, 1, 0));
        }
        cout<<"----"<<_points.size()<<endl;
        _index.push_back(i+0);
        _index.push_back(i+1);
        _index.push_back(i+2);

        _index.push_back(i+2);
        _index.push_back(i+3);
        _index.push_back(i+0);

    }

    void Bind(const char* vdir, const char* fdir, const viewControl& cam)
    {
        initShader(vdir,fdir);
        initUniform(cam);
        bindTobuffer();
        config();    
    }
    inline void Render(const viewControl& cam, const mat4& trans)
    {
        
        updateView(cam,trans);
        bindVAO();
        glDrawElements( GL_TRIANGLES, _index.size(), GL_UNSIGNED_INT, 0);

    }

};


#endif
