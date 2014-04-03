#include "../include/Angel.h"
#include "../include/mycolors.h"
#include "objLoader.h"
#include "heightmap.h"
#include <assert.h>
#include "viewControl.h"
#define FORWARD_STEP 0.4
#define SCALE_ANGLE 0.05
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 9; //(3 vertices)/faces
point4 points[NumVertices];
vec4 normal(0,0,1.0f,0);
int index = 0;
//---------------------------------------------------------------------------------
//Camera init----------------------------------------------------------------------
//---------------------------------------------------------------------------------

interfaceControl cam;

//---------------------------------------------------------------------------------
//Stack----------------------------------------------------------------------------
//---------------------------------------------------------------------------------

class MatrixStack {
    int    _index;
    int    _size;
    mat4*  _matrices;


    public:
    MatrixStack( int numMatrices = 32 ):_index(0), _size(numMatrices)
    { _matrices = new mat4[numMatrices]; }

    ~MatrixStack()
    { delete[]_matrices; }

    void push( const mat4& m ) {
        assert( _index + 1 < _size );
        _matrices[_index++] = m;
    }

    mat4& pop( void ) {
        assert( _index - 1 >= 0 );
        _index--;
        return _matrices[_index];
    }
};

//----------------------------------------------------------------------------
//Node -----------------------------------------------------------------------
//----------------------------------------------------------------------------

struct Node {
    mat4  transform;
    void  (*render)( int );

    Node* sibling;
    Node* child;
    Node* gar;
    int  color;
    Node():
        render(NULL), sibling(NULL), child(NULL), gar(NULL) {}

    Node( mat4& m, void (*render)(int), Node* sibling, Node* child, int color) :
        transform(m), render(render), sibling(sibling), child(child), gar(NULL), color(color) {}
};

//----------------------------------------------------------------------------
//Init varibles---------------------------------------------------------------
//----------------------------------------------------------------------------
GLuint projection_loc, color_loc, model_view_loc, normal_loc, camview_loc, campos_loc;
GLuint buffer, vao;


//----------------------------------------------------------------------------
//Tree Traverse---------------------------------------------------------------
//----------------------------------------------------------------------------
MatrixStack mvstack;
void traverse(Node *node)
{

    if ( node == NULL ) { return; }
    mvstack.push( cam._model_view );
    cam._model_view *= node->transform;
    node->render(node->color);
    if ( node->child ) { traverse( node->child ); }
    cam._model_view = mvstack.pop();
    if ( node->sibling ) { traverse( node->sibling ); }

}

//----------------------------------------------------------------------------
//3D meshes        -----------------------------------------------------------
//----------------------------------------------------------------------------

int gw=256,gh=256;

vector<vec2> grdTexcoord;
vector<vec4> grdPoints;
vector<vec4> grdNormals;
vector<int> grdIndex;
void genGround(vector<vec4>& temp1,vector<vec4>& temp2)
{
    const char* imgdir = "hm.ppm";
    heightmap img(imgdir,25.0f);
    for(int i=0; i<gw; ++i)
        for(int j=0; j<gh; ++j)
        {
            float h = float(img.getHeight(i,j));
            temp1.push_back(vec4(i*0.4,j*0.4, h/255.0*img.Scale() , 1.0));
            temp2.push_back(vec4(0,0,1,0));
            //std::cout<< h/255.0 <<std::endl;
        }    

}

void genGroundIndx(vector<int>& temp, int off)
{
    float tstep = 1.0f/255.0f;
    vector<vec3> normals;
    for(int i=0; i<gw-1; ++i)
        for(int j=0; j<gh-1; j++)
        {
            temp.push_back(i*gh+j+off);            
            temp.push_back(i*gh+j+1+off);
            temp.push_back((i+1)*gh+j+1+off);
            temp.push_back((i+1)*gh+j+1+off);
            temp.push_back(i*gh+j+off);
            temp.push_back((i+1)*gh+j+off);

        }
    for(int i=0; i<gw; ++i)
        for(int j=0; j<gh; j++)
        {
            grdTexcoord.push_back(vec2(j*tstep,i*tstep));
        } 
}


//----------------------------------------------------------------------------
//vertexs init     -----------------------------------------------------------
//----------------------------------------------------------------------------

void vertex_init()
{
    int ind = 0;
    points[ind] = point4(0.0f,0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(0.0f,-0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(0.0f,0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(0.5f,0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(-0.5f,0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(-0.5f,-0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(-0.5f,-0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(0.5f,-0.5f,0.0f,1.0f); ind++;
    points[ind] = point4(0.5f,0.5f,0.0f,1.0f); ind++;

}


//----------------------------------------------------------------------------
//VAO VBO GLSL init-----------------------------------------------------------
//----------------------------------------------------------------------------
int triind, recind;

GLuint IndexBufferId;
void init( char* dir )
{

    vector<vector<int> > z;

    genGround(grdPoints,grdNormals);
    objLoader obj;

    genGroundIndx(grdIndex,obj.sizeMesh());

    vertex_init();
    //---------------------------------------------------------------------------------
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer);
    glBindBuffer( GL_ARRAY_BUFFER, buffer );


    GLuint obj_1_Size = obj.sizeMesh()* sizeof(vec4);
    GLuint obj_2_Size = grdPoints.size() * sizeof(vec4);

    GLuint bufferSize =  sizeof(vec4) * ( obj.sizeMesh()*2 + grdPoints.size()*2);
    GLuint texbufferSize = sizeof(vec2) * grdTexcoord.size();

    glBufferData( GL_ARRAY_BUFFER, bufferSize + texbufferSize + obj_1_Size/2,  NULL , GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, obj_1_Size, obj.Mesh() );
    glBufferSubData(GL_ARRAY_BUFFER, bufferSize/2, obj_1_Size, obj.MeshNorm());
    glBufferSubData(GL_ARRAY_BUFFER, obj_1_Size, obj_2_Size, (vec4*)&(*grdPoints.begin()) );
    glBufferSubData(GL_ARRAY_BUFFER, bufferSize/2+obj_1_Size, obj_2_Size, (vec4*)&(*grdNormals.begin()) );
    //tex buffer reading
    glBufferSubData(GL_ARRAY_BUFFER, bufferSize+obj_1_Size/2 , texbufferSize, (vec2*)&(*grdTexcoord.begin()) );
    triind = obj.sizeMesh();
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "./glsl/vshader83.glsl", "./glsl/fshader83.glsl" );
    glUseProgram( program );

    GLuint vPosition,vNormal,texcoord;
    vPosition = glGetAttribLocation(program, "vPosition");
    vNormal = glGetAttribLocation(program,"vNormal");
    texcoord = glGetAttribLocation(program,"texcoord");
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vNormal);
    glEnableVertexAttribArray(texcoord);

    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));
    glVertexAttribPointer( vNormal , 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(bufferSize/2));
    glVertexAttribPointer( texcoord , 2, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(bufferSize));

    //Uniorm Location-------------------------------------------------------------------------
    model_view_loc = glGetUniformLocation( program, "modelview");
    color_loc = glGetUniformLocation(program, "vColor");
    projection_loc = glGetUniformLocation(program, "projection");
    camview_loc = glGetUniformLocation(program,"camview");
    campos_loc = glGetUniformLocation(program,"campos");
    GLuint diffuse_loc = glGetUniformLocation(program,"diffuse");
    GLuint ambient_loc = glGetUniformLocation(program,"ambient");
    GLuint specular_loc = glGetUniformLocation(program,"specular");
    //----------------------------------------------------------------------------
    glUniformMatrix4fv(projection_loc,1,GL_TRUE,cam._cam_proj);
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,cam._model_view);
    glUniform4fv(diffuse_loc,1,normalize(vec4(0.8,0.8,0.8,1))); 
    glUniform4fv(ambient_loc,1,vec4(0.55,0.55,0.55,1)); 
    glUniform4fv(specular_loc,1,vec4(0.2,0.2,0.2,1)); 
    //---------------------------------------------------------------------------------
    //TextureId---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    int tw,th,tmax;
    unsigned char *data = read_ppm("terrain.ppm",&tw,&th,&tmax);
    
    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);
    
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
     
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
     
    // "Bind" the newly created texture : all future texture functions will modify this texture
    GLuint textureHeight;
    glGenTextures(1, &textureHeight);
    glBindTexture(GL_TEXTURE_2D, textureHeight);
     
    unsigned char* data2 = read_ppm("hm.ppm",&tw,&th,&tmax);
    // Give the image to OpenGL
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tw, th, 0, GL_BGR, GL_UNSIGNED_BYTE, data2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);  
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------
    //pass it to shader
    GLuint uniformId1 = glGetUniformLocation(program, "myTextureSampler");
    GLuint uniformId2 = glGetUniformLocation(program, "height");
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureHeight);  
    glUniform1i(uniformId2, GL_TEXTURE1-GL_TEXTURE0); // 0 is the texture numbe
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);  
    glUniform1i(uniformId1, 0); // 0 is the texture numbe
    

    //---------------------------------------------------------------------------------
    //---------------------------------------------------------------------------------


    //printf("%d\n",grdIndex.size());
    //head = cube(3.0f,9);
    glGenBuffers(1, &IndexBufferId);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexBufferId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*grdIndex.size(),&grdIndex[0], GL_STATIC_DRAW);



    glEnable( GL_DEPTH_TEST );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void idle(void)
{
    glutPostRedisplay();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
double lastTime;
int nbc=0;
int nbFrames = 0;
void display(void)
{

    //----------------------------------------------------------------------------
    // Measure speed
    double currentTime = glutGet(GLUT_ELAPSED_TIME);
    nbFrames++;
    nbc++;
    if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        if(nbc%100==0)
            printf("%f ms/frame\n", double(nbFrames)*1000.0/(currentTime-lastTime));
        nbFrames = 0;
        lastTime = currentTime;
    }

    cam.keyboardMonitor();
    cam.mouseMonitor();
    //----------------------------------------------------------------------------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
    glBindBuffer(GL_ARRAY_BUFFER,buffer);


    glUniformMatrix4fv(camview_loc,1,GL_TRUE,cam._cam_view);
    glUniform4fv(campos_loc,1, cam._cam_pos);
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,cam._model_view*Translate(-50,-50,-10));

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,IndexBufferId);
    glDrawElements( GL_TRIANGLES, grdIndex.size(), GL_UNSIGNED_INT, 0);
    //glDrawElements( GL_LINE_LOOP, grdIndex.size() , GL_UNSIGNED_INT,0);    
    //    traverse(head);

    glutSwapBuffers();
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 800, 800 );
    glutInitWindowPosition( 512, 112);
    glutCreateWindow( "part1" );    
    glewInit();
   
    init(argv[1]);

    lastTime = glutGet(GLUT_ELAPSED_TIME);

    glutDisplayFunc( display );

    cam.initKeyboard();
    cam.initMouse();

    glutIdleFunc(idle);
   
    glutMainLoop();
    return 0;
}
