#include "../include/mesh.h"
#include "../include/Angel.h"
#include <assert.h>
#define FORWARD_STEP 0.4
#define SCALE_ANGLE 0.05
typedef Angel::vec4 point4;
typedef Angel::vec4 color4;

const int NumVertices = 9; //(6 faces)(2 triangles/face)(3 vertices/triangle)
point4 points[NumVertices];
int index = 0;


//Stack----------------------------------------------------------------------------

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
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

struct Node {
    mat4  transform;
    void  (*render)( void );
    Node* sibling;
    Node* child;
    Node* gar;
    Node() :
        render(NULL), sibling(NULL), child(NULL), gar(NULL) {}

    Node( mat4& m, void (*render)( void ), Node* sibling, Node* child ) :
        transform(m), render(render), sibling(sibling), child(child), gar(NULL) {}
};

//----------------------------------------------------------------------------
//Draw method()---------------------------------------------------------------

Node* garb= NULL;
MatrixStack mvstack;
mat4 model_view=mat4(1.0f);
GLuint projection_loc, color_loc, model_view_loc;
//----------------------------------------------------------------------------

void rectangleY()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[2]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleR()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[1]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleM()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[0]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleB()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[10]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleDY()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[9]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}
void rectangleG()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[3]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}

void rectangleGray()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[8]);
    glDrawArrays( GL_TRIANGLES, 0 , 6 );
}
void triangle()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[1]);
    glDrawArrays( GL_TRIANGLES, 6 , 3 );
}
void triangleG()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[3]);
    glDrawArrays( GL_TRIANGLES, 6 , 3 );
}
void triangleGray()
{
    glUniformMatrix4fv(model_view_loc,1,GL_TRUE,model_view);
    glUniform4fv(color_loc,1,vertex_colors[8]);
    glDrawArrays( GL_TRIANGLES, 6 , 3 );
}
//----------------------------------------------------------------------------
//Node init()------------------------------------------------------------------
//----------------------------------------------------------------------------

void blank()
{};

Node* inithouseNode(int w, int h, int l)
{
    Node *head = new Node[11];

    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------

    mat4 m(1.0f);
    m = Scale(w,l,h);

    head[0] = Node(m, blank,NULL,&head[1]);
    m = Translate(1.5, -0.5, 0.0)*RotateY(-90.0);

    head[1] = Node( m, rectangleY, &head[2], NULL);
    m = Translate( 0.0, -0.5, 0.0)*RotateY(-90.0);

    head[2] = Node( m, rectangleY, &head[3], NULL);
    m = Translate(0.0, -0.5, 0.0)*RotateX(90.0)*Scale(1.5,1,1);

    head[3] = Node( m, rectangleY, &head[4], NULL);
    m = Translate(0.0, 0.5, 0.0)*RotateX(90.0)*Scale(1.5,1,1);

    head[4] = Node( m, rectangleY, &head[5], NULL);
    m = Translate(0.0, 0.0, 0.0);

    head[5] = Node( m, blank, NULL, &head[6]);
    m = Translate( 0.0, 0.0, 1.0)*RotateY(-90.0);

    head[6] = Node( m, triangle, &head[7], NULL);
    m = Translate( 1.5, 0.0, 1.0)*RotateY(-90.0);

    head[7] = Node( m, triangle, &head[8], NULL);
    m = Translate(0.0, -0.5, 1.0)*RotateX(45)*Scale(1.5, sqrt(0.5), 1.0);

    head[8] = Node( m, rectangleM, &head[9], NULL);
    m = Translate(0.0, 0.5, 1.0)*RotateX(135)*Scale(1.5, sqrt(0.5), 1.0);

    head[9] = Node( m, rectangleM, NULL, NULL);

    return &head[0];
}

Node* tree_trunk(int number, float radius, float height, mat4 m)
{

    Node *head = new Node[number+1];
    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------

    int t_index=0;
    float width = 2.0f * sin( M_PI/ number ) * radius ; // scaled width

    float step = 360.0f/float(number);
    mat4 rec_scal = Scale( height, width,  1.0f ) ; // scale matrix

    head[t_index] = Node( m , blank, NULL, &head[1]); //head

    for(int i=0; i< number-1 ;i++)
    {
        m = RotateZ(step*i)*Translate ( radius * cos(M_PI/number) , -width/2.0f , 0.0f )*RotateY(-90.0) * rec_scal;

        head[i+1] = Node( m, rectangleB, &head[i+2], NULL);
    }
    m = RotateZ(step*(number-1))*Translate ( radius * cos(M_PI/number) , -width/2 , 0.0f )*RotateY(-90.0) * rec_scal;

    head[number] = Node( m, rectangleB, NULL, NULL); // end
    return head;

}


Node* mount_top(int number, float radius, float height, mat4 m)
{

    Node *head = new Node[number+1];

    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------

    int t_index=0;
    float width = 2.0f * sin( M_PI/ number ) * radius ; // scaled width
    float step = 360.0f/float(number); 
    float shift = radius * cos(M_PI/number);  // triangle shift to the  position
    float len = 2* sqrt ( shift*shift + height * height ); // height of triangle
    float cita = atan(height/shift)/M_PI*180.0f;
    mat4 rec_scal = Scale( - len , width,  1.0f ) ; // scale matrix

    head[t_index] = Node( m , blank, NULL, &head[1]); //head

    for(int i=0; i< number-1 ;i++)
    {
        m = RotateZ(step*i)*Translate ( shift , 0.0f , 0.0f )* RotateY(cita) * rec_scal;

        head[i+1] = Node( m, triangleGray, &head[i+2], NULL);
    }
    m = RotateZ(step*(number-1))*Translate ( shift , 0.0f , 0.0f )*RotateY(cita) * rec_scal;

    head[number] = Node( m, triangleGray, NULL, NULL); // end
    return head;

}

Node* tree_top(int number, float radius, float height, mat4 m)
{

    Node *head = new Node[number+1];

    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------

    int t_index=0;
    float width = 2.0f * sin( M_PI/ number ) * radius ; // scaled width
    float step = 360.0f/float(number); 
    float shift = radius * cos(M_PI/number);  // triangle shift to the  position
    float len = 2* sqrt ( shift*shift + height * height ); // height of triangle
    float cita = atan(height/shift)/M_PI*180.0f;
    mat4 rec_scal = Scale( - len , width,  1.0f ) ; // scale matrix

    head[t_index] = Node( m , blank, NULL, &head[1]); //head

    for(int i=0; i< number-1 ;i++)
    {
        m = RotateZ(step*i)*Translate ( shift , 0.0f , 0.0f )* RotateY(cita) * rec_scal;

        head[i+1] = Node( m, triangleG, &head[i+2], NULL);
    }
    m = RotateZ(step*(number-1))*Translate ( shift , 0.0f , 0.0f )*RotateY(cita) * rec_scal;

    head[number] = Node( m, triangleG, NULL, NULL); // end
    return head;
}

Node* init_tree(float radius, float height, float fac1, float fac2) // fac1 percenatge of the top and trunck of the tree,  fac3 radius 
{


    Node *head = new Node[3];
    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    

    mat4 m(1.0f);
    head[0] = Node(m,blank,NULL,&head[1]);
    m = Translate(0,0,fac1*height);
    head[1] = Node(m,blank,&head[2], tree_top(4,fac2*radius,(1.0f-fac1)*height ,mat4(1.0)));
    m = Translate(0,0,0);
    head[2] = Node(m,blank,NULL, tree_trunk(50,radius,0.5f*height ,mat4(1.0)));
    return head; 
    // head->sibling = tree_top(90, 0.9, 3.0, Translate(0,0,1.5));

    // head->sibling->sibling = tree_trunk(90, 0.3, 3.0, Translate(0,0,0.0));


}



Node *nodes_ground()
{
    Node *head = new Node[1];
    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    
    mat4 m = Scale(100,100,0)*Translate(-0.5,-0.5,0);
    head[0] = Node( m , rectangleDY, NULL,NULL );
    return head;
}

Node *nodes_roads(int width)
{
    Node *head = new Node[2];
    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    
    mat4 m= Translate(3.0,0.0,0.01);
    head[0] = Node(m,blank, NULL, &head[1]);
    m = Scale(width,100,0)*Translate(-0.5,-0.5,0.0f);
    head[1] = Node( m , rectangleGray, NULL, NULL );
    return head;
}

Node *nodes_mount()
{


    Node *head = new Node[4];
    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    
    mat4 m= Translate(-33.0,-33.0,0.01);
    head[0] = Node(m,blank, NULL, &head[1]);
    m = Translate(-0.5,-0.5,0.0f);
    head[1] = Node( m , rectangleGray, &head[2], mount_top(5,20,10,mat4(1.0)) );
    m = Translate(-0.0,14.5,0.0f);
    head[2] = Node( m , rectangleGray, &head[3], mount_top(5,17,9,mat4(1.0)) );
    m = Translate(-0.0,24.5,0.0f);
    head[3] = Node( m , rectangleGray, NULL, mount_top(3,20,12,mat4(1.0)) );
    return head;
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//------ Organization of scencs -------------------------------------------------
//------- Houses and trees    -----------------------------------------------

Node* org_house_tree(float fac1)  // 5
{

    Node *head = new Node[6];

    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    
    mat4 m = Translate(-3.0,3.0,0)*RotateZ(0);
    head[0] = Node( m, blank , NULL, &head[1]);
    m = Translate(1.5,1.5,0);
    head[1] = Node( m , blank , &head[2], init_tree(0.3*fac1,6.0,0.2,4));
    m = Translate(-3.0,-3.0,0)* RotateZ(45);
    head[2] = Node( m , blank , &head[3], inithouseNode(2.3,2.1*fac1,1.2));
    m = Translate(-3.0,3.0,0)* RotateZ(95);
    head[3] = Node( m , blank , &head[4], init_tree(0.2,5.0,0.4,6*fac1));
    m = Translate(3.0,-3.0,0)* RotateZ(180);
    head[4] = Node( m , blank , &head[5], inithouseNode(2.3*fac1,2.1,3.2));
    m = Translate(-5.0,0.0,0)* RotateZ(0);
    head[5] = Node( m , blank , NULL, inithouseNode(1.3,5.0*fac1,1.2));

    return head; 

}


Node * org_scen()
{

    Node *head = new Node[6];

    //-------------
    //memory manage
    head->gar=garb;
    garb=head;
    //-------------    
    mat4 m = Translate(0.0,0.0,0)*RotateZ(45);
    head[0] = Node( m, blank , NULL, &head[1]);
    m = Translate(10,10,0);
    head[1] = Node( m , blank , &head[2], org_house_tree(1.0));
    m = Translate(10.0,-10.0,0)* RotateZ(35);
    head[2] = Node( m , blank , &head[3], org_house_tree(0.5));
    m = Translate(-10.0,10.0,0)* RotateZ(95);
    head[3] = Node( m , blank , &head[4], org_house_tree(1.3));
    m = Translate(-10.0,-10.0,0)* RotateZ(15);
    head[4] = Node( m , blank , &head[5], org_house_tree(0.8));
    m = Translate(0.0,0.0,0)* RotateZ(0);
    head[5] = Node( m , blank , NULL, org_house_tree(0.7));

    return head;


}


void nodes_clean(Node *head)
{
    Node *temp = NULL;
    while( head != NULL)
    {

        temp= head->gar;
        delete [] head;
        head = temp;
    }
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void traverse(Node *node)
{

    if ( node == NULL ) { return; }
    mvstack.push( model_view );
    model_view *= node->transform;
    node->render();
    if ( node->child ) { traverse( node->child ); }
    model_view = mvstack.pop();
    if ( node->sibling ) { traverse( node->sibling ); }

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void initbase()
{
    points[index] = point4(0.0f,0.0f,0.0f,1.0f); ++index;
    points[index] = point4(1.0f,0.0f,0.0f,1.0f); ++index;
    points[index] = point4(1.0f,1.0f,0.0f,1.0f); ++index;
    points[index] = point4(0.0f,0.0f,0.0f,1.0f); ++index;
    points[index] = point4(0.0f,1.0f,0.0f,1.0f); ++index;
    points[index] = point4(1.0f,1.0f,0.0f,1.0f); ++index;
    points[index] = point4(0.0f,-0.5f,0.0f,1.0f); ++index;
    points[index] = point4(0.0f,0.5f,0.0f,1.0f); ++index;
    points[index] = point4(0.5f,0.0f,0.0f,1.0f); ++index;
}

GLuint buffer;
Node  *head;
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//  Camera's init position
point4  eye( 5.5f, 0.0f, 3.0f , 1.0f );
point4  at( 0.0f,  0.0f, 3.0f, 1.0f );
vec4    up( 0.0f, 0.0f, 1.0f, 0.0f );
vec4 n =  eye - at;
vec4 u(0.0,1.0,0.0,0.0); 
mat4  mc = Frustum(-0.1024f, 0.1024f, -0.1024f, 0.1024f, 0.2f, 100.0f);
mat4  mv = LookAt( eye, eye-n  , up );


//----------------------------------------------------------------------------
//----------------------------------------------------------------------------



void init( void )
{


    initbase();
    n = normalize(n);
    u = normalize(u);
    up = normalize(up);

    // Create a vertex array object
    GLuint vao;
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // Create and initialize a buffer object
    glGenBuffers( 1, &buffer );
    glBindBuffer( GL_ARRAY_BUFFER, buffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW );

    // Load shaders and use the resulting shader program
    GLuint program = InitShader( "./glsl/vshader83.glsl", "./glsl/fshader83.glsl" );
    glUseProgram( program );

    GLuint vPosition;
    vPosition = glGetAttribLocation(program, "vPosition");
    glEnableVertexAttribArray(vPosition);
    glVertexAttribPointer( vPosition, 4, GL_FLOAT, GL_FALSE, 0 ,BUFFER_OFFSET(0));

    //----------------------------------------------------------------------------
    model_view_loc = glGetUniformLocation( program, "modelview");
    color_loc = glGetUniformLocation(program, "vColor");
    projection_loc = glGetUniformLocation(program, "projection");

    //----------------------------------------------------------------------------
    glUniformMatrix4fv(projection_loc,1,GL_TRUE,mc);
    //head = tree_trunk(600,0.2,1.0);
    //head = inithouseNode();

    head = nodes_ground();
    head->sibling =nodes_roads(4.0f); 
    head->sibling->sibling = org_scen();
    head->sibling->sibling->sibling = nodes_mount();


    glEnable( GL_DEPTH_TEST );
    //glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 1.0, 1.0, 1.0, 1.0 );

}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

void idle(void)
{
    glutPostRedisplay();
}

void display(void)
{

    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glBindBuffer(GL_ARRAY_BUFFER,buffer);

    mv = LookAt( eye, eye - n, up );
    model_view = mv;

    // for(int i= -5; i<=5; i++)
    //   for(int j= -5; j<=5; ++j)
    {
        // model_view = mv * Translate(i*3.0f,j*3.0f, 0.0f);
        traverse(head);
    }
    glutWarpPointer(256,256);
    glutSwapBuffers();
}

//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------
void SpecKey(int key, int x, int y)
{
    switch( key ) {
        case GLUT_KEY_UP: // MOVE FORWARD
            eye[0] -= FORWARD_STEP * n[0];
            eye[1] -= FORWARD_STEP * n[1];
            eye[2] -= FORWARD_STEP * n[2];
            break;
            /****** You must write the rest of teh code yourself
              397               case GLUT_KEY_DOWN: // MOVE BACKWARD
              398             //complete....
              399              *********/
        case GLUT_KEY_DOWN: // MOVE REVERSE
            eye[0] += FORWARD_STEP * n[0];
            eye[1] += FORWARD_STEP * n[1];
            eye[2] += FORWARD_STEP * n[2];
            break;
        case GLUT_KEY_LEFT: // MOVE REVERSE
            eye[0] += FORWARD_STEP * u[0];
            eye[1] += FORWARD_STEP * u[1];
            eye[2] += FORWARD_STEP * u[2];
            break;
        case GLUT_KEY_RIGHT: // MOVE REVERSE
            eye[0] -= FORWARD_STEP * u[0];
            eye[1] -= FORWARD_STEP * u[1];
            eye[2] -= FORWARD_STEP * u[2];

        default:
            break;
    }
    glutPostRedisplay();
}


void keyboard( unsigned char key, int x, int y )
{

    GLfloat xt, yt, zt;
    GLfloat cosine, sine;

    // positive or negative rotation depending on upper or lower case letter
    if(key > 96)
    {
        cosine = cos(SCALE_ANGLE * M_PI/-180.0);
        sine = sin(SCALE_ANGLE * M_PI/-180.0);
    }
    else
    {
        cosine = cos(SCALE_ANGLE * M_PI/180.0);
        sine = sin(SCALE_ANGLE * M_PI/180.0);
    }


    switch( key ) {

        case 033: // Escape Key
        case 'q': case 'Q':
            nodes_clean(garb);
            exit( EXIT_SUCCESS );
            break;
        case 'Z': // roll counterclockwise in the xy plane
        case 'z': // roll clockwise in the xy plane
            xt = u[0];
            yt = u[1];
            zt = u[2];
            u[0] = xt*cosine - up[0]*sine;
            u[1] = yt*cosine - up[1]*sine;
            u[2] = zt*cosine - up[2]*sine;
            up[0] = xt*sine + up[0]*cosine;
            up[1] = yt*sine + up[1]*cosine;
            up[2] = zt*sine + up[2]*cosine;
            n = normalize(n);
            u = normalize(u);
            up = normalize(up);
            break;

        case 'X': // roll counterclockwise in the xy plane
        case 'x': // roll clockwise in the xy plane

            up = rotateN( u ,SCALE_ANGLE*M_PI/180.0f)*up;
            n = rotateN( u ,SCALE_ANGLE*M_PI/180.0f)*n;

            n = normalize(n);
            u = normalize(u);
            up = normalize(up);
            break;

        case 'C': // roll counterclockwise in the xy plane
        case 'c': // roll clockwise in the xy plane

            u = rotateN( vec4(0,0,1,0) ,SCALE_ANGLE*M_PI/180.0f)*u;
            n = rotateN( vec4(0,0,1,0) ,SCALE_ANGLE*M_PI/180.0f)*n;
            up = rotateN( vec4(0,0,1,0) ,SCALE_ANGLE*M_PI/180.0f)*up;

            n = normalize(n);
            u = normalize(u);
            up = normalize(up);
            break;
    }
    glutPostRedisplay();
}

void mousemoving(int dx, int dy)
{
    if(dy>1)
    {
        up = rotateN( u ,dy*SCALE_ANGLE*M_PI/180.0f)*up;
        n = rotateN( u ,dy*SCALE_ANGLE*M_PI/180.0f)*n;

        n = normalize(n);
        u = normalize(u);
        up = normalize(up);
    }
    if(dy<-1)
    {
        up = rotateN( u ,dy*SCALE_ANGLE*M_PI/180.0f)*up;
        n = rotateN( u ,dy*SCALE_ANGLE*M_PI/180.0f)*n;

        n = normalize(n);
        u = normalize(u);
        up = normalize(up);
    }

    if(dx>1)
    {
        u = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*u;
        n = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*n;
        up = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*up;

        n = normalize(n);
        u = normalize(u);
        up = normalize(up);
    }
    if(dx<-1)
    {
        u = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*u;
        n = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*n;
        up = rotateN( vec4(0,0,1,0) ,dx*SCALE_ANGLE*M_PI/180.0f)*up;

        n = normalize(n);
        u = normalize(u);
        up = normalize(up);
    }
}




void mouseMotion(int x, int y)
{
    mousemoving(x-256 , y-256 );    
    std::cout<<x-256<<" "<<y-256<<std::endl;
    glutPostRedisplay();
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
    glutWarpPointer(257,256);


    init();
    glutDisplayFunc( display );
    glutKeyboardFunc( keyboard );
    glutPassiveMotionFunc(mouseMotion);
    glutSpecialFunc( SpecKey );
    glutIdleFunc(idle);

    glutMainLoop();
    return 0;
}
