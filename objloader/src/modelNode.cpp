#include "modelNode.h"

map<vec2,vec4> _pointmap;
vector<vec4> modelNode::_points;
vector<vec4> modelNode::_normals;

void modelNode::vaoInit( )
{
    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
}

void modelNode::bindVAO()
{

    glBindVertexArray(_vao);
}

void modelNode::vboInit()
{

    glGenBuffers(1,&_vbo[VERTICE]);
    glGenBuffers(1,&_vbo[TEXCOORD]);
    glGenBuffers(1,&_vbo[NORMAL]);
    glGenBuffers(1,&_vbo[INDEX]);

}

void modelNode::initShader(const char* vdir, const char* fdir)
{
    _program = InitShader(vdir,fdir);
    glUseProgram(_program);
    _loc[VERTICE] = glGetAttribLocation(_program,"vPosition");
    _loc[TEXCOORD] = glGetAttribLocation(_program,"texcoord");
    _loc[NORMAL] = glGetAttribLocation(_program,"vNormal");
    _uniformLoc[CAMVIEW] = glGetUniformLocation(_program,"camview");
    _uniformLoc[MODVIEW] = glGetUniformLocation(_program,"modelview");
    _uniformLoc[VCOLOR] = glGetUniformLocation(_program,"vColor");
    _uniformLoc[PROJ] = glGetUniformLocation(_program,"projection");
    _uniformLoc[CAMPOS] = glGetUniformLocation(_program,"campos");
    _uniformLoc[DIFF] = glGetUniformLocation(_program,"diffuse");
    _uniformLoc[AMBI] = glGetUniformLocation(_program,"ambient");
    _uniformLoc[SPECU] = glGetUniformLocation(_program,"specular");
    cout<<_uniformLoc[PROJ]<< " 1 "<< _uniformLoc[CAMVIEW]<<endl;
}

void modelNode::initUniform(const viewControl& cam)
{
    cout<<_uniformLoc[PROJ]<< " 2 "<< _uniformLoc[CAMVIEW]<<endl;
    glUniformMatrix4fv(_uniformLoc[PROJ],1,GL_TRUE,cam._cam_proj);
    glUniformMatrix4fv(_uniformLoc[MODVIEW],1,GL_TRUE,cam._model_view*_modelview);
    glUniformMatrix4fv(_uniformLoc[CAMVIEW],1,GL_TRUE,cam._cam_view);
    glUniformMatrix4fv(_uniformLoc[CAMPOS],1,GL_TRUE,cam._cam_pos);
    glUniform4fv(_uniformLoc[DIFF],1,normalize(vec4(0.3,0.3,0.3,1)));
    glUniform4fv(_uniformLoc[AMBI],1,vec4(0.55,0.55,0.55,1));
    glUniform4fv(_uniformLoc[SPECU],1,vec4(0.4,0.4,0.4,1));
}

void modelNode::config()
{
    glEnable( GL_DEPTH_TEST );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );
}


void modelNode::updateView(const viewControl& cam, const mat4& mv)
{
    glUniformMatrix4fv(_uniformLoc[CAMVIEW],1,GL_TRUE,cam._cam_view);
    glUniformMatrix4fv(_uniformLoc[MODVIEW],1,GL_TRUE,cam._model_view*mv);
    glUniformMatrix4fv(_uniformLoc[CAMPOS],1,GL_TRUE,cam._cam_pos);
}

void modelNode::bindTobuffer()
{    
    vaoInit();
    vboInit();
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[VERTICE]);
    glBufferData(GL_ARRAY_BUFFER, _points.size()*POINTSIZE, (_nodeptr)&(*_points.begin()) , GL_STREAM_DRAW);
    glEnableVertexAttribArray(_loc[VERTICE]);
    glVertexAttribPointer( _loc[VERTICE], 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));
    
    glBindBuffer(GL_ARRAY_BUFFER, _vbo[NORMAL]);
    glBufferData(GL_ARRAY_BUFFER, _normals.size()*POINTSIZE, (_nodeptr)&(*_normals.begin()) , GL_STREAM_DRAW);
    glEnableVertexAttribArray(_loc[NORMAL]);
    glVertexAttribPointer( _loc[NORMAL], 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));

    glBindBuffer(GL_ARRAY_BUFFER, _vbo[TEXCOORD]);
    glBufferData(GL_ARRAY_BUFFER, _texcoord.size()*COORDSIZE, (_nodeptr)&(*_texcoord.begin()) , GL_STREAM_DRAW);
    glEnableVertexAttribArray(_loc[TEXCOORD]);     
    glVertexAttribPointer( _loc[TEXCOORD], 2, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vbo[INDEX]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _index.size()*POINTSIZE, (_tcrdptr)&(*_index.begin()) , GL_STREAM_DRAW);

}


