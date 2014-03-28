#include "../include/Angel.h"
#include "../include/mycolors.h"
#include "objLoader.h"
#include <assert.h>
#define FORWARD_STEP 0.4
#define SCALE_ANGLE 0.05
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 9; //(3 vertices)/faces
point4 points[NumVertices];
vec4 normal(0,0,1.0f,0);
int index = 0;

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
Node* head = NULL;
Node* garb= NULL; // Memory Links
MatrixStack mvstack; // matrix stack
mat4 model_view=mat4(1.0f); // model_view init
GLuint projection_loc, color_loc, model_view_loc, normal_loc, camview_loc, campos_loc;
GLuint buffer, vao;

//----------------------------------------------------------------------------
//Draw Function---------------------------------------------------------------
//----------------------------------------------------------------------------
void blank(int i){}


void triangle(int color)
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE, model_view);

    glUniform4fv(normal_loc,1,normalize(model_view * vec4(0,0,1,0)));

    glUniform4fv(color_loc,1,vertex_colors[color]);

    glDrawArrays(GL_TRIANGLES, 0 , 3);
}

void rectangle(int color)
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE, model_view);

    glUniform4fv(normal_loc,1,normalize( model_view * vec4(0,0,1,0)));

    glUniform4fv(color_loc,1,vertex_colors[color]);

    glDrawArrays(GL_TRIANGLES, 3 , 6);
    
    glUniform4fv(normal_loc,1,normalize( model_view * vec4(0,1 ,1,0)));

    glDrawArrays(GL_LINES, 3, 2);
    
    glUniform4fv(normal_loc,1,normalize( model_view * vec4(-1, 0,1,0)));

    glDrawArrays(GL_LINES, 4, 2);
}

//----------------------------------------------------------------------------
//Tree Traverse---------------------------------------------------------------
//----------------------------------------------------------------------------

void traverse(Node *node)
{

    if ( node == NULL ) { return; }
    mvstack.push( model_view );
    model_view *= node->transform;
    node->render(node->color);
    if ( node->child ) { traverse( node->child ); }
    model_view = mvstack.pop();
    if ( node->sibling ) { traverse( node->sibling ); }

}

//----------------------------------------------------------------------------
//3D meshes        -----------------------------------------------------------
//----------------------------------------------------------------------------

Node *cube(float w, int c)
{
    Node *head = new Node[7];

    head->gar = garb;
    garb = head;

    mat4 m(1.0f);
    mat4 S=Scale(w,w,1.0);

    head[0] = Node(m,blank,NULL,&head[1],c);
    m = Translate(0.0f,0.0f,-w/2.0f)*RotateX(180)*S;
    head[1] = Node(m,rectangle,&head[2],NULL,c);
    m = Translate(0.0f,-w/2.0f,0.0f)*RotateX(90)*S;
    head[2] = Node(m,rectangle,&head[3],NULL,c);
    m = Translate(0.0f,w/2.0f,0.0f)*RotateX(-90)*S;
    head[3] = Node(m,rectangle,&head[4],NULL,c);
    m = Translate(0.0f,0.0f,w/2.0f)*S;
    head[4] = Node(m,rectangle,&head[5],NULL,c);
    m = Translate(w/2.0f,0.0f,0.0f)*RotateY(90)*S;
    head[5] = Node(m,rectangle,&head[6],NULL,c);
    m = Translate(-w/2.0f,0.0f,0.0f)*RotateY(-90)*S;
    head[6] = Node(m,rectangle,NULL,NULL,c);
    return head; 

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

//---------------------------------------------------------------------------------
//Camera init----------------------------------------------------------------------
//---------------------------------------------------------------------------------
vec4 v(0.0f,0.0f,1.0f,0.0f);
vec4 u(0.0f,1.0f,0.0f,0.0f);
vec4 cam_pos(10.0,0.0f,0.0f,1.0f);
vec4 n(-1.0f,0.0f,0.0f,0.0f);
mat4 mc = Frustum(-0.1024f, 0.1024f, -0.1024f, 0.1024f, 0.1f, 50.0f);
mat4 mv = LookAt(cam_pos,cam_pos+n,v);

//----------------------------------------------------------------------------
//VAO VBO GLSL init-----------------------------------------------------------
//----------------------------------------------------------------------------
int triind, recind;


void init( void )
{
    
    vector<vector<int> > z;

    objLoader obj("luffy.obj");
    

    vertex_init();
    // Create a vertex array object
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );

    glBufferData( GL_ARRAY_BUFFER, sizeof(vec4) * obj.sizeMesh() * 2,  NULL , GL_STATIC_DRAW );
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vec4)*obj.sizeMesh(), obj.Mesh() );
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vec4)*obj.sizeMesh() , sizeof(vec4)*obj.sizeMesh(), obj.MeshNorm());
    triind = obj.sizeMesh();
    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "./glsl/vshader83.glsl", "./glsl/fshader83.glsl" );
    glUseProgram( program );

    GLuint vPosition,vNormal;
    vPosition = glGetAttribLocation(program, "vPosition");
    vNormal = glGetAttribLocation(program,"vNormal");
    glEnableVertexAttribArray(vPosition);
    glEnableVertexAttribArray(vNormal);
 
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));
    glVertexAttribPointer( vNormal , 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(sizeof(vec4)*obj.sizeMesh()));

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
    glUniformMatrix4fv(projection_loc,1,GL_TRUE,mc);
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(diffuse_loc,1,normalize(vec4(0.8,0.8,0.8,1))); 
    glUniform4fv(ambient_loc,1,vec4(0.15,0.15,0.15,1)); 
    glUniform4fv(specular_loc,1,vec4(0.5,0.5,0.5,1)); 
   
    //head = cube(3.0f,9);
  
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
bool ckey[256] = {0};
void control()
{
    if(ckey['w'])
    {
        model_view *= RotateZ(3);
        //cam_pos = RotateZ(-3)*cam_pos;
    }
    if(ckey['a'])
    {
        model_view *= RotateY(3);
        //cam_pos = RotateY(-3)*cam_pos;
    }
    if(ckey['d'])
    {
        model_view *= RotateX(3);
        //cam_pos = RotateX(-3) * cam_pos;
    }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void display(void)
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ); 
    glBindBuffer(GL_ARRAY_BUFFER,buffer);
    
    
    control();
    
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view*Scale(0.05,0.05,0.05));

    glUniformMatrix4fv(camview_loc,1,GL_TRUE,mv);

    glUniform4fv(campos_loc,1, cam_pos);
    
    glDrawArrays(GL_TRIANGLES, 0, triind);    
//    traverse(head);

    glutSwapBuffers();
}

void nodes_clean(Node *head)
{
    Node *temp=NULL;
    while(head!=NULL)
    {
        temp = head;
        head = head->gar;
        delete [] temp;
    }


}
//----------------------------------------------------------------------------
//key board-------------------------------------------------------------------
//----------------------------------------------------------------------------

void keyboardup( unsigned char key, int x, int y )
{
    ckey[key] = 0;
}

void keyboard( unsigned char key, int x, int y )
{    
    switch( key ) 
    {
        case 033: // Escape Key
        case 'q': case 'Q':
            nodes_clean(garb);
            exit( EXIT_SUCCESS );
            break;
        case 'w':
            ckey[key]=1;
            break;
        case 'a':
            ckey[key]=1;
            break;
        case 'd':
            ckey[key]=1;
            break;
    }
}


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
int main( int argc, char **argv )
{
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH );
    glutInitWindowSize( 512, 512 );
    glutInitWindowPosition( 512, 112);
    glutCreateWindow( "part1" );
    glewInit();


    init();
    glutDisplayFunc( display );
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardup);
    glutIdleFunc(idle);
    glutMainLoop();
    return 0;
}
